#include <stdlib.h>
#define GL_GLEXT_PROTOTYPES

#include <GL/glut.h>
#include <math.h>

#include "glchk.h"


static
void reshape (int w, int h)
{
	GLCHK(glViewport(0, 0, w, h));
	GLCHK(glMatrixMode(GL_PROJECTION));
	GLCHK(glLoadIdentity());
	GLCHK(glOrtho(0, 1, 0, 1, -1, 1));

	GLCHK(glMatrixMode(GL_MODELVIEW));

	glutPostRedisplay();
}


static
void display (void)
{
	GLCHK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GLCHK(glShadeModel(GL_SMOOTH));

	/* middle band */
	glColor3f(1,1,0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,.25,0);
	glVertex3f(1,.25,0);
	glVertex3f(1,.75,0);
	glVertex3f(0,.75,0);
	glEnd();

	/* top strip */
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,.9,0);
	glVertex3f(1,.9,0);
	glVertex3f(1,1,0);
	glVertex3f(0,1,0);
	glEnd();

	/* ramp */
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0,0,0);
	glVertex3f(0,0,-1);	/* near */

	glColor3f(1,1,1);
	glVertex3f(1,0,1);	/* far */

	glVertex3f(1,.5,1);	/* far */

	glColor3f(0,0,0);
	glVertex3f(0,.5,-1);	/* near */

	glEnd();

	glutSwapBuffers();
}


static
void keydown (unsigned char key, int x, int y)
{
	switch (key) {
	case 'd':			/* delta, triangle */
		GLCHK(glDepthFunc(GL_GEQUAL));
		break;
	case 'o':			/* round */
		GLCHK(glDisable(GL_DEPTH_TEST));
		break;
	case 'q':			/* square*/
		break;
	case 'x':			/* cross button */
		exit(0);
		break;
	default:
		;
	}

	glutPostRedisplay();
}


static
void keyup (unsigned char key, int x, int y)
{
	switch (key) {
	case 'd':
		GLCHK(glDepthFunc(GL_LESS));
		break;
	case 'o':
		GLCHK(glEnable(GL_DEPTH_TEST));
		break;
	default:
		;
	}

	glutPostRedisplay();
}

static
void joystick (unsigned int buttonMask, int x, int y, int z)
{
	GLCHK(glClearColor(x * 1.0f/2000.0f + 0.5f, y * 1.0f/2000.0f + 0.5f, 1.0f, 1.0f));
	glutPostRedisplay();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(480, 272);
	glutJoystickFunc(joystick, 0);
	glutCreateWindow( __FILE__ );
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	
	GLCHK(glDepthFunc(GL_LESS));
	GLCHK(glEnable(GL_DEPTH_TEST));

	GLCHK(glDepthRange(1,0));

	glutMainLoop();

	return 0;
}
