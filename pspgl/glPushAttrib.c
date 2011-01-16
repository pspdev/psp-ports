#include <stdlib.h>
#include <string.h>

#include "pspgl_internal.h"
#include "pspgl_texobj.h"
#include "pspgl_matrix.h"

struct pspgl_saved_attrib
{
	GLbitfield attrmask;

	struct clear clear;
	struct current current;
	struct scissor_test scissor_test;
	struct lights lights;
	struct material material;
	struct polygon polygon;
	struct write_mask write_mask;
	struct texture texture;
	struct pspgl_matrix_stack *current_matrix_stack;
	struct viewport viewport;
	struct vertexblend vertexblend;

	uint32_t regs[256];
	uint32_t regmask[256/32];
};

static void save_regs(const struct pspgl_context *c, 
		      struct pspgl_saved_attrib *a, 
		      const unsigned char *regs)
{
	unsigned char reg;

	psp_log("saving regs:\n");
	while((reg = *regs++)) {
		psp_log("  save_regs: %02x %3d = %08x\n", reg, reg, c->hw.ge_reg[reg]);
		a->regs[reg] = c->hw.ge_reg[reg];
		a->regmask[reg / 32] |= 1 << (reg % 32);
	}
}

static void restore_regs(struct pspgl_context *c,
			 const struct pspgl_saved_attrib *a)
{
	unsigned i;

	for(i = 0; i < 256; i += 32) {
		uint32_t word = a->regmask[i/32];
		unsigned j;

		for(j = i; word != 0; j++, word >>= 1) {
			if (word & 1) {
				psp_log("restoring reg %02x %3d = %06x\n",
					    j, j, a->regs[j]);
				__pspgl_context_writereg(c, j, a->regs[j]);
			}
		}
	}
}

void glPushAttrib( GLbitfield mask )
{
	struct pspgl_context *c = pspgl_curctx;
	struct pspgl_saved_attrib *a;
	GLenum error;

	error = GL_STACK_OVERFLOW;
	if (unlikely(c->attribstackdepth >= MAX_ATTRIB_STACK))
		goto out_error;

	a = malloc(sizeof(*a));
	error = GL_OUT_OF_MEMORY;
	if (a == NULL)
		goto out_error;

	memset(a, 0, sizeof(*a));

	a->attrmask = mask;

	/* GL_ACCUM_BUFFER_BIT */

	if (mask & GL_COLOR_BUFFER_BIT) {
		static const unsigned char col_regs[] = {
			CMD_ENA_ALPHA_TEST,
			CMD_ALPHA_FUNC,
			CMD_ENA_BLEND,
			CMD_BLEND_FUNC,
			CMD_FIXEDCOL_SRC,
			CMD_FIXEDCOL_DST,
			CMD_ENA_DITHER,
			CMD_ENA_LOGIC,
			CMD_LOGICOP,
			CMD_RGB_MASK,
			CMD_ALPHA_MASK,
			CMD_DRAWBUF,
			CMD_DRAWBUFWIDTH,
			CMD_PSM,

			0
		};

		save_regs(c, a, col_regs);
		a->clear.color = c->clear.color;
		a->write_mask.alpha = c->write_mask.alpha;
	}

	if (mask & GL_CURRENT_BIT)
		a->current = c->current;

	if (mask & GL_DEPTH_BUFFER_BIT) {
		static const unsigned char depth_regs[] = {
			CMD_ENA_DEPTH_TEST,
			CMD_DEPTH_FUNC,
			CMD_DEPTH_MASK,
			CMD_DEPTHBUF,
			CMD_DEPTHBUFWIDTH,
			0
		};

		save_regs(c, a, depth_regs);
		a->clear.depth = c->clear.depth;
	}

	if (mask & GL_ENABLE_BIT) {
		static const unsigned char ena_regs[] = {
			CMD_ENA_LIGHTING,
			CMD_ENA_LIGHT0,
			CMD_ENA_LIGHT1,
			CMD_ENA_LIGHT2,
			CMD_ENA_LIGHT3,
			CMD_ENA_CLIP,
			CMD_ENA_CULL,
			CMD_ENA_TEXTURE,
			CMD_ENA_FOG,
			CMD_ENA_DITHER,
			CMD_ENA_BLEND,
			CMD_ENA_ALPHA_TEST,
			CMD_ENA_DEPTH_TEST,
			CMD_ENA_STENCIL_TEST,
			CMD_ENA_ANTIALIAS,
			CMD_ENA_PATCH_CULL,
			CMD_ENA_COLOR_TEST,
			CMD_ENA_LOGIC,
			0
		};
		save_regs(c, a, ena_regs);

		a->scissor_test.enabled = c->scissor_test.enabled;
		a->vertexblend = c->vertexblend;
	}

	/* GL_EVAL_BIT */

	if (mask & GL_FOG_BIT) {
		static const unsigned char fog_regs[] = {
			CMD_FOG_NEAR,
			CMD_FOG_FAR,
			CMD_FOG_COLOR,
			0
		};

		save_regs(c, a, fog_regs);
	}

	/* GL_HINT_BIT */

	if (mask & GL_LIGHTING_BIT) {
		static const unsigned char light_regs[] = {
			CMD_ENA_LIGHTING,
			CMD_ENA_LIGHT0,
			CMD_ENA_LIGHT1,
			CMD_ENA_LIGHT2,
			CMD_ENA_LIGHT3,

			CMD_MATERIAL,
			CMD_MATERIAL_EMS_C,
			CMD_MATERIAL_AMB_C,
			CMD_MATERIAL_DIF_C,
			CMD_MATERIAL_SPC_C,
			CMD_MATERIAL_AMB_A,
			CMD_MATERIAL_SPEC_POW,

			CMD_AMBIENT_COLOR,
			CMD_AMBIENT_ALPHA,
			CMD_LIGHTMODEL,

			CMD_LIGHT0_TYPE,
			CMD_LIGHT1_TYPE,
			CMD_LIGHT2_TYPE,
			CMD_LIGHT3_TYPE,

			CMD_LIGHT0_POS_X,
			CMD_LIGHT0_POS_Y,
			CMD_LIGHT0_POS_Z,
			CMD_LIGHT1_POS_X,
			CMD_LIGHT1_POS_Y,
			CMD_LIGHT1_POS_Z,
			CMD_LIGHT2_POS_X,
			CMD_LIGHT2_POS_Y,
			CMD_LIGHT2_POS_Z,
			CMD_LIGHT3_POS_X,
			CMD_LIGHT3_POS_Y,
			CMD_LIGHT3_POS_Z,
			CMD_LIGHT0_VEC_X,
			CMD_LIGHT0_VEC_Y,
			CMD_LIGHT0_VEC_Z,
			CMD_LIGHT1_VEC_X,
			CMD_LIGHT1_VEC_Y,
			CMD_LIGHT1_VEC_Z,
			CMD_LIGHT2_VEC_X,
			CMD_LIGHT2_VEC_Y,
			CMD_LIGHT2_VEC_Z,
			CMD_LIGHT3_VEC_X,
			CMD_LIGHT3_VEC_Y,
			CMD_LIGHT3_VEC_Z,
			CMD_LIGHT0_ATT_CONST,
			CMD_LIGHT0_ATT_LINEAR,
			CMD_LIGHT0_ATT_QUAD,
			CMD_LIGHT1_ATT_CONST,
			CMD_LIGHT1_ATT_LINEAR,
			CMD_LIGHT1_ATT_QUAD,
			CMD_LIGHT2_ATT_CONST,
			CMD_LIGHT2_ATT_LINEAR,
			CMD_LIGHT2_ATT_QUAD,
			CMD_LIGHT3_ATT_CONST,
			CMD_LIGHT3_ATT_LINEAR,
			CMD_LIGHT3_ATT_QUAD,

			CMD_LIGHT0_SPOT_EXP,
			CMD_LIGHT1_SPOT_EXP,
			CMD_LIGHT2_SPOT_EXP,
			CMD_LIGHT3_SPOT_EXP,

			CMD_LIGHT0_CUTOFF,
			CMD_LIGHT1_CUTOFF,
			CMD_LIGHT2_CUTOFF,
			CMD_LIGHT3_CUTOFF,

			CMD_LIGHT0_AMB_COL,
			CMD_LIGHT0_DIF_COL,
			CMD_LIGHT0_SPC_COL,
			CMD_LIGHT1_AMB_COL,
			CMD_LIGHT1_DIF_COL,
			CMD_LIGHT1_SPC_COL,
			CMD_LIGHT2_AMB_COL,
			CMD_LIGHT2_DIF_COL,
			CMD_LIGHT2_SPC_COL,
			CMD_LIGHT3_AMB_COL,
			CMD_LIGHT3_DIF_COL,
			CMD_LIGHT3_SPC_COL,

			0
		};

		save_regs(c, a, light_regs);

		a->lights = c->lights;
		a->material = c->material;
	}

	/* GL_LINE_BIT */
	/* GL_LIST_BIT */
	/* GL_PIXEL_MODE_BIT */
	/* GL_POINT_BIT */

	if (mask & GL_POLYGON_BIT) {
		static const unsigned char polygon_regs[] = {
			CMD_ENA_CULL,
			CMD_ENA_PATCH_CULL,
			CMD_PATCH_CULL,
			CMD_CULL_FACE,
			0
		};
		save_regs(c, a, polygon_regs);
		a->polygon = c->polygon;
	}

	/* GL_POLYGON_STIPPLE_BIT */

	if (mask & GL_SCISSOR_BIT) {
		static const unsigned char scissor_regs[] = {
			CMD_SCISSOR1,
			CMD_SCISSOR2,

			0
		};
		save_regs(c, a, scissor_regs);

		a->scissor_test = c->scissor_test;
	}

	if (mask & GL_STENCIL_BUFFER_BIT) {
		static const unsigned char stencil_regs[] = {
			CMD_ENA_STENCIL_TEST,
			CMD_STENCIL_FUNC,
			CMD_STENCIL_OP,

			0
		};
		save_regs(c, a, stencil_regs);

		a->write_mask.stencil = c->write_mask.stencil;
		a->clear.stencil = c->clear.stencil;
	}

	if (mask & GL_TEXTURE_BIT) {
		static const unsigned char texture_regs[] = {
			CMD_ENA_TEXTURE,

			CMD_TEXTURE_SU,
			CMD_TEXTURE_SV,
			CMD_TEXTURE_TU,
			CMD_TEXTURE_TV,

			CMD_TEX_MIPMAP0,
			CMD_TEX_MIPMAP1,
			CMD_TEX_MIPMAP2,
			CMD_TEX_MIPMAP3,
			CMD_TEX_MIPMAP4,
			CMD_TEX_MIPMAP5,
			CMD_TEX_MIPMAP6,
			CMD_TEX_MIPMAP7,

			CMD_TEX_STRIDE0,
			CMD_TEX_STRIDE1,
			CMD_TEX_STRIDE2,
			CMD_TEX_STRIDE3,
			CMD_TEX_STRIDE4,
			CMD_TEX_STRIDE5,
			CMD_TEX_STRIDE6,
			CMD_TEX_STRIDE7,

			CMD_SET_CLUT,
			CMD_SET_CLUT_MSB,

			CMD_TEX_SIZE0,
			CMD_TEX_SIZE1,
			CMD_TEX_SIZE2,
			CMD_TEX_SIZE3,
			CMD_TEX_SIZE4,
			CMD_TEX_SIZE5,
			CMD_TEX_SIZE6,
			CMD_TEX_SIZE7,

			CMD_TEXMAPMODE,
			CMD_TEXENVMAPMAT,
			CMD_TEXMODE,
			CMD_TEXFMT,

			CMD_CLUT_MODE,

			CMD_TEXFILT,
			CMD_TEXWRAP,
			CMD_TEXBIAS,

			CMD_TEXENV_FUNC,
			CMD_TEXENV_COL,
			CMD_TEXSLOPE,

			0
		};
		save_regs(c, a, texture_regs);
		a->texture.bound = c->texture.bound;

		if (a->texture.bound)
			a->texture.bound->refcount++;
	}

	if (mask & GL_TRANSFORM_BIT) {
		a->current_matrix_stack = c->current_matrix_stack;
		a->vertexblend = c->vertexblend;
	}

	if (mask & GL_VIEWPORT_BIT) {
		static const unsigned char viewport_regs[] = {
			CMD_OFFSETX,
			CMD_OFFSETY,
			CMD_VIEWPORT_SX,
			CMD_VIEWPORT_SY,
			CMD_VIEWPORT_SZ,
			CMD_VIEWPORT_TX,
			CMD_VIEWPORT_TY,
			CMD_VIEWPORT_TZ,
			CMD_REGION1,
			CMD_REGION2,
			CMD_CLIP_NEAR,
			CMD_CLIP_FAR,

			0
		};		
		save_regs(c, a, viewport_regs);

		a->viewport = c->viewport;
	}

	c->attribstack[c->attribstackdepth++] = a;
	return;

  out_error:
	GLERROR(error);
}

void glPopAttrib( void )
{
	struct pspgl_context *c = pspgl_curctx;
	struct pspgl_saved_attrib *a;
	unsigned mask;

	if (c->attribstackdepth == 0)
		goto out_error;

	a = c->attribstack[--c->attribstackdepth];

	restore_regs(c, a);

	mask = a->attrmask;

	if (mask & GL_COLOR_BUFFER_BIT) {
		c->clear.color = a->clear.color;
		c->write_mask.alpha = a->write_mask.alpha;
	}

	if (mask & GL_CURRENT_BIT)
		c->current = a->current;

	if (mask & GL_DEPTH_BUFFER_BIT)
		c->clear.depth = a->clear.depth;

	if (mask & GL_ENABLE_BIT) {
		__pspgl_enable_state(GL_STENCIL_TEST,
				     a->regs[CMD_ENA_STENCIL_TEST] & 0xff);
		__pspgl_enable_state(GL_DEPTH_TEST,
				     a->regs[CMD_ENA_DEPTH_TEST] & 0xff);
		__pspgl_enable_state(GL_LIGHTING,
				     a->regs[CMD_ENA_LIGHTING] & 0xff);

		__pspgl_enable_state(GL_SCISSOR_TEST, a->scissor_test.enabled);
		__pspgl_enable_state(GL_VERTEX_BLEND_PSP, a->vertexblend.enabled);
	}

	if (mask & GL_FOG_BIT)
		;		/* just registers */

	if (mask & GL_LIGHTING_BIT) {
		__pspgl_enable_state(GL_LIGHTING,
				     a->regs[CMD_ENA_LIGHTING] & 0xff);
		c->lights = a->lights;
		c->material = a->material;
	}

	if (mask & GL_POLYGON_BIT)
		c->polygon = a->polygon;

	if (mask & GL_SCISSOR_BIT)
		c->scissor_test = a->scissor_test;

	if (mask & GL_STENCIL_BUFFER_BIT) {
		c->write_mask.stencil = a->write_mask.stencil;
		c->clear.stencil = a->clear.stencil;
	}

	if (mask & GL_TEXTURE_BIT) {
		if (c->texture.bound)
			__pspgl_texobj_free(c->texture.bound);
		c->texture.bound = a->texture.bound;

		/* make sure texture cache is flushed */
		__pspgl_context_writereg(c, CMD_TEXCACHE_FLUSH,
					 a->regs[CMD_TEXCACHE_FLUSH]+1);

		c->hw.dirty |= HWD_CLUT;
	}

	if (mask & GL_TRANSFORM_BIT) {
		__pspgl_matrix_select(c, a->current_matrix_stack);
		__pspgl_enable_state(GL_VERTEX_BLEND_PSP, a->vertexblend.enabled);
	}

	if (mask & GL_VIEWPORT_BIT)
		c->viewport = a->viewport;

	free(a);
	return;

  out_error:
	GLERROR(GL_STACK_UNDERFLOW);
}
