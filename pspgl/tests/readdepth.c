#include <stdlib.h>
#define GL_GLEXT_PROTOTYPES

#include <GL/glut.h>
#include <math.h>
#include <malloc.h>
#include <string.h>

#include "glchk.h"
#include "perfmeter.h"

#define DO_VB	(1 && GL_PSP_vertex_blend)
#define USE_VBO		1
#define USE_VBO_IDX	1
#define USE_TRISTRIPS	1
#define USE_CVA		0
#define USE_PBO		1

#define BONES	8

//#define CYL_SLICES	48
//#define CYL_ROWS	48
#define CYL_SLICES	16
#define CYL_ROWS	16
#define CYL_RADIUS	.35f
#define CYL_LENGTH	1.25f

#define NVERT	(CYL_SLICES * CYL_ROWS)
#define NTRIS	(CYL_SLICES * (CYL_ROWS - 1) * 6)
#define NSTRIPS (CYL_SLICES * CYL_ROWS * 2)

static int shot = 0;
static GLuint buffers[3];

static struct vertex {
#if DO_VB || 0
	float w[BONES];
#endif
	float nx, ny, nz;
	float x, y, z;
} *vertices;

#if NVERT*2 <= 256
typedef unsigned char idx_t;
#define IDXTYPE	GL_UNSIGNED_BYTE
#else
typedef unsigned short idx_t;
#define IDXTYPE	GL_UNSIGNED_SHORT
#endif

static struct indices {
	idx_t tris[NTRIS];
	idx_t tristrips[NSTRIPS];
	idx_t cap0[CYL_SLICES], cap1[CYL_SLICES];
	idx_t norms[NVERT*2];
} *indices;

#define min( a, b ) ( ((a)<(b))?(a):(b) )
#define max( a, b ) ( ((a)>(b))?(a):(b) )

typedef GLushort depth_t;
static void *depth;

#define DEPTHSZ	256
#define DEPTHBYTES	(DEPTHSZ*DEPTHSZ*sizeof(depth_t))
#define CMAPBYTES	(sizeof(short) * 65536)

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

#if DO_VB
				curr->w[q] = f;
#endif
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


static
void reshape (int w, int h)
{
	GLCHK(glViewport(0, 0, w, h));
	GLCHK(glMatrixMode(GL_PROJECTION));
	GLCHK(glLoadIdentity());
	gluPerspective(50., 16./9., 1., 100.);

	GLCHK(glMatrixMode(GL_MODELVIEW));
}


static float delta = 1.0;
static int show_normals = 0;

extern unsigned char firefox_start[];

static
void display (void)
{
	static int val;
	static const int ARMS = 5;
	int i;
	static GLfloat angle;

	pm_framestart();

	if (delta) {
		angle += delta;
		val++;
	}

	GLCHK(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
	GLCHK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#if DO_VB
	int j;
	for(j = 0; j < BONES; j++) {
		GLCHK(glMatrixMode(GL_BONE0_PSP + j));
		GLCHK(glLoadIdentity());

		GLCHK(glRotatef((180. / M_PI) * cosf(val * (M_PI/180.)), 0, 0, 1));
		if (j > 0) {
			GLfloat prev_bone[16];

			GLCHK(glTranslatef(CYL_LENGTH, 0, 0));
			GLCHK(glGetFloatv(GL_BONE0_PSP + j - 1, prev_bone));
			GLCHK(glMultMatrixf(prev_bone));
		}
	}
#endif

	GLCHK(glDisable(GL_BLEND));
	GLCHK(glEnable(GL_LIGHTING));
	GLCHK(glEnable(GL_DEPTH_TEST));
	GLCHK(glEnable(GL_CULL_FACE));
	GLCHK(glShadeModel(GL_SMOOTH));

	GLCHK(glMatrixMode(GL_MODELVIEW));
	GLCHK(glLoadIdentity());
	GLCHK(glTranslatef(0,0,-10.f));
	GLCHK(glRotatef(40 + angle*.1, 0, 1, 0));

	for(i = 0; i < ARMS; i++) {

		GLCHK(glMatrixMode(GL_MODELVIEW));
		GLCHK(glPushMatrix());
		GLCHK(glRotatef(angle + 360*i/ARMS, 0, 0, 1));

		if (USE_TRISTRIPS) {
			int s;

			for(s = 0; s < CYL_SLICES; s++)
				GLCHK(glDrawElements(GL_TRIANGLE_STRIP, CYL_ROWS*2, IDXTYPE,
						     &indices->tristrips[s * CYL_ROWS * 2]));
		} else
			GLCHK(glDrawElements(GL_TRIANGLES, NTRIS, IDXTYPE, indices->tris));

		//GLCHK(glDrawElements(GL_TRIANGLE_FAN, CYL_SLICES, GL_UNSIGNED_SHORT, cap0));
		GLCHK(glDrawElements(GL_TRIANGLE_STRIP, CYL_SLICES, IDXTYPE, indices->cap1));

		if (show_normals) {
			GLCHK(glDisable(GL_LIGHTING));
			GLCHK(glEnable(GL_BLEND));
			GLCHK(glDepthMask(GL_FALSE));

			GLCHK(glColor4f(.5,.5,.5,.5));

			GLCHK(glDrawElements(GL_LINES, NVERT*2, IDXTYPE, indices->norms));

			GLCHK(glColor4f(.25,.25,0,.25));

			GLCHK(glDrawElements(GL_LINE_STRIP, NTRIS, IDXTYPE, indices->tris));
			GLCHK(glDrawElements(GL_LINE_STRIP, CYL_SLICES, IDXTYPE, indices->cap1));
			GLCHK(glEnable(GL_LIGHTING));
			GLCHK(glDisable(GL_BLEND));
			GLCHK(glDepthMask(GL_TRUE));
		}

		GLCHK(glPopMatrix());
	}

	{
		GLCHK(glPushAttrib(GL_ENABLE_BIT));

		GLCHK(glEnable(GL_TEXTURE_2D));
		GLCHK(glDisable(GL_DEPTH_TEST));
#if DO_VB
		GLCHK(glDisable(GL_VERTEX_BLEND_PSP));
#endif
		GLCHK(glDisable(GL_CULL_FACE));
		GLCHK(glDisable(GL_LIGHTING));
		GLCHK(glDisable(GL_BLEND));
		GLCHK(glColor4f(1,1,1,1));

		GLCHK(glPushMatrix());
		GLCHK(glLoadIdentity());
		GLCHK(glTranslatef(480/2-(DEPTHSZ/2), 272/2-(DEPTHSZ/2), 0));
		GLCHK(glMatrixMode(GL_PROJECTION));
		GLCHK(glPushMatrix());
		GLCHK(glLoadIdentity());
		GLCHK(glOrtho(0, 480, 0, 272, 0, 1));

		GLCHK(glReadPixels(480/2-(DEPTHSZ/2), 272/2-(DEPTHSZ/2), DEPTHSZ, DEPTHSZ,
				   GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, depth));
		if (!SYS && 1) {
			GLCHK(glTexImage2D(GL_TEXTURE_2D, 0, GL_COLOR_INDEX16_EXT, DEPTHSZ, DEPTHSZ, 0,
					   GL_COLOR_INDEX16_EXT, GL_UNSIGNED_SHORT, depth));
		} else {
			GLCHK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DEPTHSZ, DEPTHSZ, 0,
					   GL_RGB, GL_UNSIGNED_SHORT_5_6_5_REV, depth));
		}

		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		glBegin(GL_TRIANGLE_FAN);
		  glTexCoord2f(0,0);
		  glVertex2f(0,0);

		  glTexCoord2f(1,0);
		  glVertex2f(256,0);

		  glTexCoord2f(1,1);
		  glVertex2f(256,256);

		  glTexCoord2f(0,1);
		  glVertex2f(0,256);
		glEnd();

		GLCHK(glPopMatrix());
		GLCHK(glMatrixMode(GL_MODELVIEW));
		GLCHK(glPopMatrix());

		GLCHK(glPopAttrib());
	}

	pm_frameend();

	if (shot) {
		shot = 0;
		screenshot("readdepth");
	}

#if 0
	glFinish();
	__pspgl_vram_dump();
#endif

	glutSwapBuffers();
	glutPostRedisplay();
}


static
void keydown (unsigned char key, int x, int y)
{
	switch (key) {
	case 'd':			/* delta, triangle */
		show_normals = 1;
		break;
	case 'o':			/* round */
		delta = 0.0f;
		break;
	case 'q':			/* square*/
		if (USE_CVA)
			GLCHK(glUnlockArraysEXT());
		else
			GLCHK(glLockArraysEXT(0, NVERT));
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
	case 'd':
		show_normals = 0;
		break;
	case 'o':
		delta = 1.0f;
		break;
	case 'q':
		if (USE_CVA)
			GLCHK(glLockArraysEXT(0, NVERT));
		else
			GLCHK(glUnlockArraysEXT());
		break;
	default:
		;
	}
}


static
void joystick (unsigned int buttonMask, int x, int y, int z)
{
	float c[] = { x * 1.0f/2000.0f + 0.5f, y * 1.0f/2000.0f + 0.5f, 1.0f, 1.0f };

	GLCHK(glLightModelfv(GL_LIGHT_MODEL_AMBIENT, c));
	GLCHK(glClearColor(c[0], c[1], c[2], c[3]));
}

static void init_cmap(unsigned short *cmap)
{
	for(unsigned i = 0; i < 65536; i++) {
		unsigned char c = i >> (16-5);
		cmap[i] = (c << 10) | (c << 5) | c;
	}
}

int main(int argc, char* argv[])
{
	int i;

	glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(480, 272);
	glutJoystickFunc(joystick, 0);
	glutCreateWindow( __FILE__ );
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	psp_log("config: sys=%d vbo=%d vboidx=%d pbo=%d tristrips=%d cva=%d\n",
		SYS, USE_VBO, USE_VBO_IDX, USE_PBO, USE_TRISTRIPS, USE_CVA);

	GLCHK(glShadeModel(GL_SMOOTH));

	GLCHK(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE));
	GLCHK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	struct vertex *vtxmap;
	struct indices *idxmap;


	GLCHK(glGenBuffersARB(3, buffers));

	if (USE_VBO) {
		GLCHK(glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffers[0]));
		GLCHK(glBufferDataARB(GL_ARRAY_BUFFER_ARB, 2 * NVERT * sizeof(struct vertex),
				      NULL, GL_STATIC_DRAW_ARB));
		GLCHK(vtxmap = glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB));
		vertices = 0;
	} else
		vtxmap = vertices = malloc(2 * NVERT * sizeof(struct vertex));

	if (USE_VBO_IDX) {
		GLCHK(glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, buffers[1]));
		GLCHK(glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(struct indices),
				      NULL, GL_STATIC_DRAW_ARB));
		GLCHK(idxmap = glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB));
		indices = 0;
	} else
		idxmap = indices = malloc(sizeof(struct indices));

	if (USE_PBO) {
		unsigned short *cmap;

		GLCHK(glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, buffers[2]));
		GLCHK(glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, buffers[2]));
		GLCHK(glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB, DEPTHBYTES + CMAPBYTES, NULL,
				      GL_STREAM_COPY_ARB));
		depth = NULL;
		GLCHK(cmap = glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB));
		init_cmap(cmap + DEPTHBYTES);
		GLCHK(glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB));

		GLCHK(glColorTableEXT(GL_TEXTURE_2D, GL_RGB, 65536, GL_RGB,
				      GL_UNSIGNED_SHORT_1_5_5_5_REV, NULL + DEPTHBYTES));

	} else {
		unsigned short *cmap;

		depth = memalign(64, DEPTHBYTES + CMAPBYTES);
		memset(depth, 0, DEPTHSZ*DEPTHSZ*sizeof(depth_t));
		cmap = depth + DEPTHBYTES;
		init_cmap(cmap);

		GLCHK(glColorTableEXT(GL_TEXTURE_2D, GL_RGB, 65536, GL_RGB,
				      GL_UNSIGNED_SHORT_1_5_5_5_REV, cmap));
	}

	genSkinnedCylinder(CYL_SLICES, CYL_ROWS, CYL_LENGTH, CYL_RADIUS, BONES, 
			   vtxmap, idxmap->tris, idxmap->tristrips, 
			   idxmap->cap0, idxmap->cap1);

	for(i = 0; i < NVERT; i++) {
		vtxmap[NVERT+i].x = vtxmap[i].x + vtxmap[i].nx/4;
		vtxmap[NVERT+i].y = vtxmap[i].y + vtxmap[i].ny/4;
		vtxmap[NVERT+i].z = vtxmap[i].z + vtxmap[i].nz/4;
		idxmap->norms[i*2+0] = i;
		idxmap->norms[i*2+1] = i+NVERT;
	}

	if (USE_VBO)
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);

	if (USE_VBO_IDX)
		glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);

	GLCHK(glEnableClientState(GL_VERTEX_ARRAY));
	GLCHK(glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), &vertices[0].x));

	GLCHK(glEnableClientState(GL_NORMAL_ARRAY));
	GLCHK(glNormalPointer(GL_FLOAT, sizeof(struct vertex), &vertices[0].nx));

#if DO_VB
	GLCHK(glEnableClientState(GL_WEIGHT_ARRAY_PSP));
	GLCHK(glWeightPointerPSP(BONES, GL_FLOAT, sizeof(struct vertex), &vertices[0].w));

	GLCHK(glEnable(GL_VERTEX_BLEND_PSP));
#endif

#if SYS && DO_VB
	if (0) {
		int i;

		for(i = 0; i < NVERT; i++) {
			int j;
			printf("%3d: ", i);
			for(j = 0; j < BONES; j++)
				printf("%f, ", vertices[i].w[j]);
			printf("\n");
		}
	}
#endif

	{
		GLfloat diffcol0[] = { .4, .4, 1, 1 };
		GLfloat lightdir0[] = { 0, 0, 1, 0 };
		GLfloat diffcol1[] = { .6, .2, .1, 1 };
		GLfloat lightdir1[] = { .707, .707, 0, 0 };

		GLCHK(glEnable(GL_LIGHTING));

		GLCHK(glEnable(GL_LIGHT0));
		GLCHK(glLightfv(GL_LIGHT0, GL_DIFFUSE, diffcol0));
		GLCHK(glLightfv(GL_LIGHT0, GL_POSITION, lightdir0));

		GLCHK(glEnable(GL_LIGHT1));
		GLCHK(glLightfv(GL_LIGHT1, GL_DIFFUSE, diffcol1));
		GLCHK(glLightfv(GL_LIGHT1, GL_POSITION, lightdir1));
	}

	if (USE_CVA)
		GLCHK(glLockArraysEXT(0, NVERT));

	GLCHK(glEnable(GL_DEPTH_TEST));
	GLCHK(glClearColor(0,0,.5,.1));

	GLCHK(glFrontFace(GL_CW));
	//GLCHK(glCullFace(GL_FRONT));
	GLCHK(glEnable(GL_CULL_FACE));
	//glDepthMask(0);

#if GL_PSP_statistics
	GLCHK(glEnableStatsPSP(GL_STATS_TIMING_PSP));
#endif

//	GLCHK(glPolygonOffset(1,0));
//	GLCHK(glEnable(GL_POLYGON_OFFSET_FILL));

	glutMainLoop();

	return 0;
}
