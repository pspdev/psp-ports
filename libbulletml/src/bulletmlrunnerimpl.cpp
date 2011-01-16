#include "bulletmlrunnerimpl.h"
#include "bulletmlrunner.h"
#include "bulletmlparser.h"
#include "bulletmlerror.h"
#include "formula-variables.h"

#include "auto_ptr_fix.h"

#include <cassert>
#include <cmath>

BulletMLRunnerImpl::Method
BulletMLRunnerImpl::commandMap_[BulletMLNode::nameSize] = {
	&BulletMLRunnerImpl::runBullet,
	&BulletMLRunnerImpl::runAction,
	&BulletMLRunnerImpl::runFire,
	&BulletMLRunnerImpl::runChangeDirection,
	&BulletMLRunnerImpl::runChangeSpeed,
	&BulletMLRunnerImpl::runAccel,
	&BulletMLRunnerImpl::runWait,
	&BulletMLRunnerImpl::runRepeat,
	&BulletMLRunnerImpl::runBulletRef,
	&BulletMLRunnerImpl::runActionRef,
	&BulletMLRunnerImpl::runFireRef,
	&BulletMLRunnerImpl::runVanish
};

float BulletMLRunnerImpl::getDirection(BulletMLNode* dirNode,
                                        bool prevChange) {
    float dir;

    bool isDefault = true;
    dir = getNumberContents(dirNode);

    if (dirNode->getType() != BulletMLNode::none) {
		BulletMLNode::Type type = dirNode->getType();
		isDefault = false;

		if (type == BulletMLNode::absolute) {
			if (bulletml_->isHorizontal()) {
				dir -= 90;
			}
		}
		else if (type == BulletMLNode::relative) {
			dir += runner_->getBulletDirection();
		}
		else if (type == BulletMLNode::sequence) {
			if (!prevDir_.isValidate()) {
				dir = 0;
				isDefault = true;
			}
			else dir += prevDir_;
		}
		else {
			isDefault = true;
		}
    }

    if (isDefault) {
		// 明らかに非効率
		dir += runner_->getAimDirection();
    }

    while (dir > 360) dir -= 360;
    while (dir < 0) dir += 360;

    if (prevChange) prevDir_ = dir;

    return dir;
}

float BulletMLRunnerImpl::getSpeed(BulletMLNode* spdNode) {
    float spd;

    spd = getNumberContents(spdNode);
    if (spdNode->getType() != BulletMLNode::none) {
		BulletMLNode::Type type = spdNode->getType();

		if (type == BulletMLNode::relative) {
			spd += runner_->getBulletSpeed();
		}
		else if (type == BulletMLNode::sequence) {
			if (!prevSpd_.isValidate()) spd = 1;
			else spd += prevSpd_;
		}
    }

    // デフォルトでは何もしない

    prevSpd_ = spd;

    return spd;
}

BulletMLRunnerImpl::BulletMLRunnerImpl(BulletMLState* state,
									   BulletMLRunner* runner)
	: bulletml_(state->getBulletML()),
      node_(state->getNode().begin(), state->getNode().end()),
      actIte_(0), end_(false), runner_(runner)
{
	if (state->getParameter().get() != 0) {
		parameters_ = state->getParameter();
	}

	delete state;


	for (std::vector<BulletMLNode*>::iterator ite = node_.begin(); ite != node_.end(); ++ite) {

		(*ite)->setParent(0);

	}

/*
	std::for_each(node_.begin(), node_.end(),
				  std::bind2nd(std::mem_fun(&BulletMLNode::setParent), 0));
*/
	actTurn_ = -1;
	act_ = node_[0];
}

BulletMLRunnerImpl::~BulletMLRunnerImpl() {
	while (!repeatStack_.empty()) {
		delete repeatStack_.top();
		repeatStack_.pop();
	}
}

float BulletMLRunnerImpl::getNumberContents(const BulletMLNode* node) {
    assert(node);

	Variables::rank = runner_->getRank();
	Variables::parameters = parameters_.get();
	Variables::runner = runner_;
	return node->getValue();
}

void BulletMLRunnerImpl::setSpeed() {
    BulletMLNode* spd = act_->getChild(BulletMLNode::speed);
    if (spd == 0) return;

    spd_ = getSpeed(spd);
}

void BulletMLRunnerImpl::setDirection() {
    BulletMLNode* dir = act_->getChild(BulletMLNode::direction);
    if (dir == 0) return;

    dir_ = getDirection(dir);
}

bool BulletMLRunnerImpl::isTurnEnd() {
	return isEnd() || actTurn_ > endTurn_;
}

void BulletMLRunnerImpl::doWait(int frame) {
	if (frame <= 0) return;
	actTurn_ += frame;
}

void BulletMLRunnerImpl::changes() {
	int now = runner_->getTurn();

    if (changeDir_.get() != 0) {
		if (changeDir_->isLast(now)) {
			runner_->doChangeDirection(changeDir_->getLast());
			delete auto_ptr_release(changeDir_);
		}
		else {
			runner_->doChangeDirection(changeDir_->getValue(now));
		}
    }

    if (changeSpeed_.get() != 0) {
		if (changeSpeed_->isLast(now)) {
			runner_->doChangeSpeed(changeSpeed_->getLast());
			delete auto_ptr_release(changeSpeed_);
		}
		else {
			runner_->doChangeSpeed(changeSpeed_->getValue(now));
		}
    }

    if (accelx_.get() != 0) {
		if (accelx_->isLast(now)) {
			runner_->doAccelX(accelx_->getLast());
			delete auto_ptr_release(accelx_);
		}
		else {
			runner_->doAccelX(accelx_->getValue(now));
		}
    }

    if (accely_.get() != 0) {
		if (accely_->isLast(now)) {
			runner_->doAccelY(accely_->getLast());
			delete auto_ptr_release(accely_);
		}
		else {
			runner_->doAccelY(accely_->getValue(now));
		}
    }
}

void BulletMLRunnerImpl::runSub() {
	// 見たくもないコードだね。
	while (act_ != 0 && !isTurnEnd()) {
		BulletMLNode* prev = act_;
		Method fp = commandMap_[act_->getName()];
		(this->*fp)();

		// ref から戻る
		if (act_ == 0 &&
			prev->getParent() != 0 &&
			prev->getParent()->getName() == BulletMLNode::bulletml)
		{
			assert(!refStack_.empty());
			prev = refStack_.top().first;
			parameters_ = refStack_.top().second;
			refStack_.pop();
		}

		// 次の node を探す
		if (act_ == 0) act_ = prev->next();

		// 上に遡って次の node を探す
		while (act_ == 0) {
			if (prev->getParent() != 0 &&
				prev->getParent()->getName() == BulletMLNode::repeat)
			{
				RepeatElem* rep = repeatStack_.top();
				rep->ite++;
				if (rep->ite < rep->end) {
					act_ = rep->act;
					break;
				}
				else {
					delete rep;
					repeatStack_.pop();
				}
			}

			act_ = prev->getParent();
			if (act_ == 0) break;

			prev = act_;

			if (prev->getParent() != 0 &&
				prev->getParent()->getName() == BulletMLNode::bulletml)
			{
				assert(!refStack_.empty());
				prev = act_ = refStack_.top().first;
				parameters_ = refStack_.top().second;
				refStack_.pop();
			}

			act_ = act_->next();
		}
	}
}

void BulletMLRunnerImpl::run() {
	if (isEnd()) return;

	changes();

	endTurn_ = runner_->getTurn();

	// 最後の wait, change系を待つだけ
	if (act_ == 0) {
		if (!isTurnEnd()) {
			if (changeDir_.get() == 0 && changeSpeed_.get() == 0 &&
				accelx_.get() == 0 && accely_.get() == 0)
			{
				end_ = true;
			}
		}
		return;
	}

	act_ = node_[actIte_];
	if (actTurn_ == -1) actTurn_ = runner_->getTurn();

	runSub();

	if (act_ == 0) {
		actIte_++;
		if (node_.size() != actIte_) act_ = node_[actIte_];
	}
	else {
		node_[actIte_] = act_;
	}
}

void BulletMLRunnerImpl::runBullet() {
	setSpeed();
	setDirection();
	if (!spd_.isValidate()) {
		prevSpd_ = spd_ = runner_->getDefaultSpeed();
	}
	if (!dir_.isValidate()) {
		prevDir_ = dir_ = runner_->getAimDirection();
	}

	if (act_->getChild(BulletMLNode::action) == 0 &&
		act_->getChild(BulletMLNode::actionRef) == 0) {
		runner_->createSimpleBullet(dir_, spd_);
	}
	else {
		std::vector<BulletMLNode*> acts;
		act_->getAllChildrenVec(BulletMLNode::action, acts);

		act_->getAllChildrenVec(BulletMLNode::actionRef, acts);

/*

		act_->getAllChildren(BulletMLNode::action, std::back_inserter(acts));

		act_->getAllChildren(BulletMLNode::actionRef,

							 std::back_inserter(acts));

*/

		BulletMLState* state = new BulletMLState(bulletml_, acts, parameters_);

		runner_->createBullet(state, dir_, spd_);
	}

	act_ = 0;
}

void BulletMLRunnerImpl::runFire() {
	shotInit();

	setSpeed();
	setDirection();

	BulletMLNode* bullet = act_->getChild(BulletMLNode::bullet);
	if (bullet == 0) bullet = act_->getChild(BulletMLNode::bulletRef);
	BulletMLError::doAssert(bullet != 0,
							"<fire> must have contents bullet or bulletRef");

	act_ = bullet;
}

void BulletMLRunnerImpl::runAction() {
	if (act_->childSize() == 0) act_ = 0;
	else act_ = *act_->childBegin();
}

void BulletMLRunnerImpl::runWait() {
	int frame = static_cast<int>(getNumberContents(act_));
	doWait(frame);

	act_ = 0;
}

void BulletMLRunnerImpl::runRepeat() {
	const BulletMLNode* times = act_->getChild(BulletMLNode::times);
	if (times == 0) return;

	int timesNum = static_cast<int>(getNumberContents(times));

	BulletMLNode* action = act_->getChild(BulletMLNode::action);
	if (action == 0) action = act_->getChild(BulletMLNode::actionRef);
	BulletMLError::doAssert(
		action != 0,
		"repeat elem must have contents action or actionRef");

	repeatStack_.push(new RepeatElem(0, timesNum, action));

	act_ = action;
}

void BulletMLRunnerImpl::runFireRef() {
	boost::shared_ptr<Parameters> prevPara = parameters_;
	parameters_.reset(getParameters());

	refStack_.push(std::make_pair(act_, prevPara));
	act_ = bulletml_->getFireRef(act_->getRefID());
}

void BulletMLRunnerImpl::runActionRef() {
	boost::shared_ptr<Parameters> prevPara = parameters_;
	parameters_.reset(getParameters());

	refStack_.push(std::make_pair(act_, prevPara));
	act_ = bulletml_->getActionRef(act_->getRefID());
}

void BulletMLRunnerImpl::runBulletRef() {
	boost::shared_ptr<Parameters> prevPara = parameters_;
	parameters_.reset(getParameters());

	refStack_.push(std::make_pair(act_, prevPara));
	act_ = bulletml_->getBulletRef(act_->getRefID());
}

void BulletMLRunnerImpl::runChangeDirection() {
	int term = static_cast<int>(getNumberContents(
									act_->getChild(BulletMLNode::term)));
	BulletMLNode* dirNode = act_->getChild(BulletMLNode::direction);
	BulletMLNode::Type type = dirNode->getType();

	float dir;
	if (type != BulletMLNode::sequence) dir = getDirection(dirNode, false);
	else dir = getNumberContents(dirNode);

	calcChangeDirection(dir, term, type == BulletMLNode::sequence);

	act_ = 0;
}

void BulletMLRunnerImpl::runChangeSpeed() {
	int term = static_cast<int>(getNumberContents(
									act_->getChild(BulletMLNode::term)));
	BulletMLNode* spdNode = act_->getChild(BulletMLNode::speed);
	BulletMLNode::Type type = spdNode->getType();

	float spd;
	if (type != BulletMLNode::sequence) spd = getSpeed(spdNode);
	else {
		spd = getNumberContents(spdNode) * (float)term
			+ runner_->getBulletSpeed();
	}
	

	calcChangeSpeed(spd, term);

	act_ = 0;
}

void BulletMLRunnerImpl::runAccel() {
	int term = static_cast<int>(getNumberContents(
									act_->getChild(BulletMLNode::term)));
	BulletMLNode* hnode = act_->getChild(BulletMLNode::horizontal);
	BulletMLNode* vnode = act_->getChild(BulletMLNode::vertical);

	if (bulletml_->isHorizontal()) {
		if (vnode != 0) calcAccelX(getNumberContents(vnode), term,
								   vnode->getType());
		if (hnode != 0) calcAccelY(-getNumberContents(hnode), term,
								   hnode->getType());
	}
	else {
		if (hnode != 0) calcAccelX(getNumberContents(hnode), term,
								   hnode->getType());
		if (vnode != 0) calcAccelY(getNumberContents(vnode), term,
								   vnode->getType());
	}

	act_ = 0;
}

void BulletMLRunnerImpl::calcChangeDirection(float direction, int term,
											 bool seq)
{
	int finalTurn = actTurn_ + term;

	float dirFirst = runner_->getBulletDirection();

	if (seq) {
		auto_ptr_copy(changeDir_, new LinearFunc<int, float>
					  (actTurn_, finalTurn,
					   dirFirst, dirFirst + direction * term));
	}
	else {
		float dirSpace;

		// ちゃんと近い方を回っていくのは結構難しいね
		float dirSpace1 = direction - dirFirst;
		float dirSpace2;
		if (dirSpace1 > 0) dirSpace2 = dirSpace1 - 360;
		else dirSpace2 = dirSpace1 + 360;
		if (fabsf(dirSpace1) < fabsf(dirSpace2)) dirSpace = dirSpace1;
		else dirSpace = dirSpace2;

		auto_ptr_copy(changeDir_, new LinearFunc<int, float>
					  (actTurn_, finalTurn, dirFirst, dirFirst + dirSpace));
	}
}

void BulletMLRunnerImpl::calcChangeSpeed(float speed, int term) {
	int finalTurn = actTurn_ + term;

	float spdFirst = runner_->getBulletSpeed();

	auto_ptr_copy(changeSpeed_, new LinearFunc<int, float>
				  (actTurn_, finalTurn, spdFirst, speed));
}

void BulletMLRunnerImpl::calcAccelY(float horizontal, int term,
									BulletMLNode::Type type)
{
	int finalTurn = actTurn_ + term;

	float firstSpd = runner_->getBulletSpeedY();
	float finalSpd;

	if (type == BulletMLNode::sequence) {
		finalSpd = firstSpd + horizontal * term;
	}
	else if (type == BulletMLNode::relative) {
		finalSpd = firstSpd + horizontal;
	}
	else {
		finalSpd = horizontal;
	}

	auto_ptr_copy(accely_, new LinearFunc<int, float>
				  (actTurn_, finalTurn, firstSpd, finalSpd));
}

void BulletMLRunnerImpl::calcAccelX(float vertical, int term,
									BulletMLNode::Type type)
{
	int finalTurn = actTurn_ + term;

	float firstSpd = runner_->getBulletSpeedX();
	float finalSpd;

	if (type == BulletMLNode::sequence) {
		finalSpd = firstSpd + vertical * term;
	}
	else if (type == BulletMLNode::relative) {
		finalSpd = firstSpd + vertical;
	}
	else {
		finalSpd = vertical;
	}

	auto_ptr_copy(accelx_ ,new LinearFunc<int, float>
				  (actTurn_, finalTurn, firstSpd, finalSpd));
}

void BulletMLRunnerImpl::runVanish() {
	runner_->doVanish();

	act_ = 0;
}

BulletMLRunnerImpl::Parameters* BulletMLRunnerImpl::getParameters() {
	Parameters* para = 0;
	bool first = true;

	BulletMLNode::ChildIterator ite;
	for (ite = act_->childBegin(); ite != act_->childEnd(); ite++) {
		BulletMLNode* node = *ite;
		if (node->getName() != BulletMLNode::param) continue;

		if (first) {
			first = false;
			para = new Parameters;
			// 0番要素は使わない
			para->push_back(0);
		}

		para->push_back(getNumberContents(node));
	}

	return para;
}

	
