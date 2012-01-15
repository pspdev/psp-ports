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

#ifndef _SDL_pspgl_c_h
#define _SDL_pspgl_c_h

#ifdef HAVE_OPENGL
#include <GL/gl.h>
#include <GLES/egl.h>
#endif

#include "SDL_pspvideo.h"

struct SDL_PrivateGLData {
#ifdef HAVE_OPENGL
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
#endif
};

#ifdef HAVE_OPENGL
extern int PSP_GL_Init(_THIS);

extern void * PSP_GL_GetProcAddress(_THIS, const char *proc);
extern int PSP_GL_GetAttribute(_THIS, SDL_GLattr attrib, int *value);
extern int PSP_GL_MakeCurrent(_THIS);
extern void PSP_GL_SwapBuffers(_THIS);
#endif

#endif /* _SDL_pspgl_c_h */
