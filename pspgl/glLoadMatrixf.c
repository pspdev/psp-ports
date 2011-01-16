#include <pspvfpu.h>

#include "pspgl_internal.h"
#include "pspgl_matrix.h"

void glLoadMatrixf (const GLfloat *m)
{
	struct pspgl_matrix_stack *stk = pspgl_curctx->current_matrix_stack;
	struct pspgl_matrix *mat = pspgl_curctx->current_matrix;

	assert(stk->flags & MF_VFPU);

	pspvfpu_use_matrices(pspgl_curctx->vfpu_context, VFPU_STACKTOP, 0);

	asm volatile("ulv.q	c700,  0 + %0\n"
		     "ulv.q	c710, 16 + %0\n"
		     "ulv.q	c720, 32 + %0\n"
		     "ulv.q	c730, 48 + %0\n"
		     : : "m" (*m) : "memory");

	if (!(stk->flags & MF_DISABLED))
		stk->flags |= MF_DIRTY;

	mat->flags &= ~MF_IDENTITY;
}
