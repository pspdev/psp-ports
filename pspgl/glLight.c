#include <math.h>
#include <string.h>

#include "pspgl_internal.h"
#include "pspgl_matrix.h"

static void set_light_type(struct pspgl_context *c, int light)
{
	unsigned type;
	unsigned components;
	unsigned kind;

	if (c->lights.positional[light]) {
		if (c->lights.spotlight[light])
			type = GE_SPOTLIGHT;
		else
			type = GE_POINTLIGHT;
	} else
		type = GE_DIRECTIONAL;

	components = 0;
	if (getReg(CMD_LIGHT0_AMB_COL + 3*light) & 0xffffff)
		components |= GE_AMBIENT;

	if (getReg(CMD_LIGHT0_DIF_COL + 3*light) & 0xffffff)
		components |= GE_DIFFUSE;

	if (getReg(CMD_LIGHT0_SPC_COL + 3*light) & 0xffffff)
		components |= GE_SPECULAR;

	kind = (components == (GE_SPECULAR|GE_DIFFUSE));

	psp_log("light %d, type=%d components=%d kind=%d\n", 
		light, type, components, kind);

	sendCommandi(CMD_LIGHT0_TYPE+light, (type << 8) | kind);
}

#define MAT(m,r,c) (m)[(c)*4+(r)]


/**********************************************************************/
/** \name Matrix inversion */
/*@{*/

/**
 * Swaps the values of two floating pointer variables.
 *
 * Used by invert_matrix_general() to swap the row pointers.
 */
#define SWAP_ROWS(a, b) { GLfloat *_tmp = a; (a)=(b); (b)=_tmp; }

/**
 * Compute inverse of 4x4 transformation matrix.
 * 
 * \param mat pointer to a GLmatrix structure. The matrix inverse will be
 * stored in the GLmatrix::inv attribute.
 * 
 * \return GL_TRUE for success, GL_FALSE for failure (\p singular matrix).
 * 
 * \author
 * Code contributed by Jacques Leroy jle@star.be
 *
 * Calculates the inverse matrix by performing the gaussian matrix reduction
 * with partial pivoting followed by back/substitution with the loops manually
 * unrolled.
 */
static GLboolean invert_matrix_general( const GLfloat *m, GLfloat *out )
{
   GLfloat wtmp[4][8];
   GLfloat m0, m1, m2, m3, s;
   GLfloat *r0, *r1, *r2, *r3;

   r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

   r0[0] = MAT(m,0,0), r0[1] = MAT(m,0,1),
   r0[2] = MAT(m,0,2), r0[3] = MAT(m,0,3),
   r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,

   r1[0] = MAT(m,1,0), r1[1] = MAT(m,1,1),
   r1[2] = MAT(m,1,2), r1[3] = MAT(m,1,3),
   r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,

   r2[0] = MAT(m,2,0), r2[1] = MAT(m,2,1),
   r2[2] = MAT(m,2,2), r2[3] = MAT(m,2,3),
   r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,

   r3[0] = MAT(m,3,0), r3[1] = MAT(m,3,1),
   r3[2] = MAT(m,3,2), r3[3] = MAT(m,3,3),
   r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

   /* choose pivot - or die */
   if (fabsf(r3[0])>fabsf(r2[0])) SWAP_ROWS(r3, r2);
   if (fabsf(r2[0])>fabsf(r1[0])) SWAP_ROWS(r2, r1);
   if (fabsf(r1[0])>fabsf(r0[0])) SWAP_ROWS(r1, r0);
   if (0.0 == r0[0])  return GL_FALSE;

   /* eliminate first variable     */
   m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
   s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
   s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
   s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
   s = r0[4];
   if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
   s = r0[5];
   if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
   s = r0[6];
   if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
   s = r0[7];
   if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

   /* choose pivot - or die */
   if (fabsf(r3[1])>fabsf(r2[1])) SWAP_ROWS(r3, r2);
   if (fabsf(r2[1])>fabsf(r1[1])) SWAP_ROWS(r2, r1);
   if (0.0 == r1[1])  return GL_FALSE;

   /* eliminate second variable */
   m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
   r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
   r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
   s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
   s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
   s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
   s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

   /* choose pivot - or die */
   if (fabsf(r3[2])>fabsf(r2[2])) SWAP_ROWS(r3, r2);
   if (0.0 == r2[2])  return GL_FALSE;

   /* eliminate third variable */
   m3 = r3[2]/r2[2];
   r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
   r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
   r3[7] -= m3 * r2[7];

   /* last check */
   if (0.0 == r3[3]) return GL_FALSE;

   s = 1.0F/r3[3];             /* now back substitute row 3 */
   r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

   m2 = r2[3];                 /* now back substitute row 2 */
   s  = 1.0F/r2[2];
   r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
   r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
   m1 = r1[3];
   r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
   r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
   m0 = r0[3];
   r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
   r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

   m1 = r1[2];                 /* now back substitute row 1 */
   s  = 1.0F/r1[1];
   r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
   r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
   m0 = r0[2];
   r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
   r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

   m0 = r0[1];                 /* now back substitute row 0 */
   s  = 1.0F/r0[0];
   r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
   r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

   MAT(out,0,0) = r0[4]; MAT(out,0,1) = r0[5],
   MAT(out,0,2) = r0[6]; MAT(out,0,3) = r0[7],
   MAT(out,1,0) = r1[4]; MAT(out,1,1) = r1[5],
   MAT(out,1,2) = r1[6]; MAT(out,1,3) = r1[7],
   MAT(out,2,0) = r2[4]; MAT(out,2,1) = r2[5],
   MAT(out,2,2) = r2[6]; MAT(out,2,3) = r2[7],
   MAT(out,3,0) = r3[4]; MAT(out,3,1) = r3[5],
   MAT(out,3,2) = r3[6]; MAT(out,3,3) = r3[7];

   return GL_TRUE;
}
#undef SWAP_ROWS

/*
 * Transform a point (column vector) by a matrix:   Q = M * P
 */
#define TRANSFORM_POINT( Q, M, P )					\
do {									\
   Q[0] = M[0] * P[0] + M[4] * P[1] + M[8] *  P[2] + M[12] * P[3];	\
   Q[1] = M[1] * P[0] + M[5] * P[1] + M[9] *  P[2] + M[13] * P[3];	\
   Q[2] = M[2] * P[0] + M[6] * P[1] + M[10] * P[2] + M[14] * P[3];	\
   Q[3] = M[3] * P[0] + M[7] * P[1] + M[11] * P[2] + M[15] * P[3];	\
} while(0)

/*
 * Transform a normal (row vector) by a matrix:  [NX NY NZ] = N * MAT
 */
#define TRANSFORM_NORMAL( TO, N, MAT )				\
do {								\
   TO[0] = N[0] * MAT[0] + N[1] * MAT[1] + N[2] * MAT[2];	\
   TO[1] = N[0] * MAT[4] + N[1] * MAT[5] + N[2] * MAT[6];	\
   TO[2] = N[0] * MAT[8] + N[1] * MAT[9] + N[2] * MAT[10];	\
} while (0)

void glLightfv (GLenum light, GLenum pname, const GLfloat *params)
{
	struct pspgl_context *c = pspgl_curctx;
	GLenum error;

	error = GL_INVALID_ENUM;
	if (unlikely(light < GL_LIGHT0) || unlikely(light > GL_LIGHT3))
		goto out_error;

	light -= GL_LIGHT0;

	error = GL_INVALID_VALUE;
	switch (pname) {
	case GL_AMBIENT:
		sendCommandi(CMD_LIGHT0_AMB_COL + 3*light, COLOR3(params));
		break;
	case GL_DIFFUSE:
		sendCommandi(CMD_LIGHT0_DIF_COL + 3*light, COLOR3(params));
		break;
	case GL_SPECULAR:
		sendCommandi(CMD_LIGHT0_SPC_COL + 3*light, COLOR3(params));
		break;

	case GL_POSITION: {
		const struct pspgl_matrix_stack *mv = &c->modelview_stack;
		float eye[4];

		__pspgl_matrix_sync(c, mv);

		/* Transform light into eye-space using the current
		   modelview matrix.  This is done once at
		   specification time. */
		TRANSFORM_POINT(eye, mv->stack[mv->depth].mat, params);

		c->lights.positional[light] = (eye[3] != 0.);

		sendCommandf(CMD_LIGHT0_POS_X + 3*light, eye[0]);
		sendCommandf(CMD_LIGHT0_POS_Y + 3*light, eye[1]);
		sendCommandf(CMD_LIGHT0_POS_Z + 3*light, eye[2]);
		break;
	}

	case GL_SPOT_DIRECTION: {
		const struct pspgl_matrix_stack *mv = &c->modelview_stack;
		float inv[16];
		float eye[4];
		float vec[3];

		vec[0] = -params[0];
		vec[1] = -params[1];
		vec[2] = -params[2];

		__pspgl_matrix_sync(c, mv);

		memcpy(inv, __pspgl_identity, sizeof(inv));
		invert_matrix_general(mv->stack[mv->depth].mat, inv);
		TRANSFORM_NORMAL(eye, vec, inv);

		sendCommandf(CMD_LIGHT0_VEC_X + 3*light, eye[0]);
		sendCommandf(CMD_LIGHT0_VEC_Y + 3*light, eye[1]);
		sendCommandf(CMD_LIGHT0_VEC_Z + 3*light, eye[2]);
		break;
	}

	case GL_SPOT_EXPONENT:
		if (params[0] < 0 || params[0] > 128)
			goto out_error;
		sendCommandf(CMD_LIGHT0_SPOT_EXP+light, params[0]);
		break;
	case GL_SPOT_CUTOFF:
		if (params[0] != 180.f && !(params[0] >= 0.f && params[0] <= 90.f))
			goto out_error;;
		c->lights.spotlight[light] = (params[0] != 180.);
		sendCommandf(CMD_LIGHT0_CUTOFF+light, cosf(params[0] * M_PI / 180.f));
		break;

	case GL_CONSTANT_ATTENUATION:
		sendCommandf(CMD_LIGHT0_ATT_CONST+3*light, params[0]);
		break;
	case GL_LINEAR_ATTENUATION:
		sendCommandf(CMD_LIGHT0_ATT_LINEAR+3*light, params[0]);
		break;
	case GL_QUADRATIC_ATTENUATION:
		sendCommandf(CMD_LIGHT0_ATT_QUAD+3*light, params[0]);
		break;
	default:
		error = GL_INVALID_ENUM;
		goto out_error;
	}

	set_light_type(c, light);	/* update light types, if needed */

	return;

  out_error:
	GLERROR(error);
}


void glLightf (GLenum light, GLenum pname, GLfloat param)
{
	glLightfv(light, pname, &param);
}

void glLighti (GLenum light, GLenum pname, GLint param)
{
	glLightf(light, pname, param);
}

