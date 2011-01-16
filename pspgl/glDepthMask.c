#include "pspgl_internal.h"


void glDepthMask (GLboolean flag)
{
	sendCommandi(CMD_DEPTH_MASK, flag ? 0 : ~0);
}

