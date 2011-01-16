#include "pspgl_internal.h"


void glStencilMask (GLuint mask)
{
	pspgl_curctx->write_mask.stencil = mask & pspgl_curctx->draw->stencil_mask;

	/**
	 * Alpha Channel and Stencil are shared. Only update mask register
	 * if stencil test is enabled.
	 */
	if (pspgl_curctx->hw.ge_reg[CMD_ENA_STENCIL_TEST] & 1)
	        sendCommandi(CMD_ALPHA_MASK, pspgl_curctx->write_mask.stencil);
}

