#include "pspgl_internal.h"


GLboolean glIsList (GLuint list)
{
	struct hashtable *hash = &pspgl_curctx->shared->texture_objects;
	return (__pspgl_hash_lookup(hash, list) == NULL) ? GL_FALSE : GL_TRUE;
}

