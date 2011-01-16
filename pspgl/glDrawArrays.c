#include "pspgl_internal.h"


void glDrawArrays (GLenum mode, GLint first, GLsizei count)
{
	__pspgl_varray_draw(mode, first, count);
}
