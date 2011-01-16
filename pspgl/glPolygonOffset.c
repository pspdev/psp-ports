#include "pspgl_internal.h"

void glPolygonOffset (GLfloat factor, GLfloat units)
{
	/* XXX: not correct, but may work well in most cases. */
	pspgl_curctx->viewport.depth_offset = units;
}
