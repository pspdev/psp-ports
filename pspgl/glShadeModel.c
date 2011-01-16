#include "pspgl_internal.h"


void glShadeModel (GLenum mode)
{
	switch (mode) {
	case GL_FLAT:
	case GL_SMOOTH:
		sendCommandi(CMD_SHADEMODEL, (mode & 1));
		break;
	default:
		GLERROR(GL_INVALID_ENUM);
	}
}
