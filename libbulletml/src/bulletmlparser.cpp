#include "bulletmlparser.h"
#include "bulletmlerror.h"

#include <map>

namespace {

    /// 内部のみで使用するクラス。
	class IDPool {
	public:
		static int getID(BulletMLNode::Name domain, const std::string& key) {
			KeyToID& kti = map_[domain];
			KeyToID::iterator ite = kti.find(key);
			if (ite == kti.end()) {
				int id = maxMap_[domain]++;
				map_[domain][key] = id;
				return id;
			}
			else {
				return ite->second;
			}
		}
		static void init() {
			map_.insert(std::make_pair(BulletMLNode::bullet, KeyToID()));
			map_.insert(std::make_pair(BulletMLNode::action, KeyToID()));
			map_.insert(std::make_pair(BulletMLNode::fire, KeyToID()));
			maxMap_.insert(std::make_pair(BulletMLNode::bullet, 0));
			maxMap_.insert(std::make_pair(BulletMLNode::action, 0));
			maxMap_.insert(std::make_pair(BulletMLNode::fire, 0));
		}
		static void quit() {
			map_.clear();
			maxMap_.clear();
		}

	private:
		typedef std::map<std::string, unsigned int> KeyToID;
		typedef std::map<BulletMLNode::Name, KeyToID> DomainToIDMap;
		typedef std::map<BulletMLNode::Name, int> DomainToMaxID;
		static DomainToIDMap map_;
		static DomainToMaxID maxMap_;

	};
}

IDPool::DomainToIDMap IDPool::map_;
IDPool::DomainToMaxID IDPool::maxMap_;

BulletMLParser::BulletMLParser()
	: bulletml_(0), isHorizontal_(false)
{}

BulletMLParser::~BulletMLParser() {
	// BulletMLNode はルートノードだけ破壊すれば良い
	delete bulletml_;
}

void BulletMLParser::build() {
	IDPool::init();
	parse();
	IDPool::quit();
}

BulletMLNode* BulletMLParser::getBulletRef(int id) {
	BulletMLError::doAssert((int)bulletMap_.size() > id && bulletMap_[id] != 0,
		   "bulletRef key doesn't exist.");
	return bulletMap_[id];
}

BulletMLNode* BulletMLParser::getActionRef(int id) {
	BulletMLError::doAssert((int)actionMap_.size() > id && actionMap_[id] != 0,
		   "actionRef key doesn't exist.");
	return actionMap_[id];
}

BulletMLNode* BulletMLParser::getFireRef(int id) {
	BulletMLError::doAssert((int)fireMap_.size() > id && fireMap_[id] != 0,
		   "fireRef key doesn't exist.");
	return fireMap_[id];
}

BulletMLNode* BulletMLParser::addContent(const std::string& name) {
	// ルートノードは別処理
	if (name == "bulletml") {
		bulletml_ = new BulletMLNode(name);
		return bulletml_;
	}
	BulletMLError::doAssert(bulletml_ != 0, "<bulletml> doesn't come.");

	return new BulletMLNode(name);
}

void BulletMLParser::addAttribute(const MyAttributes& attr,
								  BulletMLNode* elem)
{
	if (!attr.empty()) {
		MyAttributeIte ite = attr.begin();
		while (ite != attr.end()) {
			const std::string key(*ite);
			ite++;
			const std::string val(*ite);
			ite++;
			if (key == "type") elem->setType(val);
			else if (key == "label") {
				BulletMLNode::Name name = elem->getName();
				BulletMLNode::Name domain;
				if (name == BulletMLNode::bulletRef) {
					domain = BulletMLNode::bullet;
				}
				else if (name == BulletMLNode::actionRef) {
					domain = BulletMLNode::action;
				}
				else if (name == BulletMLNode::fireRef) {
					domain = BulletMLNode::fire;
				}
				else {
					domain = name;
				}

				int id = IDPool::getID(domain, val);
				if (name == BulletMLNode::bullet) {
					if ((int)bulletMap_.size() <= id){
						bulletMap_.resize(id+1, 0);}
					bulletMap_[id] = elem;
				}
				else if (name == BulletMLNode::action) {
					if ((int)actionMap_.size() <= id){
						actionMap_.resize(id+1, 0);
					}
					actionMap_[id] = elem;
				}
				else if (name == BulletMLNode::fire) {
					if ((int)fireMap_.size() <= id)
						fireMap_.resize(id+1, 0);
					fireMap_[id] = elem;
				}
				else if (
					name == BulletMLNode::bulletRef ||
					name == BulletMLNode::actionRef ||
					name == BulletMLNode::fireRef)
				{
					elem->setRefID(id);
				}
				else {
					BulletMLError::doAssert(
						"he can't have attribute \"label\".");
				}

				if (elem->getName() == BulletMLNode::action &&
					val.length() >= 3 && val.substr(0, 3) == "top") {
					topActions_.push_back(elem);
				}
			}
		}
	}
}

	
