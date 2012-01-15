#include "pspgl_internal.h"

// @@@ modified by Edorul
void glTexCoord4f( GLfloat s, GLfloat t, GLfloat r, GLfloat q )
{
 // @@@ from here it's allways the same code exept for params of the command
	if (pspgl_curctx->displaylists.is_in_glNewList & GL_COMPILE) { // we must record this command
		struct stDisplayElement *new;
		new = (struct stDisplayElement *) malloc (sizeof(struct stDisplayElement));
		// @@@ put new element at the end of list
		if (!__pspgl_actuallist->first)
			__pspgl_actuallist->first = new;
		if (__pspgl_actuallist->last)
			__pspgl_actuallist->last->next = new;
		new->next = NULL;
		__pspgl_actuallist->last = new;
		
		// params of the command
		new->command_num = GLTEXCOORD; // to modify
		new->parami1 = 0; // to modify
		new->parami2 = 0; // to modify
		new->paramf1 = s; // to modify
		new->paramf2 = t; // to modify
		new->paramf3 = r; // to modify
		new->paramf4 = q; // to modify
	}
	
	if ((pspgl_curctx->displaylists.is_in_glNewList == GL_COMPILE_AND_EXECUTE)||(pspgl_curctx->displaylists.is_in_glNewList == 0)) {
 // @@@ from here it's different, because it's the old function
		pspgl_curctx->current.texcoord[0] = s;
		pspgl_curctx->current.texcoord[1] = t;
		pspgl_curctx->current.texcoord[2] = r;
		pspgl_curctx->current.texcoord[3] = q;
	}
}

void glTexCoord1f (GLfloat s)
{
	glTexCoord4f(s, 0.0f, 0.0f, 1.0f);
}

void glTexCoord1fv(const GLfloat *v)
{
	glTexCoord4f(v[0], 0.0f, 0.0f, 1.0f);
}

void glTexCoord2f (GLfloat s, GLfloat t)
{
	glTexCoord4f(s, t, 0.0f, 1.0f);
}

void glTexCoord2fv(const GLfloat *v)
{
	glTexCoord4f(v[0], v[1], 0.0f, 1.0f);
}

void glTexCoord3f (GLfloat s, GLfloat t, GLfloat r)
{
	glTexCoord4f(s, t, r, 1.0f);
}

void glTexCoord3fv(const GLfloat *v)
{
	glTexCoord4f(v[0], v[1], v[2], 1.0f);
}

void glTexCoord4fv(const GLfloat *v)
{
	glTexCoord4f(v[0], v[1], v[2], v[3]);
}

void glTexCoord1d (GLdouble s)
{
	glTexCoord4f((GLfloat)s, 0.0f, 0.0f, 1.0f);
}

void glTexCoord1dv(const GLdouble *v)
{
	glTexCoord4f((GLfloat)v[0], 0.0f, 0.0f, 1.0f);
}

void glTexCoord2d (GLdouble s, GLdouble t)
{
	glTexCoord4f((GLfloat)s, (GLfloat)t, 0.0f, 1.0f);
}

void glTexCoord2dv(const GLdouble *v)
{
	glTexCoord4f((GLfloat)v[0], (GLfloat)v[1], 0.0f, 1.0f);
}

void glTexCoord3d (GLdouble s, GLdouble t, GLdouble r)
{
	glTexCoord4f((GLfloat)s, (GLfloat)t, (GLfloat)r, 1.0f);
}

void glTexCoord3dv(const GLdouble *v)
{
	glTexCoord4f((GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2], 1.0f);
}

void glTexCoord4d (GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
	glTexCoord4f((GLfloat)s, (GLfloat)t, (GLfloat)r, (GLfloat)q);
}

void glTexCoord4dv(const GLdouble *v)
{
	glTexCoord4f((GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2], (GLfloat)v[3]);
}

// @@@ end of modifs
