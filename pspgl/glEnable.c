#include "pspgl_internal.h"

void __pspgl_enable_state (GLenum cap, GLboolean enable)
{
	int i;
	unsigned char opcode = 0;

	switch (cap) {
	case GL_FOG:
		opcode = CMD_ENA_FOG;
		break;
	case GL_LIGHTING: {
		unsigned long c;
		opcode = CMD_ENA_LIGHTING;
		
		/* switch material ambient around depending on lighting state */
		if (enable)
			c = pspgl_curctx->material.ambient;
		else 
			c = pspgl_curctx->current.color;
		sendCommandi(CMD_MATERIAL_AMB_C, c);
		sendCommandi(CMD_MATERIAL_AMB_A, c>>24);
		break;
		}
	case GL_TEXTURE_2D:
		opcode = CMD_ENA_TEXTURE;
		break;
	case GL_CULL_FACE:
		opcode = CMD_ENA_CULL;
		break;
	case GL_ALPHA_TEST:
		opcode = CMD_ENA_ALPHA_TEST;
		break;
	case GL_BLEND:
		opcode = CMD_ENA_BLEND;
		break;
	case GL_COLOR_LOGIC_OP:
		opcode = CMD_ENA_LOGIC;
		break;
	case GL_DITHER:
		opcode = CMD_ENA_DITHER;
		break;
	case GL_STENCIL_TEST:
		opcode = CMD_ENA_STENCIL_TEST;
		enable = enable && pspgl_curctx->draw->stencil_mask;
		if (enable)
			sendCommandi(CMD_ALPHA_MASK, ~pspgl_curctx->write_mask.stencil);
		else
			sendCommandi(CMD_ALPHA_MASK, ~pspgl_curctx->write_mask.alpha);
		break;
	case GL_DEPTH_TEST:
		opcode = CMD_ENA_DEPTH_TEST;
		enable = enable && pspgl_curctx->draw->depth_buffer;
		break;
	case GL_LIGHT0:
	case GL_LIGHT1:
	case GL_LIGHT2:
	case GL_LIGHT3:
		opcode = cap - GL_LIGHT0 + CMD_ENA_LIGHT0;
		break;
	case GL_LINE_SMOOTH:
	case GL_POINT_SMOOTH:
		opcode = CMD_ENA_ANTIALIAS;	/* XXX : antialiasing. both line and point? */
		break;
	case GL_SCISSOR_TEST:
		pspgl_curctx->scissor_test.enabled = enable;
		__pspgl_set_scissor();
		return;

	case GL_VERTEX_BLEND_PSP:
		/* There's no specific enable flag for vertex
		   blending, but disabling the matricies (=identity)
		   should have the same effect. */
		pspgl_curctx->vertexblend.enabled = enable;
		for(i = 0; i < NBONES; i++) {
			struct pspgl_matrix_stack *m = &pspgl_curctx->bone_stacks[i];

			m->flags = (m->flags & ~MF_DISABLED) |
				(enable ? 0 : MF_DISABLED) | MF_DIRTY;
		}
		break;

	case GL_TEXTURE_GEN_S:
	case GL_TEXTURE_GEN_T:
		/* TODO */
		break;
	case GL_NORMALIZE:
		/* ? */
		break;

	case GL_COLOR_MATERIAL:
	case GL_RESCALE_NORMAL:
	case GL_POLYGON_OFFSET_FILL:
	case GL_VERTEX_ARRAY:
	case GL_NORMAL_ARRAY:
	case GL_COLOR_ARRAY:
	case GL_TEXTURE_COORD_ARRAY:
	case GL_MULTISAMPLE:
	case GL_SAMPLE_ALPHA_TO_COVERAGE:
	case GL_SAMPLE_ALPHA_TO_ONE:
	case GL_SAMPLE_COVERAGE:
	default:
		GLERROR(GL_INVALID_ENUM);
		return;
	}

	if (opcode)
		sendCommandi(opcode, enable);
}


void glEnable (GLenum cap)
{
	__pspgl_enable_state(cap, 1);
}


void glDisable (GLenum cap)
{
	__pspgl_enable_state(cap, 0);
}

