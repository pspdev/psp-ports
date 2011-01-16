#include <pspctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <GLES/egl.h>
#include <GLES/gl.h>

#include "glchk.h"

extern unsigned char logo_start[];


struct Vertex
{
	float u, v;
	unsigned int color;
	float x,y,z;
};


static const
struct Vertex __attribute__((aligned(16))) vertices [12*3] =
{
	{ 0, 0, 0xff7f0000,-1,-1, 1 }, // 0
	{ 1, 0, 0xff7f0000,-1, 1, 1 }, // 4
	{ 1, 1, 0xff7f0000, 1, 1, 1 }, // 5

	{ 0, 0, 0xff7f0000,-1,-1, 1 }, // 0
	{ 1, 1, 0xff7f0000, 1, 1, 1 }, // 5
	{ 0, 1, 0xff7f0000, 1,-1, 1 }, // 1

	{ 0, 0, 0xff7f0000,-1,-1,-1 }, // 3
	{ 1, 0, 0xff7f0000, 1,-1,-1 }, // 2
	{ 1, 1, 0xff7f0000, 1, 1,-1 }, // 6

	{ 0, 0, 0xff7f0000,-1,-1,-1 }, // 3
	{ 1, 1, 0xff7f0000, 1, 1,-1 }, // 6
	{ 0, 1, 0xff7f0000,-1, 1,-1 }, // 7

	{ 0, 0, 0xff007f00, 1,-1,-1 }, // 0
	{ 1, 0, 0xff007f00, 1,-1, 1 }, // 3
	{ 1, 1, 0xff007f00, 1, 1, 1 }, // 7

	{ 0, 0, 0xff007f00, 1,-1,-1 }, // 0
	{ 1, 1, 0xff007f00, 1, 1, 1 }, // 7
	{ 0, 1, 0xff007f00, 1, 1,-1 }, // 4

	{ 0, 0, 0xff007f00,-1,-1,-1 }, // 0
	{ 1, 0, 0xff007f00,-1, 1,-1 }, // 3
	{ 1, 1, 0xff007f00,-1, 1, 1 }, // 7

	{ 0, 0, 0xff007f00,-1,-1,-1 }, // 0
	{ 1, 1, 0xff007f00,-1, 1, 1 }, // 7
	{ 0, 1, 0xff007f00,-1,-1, 1 }, // 4

	{ 0, 0, 0xff00007f,-1, 1,-1 }, // 0
	{ 1, 0, 0xff00007f, 1, 1,-1 }, // 1
	{ 1, 1, 0xff00007f, 1, 1, 1 }, // 2

	{ 0, 0, 0xff00007f,-1, 1,-1 }, // 0
	{ 1, 1, 0xff00007f, 1, 1, 1 }, // 2
	{ 0, 1, 0xff00007f,-1, 1, 1 }, // 3

	{ 0, 0, 0xff00007f,-1,-1,-1 }, // 4
	{ 1, 0, 0xff00007f,-1,-1, 1 }, // 7
	{ 1, 1, 0xff00007f, 1,-1, 1 }, // 6

	{ 0, 0, 0xff00007f,-1,-1,-1 }, // 4
	{ 1, 1, 0xff00007f, 1,-1, 1 }, // 6
	{ 0, 1, 0xff00007f, 1,-1,-1 }, // 5
};


int done = 0;

static const EGLint attrib_list [] = {
	EGL_RED_SIZE, 1,
	EGL_GREEN_SIZE, 1,
	EGL_BLUE_SIZE, 1,
	EGL_ALPHA_SIZE, 0,
	EGL_DEPTH_SIZE, 0,
	EGL_NONE
};


int main(int argc, char* argv[])
{
	EGLDisplay dpy;
	EGLConfig config;
	EGLint num_configs;
	EGLContext ctx;
	EGLSurface surface;
	GLfloat angle = 0.0f;
	SceCtrlData pad;

	psp_log("startup\n");

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	/* pass NativeDisplay=0, we only have one screen... */
	EGLCHK(dpy = eglGetDisplay(0));
	EGLCHK(eglInitialize(dpy, NULL, NULL));

	psp_log("EGL vendor \"%s\"\n", eglQueryString(dpy, EGL_VENDOR));
	psp_log("EGL version \"%s\"\n", eglQueryString(dpy, EGL_VERSION));
	psp_log("EGL extensions \"%s\"\n", eglQueryString(dpy, EGL_EXTENSIONS));

	EGLCHK(eglChooseConfig(dpy, attrib_list, &config, 1, &num_configs));

	if (num_configs == 0) {
		__pspgl_log("eglChooseConfig returned 0 configurations");
		return 0;
	}

	psp_log("eglChooseConfige() returned config 0x%04x\n", (unsigned int) config);

	EGLCHK(ctx = eglCreateContext(dpy, config, NULL, NULL));
	EGLCHK(surface = eglCreateWindowSurface(dpy, config, 0, NULL));
	EGLCHK(eglMakeCurrent(dpy, surface, surface, ctx));

	GLCHK(glDisable(GL_STENCIL_TEST));
	GLCHK(glDisable(GL_ALPHA_TEST));
	GLCHK(glDisable(GL_SCISSOR_TEST));

	GLCHK(glDepthFunc(GL_LEQUAL));
	GLCHK(glEnable(GL_DEPTH_TEST));

	GLCHK(glFrontFace(GL_CW));
	GLCHK(glShadeModel(GL_SMOOTH));
	GLCHK(glEnable(GL_CULL_FACE));

	GLCHK(glEnable(GL_TEXTURE_2D));

	GLCHK(glViewport(0, 0, 480, 272));
	GLCHK(glScissor(0, 0, 480, 272));
	GLCHK(glDepthRangef(0.0f, 1.0f));

	while (!done) {
		sceCtrlReadBufferPositive(&pad, 1); 

		if (pad.Buttons & PSP_CTRL_CROSS)
			done = 1;

		if (!(pad.Buttons & PSP_CTRL_CIRCLE))
			angle += 1.0f;

		GLCHK(glClearColor(pad.Lx * 1.0f/255, pad.Ly * 1.0f/255, 1.0f, 1.0f));
		GLCHK(glClearDepthf(1.0f));
		GLCHK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		GLCHK(glMatrixMode(GL_PROJECTION));
		GLCHK(glLoadIdentity());
		GLCHK(gluPerspectivef(75.0f, 16.0f/9.0f, 0.5f, 1000.0f));

		GLCHK(glMatrixMode(GL_MODELVIEW));
		GLCHK(glLoadIdentity());
		GLCHK(gluLookAtf(0.0f, 0.0f, 2.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
		GLCHK(glRotatef(angle * 0.79f, 1.0f, 0.0f, 0.0f));
		GLCHK(glRotatef(angle * 0.98f, 0.0f, 1.0f, 0.0f));
		GLCHK(glRotatef(angle * 1.32f, 0.0f, 0.0f, 1.0f));

		/* setup texture */
		GLCHK(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE));
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCHK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, logo_start));

		/* draw cube */
		GLCHK(glInterleavedArrays(GL_T2F_C4UB_V3F, 0, (void *) vertices));
		GLCHK(glDrawArrays(GL_TRIANGLES, 0, 12*3));

		EGLCHK(eglSwapBuffers(dpy, surface));
	}

	EGLCHK(eglTerminate(dpy));
	return 0;
}

