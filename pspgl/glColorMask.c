#include "pspgl_internal.h"


void glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	unsigned long mask = red ? 0xffff00 : 0xffffff;

	if (green)
		mask &= 0xff00ff;

	if (blue)
		mask &= 0x00ffff;

	pspgl_curctx->write_mask.alpha = alpha ? pspgl_curctx->draw->alpha_mask : 0x00;

        sendCommandi(CMD_RGB_MASK, mask);

	/**
	 * Alpha Channel and Stencil are shared. Only update Alpha mask register
	 * if stencil test is disabled.
	 */
	if ((pspgl_curctx->hw.ge_reg[CMD_ENA_STENCIL_TEST] & 1) == 0)
	        sendCommandi(CMD_ALPHA_MASK, ~pspgl_curctx->write_mask.alpha);
}

