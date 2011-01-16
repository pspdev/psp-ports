#include <stdlib.h>
#include <GL/glut.h>

#include "glchk.h"

#include "textab.h"
#include "perfmeter.h"

#define NDXT	(sizeof(dxt)/sizeof(*dxt))


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
static unsigned incdxt = 1;
static unsigned dxtidx = 0;
static unsigned phase;

#define NTEX	5
static GLuint texid[NTEX];

extern unsigned char firefox_mono_start[];
extern unsigned char nosmoking_start[], nosmoking_end[];

static
void display (void)
{
	int i;
	static GLfloat angle;
	unsigned char cmap[256 * 4];

	angle += delta;

	pm_framestart();

	GLCHK(glShadeModel(GL_SMOOTH));

	GLCHK(glClear(GL_COLOR_BUFFER_BIT));

	GLCHK(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));
	GLCHK(glEnable(GL_BLEND));
	GLCHK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


	for(i = 0; i < 256; i++) {
		unsigned p = i + phase;
		cmap[i*4+0] = p;
		cmap[i*4+1] = p+(256*1)/3;
		cmap[i*4+2] = p+(256*2)/3;
		cmap[i*4+3] = 255;
	}
	phase++;

	GLCHK(glBindTexture(GL_TEXTURE_2D, texid[4]));

	GLCHK(glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, 128, 128, 0, 
				     dxt[dxtidx].end-dxt[dxtidx].start, dxt[dxtidx].start));
	dxtidx = (dxtidx+incdxt)%NDXT;

	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));


	for(i = 0; i < NTEX; i++) {
		int x = i % 4;
		int y = i / 4;

		GLCHK(glMatrixMode(GL_MODELVIEW));
		GLCHK(glLoadIdentity());
		GLCHK(glTranslatef(1. * (x - 2), 1. * (y - .5), 0));
		GLCHK(glScalef(2,2,1));
		GLCHK(glRotatef(angle * 1.32f, 0.0f, 0.0f, 1.0f));
		GLCHK(glTranslatef(-.5, -.5, 0));

		GLCHK(glBindTexture(GL_TEXTURE_2D, texid[i]));

		if (i == 0)
			GLCHK(glColorTable(GL_TEXTURE_2D, GL_RGBA, 256, GL_RGBA, GL_UNSIGNED_BYTE, cmap));

		glBegin(GL_TRIANGLE_FAN);
		  glColor3f(1, 0, 0);
		  glTexCoord2f(0, 0);
		  glVertex3f(0, 0, 0);

		  glColor3f(0, 1 ,0);
		  glTexCoord2f(0, 1);
		  glVertex3f(0, 1, 0);

		  glColor3f(0, 0, 1);
		  glTexCoord2f(1, 1);
		  glVertex3f(1, 1, 0);

		  glColor3f(1, 1, 1);
		  glTexCoord2f(1, 0);
		  glVertex3f(1, 0, 0);
		glEnd();
	}
#if SYS
	usleep(100000);
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
		incdxt = 0;
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
		incdxt=1;
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

/*
   F E D C B A 9 8|7 6 5 4 3 2 1 0
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |R R R R R|G G G G G G|B B B B B| 565
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |R R R R R|G G G G G|B B B B B|A| 5551
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |R R R R|G G G G|B B B B|A A A A| 4444
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define RGB565(r,g,b)		((((r) << 8) & 0xf800) | (((g) << 3) & 0x07E0) | (((b) >> 3) & 0x001f))
#define RGBA5551(r,g,b,a)	((((r) << 8) & 0xf800) | (((g) << 3) & 0x07C0) | (((b) >> 2) & 0x003e) | (((a) >> 7) & 0x0001))
#define RGBA4444(r,g,b,a)	((((r) << 8) & 0xf000) | (((g) << 4) & 0x0f00) | (((b)     ) & 0x00f0) | (((a) >> 4) & 0x000f))

static inline unsigned short swizzle_565(unsigned short in)
{
	unsigned short r = (in & 0xf800) >> 11;
	unsigned short g = (in & 0x07e0);
	unsigned short b = (in & 0x001f) << 11;

	return b | g | r;
}

int main(int argc, char* argv[])
{
	static unsigned char dxt1[8192];
	int i, j;

	glutInit(&argc, argv);
#if SYS
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
        glutInitWindowSize(480, 272);
#endif
	glutCreateWindow( __FILE__ );
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutJoystickFunc(joystick, 0);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	GLCHK(glGenTextures(NTEX, texid));

	GLCHK(glBindTexture(GL_TEXTURE_2D, texid[0]));

	GLCHK(glTexImage2D(GL_TEXTURE_2D, 0, GL_COLOR_INDEX8_EXT, 64, 64, 0, GL_COLOR_INDEX8_EXT, GL_UNSIGNED_BYTE, firefox_mono_start));

	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	GLCHK(glBindTexture(GL_TEXTURE_2D, texid[1]));

	GLCHK(glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, 128, 128, 0, 
				     nosmoking_end-nosmoking_start, nosmoking_start));

	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	GLCHK(glBindTexture(GL_TEXTURE_2D, texid[2]));

	GLCHK(glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 128, 128, 0, 
				     nosmoking_end-nosmoking_start, nosmoking_start));

	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	for(j = 0, i = 8; i < 16384; i += 16) {
		int k;
		unsigned short *sp;

		for(k = 0; k < 8; k++)
			dxt1[j+k] = nosmoking_start[i+k];
		sp = (unsigned short *)&dxt1[j];
		//sp[0] = swizzle_565(sp[0]);
		//sp[1] = swizzle_565(sp[1]);
		j += 8;
	}
	GLCHK(glBindTexture(GL_TEXTURE_2D, texid[3]));

	GLCHK(glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, 128, 128, 0, 
				     8192, dxt1));

	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));


	GLCHK(glEnable(GL_TEXTURE_2D));

	glutMainLoop();
	return 0;
}
