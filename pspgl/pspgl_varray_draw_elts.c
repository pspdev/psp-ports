#include "pspgl_internal.h"
#include "pspgl_buffers.h"

void __pspgl_varray_draw_elts(GLenum mode, GLenum idx_type, const void *const indices, GLsizei count)
{
	struct locked_arrays *l = &pspgl_curctx->vertex_array.locked;
	int minidx, maxidx;

	minidx = maxidx = -1;

	if (l->count > 0) {
		/* use locked range */

		minidx = l->first;
		maxidx = l->first + l->count;
	} 

	__pspgl_varray_draw_range_elts(mode, idx_type, indices, count, minidx, maxidx);
}
