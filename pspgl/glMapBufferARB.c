#include "pspgl_internal.h"
#include "pspgl_buffers.h"

GLvoid *glMapBufferARB(GLenum target, GLenum access)
{
	struct pspgl_bufferobj *buf, **bufp;
	void *p;

	bufp = __pspgl_bufferobj_for_target(target);

	if (bufp == NULL)
		return NULL;

	buf = *bufp;

	if (unlikely(buf == NULL) || unlikely(buf->data == NULL) || unlikely(buf->mapped))
		goto out_error;

	/* If we're writing the buffer or the hardware is writing to
	   the buffer, wait for the hardware to finish with it.  (The
	   CPU and graphics hardware are allowed to read
	   concurrently.) */
	if (access != GL_READ_ONLY_ARB ||
	    (buf->data->flags & BF_PINNED_WR))
		__pspgl_buffer_dlist_sync(buf->data);

	buf->access = access;
	buf->mapped = GL_TRUE;

	p = __pspgl_buffer_map(buf->data, access);

	psp_log("mapped buf %p data %p -> %p\n",
		buf, buf->data->base, p);

	return p;

out_error:
	GLERROR(GL_INVALID_OPERATION);
	return NULL;
}

GLvoid *glMapBuffer(GLenum target, GLenum access)
	__attribute__((alias("glMapBufferARB")));
