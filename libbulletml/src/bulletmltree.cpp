#include "bulletmltree.h"
#include "calc.h"
#include "bulletmlerror.h"

#include "auto_ptr_fix.h"

#include <iostream>
#include <algorithm>
#include <functional>

std::string BulletMLNode::name2string[nameSize] = {
	"bullet", "action", "fire", "changeDirection", "changeSpeed", "accel",
	"wait", "repeat", "bulletRef", "actionRef", "fireRef", "vanish",
	"horizontal", "vertical", "term", "times", "direction", "speed", "param",
	"bulletml"
};

BulletMLNode::Type BulletMLNode::string2type(const std::string& str) {
	if (str == "aim") return aim;
	else if (str == "absolute") return absolute;
	else if (str == "relative") return relative;
	else if (str == "sequence") return sequence;
	else BulletMLError::doAssert(
		std::string("BulletML parser: unknown type ") + str + ".");

	return typeSize;    // not reach
}

BulletMLNode::Name BulletMLNode::string2name(const std::string& str) {
	if (str == "bulletml") return bulletml;
	else if (str == "bullet") return bullet;
	else if (str == "action") return action;
	else if (str == "fire") return fire;
	else if (str == "changeDirection") return changeDirection;
	else if (str == "changeSpeed") return changeSpeed;
	else if (str == "accel") return accel;
	else if (str == "vanish") return vanish;
	else if (str == "wait") return wait;
	else if (str == "repeat") return repeat;
	else if (str == "direction") return direction;
	else if (str == "speed") return speed;
	else if (str == "horizontal") return horizontal;
	else if (str == "vertical") return vertical;
	else if (str == "term") return term;
	else if (str == "bulletRef") return bulletRef;
	else if (str == "actionRef") return actionRef;
	else if (str == "fireRef") return fireRef;
	else if (str == "param") return param;
	else if (str == "times") return times;
	else BulletMLError::doAssert(
		std::string("BulletML parser: unknown tag ") + str + ".");

	return nameSize;    // not reach
}

BulletMLNode::BulletMLNode(const std::string& name)
	: name_(string2name(name)), type_(none) {
    setReleaseDuty(true);
}

BulletMLNode::~BulletMLNode() {}

void BulletMLNode::setValue(const std::string& val) {
	auto_ptr_copy(val_, calc(val));
}

void BulletMLNode::dump() {
#if 0
    std::cout << "<" << name2string[name_];
/*
    AttributeMap::const_iterator ite;
    for (ite = attributes_.begin(); ite != attributes_.end(); ite++) {
	std::cout << " " << ite->first << "=" << ite->second;
    }
*/
    std::cout << ">" << std::endl;
/*
    if (val_ != "") std::cout << val_ << std::endl;
*/
    std::for_each(childBegin(), childEnd(), std::mem_fun(&BulletMLNode::dump));

    std::cout << "</" << name2string[name_] << ">" << std::endl;
#endif
}

BulletMLNode* BulletMLNode::getChild(Name name) {
    ChildIterator ite;
    for (ite = childBegin(); ite != childEnd(); ite++) {
		if ((*ite)->getName() == name) return *ite;
    }
    return 0;
}

bool BulletMLNode::findNode(Name name) const {
	if (getName() == name) return true;
    ConstChildIterator ite;
    for (ite = childBegin(); ite != childEnd(); ite++) {
		if ((*ite)->findNode(name)) return true;
    }
    return false;
}

BulletMLNode* BulletMLNode::next() {
    BulletMLNode* parent = getParent();
    if (parent == 0) return 0;
    ChildIterator ite =
		std::find(parent->childBegin(), parent->childEnd(), this);
    BulletMLError::doAssert(ite != parent->childEnd(), name_ + ": not found");
    ite++;
    if (ite == parent->childEnd()) return 0;
    else return *ite;
}

void BulletMLNode::getAllChildrenVec(Name name, std::vector<BulletMLNode*>& outvec) {
    ChildIterator ite;
    for (ite = childBegin(); ite != childEnd(); ite++) {
		if ((*ite)->getName() == name) outvec.push_back(*ite);
	}
}

