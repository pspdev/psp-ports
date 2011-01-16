#include <stdlib.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include <math.h>

#include "glchk.h"
#include "perfmeter.h"

#if !SYS
#include <pspuser.h>
#endif

static int width, height;
static
void reshape (int w, int h)
{
	width = w;
	height = h;

	GLCHK(glViewport(0, 0, w, h));
	GLCHK(glMatrixMode(GL_PROJECTION));
	GLCHK(glLoadIdentity());
	GLCHK(glOrtho(0, w, 0, h, -1, 1));
	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glLoadIdentity());
	GLCHK(glScissor(0, 0, w, h));
}


static int bounce = 1;
static int shot = 0;

extern unsigned char firefox_start[];

#define NVERT	16
static struct
{
	float s, t;
	float x, y;
} shape[NVERT];

#define NMAG	5
static struct magnify {
	int x, y;
	int dx, dy, dr;
	int rad;
} mag[NMAG];

static
void display (void)
{
	int i;
	static GLfloat angle;

	pm_framestart();

	angle += 1.;

	GLCHK(glShadeModel(GL_SMOOTH));

	GLCHK(glClear(GL_COLOR_BUFFER_BIT));

	GLCHK(glColor4f(0,0,0,1));

	GLCHK(glLoadIdentity());
	GLCHK(glTranslatef(width/2, height/2, 0));
	GLCHK(glScalef(height/2, height/2, 1));
	GLCHK(glRotatef(angle, 0, 0, 1));

	GLCHK(glDisable(GL_BLEND));
	GLCHK(glEnable(GL_TEXTURE_2D));
	GLCHK(glBindTexture(GL_TEXTURE_2D, 1));
	GLCHK(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));

	GLCHK(glDrawArrays(GL_TRIANGLE_FAN, 0, NVERT));

	for(i = 0; i < NMAG; i++) {
		struct magnify *m = &mag[i];
		//int texdim = pow2(m->rad/2);
		int texdim = 128;

		GLCHK(glEnable(GL_TEXTURE_2D));
		GLCHK(glBindTexture(GL_TEXTURE_2D, 2));

		GLCHK(glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
				       m->x - (texdim/2), m->y - (texdim/2),
				       texdim, texdim, 0));

		GLCHK(glLoadIdentity());
		GLCHK(glTranslatef(m->x, m->y, 0));
		GLCHK(glScalef(m->rad, m->rad, 1));
		GLCHK(glDrawArrays(GL_TRIANGLE_FAN, 0, NVERT));

		GLCHK(glDisable(GL_TEXTURE_2D));
		GLCHK(glDrawArrays(GL_LINE_LOOP, 0, NVERT));

		if (bounce) {
			m->x += m->dx;
			m->y += m->dy;
			m->rad += m->dr;

			if ((m->x < m->rad) || (m->x > 480-m->rad))
				m->dx = -m->dx;
			if ((m->y < m->rad) || (m->y > (272-m->rad)))
				m->dy = -m->dy;

			if ((m->rad < 10) || (m->rad > 100))
				m->dr = -m->dr;
		}
	}

	pm_frameend();

	//usleep(1000000/30);

	if (shot) {
		shot = 0;
		screenshot("copytex");
	}

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
		bounce = 0;
		break;
	case 'q':			/* square*/
		break;
	case 'a':
		shot = 1;
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
		bounce = 1;
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

	glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
        glutInitWindowSize(480, 272);
	glutCreateWindow( __FILE__ );
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
#if !SYS
	glutJoystickFunc(joystick, 0);
#endif
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	for(i = 0; i < NVERT; i++) {
		float a = (i * M_PI * 2) / NVERT;
		float s = sin(a);
		float c = cos(a);

		shape[i].x = c;
		shape[i].y = s;
		//shape[i].z = 0;
		shape[i].s = c/2.f + .5f;
		shape[i].t = s/2.f + .5f;
	}


	for(i = 0; i < NMAG; i++) {
		int rad = rand() % 40 + 10;
		int x = rand() % (480-rad*2) + rad;
		int y = rand() % (272-rad*2) + rad;

		mag[i].x = x;
		mag[i].y = y;
		mag[i].rad = rad;
		mag[i].dx = (rand() % 5) - 2;
		mag[i].dy = (rand() % 5) - 2;
		mag[i].dr = (rand() % 3) - 1;
	}

	GLCHK(glClearColor(.3,.7,.1,1));

	GLCHK(glVertexPointer(2, GL_FLOAT, sizeof(shape[0]), &shape[0].x));
	GLCHK(glTexCoordPointer(2, GL_FLOAT, sizeof(shape[0]), &shape[0].s));
	GLCHK(glEnableClientState(GL_VERTEX_ARRAY));
	GLCHK(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
	GLCHK(glLockArraysEXT(0, NVERT));

	GLCHK(glEnable(GL_SCISSOR_TEST));

	/* logo texture */
	GLCHK(glBindTexture(GL_TEXTURE_2D, 1));
	GLCHK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, 
			   GL_RGBA, GL_UNSIGNED_BYTE, firefox_start));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	/* magnifier texture */
	GLCHK(glBindTexture(GL_TEXTURE_2D, 2));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_DEBUG_PSP, GL_TRUE));

	GLCHK(glEnable(GL_TEXTURE_2D));

#if GL_PSP_statistics
	GLCHK(glEnableStatsPSP(GL_STATS_TIMING_PSP));
#endif

	glutMainLoop();
	return 0;
}
