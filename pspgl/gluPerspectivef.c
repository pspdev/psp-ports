#include <math.h>
#include "pspgl_internal.h"


void gluPerspectivef (GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
        GLfloat f = 1.0f / tanf(fovy * (M_PI/360.0));
	GLfloat m [16];

	m[0] = f / aspect;
	m[1] = 0.0;
	m[2] = 0.0;
	m[3] = 0.0;

	m[4] = 0.0;
	m[5] = f;
	m[6] = 0.0;
	m[7] = 0.0;

	m[8] = 0.0;
	m[9] = 0.0;
	m[10] = (zFar + zNear) / (zNear - zFar);
	m[11] = -1.0;

	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 2.0 * zFar * zNear / (zNear - zFar);
	m[15] = 0.0;

	glMultMatrixf(m);
}
