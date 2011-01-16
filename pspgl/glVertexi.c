#include "pspgl_internal.h"


void glVertex3i (GLint x, GLint y, GLint z)
{
	glVertex3f(x, y, z);
}


void glVertex2i (GLint x, GLint y)
{
	glVertex3f(x, y, 0.0);
}

