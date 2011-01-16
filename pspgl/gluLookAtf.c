#include <math.h>
#include "pspgl_internal.h"
#include "pspgl_matrix.h"

static inline
void cross_product (GLfloat a [3], GLfloat b [3], GLfloat product [3])
{
	product[0] = a[1] * b[2] - a[2] * b[1];
	product[1] = a[2] * b[0] - a[0] * b[2];
	product[2] = a[0] * b[1] - a[1] * b[0];
}


static inline
void normalize (GLfloat v [3])
{
	GLfloat len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	if (len != 0.0) {
		GLfloat scale = 1.0f / len;
		v[0] *= scale;
		v[1] *= scale;
		v[2] *= scale;
	}
}


/**
 *  Variable names like in 'man gluLookAt'
 */
void gluLookAtf (GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ,
		 GLfloat centerX, GLfloat centerY, GLfloat centerZ,
		 GLfloat upX, GLfloat upY, GLfloat upZ)
{
#if 0
	GLfloat m [16];
	GLfloat f [3];
	GLfloat u [3];
	GLfloat s [3];

	f[0] = centerX - eyeX;
	f[1] = centerY - eyeY;
	f[2] = centerZ - eyeZ;

	u[0] = upX;
	u[1] = upY;
	u[2] = upZ;

	normalize(f);
	cross_product(f, u, s);
	normalize(s);
	cross_product(s, f, u);

	m[0] =  s[0]; m[4] =  s[1]; m[8] =   s[2]; m[12] = 0.0;
	m[1] =  u[0]; m[5] =  u[1]; m[9] =   u[2]; m[13] = 0.0;
	m[2] = -f[0]; m[6] = -f[1]; m[10] = -f[2]; m[14] = 0.0;
	m[3] =   0.0; m[7] =   0.0; m[11] =   0.0; m[15] = 1.0;

	glMultMatrixf(m);
	glTranslatef(-eyeX, -eyeY, -eyeZ);
#else
	struct pspgl_context *c = pspgl_curctx;
	struct pspgl_matrix_stack *stk = c->current_matrix_stack;
	struct pspgl_matrix *mat = c->current_matrix;

	pspvfpu_use_matrices(c->vfpu_context, VFPU_STACKTOP, VMAT5 | VMAT6);

	assert(stk->flags & MF_VFPU);

	if (!(stk->flags & MF_DISABLED))
		stk->flags |= MF_DIRTY;

	mat->flags &= ~MF_IDENTITY;

	/* 
	   Matrix 5:
	   0: sX sY sZ 1/|s|
	   1: uX uY uZ -
	   2: fX fY fZ 1/|f|
	 */
	asm volatile("mtv	%0, s501\n" : : "r" (upX));
	asm volatile("mtv	%0, s511\n" : : "r" (upY));
	asm volatile("mtv	%0, s521\n" : : "r" (upZ));

	asm volatile("mtv	%0, s502\n" : : "r" (centerX - eyeX));
	asm volatile("mtv	%0, s512\n" : : "r" (centerY - eyeY));
	asm volatile("mtv	%0, s522\n" : : "r" (centerZ - eyeZ));

	asm volatile("vmmov.t	m600, m700\n"		// prepare for multiply

		     "vdot.t	s532, r502, r502\n"	// t = fx*fx + fy*fy + fz*fz
		     "vrsq.s	s532, s532\n"		// t = 1/sqrt(t)
		     "vscl.t	r502[-1:1,-1:1,-1:1], r502, s532\n"	// normalize f

		     "vcrsp.t	r500, r502, r501\n"	// s = f x u
		     "vdot.t	s530, r500, r500\n"	// t = sx*sx+sy*sy+sz*sz
		     "vrsq.s	s530, s530\n"		// t = 1/sqrt(t)
		     "vscl.t	r500[-1:1,-1:1,-1:1], r500, s530\n"	// normalize s

		     "vcrsp.t	r501, r500, r502\n"	// u = s x f

		     "vneg.t	r502, r502\n"		// f = -f

		     "vmmul.t	m700, m600, m500\n"	// top *= mat5
		);

	glTranslatef(-eyeX, -eyeY, -eyeZ);
#endif
}

