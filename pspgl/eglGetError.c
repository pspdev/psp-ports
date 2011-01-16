#include "pspgl_internal.h"


EGLint eglGetError (void)
{
	EGLint err = __pspgl_eglerror;
	__pspgl_eglerror = EGL_SUCCESS;
	return err;
}
