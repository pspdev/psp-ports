#include "pspgl_internal.h"


void glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
	__pspgl_varray_draw_elts(mode, type, indices, count);
}
