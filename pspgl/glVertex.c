#include <string.h>
#include "pspgl_internal.h"
#include "pspgl_dlist.h"

#define BUFSZ	12		/* must be a multiple of 2,3 and 4 */

void glVertex3f (GLfloat x, GLfloat y, GLfloat z)
{
	struct pspgl_context *c = pspgl_curctx;
	struct t2f_c4ub_n3f_v3f *vbuf;

	if (c->beginend.vertex_count == 0)
		c->beginend.vbuf_adr = __pspgl_dlist_insert_space(BUFSZ * sizeof(struct t2f_c4ub_n3f_v3f));

	vbuf = (struct t2f_c4ub_n3f_v3f *) c->beginend.vbuf_adr;

	if (unlikely(!vbuf))
		goto out_error;

	vbuf += c->beginend.vertex_count;

	vbuf->texcoord[0] = c->current.texcoord[0];
	vbuf->texcoord[1] = c->current.texcoord[1];
	vbuf->color = c->current.color;
	vbuf->normal[0] = c->current.normal[0];
	vbuf->normal[1] = c->current.normal[1];
	vbuf->normal[2] = c->current.normal[2];
	vbuf->vertex[0] = x;
	vbuf->vertex[1] = y;
	vbuf->vertex[2] = z;

	/* If it's a line loop, save the first vertex so that we can close the loop in glEnd(). */
	if (c->beginend.primitive == GL_LINE_LOOP && c->beginend.vertex_count == 0) {
		/* Note: If a vertex buffer is split (see below), the vertex_count of the new buffer
		   will be 1 because of the overhang copied from the previous buffer.  If there were
		   no overhang for line loops, we wouldn't be able to get away with this. */
		memcpy(&c->beginend.line_loop_start, vbuf, sizeof(*vbuf));
	}

	c->beginend.vertex_count++;

	if (unlikely(c->beginend.vertex_count == BUFSZ)
	 || unlikely(c->beginend.primitive == GL_QUADS && c->beginend.vertex_count == 4)) {
		static const char overhang_count [] = {
			0,	/* GL_POINTS */
			0,	/* GL_LINES */
			1,	/* GL_LINE_LOOP */
			1,	/* GL_LINE_STRIP */
			0,	/* GL_TRIANGLES */
			2,	/* GL_TRIANGLE_STRIP */
			2,	/* GL_TRIANGLE_FAN */
			0,	/* GL_QUADS (really trifan) */
			2,	/* GL_QUAD_STRIP (really tristrip) */
			1,	/* GL_POLYGON (really trifan) */
			0	/* GL_SPRITES_PSP */
		};
		char overhang = overhang_count[c->beginend.primitive];
		long prim = __pspgl_glprim2geprim(c->beginend.primitive);

		/* vertex buffer full, render + restart */
		__pspgl_context_render_prim(c, prim, c->beginend.vertex_count,
					    GE_TEXTURE_32BITF | GE_COLOR_8888 | GE_NORMAL_32BITF | GE_VERTEX_32BITF,
					    c->beginend.vbuf_adr, NULL);

		/* copy overhang */
		c->beginend.vertex_count = overhang;

		if (overhang) {
			struct t2f_c4ub_n3f_v3f *vbuf_start, *prev;

			prev = c->beginend.vbuf_adr;
			c->beginend.vbuf_adr = __pspgl_dlist_insert_space(BUFSZ * sizeof(struct t2f_c4ub_n3f_v3f));
			vbuf_start = c->beginend.vbuf_adr;

			if (c->beginend.primitive == GL_TRIANGLE_FAN || c->beginend.primitive == GL_POLYGON) {
				memcpy(vbuf_start, prev, sizeof(vbuf_start[0]));
				c->beginend.vertex_count++;
				vbuf_start++;
			}

			memcpy(vbuf_start, vbuf - overhang + 1, overhang * sizeof(vbuf[0]));
		}
	}
	return;

  out_error:
	GLERROR(GL_OUT_OF_MEMORY);
}


void glVertex2f (GLfloat x, GLfloat y)
{
	glVertex3f(x, y, 0.0);
}


void glVertex3fv (const GLfloat *v)
{
	glVertex3f(v[0], v[1], v[2]);
}


void glVertex4fv (const GLfloat *v)
{
	GLfloat scale = 1.0f / v[3];
	glVertex3f(scale * v[0], scale * v[1], scale * v[2]);
}


