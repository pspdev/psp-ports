#include "pspgl_internal.h"

static inline int absi(int x)
{
	return (x < 0) ? -x : x;
}

static inline int sgni(int x)
{
	if (x < 0)
		return -1;
	if (x == 0)
		return 0;
	return 1;
}

void __pspgl_copy_pixels(const void *srcbuf, int srcstride, int srcx, int srcy,
			 void *dstbuf, int dststride, int dstx, int dsty,
			 int width, int height, unsigned pixfmt)
{
	sendCommandi(CMD_COPY_SRC, (unsigned)srcbuf);
	sendCommandi(CMD_COPY_DST, (unsigned)dstbuf);

	if (srcstride < 0 || dststride < 0) {
		/* Flipping the image, so copy line-by-line */
		int sdy = sgni(srcstride);
		int ddy = sgni(dststride);

		srcstride = absi(srcstride);
		dststride = absi(dststride);

		sendCommandi(CMD_COPY_SRC_STRIDE, (((unsigned)srcbuf & 0xff000000) >> 8) | srcstride);
		sendCommandi(CMD_COPY_DST_STRIDE, (((unsigned)dstbuf & 0xff000000) >> 8) | dststride);

		sendCommandi(CMD_COPY_SIZE, ((1-1) << 10) | (width-1));

		while(height--) {
			sendCommandi(CMD_COPY_SRC_XY, (srcy << 10) | srcx);
			sendCommandi(CMD_COPY_DST_XY, (dsty << 10) | dstx);

			__pspgl_context_flush_pending_state_changes (pspgl_curctx,
								     CMD_COPY_SRC,
								     CMD_COPY_DST_STRIDE);
			__pspgl_context_flush_pending_state_changes (pspgl_curctx,
								     CMD_COPY_SRC_XY,
								     CMD_COPY_SIZE);
			sendCommandiUncached(CMD_COPY_START, (pixfmt == GE_RGBA_8888));

			srcy += sdy;
			dsty += ddy;
		}
	} else {
		/* Just copy the whole thing in one go */
		sendCommandi(CMD_COPY_SRC_STRIDE, (((unsigned)srcbuf & 0xff000000) >> 8) | srcstride);
		sendCommandi(CMD_COPY_DST_STRIDE, (((unsigned)dstbuf & 0xff000000) >> 8) | dststride);

		sendCommandi(CMD_COPY_SRC_XY, (srcy << 10) | srcx);
		sendCommandi(CMD_COPY_DST_XY, (dsty << 10) | dstx);

		sendCommandi(CMD_COPY_SIZE, ((height-1) << 10) | (width-1));

		__pspgl_context_flush_pending_state_changes (pspgl_curctx,
							     CMD_COPY_SRC,
							     CMD_COPY_DST_STRIDE);
		__pspgl_context_flush_pending_state_changes (pspgl_curctx,
							     CMD_COPY_SRC_XY,
							     CMD_COPY_SIZE);

		sendCommandiUncached(CMD_COPY_START, (pixfmt == GE_RGBA_8888));
	}
}

/* Copy memory with the GE.  Typically used for moving things to/from
   or within vidmem.  Does not synchronize anything, and doesn't pin
   any buffers.  Handles overlapping src and dst. */
void __pspgl_copy_memory(const void *src, void *dst, size_t size)
{
	size_t xfersize;

	psp_log("coping %p->%p %u\n", src, dst, size);

	xfersize = size;

	if ((dst < src && (dst+size) > src) ||
	    (src < dst && (src+size) > dst)) {
		if (dst < src)
			xfersize = src - dst;
		else
			xfersize = dst - src;
		psp_log("   OVERLAPPING: xfersize=%u\n",
			xfersize);
	}

	/* Copy 32bpp "pixels" */
	size /= 4;
	xfersize /= 4;

	/* Copy the data as a series of 512xN blits (or Nx1 for
	   transfers of less than 512 words).  If the src and dst
	   overlap, then the transfer size may be smaller. */
	while(size > 0) {
		unsigned w, h;
		unsigned chunk = size;

		if (chunk > xfersize)
			chunk = xfersize;

		if (chunk < 512) {
			w = chunk;
			h = 1;
		} else {
			w = 512;
			h = chunk / 512;
		}

		psp_log("  chunk %p->%p %dx%d\n",
			src, dst, w, h);

		__pspgl_copy_pixels(src, w, 0, 0,
				    dst, w, 0, 0,
				    w, h, GE_RGBA_8888);

		size -= w * h;
		src += w * h;
		dst += w * h;
	}
}
