#include "pspgl_internal.h"
#include "pspgl_buffers.h"

void glBindBufferARB(GLenum target, GLuint id)
{
	struct hashtable *hash = &pspgl_curctx->shared->buffers;
	struct pspgl_bufferobj *bufp, *prev, **prevp;

	prevp = __pspgl_bufferobj_for_target(target);

	if (prevp == NULL)
		return;

	prev = *prevp;

	bufp = NULL;
	if (id != 0) {
		bufp = __pspgl_hash_lookup(hash, id);

		if (bufp == NULL) {
			bufp = __pspgl_bufferobj_new(NULL);
			if (unlikely(bufp == NULL))
				goto out_error;

			__pspgl_hash_insert(hash, id, bufp);
		}
	}

	if (bufp == prev)
		return;

	if (prev) {
		if (prev->mapped && prev->data)
			__pspgl_buffer_unmap(prev->data, prev->access);

		psp_log("unbinding %p from target %x\n", prev, target);

		prev->mapped = GL_FALSE;
		__pspgl_bufferobj_free(prev);
	}

	psp_log("binding %p to target %x\n", bufp, target);

	*prevp = bufp;
	if (bufp)
		bufp->refcount++;
	return;

  out_error:
	GLERROR(GL_OUT_OF_MEMORY);
}

void glBindBuffer (GLenum, GLuint)
	__attribute__((alias("glBindBufferARB")));
