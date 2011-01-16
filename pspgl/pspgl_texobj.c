#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "pspgl_internal.h"
#include "pspgl_texobj.h"


/*
  RRRRRGGGGGGBBBBB	<- GL
  BBBBBGGGGGGRRRRR	<- PSP
 */
static inline unsigned short swap_565(unsigned short in)
{
	unsigned short r = (in & 0xf800) >> 11;
	unsigned short g = (in & 0x07e0);
	unsigned short b = (in & 0x001f) << 11;

	return b | g | r;
}

/*
  RRRRRGGGGGBBBBBA	<- GL
  ABBBBBGGGGGRRRRR	<- PSP
 */
static inline unsigned short swap_5551(unsigned short in)
{
	unsigned short r = (in & 0xf800) >> 11;
	unsigned short g = (in & 0x07c0) >> 1;
	unsigned short b = (in & 0x0036) << 9;
	unsigned short a = (in & 0x0001) << 15;

	return a | b | g | r;
}

/*
  RRRRGGGGBBBBAAAA	<- GL
  AAAABBBBGGGGRRRR	<- PSP
 */
static inline unsigned short swap_4444(unsigned short in)
{
	unsigned short r = (in & 0xf000) >> 12;
	unsigned short g = (in & 0x0f00) >> 4;
	unsigned short b = (in & 0x00f0) << 4;
	unsigned short a = (in & 0x000f) << 12;

	return a | b | g | r;
}

static void copy(const struct pspgl_texfmt *fmt, void *to, const void *from, unsigned width)
{
	memcpy(to, from, width * fmt->pixsize);
}

static void copy_565(const struct pspgl_texfmt *fmt, void *to, const void *from, unsigned width)
{
	unsigned short *dest = to;
	const unsigned short *src = from;

	while(width--)
		*dest++ = swap_565(*src++);
}

static void copy_5551(const struct pspgl_texfmt *fmt, void *to, const void *from, unsigned width)
{
	unsigned short *dest = to;
	const unsigned short *src = from;

	while(width--)
		*dest++ = swap_5551(*src++);
}

static void copy_4444(const struct pspgl_texfmt *fmt, void *to, const void *from, unsigned width)
{
	unsigned short *dest = to;
	const unsigned short *src = from;

	while(width--)
		*dest++ = swap_4444(*src++);
}

static void copy_888_alpha(const struct pspgl_texfmt *fmt, void *to, const void *from, unsigned width)
{
	unsigned char *dest = to;
	const unsigned char *src = from;

	while(width--) {
		dest[0] = src[0];
		dest[1] = src[1];
		dest[2] = src[2];
		dest[3] = 0xff;

		dest += 4;
		src += 3;
	}
}

static void copy_expand_LA(const struct pspgl_texfmt *fmt, void *to, const void *from, unsigned width)
{
       unsigned int *dest = to;
       const unsigned char *src = from;

       while(width--) {
               unsigned char l = src[0];
               unsigned char a = src[1];

               *dest = (a << 24) | (l << 16) | (l << 8) | l;

               dest++;
               src += 2;
       }
}


static void copy_index4(const struct pspgl_texfmt *fmt, void *to, const void *from, unsigned width)
{
	/* 2 pixels/byte */
	memcpy(to, from, width / 2);
}

static struct pspgl_teximg alpha_cmap = {
	.image = NULL,
	.offset = 0,

	.width = 256,
	.height = 1,

	.texfmt = NULL,
};

static struct pspgl_teximg luminance_cmap = {
	.image = NULL,
	.offset = 0,

	.width = 256,
	.height = 1,

	.texfmt = NULL,
};

static struct pspgl_teximg intensity_cmap = {
	.image = NULL,
	.offset = 0,

	.width = 256,
	.height = 1,

	.texfmt = NULL,
};

const struct pspgl_texfmt __pspgl_texformats[] = {
	/* format       type                        source sz   hwformat      hw size   converter               flags */
	{ GL_RGBA,	GL_UNSIGNED_BYTE,		4,	GE_RGBA_8888, 4,	copy,			TF_ALPHA | TF_NATIVE },
	{ GL_RGB,	GL_UNSIGNED_BYTE,		3,	GE_RGBA_8888, 4,	copy_888_alpha,		0  },

	{ GL_RGBA,	GL_UNSIGNED_SHORT_5_5_5_1,	2,	GE_RGBA_5551, 2,	copy_5551,		TF_ALPHA },
	{ GL_RGBA,	GL_UNSIGNED_SHORT_4_4_4_4,	2,	GE_RGBA_4444, 2,	copy_4444,		TF_ALPHA },

	{ GL_RGBA,	GL_UNSIGNED_SHORT_1_5_5_5_REV,	2,	GE_RGBA_5551, 2,	copy,			TF_ALPHA | TF_NATIVE },
	{ GL_RGBA,	GL_UNSIGNED_SHORT_4_4_4_4_REV,	2,	GE_RGBA_4444, 2,	copy,			TF_ALPHA | TF_NATIVE },

	{ GL_ABGR_EXT,	GL_UNSIGNED_SHORT_4_4_4_4,	2,	GE_RGBA_4444, 2,	copy,			TF_ALPHA | TF_NATIVE },

	{ GL_RGB,	GL_UNSIGNED_SHORT_5_5_5_1,	2,	GE_RGBA_5551, 2,	copy_5551,		0  },
	{ GL_RGB,	GL_UNSIGNED_SHORT_5_6_5,	2,	GE_RGB_565,   2,	copy_565,		0  },
	{ GL_RGB,	GL_UNSIGNED_SHORT_4_4_4_4,	2,	GE_RGBA_4444, 2,	copy_4444,		0  },

	{ GL_RGB,	GL_UNSIGNED_SHORT_1_5_5_5_REV,	2,	GE_RGBA_5551, 2,	copy,			TF_NATIVE  },
	{ GL_RGB,	GL_UNSIGNED_SHORT_5_6_5_REV,	2,	GE_RGB_565,   2,	copy,			TF_NATIVE  },
	{ GL_RGB,	GL_UNSIGNED_SHORT_4_4_4_4_REV,	2,	GE_RGBA_4444, 2,	copy,			TF_NATIVE  },

	{ GL_BGR,	GL_UNSIGNED_SHORT_5_6_5,	2,	GE_RGB_565,   2,	copy,			TF_NATIVE  },

	{ GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,		2,	GE_RGBA_8888, 4,	copy_expand_LA,		TF_ALPHA },

	{ GL_LUMINANCE,	GL_UNSIGNED_BYTE,		1,	GE_INDEX_8BIT, 1,	copy,			TF_NATIVE,
	  &luminance_cmap  },

	{ GL_ALPHA,	GL_UNSIGNED_BYTE,		1,	GE_INDEX_8BIT, 1,	copy,			TF_NATIVE | TF_ALPHA,
	  &alpha_cmap },

	/* an intensity (L==A) format with efficent (1 byte/texel) storage is very useful for fonts */
	{ GL_INTENSITY,	GL_UNSIGNED_BYTE,		1,	GE_INDEX_8BIT, 1,	copy,			TF_NATIVE | TF_ALPHA,
	  &intensity_cmap },

 	/* Indexed textures */
 	{ GL_COLOR_INDEX4_EXT,	GL_UNSIGNED_BYTE,	0,	GE_INDEX_4BIT, 0,	copy_index4,		TF_NATIVE },
 	{ GL_COLOR_INDEX8_EXT,	GL_UNSIGNED_BYTE,	1,	GE_INDEX_8BIT, 1,	copy,			TF_NATIVE },
 	{ GL_COLOR_INDEX16_EXT,	GL_UNSIGNED_SHORT,	2,	GE_INDEX_16BIT, 2,	copy,			TF_NATIVE },

	{0,0}
};

static void make_cmap_alpha(unsigned *ptr)
{
	int i;

	for(i = 0; i < 256; i++)
		*ptr++ = i << 24;
}

static void make_cmap_intensity(unsigned *ptr)
{
	int i;

	for(i = 0; i < 256; i++) {
		unsigned pix = (i << 8) | i;
		pix = (pix << 16) | pix;
		*ptr++ = pix;
	}
}

static void make_cmap_luminance(unsigned *ptr)
{
	int i;

	for(i = 0; i < 256; i++) {
		unsigned pix = (i << 8) | i;
		pix = 0xff000000 | (i << 16) | pix;
		*ptr++ = pix;
	}
}

static void init_internal_cmap(struct pspgl_teximg *cmap, const struct pspgl_texfmt *fmt,
			       void (*make_cmap)(unsigned *))
{
	struct pspgl_buffer *b;
	void *map;

	b = __pspgl_buffer_new(256 * 4, GL_STATIC_DRAW_ARB);

	if (b == NULL)
		return;		/* ? */

	map = __pspgl_buffer_map(b, GL_WRITE_ONLY);
	(*make_cmap)(map);
	__pspgl_buffer_unmap(b, GL_WRITE_ONLY);

	cmap->image = b;
	cmap->texfmt = fmt;
}

const struct pspgl_texfmt *__pspgl_hardware_format(const struct pspgl_texfmt *table, GLenum format, GLenum type)
{
	int i;
	const struct pspgl_texfmt *ret;
	for(i = 0; table[i].format != 0; i++)
		if (table[i].format == format &&
		    table[i].type == type) {
			ret = &table[i];

			if (format == GL_LUMINANCE && luminance_cmap.image == NULL)
				init_internal_cmap(&luminance_cmap, &__pspgl_texformats[1], make_cmap_luminance);
			else if (format == GL_ALPHA && alpha_cmap.image == NULL)
				init_internal_cmap(&alpha_cmap, &__pspgl_texformats[0], make_cmap_alpha);
			else if (format == GL_INTENSITY && intensity_cmap.image == NULL)
				init_internal_cmap(&intensity_cmap, &__pspgl_texformats[0], make_cmap_intensity);

			return ret;
		}

	return NULL;
}

static void convert_compressed_image(const void *pixels, unsigned width, unsigned height, unsigned size,
				     void *to, const struct pspgl_texfmt *texfmt)
{
	(*texfmt->convert)(texfmt, to, pixels, size);
}

static void convert_image(const void *pixels, unsigned width, unsigned height, unsigned srcstride,
			  void *to, const struct pspgl_texfmt *texfmt)
{
	const unsigned char *src = pixels;
	unsigned char *dest = to;

	psp_log("convert %p -> %p size=%dx%d stride=%d texfmt=%p (%x/%x -> %d)\n",
		pixels, to, width, height, srcstride,
		texfmt, texfmt->format, texfmt->type, texfmt->hwformat);

	while(height--) {
		(*texfmt->convert)(texfmt, dest, src, width);
		src += texfmt->pixsize * srcstride;
		dest += texfmt->hwsize * width;
	}
}

static void convert_swizzle_image(const void *pixels, unsigned width, unsigned height,
				  void *to, const struct pspgl_texfmt *texfmt)
{
	unsigned dst_bytewidth = width * texfmt->hwsize;
	unsigned lg2_w = lg2(dst_bytewidth);
	unsigned dst_chunk = 16;
	unsigned pix_per_chunk = dst_chunk / texfmt->hwsize;
	unsigned src_chunk = pix_per_chunk * texfmt->pixsize;
	unsigned src_size = width * height * texfmt->pixsize;
	unsigned src_off, dst_off;

	psp_log("convert_swizzle: %dx%d, hwfmt=%d pixsize=%d hwsize=%d size_src=%d pix_per_chunk=%d chunk_src=%d, lg2(%d)=%d\n",
		width, height, texfmt->hwformat, texfmt->pixsize, texfmt->hwsize, 
		src_size, pix_per_chunk, src_chunk, dst_bytewidth, lg2_w);

	for(src_off = 0, dst_off = 0;
	    src_off < src_size;
	    src_off += src_chunk, dst_off += dst_chunk) {
		unsigned swizoff = swizzle(dst_off, lg2_w);

		//psp_log("  %d: swiz(%u, %d) -> %u\n", src_off, dst_off, lg2_w, swizoff);
		(*texfmt->convert)(texfmt,
				   to + swizoff,
				   pixels + src_off, 
				   pix_per_chunk);
	}
}

static void tobj_setstate(struct pspgl_texobj *tobj, uint32_t reg, uint32_t setting)
{
	tobj->ge_texreg[reg - TEXSTATE_START] = (reg << 24) | (setting & 0x00ffffff);
}

struct pspgl_texobj *__pspgl_texobj_new(GLuint id, GLenum target)
{
	struct pspgl_texobj *tobj = malloc(sizeof(*tobj));
	unsigned i;

	if (tobj == NULL)
		return NULL;

	memset(tobj, 0, sizeof(*tobj));

	tobj->refcount = 1;
	tobj->target = target;
	tobj->flags = TOF_SWIZZLED; /* swizzle by default */

	for(i = TEXSTATE_START; i <= TEXSTATE_END; i++)
		tobj_setstate(tobj, i, 0);

	tobj_setstate(tobj, CMD_TEXFILT, (GE_TEX_FILTER_LINEAR << 8) | GE_TEX_FILTER_LINEAR);
	tobj_setstate(tobj, CMD_TEXWRAP, (GE_TEX_WRAP_REPEAT << 8) | GE_TEX_WRAP_REPEAT);

	return tobj;
}

void __pspgl_texobj_unswizzle(struct pspgl_texobj *tobj)
{
	int i;

	if (tobj == NULL || (tobj->flags & TOF_SWIZZLED) == 0)
		return;

	tobj->flags &= ~TOF_SWIZZLED;

	for(i = 0; i < MIPMAP_LEVELS; i++) {
		struct pspgl_teximg *timg = tobj->images[i];

		if (timg == NULL || timg->image)
			continue;

		/* XXX actually unswizzle here */
	}
}

void __pspgl_texobj_free(struct pspgl_texobj *tobj)
{
	int i;

	assert(tobj->refcount > 0);

	if (--tobj->refcount)
		return;

	for(i = 0; i < MIPMAP_LEVELS; i++)
		if (tobj->images[i] != NULL)
			__pspgl_teximg_free(tobj->images[i]);

	if (tobj->cmap != NULL)
		__pspgl_teximg_free(tobj->cmap);

	free(tobj);
}

struct pspgl_teximg *__pspgl_texobj_cmap(const struct pspgl_texobj *tobj)
{
	struct pspgl_teximg *ret = NULL;

	if (tobj != NULL) {
		if (tobj->texfmt)
			ret = tobj->texfmt->cmap;
		if (ret == NULL)
			ret = tobj->cmap;
	}

	return ret;
}

struct pspgl_teximg *__pspgl_teximg_new(const void *pixels, const struct pixelstore *unpack,
					unsigned width, unsigned height, unsigned size,
					GLboolean swizzled, const struct pspgl_texfmt *texfmt)
{
	struct pspgl_teximg *timg;
	unsigned srcsize;
	unsigned srcstride;
	GLenum error;
	struct pspgl_bufferobj *buffer = unpack->pbo;

	srcstride = width;
	if (unpack->row_length != 0)
		srcstride = unpack->row_length;

	if (unpack->alignment > texfmt->pixsize)
		srcstride = ROUNDUP(srcstride, unpack->alignment / texfmt->pixsize);

	pixels += (unpack->skip_rows * srcstride + unpack->skip_pixels) * texfmt->pixsize;

	error = GL_OUT_OF_MEMORY;
	timg = malloc(sizeof(*timg));
	if (timg == NULL)
		goto out_error;

	memset(timg, 0, sizeof(*timg));

	srcsize = size;
	if (size == 0) {
		size = width * height * texfmt->hwsize;
		srcsize = srcstride * height * texfmt->pixsize;
	}

	timg->image = NULL;
	timg->offset = 0;

	timg->width = width;
	timg->height = height;
	timg->stride = width;

	timg->texfmt = texfmt;

	error = GL_INVALID_OPERATION;
	if (buffer != NULL) {
		if (buffer->mapped ||
		    ((pixels - NULL) + srcsize) > buffer->data->size)
			goto out_error;

		if (texfmt->flags & TF_NATIVE) {
			/* They provided the texture in a buffer in
			   native format; use it directly */
			timg->image = buffer->data;
			timg->image->refcount++;
			timg->offset = pixels - NULL;
			timg->stride = srcstride;
		} 
	}

	if (timg->image == NULL && size > 0) {
		const void *src;

		timg->image = __pspgl_buffer_new(size, GL_STATIC_DRAW_ARB);

		error = GL_OUT_OF_MEMORY;
		if (timg->image == NULL)
			goto out_error;

		psp_log("allocated %p->image=%p->base=%p\n", 
			timg, timg->image, timg->image->base);

		src = __pspgl_bufferobj_map(buffer, GL_READ_ONLY_ARB,
					    (void *)pixels);

		if (src != NULL) {
			void *p;

			p = __pspgl_buffer_map(timg->image, GL_WRITE_ONLY_ARB);

			if (texfmt->hwformat >= GE_DXT1)
				convert_compressed_image(src, width, height, size, 
							 p, texfmt);
			else if (swizzled && (width*texfmt->hwsize > 16) && (height >= 8))
				convert_swizzle_image(src, width, height, p, texfmt);
			else
				convert_image(src, width, height, srcstride, p, texfmt);

			__pspgl_buffer_unmap(timg->image, GL_WRITE_ONLY_ARB);
		}

		__pspgl_bufferobj_unmap(buffer, GL_READ_ONLY_ARB);
	}

	return timg;

  out_error:
	GLERROR(error);
	if (timg)
		__pspgl_teximg_free(timg);

	return NULL;
}

/* Construct a teximg to refer to pixel data which already exists
   within a buffer at a particular offset */
struct pspgl_teximg *__pspgl_teximg_from_buffer(struct pspgl_buffer *buffer, unsigned offset,
						unsigned width, unsigned height, unsigned stride,
						const struct pspgl_texfmt *texfmt)
{
	struct pspgl_teximg *timg;

	timg = malloc(sizeof(*timg));
	if (timg == NULL)
		goto out;

	memset(timg, 0, sizeof(*timg));

	timg->image = buffer;
	buffer->refcount++;

	timg->offset = offset;

	timg->width = width;
	timg->height = height;
	timg->stride = stride;
	timg->texfmt = texfmt;

  out:
	return timg;
}

void __pspgl_teximg_free(struct pspgl_teximg *timg)
{
	psp_log("freeing %p->image=%p\n",
		timg, timg->image);

	if (timg->image)
		__pspgl_buffer_free(timg->image);
	free(timg);
}

void __pspgl_update_texenv(struct pspgl_texobj *tobj)
{
	const struct pspgl_texfmt *fmt;

	if (tobj == NULL)
		return;

	/* If this is a user-supplied indexed format texture and
	   there's a cmap, use the cmap's format */
	fmt = tobj->texfmt;
	if (fmt &&
	    (fmt->format >= GL_COLOR_INDEX4_EXT && fmt->format <= GL_COLOR_INDEX16_EXT) &&
	    tobj->cmap != NULL)
		fmt = tobj->cmap->texfmt;

	if (fmt)
		__pspgl_context_writereg_masked(pspgl_curctx, CMD_TEXENV_FUNC,
						(fmt->flags & TF_ALPHA) ? GE_TEXENV_RGBA : GE_TEXENV_RGB, 0x100);

	/* If we've switched textures or changed the flip status, mark
	   the texture matrix as dirty so that any adjustment can be
	   applied. */
	pspgl_curctx->texture_stack.flags |= MF_DIRTY;
}
