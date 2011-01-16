#include <stdlib.h>
#include <GL/glut.h>

#include "glchk.h"

static
void reshape (int w, int h)
{
	GLCHK(glViewport(0, 0, w, h));
	GLCHK(glMatrixMode(GL_PROJECTION));
	GLCHK(glLoadIdentity());
	GLCHK(glOrtho(-2, 2, -2, 2, -2, 2));
	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glLoadIdentity());
}


static float delta = 1.0;

static
void display (void)
{
	static GLfloat angle;
	angle += delta;

	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glLoadIdentity());
	//GLCHK(glTranslatef(0.0f, 0.0f, -2.5f));
//	GLCHK(glRotatef(angle * 0.79f, 1.0f, 0.0f, 0.0f));
//	GLCHK(glRotatef(angle * 0.98f, 0.0f, 1.0f, 0.0f));
	GLCHK(glRotatef(angle * 1.32f, 0.0f, 0.0f, 1.0f));

	GLCHK(glShadeModel(GL_SMOOTH));

	GLCHK(glClear(GL_COLOR_BUFFER_BIT));
	glBegin(GL_TRIANGLES);
		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	GLCHK(glEnd());
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
		delta = 0.0f;
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
		delta = 1.0f;
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
	glutInit(&argc, argv);
	glutCreateWindow( __FILE__ );
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutJoystickFunc(joystick, 0);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}

