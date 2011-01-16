#include <stdlib.h>
#define GL_GLEXT_PROTOTYPES

#include <GL/glut.h>
#include <math.h>
#include <sys/time.h>

#include "glchk.h"
#include "perfmeter.h"

extern unsigned char firefox_start[];



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
static float exponent = 20, d_exponent = 0;
static int nlights = 4;
static int lights_changed = 1;
static unsigned render;
static const GLenum prims[] = { GL_TRIANGLES, GL_LINES, GL_POINTS };
static int primidx = 0;
static int shot = 0;

#define R_HULL	(1<<0)
#define R_NORM	(1<<1)
#define R_TEX	(1<<2)
#define R_FLAT	(1<<3)

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

/* Mesh dimensions; should be of the form 4+3*n; if not, then any
   unused vertices will be ignored */
#define MESHX	10
#define MESHY	10

#if MESHX*MESHY < 256
typedef unsigned char idx_t;
#define IDX_TYPE	GL_UNSIGNED_BYTE
#else
typedef unsigned short idx_t;
#define IDX_TYPE	GL_UNSIGNED_SHORT
#endif

static struct vertex mesh[MESHX*MESHY];

static void genmesh(void)
{
	int x,y;

	for(y = 0; y < MESHY; y++) {
		float fy = (float)y / (MESHY-1);
		int ty = y * 64 / (MESHY-1);

		for(x = 0; x < MESHX; x++) {
			struct vertex *v = &mesh[y*MESHX+x];
			float fx = (float)x / (MESHX-1);
			int tx = x * 64 / (MESHX-1);

			v->s = fx;
			v->t = fy;
			v->x = fx - .5;
			v->y = fy - .5;
			/* generate height map from green channel */
			v->z = firefox_start[ty * 64*4 + tx*4 + 1] / 255.;
		}
	}

	/* second pass for normals */
	for(y = 0; y < MESHY; y++) {
		for(x = 0; x < MESHX; x++) {
			struct vertex *p;
			static const struct {
				int dx, dy;
			} n[] = {
				{ -1, 0, },
				{  0, 1, },
				{  1, 0, },
				{  0,-1, },
				{ -1, 0, },
			};
			int i;
			float norm[3] = { 0, 0, 0 };
			float len;

			p = &mesh[y*MESHX+x];

			for(i = 0; i < 4; i++) {
				const struct vertex *p0, *p1;
				float v0[3], v1[3];

				if (y + n[i].dy < 0 ||
				    y + n[i].dy >= MESHY)
					continue;
				if (x + n[i].dx < 0 ||
				    x + n[i].dx >= MESHX)
					continue;
				if (y + n[i+1].dy < 0 ||
				    y + n[i+1].dy >= MESHY)
					continue;
				if (x + n[i+1].dx < 0 ||
				    x + n[i+1].dx >= MESHX)
					continue;

				p0 = &mesh[(y+n[i+1].dy)*MESHX + (x+n[i+1].dx)];
				p1 = &mesh[(y+n[i  ].dy)*MESHX + (x+n[i  ].dx)];

				v0[0] = p->x - p0->x;
				v0[1] = p->y - p0->y;
				v0[2] = p->z - p0->z;

				v1[0] = p->x - p1->x;
				v1[1] = p->y - p1->y;
				v1[2] = p->z - p1->z;

				norm[0] +=  v0[1] * v1[2] - v0[2] * v1[1];
				norm[1] +=  v0[2] * v1[0] - v0[0] * v1[2];
				norm[2] +=  v0[0] * v1[1] - v0[1] * v1[0];
			}

			len = sqrt(norm[0]*norm[0] +
				   norm[1]*norm[1] +
				   norm[2]*norm[2]);

			p->nx = norm[0] / len;
			p->ny = norm[1] / len;
			p->nz = norm[2] / len;
		}
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

static
void display (void)
{
	static GLfloat angle;
	int i;

	angle += delta;

	pm_framestart();

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
	GLCHK(glShadeModel(render & R_FLAT ? GL_FLAT : GL_SMOOTH));
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

	GLCHK(glScalef(3,3,1));

	GLCHK(glDisable(GL_BLEND));

	/* axes */
	glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex3f(0,0,0);
		glVertex3f(1,0,0);

		glColor3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,1,0);

		glColor3f(1,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,0,1);
	glEnd();

	if (render & R_TEX)
		GLCHK(glEnable(GL_TEXTURE_2D));
	GLCHK(glEnable(GL_LIGHTING));
#if GL_PSP_bezier_patch
	if ((primidx / 3) == 0) {
		if (0) {
			GLCHK(glDrawBezierArraysPSP(prims[primidx%3], MESHX, MESHY, 0));
		} else {
			idx_t idx[MESHX*MESHY];
			int x, y, i;

			i = 0;
			for(y = 0; y < MESHY-3; y++)
				for(x = 0; x < MESHX; x++)
					idx[i++] = y*MESHX+x;
			GLCHK(glDrawBezierElementsPSP(prims[primidx%3],
						      MESHX, MESHY-3, IDX_TYPE, idx));

			/* need to overlap a row */
			i = 0;
			for(y = MESHY-4; y < MESHY; y++)
				for(x = 0; x < MESHX; x++)
					idx[i++] = y*MESHX+x;
			GLCHK(glDrawBezierElementsPSP(prims[primidx%3],
						      MESHX, 4, IDX_TYPE, idx));
		}
	} else {
		if (0) {
			GLCHK(glDrawSplineArraysPSP(prims[primidx%3],
						    MESHX, MESHY,
						    GL_PATCH_OUTER_INNER_PSP,
						    GL_PATCH_INNER_INNER_PSP,
						    0));
		} else {
			idx_t idx[MESHX*MESHY];
			int x, y, i;

			i = 0;
			for(y = 0; y < MESHY-3; y++)
				for(x = 0; x < MESHX; x++)
					idx[i++] = y*MESHX+x;
			GLCHK(glDrawSplineElementsPSP(prims[primidx%3],
						      MESHX, MESHY-3,
						      GL_PATCH_OUTER_OUTER_PSP,
						      GL_PATCH_INNER_OUTER_PSP,
						      IDX_TYPE, idx));

			/* need to overlap 3 rows */
			i = 0;
			for(y = MESHY-6; y < MESHY; y++)
				for(x = 0; x < MESHX; x++)
					idx[i++] = y*MESHX+x;
			GLCHK(glDrawSplineElementsPSP(prims[primidx%3],
						      MESHX, 6,
						      GL_PATCH_OUTER_OUTER_PSP,
						      GL_PATCH_OUTER_INNER_PSP,
						      IDX_TYPE, idx));
		}
	}
#else
	glBegin(GL_QUADS);
	for(i = 0; i < MESHY-1; i++) {
		int j;
		for(j = 0; j < MESHX-1; j++) {
			glArrayElement(i*MESHX + j);
			glArrayElement(i*MESHX + j+1);
			glArrayElement((i+1)*MESHX + j+1);
			glArrayElement((i+1)*MESHX + j);
		}
	}
	GLCHK(glEnd());
#endif

	GLCHK(glDisable(GL_LIGHTING));
	GLCHK(glDisable(GL_TEXTURE_2D));
	GLCHK(glShadeModel(GL_SMOOTH));

	GLCHK(glEnable(GL_BLEND));
	GLCHK(glColor4f(.2,.2,.2,.2));
	GLCHK(glBlendFunc(GL_ONE, GL_ONE));

	if (render & R_HULL) {
		/* draw hull */
		for(i = 0; i < MESHY; i++)
			GLCHK(glDrawArrays(GL_LINE_STRIP, i*MESHX, MESHX));

		for(i = 0; i < MESHX; i++) {
			idx_t idx[MESHY];
			int j;

			for(j = 0; j < MESHY; j++)
				idx[j] = i + j*MESHX;

			GLCHK(glDrawElements(GL_LINE_STRIP, MESHY, IDX_TYPE, idx));
		}
	}

	if (render & R_NORM) {
		/* draw normals */
		glBegin(GL_LINES);
		for(i = 0; i < MESHX*MESHY; i++) {
			glVertex3fv(&mesh[i].x);
			glVertex3f(mesh[i].x + mesh[i].nx * .1,
				   mesh[i].y + mesh[i].ny * .1,
				   mesh[i].z + mesh[i].nz * .1);
		}
		glEnd();
	}

	pm_frameend();

	if (shot) {
		shot = 0;
		screenshot("bezier");
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
	case 'x':
		render++;
		break;
	case 's':		/* select */
		primidx = (primidx + 1) % 6;
		break;
	case 'a':			/* startbutton */
		shot = 1;
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

	GLCHK(glLockArraysEXT(0, MESHX*MESHY));

	GLCHK(glEnable(GL_DEPTH_TEST));
	GLCHK(glClearColor(0,0,.5,1));

	GLCHK(glEnable(GL_NORMALIZE));

#if GL_PSP_bezier_patch
	GLCHK(glPatchSubdivisionPSP(8,8));
	//GLCHK(glPatchSubdivisionPSP(16,16));
	//GLCHK(glPatchSubdivisionPSP(64,64));
#endif

//	GLCHK(glPolygonOffset(1,0));
//	GLCHK(glEnable(GL_POLYGON_OFFSET_FILL));

	glutMainLoop();

	return 0;
}
