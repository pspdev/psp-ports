#include <stdlib.h>
#include <malloc.h>

#include <psputils.h>
#include <pspge.h>

#include "pspgl_internal.h"
#include "pspgl_buffers.h"
#include "pspgl_dlist.h"

static int list_pin;		/* if != 0, prevent buffer list rearrangements */

/* All living buffers are kept in a circular list, with list_base
   pointing to the head; list_base->list_prev is the tail.  list_base
   is NULL if the list is empty. */
static struct pspgl_buffer *list_base = NULL;

/* Freelist of buffers, for quick allocation */
static struct pspgl_buffer *buffer_freelist = NULL;

static void buffer_remove(struct pspgl_buffer *b)
{
	assert(b->list_next != NULL);
	assert(b->list_prev != NULL);

	if (list_base == b) {
		list_base = b->list_next; /* shift to next head */
		if (list_base == b)
			list_base = NULL;	/* was last node on the list */
	}

	b->list_prev->list_next = b->list_next;
	b->list_next->list_prev = b->list_prev;

	b->list_next = b->list_prev = NULL;
}

static inline void buffer_insert_after(struct pspgl_buffer *after, struct pspgl_buffer *b)
{
	assert(b->list_next == NULL);
	assert(b->list_prev == NULL);
	assert(after->list_next != NULL);
	assert(after->list_prev != NULL);

	b->list_prev = after;
	b->list_next = after->list_next;

	after->list_next->list_prev = b;
	after->list_next = b;
}

static inline void buffer_insert_head(struct pspgl_buffer *b)
{
	if (list_base == NULL) {
		b->list_next = b;
		b->list_prev = b;
	} else
		buffer_insert_after(list_base, b);

	list_base = b;
}

static inline void buffer_insert_tail(struct pspgl_buffer *b)
{
	if (list_base == NULL) {
		b->list_next = b;
		b->list_prev = b;
		list_base = b;
	} else
		buffer_insert_after(list_base->list_prev, b);
}

struct pspgl_bufferobj *__pspgl_bufferobj_new(struct pspgl_buffer *data)
{
	struct pspgl_bufferobj *bufp;

	bufp = malloc(sizeof(*bufp));

	if (bufp) {
		bufp->refcount = 1;
		bufp->usage = 0;
		bufp->access = 0;
		bufp->mapped = GL_FALSE;

		bufp->data = data;

		if (data != NULL)
			data->refcount++;
	}

	return bufp;
}

void __pspgl_bufferobj_free(struct pspgl_bufferobj *bufp)
{
	assert(bufp->refcount > 0);

	if (--bufp->refcount)
		return;

	if (bufp->data)
		__pspgl_buffer_free(bufp->data);

	free(bufp);
}

void *__pspgl_bufferobj_deref(const struct pspgl_bufferobj *bufp, void *off)
{
	if (bufp == NULL || bufp->data == NULL)
		return off;

	return bufp->data->base + (off - NULL);
}

void *__pspgl_bufferobj_map(const struct pspgl_bufferobj *buf, GLenum access, void *off)
{
	if (buf && buf->data)
		off += __pspgl_buffer_map(buf->data, access) - NULL;

	return off;
}

void __pspgl_bufferobj_unmap(const struct pspgl_bufferobj *buf, GLenum access)
{
	if (buf && buf->data)
		__pspgl_buffer_unmap(buf->data, access);
}

static int is_edram_addr(void *p)
{
	static void *edram_start, *edram_end;

	if (edram_end == NULL) {
		edram_start = sceGeEdramGetAddr();
		edram_end = sceGeEdramGetAddr() + sceGeEdramGetSize();
	}

	return (p >= edram_start) && (p < edram_end);
}

/* Kick things out of vidmem until there's enough free space */
static void evict_vidmem(size_t need)
{
	size_t evicted = 0;

	/* If we're allocating lots of small stuff and we need to
	   evict, then we may as well try to amortize the eviction for
	   multiple allocations by doing a bit more work now. */
	if (need < 64*1024)
		need *= 4;

	if (list_base) {
		struct pspgl_buffer *buf;

		list_pin++;

		buf = list_base;
		do {
			if (is_edram_addr(buf->base)) {
				evicted += __pspgl_vidmem_evict(buf);

				if (evicted >= need)
					break;
			}
			buf = buf->list_next;
		} while(buf != list_base);

		list_pin--;
	}

	/* Synchronize evictions to actually free the memory */
	if (evicted > 0) {
		__pspgl_moved_textures();
		glFinish();
	}
}

static GLboolean __pspgl_buffer_init(struct pspgl_buffer *buf,
				     GLsizeiptr size, GLenum usage)
{
	void *p = NULL;
	int try_hard = 0;

	size = ROUNDUP(size, CACHELINE_SIZE);

	buf->base = NULL;
	buf->size = size;

	switch(usage) {
	case GL_STATIC_COPY_ARB:	/* must be in edram */
	case GL_DYNAMIC_COPY_ARB:
	case GL_STREAM_COPY_ARB:
		/* We'll try hard to get vidmem, but it doesn't matter
		   if we fail because the buffer will be moved into
		   vidmem when needed. */
		try_hard = 1;
		/* FALLTHROUGH */

	case GL_STATIC_DRAW_ARB:	/* nice to have in edram */
	case GL_STATIC_READ_ARB:
	case GL_DYNAMIC_READ_ARB:
		if (__pspgl_vidmem_alloc(buf))
			p = buf->base;
		else if (try_hard) {
			/* Work hard to get the memory... */

			/* If there just isn't enough space, evict some buffers */
			if (__pspgl_vidmem_avail() < size)
				evict_vidmem(size);

			/* Try again after some evicitions */
			if (__pspgl_vidmem_alloc(buf))
				p = buf->base;
			else if (__pspgl_vidmem_compact()) {
				__pspgl_moved_textures();

				/* and one last time before giving up */
				if (__pspgl_vidmem_alloc(buf))
					p = buf->base;
			}
		}
		break;

	case GL_DYNAMIC_DRAW_ARB:	/* prefer in system memory */
	case GL_STREAM_READ_ARB:
	case GL_STREAM_DRAW_ARB:
		/* fallthrough to allocation */
		break;

	default:
		GLERROR(GL_INVALID_ENUM);
		return GL_FALSE;
	}

	if (p == NULL) {
		p = memalign(CACHELINE_SIZE, size);

		if (p == NULL)
			return GL_FALSE;
	}

  	/* put cache into appropriate unmapped state */
	sceKernelDcacheWritebackInvalidateRange(p, size);

	buf->refcount = 1;
	buf->mapped = 0;
	buf->flags = 0;
	buf->generation = 0;

	buf->pin_prevp = NULL;
	buf->pin_next = NULL;

	buf->list_prev = NULL;
	buf->list_next = NULL;

	buf->base = p;
	buf->size = size;

	buffer_insert_tail(buf);

	return GL_TRUE;
}

struct pspgl_buffer *__pspgl_buffer_new(GLsizeiptr size, GLenum usage)
{
	struct pspgl_buffer *ret;

	if (likely(buffer_freelist != NULL)) {
		ret = buffer_freelist;
		buffer_freelist = buffer_freelist->list_next;
	} else
		ret = malloc(sizeof(*ret));

	if (likely(ret != NULL))
		if (!__pspgl_buffer_init(ret, size, usage)) {
			free(ret);
			ret = NULL;
		}

	return ret;
}

void __pspgl_buffer_free(struct pspgl_buffer *data)
{
	assert(data->refcount > 0);

	if (--data->refcount)
		return;

	buffer_remove(data);

	if (data->base) {
		if (is_edram_addr(data->base))
			__pspgl_vidmem_free(data);
		else
			free(data->base);
	}

	data->list_next = buffer_freelist;
	buffer_freelist = data;
}

void __pspgl_buffer_want_vidmem(struct pspgl_buffer *buf)
{
	/* Move buffer away from eviction end of the list */
	if (!list_pin) {
		buffer_remove(buf);
		buffer_insert_tail(buf);
	}
}


struct pspgl_bufferobj **__pspgl_bufferobj_for_target(GLenum target)
{
	struct pspgl_bufferobj **ret = NULL;

	switch(target) {
	case GL_ARRAY_BUFFER_ARB:
		ret = &pspgl_curctx->vertex_array.arraybuffer;
		break;

	case GL_ELEMENT_ARRAY_BUFFER_ARB:
		ret = &pspgl_curctx->vertex_array.indexbuffer;
		break;

	case GL_PIXEL_PACK_BUFFER_ARB:
		ret = &pspgl_curctx->pack.pbo;
		break;

	case GL_PIXEL_UNPACK_BUFFER_ARB:
		ret = &pspgl_curctx->unpack.pbo;
		break;

	default:
		GLERROR(GL_INVALID_ENUM);
		break;
	}

	return ret;
}

void *__pspgl_buffer_map(struct pspgl_buffer *data, GLenum access)
{
	void *p = data->base;

	assert(data->mapped >= 0);

	switch(access) {
	case GL_READ_WRITE_ARB:
		/* FALLTHROUGH */

	case GL_READ_ONLY_ARB:
		/* Need to invalidate if written by hardware, but only the first time */
		if (!data->mapped)
			sceKernelDcacheInvalidateRange(data->base, data->size);
		break;

	case GL_WRITE_ONLY_ARB:
		/* Write-only streams are uncached to prevent cache
		   pollution.  If data->mapped != 0, this should be a
		   no-op. */
		p = __pspgl_uncached(p, data->size);
		break;

	default:
		GLERROR(GL_INVALID_ENUM);
		return NULL;
	}

	/* Buffers at the head are first to be moved into system
	   memory.  Mapping means we want it in system memory if
	   possible, so make it more likely.  XXX This is an overly
	   simplistic policy, and there's no corresponding mechanism
	   to move things into vidmem yet. */
	if (!list_pin) {
		buffer_remove(data);
		buffer_insert_head(data);
	}

	data->mapped++;

	return p;
}

void __pspgl_buffer_unmap(struct pspgl_buffer *data, GLenum access)
{
	assert(data->mapped > 0);

	if (--data->mapped > 0)
		return;

	switch(access) {
	case GL_READ_ONLY_ARB:
		/* do nothing; no dirty cache lines */
		break;

	case GL_READ_WRITE_ARB:
		sceKernelDcacheWritebackInvalidateRange(data->base, data->size);
		break;

	case GL_WRITE_ONLY_ARB:
		/* do nothing; all uncached */
		break;

	default:
		return;
	}

	if (access != GL_READ_ONLY_ARB)
		data->generation++;
}

static int buffer_synced(void *p)
{
	struct pspgl_buffer *buf = (struct pspgl_buffer *)p;

	return (buf->flags & BF_PINNED) == 0;
}

/* Wait until the last hardware use of a databuffer has happened. If
   the caller wants to use the buffer after this call, it must
   increment the refcount to prevent it from being (potentially)
   freed. */
void __pspgl_buffer_dlist_sync(struct pspgl_buffer *data)
{
	data->refcount++;	/* prevent freeing */

	if (data->flags & BF_PINNED) {
		__pspgl_dlist_submit();
		__pspgl_dlist_await_completion(buffer_synced, data);
	}

	assert((data->flags & BF_PINNED) == 0);

	__pspgl_buffer_free(data); /* drop refcount */
}
