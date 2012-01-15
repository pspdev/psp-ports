#include "pspgl_internal.h"

void __pspgl_update_color(unsigned long c)
{
 // @@@ from here it's allways the same code exept for index of the command
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
		
		new->command_num = GLCOLOR;
		new->parami1 = c;
	}
	
	if ((pspgl_curctx->displaylists.is_in_glNewList == GL_COMPILE_AND_EXECUTE)||(pspgl_curctx->displaylists.is_in_glNewList == 0)) {
 // @@@ from here it's different, because it's the old function
	pspgl_curctx->current.color = c;

	/* only apply when lighting is disabled */
	if ((getReg(CMD_ENA_LIGHTING) & 0xff) == 0) {
		sendCommandi(CMD_MATERIAL_AMB_C, c);
		sendCommandi(CMD_MATERIAL_AMB_A, c>>24);
	}
	}
}

void glColor4fv (const GLfloat *color)
{
	__pspgl_update_color(COLOR4(color));
}


void glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	__pspgl_update_color((((int) (255.0 * CLAMPF(alpha))) << 24) |
		     (((int) (255.0 * CLAMPF(blue))) << 16) |
		     (((int) (255.0 * CLAMPF(green))) << 8) |
		     ((int) (255.0 * CLAMPF(red))));
}

// @@@ added by Edorul
void glColor3fv(const GLfloat *color)
{
	__pspgl_update_color((((int) (255.0 * CLAMPF(1.0f))) << 24) |
		(((int) (255.0 * CLAMPF(color[2]))) << 16) |
		(((int) (255.0 * CLAMPF(color[1]))) << 8) |
		 ((int) (255.0 * CLAMPF(color[0]))));
}
// @@@ end of Edorul's addition

void glColor3f (GLfloat red, GLfloat green, GLfloat blue)
{
	glColor4f(red, green, blue, 1.0);
}


void glColor4ubv (const GLubyte *c)
{
	__pspgl_update_color((c[3] << 24) | (c[2] << 16) | (c[1] << 8) | c[0]);
}


void glColor3ub (GLubyte r, GLubyte g, GLubyte b)
{
	__pspgl_update_color(0xff000000 | (b << 16) | (g << 8) | r);
}

 
void glColor4ub (GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	__pspgl_update_color((a << 24) | (b << 16) | (g << 8) | r);
}

