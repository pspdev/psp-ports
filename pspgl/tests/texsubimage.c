#include <stdlib.h>
#include <GL/glut.h>

#include "glchk.h"

static
void reshape (int w, int h)
{
	GLCHK(glViewport(0, 0, w, h));
	GLCHK(glMatrixMode(GL_PROJECTION));
	GLCHK(glLoadIdentity());
	GLCHK(glOrtho(-2*1.7, 2*1.7, 2, -2, -2, 2));
	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glLoadIdentity());
}


static int freezecol = 0;

extern unsigned char one_start[];
extern unsigned char two_start[];
extern unsigned char three_start[];
extern unsigned char four_start[];

static unsigned char *img[] = {
	one_start,
	two_start,
	three_start,
	four_start,
};

static
void display (void)
{
	int i;

	GLCHK(glShadeModel(GL_SMOOTH));

	GLCHK(glClear(GL_COLOR_BUFFER_BIT));

	GLCHK(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE));
	GLCHK(glEnable(GL_BLEND));
	GLCHK(glEnable(GL_TEXTURE_2D));
	GLCHK(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));

	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glLoadIdentity());
	GLCHK(glTranslatef(-2, 0, 0));		
		
	if (!freezecol)
		GLCHK(glColor3ub(rand(), rand(), rand()));

	glFlush();
	for(i = 0; i < 4; i++) {
		GLCHK(glTexSubImage2D(GL_TEXTURE_2D, 0, 13*i, 13*i, 64, 64, GL_RGBA, GL_UNSIGNED_BYTE, img[i]));

		glBegin(GL_TRIANGLE_FAN);
		  glTexCoord2f(0, 0);
		  glVertex3f(0, 0, 0);

		  glTexCoord2f(0, 1);
		  glVertex3f(0, 1, 0);

		  glTexCoord2f(1, 1);
		  glVertex3f(1, 1, 0);

		  glTexCoord2f(1, 0);
		  glVertex3f(1, 0, 0);
		glEnd();

		GLCHK(glTranslatef(1, 0, 0));		
	}

#if SYS
	usleep(100000);
#endif
	glutSwapBuffers();
	glutPostRedisplay();
}


static
void keydown (unsigned char key, int x, int y)
{
	switch (key) {
	case 'd':			/* delta, triangle */
		break;
	case 'o':			/* round */
		freezecol=1;
		break;
	case 'q':			/* square*/
		break;
	case 'x':			/* cross button */
		exit(0);
		break;
	default:
		;
	}
}


static
void keyup (unsigned char key, int x, int y)
{
	switch (key) {
	case 'o':
		freezecol=0;
		break;
	default:
		;
	}
}


static
void joystick (unsigned int buttonMask, int x, int y, int z)
{
	GLCHK(glClearColor(x * 1.0f/2000.0f + 0.5f, y * 1.0f/2000.0f + 0.5f, 1.0f, 1.0f));
}

int main(int argc, char* argv[])
{
	unsigned int pix[128*128];
	int i;

	glutInit(&argc, argv);

#if SYS
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
        glutInitWindowSize(480, 272);
#endif
	glutCreateWindow( __FILE__ );
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutJoystickFunc(joystick, 0);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	for(i = 0; i < 128*128; i++)
		pix[i] = rand();
	GLCHK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix));

	GLCHK(glEnable(GL_TEXTURE_2D));

	glutMainLoop();
	return 0;
}
