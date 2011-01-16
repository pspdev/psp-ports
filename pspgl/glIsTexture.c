#include "pspgl_internal.h"


GLboolean glIsTexture (GLuint texture)
{
	struct hashtable *hash = &pspgl_curctx->shared->texture_objects;
	return (texture == 0 || __pspgl_hash_lookup(hash, texture) == NULL) ? GL_FALSE : GL_TRUE;
}

