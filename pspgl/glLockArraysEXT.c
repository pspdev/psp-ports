#include <stdlib.h>
#include <malloc.h>
#include <pspge.h>
#include <psputils.h>

#include "pspgl_internal.h"
#include "pspgl_buffers.h"

/* Maximum size of cached vertices */
#define MAX_CACHED_ARRAY	(128*1024)

/* Cache current arrays into a buffer in hardware form, if possible.
   Could fail for any number of reasons; returns true if it
   succeeds. */
GLboolean __pspgl_cache_arrays(void)
{
	struct locked_arrays *l = &pspgl_curctx->vertex_array.locked;
	struct pspgl_buffer *cached;
	unsigned size;
	int count;

	if (likely(l->cached_array != NULL)) {
		psp_log("OK: already cached\n");
		return GL_TRUE;
	}

	__pspgl_ge_vertex_fmt(pspgl_curctx, &l->vfmt);

	/* If the vertex is in hardware format, and the vertex arrays
	   are in buffer objects then we can just use them directly as
	   a locked array. */
	if (__pspgl_vertex_is_native(&l->vfmt) &&
	    l->vfmt.nattrib >= 1) {
		struct pspgl_vertex_array *va =  l->vfmt.attribs[0].array;
		struct pspgl_bufferobj *buf = va->buffer;
		int i;

		for (i = 1; buf && i < l->vfmt.nattrib; i++)
			if (l->vfmt.attribs[i].array->buffer != buf)
				buf = NULL;

		if (buf) {
			psp_log("using buffer %p(->%p), offset %d as cached array\n",
				buf, buf->data->base, (va->ptr - NULL));

			cached = buf->data;
			cached->refcount++;

			assert(cached->refcount > 1);

			l->cached_array = cached;
			l->cached_array_offset = (va->ptr - NULL);
			l->cached_first = 0;

			return GL_TRUE;
		}
	}

	size = l->vfmt.vertex_size * l->count;

	psp_log("caching arrays %d-%d, vfmt=%x arrays=%x, %d bytes\n",
		l->first, l->first + l->count, l->vfmt.hwformat, l->vfmt.arrays, size);

	/* No serendipidous locking, so only cache an array if the app
	   explicitly locked the arrays. */
	if (l->count == 0) {
		psp_log("failed: not locked\n");
		return GL_FALSE; /* nothing locked */
	}

	if (size == 0 || size > MAX_CACHED_ARRAY) {
		psp_log("failed: size=%d\n", size);
		return GL_FALSE; /* too small or too big */
	}

	cached = __pspgl_buffer_new(size, GL_STATIC_DRAW_ARB);
	if (cached == NULL) {
		psp_log("failed: out of memory\n");
		return GL_FALSE; /* out of memory */
	}

	l->cached_array = cached;
	l->cached_array_offset = 0;
	l->cached_first = l->first;

	void *p = __pspgl_buffer_map(cached, GL_WRITE_ONLY_ARB);

	count = __pspgl_gen_varray(&l->vfmt, l->first, l->count, p, size);

	__pspgl_buffer_unmap(cached, GL_WRITE_ONLY_ARB);

	if (count != l->count) {
		psp_log("failed: wanted %d vertices, only got %d\n", l->count, count);
		__pspgl_uncache_arrays();
		return GL_FALSE; /* conversion failed */
	}

	psp_log("OK: vertex arrays cached\n");

	return GL_TRUE;
}

void glLockArraysEXT(GLint first, GLsizei count)
{
	struct locked_arrays *l = &pspgl_curctx->vertex_array.locked;
	GLenum error;

	error = GL_INVALID_VALUE;

	if (unlikely(first < 0) || unlikely(count <= 0))
		goto out_error;

	error = GL_INVALID_OPERATION;
	if (l->count != 0)
		goto out_error;

	l->first = first;
	l->count = count;

	psp_log("locking arrays %d %d\n", first, count);

	/* defer actually caching things until first use of arrays */
	return;

  out_error:
	GLERROR(error);
}

/* Clear out the cached array (if any), but doesn't change the locked state */
void __pspgl_uncache_arrays(void)
{
	struct locked_arrays *l = &pspgl_curctx->vertex_array.locked;

	if (l->cached_array)
		__pspgl_buffer_free(l->cached_array);

	l->cached_array = NULL;
	l->vfmt.hwformat = 0;
	l->vfmt.vertex_size = 0;
	l->vfmt.arrays = 0;
}

void glUnlockArraysEXT(void)
{
	struct locked_arrays *l = &pspgl_curctx->vertex_array.locked;

	if (unlikely(l->count == 0)) {
		GLERROR(GL_INVALID_OPERATION);
		return;
	}

	l->first = l->count = 0;

	__pspgl_uncache_arrays();
}
