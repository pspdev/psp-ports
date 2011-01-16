#include "pspgl_internal.h"


#define GE_TEXENV_MODULATE				0
#define GE_TEXENV_DECAL					1
#define GE_TEXENV_BLEND					2
#define GE_TEXENV_REPLACE				3
#define GE_TEXENV_ADD					4


void glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params)
{
	int mode;

	if (target != GL_TEXTURE_ENV)
		goto out_error;

	switch (pname) {
	case GL_TEXTURE_ENV_MODE:
		switch ((GLenum) params[0]) {
		case GL_MODULATE:
			mode = GE_TEXENV_MODULATE;
			break;
		case GL_DECAL:
			mode = GE_TEXENV_DECAL;
			break;
		case GL_BLEND:
			mode = GE_TEXENV_BLEND;
			break;
		case GL_REPLACE:
			mode = GE_TEXENV_REPLACE;
			break;
		case GL_ADD:
			mode = GE_TEXENV_ADD;
			break;
		default:
			goto out_error;
		}
		__pspgl_context_writereg_masked(pspgl_curctx, CMD_TEXENV_FUNC, mode, 0x0000ff);
		break;

	case GL_TEXTURE_ENV_COLOR:
		__pspgl_context_writereg(pspgl_curctx, CMD_TEXENV_COL, COLOR3(params));
		break;

	default:
		goto out_error;
	}

	return;
	
out_error:
	GLERROR(GL_INVALID_ENUM);
}


void glTexEnvf (GLenum target, GLenum pname, GLfloat param)
{
	glTexEnvfv(target, pname, &param);
}


void glTexEnvi (GLenum target, GLenum pname, GLint param)
{
	GLfloat p = param;
	glTexEnvfv(target, pname, &p);
}

