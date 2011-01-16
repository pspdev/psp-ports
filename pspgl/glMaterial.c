#include "pspgl_internal.h"


void glMaterialfv (GLenum face, GLenum pname, const GLfloat *params)
{
	switch (pname) {
	case GL_AMBIENT:
		pspgl_curctx->material.ambient = COLOR4(params);

		/* material ambient is overloaded by glColor() */
		if (getReg(CMD_ENA_LIGHTING) & 0xff) {
			sendCommandi(CMD_MATERIAL_AMB_C, pspgl_curctx->material.ambient);
			sendCommandi(CMD_MATERIAL_AMB_A, pspgl_curctx->material.ambient >> 24);
		}
		break;
	case GL_EMISSION:
		sendCommandi(CMD_MATERIAL_EMS_C, COLOR3(params));
		break;
	case GL_DIFFUSE:
		sendCommandi(CMD_MATERIAL_DIF_C, COLOR3(params));
		break;
	case GL_AMBIENT_AND_DIFFUSE:
		glMaterialfv(face, GL_AMBIENT, params);
		glMaterialfv(face, GL_DIFFUSE, params);
		break;
	case GL_SPECULAR:
		sendCommandi(CMD_MATERIAL_SPC_C, COLOR3(params));
		break;
	case GL_SHININESS:
		sendCommandf(CMD_MATERIAL_SPEC_POW, params[0]);
		break;
	default:
		GLERROR(GL_INVALID_ENUM);
		return;
	}

#if 0
	unsigned components;

	/* glColorMaterial? */
	components = 0;
	if (pspgl_curctx->material.ambient)
		components |= GE_AMBIENT;
	if ((getReg(CMD_MATERIAL_SPC_C) & 0xffffff) != 0)
		components |= GE_SPECULAR;
	if ((getReg(CMD_MATERIAL_DIF_C) & 0xffffff) != 0)
		components |= GE_DIFFUSE;
	sendCommandi(CMD_MATERIAL, components);
#endif
}


void glMaterialf (GLenum face, GLenum pname, GLfloat param)
{
	glMaterialfv(face, pname, &param);
}

void glMateriali (GLenum face, GLenum pname, GLint param)
{
	glMaterialf(face, pname, param);
}

