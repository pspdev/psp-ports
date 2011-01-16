#include <string.h>

#include "pspgl_internal.h"
#include "pspgl_buffers.h"

void glGetBufferSubDataARB(GLenum target, GLintptrARB offset,
			   GLsizeiptrARB size, GLvoid *data)
{
	struct pspgl_bufferobj *buf, **bufp;
	void *p;
	GLenum error;

	bufp = __pspgl_bufferobj_for_target(target);

	if (bufp == NULL)
		return;

	buf = *bufp;

	error = GL_INVALID_OPERATION;
	if (buf == NULL || buf->data == NULL || buf->mapped)
		goto out_error;

	error = GL_INVALID_VALUE;
	if (size < 0 || offset+size > buf->data->size)
		goto out_error;

	p = __pspgl_buffer_map(buf->data, GL_READ_ONLY);

	memcpy(data, p+offset, size);

	__pspgl_buffer_unmap(buf->data, GL_READ_ONLY);
	return;

  out_error:
	GLERROR(error);
}

void glGetBufferSubData(GLenum target, GLintptrARB offset,
			GLsizeiptrARB size, GLvoid *data)
	__attribute__((alias("glGetBufferSubDataARB")));
