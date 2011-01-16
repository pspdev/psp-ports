#include "pspgl_internal.h"


void glLightModelfv (GLenum pname, const GLfloat *params)
{
	switch (pname) {
	case GL_LIGHT_MODEL_AMBIENT:
		sendCommandi(CMD_AMBIENT_COLOR, COLOR3(params));
		sendCommandi(CMD_AMBIENT_ALPHA, (int) (255.0 * CLAMPF(params[3])));
		break;
	case GL_LIGHT_MODEL_COLOR_CONTROL:
		sendCommandi(CMD_LIGHTMODEL,
			     (params[0] == GL_SEPARATE_SPECULAR_COLOR) ? 1 : 0);
		break;
	default:
		GLERROR(GL_INVALID_ENUM);
		return;
	}
}


void glLightModelf (GLenum pname, GLfloat param)
{
	glLightModelfv(pname, &param);
}

void glLightModeli (GLenum pname, GLint param)
{
	glLightModelf(pname, param);
}

