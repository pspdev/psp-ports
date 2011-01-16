#include <pspuser.h>
#include <pspge.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "pspgl_internal.h"
#include "pspgl_buffers.h"

void *__pspgl_uncached(void *p, size_t size)
{
	assert(((unsigned long)p & (CACHELINE_SIZE-1)) == 0);
	assert((size & (CACHELINE_SIZE-1)) == 0);

	sceKernelDcacheWritebackInvalidateRange(p, size);

	return (void *)((unsigned long)p | 0x40000000);
}


void __pspgl_log (const char *fmt, ...)
{
	va_list ap;

#if 1
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
#else
	char buf [1024];
	int len;
	int log_fd;

	log_fd = sceIoOpen("ms0:/log.txt", PSP_O_CREAT | PSP_O_APPEND | PSP_O_WRONLY, 0644);

	if (log_fd >= 0) {
		va_start(ap, fmt);
		len = vsnprintf(buf, sizeof(buf), fmt, ap);
		va_end(ap);
		sceIoWrite(log_fd, buf, len);
		sceIoClose(log_fd);
	}
#endif
}


#define PSPGL_GE_DUMPFILE "ms0:/pspgl.ge"

static
void pspgl_ge_writefile (void *buf, unsigned long len)
{
	int fd = sceIoOpen(PSPGL_GE_DUMPFILE, PSP_O_CREAT | PSP_O_APPEND | PSP_O_WRONLY, 0644);
	sceIoWrite(fd, buf, len);
	sceIoClose(fd);
}


void __pspgl_ge_matrix_dump (void)
{
	unsigned long buf [2 + 12 * 16];
	int i;

	buf[0] = PSPGL_GE_DUMP_MATRIX;
	buf[1] = sizeof(buf);

	for (i=0; i<12; i++)
		sceGeGetMtx(i, (float *) &buf[2+i*16]);

	pspgl_ge_writefile(buf, sizeof(buf));
}


void __pspgl_ge_register_dump (void)
{
	unsigned long buf [2 + 256];
	int i;

	buf[0] = PSPGL_GE_DUMP_REGISTERS;
	buf[1] = sizeof(buf);

	for (i=0; i<256; i++)
		buf[2+i] = sceGeGetCmd(i);

	pspgl_ge_writefile(buf, sizeof(buf));
}


void __pspgl_dlist_dump (unsigned long *cmd_buf, unsigned long len)
{
	unsigned long header [3];
	int fd;

	header[0] = PSPGL_GE_DUMP_DLIST;
	header[1] = sizeof(header) + len * sizeof(cmd_buf[0]);
	header[2] = (unsigned long) cmd_buf;

	fd = sceIoOpen(PSPGL_GE_DUMPFILE, PSP_O_CREAT | PSP_O_APPEND | PSP_O_WRONLY, 0644);
	sceIoWrite(fd, header, sizeof(header));
	sceIoWrite(fd, (void *) cmd_buf, len * sizeof(cmd_buf[0]));
	sceIoClose(fd);
}


void __pspgl_vram_dump (void)
{
	unsigned long vram_start = (unsigned long) sceGeEdramGetAddr();
	unsigned long vram_size = (unsigned long) sceGeEdramGetSize() * 4;
	unsigned long header [4];
	unsigned char vram_copy [0x10000];
	int fd;
	int i;

	fd = sceIoOpen(PSPGL_GE_DUMPFILE, PSP_O_CREAT | PSP_O_APPEND | PSP_O_WRONLY, 0644);

	if (pspgl_curctx) {
		struct pspgl_surface *s = pspgl_curctx->draw;
		struct pspgl_dump_surfaces surf;

		header[0] = PSPGL_GE_DUMP_SURFACES;
		header[1] = sizeof(header) + sizeof(surf);
		header[2] = 0;
		header[3] = 0;

		memset(&surf, 0, sizeof(surf));

		surf.pixfmt = s->pixfmt;
		surf.alpha_mask = s->alpha_mask;
		surf.stencil_mask = s->stencil_mask;

		surf.front.start = s->color_front->base - sceGeEdramGetAddr();
		surf.front.size = s->height * s->pixelperline * (s->pixfmt == GE_RGBA_8888 ? 4 : 2);
		surf.front.stride = s->pixelperline;

		surf.back.start = s->color_back->base - sceGeEdramGetAddr();
		surf.back.size = s->height * s->pixelperline * (s->pixfmt == GE_RGBA_8888 ? 4 : 2);
		surf.back.stride = s->pixelperline;

		if (s->depth_buffer) {
			surf.depth.start = s->depth_buffer->base - sceGeEdramGetAddr();
			surf.depth.size = s->height * s->pixelperline * 2;
			surf.depth.stride = s->pixelperline;
		}

		sceIoWrite(fd, header, sizeof(header));
		sceIoWrite(fd, &surf, sizeof(surf));
	}

	header[0] = PSPGL_GE_DUMP_VRAM;
	header[1] = sizeof(header) + vram_size;
	header[2] = vram_start;
	header[3] = vram_size;

	sceIoWrite(fd, header, sizeof(header));

	/* copy in blocks, direct writes from VRAM to file don't seem to work... */
	for (i=0; i<vram_size/sizeof(vram_copy); i++, vram_start+=sizeof(vram_copy)) {
		memcpy(vram_copy, (void *) vram_start, sizeof(vram_copy));
		sceIoWrite(fd, (void *) vram_copy, sizeof(vram_copy));
	}

	sceIoClose(fd);
}

void __pspgl_assert_fail(const char *expr, const void *retaddr, 
			 const char *func, const char *file, int line)
{
	__pspgl_log("ASSERTION FAILURE: %s:%d (%s, called from %p): %s\n",
		    file, line, func, retaddr, expr);
}


#undef calloc
#undef malloc
#undef realloc
#undef free
#include <stdlib.h>

void* __pspgl_calloc (size_t count, size_t size, const char *function, unsigned int line)
{
	void *ptr = calloc(count, size);
	__pspgl_log("%s(count %d, size %d, called from %s: %u): %p\n", __FUNCTION__, count, size, function, line, ptr);
	return ptr;
}


void* __pspgl_realloc (void *ptr, size_t size, const char *function, unsigned int line)
{
	void *new_ptr = realloc(ptr, size);
	__pspgl_log("%s(ptr %p, size %d, called from %s: %u): %p\n", __FUNCTION__, ptr, size, function, line, new_ptr);
	return new_ptr;
}


void __pspgl_free (void *ptr, const char *function, unsigned int line)
{
	__pspgl_log("%s(ptr %p, called from %s: %u)\n", __FUNCTION__, ptr, function, line);
	free(ptr);
}

