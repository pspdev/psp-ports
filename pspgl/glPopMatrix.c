#include <stdlib.h>

#include "pspgl_internal.h"
#include "pspgl_matrix.h"

void glPopMatrix (void)
{
	struct pspgl_context *c = pspgl_curctx;
	struct pspgl_matrix_stack *curstk = c->current_matrix_stack;

	assert(curstk->flags & MF_VFPU);

	if (unlikely(curstk->depth == 0))
		goto out_error;

	c->current_matrix = &curstk->stack[--curstk->depth];
	if (!(curstk->flags & MF_DISABLED))
		curstk->flags |= MF_DIRTY;

	/* Reload VFPU with new top-of-stack */
	__pspgl_matrix_load(c, curstk);
	return;

  out_error:
	GLERROR(GL_STACK_UNDERFLOW);
}
