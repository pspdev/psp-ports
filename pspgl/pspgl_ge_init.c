#include <string.h>
#include "pspgl_internal.h"

/* This sets the inital value of the register set.  It initializes
   every useful value register (it does not touch registers which
   trigger an action like drawing a primitive), because its also used
   to build the set of registers which need to be updated on a context
   switch.  These entries do not include the register number - the 8
   MSB are set to non-zero for defined entries, and the index itself
   is the command number.  */
#define INIT(c, v)		[c]         = ((1 << 24) | ((v) & 0xffffff))
#define INITRANGE(s,e,v)	[(s)...(e)] = ((1 << 24) | ((v) & 0xffffff))
#define UNINIT(c)		[c]	    = 0

#define FP_1_0			0x3f8000 /* 1.0 in PSP 24-bit FP */

static const
uint32_t ge_init_state[256] =
{
	INIT(CMD_VERTEXPTR,	0),
	INIT(CMD_INDEXPTR,	0),
	INIT(CMD_BASE,		0),
	INIT(CMD_VERTEXTYPE,	0),

	INIT(CMD_OFFSET_ADDR,	0),

	INIT(CMD_REGION1,	0),
	INIT(CMD_REGION2,	0),

	INITRANGE(CMD_ENA_LIGHTING, CMD_ENA_LOGIC, 0),
	INIT(CMD_ENA_DITHER,	1),
	INIT(CMD_ENA_CLIP,	1),

	INITRANGE(CMD_MORPH_WEIGHT0, CMD_MORPH_WEIGHT7, 0),

	INIT(CMD_PATCH_SUBDIV,	0x404),
	INIT(CMD_PATCH_PRIM,	0),
	INIT(CMD_PATCH_CULL,	0),

	INITRANGE(CMD_VIEWPORT_SX, CMD_VIEWPORT_TZ, 0),

	INIT(CMD_TEXTURE_SU,	FP_1_0),
	INIT(CMD_TEXTURE_SV,	FP_1_0),
	INIT(CMD_TEXTURE_TU,	0),
	INIT(CMD_TEXTURE_TV,	0),

	INIT(CMD_OFFSETX,	0),
	INIT(CMD_OFFSETY,	0),

	INIT(CMD_SHADEMODEL,	0),
	INIT(CMD_REV_NORM,	0),

	INIT(CMD_MATERIAL,		0),
	INIT(CMD_MATERIAL_EMS_C,	0x000000), /* emissive is 0,0,0,1 */
	INIT(CMD_MATERIAL_AMB_C,	0xffffff), /* ambient  is 1,1,1,1 */
	INIT(CMD_MATERIAL_DIF_C,	0xcccccc), /* diffuse  is .8,.8,.8,1 */
	INIT(CMD_MATERIAL_SPC_C,	0x000000), /* specular is 0,0,0,1 */
	INIT(CMD_MATERIAL_AMB_A,	0x0000ff), /* ambient alpha is 1 */
	INIT(CMD_MATERIAL_SPEC_POW,	FP_1_0),

	INIT(CMD_AMBIENT_COLOR,		0),
	INIT(CMD_AMBIENT_ALPHA,		0),

	INIT(CMD_LIGHTMODEL,		0),
	INITRANGE(CMD_LIGHT0_TYPE, CMD_LIGHT3_SPC_COL, 0),

	INIT(CMD_LIGHT0_POS_Z,		FP_1_0),
	INIT(CMD_LIGHT1_POS_Z,		FP_1_0),
	INIT(CMD_LIGHT2_POS_Z,		FP_1_0),
	INIT(CMD_LIGHT3_POS_Z,		FP_1_0),

	INIT(CMD_LIGHT0_VEC_Z,		FP_1_0),
	INIT(CMD_LIGHT1_VEC_Z,		FP_1_0),
	INIT(CMD_LIGHT2_VEC_Z,		FP_1_0),
	INIT(CMD_LIGHT3_VEC_Z,		FP_1_0),

	INIT(CMD_LIGHT0_ATT_CONST,	FP_1_0),
	INIT(CMD_LIGHT1_ATT_CONST,	FP_1_0),
	INIT(CMD_LIGHT2_ATT_CONST,	FP_1_0),
	INIT(CMD_LIGHT3_ATT_CONST,	FP_1_0),

	INIT(CMD_LIGHT0_AMB_COL,	0x000000),
	INIT(CMD_LIGHT0_DIF_COL,	0xffffff),
	INIT(CMD_LIGHT0_SPC_COL,	0xffffff),

	INIT(CMD_CULL_FACE,	0),

	INIT(CMD_DRAWBUF,	0),
	INIT(CMD_DRAWBUFWIDTH,	0),
	INIT(CMD_DEPTHBUF,	0),
	INIT(CMD_DEPTHBUFWIDTH,	0),

	INITRANGE(TEXSTATE_START, TEXSTATE_END, 0),
	INIT(CMD_TEX_MIPMAP0,	0x000000), /* 1.0 PSP firmware workaround? */
	INIT(CMD_TEX_STRIDE0,	0x040004),
	INIT(CMD_TEX_SIZE0,	0x000101),
	UNINIT(182),
	UNINIT(183),
	UNINIT(CMD_CLUT_LOAD),

	INIT(CMD_TEXENV_FUNC,	0),
	INIT(CMD_TEXENV_COL,	0),
	INIT(CMD_TEXCACHE_SYNC,		0),
	INIT(CMD_TEXCACHE_FLUSH,	0),

	INIT(CMD_FOG_FAR,	0),
	INIT(CMD_FOG_NEAR,	0),
	INIT(CMD_FOG_COLOR,	0),

	INIT(CMD_TEXSLOPE,	0),

	INIT(CMD_PSM,		0),
	INIT(CMD_CLEARMODE,	0),

	INIT(CMD_SCISSOR1,	0),
	INIT(CMD_SCISSOR2,	0),

	INIT(CMD_CLIP_NEAR,	0),
	INIT(CMD_CLIP_FAR,	0),

	INITRANGE(CMD_COLOR_FUNC, CMD_FIXEDCOL_DST, 0),
	INIT(CMD_DEPTH_FUNC,	GE_GEQUAL), /* GL_LESS -> GE_GEQUAL */

	INIT(CMD_DITHER0,	0x001d0c),
	INIT(CMD_DITHER1,	0x00f3e2),
	INIT(CMD_DITHER2,	0x000c1d),
	INIT(CMD_DITHER3,	0x00e2f3),

	INIT(CMD_LOGICOP,	0),

	INIT(CMD_DEPTH_MASK,	0),
	INIT(CMD_RGB_MASK,	0),
	INIT(CMD_ALPHA_MASK,	0),

	INIT(CMD_COPY_SRC_XY,	0),
	INIT(CMD_COPY_DST_XY,	0),
	INIT(CMD_COPY_SIZE,	0),

	INITRANGE(0xf0, 0xf9, 0), /* sceGuInit does this */
};

void __pspgl_ge_init (struct pspgl_context *c)
{
	int reg;

	assert(c == pspgl_curctx);

	for (reg = 0; reg < sizeof(ge_init_state)/sizeof(ge_init_state[0]); reg++) {
		unsigned long val = ge_init_state[reg];

		if (val & 0xff000000) {
			val &= 0x00ffffff;
			__pspgl_context_writereg_uncached(c, reg, val);
			__pspgl_context_register[reg / 32] |= 1 << (reg % 32);
		}
	}

	glScissor(0, 0, c->draw->width, c->draw->height);
	glViewport(0, 0, c->draw->width, c->draw->height);
	glDepthRange(0.0, 1.0);
	c->clear.depth = 0;	/* = 1.0f in OpenGL coord system */
	c->viewport.depth_offset = 0.0;
	c->swap_interval = 1;

	/* Material ambient and current color share the same hardware
	   register, so we need to keep separate copies of the state;
	   the other material colors are set up with all the other
	   hardware state */
	c->current.color = 0xffffffff;		/* default color is 1,1,1,1 */
	c->material.ambient = 0xff333333;	/* material ambient color is .2, .2, .2, 1 */

	/* make sure all the stencil/alpha mask state is set up right */
	glStencilMask(~0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDisable(GL_STENCIL_TEST);
}

