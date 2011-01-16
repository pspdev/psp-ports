#include "pspgl_internal.h"


static void enable_clientstate (GLenum array, GLboolean enable)
{
	struct varray *va = &pspgl_curctx->vertex_array;

	switch (array) {
	case GL_VERTEX_ARRAY:
		va->vertex.enabled = enable;
		break;
	case GL_COLOR_ARRAY:
		va->color.enabled = enable;
		break;
	case GL_NORMAL_ARRAY:
		va->normal.enabled = enable;
		break;
	case GL_TEXTURE_COORD_ARRAY:
		va->texcoord.enabled = enable;
		break;
	case GL_WEIGHT_ARRAY_PSP:
		va->weight.enabled = enable;
		break;
	default:
		GLERROR(GL_INVALID_ENUM);
		return;
	}

	if (va->locked.cached_array &&
	    va->locked.vfmt.arrays != __pspgl_enabled_array_bits()) {
		psp_log("array state changed %x->%x; invalidating cached arrays\n",
			va->locked.vfmt.arrays, __pspgl_enabled_array_bits());
		__pspgl_uncache_arrays();
	}
}


void glEnableClientState (GLenum array)
{
	enable_clientstate(array, GL_TRUE);
}


void glDisableClientState (GLenum array)
{
	enable_clientstate(array, GL_FALSE);
}
