#include "pspgl_internal.h"

void glVertex2d(GLdouble x, GLdouble y)
{
	glVertex2f(x,y);
}

void glVertex2dv(const GLdouble *p)
{
	glVertex2f(p[0], p[1]);
}

void glVertex3d(GLdouble x, GLdouble y, GLdouble z)
{
	glVertex3f(x,y,z);
}

void glVertex3dv(const GLdouble *p)
{
	glVertex3f(p[0], p[1], p[2]);
}
