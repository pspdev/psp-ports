#include "pspgl_internal.h"


static const char stenciltestfunc_mapping [] = { 0, 4, 2, 5, 6, 3, 7, 1 };

void glStencilFunc( GLenum func, GLint ref, GLuint mask)
{
	unsigned char sref = (unsigned char) ref;

	if (unlikely(func < GL_NEVER) || unlikely(func > GL_ALWAYS))
		goto out_error;

	func &= 0x0007;

	sendCommandi(CMD_STENCIL_FUNC, (mask << 16) | (sref << 8) | stenciltestfunc_mapping[func]);
	return;

  out_error:
	GLERROR(GL_INVALID_ENUM);
}

