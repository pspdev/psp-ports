#include <GLES/gl.h>
#include "pspgl_internal.h"

void glOrthof (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
{
	GLfloat m [16];
	GLfloat rml = right - left;
	GLfloat fmn = far - near;
	GLfloat tmb = top - bottom;
	GLfloat _1over_rml;
	GLfloat _1over_fmn;
	GLfloat _1over_tmb;

	if (rml == 0.0 || fmn == 0.0 || tmb == 0.0) {
		GLERROR(GL_INVALID_VALUE);
		return;
	}

	_1over_rml = 1.0 / rml;
	_1over_fmn = 1.0 / fmn;
	_1over_tmb = 1.0 / tmb;

	m[0] = 2.0 * _1over_rml;
	m[1] = 0.0;
	m[2] = 0.0;
	m[3] = 0.0;

	m[4] = 0.0;
	m[5] = 2.0 * _1over_tmb;
	m[6] = 0.0;
	m[7] = 0.0;

	m[8] = 0.0;
	m[9] = 0.0;
	m[10] = -2.0 * _1over_fmn;
	m[11] = 0.0;

	m[12] = -(right + left) *  _1over_rml;
	m[13] = -(top + bottom) *  _1over_tmb;
	m[14] = -(far + near) * _1over_fmn;
	m[15] = 1.0;

	glMultMatrixf(m);
}


