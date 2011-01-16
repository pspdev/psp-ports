#include <pspvfpu.h>

#include "pspgl_internal.h"
#include "pspgl_matrix.h"

#if VFPU_STACKTOP != VMAT7
#error Fix this VFPU assembler
#endif

/* Write a copy of a matrix from the VFPU into memory; does not change
   the state of MF_VFPU. */
void __pspgl_matrix_sync(struct pspgl_context *c, const struct pspgl_matrix_stack *s)
{
	struct pspgl_matrix *m = &s->stack[s->depth];

	if ((s->flags & MF_VFPU) == 0)
		return;

	pspvfpu_use_matrices(c->vfpu_context, VFPU_STACKTOP, 0);

	asm volatile("sv.q	c700,  0 + %0, wb\n"
		     "sv.q	c710, 16 + %0, wb\n"
		     "sv.q	c720, 32 + %0, wb\n"
		     "sv.q	c730, 48 + %0, wb\n"
		     : "=m" (m->mat[0]) : : "memory");
}

/* Load the top of a matrix stack into the VFPU. */
void __pspgl_matrix_load(struct pspgl_context *c, struct pspgl_matrix_stack *s)
{
	const struct pspgl_matrix *m = &s->stack[s->depth];

	pspvfpu_use_matrices(c->vfpu_context, VFPU_STACKTOP, 0);

	if (m->flags & MF_IDENTITY)
		asm volatile("vmidt.q	m700\n" : : : "memory");
	else
		asm volatile("lv.q	c700,  0 + %0\n"
			     "lv.q	c710, 16 + %0\n"
			     "lv.q	c720, 32 + %0\n"
			     "lv.q	c730, 48 + %0\n"
			     : : "m" (m->mat[0]) : "memory");
	
	s->flags |= MF_VFPU;
}

/* Select a new matrix stack as the current matrix stack. This
   enforces the invarient that the top of the current matrix stack is
   always in the VFPU. */
void __pspgl_matrix_select(struct pspgl_context *c, struct pspgl_matrix_stack *s)
{
	struct pspgl_matrix_stack *stk = c->current_matrix_stack;

	if (stk == s)
		return;

	if (stk) {
		/* Flush out the old stack state */
		assert(stk->flags & MF_VFPU);
		__pspgl_matrix_sync(c, stk);
		stk->flags &= ~MF_VFPU;
	}

	c->current_matrix_stack = s;
	c->current_matrix = &s->stack[s->depth];

	/* Load up the new VFPU state */
	assert((s->flags & MF_VFPU) == 0);
	__pspgl_matrix_load(c, s);
}
