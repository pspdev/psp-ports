#include "pspgl_internal.h"
#include <GL/glu.h>

extern
void gluLookAtf (GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ,
                 GLfloat centerX, GLfloat centerY, GLfloat centerZ,
                 GLfloat upX, GLfloat upY, GLfloat upZ);


void gluLookAt (GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
		GLdouble centerX, GLdouble centerY, GLdouble centerZ,
		GLdouble upX, GLdouble upY, GLdouble upZ)
{
	gluLookAtf(eyeX, eyeY, eyeZ,
		   centerX, centerY, centerZ,
		   upX, upY, upZ);
}

