#include "pspgl_internal.h"
#include "pspgl_buffers.h"

void glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	struct pspgl_vertex_array *va = &pspgl_curctx->vertex_array.vertex;
	GLboolean native;
	GLenum error;

	error = GL_INVALID_VALUE;
	if (unlikely(size < 2 || size > 4))
		goto out_error;

	if (unlikely(stride < 0))
		goto out_error;

	error = GL_INVALID_ENUM;
	if (unlikely(type != GL_BYTE && type != GL_SHORT && type != GL_FLOAT))
		goto out_error;

	if (stride == 0)
		stride = __pspgl_gl_sizeof(type) * size;

	native = (size == 3);

	psp_log("ptr=%p(%p) size=%d type=%x stride=%d native=%d\n",
		pointer, __pspgl_bufferobj_deref(pspgl_curctx->vertex_array.arraybuffer, (void *)pointer),
		size, type, stride, native);

	/* If we're changed vertex formats, then mark the modelview matrix
	   as dirty, so that any adjustment for this format can be
	   applied. */
	if (va->type != type)
		pspgl_curctx->modelview_stack.flags |= MF_DIRTY;

	va->size = size;
	va->type = type;
	va->stride = stride;
	va->ptr = pointer;
	va->native = native;

	__pspgl_varray_bind_buffer(va, pspgl_curctx->vertex_array.arraybuffer);

	return;

  out_error:
	GLERROR(error);
}
