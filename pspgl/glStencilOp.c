#include "pspgl_internal.h"


static
int map_sfunc (GLenum opcode)
{
	int ret;

	switch (opcode) {
	case GL_ZERO:
		ret = GU_ZERO;
		break;
	case GL_INVERT:
		ret = GU_INVERT;
		break;
	case GL_KEEP:
		ret = GU_KEEP;
		break;
	case GL_REPLACE:
		ret = GU_REPLACE;
		break;
	case GL_INCR:
		ret = GU_INCR;
		break;
	case GL_DECR:
		ret = GU_DECR;
		break;
	default:
		GLERROR(GL_INVALID_ENUM);
		ret = 0;
	}
	
	return ret;
}

void glStencilOp (GLenum fail, GLenum zfail, GLenum zpass)
{
	sendCommandi(CMD_STENCIL_OP, (map_sfunc(zpass) << 16) | (map_sfunc(zfail) << 8) | map_sfunc(fail));
}
