#include <string.h>

#include "pspgl_internal.h"
#include "pspgl_buffers.h"

void glBufferSubDataARB(GLenum target, GLintptrARB offset, GLsizeiptrARB size,
			const GLvoid *data)
{
	struct pspgl_bufferobj *buf, **bufp;
	void *p;
	GLenum error;

	bufp = __pspgl_bufferobj_for_target(target);

	if (bufp == NULL)
		return;

	buf = *bufp;

	error = GL_INVALID_OPERATION;
	if (unlikely(buf == NULL) || unlikely(buf->mapped)) 
		goto out_error;

	error = GL_INVALID_VALUE;
	if (size < 0 || 
	    buf->data == NULL || 
	    size+offset > buf->data->size)
		goto out_error;

	__pspgl_buffer_dlist_sync(buf->data);

	p = __pspgl_buffer_map(buf->data, GL_WRITE_ONLY_ARB);

	memcpy(p + offset, data, size);

	__pspgl_buffer_unmap(buf->data, GL_WRITE_ONLY_ARB);
	return;

  out_error:
	GLERROR(error);
}

void glBufferSubData(GLenum target, GLintptrARB offset, GLsizeiptrARB size,
		     const GLvoid *data)
	__attribute__((alias("glBufferSubDataARB")));

