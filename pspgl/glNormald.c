#include "pspgl_internal.h"

void glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz)
{
	glNormal3f(nx, ny, nz);
}

void glNormal3dv(const GLdouble *p)
{
	glNormal3f(p[0], p[1], p[2]);
}
