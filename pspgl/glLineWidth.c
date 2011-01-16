#include "pspgl_internal.h"


void glLineWidth (GLfloat width)
{
	if (width <= 0.f)
		GLERROR(GL_INVALID_VALUE);
}

