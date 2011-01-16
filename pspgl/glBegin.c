#include "pspgl_internal.h"


void glBegin (GLenum mode)
{
	if (likely(mode >= GL_POINTS && mode <= GL_SPRITES_PSP)) {
		pspgl_curctx->beginend.primitive = mode;
		pspgl_curctx->beginend.vertex_count = 0;
		return;
	} 
	GLERROR(GL_INVALID_ENUM);
}
