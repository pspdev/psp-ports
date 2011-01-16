/// BulletMLツリー。
/**
 * BulletML に特化していて非常にいんちきくさいのが特徴。
 */

#ifndef BULLETMLTREE_H_
#define BULLETMLTREE_H_

#include <string>
#include <algorithm>
#include <memory>

#include <vector>

#include "tree.h"
#include "formula.h"
#include "bulletmlcommon.h"

class BulletMLNode : public TreeNode<BulletMLNode> {
public:
	typedef Formula<float> Number;
	typedef enum { none, aim, absolute, relative, sequence, typeSize } Type;
	typedef enum { bullet, action, fire, changeDirection, changeSpeed, accel,
				   wait, repeat, bulletRef, actionRef, fireRef, vanish,
				   horizontal, vertical, term, times, direction, speed, param,
				   bulletml, nameSize } Name;

private:
	static Type string2type(const std::string& str);
	static Name string2name(const std::string& str);
	static std::string name2string[nameSize];

public:
    typedef TreeNode<BulletMLNode>::Children Children;
    typedef TreeNode<BulletMLNode>::ChildIterator ChildIterator;

public:
	DECLSPEC explicit BulletMLNode(const std::string& name);
    DECLSPEC virtual ~BulletMLNode();

    DECLSPEC Name getName() const { return name_; }

    DECLSPEC void setValue(const std::string& val);
    DECLSPEC float getValue() const { return val_->value(); }

	DECLSPEC void setType(const std::string& type) { type_ = string2type(type); }
	DECLSPEC Type getType() const { return type_; }

	DECLSPEC void setRefID(int id) { refID_ = id; }
	DECLSPEC int getRefID() const { return refID_; }

    DECLSPEC BulletMLNode* getChild(Name name);
/*

    template <class OutIte_>
    void getAllChildren(Name name, OutIte_ outIte);
*/
    DECLSPEC void getAllChildrenVec(Name name, std::vector<BulletMLNode*>& outvec);



    /// 子孫の中に指定した名前に一致するものがあるかどうか
    DECLSPEC bool findNode(Name name) const;

    DECLSPEC BulletMLNode* next();

    virtual void dump();

protected:
    Name name_;
	Type type_;
	int refID_;
    std::auto_ptr<Number> val_;
};


/*
template <class OutIte_>
void BulletMLNode::getAllChildren(Name name, OutIte_ outIte) {
    ChildIterator ite;
    for (ite = childBegin(); ite != childEnd(); ite++) {
		if ((*ite)->getName() == name) *outIte = *ite;
		outIte++;
    }
}

*/

#endif // ! BULLETMLTREE_H_
