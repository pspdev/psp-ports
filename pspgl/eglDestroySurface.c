#include <stdlib.h>
#include "pspgl_internal.h"
#include "pspgl_buffers.h"

EGLBoolean eglDestroySurface (EGLDisplay dpy, EGLSurface surface)
{
	struct pspgl_surface *s = (struct pspgl_surface*) surface; 

	if (!s)
		return EGL_FALSE;

	assert(s->refcount > 0);
	if (--s->refcount)
		return EGL_TRUE;

	/* Even if single-buffered, both front and back are initialized */
	__pspgl_buffer_free(s->color_front);
	__pspgl_buffer_free(s->color_back);

	if (s->depth_buffer)
		__pspgl_buffer_free(s->depth_buffer);

	free(s);

	return EGL_TRUE;
}
