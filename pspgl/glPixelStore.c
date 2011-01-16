#include "pspgl_internal.h"

void glPixelStorei( GLenum pname, GLint param )
{
	struct pspgl_context *c = pspgl_curctx;
	GLenum error;

	error = GL_INVALID_VALUE;
	if (param < 0)
		goto out_error;

	error = GL_INVALID_ENUM;
	switch(pname) {
	case GL_PACK_ROW_LENGTH:
		c->pack.row_length = param;
		break;
	case GL_PACK_ALIGNMENT:
		error = GL_INVALID_VALUE;
		if (!ispow2(param) || param < 1 || param > 8)
			goto out_error;
		c->pack.alignment = param;
		break;
	case GL_PACK_SKIP_PIXELS:
		c->pack.skip_pixels = param;
		break;
	case GL_PACK_SKIP_ROWS:
		c->pack.skip_rows = param;
		break;
	case GL_PACK_INVERT_MESA:
		c->pack.invert = param;
		break;

	case GL_UNPACK_ROW_LENGTH:
		c->unpack.row_length = param;
		break;
	case GL_UNPACK_ALIGNMENT:
		error = GL_INVALID_VALUE;
		if (!ispow2(param) || param < 1 || param > 8)
			goto out_error;
		c->unpack.alignment = param;
		break;
	case GL_UNPACK_SKIP_PIXELS:
		c->unpack.skip_pixels = param;
		break;
	case GL_UNPACK_SKIP_ROWS:
		c->unpack.skip_rows = param;
		break;

	default:
		goto out_error;
	}

	return;

  out_error:
	GLERROR(error);
}

void glPixelStoref( GLenum pname, GLfloat param )
{
	glPixelStorei(pname, param);
}
