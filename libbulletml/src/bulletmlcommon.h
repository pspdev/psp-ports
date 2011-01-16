#ifndef bulletmlcommon_h_
#define bulletmlcommon_h_

#ifdef WIN32_DLL_EXPORT
# define DECLSPEC __declspec(dllexport)
#else
# define DECLSPEC
#endif

#ifndef __EXCEPTIONS
#include <boost/throw_exception.hpp>

inline void boost::throw_exception(std::exception const &) {}
#endif

#endif // ! bulletmlcommon_h_

