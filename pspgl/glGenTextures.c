#include "pspgl_internal.h"
#include "pspgl_texobj.h"

void glGenTextures (GLsizei n, GLuint *textures)
{
	struct hashtable *hash = &pspgl_curctx->shared->texture_objects;
	GLsizei i;
	GLenum error;

	error = GL_INVALID_VALUE;
	if (unlikely(n < 0))
		goto out_error;

	for(i = 0; i < n; i++) {
		unsigned id;
		struct pspgl_texobj *tobj;

		id = __pspgl_hash_uniquekey(hash);

		tobj = __pspgl_texobj_new(id, 0);
		error = GL_OUT_OF_MEMORY;
		if (tobj == NULL)
			goto out_error;

		__pspgl_hash_insert(hash, id, tobj);

		textures[i] = id;
	}
	return;

  out_error:
	GLERROR(GL_INVALID_VALUE);
}

