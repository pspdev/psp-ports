#include <string.h>
#include "pspgl_internal.h"


void glEnd (void)
{
	struct pspgl_context *c = pspgl_curctx;

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
		
		new->command_num = GLEND;
	}
	
	if ((pspgl_curctx->displaylists.is_in_glNewList == GL_COMPILE_AND_EXECUTE)||(pspgl_curctx->displaylists.is_in_glNewList == 0)) {
 // @@@ from here it's different
	if (likely(c->beginend.vertex_count > 0)) {
		long prim = __pspgl_glprim2geprim(c->beginend.primitive);

		if (unlikely(prim < 0))
			goto out_error;

		/* If we've finished a line loop, connect the last
		   edge to the first vertex to close the loop. */
		if (c->beginend.primitive == GL_LINE_LOOP) {
			struct t2f_c4ub_n3f_v3f *vbuf = (struct t2f_c4ub_n3f_v3f *) c->beginend.vbuf_adr;

			vbuf += c->beginend.vertex_count;
			memcpy(vbuf, &c->beginend.line_loop_start, sizeof(*vbuf));
			c->beginend.vertex_count++;
		}

		__pspgl_context_render_prim(pspgl_curctx, prim, c->beginend.vertex_count,
					    GE_TEXTURE_32BITF | GE_COLOR_8888 | GE_NORMAL_32BITF | GE_VERTEX_32BITF,
					    c->beginend.vbuf_adr, NULL);
	}

	pspgl_curctx->beginend.primitive = -1;
	return;

  out_error:
	GLERROR(GL_INVALID_ENUM);
	}
}
