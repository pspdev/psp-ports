#include "pspgl_internal.h"


static const
struct desc {
	unsigned int size_texcoord : 3;
	unsigned int size_color : 3;
	unsigned int size_normal : 1;
	unsigned int size_vertex : 3;

	unsigned int color_is_float : 1;

	unsigned int offset_color : 3;
	unsigned int offset_normal: 4;
	unsigned int offset_vertex : 4;
} array_desc [] = {
#define VFMT(tcsz, csz, norm, vsz, fpcol)		\
	{ tcsz, csz, norm, vsz, fpcol,			\
	tcsz,						\
	tcsz + (csz / (fpcol ? 1 : 4)),			\
	tcsz + (csz / (fpcol ? 1 : 4)) + 3*norm }

	/*   T  C  N  V  fpcol		 */
	VFMT(0, 0, 0, 2, 0),	/* GL_V2F		*/
	VFMT(0, 0, 0, 3, 0),	/* GL_V3F		*/
	VFMT(0, 4, 0, 2, 0),	/* GL_C4UB_V2F		*/
	VFMT(0, 4, 0, 3, 0),	/* GL_C4UB_V3F		*/

	VFMT(0, 3, 0, 3, 1),	/* GL_C3F_V3F		*/
	VFMT(0, 0, 1, 3, 0),	/* GL_N3F_V3F		*/
	VFMT(0, 4, 1, 3, 1),	/* GL_C4F_N3F_V3F	*/
	VFMT(2, 0, 0, 3, 0),	/* GL_T2F_V3F		*/

	VFMT(4, 0, 0, 4, 0),	/* GL_T4F_V4F		*/
	VFMT(2, 4, 0, 3, 0),	/* GL_T2F_C4UB_V3F	*/
	VFMT(2, 3, 0, 3, 1),	/* GL_T2F_C3F_V3F	*/
	VFMT(2, 0, 1, 3, 0),	/* GL_T2F_N3F_V3F	*/

	VFMT(2, 4, 1, 3, 1),	/* GL_T2F_C4F_N3F_V3F	*/
	VFMT(4, 4, 1, 4, 1),	/* GL_T4F_C4F_N3F_V4F	*/
#undef VFMT
};


void glInterleavedArrays (GLenum format, GLsizei stride, const GLvoid *p)
{
	struct varray *va = &pspgl_curctx->vertex_array;
	unsigned long idx = format - GL_V2F;
	const struct desc *desc;

	if (idx > sizeof(array_desc)/sizeof(array_desc[0]))
		goto out_error;

	desc = &array_desc[idx];

	va->vertex.enabled = GL_TRUE;
	va->texcoord.enabled = desc->size_texcoord != 0;
	va->color.enabled = desc->size_color != 0;
	va->normal.enabled = desc->size_normal != 0;

	va->vertex.size = desc->size_vertex;
	va->texcoord.size = desc->size_texcoord;
	va->color.size = desc->size_color;
	va->normal.size = 3;

	va->vertex.type = GL_FLOAT;
	va->texcoord.type = GL_FLOAT;
	va->color.type = desc->color_is_float ? GL_FLOAT : GL_UNSIGNED_BYTE;
	va->normal.type = GL_FLOAT;

	if (stride == 0)
		stride = 4 * (desc->offset_vertex + desc->size_vertex);

	va->vertex.stride = stride;
	va->texcoord.stride = stride;
	va->color.stride = stride;
	va->normal.stride = stride;

	va->vertex.native = (va->vertex.size == 3);
	va->texcoord.native = (va->texcoord.size == 2);
	va->color.native = (va->color.type == GL_UNSIGNED_BYTE && va->color.size == 4);
	va->normal.native = GL_TRUE;

	va->vertex.ptr = (p + 4 * desc->offset_vertex);
	va->normal.ptr = (p + 4 * desc->offset_normal);
	va->color.ptr = (p + 4 * desc->offset_color);
	va->texcoord.ptr = p;

	__pspgl_varray_bind_buffer(&va->vertex, va->arraybuffer);
	__pspgl_varray_bind_buffer(&va->normal, va->arraybuffer);
	__pspgl_varray_bind_buffer(&va->color, va->arraybuffer);
	__pspgl_varray_bind_buffer(&va->texcoord, va->arraybuffer);

	if (va->locked.cached_array &&
	    va->locked.vfmt.arrays != __pspgl_enabled_array_bits()) {
		psp_log("array state changed %x->%x; invalidating cached arrays\n",
			va->locked.vfmt.arrays, __pspgl_enabled_array_bits());
		__pspgl_uncache_arrays();
	}
	return;

  out_error:
	GLERROR(GL_INVALID_ENUM);
}
