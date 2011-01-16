#include <string.h>
#include <malloc.h>

#include "pspgl_internal.h"
#include "pspgl_texobj.h"
#include "pspgl_dlist.h"

static unsigned max_mipmap(unsigned width, unsigned height)
{
	int maxlvl;
	int lw, lh;

	lw = lg2(width);
	lh = lg2(height);

	maxlvl = ((lw > lh) ? lw : lh);
	if (maxlvl >= MIPMAP_LEVELS)
		maxlvl = MIPMAP_LEVELS-1;
	return maxlvl;
}

static void set_mipmap_regs(unsigned level, struct pspgl_teximg *img)
{
	if (img) {
		unsigned ptr = (unsigned)img->image->base + img->offset;
		unsigned w_lg2 = lg2(img->width);
		unsigned h_lg2 = lg2(img->height);

		psp_log("set level %d image=%p %dx%d\n",
			level, img->image, img->width, img->height);

		sendCommandi(CMD_TEX_MIPMAP0 + level, ptr);
		sendCommandi(CMD_TEX_STRIDE0 + level, ((ptr >> 8) & 0xf0000) | img->stride);
		sendCommandi(CMD_TEX_SIZE0 + level, (h_lg2 << 8) | w_lg2);
	} else {
		psp_log("set level %d image=NULL\n", level);

		sendCommandi(CMD_TEX_MIPMAP0 + level, 0);
		sendCommandi(CMD_TEX_STRIDE0 + level, 0);
		sendCommandi(CMD_TEX_SIZE0 + level, 0);
	}
}

/* Update mipmaps for bound texture.  Uses render-to-texture to
   perform the scaling.  The core of this is very simple, but managing
   all the state is pretty fiddley. */
void __pspgl_update_mipmaps(void)
{
	struct pspgl_teximg *timg;
	const struct pspgl_texfmt *texfmt;
	unsigned levels, level;
	struct pspgl_buffer *mipmaps;
	struct pspgl_texobj *tobj = pspgl_curctx->texture.bound;
	GLenum saved_err;

	if (tobj == NULL ||
	    (tobj->flags & TOF_GENERATE_MIPMAPS) == 0 ||
	    tobj->images[0] == NULL ||
	    tobj->texfmt == NULL ||
	    tobj->texfmt->hwformat > GE_RGBA_8888)
		return;

	__pspgl_texobj_unswizzle(tobj);

	timg = tobj->images[0];

	/* 
	   Save state: lighting, texture, depth/alpha/color tests, framebuffer

	   Set up state

	   for (each level, 0-(N-1))
	     set MIPMAP0 -> level
	     allocate image for level+1
	     set framebuffer -> level+1
	     draw quad at 1/4 scale

	   restore state

	   XXX May fail if attribute stack overflows
	 */
	saved_err = pspgl_curctx->glerror;
	pspgl_curctx->glerror = GL_NO_ERROR;

	glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
	if (pspgl_curctx->glerror != GL_NO_ERROR)
		return;

	pspgl_curctx->glerror = saved_err;

	/* Just save the shademodel rather than the complete lighting state */
	GLuint shademodel = getReg(CMD_SHADEMODEL);

	glDisable(GL_SCISSOR_TEST);

	sendCommandi(CMD_ENA_TEXTURE, 1);
	sendCommandi(CMD_ENA_LIGHTING, 0);
	sendCommandi(CMD_ENA_ALPHA_TEST, 0);
	sendCommandi(CMD_ENA_DEPTH_TEST, 0);
	sendCommandi(CMD_ENA_STENCIL_TEST, 0);
	sendCommandi(CMD_ENA_BLEND, 0);
	sendCommandi(CMD_ENA_LOGIC, 0);
	sendCommandi(CMD_RGB_MASK, 0);
	sendCommandi(CMD_ALPHA_MASK, 0);
	sendCommandi(CMD_SHADEMODEL, 0);
	sendCommandi(CMD_TEXFILT, (GE_TEX_FILTER_LINEAR << 8) | GE_TEX_FILTER_LINEAR);
	sendCommandi(CMD_TEXWRAP, (GE_TEX_WRAP_CLAMP << 8) | GE_TEX_WRAP_CLAMP);
	sendCommandi(CMD_TEXMODE, (0 << 16) | 0);

	if (tobj->flags & TOF_GENERATE_MIPMAP_DEBUG)
		sendCommandi(CMD_TEXENV_FUNC, GE_TEXENV_RGBA | GE_TEXENV_MODULATE);
	else
		sendCommandi(CMD_TEXENV_FUNC, GE_TEXENV_RGBA | GE_TEXENV_REPLACE);

	unsigned mipmap_size = 0;
	unsigned w = timg->width;
	unsigned h = timg->height;

	/* Compute how much memory we'll need for this mipmap
	   pyramid */
	levels = max_mipmap(w, h);
	for(level = 1; level <= levels; level++) {
		if (w > 1)
			w /= 2;
		if (h > 1)
			h /= 2;
		mipmap_size += w*h;
	}

	texfmt = tobj->texfmt;
	mipmaps = __pspgl_buffer_new(mipmap_size * texfmt->hwsize, GL_STATIC_COPY_ARB);
	unsigned mipmapoff = 0;

	psp_log("allocated %d pixels (%d bytes) of mipmap buffer at %p\n",
		mipmap_size, mipmap_size * texfmt->hwsize, mipmaps);

	w = timg->width;
	h = timg->height;
	unsigned tw = w;
	unsigned th = h;

	/* texture format and framebuffer format match */
	sendCommandi(CMD_TEXFMT, texfmt->hwformat);
	sendCommandi(CMD_PSM, texfmt->hwformat);

	/* mipmap0 is already pointing to our base texture */
	for(level = 1; level <= levels; level++) {
		static const unsigned levelcols[] = {
			/*a b g r  */
			0xffffffff,
			0xffc0ffff,
			0xffffc0ff,
			0xffc0c0ff,
			0xffffffc0,
			0xffc0ffc0,
			0xffffc0c0,
			0xffc0c0c0,
		};
		struct vertex {
			unsigned short s,t;
			unsigned long color;
			unsigned short x,y,z;
		} *vertexbuf;
		unsigned fbptr;
		struct pspgl_teximg *timg;

		if (tw > 1)
			tw /= 2;
		if (th > 1)
			th /= 2;

		/* free old teximg if necessary */
		if (tobj->images[level] != NULL) {
			__pspgl_teximg_free(tobj->images[level]);
			tobj->images[level] = NULL;
		}

		timg = __pspgl_teximg_from_buffer(mipmaps, mipmapoff, tw, th, tw, texfmt);
		tobj->images[level] = timg;

		psp_log("level %d has mipmap at offset %d (ptr %p); %dx%d->%dx%d\n",
			level, mipmapoff, mipmaps->base+mipmapoff,
			w, h, tw, th);

		fbptr = (unsigned)(mipmaps->base + mipmapoff);
		sendCommandi(CMD_DRAWBUF, fbptr);
		sendCommandi(CMD_DRAWBUFWIDTH, ((fbptr & 0xff000000) >> 8) | tw);

		mipmapoff += tw * th * texfmt->hwsize;

		sendCommandi(CMD_SCISSOR1, (0 << 10) | 0);
		sendCommandi(CMD_SCISSOR2, ((th-1) << 10) | (tw-1));

		vertexbuf = __pspgl_dlist_insert_space(sizeof(*vertexbuf) * 2);

		vertexbuf[0].s = 0;
		vertexbuf[0].t = 0;
		vertexbuf[0].color = 0;
		vertexbuf[0].x = 0;
		vertexbuf[0].y = 0;
		vertexbuf[0].z = 0;

		vertexbuf[1].s = w;
		vertexbuf[1].t = h;
		vertexbuf[1].color = levelcols[level];
		vertexbuf[1].x = tw;
		vertexbuf[1].y = th;
		vertexbuf[1].z = 0;

		__pspgl_context_render_prim(pspgl_curctx, GE_SPRITES, 2,
					    GE_COLOR_8888 | GE_VERTEX_16BIT |
					    GE_TEXTURE_16BIT | GE_TRANSFORM_2D,
					    vertexbuf, NULL);
		__pspgl_dlist_pin_buffer(mipmaps, BF_PINNED);

		/* set mipmap0 to point to the mipmap we just generated */
		w = tw;
		h = th;
		set_mipmap_regs(0, timg);
		sendCommandi(CMD_TEXCACHE_FLUSH, getReg(CMD_TEXCACHE_FLUSH)+1);
	}

	/* all the appropriate texture images have a reference now */
	__pspgl_buffer_free(mipmaps);

	/* restore GL state */
	glPopAttrib();
	sendCommandi(CMD_SHADEMODEL, shademodel);

	/* restore all mipmaps to their proper places - do this after
	   glPopAttrib, so that we don't get stuck with old mipmap
	   values */
	for(level = 0; level <= levels; level++)
		set_mipmap_regs(level, tobj->images[level]);
}

/* If we've compacted the vidmem then we may have moved the currently
   bound texture, so update the registers accordingly. */
void __pspgl_moved_textures()
{
	int i;
	struct pspgl_texobj *tobj = pspgl_curctx->texture.bound;

	if (tobj == NULL)
		return;

	for(i = 0; i < MIPMAP_LEVELS; i++)
		set_mipmap_regs(i, tobj->images[i]);
}

void __pspgl_set_texture_image(struct pspgl_texobj *tobj, unsigned level,
			       struct pspgl_teximg *timg)
{
	struct pspgl_teximg *old_timg = tobj->images[level];

	if (timg == old_timg)
		return;

	tobj->images[level] = NULL;

	if (timg) {
		/* if we're changing texture formats, then invalidate all the other images */
		if (tobj->texfmt != timg->texfmt) {
			int i;
			for(i = 0; i < MIPMAP_LEVELS; i++) {
				if (i == level)
					continue;

				if (tobj->images[i] && 
				    tobj->images[i]->texfmt != timg->texfmt) {
					__pspgl_teximg_free(tobj->images[i]);
					tobj->images[i] = NULL;
					set_mipmap_regs(i, NULL);
				}
			}
		}
		tobj->texfmt = timg->texfmt;

		sendCommandi(CMD_TEXFMT, tobj->texfmt->hwformat);
	}
	tobj->images[level] = timg;
	set_mipmap_regs(level, timg);

	if (old_timg != NULL) {
		psp_log("replaced texture image %p at level %d with %p\n", 
			old_timg, level, timg);

		__pspgl_teximg_free(old_timg);
	}

	if (timg && (level == 0)) {
		int maxlvl = max_mipmap(timg->width, timg->height);
		int swizzled = (tobj->flags & TOF_SWIZZLED) != 0;

		psp_log("setting %dx%d maxlevel to %d; swizzled=%d\n", 
			timg->width, timg->height, maxlvl, swizzled);
		sendCommandi(CMD_TEXMODE, (maxlvl << 16) | (0 << 8) | swizzled);
	}

	sendCommandi(CMD_TEXCACHE_FLUSH, getReg(CMD_TEXCACHE_FLUSH)+1);
}

#if 0
static inline GLboolean mipmap_filter(GLenum filter)
{
	return filter >= GL_NEAREST_MIPMAP_NEAREST && filter <= GL_LINEAR_MIPMAP_LINEAR;
}

static GLboolean texobj_is_complete(struct pspgl_texobj *tobj)
{
	if (tobj == NULL) {
		psp_log("incomplete: tobj NULL\n");
		return GL_FALSE;
	}

	if (tobj->images[0] == NULL) {
		psp_log("incomplete: base NULL\n");
		return GL_FALSE;
	}

	if (mipmap_filter(tobj->min_filter)) {
		int i;

		/* XXX check sizes too */
		for(i = 1; i < MIPMAP_LEVELS; i++) {
			if (tobj->images[i] == NULL) {
				psp_log("incomplete: mipmap level %d missing\n", i);
				return GL_FALSE;
			}

			if (tobj->images[i]->texfmt != tobj->texfmt) {
				psp_log("incomplete: level %s mismatched format %d %d\n",
					i, tobj->images[i]->texfmt->hwformat, tobj->texfmt->hwformat);
				return GL_FALSE;
			}
		}
	}

	if (texfmt_is_indexed(tobj->texfmt) && tobj->cmap == NULL)
		return GL_FALSE;

	return GL_TRUE;
}
#endif

void glTexImage2D (GLenum target, GLint level, GLint internalformat, 
		   GLsizei width, GLsizei height, GLint border, 
		   GLenum format, GLenum type, const GLvoid *texels)
{
	GLenum error;
	static const GLenum format_equiv[] = {
		0,
		GL_LUMINANCE,
		GL_LUMINANCE_ALPHA,
		GL_RGB,
		GL_RGBA,
	};

	struct pspgl_texobj *tobj;
	struct pspgl_teximg *timg;
	const struct pspgl_texfmt *texfmt;

	error = GL_INVALID_ENUM;
	if (unlikely(target != GL_TEXTURE_2D))
		goto out_error;

	error = GL_INVALID_VALUE;
	if (!ispow2(width) || !ispow2(height))
		goto out_error;

	if (level < 0 || level > MIPMAP_LEVELS)
		goto out_error;

	/* PSP supports 512x512 textures, but only goes down to 2x2
	   mipmaps for them, so just ignore any higher level */
	if (level == MIPMAP_LEVELS)
		return;

	if (border != 0)
		goto out_error;

	/* we need to special-case a few things */
	switch(internalformat) {
	case 1 ... 4:
		/* old-style formats are just number of components */
		internalformat = format_equiv[internalformat];
		break;

	case GL_INTENSITY:
		/* intensity is always specified as luminance */
		error = GL_INVALID_ENUM;
		if (format != GL_LUMINANCE)
			goto out_error;
		format = internalformat;
		break;
	}

	error = GL_INVALID_OPERATION;
	if (format != internalformat)
		goto out_error;

	error = GL_INVALID_ENUM;
	texfmt = __pspgl_hardware_format(__pspgl_texformats, internalformat, type);
	if (texfmt == NULL)
		goto out_error;

	if (texfmt->hwformat >= GE_DXT1)
		goto out_error;

	psp_log("selected texfmt %d for fmt=%x type=%x\n",
		texfmt->hwformat, internalformat, type);

	if (!pspgl_curctx->texture.bound)
		glBindTexture(target, 0);

	error = GL_OUT_OF_MEMORY;
	tobj = pspgl_curctx->texture.bound;
	if (tobj == NULL)
		goto out_error;

	if (pspgl_curctx->unpack.pbo != NULL)
		tobj->flags &= ~TOF_SWIZZLED;

	timg = __pspgl_teximg_new(texels, &pspgl_curctx->unpack,
				  width, height, 0, (tobj->flags & TOF_SWIZZLED) != 0, texfmt);
	if (timg == NULL)
		return;		/* error already set */

	__pspgl_set_texture_image(tobj, level, timg);

	if (__pspgl_texobj_cmap(tobj) != NULL)
		pspgl_curctx->hw.dirty |= HWD_CLUT;

	__pspgl_update_texenv(tobj);

	if (level == 0 && (tobj->flags & TOF_GENERATE_MIPMAPS))
		__pspgl_update_mipmaps();
	return;

  out_error:
	GLERROR(error);
}
