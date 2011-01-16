#include "pspgl_internal.h"


extern void glOrthof (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);


void glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	glOrthof(left, right, bottom, top, zNear, zFar);
}

