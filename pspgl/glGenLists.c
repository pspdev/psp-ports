#include "pspgl_internal.h"


GLuint glGenLists(GLsizei range)
{
	struct hashtable *hash = &pspgl_curctx->shared->display_lists;
	GLuint id, start, i;

	if (range < 0)
		GLERROR(GL_INVALID_VALUE);

	if (range <= 0)
		return 0;

	start = id = __pspgl_hash_uniquekey(hash);

again:
	for(i = 0; i < range; i++) {
		if (id < start && id+range >= start) {
			/* no space in the ID range */
			id = 0;
			break;
		}	

		if (__pspgl_hash_lookup(hash, id+i) != NULL) {
			id = id+i+1;
			goto again;
		}
	}

	for(i = 0; i < range; i++)
		__pspgl_hash_insert(hash, id+i, "foo");

	return id;	
}

