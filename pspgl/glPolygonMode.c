#include "pspgl_internal.h"


void glPolygonMode (GLenum face, GLenum mode)
{
	if (mode != GL_FILL)
		GLERROR(GL_INVALID_ENUM);
}

