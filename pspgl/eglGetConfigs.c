#include "pspgl_internal.h"


EGLBoolean eglGetConfigs (EGLDisplay dpy, EGLConfig *configs, 
			  EGLint config_size, EGLint *num_config)
{
	int depth;
	int nconfig = 0;

	for(depth = 0; depth <= 1; depth++) {
		int pixidx;

		for(pixidx = 0; __pspgl_pixconfigs[pixidx].hwformat != -1; pixidx++) {
			if (configs != NULL) {
				if (nconfig >= config_size)
					goto out;

				configs[nconfig] = EGLCONFIGIDX(pixidx, depth);
			}
			nconfig++;
		}
	}

  out:
	*num_config = nconfig;
	return EGL_TRUE;
}
