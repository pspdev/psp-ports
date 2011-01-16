#include "pspgl_internal.h"


void glFogf (GLenum pname, GLfloat param)
{
	float distance;
	GLenum error;

	error = GL_INVALID_VALUE;
	switch (pname) {
	case GL_FOG_MODE:
		if (param != GL_LINEAR)
			goto out_error;
		break;

	case GL_FOG_START:
		pspgl_curctx->fog.near = param;
		distance = pspgl_curctx->fog.far - pspgl_curctx->fog.near;
		if (unlikely(distance == 0))
			goto out_error;
		distance = 1.0f / distance;
		sendCommandf(CMD_FOG_NEAR, distance);
		break;

	case GL_FOG_END:
		pspgl_curctx->fog.far = param;
		sendCommandf(CMD_FOG_FAR, pspgl_curctx->fog.far);
		break;
	/**
	case XXXX:
		pspgl_curctx->fog.XXXXX = param;
		sendCommandf(248, ??fog type??);
		break;
	 */
	default:
		error = GL_INVALID_ENUM;
		goto out_error;
	}

	return;

  out_error:
	GLERROR(error);
}


void glFogi (GLenum pname, GLint param)
{
	glFogf(pname, (GLfloat) param);
}


void glFogfv (GLenum pname, const GLfloat *param)
{
	switch (pname) {
	case GL_FOG_COLOR:
		sendCommandi(CMD_FOG_COLOR, COLOR3(param));
		break;
	default:
		glFogf(pname, param[0]);
	}
}

