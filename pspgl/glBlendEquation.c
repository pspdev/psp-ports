#include "pspgl_internal.h"


/* Blend Equation */
#define GE_ADD               0
#define GE_SUBTRACT          1
#define GE_REVERSE_SUBTRACT  2
#define GE_MIN               3
#define GE_MAX               4
#define GE_ABS               5


void glBlendEquation (GLenum equation)
{
	int e;

	switch (equation) {
	case GL_FUNC_SUBTRACT:
		e = GE_SUBTRACT;
		break;
	case GL_FUNC_REVERSE_SUBTRACT:
		e = GE_REVERSE_SUBTRACT;
		break;
	case GL_MIN:
		e = GE_MIN;
		break;
	case GL_MAX:
		e = GE_MAX;
		break;
	case GL_FUNC_ADD:
		e = GE_ADD;
		break;
	default:
		GLERROR(GL_INVALID_ENUM);
		return;
	}

	__pspgl_context_writereg_masked(pspgl_curctx, CMD_BLEND_FUNC, (e << 8), 0xf00);
}

