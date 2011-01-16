#include "pspgl_internal.h"


void glLogicOp (GLenum opcode)
{
	if (unlikely(opcode < GL_CLEAR) || unlikely(opcode > GL_SET))
		goto out_error;

	opcode &= 0x000f;
	sendCommandi(CMD_LOGICOP, opcode);
	return;

out_error:
	GLERROR(GL_INVALID_ENUM);
}

