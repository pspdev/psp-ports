/// 数式クラス

#ifndef FORMULA_H_
#define FORMULA_H_

#include "bulletmlcommon.h"

template <typename Val_>
class AbstractNumber {
public:
	DECLSPEC virtual Val_ value() const =0;
	DECLSPEC virtual ~AbstractNumber() {}
};

template <typename Val_>
class Number : public AbstractNumber<Val_> {
public:
	DECLSPEC explicit Number(Val_ val) : val_(val) {}

	DECLSPEC virtual Val_ value() const { return val_; }

private:
	Val_ val_;
};

typedef enum { op_null =0, op_add, op_sub, op_mul, op_div } Operator;

template <typename Val_>
class Formula : public AbstractNumber<Val_> {
private:
	typedef AbstractNumber<Val_> ANumber;

public:
	DECLSPEC virtual ~Formula() {
		delete lhs_;
		delete rhs_;
	}

	/// public だけど呼ばないで下さい。
	/**
	 * @todo yacc の使いかたを調べて、これを private に
	 */
	//@{
	DECLSPEC explicit Formula(ANumber* val)
		: lhs_(val), rhs_(0), op_(op_null), headsub_(false) {}
	DECLSPEC Formula(ANumber* lhs, Operator op, ANumber* rhs)
		: lhs_(lhs), rhs_(rhs), op_(op), headsub_(false) {}

	DECLSPEC Formula* setHeadSub() { headsub_ = true; return this; }
	//@}

	DECLSPEC virtual Val_ value() const {
		if (headsub_) return -valueBeforeHeadSub();
		else return valueBeforeHeadSub();
	}

private:
	Val_ valueBeforeHeadSub() const {
		switch (op_) {
		case op_null:
			return lhs_->value();
		case op_add:
			return lhs_->value() + rhs_->value();
		case op_sub:
			return lhs_->value() - rhs_->value();
		case op_mul:
			return lhs_->value() * rhs_->value();
		case op_div:
			return lhs_->value() / rhs_->value();
		default:
			return 0;	// avoid warning
		}
	}

private:
	ANumber *lhs_, *rhs_;
	Operator op_;
	bool headsub_;

};

#endif // ! FORMULA_H_
