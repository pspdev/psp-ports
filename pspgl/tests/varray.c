#define GL_GLEXT_PROTOTYPES
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#include "glchk.h"
#include "perfmeter.h"

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


static float delta = 1.0;
static int lock_arrays = 0;

extern unsigned char firefox_start[];

/*
          1
     2      
         +   0
     3
          4
 */

static const GLfloat vtx[] = 
{
	1.000000, 0.000000,
	0.309017, 0.951057,
	-0.809017, 0.587785,
	-0.809017, -0.587785,
	0.309017, -0.951057,
};

static const GLfloat cols[] = {
	1, 1, 0,
	0, 1, 1,
	1, 0, 1,
	.5, 0, 1,
	0, .5, .5
};

static const GLuint idx[] = {
	0, 1, 4, 2, 3
};

static const GLushort triidx[] = {
	0, 1, 2, 0, 3, 4, 2, 0, 3,
};

static const GLubyte staridx[] = {
	1, 4, 2, 0, 3, 1
};

#define CVERT	160
static GLfloat cvert[CVERT*2];
static GLubyte cstripidx[CVERT];

static
void display (void)
{
	static GLfloat angle;

	angle += delta;

	pm_framestart();

	GLCHK(glColor3f(1,1,0));
	GLCHK(glClear(GL_COLOR_BUFFER_BIT));

	GLCHK(glMatrixMode(GL_TEXTURE));
	GLCHK(glLoadIdentity());

	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glLoadIdentity());
	GLCHK(glRotatef(angle * 1.32f, 0.0f, 0.0f, 1.0f));

	/* set up pentagon/star arrays */
	GLCHK(glVertexPointer(2, GL_FLOAT, 0, vtx));
	GLCHK(glEnableClientState(GL_VERTEX_ARRAY));

	GLCHK(glTexCoordPointer(2, GL_FLOAT, 0, vtx));
	GLCHK(glEnableClientState(GL_TEXTURE_COORD_ARRAY));

	GLCHK(glColorPointer(3, GL_FLOAT, 0, cols));
	GLCHK(glEnableClientState(GL_COLOR_ARRAY));

	if (lock_arrays)
		GLCHK(glLockArraysEXT(0, 5));

#if 1
	/* simple direct triangle fan */
	GLCHK(glEnable(GL_BLEND));
	GLCHK(glEnable(GL_TEXTURE_2D));
	GLCHK(glDrawArrays(GL_TRIANGLE_FAN, 0, 5));

	/* outline */
	GLCHK(glDisable(GL_TEXTURE_2D));
	GLCHK(glDisable(GL_BLEND));
	GLCHK(glDrawArrays(GL_LINE_LOOP, 0, 5));


	GLCHK(glTranslatef(1,1,0));
	
	/* indexed fan */
	GLCHK(glEnable(GL_BLEND));
	GLCHK(glEnable(GL_TEXTURE_2D));
	GLCHK(glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_INT, idx));

	/* wireframe */
	GLCHK(glDisable(GL_TEXTURE_2D));
	GLCHK(glDisable(GL_BLEND));
	GLCHK(glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_INT, idx));

	GLCHK(glTranslatef(-2,-2,0));

	/* indexed independent triangles */
	GLCHK(glEnable(GL_BLEND));
	GLCHK(glEnable(GL_TEXTURE_2D));
	GLCHK(glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, triidx));

	/* indexed star */
	GLCHK(glDisable(GL_TEXTURE_2D));
	GLCHK(glDisable(GL_BLEND));
	GLCHK(glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_BYTE, staridx));
#endif

#if 1
	GLCHK(glTranslatef(2,0,0));
	GLCHK(glEnable(GL_TEXTURE_2D));

	/* element fan */
	glBegin(GL_TRIANGLE_STRIP);
		glArrayElement(0);
		glArrayElement(1);
		glArrayElement(4);
		glArrayElement(2);
		glArrayElement(3);
	GLCHK(glEnd());

	GLCHK(glDisable(GL_TEXTURE_2D));
	GLCHK(glDisableClientState(GL_COLOR_ARRAY));
	GLCHK(glColor3f(1,1,.5));
	
	/* element line fan */
	glBegin(GL_LINES);
		glArrayElement(0);
		glArrayElement(1);

		glArrayElement(0);
		glArrayElement(4);

		glArrayElement(0);
		glArrayElement(2);

		glArrayElement(0);
		glArrayElement(3);
	GLCHK(glEnd());
#endif

#if 1
	/* lots of vertices */
	GLCHK(glVertexPointer(2, GL_FLOAT, 0, cvert));
	GLCHK(glTexCoordPointer(2, GL_FLOAT, 0, cvert));

	GLCHK(glDisableClientState(GL_COLOR_ARRAY));

	if (lock_arrays) {
		GLCHK(glUnlockArraysEXT());
		GLCHK(glLockArraysEXT(0, CVERT));
	}

	GLCHK(glColor4f(.5,.5,.5,.5));

	GLCHK(glEnable(GL_BLEND));
	GLCHK(glEnable(GL_TEXTURE_2D));

	GLCHK(glLoadIdentity());

	GLCHK(glMatrixMode(GL_TEXTURE));
	GLCHK(glLoadIdentity());
	GLCHK(glTranslatef(.5,.5,0));
	GLCHK(glScalef(.25,.25,1));

	//GLCHK(glDrawArrays(GL_TRIANGLE_FAN, 0, CVERT));
	GLCHK(glDrawElements(GL_TRIANGLE_STRIP, CVERT, GL_UNSIGNED_BYTE, cstripidx));

	GLCHK(glDisable(GL_TEXTURE_2D));
	GLCHK(glDisable(GL_BLEND));
	GLCHK(glColor3f(1,1,1));
	GLCHK(glDrawArrays(GL_LINE_LOOP, 0, CVERT));

	if (lock_arrays)
		GLCHK(glUnlockArraysEXT());
#endif

	pm_frameend();

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
		lock_arrays = 1;
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
	case 'q':
		lock_arrays = 1;
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
	int i;
	float a;

	glutInit(&argc, argv);
#if SYS
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
        glutInitWindowSize(480, 272);
#else
	glutJoystickFunc(joystick, 0);
#endif
	glutCreateWindow( __FILE__ );
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	GLCHK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, 
			   GL_RGBA, GL_UNSIGNED_BYTE, firefox_start));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	GLCHK(glShadeModel(GL_SMOOTH));

	GLCHK(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE));
	GLCHK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	for(i = 0, a = 0; a < 2*M_PI; a += 2*M_PI/CVERT, i++) {
		cvert[i*2+0] = cos(a) * 2;
		cvert[i*2+1] = sin(a) * 2;
	}
	for(i = 0; i < CVERT; i++) {
		if (i & 1)
			cstripidx[i] = CVERT-1-(i/2);
		else
			cstripidx[i] = i/2;
	}

	GLCHK(glClearColor(0,0,.5,.1));

	glutMainLoop();
	return 0;
}
