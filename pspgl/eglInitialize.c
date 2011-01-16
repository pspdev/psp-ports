#include <stdlib.h>
#include <pspdisplay.h>
#include "pspgl_internal.h"


EGLBoolean eglInitialize (EGLDisplay dpy, EGLint *major, EGLint *minor)
{
	static int initialized = 0;

	psp_log("\n\n=============== pspGL, build " __DATE__ ", " __TIME__ " ===============\n");

	if (!initialized) {
		atexit((void *) eglTerminate);
		sceDisplaySetMode(0, 480, 272);
		initialized = 1;
	}

	if (major)
		*major = 1;
	if (minor)
		*minor = 1;

	return EGL_TRUE;
}
