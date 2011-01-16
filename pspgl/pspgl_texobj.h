#ifndef __pspgl_texobj_h__
#define __pspgl_texobj_h__

#include <GL/gl.h>
#include <GLES/egl.h>

#include <pspgl_buffers.h>
#include "guconsts.h"

#define MIPMAP_LEVELS		8

struct pspgl_texfmt {
	GLenum format;
	GLenum type;
	unsigned pixsize;

	unsigned hwformat;
	unsigned hwsize;

	void (*convert)(const struct pspgl_texfmt *, void *to, const void *from, unsigned width);

	unsigned flags;
#define TF_ALPHA	(1<<0)
#define TF_NATIVE	(1<<1)

	struct pspgl_teximg *cmap;
};


struct pspgl_texobj {
	unsigned	refcount;
	GLenum		target;		/* either 0 or GL_TEXTURE_2D */
	GLclampf	priority;

	unsigned	flags;
#define TOF_GENERATE_MIPMAPS		(1<<0)
#define TOF_GENERATE_MIPMAP_DEBUG	(1<<1)
#define TOF_SWIZZLED			(1<<2)
#define TOF_FLIPPED			(1<<3)

#define TEXOBJ_NTEXREG	(TEXSTATE_END - TEXSTATE_START + 1)
	uint32_t	ge_texreg[TEXOBJ_NTEXREG];
	const struct pspgl_texfmt *texfmt;

	struct pspgl_teximg *cmap;

	struct pspgl_teximg *images[MIPMAP_LEVELS];
};

struct pspgl_teximg {
	struct pspgl_buffer	*image;		/* image pixels */
	unsigned		offset;		/* byte offset of pixels in image buffer */

	unsigned	width, height;
	unsigned	stride;

	const struct pspgl_texfmt *texfmt;
};

extern const struct pspgl_texfmt __pspgl_texformats[];

extern struct pspgl_texobj* __pspgl_texobj_new (GLuint id, GLenum target);
extern void __pspgl_texobj_free (struct pspgl_texobj *t);
extern void __pspgl_texobj_unswizzle(struct pspgl_texobj *tobj);
extern struct pspgl_teximg *__pspgl_texobj_cmap(const struct pspgl_texobj *t);

extern struct pspgl_teximg *__pspgl_teximg_new(const void *pixels, const struct pixelstore *unpack,
					       unsigned width, unsigned height, unsigned size,
					       GLboolean swizzle, const struct pspgl_texfmt *texfmt);
struct pspgl_teximg *__pspgl_teximg_from_buffer(struct pspgl_buffer *buffer, unsigned offset,
						unsigned width, unsigned height, unsigned stride,
						const struct pspgl_texfmt *texfmt);

extern void __pspgl_teximg_free(struct pspgl_teximg *timg);

extern void __pspgl_update_texenv(struct pspgl_texobj *tobj);

extern const struct pspgl_texfmt *__pspgl_hardware_format(const struct pspgl_texfmt *, GLenum format, GLenum type);


/*
   Given an offset in a texture 2^log2_w bytes wide, return the byte
   offset in swizzledom.

   Variables are:
   bx,by: coords of 16x8 block within the texture
   mx,my: offsets within the block

   The swizzle function is essentially a 3-bit left-rotate of bits
   4-(log2(width)+7) within the offset.  The by and mx fields are
   unchanged after the operation.

   Unswizzle is a 3-bit right-rotate of the same bitfield.

                              v log2(width)+3         v
   ...|19|18|17|16|15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
             ...by by by by by my my my bx bx bx bx bx mx mx mx mx

                               --<--<--<--<--<--<--<-- <rotate

             ...by by by by by bx bx bx bx bx my my my mx mx mx mx
   ...|19|18|17|16|15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
                              ^log2(width)+3 ^        ^

                               -->-->-->-->-->-->-->-- >rotate

   ...|19|18|17|16|15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
             ...by by by by by my my my bx bx bx bx bx mx mx mx mx

 */
static inline unsigned swizzle(unsigned offset, unsigned log2_w)
{
	unsigned fixed = offset & ((~7 << log2_w) | 0xf);
	unsigned w_mask = (1 << log2_w) - 1;
	unsigned bx = offset & w_mask & ~0xf;
	unsigned my = offset & (7 << log2_w);

	return fixed | (bx << 3) | (my >> (log2_w - 4));
}

static inline unsigned unswizzle(unsigned offset, unsigned log2_w)
{
	unsigned w_mask = (1 << log2_w) - 1;
	unsigned fixed = offset & ((~7 << log2_w) | 0xf);
	unsigned bx = offset & ((w_mask & 0xf) << 7);
	unsigned my = offset & 0x70;

	return fixed | (bx >> 3) | (my << (log2_w - 4));
}


#endif

