/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
#include "../quake3-1.32b/code/renderer/tr_local.h"

/*
qboolean ( * qwglSwapIntervalEXT)( int interval );
void ( * qglMultiTexCoord2fARB )( GLenum texture, float s, float t );
void ( * qglActiveTextureARB )( GLenum texture );
void ( * qglClientActiveTextureARB )( GLenum texture );


void ( * qglLockArraysEXT)( int, int);
void ( * qglUnlockArraysEXT) ( void );
*/

#include <glut.h>

void GLimp_EndFrame (void)
{
	glutSwapBuffers();
	glutPostRedisplay();
}


void GLimp_Init (void)
{
	GLint val [4];

	glConfig.driverType = GLDRV_ICD;
	glConfig.hardwareType = GLHW_GENERIC;
	glConfig.isFullscreen = qtrue;

	qglGetIntegerv(GL_VIEWPORT, val);
	glConfig.vidWidth = val[2];
	glConfig.vidHeight = val[3];
	glConfig.windowAspect = (GLfloat) glConfig.vidWidth / (GLfloat) glConfig.vidHeight;

	qglGetIntegerv(GL_MAX_ACTIVE_TEXTURES_ARB, &val[0]);
	qglGetIntegerv(GL_DEPTH_BITS, &val[1]);
	qglGetIntegerv(GL_STENCIL_BITS, &val[2]);

	glConfig.maxActiveTextures = val[0];
	glConfig.depthBits = val[1];
	glConfig.stencilBits = val[2];

	qglGetIntegerv(GL_RED_BITS, &val[0]);
	qglGetIntegerv(GL_BLUE_BITS, &val[1]);
	qglGetIntegerv(GL_GREEN_BITS, &val[2]);
	qglGetIntegerv(GL_ALPHA_BITS, &val[3]);
	glConfig.colorBits = val[0] + val[1] + val[2] + val[3];

	Q_strncpyz(glConfig.vendor_string, (const char *) qglGetString (GL_VENDOR), sizeof(glConfig.vendor_string));
	Q_strncpyz(glConfig.version_string, (const char *) qglGetString (GL_VERSION), sizeof(glConfig.version_string));
	Q_strncpyz(glConfig.extensions_string, (const char *) qglGetString (GL_EXTENSIONS), sizeof(glConfig.extensions_string));
	Q_strncpyz(glConfig.renderer_string, (const char *) qglGetString (GL_RENDERER), sizeof( glConfig.renderer_string));

	if (*glConfig.renderer_string && glConfig.renderer_string[strlen(glConfig.renderer_string) - 1] == '\n')
		glConfig.renderer_string[strlen(glConfig.renderer_string) - 1] = 0;

	ri.Cvar_Set("r_lastValidRenderer", glConfig.renderer_string);

	printf("%s (%d):\n", __FUNCTION__, __LINE__);
}


void GLimp_Shutdown (void) { printf("%s (%d):\n", __FUNCTION__, __LINE__); }

void GLimp_EnableLogging (qboolean enable) { printf("%s (%d):\n", __FUNCTION__, __LINE__); }

void GLimp_LogComment (char *comment) { /* printf("%s", comment); */ }

void *GLimp_RendererSleep (void) { printf("%s (%d):\n", __FUNCTION__, __LINE__); return NULL; }

void GLimp_FrontEndSleep (void) { printf("%s (%d):\n", __FUNCTION__, __LINE__); }

qboolean GLimp_SpawnRenderThread (void (*function)(void)) { return qfalse; }

void GLimp_WakeRenderer( void *data ) { printf("%s (%d):\n", __FUNCTION__, __LINE__); }

void GLimp_SetGamma (unsigned char red[256], unsigned char green[256], unsigned char blue[256]) { }

qboolean QGL_Init (const char *dllname) { printf("%s (%d):\n", __FUNCTION__, __LINE__); return qtrue; }

void QGL_Shutdown (void) { printf("%s (%d):\n", __FUNCTION__, __LINE__); }

