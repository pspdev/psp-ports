#include <stdlib.h>
#include "pspgl_internal.h"


void glDeleteLists (GLuint list, GLsizei range)
{
	struct hashtable *hash = &pspgl_curctx->shared->display_lists;

	if (unlikely(range < 0))
		goto out_error;

	for (; range>0; list++, range--) {
		void *dlist = __pspgl_hash_remove(hash, list);
		if (dlist) {
			if (dlist == pspgl_curctx->dlist_current)
				/*set default dlist*/;
			/* should become a dlist_free() */
			free(dlist);
		}
	}

	return;
  out_error:
	GLERROR(GL_INVALID_VALUE);
}

