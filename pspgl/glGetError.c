#include "pspgl_internal.h"


GLenum glGetError (void)
{
	GLenum err = pspgl_curctx->glerror;
	pspgl_curctx->glerror = GL_NO_ERROR;
	return err;
}
