#include <string.h>
#include "pspgl_internal.h"


void glEnd (void)
{
	struct pspgl_context *c = pspgl_curctx;

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
