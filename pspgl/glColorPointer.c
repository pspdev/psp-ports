#include "pspgl_internal.h"


void glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
	struct pspgl_vertex_array *va = &pspgl_curctx->vertex_array.color;
	GLboolean native;
	GLenum error;

	error = GL_INVALID_VALUE;
	if (unlikely(size != 3 && size != 4))
		goto out_error;

	if (unlikely(stride < 0))
		goto out_error;

	error = GL_INVALID_ENUM;
	if (unlikely(type != GL_FLOAT && type != GL_UNSIGNED_BYTE))
		goto out_error;

	if (stride == 0)
		stride = __pspgl_gl_sizeof(type) * size;

	native = (type == GL_UNSIGNED_BYTE) && (size == 4);

	psp_log("ptr=%p size=%d type=%x stride=%d native=%d\n",
		pointer, size, type, stride, native);

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
