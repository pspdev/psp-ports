#ifndef _PSPGL_BUFFERS_H
#define _PSPGL_BUFFERS_H

#include "pspgl_internal.h"

struct pspgl_buffer {
	short refcount;
	short generation;	/* generation counter to detect changes */
	signed char mapped;	/* mapping counter */
	unsigned char flags;
#define BF_PINNED_RD	(1<<0)	/* buffer is pinned for reading (by hardware) */
#define BF_PINNED_WR	(1<<1)	/* buffer if pinned for writing (by hardware) */
#define BF_PINNED (BF_PINNED_RD|BF_PINNED_WR)
#define BF_PINNED_FIXED	(1<<2)	/* buffer is pinned permanently */
#define BF_UNMANAGED	(1<<3)	/* buffer is not allocated by us */
#define BF_TRANSIENT	(1<<4)	/* buffer is in the transient pool */
#define BF_MIGRATING	(1<<5)	/* buffer is marked for migration (transient) */

	/* Pointers for the pin list */
	struct pspgl_buffer **pin_prevp;
	struct pspgl_buffer  *pin_next;

	/* Pointer used for the global buffer list */
	struct pspgl_buffer *list_prev;
	struct pspgl_buffer *list_next;

	void *base;
	GLsizeiptr size;
};

struct pspgl_bufferobj {
	int refcount;

	GLenum usage, access;
	GLboolean mapped;	/* glMapBuffer called */

	struct pspgl_buffer *data;
};

/* Create new buffer, but does not allocate any storage for it.
   Returns a buffer with a refcount of 1 */
struct pspgl_bufferobj *__pspgl_bufferobj_new(struct pspgl_buffer *data)
	__attribute__((malloc));

/* Decrements refcount, and frees if it hits 0 */
void __pspgl_bufferobj_free(struct pspgl_bufferobj *);

/* Convert a buffer+offset into a pointer; if buffer is NULL, then
   offset is returned as a pointer.  This pointer has not been mapped,
   and should not be dereferenced. */
void *__pspgl_bufferobj_deref(const struct pspgl_bufferobj *buf, void *offset)
	__attribute__((pure));

/* Helper to map a buffer and return a pointer to an offset; if buffer
   is NULL, then offset is returned as a pointer. */
void *__pspgl_bufferobj_map(const struct pspgl_bufferobj *buf, GLenum access, void *off);
/* Unmap a buffer mapped above.  Should always be paired. */
void __pspgl_bufferobj_unmap(const struct pspgl_bufferobj *buf, GLenum access);

/* Return a bufferobj ** for a particular target.  Returns NULL if target
   is unknown/invalid, and sets GLERROR appropriately. */
struct pspgl_bufferobj **__pspgl_bufferobj_for_target(GLenum target);

/* Allocate and initialize a buffer, allocating data according the "usage" */
struct pspgl_buffer *__pspgl_buffer_new(GLsizeiptr size, GLenum usage)
	__attribute__((malloc));

/* Decrement a buffer's count and free if it hits 0 */
void __pspgl_buffer_free(struct pspgl_buffer *data);

/* Map a buffer for access type "access".  May be called repeatedly. */
void *__pspgl_buffer_map(struct pspgl_buffer *data, GLenum access);

/* Unmap a buffer and do the appropriate fixups.  Must be paired with
   buffer_map(). */
void  __pspgl_buffer_unmap(struct pspgl_buffer *data, GLenum access);

/* Buffer wants to be in vidmem (a hint, not an absolute requirement) */
void __pspgl_buffer_want_vidmem(struct pspgl_buffer *buf);

/* Wait for hardware to finished with a buffer */
void __pspgl_buffer_dlist_sync(struct pspgl_buffer *data);

#endif	/* PSPGL_BUFFERS_H */
