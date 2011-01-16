#include <pspvfpu.h>

#include "pspgl_internal.h"


void glTranslatef (GLfloat x, GLfloat y, GLfloat z)
{
	struct pspgl_context *c = pspgl_curctx;
	struct pspgl_matrix_stack *stk = c->current_matrix_stack;
	struct pspgl_matrix *mat = c->current_matrix;

	assert(stk->flags & MF_VFPU);

	if (!(stk->flags & MF_DISABLED))
		stk->flags |= MF_DIRTY;

	mat->flags &= ~MF_IDENTITY;

	pspvfpu_use_matrices(c->vfpu_context, VFPU_STACKTOP, VMAT6);

	asm volatile("mtv	%0, s630\n"	// x
		     "mtv	%1, s631\n"	// y
		     "mtv	%2, s632\n"	// z

		     "vscl.q	c600, c700, s630\n"	// col[0] * x
		     "vscl.q	c610, c710, s631\n"	// col[1] * y
		     "vscl.q	c620, c720, s632\n"	// col[2] * z

		     "vadd.q	c610, c610, c600\n"	// col[1] += col[0]
		     "vadd.q	c730, c730, c620\n"	// col[3] += col[2]
		     "vadd.q	c730, c730, c610\n"	// col[3] += col[1]+col[0]
		     : : "r" (x), "r" (y), "r" (z));
}
