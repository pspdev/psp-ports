#include "pspgl_internal.h"
#include "pspgl_buffers.h"

void glWeightPointerPSP(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	struct pspgl_vertex_array *va = &pspgl_curctx->vertex_array.weight;
	GLenum error;

	error = GL_INVALID_VALUE;
	if (unlikely(size < 1 || size > NBONES))
		goto out_error;

	if (unlikely(stride < 0))
		goto out_error;

	error = GL_INVALID_ENUM;
	if (unlikely(type != GL_BYTE && type != GL_SHORT && type != GL_FLOAT))
		goto out_error;

	if (stride == 0)
		stride = __pspgl_gl_sizeof(type) * size;

	psp_log("ptr=%p(%p) size=%d type=%x stride=%d\n",
		pointer, __pspgl_bufferobj_deref(pspgl_curctx->vertex_array.arraybuffer,
						 (void *)pointer),
		size, type, stride);

	va->size = size;
	va->type = type;
	va->stride = stride;
	va->ptr = pointer;
	va->native = GL_TRUE;

	__pspgl_varray_bind_buffer(va, pspgl_curctx->vertex_array.arraybuffer);
	return;

  out_error:
	GLERROR(error);
}
