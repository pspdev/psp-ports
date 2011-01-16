#include "pspgl_internal.h"


void glClearStencil (GLint s)
{
	pspgl_curctx->clear.stencil = s;
}

