#include "pspgl_internal.h"

void glVertex2d(GLdouble x, GLdouble y)
{
	glVertex3f(x,y, 0.0f); // @@@ modified by edorul
}

void glVertex2dv(const GLdouble *p)
{
	glVertex3f(p[0], p[1], 0.0f); // @@@ modified by edorul
}

void glVertex3d(GLdouble x, GLdouble y, GLdouble z)
{
	glVertex3f(x,y,z);
}

void glVertex3dv(const GLdouble *p)
{
	glVertex3f(p[0], p[1], p[2]);
}
