#include "pspgl_internal.h"


void glReadBuffer(GLenum mode)
{
	struct pspgl_surface *s = pspgl_curctx->read;

	switch (mode) {
	case GL_BACK:
		s->read = &s->color_back;
		break;
	case GL_FRONT:
		s->read = &s->color_front;
		break;
	default:
		/* XXX IMPROVE: support front & aux buffers */
		GLERROR(GL_INVALID_ENUM);
	}
}


