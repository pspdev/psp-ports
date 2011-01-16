#include "pspgl_internal.h"
#include "pspgl_buffers.h"

void glDrawBuffer(GLenum mode)
{
	struct pspgl_surface *s = pspgl_curctx->draw;

	switch (mode) {
	case GL_BACK:
		s->draw = &s->color_back;
		break;
	case GL_FRONT:
		s->draw = &s->color_front;
		break;
	default:
		/* XXX IMPROVE: support front & aux buffers */
		GLERROR(GL_INVALID_ENUM);
		return;
	}

	unsigned adr = (unsigned long) (*s->draw)->base;
	sendCommandi(CMD_DRAWBUF, (adr & 0x00ffffff));
	sendCommandi(CMD_DRAWBUFWIDTH, ((adr & 0xff000000) >> 8) | s->pixelperline);
}

