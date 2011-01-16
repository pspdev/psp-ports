/// 一般的なツリー。再生産だね

#ifndef TREE_H_
#define TREE_H_

#include "bulletmlcommon.h"

#include <list>

/// ツリーのクラス
/**
 * ツリーってのはコンテナが存在しない集合体であると思う。
 * んで、ノードっていう属性を帯びたクラスの集合がコンテナであると。
 * でインターフェイスは、
 * class YourNode : public TreeNode<YourNode>;
 * って具合い。
 * ポインタ管理を前提としている。
 * インスタンスの管理は普段はしないけど、
 * setReleaseDuty を呼ばれたノードが破壊されると、
 * それの息子以下の世代は全て破壊される。
 */
template <class C_>
class TreeNode {
public:
    // これをテンプレート引数で差し換えうる設計にしたいのだが
    typedef std::list<C_*> Children;
    typedef typename Children::iterator ChildIterator;
    typedef typename Children::const_iterator ConstChildIterator;

public:
    DECLSPEC TreeNode() {
		releaseDuty_ = false;
    }
    DECLSPEC virtual ~TreeNode();

    DECLSPEC void addChild(C_* c) {
		c->setParent(dynamic_cast<C_*>(this));
		children_.push_back(c);
    }
    DECLSPEC void setReleaseDuty(bool bl) {
		releaseDuty_ = bl;
    }
    DECLSPEC void setParent(C_* c) {
		parent_ = c;
    }

    DECLSPEC ChildIterator childBegin() { return children_.begin(); }
    DECLSPEC ChildIterator childEnd() { return children_.end(); }
	DECLSPEC size_t childSize() { return children_.size(); }
    DECLSPEC ConstChildIterator childBegin() const { return children_.begin(); }
    DECLSPEC ConstChildIterator childEnd() const { return children_.end(); }
    DECLSPEC C_* getParent() { return parent_; }

private:
    Children children_;
    C_* parent_;
    bool releaseDuty_;
};

template <class C_>
TreeNode<C_>::~TreeNode() {
    if (releaseDuty_) {
		ChildIterator ite;
		for (ite = children_.begin(); ite != children_.end(); ite++) {
			(*ite)->setReleaseDuty(true);
			delete *ite;
		}
    }
}

#endif // ! TREE_H_
