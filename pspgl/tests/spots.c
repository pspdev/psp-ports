#include <stdlib.h>
#define GL_GLEXT_PROTOTYPES

#include <GL/glut.h>
#include <math.h>

#define LOGME 0
#include "glchk.h"
#include "perfmeter.h"

struct vertex {
	float s,t;
	float nx,ny,nz;
	float x,y,z;
};

static const struct light {
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat ambient[4];
} lights[4] = {
	{
		{ 0, .6, .6, 1 },
		{ 0, 1, 1, 1 },
	},
	{
		{ .6, 0, .6, 1 },
		{ 1, 0, 1, 1 },
	},
	{
		{ .6, .6, 0, 1 },
		{ 1, 1, 0, 1 },
	},
	{
		{ .3, .6, .6, 1 },
		{ .5, 1, 1, 1 },
	}
};

static float cutoff = 40, d_cutoff = 0;
static float exponent = 40, d_exponent = 0;
static int nlights = 4;
static int lights_changed = 1;
static int shot = 0;

static void set_lights()
{
	int i;

	if (!lights_changed)
		return;
	lights_changed = 0;

	for(i = 0; i < 4; i++) {
		GLCHK(glLightfv(GL_LIGHT0+i, GL_DIFFUSE, lights[i].diffuse));
		GLCHK(glLightfv(GL_LIGHT0+i, GL_SPECULAR, lights[i].specular));
		GLCHK(glLightfv(GL_LIGHT0+i, GL_AMBIENT, lights[i].ambient));

		GLCHK(glLightf(GL_LIGHT0+i, GL_SPOT_EXPONENT, exponent));
		GLCHK(glLightf(GL_LIGHT0+i, GL_SPOT_CUTOFF, cutoff));

		if (i < nlights)
			GLCHK(glEnable(GL_LIGHT0+i));
		else
			GLCHK(glDisable(GL_LIGHT0+i));
	}
}


#define MESH	64

#if MESH*MESH < 256
typedef unsigned char idx_t;
#define IDX_TYPE	GL_UNSIGNED_BYTE
#else
typedef unsigned short idx_t;
#define IDX_TYPE	GL_UNSIGNED_SHORT
#endif

static struct vertex mesh[MESH*MESH];
static idx_t meshidx[(MESH-1)*MESH*2];

static void genmesh(void)
{
	int x,y;

	for(y = 0; y < MESH; y++) {
		float fy = (float)y / MESH;

		for(x = 0; x < MESH; x++) {
			struct vertex *v = &mesh[y*MESH+x];
			float fx = (float)x / MESH;

			v->s = fx;
			v->t = fy;
			v->x = fx - .5;
			v->y = fy - .5;
			v->z = 0;//hypotf(fx-.5,fy-.5);
			v->nx = 0;
			v->ny = 0;
			v->nz = 1;
		}
	}

	for(x = 0; x < (MESH-1)*MESH*2; x += 2) {
		meshidx[x+0] = (x/2)+MESH;
		meshidx[x+1] = x/2;
	}
}

static int width, height;

static
void reshape (int w, int h)
{
	width = w;
	height = h;

	GLCHK(glEnable(GL_SCISSOR_TEST));
	GLCHK(glScissor(0,0,w,h));
	GLCHK(glViewport(0, 0, w, h));
	GLCHK(glMatrixMode(GL_PROJECTION));
	GLCHK(glLoadIdentity());
	gluPerspective(30., 16./9., 1., 40.);

	GLCHK(glMatrixMode(GL_MODELVIEW));
}


static float delta = 1.0;
static float elevation = 45;
static float bearing = 30;

extern unsigned char firefox_start[];


static
void display (void)
{
	static GLfloat angle;
	int i;

	pm_framestart();

	angle += delta;

	GLCHK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glLoadIdentity());

	if (d_exponent != 0) {
		exponent += d_exponent;
		if (exponent >= 128)
			exponent = 128;
		if (exponent < 1)
			exponent = 1;
		lights_changed = 1;
	}

	if (d_cutoff != 0) {
		cutoff += d_cutoff;
		if (cutoff > 90)
			cutoff = 90;
		if (cutoff < 2)
			cutoff = 2;
		lights_changed = 1;
	}

	set_lights();

	static const float len = 6;

	float se = sinf(elevation * M_PI / 180);
	float ce = cosf(elevation * M_PI / 180);
	float sb = sinf(bearing * M_PI / 180);
	float cb = cosf(bearing * M_PI / 180);

	float x = ce * cb * len;
	float y = ce * sb * len;
	float z = se * len;

	gluLookAt(x,y,z,  0,0,.5,  0,0,1);

	GLCHK(glDisable(GL_TEXTURE_2D));
	GLCHK(glEnable(GL_BLEND));
	//GLCHK(glEnable(GL_CULL_FACE));
	GLCHK(glEnable(GL_DEPTH_TEST));
	GLCHK(glDisable(GL_LIGHTING));
	GLCHK(glShadeModel(GL_SMOOTH));
	GLCHK(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));


	for(i = 0; i < nlights; i++) {
		GLfloat pos[] = { 1, 0, 0, 1 };
		GLfloat vec[] = {-1, 0, 0 };

		GLCHK(glPushMatrix());

		GLCHK(glRotatef(angle + 90*i, 0,0,1));
		GLCHK(glRotatef(-40, 0,1,0));
		GLCHK(glTranslatef(.9,.9,0));

		GLCHK(glLightfv(GL_LIGHT0 + i, GL_POSITION, pos));
		GLCHK(glLightfv(GL_LIGHT0 + i, GL_SPOT_DIRECTION, vec));

		GLCHK(glColor4fv(lights[i].specular));

		glBegin(GL_LINE_STRIP);
		glVertex3fv(pos);
		glColor4f(0,0,0,0);
		glVertex3f(pos[0]+vec[0],
			   pos[1]+vec[1],
			   pos[2]+vec[2]);
		glEnd();

		GLCHK(glPopMatrix());
	}

	//GLCHK(glColor4f(1,1,1,1));
	//GLCHK(glRotatef(angle, 0,0,1));
	GLCHK(glScalef(3,3,1));
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLCHK(glDisable(GL_BLEND));

	GLCHK(glEnable(GL_LIGHTING));
#if 1
	GLCHK(glDisable(GL_TEXTURE_2D));
	for(i = 0; i < MESH-1; i++)
		GLCHK(glDrawElements(GL_TRIANGLE_STRIP, MESH*2, IDX_TYPE, NULL+(i * MESH*2 * sizeof(idx_t))));
#endif
#if 1
	GLCHK(glTranslatef(0,0,.001));
	GLCHK(glEnable(GL_BLEND));
	GLCHK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCHK(glEnable(GL_TEXTURE_2D));
	for(i = 0; i < MESH-1; i++)
		GLCHK(glDrawElements(GL_TRIANGLE_STRIP, MESH*2, IDX_TYPE, NULL+(i * MESH*2 * sizeof(idx_t))));
#endif
#if 0
	GLCHK(glTranslatef(0,0,3));
	GLCHK(glScalef(-1,1,-1));
	//GLCHK(glDisable(GL_BLEND));
	GLCHK(glDisable(GL_TEXTURE_2D));
	for(i = 0; i < MESH-1; i++)
		GLCHK(glDrawElements(GL_TRIANGLE_STRIP, MESH*2, IDX_TYPE, NULL+(i * MESH*2 * sizeof(idx_t))));
#endif

	pm_frameend();

	if (shot) {
		shot = 0;
		screenshot("spots");
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

static void special_down(int key, int x, int y)
{
	switch(key) {
	case GLUT_KEY_UP:
		d_exponent = 2;
		break;

	case GLUT_KEY_DOWN:
		d_exponent = -2;
		break;

	case GLUT_KEY_LEFT:
		d_cutoff = 1;
		break;

	case GLUT_KEY_RIGHT:
		d_cutoff = -1;
		break;
	}

	lights_changed = 1;
}

static void special_up(int key, int x, int y)
{
	switch(key) {
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		d_exponent = 0;
		break;

	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		d_cutoff = 0;
		break;
	}
}

static int js_lock = 0;

static
void keydown (unsigned char key, int x, int y)
{
	switch (key) {
	case 'd':			/* delta, triangle */
		nlights = (nlights + 1) % 5;
		lights_changed = 1;
		break;
	case 'o':			/* round */
		delta = (delta == 0) ? 1 : 0;
		break;
	case 'q':			/* square*/
		js_lock = !js_lock;
		break;
	case 'a':		/* start */
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
	case 'd':
		break;
	case 'o':
		//delta = 1;
		break;
	case 'q':
		break;
	default:
		;
	}
}

#if !SYS
static
void joystick (unsigned int buttonMask, int x, int y, int z)
{
	if (js_lock && hypotf(x,y) < 500)
		return;
	js_lock = 0;

	elevation += y / 500.;
	if (elevation > 89.)
		elevation = 89;
	if (elevation < -10.)
		elevation = -10.;
	bearing += x / 500.;
}
#else  /* SYS */
static void motion(int x, int y)
{
	elevation = (height-y) * 109 / height - 10;
	bearing = x * 360 / width;
}
#endif

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(480, 272);
	glutCreateWindow( __FILE__ );

#if !SYS
	glutJoystickFunc(joystick, 0);
#else
	glutPassiveMotionFunc(motion);
#endif

	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutSpecialFunc(special_down);
	glutSpecialUpFunc(special_up);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	GLCHK(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE));

	GLCHK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, 
			   GL_RGBA, GL_UNSIGNED_BYTE, firefox_start));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	genmesh();

	GLCHK(glEnableClientState(GL_VERTEX_ARRAY));
	GLCHK(glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), &mesh[0].x));

	GLCHK(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
	GLCHK(glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex), &mesh[0].s));

	GLCHK(glEnableClientState(GL_NORMAL_ARRAY));
	GLCHK(glNormalPointer(GL_FLOAT, sizeof(struct vertex), &mesh[0].nx));

	GLCHK(glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 1));
	GLCHK(glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,
			      sizeof(meshidx), meshidx,
			      GL_STATIC_DRAW_ARB));

	{
		GLfloat ambient[] = { .3,.3,.3,1 };
		GLCHK(glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient));
		
		GLCHK(glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR));
	}

	{
		GLfloat diff[] = { 1,1,1,1 };
		GLfloat spec[] = { 1,1,1,1 };
		GLfloat ambient[] = { .4,.4,.4,1 };

		GLCHK(glMaterialfv(GL_FRONT, GL_DIFFUSE, diff));
		GLCHK(glMaterialfv(GL_FRONT, GL_AMBIENT, ambient));
		GLCHK(glMaterialfv(GL_FRONT, GL_SPECULAR, spec));
		GLCHK(glMateriali(GL_FRONT, GL_SHININESS, 40));
	}

	GLCHK(glLockArraysEXT(0, MESH*MESH));

	GLCHK(glEnable(GL_DEPTH_TEST));
	GLCHK(glClearColor(0,0,.5,1));

	GLCHK(glEnable(GL_NORMALIZE));


#if GL_PSP_statistics
	GLCHK(glEnableStatsPSP(GL_STATS_TIMING_PSP));
#endif

//	GLCHK(glPolygonOffset(1,0));
//	GLCHK(glEnable(GL_POLYGON_OFFSET_FILL));

	glutMainLoop();

	return 0;
}
