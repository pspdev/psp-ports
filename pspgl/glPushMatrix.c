#include <stdlib.h>
#include <string.h>

#include "pspgl_internal.h"
#include "pspgl_matrix.h"

void glPushMatrix (void)
{
	struct pspgl_context *c = pspgl_curctx;
	struct pspgl_matrix_stack *curstk = c->current_matrix_stack;
	unsigned flags;

	assert(curstk->flags & MF_VFPU);

	/* Make sure VFPU state is written back to memory */
	__pspgl_matrix_sync(c, curstk);

	if (unlikely(++curstk->depth == curstk->limit))
		goto out_error;

	/* No need to copy matrix values, since it's already in the
	   VFPU.  Do need to copy the flags though. */

	flags = c->current_matrix->flags;
	c->current_matrix = &curstk->stack[curstk->depth];
	c->current_matrix->flags = flags;
	return;

  out_error:
	curstk->depth--;
	GLERROR(GL_STACK_OVERFLOW);
}

