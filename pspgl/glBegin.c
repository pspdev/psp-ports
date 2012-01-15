#include "pspgl_internal.h"


void glBegin (GLenum mode)
{
 // @@@ from here it's allways the same code exept for index of the command
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
		
		new->command_num = GLBEGIN;
		new->parami1 = mode;
	}
	
	if ((pspgl_curctx->displaylists.is_in_glNewList == GL_COMPILE_AND_EXECUTE)||(pspgl_curctx->displaylists.is_in_glNewList == 0)) {
 // @@@ from here it's different
	if (likely(mode >= GL_POINTS && mode <= GL_SPRITES_PSP)) {
		pspgl_curctx->beginend.primitive = mode;
		pspgl_curctx->beginend.vertex_count = 0;
		return;
	} 
	GLERROR(GL_INVALID_ENUM);
	}
}
