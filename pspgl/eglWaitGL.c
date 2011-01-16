#include "pspgl_internal.h"


EGLBoolean eglWaitGL (void)
{
	glFinish();
	return EGL_TRUE;
}
