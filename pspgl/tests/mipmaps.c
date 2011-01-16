#define GL_GLEXT_PROTOTYPES

#include <stdlib.h>
#include <GL/glut.h>

#include "glchk.h"
#include "perfmeter.h"

static
void reshape (int w, int h)
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(0,0,w,h);

	GLCHK(glViewport(0, 0, w, h));
	GLCHK(glMatrixMode(GL_PROJECTION));

	GLCHK(glLoadIdentity());
	gluPerspective(30., 16./9., 1., 1000.);

	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glLoadIdentity());
}


static float delta = 1.0;
static int mipmaps = 1;
static int shot = 0;

#define NTEX	2
static GLuint texid[NTEX];

#define GROUND_SIZE	256
extern unsigned char ground_start[];
#define SKY_SIZE	128
extern unsigned char sky_start[];

#define MESH	64

#if MESH*MESH < 256
typedef unsigned char idx_t;
#define IDX_TYPE	GL_UNSIGNED_BYTE
#else
typedef unsigned short idx_t;
#define IDX_TYPE	GL_UNSIGNED_SHORT
#endif

struct vertex {
	float s,t;
	float x,y,z;
	float nx,ny,nz;
};

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

			v->s = fx*10;
			v->t = fy*10;
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

#define FILTER		GL_NEAREST
#define FILTER_MIPMAP	GL_NEAREST_MIPMAP_NEAREST


static
void display (void)
{
	static GLfloat angle;
	int i;

	pm_framestart();

	angle += delta;

	GLCHK(glShadeModel(GL_SMOOTH));

	GLCHK(glClear(GL_COLOR_BUFFER_BIT));

	GLCHK(glColor4f(1,1,1,1));
	GLCHK(glEnable(GL_TEXTURE_2D));
	GLCHK(glDisable(GL_BLEND));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

#if GL_PSP_view_matrix
	GLCHK(glMatrixMode(GL_VIEW_PSP));
	GLCHK(glLoadIdentity());
#endif

	gluLookAt(50, 50, 10,
		  0, 0, 1,
		  0, 0, 1);
	GLCHK(glRotatef(angle, 0,0,1));
	GLCHK(glMatrixMode(GL_MODELVIEW));

	GLCHK(glScalef(100,100,1));

	GLCHK(glBindTexture(GL_TEXTURE_2D, texid[0]));
	if (mipmaps) {
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				      FILTER_MIPMAP));
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				      FILTER));
	} else {
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				      FILTER));
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				      FILTER));
	}
	for(i = 0; i < MESH-1; i++)
		GLCHK(glDrawElements(GL_TRIANGLE_STRIP, MESH*2, IDX_TYPE, NULL+(i * MESH*2 * sizeof(idx_t))));

	GLCHK(glBindTexture(GL_TEXTURE_2D, texid[1]));
	if (mipmaps) {
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				      FILTER_MIPMAP));
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				      FILTER));
	} else {
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				      FILTER));
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				      FILTER));
	}

	glTranslatef(0,0,15);
	for(i = 0; i < MESH-1; i++)
		GLCHK(glDrawElements(GL_TRIANGLE_STRIP, MESH*2, IDX_TYPE, NULL+(i * MESH*2 * sizeof(idx_t))));


	pm_frameend();

#if SYS
	usleep(1000000/100);
#endif

	if (shot) {
		shot = 0;
		screenshot("mipmaps");
	}

	glutSwapBuffers();
	glutPostRedisplay();
}


static
void keydown (unsigned char key, int x, int y)
{
	switch (key) {
	case 'd':			/* delta, triangle */
		mipmaps = 0;
		break;
	case 'o':			/* round */
		delta = 0.0f;
		break;
	case 'a':		/* start */
		shot = 1;
		break;
	case 'n':			/* note */
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

	case 'd':
		mipmaps = 1;
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

static int lg2(int x)
{
	int ret = -1;

	if (x == 0 || ((x-1) & x))
		return -1;

	while(x) {
		x >>= 1;
		ret++;
	}

	return ret;
}

static void build_tinted_mipmaps(unsigned width, unsigned height, const void *data)
{
	int level, levels;
	static const GLubyte  tints[] = {
		255,255,255,255, /* white */
		255,255,192,255, /* yellow */
		255,192,255,255, /* magenta */
		255,192,192,255, /* red */
		192,255,255,255, /* cyan */
		192,255,192,255, /* green */
		192,192,255,255, /* blue */
		192,192,192,255, /* black */
	};
	GLubyte *dst;
	const GLubyte *src;
	unsigned tw = width;
	unsigned th = height;

	levels = lg2(width);
	level = lg2(height);
	if (level > levels)
		levels = level;

	src = data;
	dst = NULL;
	for(level = 0; level <= levels; level++) {
		unsigned r,g,b, i;

		r = tints[level*4 + 0];
		g = tints[level*4 + 1];
		b = tints[level*4 + 2];

		dst = malloc(tw * th * 4);

		gluScaleImage(GL_RGBA,
			      width, height, GL_UNSIGNED_BYTE, src,
			      tw, th, GL_UNSIGNED_BYTE, dst);

		for(i = 0;  i < tw*th*4; i += 4) {
			dst[i+0] = dst[i+0] * r / 256;
			dst[i+1] = dst[i+1] * g / 256;
			dst[i+2] = dst[i+2] * b / 256;
		}
#if 0
		for(i = 0; i < tw*4; i += 4) {
			dst[i+0] = 255;
			dst[i+1] = 0;
			dst[i+2] = 0;
		}
		for(i = 0; i < tw*th*4; i += tw*4) {
			dst[i+0] = 0;
			dst[i+1] = 255;
			dst[i+2] = 0;
		}
#endif
		glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, 
			     tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, dst);

		if (src != data)
			free((void *)src);
		src = dst;
		width = tw;
		height = th;

		if (tw > 1)
			tw /= 2;
		if (th > 1)
			th /= 2;
	}
	free(dst);
}

int main(int argc, char* argv[])
{
	psp_log("main starting\n");

	glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(480, 272);
	glutCreateWindow( __FILE__ );
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
#if !SYS
	glutJoystickFunc(joystick, 0);
#else
	glClearColor(.3,.3,.6,1);
#endif
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	GLCHK(glGenTextures(NTEX, texid));

	GLCHK(glBindTexture(GL_TEXTURE_2D, texid[0]));
	if (1) {
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE));
#if GL_PSP_mipmap_debug
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_DEBUG_PSP, GL_TRUE));
#endif
		GLCHK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
				   GROUND_SIZE, GROUND_SIZE, 0,
				   GL_RGBA, GL_UNSIGNED_BYTE, ground_start));
	} else {
		if (0)
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, GROUND_SIZE, GROUND_SIZE, 
					  GL_RGBA, GL_UNSIGNED_BYTE, ground_start);
		else
			build_tinted_mipmaps(GROUND_SIZE, GROUND_SIZE, ground_start);
	}

	GLCHK(glBindTexture(GL_TEXTURE_2D, texid[1]));
	if (1) {
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE));
#if GL_PSP_mipmap_debug
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_DEBUG_PSP, GL_TRUE));
#endif
		GLCHK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
				   SKY_SIZE, SKY_SIZE, 0,
				   GL_RGBA, GL_UNSIGNED_BYTE, sky_start));
	} else {
		if (0)
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, SKY_SIZE, SKY_SIZE, 
					  GL_RGBA, GL_UNSIGNED_BYTE, sky_start);
		else
			build_tinted_mipmaps(SKY_SIZE, SKY_SIZE, sky_start);
	}

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
	GLCHK(glLockArraysEXT(0, MESH*MESH));

	glutMainLoop();
	return 0;
}
