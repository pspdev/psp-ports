#include "pspgl_internal.h"
#include "pspgl_texobj.h"
#include "pspgl_dlist.h"
#include "pspgl_matrix.h"

/**
 *  cached register write, save value and mark as touched...
 */
void __pspgl_context_writereg (struct pspgl_context *c, uint32_t cmd,
			       uint32_t argi) 
{
	uint32_t new = (cmd << 24) | (argi & 0xffffff);

	if (new != c->hw.ge_reg[cmd]) {
		c->hw.ge_reg[cmd] = new;
		c->hw.ge_reg_touched[cmd/32] |= (1 << (cmd % 32));
	}
}

void __pspgl_context_writereg_masked (struct pspgl_context *c, uint32_t cmd,
				      uint32_t argi, uint32_t mask)
{
	uint32_t new = (cmd << 24) | (c->hw.ge_reg[cmd] & ~mask) | (argi & mask & 0xffffff);

	if (new != c->hw.ge_reg[cmd]) {
		c->hw.ge_reg[cmd] = new;
		c->hw.ge_reg_touched[cmd/32] |= (1 << (cmd % 32));
	}
}


/**
 *  flush all pending, cached values, then clear register-touch mark words.
 */
void __pspgl_context_flush_pending_state_changes (struct pspgl_context *c,
						  unsigned first, unsigned last)
{
	first = first & ~31;
	last = (last + 31 + 1) & ~31;

	for(unsigned i = first; i < last; i += 32) {
		uint32_t word = c->hw.ge_reg_touched[i/32];
		unsigned j;

		c->hw.ge_reg_touched[i/32] = 0;

		if (word && 0)
			psp_log("setting i %d word %08x\n",
				i, word);

		for(j = i; word != 0; j++, word >>= 1) {
			if ((word & 1) && (c->hw.ge_reg[j] >> 24) == j)
				__pspgl_dlist_enqueue_cmd(c->hw.ge_reg[j]);
		}
	}
}


/**
 *  trigger some real action
 */
void __pspgl_context_writereg_uncached (struct pspgl_context *c, uint32_t cmd, uint32_t argi) 
{
	uint32_t val = ((cmd) << 24) | ((argi) & 0xffffff);

	c->hw.ge_reg[cmd] = val;	/* still need to record value */
	c->hw.ge_reg_touched[cmd/32] &= ~(1 << (cmd % 32)); /* not dirty */

	__pspgl_dlist_enqueue_cmd(val);
}


/* write a uncached matrix register */
static inline
void pspgl_context_writereg_mtx (struct pspgl_context *c, int cmd, GLfloat argf)
{
	union { float f; uint32_t i; } arg = { .f = argf };
	unsigned long val = (cmd << 24) | (arg.i >> 8);
	__pspgl_dlist_enqueue_cmd(val);
}


static void flush_matrix(struct pspgl_context *c, unsigned opcode, int index,
			 struct pspgl_matrix_stack *stk)
{
	const GLfloat *m;
	GLfloat adjusted[16];

	if (!(stk->flags & MF_DIRTY))
		return;
	stk->flags &= ~MF_DIRTY;

	if (stk->flags & MF_DISABLED)
		m = __pspgl_identity;
	else {
		__pspgl_matrix_sync(c, stk);

		m = stk->stack[stk->depth].mat;
	}

	if (stk->flags & MF_ADJUST) {
		/* Apply adjustment to the matrix.  This is used for
		   scaling OpenGL integral vertex and texcoord data to
		   PSP integral vertex/texture coords (OpenGL
		   interprets them as literal integers, but the PSP
		   treats them as being in the range [-1, 1]).  Also
		   used for flipping inverted textures. */
		pspvfpu_use_matrices(c->vfpu_context, 0, VMAT4 | VMAT5 | VMAT6);

		asm volatile("vmzero.t	m500\n"

			     /* original matrix */
			     "lv.q	c600,  0 + %3\n"
			     "lv.q	c610, 16 + %3\n"
			     "lv.q	c620, 32 + %3\n"
			     "lv.q	c630, 48 + %3\n"

			     /* translate */
			     "lv.q	c530, 0 + %2\n"

			     /* scale - doesn't seem to be a way of
				loading a diagonal in one go... */
			     "lv.s	s500, 0 + %1\n"
			     "lv.s	s511, 4 + %1\n"
			     "lv.s	s522, 8 + %1\n"
			     
			     "vidt.q	r503\n"

			     "vmmul.q	m400, m600, m500\n"

			     "usv.q	c400,  0 + %0\n"
			     "usv.q	c410, 16 + %0\n"
			     "usv.q	c420, 32 + %0\n"
			     "usv.q	c430, 48 + %0\n"
			     : "=m" (adjusted) : "m" (stk->scale[0]), "m" (stk->trans[0]), "m" (m[0]));

		m = adjusted;
	}

	/*
	  Different matrices skip different rows:
	  PROJ is a full 4x4 projective matrix
	  BONE, VIEW and MODEL are 4x3 matricies, skipping the 4th row

	  TEXTURE is a 4x3 matrix which skips the 3rd row (z) because
	  texcoords never have a Z value.  This still allows
	  projective texture effects.
	 */
	int skip;

	switch(opcode) {
	case CMD_MAT_PROJ_TRIGGER:	skip = -1; break;
	case CMD_MAT_TEXTURE_TRIGGER:	skip = 2; break;
	case CMD_MAT_BONE_TRIGGER:
	case CMD_MAT_VIEW_TRIGGER:
	case CMD_MAT_MODEL_TRIGGER:	skip = 3; break;
	default:
		return;
	}

	int n = (skip == -1) ? 4 : 3;

	__pspgl_context_writereg_uncached(c, opcode, index * n * 4);
	opcode++;
	for (int j=0; j<4; j++)
		for (int i=0; i<4; i++)
			if (i != skip)
				pspgl_context_writereg_mtx(c, opcode, m[4*j + i]);
}

static void flush_pending_matrix_changes (struct pspgl_context *c)
{
	int i;

	flush_matrix(c, CMD_MAT_PROJ_TRIGGER, 0, &c->projection_stack);
	flush_matrix(c, CMD_MAT_MODEL_TRIGGER, 0, &c->modelview_stack);
	flush_matrix(c, CMD_MAT_TEXTURE_TRIGGER, 0, &c->texture_stack);
	flush_matrix(c, CMD_MAT_VIEW_TRIGGER, 0, &c->view_stack);

	for(i = 0; i < NBONES; i++)
		flush_matrix(c, CMD_MAT_BONE_TRIGGER, i, &c->bone_stacks[i]);
}

/* Pin various state buffers for a render operation.  These are:
    - destination framebuffer
    - texture images
    - colour maps
*/
void __pspgl_context_pin_buffers(struct pspgl_context *c)
{
	struct pspgl_texobj *tobj;

	/* pin draw buffer and depth buffer */
	__pspgl_dlist_pin_buffer(*c->draw->draw, BF_PINNED);
	if ((c->hw.ge_reg[CMD_ENA_DEPTH_TEST] & 1) && c->draw->depth_buffer)
		__pspgl_dlist_pin_buffer(c->draw->depth_buffer, BF_PINNED);

	tobj = c->texture.bound;	

	/* If we're texturing, then pin texture-related buffers */
	if ((tobj != NULL) &&
	    (c->hw.ge_reg[CMD_ENA_TEXTURE] & 1)) {
		struct pspgl_teximg *cmap;

		/* find the effective cmap, if any */
		cmap = __pspgl_texobj_cmap(tobj);
		if (cmap)
			__pspgl_dlist_pin_buffer(cmap->image, BF_PINNED_RD);

		/* Walk the images pointed to by the texture object and make
		   sure they're pinned. */
		for (int i = 0; i < MIPMAP_LEVELS; i++)
			if (tobj->images[i] && tobj->images[i]->image)
				__pspgl_dlist_pin_buffer(tobj->images[i]->image,
							 BF_PINNED_RD);
	}
}

void __pspgl_context_render_setup(struct pspgl_context *c, unsigned vtxfmt, 
				  const void *vertex, const void *index)
{
	struct pspgl_texobj *tobj;
	int clut_load = 0;

	tobj = c->texture.bound;	

	/* Set up cmap state; if the texture format has an inherent
	   cmap, use that, otherwise set up the texture object's cmap
	   (if any).  Only bother if texturing is enabled and someone
	   said they changed the CLUT state.  */
	if ((tobj != NULL) &&
	    (c->hw.ge_reg[CMD_ENA_TEXTURE] & 1)) {
		if (c->hw.dirty & HWD_CLUT) {
			struct pspgl_teximg *cmap = __pspgl_texobj_cmap(tobj);

			c->hw.dirty &= ~HWD_CLUT;

			if (cmap) {
				unsigned long p = (unsigned long)cmap->image->base + cmap->offset;

				__pspgl_context_writereg(c, CMD_SET_CLUT, p);
				__pspgl_context_writereg(c, CMD_SET_CLUT_MSB, (p >> 8) & 0xf0000);
				__pspgl_context_writereg(c, CMD_CLUT_MODE, cmap->texfmt->hwformat | (0xff << 8));
				clut_load = cmap->width / 8;
			}
		}

		__pspgl_context_writereg(c, CMD_TEXMAPMODE, (GE_UV << 8) | GE_TEXTURE_MATRIX);

		/* If we're using an integral texcoord format (byte or
		   short), then we need to add a scaling factor.  GL
		   interprets the texcoord as a simple integer,
		   whereas the PSP treats it as number scaled (-1, 1).

		   This is combined with flipping for flipped textures.
		*/
		{
			struct pspgl_matrix_stack *tm = &c->texture_stack;
			float su = 1.f, sv = 1.f;
			float tu = 0.f, tv = 0.f;

			switch(vtxfmt & GE_TEXTURE_SHIFT(3)) {
			case GE_TEXTURE_8BIT:
				su = 127.f;
				sv = 127.f;
				break;

			case GE_TEXTURE_16BIT:
				su = 32767.f;
				sv = 32767.f;
				break;
			}

			if (tobj->flags & TOF_FLIPPED) {
				sv = -sv;
				tv = 1.f;
			}

			tm->flags &= ~MF_ADJUST;
			if (su != 1 || sv != 1 || tu != 0 || tv != 0) {
				tm->flags |= MF_ADJUST;
				tm->scale[0] = su;
				tm->scale[1] = sv;
				tm->scale[2] = 1.f;
				tm->trans[0] = tu;
				tm->trans[1] = tv;
				tm->trans[2] = 0.f;
			}
		}
	}

	/* If the program is used 8 or 16 bit vertex coords, we need
	   to scale them to match what the PSP hardware wants.  OpenGL
	   defines integral coords as just signed integers, but the
	   PSP interprets them as scaled floats in the range -1 - 1.
	   Therefore, we need to apply an appropriate scale factor as
	   a backend adjustment.  The modelview matrix is the first one to
	   be applied to a vertex, so put the scaling there (XXX
	   except for bones when skinning is enabled...) */
	c->modelview_stack.flags &= ~MF_ADJUST;
	
	switch(vtxfmt & GE_VERTEX_SHIFT(3)) {
	case GE_VERTEX_8BIT:
		c->modelview_stack.scale[0] = 127.f;
		c->modelview_stack.scale[1] = 127.f;
		c->modelview_stack.scale[2] = 127.f;
		c->modelview_stack.flags |= MF_ADJUST;
		break;

	case GE_VERTEX_16BIT:
		c->modelview_stack.scale[0] = 32767.f;
		c->modelview_stack.scale[1] = 32767.f;
		c->modelview_stack.scale[2] = 32767.f;
		c->modelview_stack.flags |= MF_ADJUST;
		break;
	}

	__pspgl_context_writereg(c, CMD_VERTEXTYPE, vtxfmt);

	if ((vtxfmt & GE_TRANSFORM_SHIFT(1)) == GE_TRANSFORM_3D)
		flush_pending_matrix_changes(c);

	__pspgl_context_flush_pending_state_changes(c, 0, 255);

	if (clut_load != 0)
		__pspgl_context_writereg_uncached(c, CMD_CLUT_LOAD, clut_load);

	__pspgl_context_writereg_uncached(c, CMD_BASE, ((unsigned)vertex >> 8) & 0x000f0000);
	__pspgl_context_writereg_uncached(c, CMD_VERTEXPTR, ((unsigned)vertex) & 0x00ffffff);
	if (vtxfmt & GE_VINDEX_SHIFT(3)) {
		assert(index != NULL);
		__pspgl_context_writereg_uncached(c, CMD_BASE, ((unsigned)index >> 8) & 0x000f0000);
		__pspgl_context_writereg_uncached(c, CMD_INDEXPTR,  ((unsigned)index) & 0x00ffffff);
	} else
		assert(index == NULL);
}


/* Do all the pre-render state flushing, and actually emit a primitive */
void __pspgl_context_render_prim(struct pspgl_context *c, 
				 unsigned prim, unsigned count, unsigned vtxfmt,
				 const void *vertex, const void *index)
{
	__pspgl_context_render_setup(c, vtxfmt, vertex, index);

	__pspgl_context_writereg_uncached(c, CMD_PRIM, (prim << 16) | count);

	/* Pin after issuing the CMD_PRIM command, since any command
	   could cause the dlist to be submitted and subsequently
	   unpin everything before the PRIM has actually been issued;
	   it better to unpin too late than too early. */
	__pspgl_context_pin_buffers(c);
}
