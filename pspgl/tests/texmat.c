#include <stdlib.h>
#include <GL/glut.h>

#include "glchk.h"


static
void reshape (int w, int h)
{
	GLCHK(glViewport(0, 0, w, h));
	GLCHK(glMatrixMode(GL_PROJECTION));
	GLCHK(glLoadIdentity());
	GLCHK(glOrtho(-2*16.f/9.f, 2*16.f/9.f, -2, 2, -2, 2));
	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glLoadIdentity());
}

static int smooth, texmat = 1, clamp, projmat = 1;

static
void display (void)
{
	GLCHK(glShadeModel(GL_SMOOTH));

	GLCHK(glClear(GL_COLOR_BUFFER_BIT));

	GLCHK(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));
	//GLCHK(glEnable(GL_BLEND));
	GLCHK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCHK(glMatrixMode(GL_TEXTURE));
	GLCHK(glLoadIdentity());

	if (texmat) {
		//GLCHK(glScalef(.5,.5,1));
		if (projmat) {
			static const float m[] = {
#if 0
				  .5,  .5, 0,  0,
				 -.5,  .5, 0,  0,
				   0,   0, 1,  0,
				 -.5,  -1, 0,  1,
#else
#if 0
				   0.651613,  0.000000,  0.000000,  0.000000,
				  -0.433013, -0.758819, -0.866026, -0.866026,
				  -0.250000,  0.314313, -0.500000, -0.500000,
				   0.5000000, 0.500000,  3.000000,  3.000000,
#else
				   0.651613,  0.000000,  0.000000,  0.000000,
				  -0.433013, -0.758819, 0, -0.866026,
				  -0.250000,  0.314313, 0, -0.500000,
				   0.5000000, 0.500000,  0,  3.000000,
#endif
#endif
			};
			glLoadMatrixf(m);
		} else {
			static const float m[] = {
				  .5,   0, 0,  0,
				   0,  .5, 0,  0,
				   0,   0, 0,  0,
				   0,   0, 0,  1,
			};
			glLoadMatrixf(m);
		}
	}

	if (clamp) {
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	} else {
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	}

	if (smooth) {
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	} else {
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	}

	GLCHK(glEnable(GL_TEXTURE_2D));

	glBegin(GL_TRIANGLE_FAN);

#if 1
	  glTexCoord2f(0,0);
	  glVertex2f(-2,-2);

	  glTexCoord2f(2,0);
	  glVertex2f(2,-2);

	  glTexCoord2f(2,2);
	  glVertex2f(2,2);

	  glTexCoord2f(0,2);
	  glVertex2f(-2,2);
#else
	  glTexCoord2f(-2,-2);
	  glVertex2f(-2,-2);

	  glTexCoord2f(2,-2);
	  glVertex2f(2,-2);

	  glTexCoord2f(2,2);
	  glVertex2f(2,2);

	  glTexCoord2f(-2,2);
	  glVertex2f(-2,2);
#endif
	glEnd();

	glutSwapBuffers();
	glutPostRedisplay();
}


static
void keydown (unsigned char key, int x, int y)
{
	switch (key) {
	case 'd':			/* delta, triangle */
		texmat = !texmat;
		break;
	case 'o':			/* round */
		smooth = !smooth;
		break;
	case 'q':			/* square*/
		clamp = !clamp;
		break;
	case 'x':			/* cross button */
		projmat = !projmat;
		break;
	case 'a':		/* start */
	case 27:
		exit(0);

	default:
		;
	}
}


static
void keyup (unsigned char key, int x, int y)
{
	switch (key) {
	case 'o':
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

static const GLubyte texture[] = {
	 255,  0,  0,  128,128,128,  128,128,128,    0,255,  0,
	 128,128,128,  128,128,128,  128,128,128,  128,128,128,
	 128,128,128,  128,128,128,  128,128,128,  128,128,128,
	 255,255,  0,  128,128,128,  128,128,128,    0,255,255,
};

int main(int argc, char* argv[])
{
	psp_log("main starting\n");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
        glutInitWindowSize(480, 272);
	glutCreateWindow( __FILE__ );
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutJoystickFunc(joystick, 0);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	GLCHK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, texture));

	glutMainLoop();
	return 0;
}
