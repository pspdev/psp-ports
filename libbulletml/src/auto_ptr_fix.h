/**
 *
 */

#ifndef auto_ptr_fix_h_
#define auto_ptr_fix_h_

#include <memory>

template <class T_>
inline void auto_ptr_copy (std::auto_ptr<T_>& lhs, std::auto_ptr<T_> rhs) {
	lhs = rhs;
}
template <class T_>
inline void auto_ptr_copy (std::auto_ptr<T_>& lhs, T_* rhs) {
	std::auto_ptr<T_> p(rhs);
	lhs = p;
}
template <class T_>
inline T_* auto_ptr_release(std::auto_ptr<T_>& p) {
	T_* ret = p.release();
	auto_ptr_copy(p, std::auto_ptr<T_>());
	return ret;
}

#endif // ! auto_ptr_fix_h_

