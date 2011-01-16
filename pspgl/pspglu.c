#include <string.h>
#include "pspglu.h"

static void unpack_8888(const struct format *fmt, const void *src, pix_t *dst,
			int count)
{
	memcpy(dst, src, count*fmt->size);
}

static void pack_8888(const struct format *fmt, const pix_t *src, void *dst,
		      int count)
{
	memcpy(dst, src, count*fmt->size);
}

static void unpack_888(const struct format *fmt, const void *src, pix_t *dst,
		       int count)
{
	const GLubyte *srcp = src;

	while(count--) {
		*dst++ = RGBA(srcp[0], srcp[1], srcp[2], 255);
		srcp += 3;
	}
}

static void pack_888(const struct format *fmt, const pix_t *src, void *dst, int count)
{
	GLubyte *dstp = dst;

	while(count--) {
		pix_t p = *src++;
		dstp[0] = R(p);
		dstp[1] = G(p);
		dstp[2] = B(p);
		dstp += 3;
	}
}

static void unpack_5551(const struct format *fmt, const void *src, pix_t *dst,
			int count)
{
	const GLushort *srcp = src;
	pix_t alpha = 0;
	if (fmt->channels == 3)
		alpha = RGBA(0,0,0,0xff);

	while(count--) {
		GLushort p = *srcp++;
		pix_t a = (p & 0x0001) >>  0;
		pix_t r = (p & 0xf800) >> 11;
		pix_t g = (p & 0x07c0) >>  6;
		pix_t b = (p & 0x0036) >>  1;

		*dst++ = RGBA(r * 255 / 31,
			      g * 255 / 31,
			      b * 255 / 31,
			      a * 255 / 1) | alpha;
	}
}

static void pack_5551(const struct format *fmt, const pix_t *src, void *dst,
		      int count)
{
	GLushort *dstp = dst;

	while(count--) {
		pix_t p = *src++;

		*dstp++=(((R(p) * 31 / 255) << 11) & 0xf800) |
			(((G(p) * 31 / 255) <<  6) & 0x07c0) |
			(((B(p) * 31 / 255) <<  1) & 0x0036) |
			(((A(p) *  1 / 255) <<  0) & 0x0001);
	}
}

static void unpack_1555(const struct format *fmt, const void *src, pix_t *dst,
			int count)
{
	const GLushort *srcp = src;
	pix_t alpha = 0;
	if (fmt->channels == 3)
		alpha = RGBA(0,0,0,0xff);

	while(count--) {
		GLushort p = *srcp++;
		pix_t a = (p & 0x8000) >> 15;
		pix_t r = (p & 0x001f) >>  0;
		pix_t g = (p & 0x03e0) >>  5;
		pix_t b = (p & 0x7c00) >> 10;

		*dst++ = RGBA(r * 255 / 31,
			      g * 255 / 31,
			      b * 255 / 31,
			      a * 255 / 1) | alpha;
	}
}

static void pack_1555(const struct format *fmt, const pix_t *src, void *dst,
		      int count)
{
	GLushort *dstp = dst;

	while(count--) {
		pix_t p = *src++;

		*dstp++=(((R(p) * 31 / 255) <<  0) & 0x001f) |
			(((G(p) * 31 / 255) <<  5) & 0x03e0) |
			(((B(p) * 31 / 255) << 10) & 0x7c00) |
			(((A(p) *  1 / 255) << 15) & 0x8000);
	}
}

static void unpack_565(const struct format *fmt, const void *src, pix_t *dst,
		       int count)
{
	const GLushort *srcp = src;

	while(count--) {
		GLushort p = *srcp++;
		pix_t r = (p & 0xf800) >> 11;
		pix_t g = (p & 0x07e0) >> 5;
		pix_t b = (p & 0x001f) >> 0;

		*dst++ = RGBA(r * 255 / 31,
			      g * 255 / 63,
			      b * 255 / 31,
			      255);
	}
}

static void pack_565(const struct format *fmt, const pix_t *src, void *dst,
		       int count)
{
	GLushort *dstp = dst;

	while(count--) {
		pix_t p = *src++;

		*dstp++=(((R(p) * 31 / 255) << 11) & 0xf800) |
			(((G(p) * 63 / 255) <<  5) & 0x07e0) |
			(((B(p) * 31 / 255) <<  0) & 0x001f);
	}
}

static void unpack_4444(const struct format *fmt, const void *src, pix_t *dst,
			int count)
{
	const GLushort *srcp = src;
	pix_t alpha = 0;
	if (fmt->channels == 3)
		alpha = RGBA(0,0,0,255);

	while(count--) {
		GLushort p = *srcp++;
		pix_t a = (p & 0xf000) >> 12;
		pix_t r = (p & 0x000f) >>  0;
		pix_t g = (p & 0x00f0) >>  4;
		pix_t b = (p & 0x0f00) >>  8;

		*dst++ = RGBA(r * 255 / 15,
			      g * 255 / 15,
			      b * 255 / 15,
			      a * 255 / 15) | alpha;
	}
}

static void pack_4444(const struct format *fmt, const pix_t *src, void *dst,
		      int count)
{
	GLushort *dstp = dst;

	while(count--) {
		pix_t p = *src++;

		*dstp++=(((R(p) * 15 / 255) <<  0) & 0x000f) |
			(((G(p) * 15 / 255) <<  4) & 0x00f0) |
			(((B(p) * 15 / 255) <<  8) & 0x0f00) |
			(((A(p) * 15 / 255) << 12) & 0xf000);
	}
}

static void unpack_A(const struct format *fmt, const void *src, pix_t *dst,
		     int count)
{
	const GLubyte *srcp = src;

	while(count--)
		*dst++ = RGBA(0,0,0,*srcp++);
}

static void pack_A(const struct format *fmt, const pix_t *src, void *dst,
		   int count)
{
	GLubyte *dstp = dst;

	while(count--)
		*dstp++ = A(*src++);
}

static void unpack_L(const struct format *fmt, const void *src, pix_t *dst,
		     int count)
{
	const GLubyte *srcp = src;

	while(count--) {
		GLubyte p = *srcp++;

		*dst++ = RGBA(p,p,p,255);
	}
}

static void pack_L(const struct format *fmt, const pix_t *src, void *dst,
		   int count)
{
	GLubyte *dstp = dst;

	while(count--)
		*dstp++ = R(*src++);
}

static void unpack_LA(const struct format *fmt, const void *src, pix_t *dst,
		     int count)
{
	const GLubyte *srcp = src;

	while(count--) {
		GLubyte l = *srcp++;
		GLubyte a = *srcp++;

		*dst++ = RGBA(l,l,l,a);
	}
}

static void pack_LA(const struct format *fmt, const pix_t *src, void *dst,
		   int count)
{
	GLubyte *dstp = dst;

	while(count--) {
		pix_t p = *src++;
		*dstp++ = R(p);
		*dstp++ = A(p);
	}
}

const static struct format formats[] = {
	{ GL_RGBA, GL_UNSIGNED_BYTE,			4, 4, unpack_8888, pack_8888 },
	{ GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1,		4, 2, unpack_5551, pack_5551 },
	{ GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4,		4, 2, unpack_4444, pack_4444 },
	{ GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV,	4, 2, unpack_1555, pack_1555 },
	{ GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4_REV,	4, 2, unpack_4444, pack_4444 },
	{ GL_ABGR_EXT, GL_UNSIGNED_SHORT_4_4_4_4,	4, 2, unpack_4444, pack_4444 },

	{ GL_RGB,  GL_UNSIGNED_BYTE,			3, 3, unpack_888, pack_888 },
	{ GL_RGB,  GL_UNSIGNED_SHORT_5_6_5,		3, 2, unpack_565, pack_565 },
	{ GL_RGB,  GL_UNSIGNED_SHORT_5_6_5_REV,		3, 2, unpack_565, pack_565 },
	{ GL_RGB,  GL_UNSIGNED_SHORT_5_5_5_1,		3, 2, unpack_5551, pack_5551 },
	{ GL_RGB , GL_UNSIGNED_SHORT_1_5_5_5_REV,	3, 2, unpack_1555, pack_1555 },
	{ GL_RGB,  GL_UNSIGNED_SHORT_4_4_4_4,		3, 2, unpack_4444, pack_4444 },
	{ GL_RGB,  GL_UNSIGNED_SHORT_4_4_4_4_REV,	3, 2, unpack_4444, pack_4444 },

	{ GL_LUMINANCE,		GL_UNSIGNED_BYTE,	1, 1, unpack_L,  pack_L },
	{ GL_LUMINANCE_ALPHA,	GL_UNSIGNED_BYTE,	1, 1, unpack_LA, pack_LA },
	{ GL_ALPHA,		GL_UNSIGNED_BYTE,	1, 1, unpack_A,  pack_A },

	{0,0}
};

const struct format *__pspglu_getformat(GLenum format, GLenum type)
{
	const struct format *ret;

	for(ret = formats; ret->format != 0; ret++)
		if (ret->format == format && ret->type == type)
			return ret;

	return NULL;
}

