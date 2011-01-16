#include <pspvfpu.h>

#include "pspgl_internal.h"


void glMultMatrixf (const GLfloat *m)
{
	struct pspgl_context *c = pspgl_curctx;
	struct pspgl_matrix_stack *stk = c->current_matrix_stack;
	struct pspgl_matrix *mat = c->current_matrix;

	if (!(stk->flags & MF_DISABLED))
		stk->flags |= MF_DIRTY;

	mat->flags &= ~MF_IDENTITY;

	pspvfpu_use_matrices(pspgl_curctx->vfpu_context, VFPU_STACKTOP, VMAT6 | VMAT5);

	assert(stk->flags & MF_VFPU);

	asm volatile("vmmov.q	m500, m700\n"

		     "ulv.q	c600,  0 + %0\n"
		     "ulv.q	c610, 16 + %0\n"
		     "ulv.q	c620, 32 + %0\n"
		     "ulv.q	c630, 48 + %0\n"

		     "vmmul.q	m700, m500, m600\n"
		     : : "m" (*m) : "memory");
}

