#include "pspgl_internal.h"

static void update_color(unsigned long c)
{
	pspgl_curctx->current.color = c;

	/* only apply when lighting is disabled */
	if ((getReg(CMD_ENA_LIGHTING) & 0xff) == 0) {
		sendCommandi(CMD_MATERIAL_AMB_C, c);
		sendCommandi(CMD_MATERIAL_AMB_A, c>>24);
	}
}

void glColor4fv (const GLfloat *color)
{
	update_color(COLOR4(color));
}


void glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	update_color((((int) (255.0 * CLAMPF(alpha))) << 24) |
		     (((int) (255.0 * CLAMPF(blue))) << 16) |
		     (((int) (255.0 * CLAMPF(green))) << 8) |
		     ((int) (255.0 * CLAMPF(red))));
}


void glColor3f (GLfloat red, GLfloat green, GLfloat blue)
{
	glColor4f(red, green, blue, 1.0);
}


void glColor4ubv (const GLubyte *c)
{
	update_color((c[3] << 24) | (c[2] << 16) | (c[1] << 8) | c[0]);
}


void glColor3ub (GLubyte r, GLubyte g, GLubyte b)
{
	update_color(0xff000000 | (b << 16) | (g << 8) | r);
}

 
void glColor4ub (GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	update_color((a << 24) | (b << 16) | (g << 8) | r);
}

