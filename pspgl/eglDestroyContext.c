#include <stdlib.h>

#include <pspvfpu.h>

#include "pspgl_internal.h"
#include "pspgl_texobj.h"

EGLBoolean eglDestroyContext (EGLDisplay dpy, EGLContext ctx)
{
	struct pspgl_context *c = (struct pspgl_context *) ctx;

	if (!c) {
		EGLERROR(EGL_BAD_CONTEXT);
		return EGL_FALSE;
	}

	assert(c->refcount > 0);
	if (--c->refcount)
		return EGL_TRUE;

	c->shared->refcount--;

	if (c->shared->refcount == 0) {
		__pspgl_hash_foreach_free(&c->shared->texture_objects, (void (*) (void *)) __pspgl_texobj_free);
		__pspgl_hash_foreach_free(&c->shared->display_lists, /* XXX should become dlist_free() */ free);
		free(c->shared);
	}

	if (c->draw)
		eglDestroySurface(dpy, c->draw);
	if (c->read)
		eglDestroySurface(dpy, c->read);

	free(c->projection_stack.stack);
	free(c->modelview_stack.stack);
	free(c->texture_stack.stack);
	free(c->view_stack.stack);
	for(int i = 0; i < NBONES; i++)
		free(c->bone_stacks[i].stack);

	if (c->vfpu_context)
		pspvfpu_deletecontext(c->vfpu_context);

	free(c);

	return EGL_TRUE;
}

