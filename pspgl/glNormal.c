#include "pspgl_internal.h"


void glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz)
{
	pspgl_curctx->current.normal[0] = nx;
	pspgl_curctx->current.normal[1] = ny;
	pspgl_curctx->current.normal[2] = nz;
}


void glNormal3fv (const GLfloat *n)
{
	pspgl_curctx->current.normal[0] = n[0];
	pspgl_curctx->current.normal[1] = n[1];
	pspgl_curctx->current.normal[2] = n[2];
}

