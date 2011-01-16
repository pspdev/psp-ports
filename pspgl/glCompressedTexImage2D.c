#include <stdlib.h>
#include <string.h>

#include "pspgl_internal.h"
#include "pspgl_texobj.h"

/* 
   For compressed formats, convert the whole image; 
   width is the compressed image size in bytes
 */

/* PSP DXT1 hardware format reverses the colors and the per-pixel
   bits, and encodes the color in RGB 565 format */
static void copy_dxt1(const struct pspgl_texfmt *fmt, void *to, const void *from, unsigned size)
{
	const unsigned long *src = from;
	unsigned long *dest = to;

	for(; size >= 8; size -= 8) {
		dest[0] = src[1];
		dest[1] = src[0];

		dest += 2;
		src += 2;
	}
}

/* PSP DXT3 format reverses the alpha and color parts of each block,
   and reverse the color and per-pixel terms in the color part. */
static void copy_dxt3(const struct pspgl_texfmt *fmt, void *to, const void *from, unsigned size)
{
	const unsigned long *src = from;
	unsigned long *dest = to;

	for(; size >= 16; size -= 16) {
		/* colour */
		dest[0] = src[3];
		dest[1] = src[2];
		/* alpha */
		dest[2] = src[0];
		dest[3] = src[1];

		dest += 4;
		src += 4;
	}
}

/* PSP DXT5 format reverses the alpha and color parts of each block,
   and reverse the color and per-pixel terms in the color part.  In
   the alpha part, the 2 reference alpha values are swapped with the
   alpha interpolation values. */
static void copy_dxt5(const struct pspgl_texfmt *fmt, void *to, const void *from, unsigned size)
{
	const unsigned short *src = from;
	unsigned short *dest = to;

	for(; size >= 16; size -= 16) {
		/* colour */
		((unsigned long *)dest)[0] = ((unsigned long *)src)[3];
		((unsigned long *)dest)[1] = ((unsigned long *)src)[2];

		/* alpha */
		dest[4] = src[1];
		dest[5] = src[2];
		dest[6] = src[3];
		dest[7] = src[0];

		dest += 8;
		src += 8;
	}
}

static const struct pspgl_texfmt comptexformats[] = {
	/* Compressed textures */
	{ GL_COMPRESSED_RGB_S3TC_DXT1_EXT,	GL_UNSIGNED_BYTE,	1, GE_DXT1,	1,	copy_dxt1,	0  },
	{ GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,	GL_UNSIGNED_BYTE,	1, GE_DXT1,	1,	copy_dxt1,	TF_ALPHA },
	{ GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,	GL_UNSIGNED_BYTE,	1, GE_DXT3,	1,	copy_dxt3,	TF_ALPHA },
	{ GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,	GL_UNSIGNED_BYTE,	1, GE_DXT5,	1,	copy_dxt5,	TF_ALPHA },

	{ 0, 0 }
};

void glCompressedTexImage2D(GLenum target, GLint level,
			    GLenum internalformat, GLsizei width,
			    GLsizei height, GLint border,
			    GLsizei imageSize, const void *data)
{
	const struct pspgl_texfmt *texfmt;
	struct pspgl_texobj *tobj;
	struct pspgl_teximg *timg;
	GLenum error;

	error = GL_INVALID_VALUE;
	if (!ispow2(width) || !ispow2(height))
		goto out_error;

	if (level < 0 || level > MIPMAP_LEVELS)
		goto out_error;

	if (border != 0)
		goto out_error;

	texfmt = __pspgl_hardware_format(comptexformats, internalformat, GL_UNSIGNED_BYTE);
	error = GL_INVALID_ENUM;
	if (unlikely(texfmt == NULL))
		goto out_error;

	if (!pspgl_curctx->texture.bound)
		glBindTexture(target, 0);

	tobj = pspgl_curctx->texture.bound;
	error = GL_OUT_OF_MEMORY;
	if (unlikely(tobj == NULL))
		goto out_error;

	__pspgl_texobj_unswizzle(tobj);

	timg = __pspgl_teximg_new(data, &pspgl_curctx->unpack,
				  width, height, imageSize, GL_FALSE, texfmt);
	if (timg == NULL)
		return;		/* error already set */

	__pspgl_set_texture_image(tobj, level, timg);

	__pspgl_update_texenv(tobj);
	return;

out_error:
	GLERROR(error);
}
