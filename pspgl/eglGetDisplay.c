#include "pspgl_internal.h"



EGLDisplay eglGetDisplay (NativeDisplayType display)
{
	return (EGLDisplay)"foo"; /* non-EGL_NO_DISPLAY thing */
}
