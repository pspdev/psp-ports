#include "pspgl_internal.h"

void glDepthRangef (GLclampf zNear, GLclampf zFar);

void glDepthRange (GLclampd zNear, GLclampd zFar)
{
	glDepthRangef(zNear, zFar);
}

