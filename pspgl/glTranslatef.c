#include <pspvfpu.h>

#include "pspgl_internal.h"


void glTranslatef (GLfloat x, GLfloat y, GLfloat z)
{
	struct pspgl_context *c = pspgl_curctx;
	struct pspgl_matrix_stack *stk = c->current_matrix_stack;
	struct pspgl_matrix *mat = c->current_matrix;

// @@@ from here it's allways the same code exept for params of the command
	if (pspgl_curctx->displaylists.is_in_glNewList & GL_COMPILE) { // we must record this command
		struct stDisplayElement *new;
		new = (struct stDisplayElement *) malloc (sizeof(struct stDisplayElement));
		// @@@ put new element at the end of list
		if (!__pspgl_actuallist->first)
			__pspgl_actuallist->first = new;
		if (__pspgl_actuallist->last)
			__pspgl_actuallist->last->next = new;
		new->next = NULL;
		__pspgl_actuallist->last = new;
		
		// params of the command
		new->command_num = GLTRANSLATE; // to modify
		new->paramf1 = x; // to modify
		new->paramf2 = y; // to modify
		new->paramf3 = z; // to modify
	}
	
	if ((pspgl_curctx->displaylists.is_in_glNewList == GL_COMPILE_AND_EXECUTE)||(pspgl_curctx->displaylists.is_in_glNewList == 0)) {
 // @@@ from here it's different, because it's the old function
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
}

// @@@ added by Edorul
void glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)z);
}
