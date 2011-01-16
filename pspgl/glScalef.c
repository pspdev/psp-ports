#include <pspvfpu.h>

#include "pspgl_internal.h"


void glScalef (GLfloat x, GLfloat y, GLfloat z)
{
	struct pspgl_context *c = pspgl_curctx;
	struct pspgl_matrix_stack *stk = c->current_matrix_stack;
	struct pspgl_matrix *mat = c->current_matrix;

	if (!(stk->flags & MF_DISABLED))
		stk->flags |= MF_DIRTY;

	mat->flags &= ~MF_IDENTITY;

	pspvfpu_use_matrices(pspgl_curctx->vfpu_context, VFPU_STACKTOP, VMAT6);

	assert(stk->flags & MF_VFPU);

	asm volatile("mtv	%0, s600\n"
		     "mtv	%1, s610\n"
		     "mtv	%2, s620\n"
		     "vscl.q	c700, c700, s600\n"
		     "vscl.q	c710, c710, s610\n"
		     "vscl.q	c720, c720, s620\n"
		     : : "r" (x), "r" (y), "r" (z));
}
