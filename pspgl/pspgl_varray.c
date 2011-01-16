#include <string.h>

#include "pspgl_internal.h"
#include "pspgl_buffers.h"

unsigned __pspgl_gl_sizeof(GLenum type)
{
	switch(type) {
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		return sizeof(GLubyte);

	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
		return sizeof(GLushort);

	case GL_UNSIGNED_INT:
		return sizeof(GLuint);

	case GL_FLOAT:
		return sizeof(GLfloat);
	}

	return 0;
}

static
int glfmt2gefmt (GLenum type)
{
	int ge_type;

	switch (type) {
	case GL_BYTE:
		ge_type = GE_INT_8BIT;
		break;
	case GL_SHORT:
		ge_type = GE_INT_16BIT;
		break;
	case GL_FLOAT:
		ge_type = GE_FLOAT_32BIT;
		break;
	default:
		ge_type = -1;
	}

	return ge_type;
}

static unsigned ge_sizeof(unsigned type)
{
	switch(type) {
	case GE_INT_8BIT:	return 1;
	case GE_INT_16BIT:	return 2;
	case GE_FLOAT_32BIT:	return 4;
	default:		return 0;
	}
}

static void cvt_color_float3_ub(void *to, const void *from, const struct attrib *attr)
{
	const GLfloat *src = from;
	unsigned long *dest = to;
	unsigned long ret = COLOR3(src) | 0xff000000;

	//psp_log("(%g,%g,%g) -> %08x\n", src[0], src[1], src[2], ret);
	*dest = ret;
}

static void cvt_color_float4_ub(void *to, const void *from, const struct attrib *attr)
{
	const GLfloat *src = from;
	unsigned long *dest = to;
	unsigned long ret = COLOR4(src);

	//psp_log("(%g,%g,%g,%g) -> %08x\n", src[0], src[1], src[2], src[3], ret);
	*dest = ret;
}

static void cvt_color_ub3_ub(void *to, const void *from, const struct attrib *attr)
{
	const unsigned char *src = from;
	unsigned long *dest = to;

	*dest = 0xff000000 | (src[2] << 16) | (src[1] << 8) | src[0];
}

static void cvt_float2_float3(void *to, const void *from, const struct attrib *attr)
{
	const float *src = from;
	float *dest = to;

	dest[0] = src[0];
	dest[1] = src[1];
	dest[2] = 0.f;
}

static void cvt_short2_short3(void *to, const void *from, const struct attrib *attr)
{
	const short *src = from;
	short *dest = to;

	dest[0] = src[0];
	dest[1] = src[1];
	dest[2] = 0;
}

static void cvt_byte2_byte3(void *to, const void *from, const struct attrib *attr)
{
	const unsigned char *src = from;
	unsigned char *dest = to;

	dest[0] = src[0];
	dest[1] = src[1];
	dest[2] = 0;
}

/* Returns true if the application arrays are configured in hardware format */
GLboolean __pspgl_vertex_is_native(const struct vertex_format *vfmt)
{
	int i;
	const struct pspgl_vertex_array *array, *next;
	const int nattr = vfmt->nattrib;
	const void *ptr, *cycle;

	if (nattr == 0) {
		assert(vfmt->vertex_size == 0);
		return GL_TRUE;
	}

	array = vfmt->attribs[0].array;
	next = vfmt->attribs[1].array;
	ptr = __pspgl_bufferobj_deref(array->buffer, (void *)array->ptr) + 
		vfmt->attribs[0].size;
	cycle = __pspgl_bufferobj_deref(array->buffer, (void *)array->ptr) + array->stride;

	/* Check each attribute array element is contigious with the
	   following one */
	i = 0;
	while(i < nattr-1) {
		const void *nextptr = __pspgl_bufferobj_deref(next->buffer, (void *)next->ptr);

		psp_log("attr %d array=%p next=%p, native=%d size=%d ptr(%p)+size=%p next(%p)->ptr=%p\n",
			i, array, next, array->native, vfmt->attribs[0].size,
			__pspgl_bufferobj_deref(array->buffer, (void *)array->ptr),
			ptr, nextptr);

		if (!array->native || ptr != nextptr)
			return GL_FALSE;

		i++;
		array = next;
		next = vfmt->attribs[i+1].array;
		ptr = __pspgl_bufferobj_deref(array->buffer, (void *)array->ptr) +
			vfmt->attribs[i].size;
	}

	psp_log("attr %d native=%d ptr+size=%p cycle=%p\n",
		i, array->native, ptr, cycle);

	/* Make sure the last attribute of the first vertex is
	   contigious with the first attribute of the next vertex.  */
	if (!array->native || ptr != cycle)
		return GL_FALSE;

	return GL_TRUE;
}

unsigned __pspgl_enabled_array_bits(void)
{
	struct varray *v = &pspgl_curctx->vertex_array;
	unsigned ret = 0;

	if (v->texcoord.enabled)
		ret |= VA_TEXCOORD_BIT;
	if (v->weight.enabled)
		ret |= VA_WEIGHT_BIT;
	if (v->color.enabled)
		ret |= VA_COLOR_BIT;
	if (v->normal.enabled)
		ret |= VA_NORMAL_BIT;
	if (v->vertex.enabled)
		ret |= VA_VERTEX_BIT;

	return ret;
}

/* 
   Examine the currently enabled vertex attribute arrays to compute a
   hardware vertex format, along with enough information to convert
   the arrays into that format.
 */
void __pspgl_ge_vertex_fmt(struct pspgl_context *ctx, struct vertex_format *vfmt)
{
	unsigned hwformat = 0;
	struct varray *varray = &ctx->vertex_array;
	struct attrib *attr = vfmt->attribs;
	unsigned offset;

	offset = 0;

	if (unlikely(!varray->vertex.enabled))
		goto out_disabled;

	/* These attributes are tested in the order the hardware
	   vertex format requires. */
	if (varray->texcoord.enabled) {
		unsigned hwtype = glfmt2gefmt(varray->texcoord.type);

		hwformat |= GE_TEXTURE_SHIFT(hwtype);

		assert(varray->texcoord.size >= 2 && varray->texcoord.size <= 4);

		attr->convert = NULL;
		attr->array = &varray->texcoord;
		offset = ROUNDUP(offset, ge_sizeof(hwtype));
		attr->offset = offset;
		attr->size = ge_sizeof(hwtype) * 2;
		offset += attr->size;

		psp_log("attr %d: array=%p offset=%d size=%d\n",
			attr-vfmt->attribs, attr->array, attr->offset, attr->size);
		attr++;
	}

	if (varray->weight.enabled) {
		unsigned hwtype = glfmt2gefmt(varray->weight.type);
		unsigned nweights = varray->weight.size;

		hwformat |= GE_WEIGHT_SHIFT(hwtype);
		hwformat |= GE_WEIGHTS(nweights);

		attr->convert = NULL;
		attr->array = &varray->weight;
		offset = ROUNDUP(offset, ge_sizeof(hwtype));
		attr->offset = offset;
		attr->size = ge_sizeof(hwtype) * nweights;
		offset += attr->size;

		psp_log("attr %d: array=%p offset=%d size=%d\n",
			attr-vfmt->attribs, attr->array, attr->offset, attr->size);
		attr++;
	}

	if (varray->color.enabled) {
		unsigned type = varray->color.type;
		unsigned size = varray->color.size;

		/* Always use RGBA8888 for now, but we could use other
		   color formats by extending glColorPointer to accept
		   more types (GL_UNSIGNED_SHORT_5_6_5, etc). */
		hwformat |= GE_COLOR_8888;

		assert(type == GL_FLOAT || type == GL_UNSIGNED_BYTE);
		assert(size == 3 || size == 4);

		attr->convert = NULL;
		if (type == GL_FLOAT) {
			if (size == 3)
				attr->convert = cvt_color_float3_ub;
			else
				attr->convert = cvt_color_float4_ub;
		} else {
			if (size == 3)
				attr->convert = cvt_color_ub3_ub;
		}

		attr->array = &varray->color;
		offset = ROUNDUP(offset, 4);
		attr->offset = offset;
		attr->size = 4;
		offset += 4;

		psp_log("attr %d: array=%p offset=%d size=%d\n",
			attr-vfmt->attribs, attr->array, attr->offset, attr->size);
		attr++;
	}

	if (varray->normal.enabled) {
		unsigned hwtype = glfmt2gefmt(varray->normal.type);

		hwformat |= GE_NORMAL_SHIFT(hwtype);

		attr->convert = NULL;
		attr->array = &varray->normal;
		offset = ROUNDUP(offset, ge_sizeof(hwtype));
		attr->offset = offset;
		attr->size = ge_sizeof(hwtype) * 3;
		offset += attr->size;

		psp_log("attr %d: array=%p offset=%d size=%d\n",
			attr-vfmt->attribs, attr->array, attr->offset, attr->size);
		attr++;
	}

	if (varray->vertex.enabled) {
		unsigned hwtype = glfmt2gefmt(varray->vertex.type);
		unsigned size = varray->vertex.size;

		hwformat |= GE_VERTEX_SHIFT(hwtype);

		assert(size >= 2 && size <= 4);

		/* Size must be either 2, 3 or 4. For size==2, we need
		   to fill in an extra z coord; for size==4, we just
		   ignore w  */
		attr->convert = NULL;
		if (size == 2) {
			switch(hwtype) {
			case GE_INT_8BIT:	attr->convert = cvt_byte2_byte3; break;
			case GE_INT_16BIT:	attr->convert = cvt_short2_short3; break;
			case GE_FLOAT_32BIT:	attr->convert = cvt_float2_float3; break;
			}
		}

		attr->array = &varray->vertex;
		offset = ROUNDUP(offset, ge_sizeof(hwtype));
		attr->offset = offset;
		attr->size = ge_sizeof(hwtype) * 3;
		offset += attr->size;

		psp_log("attr %d: array=%p offset=%d size=%d\n",
			attr-vfmt->attribs, attr->array, attr->offset, attr->size);
		attr++;
	}

	offset = ROUNDUP(offset, 4);

  out_disabled:
	vfmt->nattrib = attr - vfmt->attribs;
	vfmt->hwformat = hwformat;
	vfmt->vertex_size = offset;

	vfmt->arrays = __pspgl_enabled_array_bits();

	psp_log("format: %x %d attr, %d byte vertex\n",
		vfmt->hwformat, vfmt->nattrib, vfmt->vertex_size);
}

/* 
   Generate a vertex array in hardware format based on the current set
   of array pointers set up in the context.  It will copy as many
   vertices into memory at "to" as will fit into "space" (up to
   "count"), and return the number actually copied.
*/
int __pspgl_gen_varray(const struct vertex_format *vfmt, int first, int count, 
		       void *to, int space)
{
	unsigned char *dest = to;
	int nvtx = space / vfmt->vertex_size;
	void *ptrs[MAX_ATTRIB];

	if (nvtx > count)
		nvtx = count;

	/* Check to see if the user was kind enough to supply the
	   arrays in hardware format. */
	if (__pspgl_vertex_is_native(vfmt)) {
		struct pspgl_vertex_array *va;
		void *from;

		psp_log("arrays in native layout\n");

		va = vfmt->attribs[0].array;

		from = __pspgl_bufferobj_map(va->buffer, GL_READ_ONLY_ARB, (void *)va->ptr);
		from += va->stride * first;

		memcpy(to, from, nvtx * vfmt->vertex_size);

		__pspgl_bufferobj_unmap(va->buffer, GL_READ_ONLY_ARB);

		return nvtx;
	}

	psp_log("converting arrays\n");
	for(int i = 0; i < vfmt->nattrib; i++) {
		struct pspgl_vertex_array *a = vfmt->attribs[i].array;

		ptrs[i] = __pspgl_bufferobj_map(a->buffer, GL_READ_ONLY_ARB, (void *)a->ptr) +
			(first * a->stride);
		psp_log("  mapped ptr[%d]=%p\n", i, ptrs[i]);
	}

	for(int i = 0; i < nvtx; i++) {
		for(int j = 0; j < vfmt->nattrib; j++) {
			const struct attrib *attr = &vfmt->attribs[j];
			struct pspgl_vertex_array *a = attr->array;

			psp_log("vtx %d attr %d attr->offset=%d attr->convert=%p dest=%p\n",
				i, j, attr->offset, attr->convert, dest);

			if (attr->convert)
				(*attr->convert)(&dest[attr->offset], ptrs[j], attr);
			else
				memcpy(&dest[attr->offset], ptrs[j], attr->size);

			ptrs[j] += a->stride;
		}
		dest += vfmt->vertex_size;
	}

	for(int i = 0; i < vfmt->nattrib; i++) {
		struct pspgl_vertex_array *a = vfmt->attribs[i].array;

		__pspgl_bufferobj_unmap(a->buffer, GL_READ_ONLY_ARB);
	}

	psp_log("converted %d vertices\n", nvtx);

	return nvtx;
}

long __pspgl_glprim2geprim (GLenum glprim)
{
	static const char geprim_tab [] = {
		GE_POINTS,		/* GL_POINTS */
		GE_LINES,		/* GL_LINES */
		GE_LINE_STRIP,		/* GL_LINE_LOOP */
		GE_LINE_STRIP,		/* GL_LINE_STRIP */
		GE_TRIANGLES,		/* GL_TRIANGLES */
		GE_TRIANGLE_STRIP,	/* GL_TRIANGLE_STRIP */
		GE_TRIANGLE_FAN,	/* GL_TRIANGLE_FAN */
		GE_TRIANGLE_FAN,	/* GL_QUADS */
		GE_TRIANGLE_STRIP,	/* GL_QUAD_STRIP */
		GE_TRIANGLE_FAN,	/* GL_POLYGON */
		GE_SPRITES,		/* GL_SPRITES_PSP */
	};

	if ((unsigned) glprim > sizeof(geprim_tab)/sizeof(geprim_tab[0]))
		return -1;

	return geprim_tab[glprim];
}

/* Bind a new buffer object to an array's state, indicating the buffer
   object should be used as the source of array data.  A NULL buffer
   object means that the array's pointer should be treated
   normally. */
void __pspgl_varray_bind_buffer(struct pspgl_vertex_array *va,
				struct pspgl_bufferobj *buf)
{
	if (unlikely(buf && buf->mapped))
		goto out_error;

	/* If the array was previously coming out of an array we've
	   been using as a vertex cache, then uncache it.  We do this
	   even if the actual buffer hasn't changed, in case the
	   gl*Pointer call changed the type/size of the array (we
	   could check to see if this happened, but programs should be
	   careful not to make redundant state changes, and it isn't
	   very expensive to restore the cached_array anyway). */
	if (va->buffer &&
	    pspgl_curctx->vertex_array.locked.cached_array == va->buffer->data) {
		psp_log("uncaching array due to pointer update\n");
		__pspgl_uncache_arrays();
	}

	if (buf)
		buf->refcount++;

	if (va->buffer != NULL)
		__pspgl_bufferobj_free(va->buffer);

	va->buffer = buf;
	return;

  out_error:
	GLERROR(GL_INVALID_OPERATION);
}

/* Generate a new buffer containing a converted vertex array.  This is
   used when the caller provides a set of arrays, but isn't using CVA
   or VBOs.  It is also used if the VBO is not in hardware format. 
   The caller must free the refcount on the returned buffer. */
struct pspgl_buffer *__pspgl_varray_convert(const struct vertex_format *vfmt, 
					    int first, int count)
{
	unsigned size = vfmt->vertex_size * count;
	struct pspgl_buffer *buf;
	void *bufp;

	if (unlikely(size == 0))
		return NULL;

	buf = __pspgl_buffer_new(size, GL_STREAM_DRAW_ARB); /* used once */
	if (unlikely(buf == NULL))
		goto out_error;

	bufp = __pspgl_buffer_map(buf, GL_WRITE_ONLY_ARB);
	
	if (unlikely(__pspgl_gen_varray(vfmt, first, count, bufp, size) != count))
		goto out_unmap_buf;
	__pspgl_buffer_unmap(buf, GL_WRITE_ONLY_ARB);

	return buf;

  out_unmap_buf:
	__pspgl_buffer_unmap(buf, GL_WRITE_ONLY_ARB);
	__pspgl_buffer_free(buf);

  out_error:
	GLERROR(GL_OUT_OF_MEMORY);
	return NULL;
}

static int idx_sizeof(GLenum idx_type)
{
	int idxsize;

	switch(idx_type) {
	case GL_UNSIGNED_BYTE:	idxsize = 1; break;
	case GL_UNSIGNED_SHORT:
	case GL_UNSIGNED_INT:	idxsize = 2; break;
	default:		idxsize = 0;
	}

	return idxsize;
}

/* Copy and convert an index array into hardware format; 32-bit
   indices are truncated to 16 bits.  Offset is subtracted from the
   indices if there's a non-0 minidx.  Expects from and to to be
   appropriately mapped pointers.  Returns the appropriate hwformat
   for the index type. */
static unsigned convert_indices(void *to, const void *from, GLenum idx_type, 
				int offset, GLsizei count)
{
	unsigned hwformat = 0;
	unsigned i;

	switch(idx_type) {
	case GL_UNSIGNED_BYTE:
		hwformat = GE_VINDEX_8BIT;
		for(i = 0; i < count; i++) 
			((GLubyte *)to)[i] = ((GLubyte *)from)[i] - offset;
		break;

	case GL_UNSIGNED_SHORT:
		hwformat = GE_VINDEX_16BIT;
		for(i = 0; i < count; i++)
			((GLushort *)to)[i] = ((GLushort *)from)[i] - offset;
		break;

	case GL_UNSIGNED_INT:
		hwformat = GE_VINDEX_16BIT;
		for(i = 0; i < count; i++)
			((GLushort *)to)[i] = ((GLuint *)from)[i] - offset;
		break;

	}

	return hwformat;
}

/* Convert an array of indices into a hardware format. If the array is
   already in a buffer object in a hardware format, we simply return a
   reference to that buffer. *hwformat is also updated to include the
   appropriate index type. The returned buffer will always have a
   refcount which needs to be freed by the caller. */
struct pspgl_buffer *__pspgl_varray_convert_indices(GLenum idxtype, const void *indices,
						    int minidx, int count,
						    unsigned *buffer_offset,
						    unsigned *hwformat)
{
	const struct pspgl_bufferobj *idxbuf = pspgl_curctx->vertex_array.indexbuffer;
	struct pspgl_buffer *ret;
	void *retp;
	const void *idxp;

	ret = NULL;
	*buffer_offset = 0;

	if (idxbuf) {
		if (unlikely(idxbuf->mapped))
			goto out_error;

		if (minidx == 0 &&
		    (idxtype == GL_UNSIGNED_BYTE || idxtype == GL_UNSIGNED_SHORT)) {
			/* Index buffer object in hardware format */
			assert(idxbuf->data->mapped == 0);

			if (idxtype == GL_UNSIGNED_BYTE)
				*hwformat |= GE_VINDEX_8BIT;
			else
				*hwformat |= GE_VINDEX_16BIT;

			*buffer_offset = indices - NULL;
			ret = idxbuf->data;
			ret->refcount++;
		}
	}

	if (ret == NULL) {
		/* Index buffer object, but not in hardware format,
		   or no index buffer object */
		ret = __pspgl_buffer_new(idx_sizeof(idxtype) * count, 
					 GL_STREAM_DRAW_ARB);
		if (unlikely(ret == NULL))
			return NULL;

		/* map buffers appropriately for conversion */
		retp = __pspgl_buffer_map(ret, GL_WRITE_ONLY_ARB);
		idxp = __pspgl_bufferobj_map(idxbuf, GL_READ_ONLY_ARB, (void *)indices);

		*hwformat |= convert_indices(retp, idxp, idxtype, minidx, count);

		__pspgl_buffer_unmap(ret, GL_WRITE_ONLY_ARB);
		__pspgl_bufferobj_unmap(idxbuf, GL_READ_ONLY_ARB);
	}

	return ret;

  out_error:
	GLERROR(GL_INVALID_OPERATION);
	return NULL;
}
