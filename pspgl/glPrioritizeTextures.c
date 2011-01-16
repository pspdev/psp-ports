#include "pspgl_internal.h"
#include "pspgl_texobj.h"

void glPrioritizeTextures (GLsizei n, const GLuint *textures, const GLclampf *priorities)
{
	struct hashtable *hash = &pspgl_curctx->shared->texture_objects;
	struct pspgl_texobj *texobj;
	GLsizei i;

	if (unlikely(n < 0)) {
		GLERROR(GL_INVALID_VALUE);
		return;
	}

	for (i=0; i<n; i++) {
		if ((texobj = __pspgl_hash_lookup(hash, textures[i])))
			texobj->priority = priorities[i];
	}
}

