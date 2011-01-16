#ifndef FORMULA_VARIABLE_H_
#define FORMULA_VARIABLE_H_

#include "formula.h"
#include "bulletmlrunner.h"
#include "bulletmlcommon.h"

#include <cstdlib>
#include <vector>

namespace Variables {
	DECLSPEC extern float rank;
	DECLSPEC extern std::vector<float>* parameters;
	DECLSPEC extern BulletMLRunner* runner;
}

template <typename Val_>
class Random : public AbstractNumber<Val_> {
public:
	DECLSPEC virtual Val_ value() const {
		return Variables::runner->getRand();
	}
};

template <typename Val_>
class Rank : public AbstractNumber<Val_> {
public:
	DECLSPEC virtual Val_ value() const {
		return Variables::rank;
	}
};

template <typename Val_>
class Param : public AbstractNumber<Val_> {
public:
	DECLSPEC explicit Param(int id) : id_(id) {}
	DECLSPEC virtual Val_ value() const {
		if (Variables::parameters && id_ < Variables::parameters->size()) {
			return (*Variables::parameters)[id_];
		}
		else {
			return 1;
		}
	}

private:
	int id_;
};

#endif // ! FORMULA_VARIABLE_H_
