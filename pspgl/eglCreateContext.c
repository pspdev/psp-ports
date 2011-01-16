#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <pspvfpu.h>

#include "pspgl_internal.h"
#include "pspgl_matrix.h"

static GLboolean init_matrix_stack(struct pspgl_matrix_stack *mstk, int limit, unsigned flags)
{
	mstk->stack = memalign(VFPU_ALIGNMENT, sizeof(struct pspgl_matrix) * limit);
	if (mstk->stack == NULL)
		return GL_FALSE;

	memcpy(mstk->stack[0].mat, __pspgl_identity, sizeof(mstk->stack[0].mat));
	mstk->stack[0].flags = MF_IDENTITY;

	mstk->limit = limit;
	mstk->depth = 0;
	mstk->flags = flags | MF_DIRTY;

	mstk->scale[0] = mstk->scale[1] = mstk->scale[2] = mstk->scale[3] = 1.0f;
	mstk->trans[0] = mstk->trans[1] = mstk->trans[2] = mstk->trans[3] = 0.0f;

	return GL_TRUE;
}

EGLContext eglCreateContext (EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list)
{
	struct pspgl_context *ctx = memalign(VFPU_ALIGNMENT, sizeof(struct pspgl_context));

	if (!ctx)
		goto out_error;

	psp_log("created context %p\n", ctx);

	memset(ctx, 0, sizeof(*ctx));

	ctx->refcount = 1;

	/* Use VFPU matrix 7 as the current matrix */
	ctx->vfpu_context = pspvfpu_initcontext();
	if (ctx->vfpu_context == NULL)
		goto out_free_partial;

	/* OpenGL/ES stack size limits; we could probably afford to be more generous */
	if (!init_matrix_stack(&ctx->projection_stack,	 2, 0) ||
	    !init_matrix_stack(&ctx->texture_stack,	 2, 0) ||
	    !init_matrix_stack(&ctx->modelview_stack,	16, 0) ||
	    !init_matrix_stack(&ctx->view_stack,	 8, 0))
		goto out_free_partial;

	for(int i = 0; i < NBONES; i++)
		if (!init_matrix_stack(&ctx->bone_stacks[i], 1, MF_DISABLED))
			goto out_free_partial;

	/* Make modelview current by default; this also loads the
	   modelview into the VFPU */
	ctx->current_matrix_stack = NULL;
	__pspgl_matrix_select(ctx, &ctx->modelview_stack);

	ctx->vertexblend.enabled = GL_FALSE;

	if (share_context == NULL) {
		ctx->shared = malloc(sizeof(struct pspgl_shared_context));
		if (ctx->shared == NULL)
			goto out_free_partial;

		memset(ctx->shared, 0, sizeof(struct pspgl_shared_context));
	} else
		ctx->shared = ((struct pspgl_context *) share_context)->shared;
	ctx->shared->refcount++;

	ctx->attribstackdepth = 0;
	ctx->clattribstackdepth = 0;

	ctx->pack.alignment = 4;
	ctx->unpack.alignment = 4;

	return (EGLContext) ctx;

  out_free_partial:
	eglDestroyContext(dpy, ctx);

  out_error:
	EGLERROR(EGL_BAD_ALLOC);
	return EGL_NO_CONTEXT;
}


