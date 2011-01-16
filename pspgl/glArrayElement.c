#include "pspgl_internal.h"
#include "pspgl_buffers.h"

static const
GLfloat scale_factor [] = { 
	1.0f/127.0f,
	1.0f/255.0f, 
	1.0f/32767.0f,
	1.0f/65535.0f,
	1.0f/2147483647.0f,
	1.0f/4294967295.0f
};


static
void dereference (struct pspgl_vertex_array *a, GLint idx, 
		  void (*glfunc) (const GLfloat *v), int scale)
{
	const void *ptr;
	GLfloat val [4];
	GLfloat factor;
	int i;

	if (!a->enabled)
		return;


	ptr = __pspgl_bufferobj_map(a->buffer, GL_READ_ONLY_ARB, (void *)a->ptr);
	ptr += a->stride * idx;

	if (scale && a->type >= GL_BYTE && a->type <= GL_UNSIGNED_INT)
		factor = scale_factor[a->type - GL_BYTE];
	else
		factor = 1.0f;

	/* be prepared for incomplete initialisations... */
	val[2] = 0.0f;
	val[3] = 1.0f;

	switch (a->type) {
	case GL_BYTE:
		for (i=0; i<a->size; i++)
			val[i] = ((GLbyte *) ptr)[i] * factor;
		break;

	case GL_UNSIGNED_BYTE:
		for (i=0; i<a->size; i++)
			val[i] = ((GLubyte *) ptr)[i] * factor;
		break;

	case GL_SHORT:
		for (i=0; i<a->size; i++)
			val[i] = ((GLshort *) ptr)[i] * factor;
		break;

	case GL_UNSIGNED_SHORT:
		for (i=0; i<a->size; i++)
			val[i] = ((GLushort *) ptr)[i] * factor;
		break;

	case GL_INT:
		for (i=0; i<a->size; i++)
			val[i] = ((GLint *) ptr)[i] * factor;
		break;

	case GL_UNSIGNED_INT:
		for (i=0; i<a->size; i++)
			val[i] = ((GLuint *) ptr)[i] * factor;
		break;

	case GL_FLOAT:
		for (i=0; i<a->size; i++)
			val[i] = ((GLfloat *) ptr)[i];
		break;

	case GL_DOUBLE:
		for (i=0; i<a->size; i++)
			val[i] = ((GLdouble *) ptr)[i];
		break;

	default:
		GLERROR(GL_INVALID_ENUM);
	}

	(*glfunc)(val);

	__pspgl_bufferobj_unmap(a->buffer, GL_READ_ONLY_ARB);
}


void glArrayElement (GLint i)
{
	dereference(&pspgl_curctx->vertex_array.texcoord, i, glTexCoord4fv, 1);
	dereference(&pspgl_curctx->vertex_array.color, i, glColor4fv, 1);
	dereference(&pspgl_curctx->vertex_array.normal, i, glNormal3fv, 1);
	dereference(&pspgl_curctx->vertex_array.vertex, i, glVertex4fv, 0);
}

