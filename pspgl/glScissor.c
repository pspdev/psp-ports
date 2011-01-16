#include "pspgl_internal.h"

void __pspgl_set_scissor(void)
{
	int t,b,l,r;
	struct pspgl_context *c = pspgl_curctx;

	if (c->scissor_test.enabled) {
		b = c->draw->height - c->scissor_test.y;
		t = b - c->scissor_test.height;
		l = c->scissor_test.x;
		r = l + c->scissor_test.width;
	} else {
		b = c->draw->height;
		t = 0;
		l = 0;
		r = c->draw->width;
	}

	sendCommandi(CMD_SCISSOR1, (t << 10) | l);
	sendCommandi(CMD_SCISSOR2, ((b - 1) << 10) | (r - 1));
}

void glScissor (GLint x, GLint y, GLsizei width, GLsizei height)
{
	pspgl_curctx->scissor_test.x = x;
	pspgl_curctx->scissor_test.y = y;
	pspgl_curctx->scissor_test.width = width;
	pspgl_curctx->scissor_test.height = height;

	__pspgl_set_scissor();
}

