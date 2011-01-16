#include "pspgl_internal.h"
#include "pspgl_buffers.h"

void glDeleteBuffersARB (GLsizei n, const GLuint *buffers)
{
	int i;
	struct hashtable *hash = &pspgl_curctx->shared->buffers;

	if (unlikely(n < 0))
		goto out_error;

	for(i = 0; i < n; i++) {
		GLuint id  = buffers[i];
		struct pspgl_bufferobj *bufp;

		if (id == 0)
			continue;

		bufp = __pspgl_hash_remove(hash, id);

		if (bufp) {
			int t;
			struct pspgl_bufferobj **boundp;
			static const GLenum targets[] = {
				GL_ARRAY_BUFFER_ARB, GL_ELEMENT_ARRAY_BUFFER_ARB
			};

			for(t = 0; t < sizeof(targets)/sizeof(*targets); t++) {
				boundp = __pspgl_bufferobj_for_target(targets[t]);

				if (boundp != NULL && *boundp == bufp)
					glBindBufferARB(targets[t], 0);
			}

			__pspgl_bufferobj_free(bufp);
		}
	}
	return;

  out_error:
	GLERROR(GL_INVALID_VALUE);
}

void glDeleteBuffers (GLsizei n, const GLuint *buffers)
	__attribute__((alias("glDeleteBuffersARB")));
