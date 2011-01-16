#include <stdlib.h>
#include "pspgl_internal.h"


static const
char *egl_strings [] = {
        /* EGL_VENDOR */
        "pspEGL",
        /* EGL_VERSION */
        "(pspEGL build " __DATE__ ", " __TIME__ ")",
        /* EGL_EXTENSIONS */
        ""
};


const char* eglQueryString (EGLDisplay dpy, EGLint name)
{
        unsigned long idx = name - EGL_VENDOR;
        const char *s;

        if (idx >= sizeof(egl_strings)/sizeof(egl_strings[0])) {
		EGLERROR(EGL_BAD_PARAMETER);
                s = NULL;
        } else {
                s = egl_strings[idx];
        }

        return s;
}

