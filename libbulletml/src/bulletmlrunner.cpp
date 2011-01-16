#include "bulletmlrunner.h"
#include "bulletmlparser.h"
#include "bulletmlerror.h"
#include "formula-variables.h"
#include "bulletmlrunnerimpl.h"

BulletMLRunner::BulletMLRunner(BulletMLParser* bulletml) {
	const std::vector<BulletMLNode*>& acts = bulletml->getTopActions();
	for (size_t i = 0; i < acts.size(); i++) {
		std::vector<BulletMLNode*> act;
		act.push_back(acts[i]);
		BulletMLState* state =
			new BulletMLState(bulletml, act,
							  boost::shared_ptr<BulletMLParameter>());
		impl_.push_back(makeImpl(state));
	}
}

BulletMLRunner::BulletMLRunner(BulletMLState* state) {
	impl_.push_back(makeImpl(state));
}

BulletMLRunner::~BulletMLRunner() {
	for (size_t i = 0; i < impl_.size(); i++) {
		delete impl_[i];
	}
}

void BulletMLRunner::run() {
/*
	std::for_each(impl_.begin(), impl_.end(),
				  std::mem_fun(&BulletMLRunnerImpl::run));
*/
	for (std::vector<BulletMLRunnerImpl*>::const_iterator ite = impl_.begin();
		ite != impl_.end(); ++ite)
	{
			(*ite)->run();
	}
}

bool BulletMLRunner::isEnd() const {
/*
	return
		std::find_if(impl_.begin(), impl_.end(),
					 std::not1(std::mem_fun(&BulletMLRunnerImpl::isEnd)))
		== impl_.end();
*/
	for (std::vector<BulletMLRunnerImpl*>::const_iterator ite = impl_.begin();
		ite != impl_.end(); ++ite)
	{
		if ((*ite)->isEnd()) return true;
	}
	return false;
}

BulletMLRunnerImpl* BulletMLRunner::makeImpl(BulletMLState* state) {
	return new BulletMLRunnerImpl(state, this);
}
