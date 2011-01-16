#include <malloc.h>

#include "pspgl_internal.h"
#include "pspgl_buffers.h"
#include "pspgl_dlist.h"

void glDrawBezierArraysPSP(GLenum mode, GLuint u, GLuint v, GLint first)
{
	struct pspgl_context *c = pspgl_curctx;
	struct vertex_format vfmt, *vfmtp;
	struct pspgl_buffer *vbuf;
	unsigned vbuf_offset;
	const void *buf;
	unsigned prim;
	unsigned count;
	GLenum error;

	error = GL_INVALID_ENUM;

	switch(mode) {
	case GL_TRIANGLES:	prim = 0; break;
	case GL_LINES:		prim = 1; break;
	case GL_POINTS:		prim = 2; break;

	default:
		goto out_error;
	}
	sendCommandi(CMD_PATCH_PRIM, prim);

	count = u * v;

	vbuf = NULL;
	vbuf_offset = 0;
	vfmtp = &vfmt;

	if (__pspgl_cache_arrays()) {
		/* FAST: draw from locked array */
		struct locked_arrays *l = &c->vertex_array.locked;

		vbuf = l->cached_array;
		vbuf_offset = l->cached_array_offset;
		vfmtp = &l->vfmt;
		first -= l->cached_first;

		vbuf->refcount++;
	}

	if (unlikely(vbuf == NULL)) {
		/* SLOW: convert us some arrays */
		__pspgl_ge_vertex_fmt(c, &vfmt);

		if (vfmt.hwformat == 0)
			return;

		vbuf = __pspgl_varray_convert(&vfmt, first, count);

		error = GL_OUT_OF_MEMORY;
		if (vbuf == NULL)
			goto out_error;
	}

	buf = vbuf->base + vbuf_offset;
	buf += first * vfmtp->vertex_size;

	__pspgl_context_render_setup(c, vfmtp->hwformat, buf, NULL);
	__pspgl_context_writereg_uncached(c, CMD_BEZIER, (v << 8) | u);
	__pspgl_context_pin_buffers(c);
	__pspgl_dlist_pin_buffer(vbuf, BF_PINNED_RD);

	__pspgl_buffer_free(vbuf);
	return;

  out_error:
	GLERROR(error);
}


void glPatchSubdivisionPSP(GLuint u, GLuint v)
{
	sendCommandi(CMD_PATCH_SUBDIV, (v << 8) | u);
}
