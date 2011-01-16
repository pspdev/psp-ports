#include <string.h>
#include <psputils.h>
#include <pspge.h>

#include "pspgl_internal.h"
#include "pspgl_texobj.h"
#include "pspgl_dlist.h"

void glReadPixels( GLint x, GLint y,
		   GLsizei width, GLsizei height,
		   GLenum format, GLenum type,
		   GLvoid *pixels )
{
	struct pspgl_surface *read;
	struct pspgl_buffer *framebuffer;
	unsigned fb_offset;
	unsigned hwsize;
	unsigned pixfmt;
	int dest_x, dest_y;
	GLsizei dst_stride, src_stride;
	GLenum error;
	const struct pixelstore *pack = &pspgl_curctx->pack;

	read = pspgl_curctx->read;

	if (format != GL_DEPTH_COMPONENT) {
		const struct pspgl_texfmt *fmt;	

		fmt = __pspgl_hardware_format(__pspgl_texformats, format, type);

		error = GL_INVALID_ENUM;
		if (unlikely(fmt->hwformat != read->pixfmt))
			goto out_error;

		error = GL_INVALID_OPERATION;
		if ((fmt->flags & TF_NATIVE) == 0)
			goto out_error;

		framebuffer = *(read->read);
		fb_offset = 0;

		hwsize = fmt->hwsize;
		pixfmt = read->pixfmt;
	} else {
		error = GL_INVALID_OPERATION;
		if (read->depth_buffer == NULL)
			goto out_error;

		error = GL_INVALID_ENUM;
		if (type != GL_UNSIGNED_SHORT)
			goto out_error;

		/* The PSP provides a window into VRAM which makes the
		   depth buffer look properly linearized. */
		framebuffer = read->depth_buffer;
		fb_offset = sceGeEdramGetSize() * 3;

		hwsize = 2;
		pixfmt = GE_RGBA_4444; /* any 16-bit sized format */
	}

	dest_x = 0;
	dest_y = 0;

	dst_stride = width;
	if (pack->row_length != 0)
		dst_stride = pack->row_length;
	if (pack->alignment > hwsize)
		dst_stride = ROUNDUP(dst_stride, pack->alignment / hwsize);
	error = GL_INVALID_OPERATION;
	if (pack->pbo && pack->pbo->mapped)
		goto out_error;
	pixels += (pack->skip_rows * dst_stride + pack->skip_pixels) * hwsize;

	if (x < 0) {
		x = -x;
		dest_x += x;
		width -= x;
		x = 0;
	}
	if ((x + width) > read->width)
		width -= (x + width) - read->width;

	if (y < 0) {
		y = -y;
		dest_y += y;
		height -= y;
		y = 0;
	}
	if ((y + height) > read->height)
		height -= (y + height) - read->height;

	error = GL_INVALID_VALUE;
	if (unlikely(width <= 0) || unlikely(height <= 0))
		goto out_error;

	src_stride = read->pixelperline;

	if (!pack->invert) {
		/* The framebuffer and the output are upside down with
		   respect to each other, so we need to flip the image (in the
		   framebuffer, lower addresses are in the upper-left, but for
		   textures, lower addresses are lower-left). */
		y = read->height - y;
		src_stride = -src_stride;
	}

	if (((unsigned)pixels & 0xf) != 0) {
		/* Unaligned dest buffer; we can't use DMA */
		const void *map;
		const void *src;
		void *dst;

		src_stride *= hwsize;
		dst_stride *= hwsize;

		__pspgl_buffer_dlist_sync(framebuffer);

		map = __pspgl_buffer_map(framebuffer, GL_READ_ONLY_ARB);
		src = map + fb_offset + (y * read->pixelperline + x) * hwsize;
		dst = __pspgl_bufferobj_map(pack->pbo, GL_WRITE_ONLY_ARB,
					    pixels + (dest_y * dst_stride + dest_y) * hwsize);

		while(height--) {
			memcpy(dst, src, width * hwsize);
			dst += dst_stride;
			src += src_stride;
		}

		__pspgl_bufferobj_unmap(pack->pbo, GL_WRITE_ONLY_ARB);
		__pspgl_buffer_unmap(framebuffer, GL_READ_ONLY_ARB);
	} else {
		void *dst = __pspgl_bufferobj_deref(pack->pbo, pixels);

		if (pack->pbo == NULL) {
			/* Make sure the cache has no aliased content
			   before the DMA transfer.  Not necessary if
			   we're copying into a PBO, since we've already
			   made sure its unmapped. */
			sceKernelDcacheWritebackInvalidateRange(dst, dst_stride*height*hwsize);
		}

		__pspgl_copy_pixels(framebuffer->base + fb_offset, src_stride, x, y,
				    dst, dst_stride, 0, 0,
				    width, height, pixfmt);

		if (pack->pbo != NULL)
			__pspgl_dlist_pin_buffer(pack->pbo->data, BF_PINNED_WR);
		else {
			/* Wait for copy to finish if we're copying
			   directly into application address space */
			glFinish();
		}
	}
	return;

  out_error:
	GLERROR(error);
}
