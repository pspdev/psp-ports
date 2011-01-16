#include "pspgl_internal.h"


/* Blend Src Factor */
#define GE_SF_DST_COLOR			0
#define GE_SF_ONE_MINUS_DST_COLOR	1
#define GE_SF_SRC_ALPHA			2
#define GE_SF_ONE_MINUS_SRC_ALPHA	3
#define GE_SF_DST_ALPHA			4
#define GE_SF_ONE_MINUS_DST_ALPHA	5
#define GE_SF_FIX			10

/* Blend Dst Factor */
#define GE_DF_SRC_COLOR			0
#define GE_DF_ONE_MINUS_SRC_COLOR	1
#define GE_DF_SRC_ALPHA			2
#define GE_DF_ONE_MINUS_SRC_ALPHA	3
#define GE_DF_DST_ALPHA			4
#define GE_DF_ONE_MINUS_DST_ALPHA	5
#define GE_DF_FIX			10


void glBlendFunc (GLenum sfactor, GLenum dfactor)
{
	unsigned int srcfact, dstfact;

	switch (dfactor) {
	case GL_ZERO:
		sendCommandi(CMD_FIXEDCOL_DST, 0x000000);
		dstfact = GE_DF_FIX;
		break;
	case GL_ONE:
		sendCommandi(CMD_FIXEDCOL_DST, 0xffffff);
		dstfact = GE_DF_FIX;
		break;
	case GL_SRC_ALPHA:
		dstfact = GE_DF_SRC_ALPHA;
		break;
	case GL_ONE_MINUS_SRC_ALPHA:
		dstfact = GE_DF_ONE_MINUS_SRC_ALPHA;
		break;
	case GL_DST_ALPHA:
		dstfact = GE_DF_DST_ALPHA;
		break;
	case GL_ONE_MINUS_DST_ALPHA:
		dstfact = GE_DF_ONE_MINUS_DST_ALPHA;
		break;
	case GL_SRC_COLOR:
		dstfact = GE_DF_SRC_COLOR;
		break;
	case GL_ONE_MINUS_SRC_COLOR:
		dstfact = GE_DF_ONE_MINUS_SRC_COLOR;
		break;

	case GL_DST_COLOR:
	case GL_ONE_MINUS_DST_COLOR:
	default:
		goto out_error;
	}

	switch (sfactor) {
	case GL_ZERO:
		sendCommandi(CMD_FIXEDCOL_SRC, 0x000000);
		srcfact = GE_SF_FIX;
		break;
	case GL_ONE:
		sendCommandi(CMD_FIXEDCOL_SRC, 0xffffff);
		srcfact = GE_SF_FIX;
		break;
	case GL_SRC_ALPHA:
		srcfact = GE_SF_SRC_ALPHA;
		break;
	case GL_ONE_MINUS_SRC_ALPHA:
		srcfact = GE_SF_ONE_MINUS_SRC_ALPHA;
		break;
	case GL_DST_COLOR:
		srcfact = GE_SF_DST_COLOR;
		break;
	case GL_ONE_MINUS_DST_COLOR:
		srcfact = GE_SF_ONE_MINUS_DST_COLOR;
		break;
	case GL_DST_ALPHA:
		srcfact = GE_SF_DST_ALPHA;
		break;
	case GL_ONE_MINUS_DST_ALPHA:
		srcfact = GE_SF_ONE_MINUS_DST_ALPHA;
		break;

	case GL_SRC_ALPHA_SATURATE:
	case GL_SRC_COLOR:
	case GL_ONE_MINUS_SRC_COLOR:
	default:
		goto out_error;
	}

	__pspgl_context_writereg_masked(pspgl_curctx, CMD_BLEND_FUNC,
					(dstfact << 4) | srcfact, 0x0ff);
	return;

  out_error:
	GLERROR(GL_INVALID_ENUM);
}

