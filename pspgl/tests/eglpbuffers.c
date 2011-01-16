#include <math.h>
#include <stdlib.h>

#include <pspctrl.h>

#define GL_GLEXT_PROTOTYPES

#include <GLES/egl.h>
#include <GLES/gl.h>

#define LOGME 0

#include "glchk.h"
#include "perfmeter.h"

#define SCR_WIDTH	480
#define SCR_HEIGHT	272

#define BONES	8

static const int screen_attrib[] = {
	EGL_RED_SIZE,		8,
	EGL_GREEN_SIZE,		8,
	EGL_BLUE_SIZE,		8,
	EGL_ALPHA_SIZE,		0,
	EGL_STENCIL_SIZE,	0,
	EGL_DEPTH_SIZE,		0,
	EGL_NONE
};

static const int pbuffer_attrib[] = {
	EGL_RED_SIZE,	8,
	EGL_GREEN_SIZE,	8,
	EGL_BLUE_SIZE,	8,
	EGL_ALPHA_SIZE,	8,
	EGL_DEPTH_SIZE,	16,

	EGL_NONE
};

#define TILESIZE	128
#define TILEW		2
#define TILEH		4

#define TILEX(idx)	((idx) / TILEH)
#define TILEY(idx)	((idx) % TILEH)

static const int texture_attrib[] = {
	EGL_TEXTURE_FORMAT,	EGL_TEXTURE_RGBA,
	EGL_TEXTURE_TARGET,	EGL_TEXTURE_2D,
	EGL_WIDTH,		TILESIZE * TILEW,
	EGL_HEIGHT,		TILESIZE * TILEH,

	EGL_NONE
};


//#define CYL_SLICES	48
//#define CYL_ROWS	48
#define CYL_SLICES	16
#define CYL_ROWS	16
#define CYL_RADIUS	.35f
#define CYL_LENGTH	1.25f

#define NVERT	(CYL_SLICES * CYL_ROWS)
#define NTRIS	(CYL_SLICES * (CYL_ROWS - 1) * 6)
#define NSTRIPS (CYL_SLICES * CYL_ROWS * 2)

struct vertex {
	float w[BONES];
	float nx, ny, nz;
	float x, y, z;
};

#if NVERT*2 <= 256
typedef unsigned char idx_t;
#define IDXTYPE	GL_UNSIGNED_BYTE
#else
typedef unsigned short idx_t;
#define IDXTYPE	GL_UNSIGNED_SHORT
#endif

struct indices {
	idx_t tris[NTRIS];
	idx_t tristrips[NSTRIPS];
	idx_t cap0[CYL_SLICES], cap1[CYL_SLICES];
	idx_t norms[NVERT*2];
};

#define min( a, b ) ( ((a)<(b))?(a):(b) )
#define max( a, b ) ( ((a)>(b))?(a):(b) )

/* useful geometry functions */
void genSkinnedCylinder( unsigned slices, unsigned rows, float length, float radius, unsigned bones,
			 struct vertex  *dstVertices, idx_t* dstIndices, idx_t *stripidx,
			 idx_t *cap0, idx_t *cap1 )
{
	unsigned int i,j;

	float lengthStep = length / (float)rows;
	float boneStep = ((float)bones-1)/((float)rows);

	// generate torus
	for (j = 0; j < slices; ++j)
	{
		for (i = 0; i < rows; ++i)
		{
			struct vertex* curr = &dstVertices[i+j*rows];
			float t = j;
			float ct, st;
			int q;

			//psp_log("v: slice=%d row=%d\n", j, i);

			//cs = cosf(s * (2*M_PI)/slices);
			//ss = sinf(s * (2*M_PI)/slices);

			ct = cosf(t * (2*M_PI)/rows);
			st = sinf(t * (2*M_PI)/rows);

			if (i == (rows - 1)) {
				float s;

				curr->nx = 1;
				curr->ny = ct;
				curr->nz = st;
				s = 1/sqrtf(curr->nx*curr->nx + 
					    curr->ny*curr->ny + 
					    curr->nz*curr->nz);
				curr->nx *= s;
				curr->ny *= s;
				curr->nz *= s;
			} else {
				curr->nx = 0;
				curr->ny = ct;
				curr->nz = st;
			}

			curr->x = lengthStep * (float)i;
			curr->y = radius * ct;
			curr->z = radius * st;

			for(q = 0 ; q < bones; q++ )
			{
				float b = min( ((float)bones-1), boneStep * (float)i );
				float t = b - (float)q;

				// calculate cubic blending functions for skinning weights
				// each vertex is influenced by 4 bones (matrices) at most

				float t2 = t*t;
				float t3 = t*t*t;

				float f = 0;
				if( t >= 0.0f && t < 1.0f )	f =  t3/6.0f;
				if( t >= 1.0f && t < 2.0f )	f = -0.5f*t3 + 2.0f*t2 - 2.0f*t  +  2.0f/3.0f; 			
				if( t >= 2.0f && t < 3.0f )	f =  0.5f*t3 - 4.0f*t2 + 10.0f*t - 22.0f/3.0f;
				if( t >= 3.0f && t < 4.0f )	f = -t3/6.0f + 2.0f*t2 - 8.0f*t  + 32.0f/3.0f;

				curr->w[q] = f;
			}
		}
	}

	for (j = 0; j < slices; ++j)
	{
		for (i = 0; i < rows-1; ++i)
		{
			idx_t * curr = &dstIndices[(i+(j*(rows-1)))*6];

			//psp_log("i: slice=%d row=%d idx=%d\n", j, i, (i+(j*(rows-1)))*6);

			*curr++ = i + j * rows;
			*curr++ = (i+1) + j * rows;
			*curr++ = i + ((j+1)%slices) * rows;

			*curr++ = (i+1) + j * rows;
			*curr++ = (i+1) + ((j+1)%slices) * rows;
			*curr++ = i + ((j+1)%slices) * rows;
		}

		for (i = 0; i < rows; ++i)
		{
			idx_t * curr = &stripidx[(i+(j*rows))*2];

			//psp_log("i: slice=%d row=%d idx=%d\n", j, i, (i+(j*(rows-1)))*6);

			*curr++ = i + ((j+1)%slices) * rows;
			*curr++ = i + j * rows;
		}

		{
			int idx;

			if (j % 2)
				idx = CYL_SLICES-1-j/2;
			else
				idx = j/2;
		
			cap0[j] = idx*rows;
			cap1[j] = idx*rows+(rows-1);
		}
	}

	//psp_log("cylinder done\n");
}

static GLuint tube_idx, tube_vtx;
static struct vertex *tube_vertices;
static struct indices *tube_indices;

static void init_tubes()
{
	struct vertex *vtxmap;
	struct indices *idxmap;

	GLCHK(glGenBuffers(1, &tube_vtx));
	GLCHK(glBindBuffer(GL_ARRAY_BUFFER, tube_vtx));
	GLCHK(glBufferData(GL_ARRAY_BUFFER, 2 * NVERT * sizeof(struct vertex),
			   NULL, GL_STATIC_DRAW));
	GLCHK(vtxmap = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE));
	tube_vertices = 0;

	GLCHK(glGenBuffers(1, &tube_idx));
	GLCHK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tube_idx));
	GLCHK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(struct indices),
			   NULL, GL_STATIC_DRAW));
	GLCHK(idxmap = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
	tube_indices = 0;

	genSkinnedCylinder(CYL_SLICES, CYL_ROWS, CYL_LENGTH, CYL_RADIUS, BONES, 
			   vtxmap, idxmap->tris, idxmap->tristrips, 
			   idxmap->cap0, idxmap->cap1);

	GLCHK(glUnmapBuffer(GL_ARRAY_BUFFER));
	GLCHK(glBindBuffer(GL_ARRAY_BUFFER, 0));

	GLCHK(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));
	GLCHK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

static void draw_tubes(int arms, float angle, int phase)
{
	GLCHK(glBindBuffer(GL_ARRAY_BUFFER, tube_vtx));
	GLCHK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tube_idx));

	GLCHK(glEnableClientState(GL_VERTEX_ARRAY));
	GLCHK(glVertexPointer(3, GL_FLOAT, sizeof(struct vertex),
			      &tube_vertices[0].x));

	GLCHK(glEnableClientState(GL_NORMAL_ARRAY));
	GLCHK(glNormalPointer(GL_FLOAT, sizeof(struct vertex),
			      &tube_vertices[0].nx));

	GLCHK(glEnableClientState(GL_WEIGHT_ARRAY_PSP));
	GLCHK(glWeightPointerPSP(BONES, GL_FLOAT, sizeof(struct vertex),
				 &tube_vertices[0].w));

	GLCHK(glEnable(GL_VERTEX_BLEND_PSP));

	GLCHK(glDisableClientState(GL_TEXTURE_COORD_ARRAY));

	for(int j = 0; j < BONES; j++) {
		GLCHK(glMatrixMode(GL_BONE0_PSP + j));
		GLCHK(glLoadIdentity());

		GLCHK(glRotatef((180. / M_PI) * cosf(phase * (M_PI/180.)), 0, 0, 1));
		if (j > 0) {
			GLfloat prev_bone[16];

			GLCHK(glTranslatef(CYL_LENGTH, 0, 0));
			GLCHK(glGetFloatv(GL_BONE0_PSP + j - 1, prev_bone));
			GLCHK(glMultMatrixf(prev_bone));
		}
	}

	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glLoadIdentity());
	GLCHK(glTranslatef(0,0,-10));
	GLCHK(glRotatef(40 + angle*.1, 0, 1, 0));

	for(int i = 0; i < arms; i++) {

		GLCHK(glMatrixMode(GL_MODELVIEW));
		GLCHK(glPushMatrix());
		GLCHK(glRotatef(angle + 360*i/arms, 0, 0, 1));

		for(int s = 0; s < CYL_SLICES; s++)
			GLCHK(glDrawElements(GL_TRIANGLE_STRIP, CYL_ROWS*2, IDXTYPE,
					     &tube_indices->tristrips[s * CYL_ROWS * 2]));

		//GLCHK(glDrawElements(GL_TRIANGLE_FAN, CYL_SLICES, GL_UNSIGNED_SHORT, cap0));
		GLCHK(glDrawElements(GL_TRIANGLE_STRIP, CYL_SLICES, IDXTYPE, tube_indices->cap1));

		GLCHK(glPopMatrix());
	}

	GLCHK(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCHK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCHK(glDisableClientState(GL_VERTEX_ARRAY));
	GLCHK(glDisableClientState(GL_NORMAL_ARRAY));
	GLCHK(glDisableClientState(GL_WEIGHT_ARRAY_PSP));
}

struct cube_vtx {
	float s, t;
	float x, y, z;
};

static void draw_cube()
{
	/* no sharing of vertices between faces; they don't have the
	   same attributes */
#define ST(face, s, t)					\
	((((float)s) + TILEX(face)) / TILEW),		\
	((((float)t) + TILEY(face)) / TILEH)

	static const struct cube_vtx faces[] = {
		/* top: y = 1, tex tile 0 */
		{ ST(0, 0, 0),  -1,  1, -1 },
		{ ST(0, 1, 0),   1,  1, -1 },
		{ ST(0, 1, 1),   1,  1,  1 },
		{ ST(0, 0, 1),  -1,  1,  1 },

		/* bottom: y = -1 */
		{ ST(1, 0, 0),  -1, -1,  1 },
		{ ST(1, 1, 0),   1, -1,  1 },
		{ ST(1, 1, 1),   1, -1, -1 },
		{ ST(1, 0, 1),  -1, -1, -1 },

		/* left: x = -1 */
		{ ST(2, 0, 0),  -1, -1,  1 },
		{ ST(2, 1, 0),  -1, -1, -1 },
		{ ST(2, 1, 1),  -1,  1, -1 },
		{ ST(2, 0, 1),  -1,  1,  1 },

		/* right: x = 1 */
		{ ST(3, 0, 1),   1,  1,  1 },
		{ ST(3, 1, 1),   1,  1, -1 },
		{ ST(3, 1, 0),   1, -1, -1 },
		{ ST(3, 0, 0),   1, -1,  1 },

		/* front: z = -1 */
		{ ST(4, 0, 0),  -1, -1, -1 },
		{ ST(4, 1, 0),   1, -1, -1 },
		{ ST(4, 1, 1),   1,  1, -1 },
		{ ST(4, 0, 1),  -1,  1, -1 },

		/* back: z = 1 */
		{ ST(5, 0, 0),   1, -1,  1 },
		{ ST(5, 1, 0),  -1, -1,  1 },
		{ ST(5, 1, 1),  -1,  1,  1 },
		{ ST(5, 0, 1),   1,  1,  1 },
#undef ST
	};
	static const GLubyte indices[] = {
#define FACE(face)	\
		(face * 4 + 0), (face * 4 + 1), (face * 4 + 2), \
		(face * 4 + 0), (face * 4 + 2), (face * 4 + 3)

		FACE(0),
		FACE(1),
		FACE(2),
		FACE(3),
		FACE(4),
		FACE(5),
#undef FACE
	};
	static const int nvert = sizeof(indices)/sizeof(*indices);

	GLCHK(glEnableClientState(GL_VERTEX_ARRAY));
	GLCHK(glDisableClientState(GL_NORMAL_ARRAY));
	GLCHK(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
	GLCHK(glDisableClientState(GL_WEIGHT_ARRAY_PSP));

	GLCHK(glVertexPointer(3, GL_FLOAT, sizeof(struct cube_vtx), &faces[0].x));
	GLCHK(glTexCoordPointer(2, GL_FLOAT, sizeof(struct cube_vtx), &faces[0].s));

	GLCHK(glLockArraysEXT(0, nvert));

	GLCHK(glFrontFace(GL_CCW));
	GLCHK(glDrawRangeElements(GL_TRIANGLES, 0, nvert-1, nvert,
				  GL_UNSIGNED_BYTE, indices));

	GLCHK(glFrontFace(GL_CW));
	GLCHK(glDrawRangeElements(GL_TRIANGLES, 0, nvert-1, nvert,
				  GL_UNSIGNED_BYTE, indices));

	GLCHK(glUnlockArraysEXT());
}

static void init_geometry()
{
	init_tubes();
}

static void display(EGLContext ctx, EGLDisplay dpy,
		    EGLSurface screen, EGLSurface pbuffer,
		    int rotate)
{
	static float angle;
	static int phase;

	/* set up context to render to pbuffer */
	EGLCHK(eglMakeCurrent(dpy, pbuffer, pbuffer, ctx));
	GLCHK(glViewport(0, 0, TILESIZE * TILEW, TILESIZE * TILEH));
	GLCHK(glScissor(0, 0, TILESIZE * TILEW, TILESIZE * TILEH));

	/* Set up projection: 1:1 aspect ratio for the texture */
	GLCHK(glMatrixMode(GL_PROJECTION));
	GLCHK(glLoadIdentity());
	gluPerspectivef(50., 1, 1., 100.);

	GLCHK(glClearColor(0,0,0,0));
	GLCHK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	/* render to texture tiles */
	GLCHK(glEnable(GL_SCISSOR_TEST));
	GLCHK(glDisable(GL_TEXTURE_2D));
	GLCHK(glDisable(GL_BLEND));
	GLCHK(glEnable(GL_DEPTH_TEST));
	GLCHK(glEnable(GL_LIGHTING));
	GLCHK(glEnable(GL_CULL_FACE));
	GLCHK(glFrontFace(GL_CW));
	GLCHK(glShadeModel(GL_SMOOTH));

	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glPushMatrix());
	for(int i = 0; i < 6; i++) {
		GLCHK(glViewport(TILESIZE * TILEX(i), TILESIZE * TILEY(i),
				 TILESIZE, TILESIZE));
		/* leave a border around each tile */
		GLCHK(glScissor(TILESIZE * TILEX(i) + 1, TILESIZE * TILEY(i) + 1,
				TILESIZE - 2, TILESIZE - 2));

		glClearColor(.1,.1,.1,.1);
		glClear(GL_COLOR_BUFFER_BIT);
		/* draw stuff into cube face */
		draw_tubes(i+1, angle + i*30, phase + i);
	}
	if (rotate)
		angle += 2.0f;
	phase++;

	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glPopMatrix());


	/* now render to the screen */
	EGLCHK(eglMakeCurrent(dpy, screen, screen, ctx));
	GLCHK(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
	GLCHK(glDisable(GL_SCISSOR_TEST));

	/* use default texture */
	EGLCHK(eglBindTexImage(dpy, pbuffer, EGL_BACK_BUFFER));

	/* render with texture */
	GLCHK(glClearColor(.3,.3,0,1));
	GLCHK(glClear(GL_COLOR_BUFFER_BIT));

	GLCHK(glEnable(GL_TEXTURE_2D));
	GLCHK(glEnable(GL_BLEND));
	GLCHK(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
	GLCHK(glDisable(GL_DEPTH_TEST));
	GLCHK(glDisable(GL_LIGHTING));
	GLCHK(glDisable(GL_VERTEX_BLEND_PSP));

#if 0
	GLCHK(glDisable(GL_CULL_FACE));
	GLCHK(glMatrixMode(GL_PROJECTION));
	GLCHK(glLoadIdentity());
	GLCHK(glOrthof(-16./9, 16./9, -1, 1, -1, 1));
	GLCHK(glMatrixMode(GL_MODELVIEW));

	const float aspect = (float)TILEW / TILEH;

	glBegin(GL_TRIANGLE_FAN);
	  glTexCoord2f(0, 0);
	  glVertex2f(-aspect, -1);

	  glTexCoord2f(1, 0);
	  glVertex2f(aspect, -1);

	  glTexCoord2f(1, 1);
	  glVertex2f(aspect, 1);

	  glTexCoord2f(0, 1);
	  glVertex2f(-aspect, 1);
	glEnd();

	GLCHK(glColor3f(1,1,1));
	GLCHK(glDisable(GL_TEXTURE_2D));
	glBegin(GL_LINE_LOOP);
	  glVertex2f(-aspect, -1);
	  glVertex2f(aspect, -1);
	  glVertex2f(aspect, 1);
	  glVertex2f(-aspect, 1);
	glEnd();
#else
	GLCHK(glMatrixMode(GL_PROJECTION));
	GLCHK(glLoadIdentity());
	gluPerspectivef(45., 16./9, 1., 100.);

	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glLoadIdentity());
	GLCHK(gluLookAtf(0.0f, 0.0f, 5.0f,
			 0.0f, 0.0f, 0.0f,
			 0.0f, 1.0f, 0.0f));
	GLCHK(glRotatef(angle * 0.79f, 1.0f, 0.0f, 0.0f));
	GLCHK(glRotatef(angle * 0.98f, 0.0f, 1.0f, 0.0f));
	GLCHK(glRotatef(angle * 1.32f, 0.0f, 0.0f, 1.0f));

	draw_cube();
#endif
}

int main()
{
	EGLDisplay	dpy;
	EGLConfig	screen_config, pbuffer_config;
	EGLContext	ctx;
	EGLSurface	screen, pbuffer;
	int		nconfigs;
	int		running = 1;

	EGLCHK(dpy = eglGetDisplay(0));
	EGLCHK(eglInitialize(dpy, NULL, NULL));

	EGLCHK(eglChooseConfig(dpy, screen_attrib, &screen_config, 1, &nconfigs));
	if (nconfigs == 0) {
		__pspgl_log("Couldn't find config for screen\n");
		return 0;
	}

	EGLCHK(eglChooseConfig(dpy, pbuffer_attrib, &pbuffer_config, 1, &nconfigs));
	if (nconfigs == 0) {
		__pspgl_log("Couldn't find config for pbuffer\n");
		return 0;
	}

	EGLCHK(ctx = eglCreateContext(dpy, screen_config, EGL_NO_CONTEXT, NULL));
	EGLCHK(screen = eglCreateWindowSurface(dpy, screen_config, 0, NULL));
	EGLCHK(pbuffer = eglCreatePbufferSurface(dpy, pbuffer_config, texture_attrib));

	EGLCHK(eglMakeCurrent(dpy, screen, screen, ctx));

	init_geometry();

#if 0
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_DEBUG_PSP, GL_TRUE));
#endif
	GLCHK(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));
	GLCHK(glDisable(GL_DITHER));

	{
		GLfloat diffcol0[] = { .4, .4, 1, 1 };
		GLfloat lightdir0[] = { 0, 0, 1, 0 };
		GLfloat diffcol1[] = { .6, .2, .1, 1 };
		GLfloat lightdir1[] = { .707, .707, 0, 0 };

		GLCHK(glEnable(GL_LIGHT0));
		GLCHK(glLightfv(GL_LIGHT0, GL_DIFFUSE, diffcol0));
		GLCHK(glLightfv(GL_LIGHT0, GL_POSITION, lightdir0));

		GLCHK(glEnable(GL_LIGHT1));
		GLCHK(glLightfv(GL_LIGHT1, GL_DIFFUSE, diffcol1));
		GLCHK(glLightfv(GL_LIGHT1, GL_POSITION, lightdir1));
	}

	while(running) {
		SceCtrlData pad;
		int rotate = 1;

		sceCtrlReadBufferPositive(&pad, 1); 

		if (pad.Buttons & PSP_CTRL_CROSS)
			running = 0;

		if (pad.Buttons & PSP_CTRL_CIRCLE)
			rotate = 0;

		pm_framestart();
		display(ctx, dpy, screen, pbuffer, rotate);
		pm_frameend();

		if (pad.Buttons & PSP_CTRL_START)
			screenshot("eglpbuffer");

		EGLCHK(eglSwapBuffers(dpy, screen));
	}

	EGLCHK(eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE,
			      EGL_NO_CONTEXT));
	EGLCHK(eglDestroySurface(dpy, screen));
	EGLCHK(eglDestroySurface(dpy, pbuffer));
	EGLCHK(eglDestroyContext(dpy, ctx));

	return 0;
}
