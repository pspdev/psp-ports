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

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>

#include "../quake3-1.32b/code/game/q_shared.h"
#include "../quake3-1.32b/code/qcommon/qcommon.h"
#include "../quake3-1.32b/code/ui/keycodes.h"

//===================================================================

void Sys_BeginProfiling (void) { }

void Sys_BeginStreamedFile (fileHandle_t f, int readahead)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
}


void Sys_EndStreamedFile (fileHandle_t f)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
}


int Sys_StreamedRead (void *buffer, int size, int count, fileHandle_t f)
{
	return FS_Read(buffer, size * count, f);
}

void Sys_StreamSeek (fileHandle_t f, int offset, int origin)
{
	FS_Seek(f, offset, origin);
}


//===================================================================
#define      MAX_FOUND_FILES 0x1000

void Sys_ListFilteredFiles (const char *basedir, char *subdirs, char *filter, char **list, int *numfiles)
{
	char	    search[MAX_OSPATH], newsubdirs[MAX_OSPATH];
	char	    filename[MAX_OSPATH];
	DIR		     *fdir;
	struct dirent *d;
	struct stat st;

	if ( *numfiles >= MAX_FOUND_FILES - 1 ) {
		return;
	}

	if (strlen(subdirs)) {
		Com_sprintf( search, sizeof(search), "%s/%s", basedir, subdirs );
	}
	else {
		Com_sprintf( search, sizeof(search), "%s", basedir );
	}

	if ((fdir = opendir(search)) == NULL) {
		return;
	}

	while ((d = readdir(fdir)) != NULL) {
		Com_sprintf(filename, sizeof(filename), "%s/%s", search, d->d_name);
		if (stat(filename, &st) == -1)
			continue;

		if (st.st_mode & S_IFDIR) {
			if (Q_stricmp(d->d_name, ".") && Q_stricmp(d->d_name, "..")) {
				if (strlen(subdirs)) {
					Com_sprintf( newsubdirs, sizeof(newsubdirs), "%s/%s", subdirs, d->d_name);
				}
				else {
					Com_sprintf( newsubdirs, sizeof(newsubdirs), "%s", d->d_name);
				}
				Sys_ListFilteredFiles( basedir, newsubdirs, filter, list, numfiles );
			}
		}
		if ( *numfiles >= MAX_FOUND_FILES - 1 ) {
			break;
		}
		Com_sprintf( filename, sizeof(filename), "%s/%s", subdirs, d->d_name );
		if (!Com_FilterPath( filter, filename, qfalse ))
			continue;
		list[ *numfiles ] = CopyString( filename );
		(*numfiles)++;
	}

	closedir(fdir);
}


char **Sys_ListFiles (const char *directory, const char *extension, char *filter, int *numfiles, qboolean wantsubs)
{
	struct dirent *d;
	DIR *fdir;
	qboolean dironly = wantsubs;
	char search[MAX_OSPATH];
	int nfiles;
	char **listCopy;
	char *list[MAX_FOUND_FILES];
	int i;
	struct stat st;
	int extLen;

	if (filter) {
		nfiles = 0;
		Sys_ListFilteredFiles(directory, "", filter, list, &nfiles);
		list[nfiles] = 0;
		*numfiles = nfiles;

		if (!nfiles)
			return NULL;

		listCopy = Z_Malloc((nfiles + 1) * sizeof(*listCopy));

		for (i=0; i<nfiles; i++)
			listCopy[i] = list[i];

		listCopy[i] = NULL;
		return listCopy;
	}

	if (!extension)
		extension = "";

	if (extension[0] == '/' && extension[1] == 0) {
		extension = "";
		dironly = qtrue;
	}

	extLen = strlen(extension);

	// search
	nfiles = 0;

	if ((fdir = opendir(directory)) == NULL) {
		*numfiles = 0;
		return NULL;
	}

	while ((d = readdir(fdir)) != NULL) {
		Com_sprintf(search, sizeof(search), "%s/%s", directory, d->d_name);
		if (stat(search, &st) == -1)
			continue;
		if ((dironly && !(st.st_mode & S_IFDIR)) ||
			(!dironly && (st.st_mode & S_IFDIR)))
			continue;

		if (*extension) {
			if ( strlen( d->d_name ) < strlen( extension ) ||
				Q_stricmp( 
					d->d_name + strlen( d->d_name ) - strlen( extension ),
					extension ) ) {
				continue; // didn't match
			}
		}

		if ( nfiles == MAX_FOUND_FILES - 1 )
			break;
		list[nfiles] = CopyString(d->d_name);
		nfiles++;
	}

	list[nfiles] = 0;

	closedir(fdir);

	// return a copy of the list
	*numfiles = nfiles;

	if (!nfiles)
		return NULL;

	listCopy = Z_Malloc( ( nfiles + 1 ) * sizeof( *listCopy ) );

	for (i=0; i<nfiles; i++)
		listCopy[i] = list[i];

	listCopy[i] = NULL;

	return listCopy;
}


void Sys_FreeFileList (char **list)
{
	int i;

	if (!list)
		return;

	for (i=0; list[i]; i++)
		Z_Free(list[i]);

	Z_Free(list);
}


char *Sys_Cwd (void) 
{
	static char cwd [MAX_OSPATH];

	getcwd(cwd, sizeof(cwd) - 1);
	cwd[MAX_OSPATH-1] = 0;

	return cwd;
}


char *Sys_DefaultInstallPath(void)
{
	return Sys_Cwd();
}


char *Sys_DefaultHomePath(void)
{
	return NULL;
}


char *Sys_DefaultCDPath (void)
{
	return "";
}


qboolean Sys_CheckCD (void)
{
	return qtrue;
}


void Sys_Mkdir (const char *path)
{
	/* not needed if filesystem is prepared properly? */
	printf("%s (%d): path '%s'\n", __FUNCTION__, __LINE__, path);
}


sysEvent_t Sys_GetEvent (void)
{
	/* create an empty event to return. Do we need this? */
	sysEvent_t ev = { 0 };
	ev.evTime = Sys_Milliseconds();;
	return ev;
}


void Sys_Error (const char *error, ...)
{
	va_list argptr;

	printf("Sys_Error: ");	
	va_start(argptr,error);
	vprintf(error,argptr);
	va_end(argptr);
	printf("\n");

	exit(1);
}

void Sys_Quit (void)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
	exit(0);
}


void Sys_UnloadGame (void)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
}


void *Sys_GetGameAPI (void *parms)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
	return NULL;
}

char *Sys_GetClipboardData (void)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
	return NULL;
}

int Sys_Milliseconds (void)
{
	static long sys_timeBase = 0;
	struct timeval tp;

	gettimeofday(&tp, NULL);

	if (!sys_timeBase) {
		sys_timeBase = tp.tv_sec;
		return tp.tv_usec / 1000;
	}

	return (tp.tv_sec - sys_timeBase) * 1000 + tp.tv_usec / 1000;
}


char *Sys_FindFirst (char *path, unsigned musthave, unsigned canthave)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
	return NULL;
}

char *Sys_FindNext (unsigned musthave, unsigned canthave)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
	return NULL;
}

void Sys_FindClose (void)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
}


void Sys_Init (void)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
}


void *Sys_LoadDll (const char *name, char *fqpath ,
		   int (**entryPoint)(int, ...),
		   int (*systemcalls)(int, ...)) 
{
	return NULL;
}

void Sys_UnloadDll (void *dllHandle)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
}


void Sys_EarlyOutput (char *string)
{
	printf("%s", string);
}


void  Sys_Print (const char *msg)
{
	printf("%s", msg);
}


void Sys_ShowConsole (int visLevel, qboolean quitOnClose)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
}


qboolean Sys_LowPhysicalMemory (void)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
	return qtrue;
}


void Sys_SnapVector (float *v)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
}


int VM_CallCompiled (vm_t *vm, int *args)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
	return 0;
}


void VM_Compile (vm_t *vm, vmHeader_t *header)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
}

#include <glut.h>

static
void display (void)
{
	static int initialized = 0;

	if (!initialized) {
		char *cmdline = strdup(""); /*strdup("+demo four " "+set cg_drawFPS 1");*/
		Com_Init(cmdline);
		free(cmdline);
		initialized = 1;
	}

	Com_Frame();
	glutPostRedisplay();
}


static const
unsigned char keytab [] = {
	K_LEFTARROW, K_UPARROW, K_RIGHTARROW, K_DOWNARROW,
	K_PGUP, K_PGDN, K_HOME, K_END, K_INS
};


static
void special_key (int key, int down)
{
	switch (key) {
	case 13:
		CL_KeyEvent(K_ENTER, down, Sys_Milliseconds());
		break;
	case ' ':
		CL_KeyEvent(K_SPACE, down, Sys_Milliseconds());
		break;
	case 27:
		CL_KeyEvent(K_ESCAPE, down, Sys_Milliseconds());
		break;
	case 127:
		CL_KeyEvent(K_BACKSPACE, down, Sys_Milliseconds());
		break;
	default:
		if (key >= GLUT_KEY_LEFT && key <= GLUT_KEY_LEFT + sizeof(keytab)/sizeof(*keytab))
			CL_KeyEvent(keytab[key - GLUT_KEY_LEFT], down, Sys_Milliseconds());
	}
}


static
void special_keydown (int key, int x, int y)
{
	special_key(key, qtrue);
}


static
void special_keyup (int key, int x, int y)
{
	special_key(key, qfalse);
}


static
void key (unsigned char key, int down)
{
	switch (key) {
	case 13:
		CL_KeyEvent(K_ENTER, down, Sys_Milliseconds());
		break;
	case ' ':
		CL_KeyEvent(K_SPACE, down, Sys_Milliseconds());
		break;
	case 27:
		CL_KeyEvent(K_ESCAPE, down, Sys_Milliseconds());
		break;
	case 127:
		CL_KeyEvent(K_BACKSPACE, down, Sys_Milliseconds());
		break;
	default:
		CL_CharEvent(key);
	}
}


static
void keydown (unsigned char keycode, int x, int y)
{
	key(keycode, qtrue);
}


static
void keyup (unsigned char keycode, int x, int y)
{
	key(keycode, qfalse);
}


static int mouse_x, mouse_y;

static
void mouse_motion (int x, int y)
{
	CL_MouseEvent(2 * (x - mouse_x), 2 * (y - mouse_y), Sys_Milliseconds());
	mouse_x = x;
	mouse_y = y;
}

static
void mouse_button (int button, int state, int x, int y)
{
	int keycode = K_MOUSE1 + button - GLUT_LEFT_BUTTON;
	int press = (state == GLUT_DOWN) ? qtrue : qfalse;
	CL_KeyEvent(keycode, press, Sys_Milliseconds());
	mouse_x = x;
	mouse_y = y;
}

static
void joystick (unsigned int buttons, int x, int y, int z)
{
	static unsigned int prev_buttons;
	unsigned int changed = buttons ^ prev_buttons;
	int time = Sys_Milliseconds();
	int i;

	CL_JoystickEvent(0, x, time);
	CL_JoystickEvent(1, y, time);

	for (i=0; i<32; i++) {
		if (changed & (1 << i)) {
			int press = (buttons & (1 << i)) ? qtrue : qfalse;
			CL_KeyEvent(K_JOY1 + i, press, time);
		}
	}

	prev_buttons = buttons;
}


int main (int argc, char **argv)
{
	printf("%s (%d):\n", __FUNCTION__, __LINE__);
	glutInit(&argc, argv);
	glutInitWindowSize(480, 272);
//	glutInitWindowSize(640, 480);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutCreateWindow("Quake3");
	glutSpecialFunc(special_keydown);
	glutSpecialUpFunc(special_keyup);
	glutKeyboardFunc(keydown);
	glutKeyboardUpFunc(keyup);
	glutJoystickFunc(joystick, 0);
	glutMouseFunc(mouse_button);
	glutMotionFunc(mouse_motion);
	glutPassiveMotionFunc(mouse_motion);
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}

