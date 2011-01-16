#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "pspgl_internal.h"
#include "pspgl_buffers.h"

void glBufferDataARB(GLenum target, GLsizeiptr size,
		     const GLvoid *init_data, GLenum usage)
{
	struct pspgl_bufferobj *buf, **bufp;
	struct pspgl_buffer *databuf;
	GLenum error;

	bufp = __pspgl_bufferobj_for_target(target);

	if (bufp == NULL)
		return;

	buf = *bufp;

	error = GL_INVALID_OPERATION;
	if (unlikely(buf == NULL))
		goto out_error;

	error = GL_INVALID_VALUE;
	if (unlikely(size < 0))
		goto out_error;

	databuf = __pspgl_buffer_new(size, usage);

	error = GL_OUT_OF_MEMORY;
	if (unlikely(databuf == NULL))
		goto out_error;

	if (buf->data) {
		if (buf->mapped)
			__pspgl_buffer_unmap(buf->data, buf->access);

		psp_log("freeing data %p for buffer %p\n", buf->data, buf);
		__pspgl_buffer_free(buf->data);
	}

	buf->mapped = GL_FALSE;
	buf->usage = usage;
	buf->data = databuf;

	psp_log("attaching data %p(%p) to buffer %p\n",
		databuf, databuf->base, buf);

	if (init_data != NULL) {
		void *p;

		p = __pspgl_buffer_map(databuf, GL_WRITE_ONLY_ARB);

		memcpy(p, init_data, size);

		__pspgl_buffer_unmap(databuf, GL_WRITE_ONLY_ARB);
	}
	return;

  out_error:
	GLERROR(error);
}

void glBufferData(GLenum target, GLsizeiptr size,
		  const GLvoid *init_data, GLenum usage)
	__attribute__((alias("glBufferDataARB")));
