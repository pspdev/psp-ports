#ifndef __PSPGLU_H
#define __PSPGLU_H

#include <GL/gl.h>

#define RGBA(r,g,b,a)	((((a)&0xff) << 24) | \
			 (((b)&0xff) << 16) | \
			 (((g)&0xff) <<  8) | \
			 (((r)&0xff) <<  0))

#define R(rgba)	(((rgba) & 0x000000ff) >> 0)
#define G(rgba)	(((rgba) & 0x0000ff00) >> 8)
#define B(rgba)	(((rgba) & 0x00ff0000) >>16)
#define A(rgba)	(((rgba) & 0xff000000) >>24)


struct format;
typedef unsigned pix_t;

typedef void (*unpack_t)(const struct format *fmt, const void *src, pix_t *dst, int count);
typedef void (*pack_t)(const struct format *fmt, const pix_t *src, void *dst, int count);

struct format {
	GLenum format;
	GLenum type;

	GLubyte channels;
	GLubyte size;

	unpack_t unpack;
	pack_t pack;
};

const struct format *__pspglu_getformat(GLenum format, GLenum type);

#endif /* PSPGLU_H */
