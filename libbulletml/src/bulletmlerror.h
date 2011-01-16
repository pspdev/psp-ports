#ifndef BULLETMLERROR_H_
#define BULLETMLERROR_H_

#include "bulletmlcommon.h"

#include <stdexcept>

/// Ç≥Ç∑Ç™Ç…Ç‡Ç§ÇøÇÂÇ¡Ç∆è[é¿Ç≥ÇπÇÒÇ∆ÇÀÇ¶
class BulletMLError : public std::runtime_error {
public:
	DECLSPEC BulletMLError(const std::string& msg)
		: std::runtime_error(msg) {}

#ifdef __EXCEPTIONS  
	DECLSPEC static void doAssert(const char* str) {
		throw BulletMLError(str);
	}
	DECLSPEC static void doAssert(const std::string& str) {
		throw BulletMLError(str);
	}
	DECLSPEC static void doAssert(bool t, const char* str) {
		if (!t) throw BulletMLError(str);
	}
	DECLSPEC static void doAssert(bool t, const std::string& str) {
		if (!t) throw BulletMLError(str);
	}
#else
	DECLSPEC static void doAssert(const char*) {
	}
	DECLSPEC static void doAssert(const std::string&) {
	}
	DECLSPEC static void doAssert(bool, const char*) {
	}
	DECLSPEC static void doAssert(bool, const std::string&) {
	}
#endif // __EXCEPTIONS

};

#endif // ! BULLETMLERROR_H_

