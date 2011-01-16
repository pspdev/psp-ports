#include "pspgl_internal.h"


void glTexCoord2f (GLfloat s, GLfloat t)
{
	pspgl_curctx->current.texcoord[0] = s;
	pspgl_curctx->current.texcoord[1] = t;
}


void glTexCoord2fv(const GLfloat *v)
{
	pspgl_curctx->current.texcoord[0] = v[0];
        pspgl_curctx->current.texcoord[1] = v[1];
}


void glTexCoord4fv(const GLfloat *v)
{
	pspgl_curctx->current.texcoord[0] = v[0];
        pspgl_curctx->current.texcoord[1] = v[1];
        pspgl_curctx->current.texcoord[2] = v[2];
        pspgl_curctx->current.texcoord[3] = v[3];
}

