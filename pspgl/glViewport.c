#include "pspgl_internal.h"


void glViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
	struct pspgl_context *c = pspgl_curctx;
	struct pspgl_surface *s = c->draw;

	c->viewport.x = x;
	c->viewport.y = y;
	c->viewport.width = width;
	c->viewport.height = height;

	if (x < 0 || y < 0 || width < 0 || height < 0 ||
	    x+width > s->width ||
	    y+height > s->height)
	{
		GLERROR(GL_INVALID_VALUE);
		return;
	}

	/* Flip y coord */
	y = s->height - y;
	height = -height;

	/* Viewport / Screen Offset */
	sendCommandi(CMD_OFFSETX, (2048*16 - s->width*16/2));
	sendCommandi(CMD_OFFSETY, (2048*16 - s->height*16/2));

	/* Viewport Size (X/Y, Width/Height) */
	sendCommandf(CMD_VIEWPORT_SX, width * .5f);
	sendCommandf(CMD_VIEWPORT_SY, height * .5f);

	/* Viewport Center (X/Y) */
	float cx = 2048.f - (s->width * .5f) + (width * .5f) + x;
	float cy = 2048.f - (s->height * .5f) + (height * .5f) + y;
	sendCommandf(CMD_VIEWPORT_TX, cx);
	sendCommandf(CMD_VIEWPORT_TY, cy);

	/* Drawing Rectangle (unaffected by viewport) */
	sendCommandi(CMD_REGION1, (0 << 10) | 0);
	sendCommandi(CMD_REGION2, ((s->height-1) << 10) | (s->width-1));
}
