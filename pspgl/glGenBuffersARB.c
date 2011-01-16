#include "pspgl_internal.h"
#include "pspgl_buffers.h"

void glGenBuffersARB (GLsizei n, GLuint *buffers)
{
	struct hashtable *hash = &pspgl_curctx->shared->buffers;
	int i;

	if (unlikely(n < 0))
		goto out_error;

	if (unlikely(!buffers))
		return;

	for(i = 0; i < n; i++) {
		unsigned id;
		struct pspgl_bufferobj *bufp;

		id = __pspgl_hash_uniquekey(hash);

		bufp = __pspgl_bufferobj_new(NULL);
		__pspgl_hash_insert(hash, id, bufp);

		buffers[i] = id;
	}
	return;

  out_error:
	GLERROR(GL_INVALID_VALUE);
}

void glGenBuffers (GLsizei count, GLuint *buffers)
	__attribute__((alias("glGenBuffersARB")));
