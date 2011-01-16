#include <GL/glu.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "pspglu.h"

#define FIXED(x)	((int)((x) * 65536))
#define INT(x)		((x) / 65536)
#define FRAC(x)		((x) & 0xffff)

/* weight is a 1.16 fixed-point fraction */
static inline pix_t lerp(pix_t a, pix_t b, unsigned weight)
{
	unsigned omw = FIXED(1) - weight;

	return RGBA(INT(R(a) * omw + R(b) * weight),
		    INT(G(a) * omw + G(b) * weight),
		    INT(B(a) * omw + B(b) * weight),
		    INT(A(a) * omw + A(b) * weight));
}

static void rescale_horiz_up(const pix_t *image, pix_t *ret,
			     GLsizei src_width, GLsizei dst_width, GLsizei height)
{
	unsigned scale = FIXED(src_width) / dst_width;
	unsigned y;

	if (0)
		printf("\nscale %dx%d -> %dx%d, scale=%g 1/scale=%g\n",
		       src_width, height, dst_width, height,
		       scale / 65536., 65536. / scale);

	for(y = 0; y < height; y++, image += src_width) {
		unsigned src_x, dst_x;

		for(src_x = dst_x = 0; dst_x < dst_width; dst_x++, src_x += scale) {
			unsigned frac_src_x = FRAC(src_x);
			const pix_t *pix = &image[INT(src_x)];
			int next = 0;
			if (INT(src_x) + 1 < src_width)
				next = 1;

			*ret++ = lerp(pix[0], pix[next], frac_src_x);
		}
	}
}

static void rescale_horiz_down(const pix_t *image, pix_t *ret,
			       GLsizei src_width, GLsizei dst_width, GLsizei height)
{
	unsigned scale = FIXED(src_width) / dst_width;
	unsigned y;

	for(y = 0; y < height; y++, image += src_width) {
		unsigned src_x, dst_x;

		for(src_x = dst_x = 0; dst_x < dst_width; dst_x++, src_x += scale) {
			unsigned r,g,b,a;
			unsigned int_src_x = INT(src_x);
			int i, count;

			r=g=b=a = 0;

			count = 0;
			for(i = int_src_x; i < INT(src_x + scale + 65535); i++) {
				int c = (i >= src_width) ? src_width-1 : i;

				count++;
				r += R(image[c]);
				g += G(image[c]);
				b += B(image[c]);
				a += A(image[c]);
			}

			*ret++ = RGBA(r / count,
				      g / count,
				      b / count,
				      a / count);
		}
	}
}

static pix_t *rescale_horiz(pix_t *image,
			    GLsizei src_width, GLsizei dst_width, GLsizei height)
{
	pix_t *ret;

	if (image == NULL)
		return NULL;

	ret = malloc(dst_width * height * sizeof(pix_t));

	if (ret == NULL)
		goto out;

	if (dst_width < src_width)
		rescale_horiz_down(image, ret, src_width, dst_width, height);
	else
		rescale_horiz_up(image, ret, src_width, dst_width, height);

  out:
	free(image);
	return ret;
}

static void rescale_vert_up(const pix_t *image, pix_t *ret,
			     GLsizei width, GLsizei src_height, GLsizei dst_height)
{
	unsigned scale = FIXED(src_height) / dst_height;
	unsigned src_y, dst_y;

	for(dst_y = src_y = 0; dst_y < dst_height; dst_y++, src_y += scale) {
		unsigned int_src_y = INT(src_y);
		unsigned frac_src_y = FRAC(src_y);
		unsigned x;
		const pix_t *row, *next;

		row = &image[int_src_y * width];
		next = row;
		if (int_src_y+1 < src_height)
			next = row+width;

		for(x = 0; x < width; x++)
			*ret++ = lerp(*row++, *next++, frac_src_y);
	}
}

static void rescale_vert_down(const pix_t *image, pix_t *ret,
			      GLsizei width, GLsizei src_height, GLsizei dst_height)
{
	unsigned scale = FIXED(src_height) / dst_height;
	unsigned r_scale = (FIXED(dst_height) / src_height) / 256;
	unsigned src_y, dst_y;

	if (0)
		printf("\nscale %dx%d -> %dx%d, scale=%g 1/scale=%g\n",
		       width, src_height, width, dst_height,
		       scale / 65536., 65536. / scale);

	for(dst_y = src_y = 0; 
	    dst_y < dst_height; 
	    dst_y++, src_y += scale) {
		int x;
		unsigned box_start = FIXED(1) - FRAC(src_y);
		unsigned box_middle = INT(src_y + scale) - INT(src_y + FIXED(1));
		unsigned box_end = FRAC(src_y + scale);

		if (0)
			printf("dst_y=%d src_y=%g - %g; box start=%g middle=%d end=%g sum=%g\n",
			       dst_y, 
			       src_y / 65536., (src_y + scale) / 65536.,
			       box_start / 65536.,
			       box_middle,
			       box_end / 65536.,
			       (box_start + FIXED(box_middle) + box_end) / 65536.);

		for(x = 0; x < width; x++) {
			unsigned r,g,b,a;
			const pix_t *row;
			int i;

			r=g=b=a = 0;

			/* 
			               s-----e
			   src   |----|----|----|----|----|----|
			   dst   |-----|-----|-----|-----|-----

			                   s--mm-mm--e
			   src   |--|--|--|--|--|--|--|--|--|--|
			   dst   |---------|---------|---------

			   dst = box filter over src pixels
			 */

			row = &image[(INT(src_y) * width) + x];

			if (box_start) {
				r += R(*row) * box_start;
				g += G(*row) * box_start;
				b += B(*row) * box_start;
				a += A(*row) * box_start;
				row += width;
			}

			for(i = 0; i < box_middle; i++) {
				r += FIXED(R(*row));
				g += FIXED(G(*row));
				b += FIXED(B(*row));
				a += FIXED(A(*row));
				row += width;
			}

			if (box_end) {
				r += R(*row) * box_end;
				g += G(*row) * box_end;
				b += B(*row) * box_end;
				a += A(*row) * box_end;
			}

			*ret++ = RGBA(INT((r / 256) * r_scale),
				      INT((g / 256) * r_scale),
				      INT((b / 256) * r_scale),
				      INT((a / 256) * r_scale));

		}
	}
}

static pix_t *rescale_vert(pix_t *image,
			   GLsizei width, GLsizei src_height, GLsizei dst_height)
{
	pix_t *ret;

	if (image == NULL)
		return NULL;

	ret = malloc(width * dst_height * sizeof(pix_t));

	if (ret == NULL)
		goto out;

	if (dst_height < src_height)
		rescale_vert_down(image, ret, width, src_height, dst_height);
	else
		rescale_vert_up(image, ret, width, src_height, dst_height);

  out:
	free(image);
	return ret;
}


GLint gluScaleImage(GLenum format,
		    GLsizei src_width, GLsizei src_height,
		    GLenum src_type, const GLvoid *src,
		    GLsizei dst_width, GLsizei dst_height,
		    GLenum dst_type, GLvoid *dst)
{
	const struct format *srcfmt = __pspglu_getformat(format, src_type);
	const struct format *dstfmt = __pspglu_getformat(format, dst_type);
	pix_t *image;
	int i;

	if (srcfmt == NULL || dstfmt == NULL)
		return GLU_INVALID_ENUM;

	if (src_width == dst_width &&
	    src_height == dst_height &&
	    src_type == dst_type) {
		memcpy(dst, src, dst_width * dst_height * dstfmt->size);
		return 0;
	}

	image = malloc(src_width * src_height * sizeof(pix_t));
	if (image == NULL)
		return GLU_OUT_OF_MEMORY;
	for(i = 0; i < src_height; i++)
		(*srcfmt->unpack)(srcfmt,
				  src + i * src_width * srcfmt->size,
				  &image[i * src_width],
				  src_width);

	if (src_width != dst_width)
		image = rescale_horiz(image, src_width, dst_width, src_height);

	if (src_height != dst_height)
		image = rescale_vert(image, dst_width, src_height, dst_height);

	if (image == NULL)
		return GL_OUT_OF_MEMORY;

	for(i = 0; i < dst_height; i++)
		(*dstfmt->pack)(dstfmt,
				&image[i * dst_width],
				dst + i * dst_width * dstfmt->size,
				dst_width);
	free(image);
	
	return 0;
}
