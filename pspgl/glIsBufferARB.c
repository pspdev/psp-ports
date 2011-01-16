#include "pspgl_internal.h"

GLboolean glIsBufferARB(GLuint buffer)
{
	struct hashtable *hash = &pspgl_curctx->shared->buffers;
	return __pspgl_hash_lookup(hash, buffer) != NULL;
}

GLboolean glIsBuffer(GLuint buffer)
	__attribute__((alias("glIsBufferARB")));

