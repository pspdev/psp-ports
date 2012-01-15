/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#include <stdlib.h>
#include <string.h>

#include "SDL_error.h"
#include "SDL_pspvideo.h"
#include "SDL_pspgl_c.h"

#ifdef HAVE_OPENGL

/* pspgl doesn't provide this call, so stub it out since SDL requires it. */
#define GLSTUB(func,params) void func params {}
GLSTUB(glCopyTexImage1D, (GLenum target, GLint level, GLenum internalFormat,
			GLint x, GLint y, GLsizei width, GLint border))

static const struct {
	const char *proc;
	void *func;
} glfuncs[] = {
#define SDL_PROC(ret,func,params) {#func,&func},
#include "../SDL_glfuncs.h"
#undef SDL_PROC
};
#define GLFUNCS_COUNT (sizeof(glfuncs) / sizeof(glfuncs[0]))

void * PSP_GL_GetProcAddress(_THIS, const char *proc)
{
	unsigned int i;

	for (i = 0; i < GLFUNCS_COUNT; i++)
	{
		if (strcmp(proc, glfuncs[i].proc) == 0)
			return glfuncs[i].func;
	}

	return NULL;
}

int PSP_GL_GetAttribute(_THIS, SDL_GLattr attrib, int *value)
{
	return 0;
}

int PSP_GL_MakeCurrent(_THIS)
{
	struct SDL_PrivateGLData *gl_data = this->gl_data;

	if (!eglMakeCurrent(gl_data->display, gl_data->surface, gl_data->surface, gl_data->context))
	{
		SDL_SetError("Unable to make EGL context current");
		return -1;
	}

	return 0;
}

void PSP_GL_SwapBuffers(_THIS)
{
	struct SDL_PrivateGLData *gl_data = this->gl_data;

	eglSwapBuffers(gl_data->display, gl_data->surface);
}

#define EGLCHK(stmt)							\
	do {										\
		EGLint err;								\
												\
		stmt;									\
		err = eglGetError();					\
		if (err != EGL_SUCCESS) {				\
			SDL_SetError("EGL error %d", err);	\
			return 0;							\
		}										\
	} while (0)

/* Return 1 if we were able to initialize PSPGL successfully. */
int PSP_GL_Init(_THIS)
{
	EGLint attribs[32];
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	EGLint num_configs;
	int i;

	/* EGL init taken from glutCreateWindow() in PSPGL's glut.c. */
	EGLCHK(display = eglGetDisplay(0));
	EGLCHK(eglInitialize(display, NULL, NULL));

	/* Setup the config based on SDL's current values. */
	i = 0;
	attribs[i++] = EGL_RED_SIZE;
	attribs[i++] = this->gl_config.red_size;
	attribs[i++] = EGL_GREEN_SIZE;
	attribs[i++] = this->gl_config.green_size;
	attribs[i++] = EGL_BLUE_SIZE;
	attribs[i++] = this->gl_config.blue_size;
	attribs[i++] = EGL_DEPTH_SIZE;
	attribs[i++] = this->gl_config.depth_size;

	if (this->gl_config.alpha_size)
	{
		attribs[i++] = EGL_ALPHA_SIZE;
		attribs[i++] = this->gl_config.alpha_size;
	}
	if (this->gl_config.stencil_size)
	{
		attribs[i++] = EGL_STENCIL_SIZE;
		attribs[i++] = this->gl_config.stencil_size;
	}

	attribs[i++] = EGL_NONE;
	EGLCHK(eglChooseConfig(display, attribs, &config, 1, &num_configs));

	if (num_configs == 0)
	{
		SDL_SetError("No valid EGL configs for requested mode");
		return 0;
	}

	EGLCHK(context = eglCreateContext(display, config, NULL, NULL));
	EGLCHK(surface = eglCreateWindowSurface(display, config, 0, NULL));

	this->gl_data->display = display;
	this->gl_data->context = context;
	this->gl_data->surface = surface;
	return 1;
}
#endif /* HAVE_OPENGL */
