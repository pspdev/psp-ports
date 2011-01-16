#include "pspgl_internal.h"


EGLBoolean eglGetConfigAttrib (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value)
{
	switch (attribute) {
	case EGL_WIDTH:
		*value = 480;
		break;
	case EGL_HEIGHT:
		*value = 272;
		break;
	default:
		return EGL_FALSE;
	}

	return EGL_TRUE;
}

