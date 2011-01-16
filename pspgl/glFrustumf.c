#include <GLES/gl.h>

#include "pspgl_internal.h"


void glFrustumf (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
{
	GLfloat m [16];
	GLfloat _2n = 2.0 * near;
	GLfloat _1over_rml = 1.0 / (right - left);
	GLfloat _1over_fmn = 1.0 / (far - near);
	GLfloat _1over_tmb = 1.0 / (top - bottom);

	m[0] = _2n * _1over_rml;
	m[1] = 0.0;
	m[2] = 0.0;
	m[3] = 0.0;

	m[4] = 0.0;
	m[5] = _2n * _1over_tmb;
	m[6] = 0.0;
	m[7] = 0.0;

	m[8] = (right + left) * _1over_rml;
	m[9] = (top + bottom) * _1over_tmb;
	m[10] = (-(far + near)) * _1over_fmn;
	m[11] = -1.0;

	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = -(_2n * far * _1over_fmn);
	m[15] = 0.0;

	glMultMatrixf(m);
}

