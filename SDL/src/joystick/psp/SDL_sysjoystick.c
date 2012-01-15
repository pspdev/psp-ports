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
    slouken@devolution.com
*/

/* PSP port contributed by Marcus R. Brown <mrbrown@ocgnet.org>. */
/* Joystick stuff by Matthew H <matthewh@webone.com.au>. */

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_sysjoystick.c,v 1.3 2005/02/12 18:01:30 slouken Exp $";
#endif

/* This is the system specific header for the SDL joystick API */
#include <pspctrl.h>
#include <pspkernel.h>

#include <stdio.h>		/* For the definition of NULL */
#include <stdlib.h>

#include "SDL_error.h"
#include "SDL_joystick.h"
#include "../SDL_sysjoystick.h"
#include "../SDL_joystick_c.h"
#include "SDL_thread.h"
#include "SDL_mutex.h"
#include "SDL_timer.h"
#include "SDL_events.h"

/* Current pad state */
static SceCtrlData pad = { .Lx = 0, .Ly = 0, .Buttons = 0 };
static SDL_sem *pad_sem = NULL;
static SDL_Thread *thread = NULL;
static int running = 0;
static const enum PspCtrlButtons button_map[] = {
	PSP_CTRL_TRIANGLE, PSP_CTRL_CIRCLE, PSP_CTRL_CROSS, PSP_CTRL_SQUARE,
	PSP_CTRL_LTRIGGER, PSP_CTRL_RTRIGGER,
	PSP_CTRL_DOWN, PSP_CTRL_LEFT, PSP_CTRL_UP, PSP_CTRL_RIGHT,
	PSP_CTRL_SELECT, PSP_CTRL_START, PSP_CTRL_HOME, PSP_CTRL_HOLD };
static int analog_map[256];  /* Map analog inputs to -32768 -> 32767 */

typedef struct
{
  int x;
  int y;
} point;

// 4 points define the bezier-curve.  
static point a = { 0, 0 };
static point b = { 50, 0  };
static point c = { 78, 32767 };
static point d = { 128, 32767 };

// simple linear interpolation between two points
static __inline__ void lerp (point *dest, point *a, point *b, float t)
{
	dest->x = a->x + (b->x - a->x)*t;
	dest->y = a->y + (b->y - a->y)*t;
}

// evaluate a point on a bezier-curve. t goes from 0 to 1.0
static int calc_bezier_y(float t)
{
	point ab, bc, cd, abbc, bccd, dest;
	lerp (&ab, &a, &b, t);           // point between a and b
	lerp (&bc, &b, &c, t);           // point between b and c
	lerp (&cd, &c, &d, t);           // point between c and d
	lerp (&abbc, &ab, &bc, t);       // point between ab and bc
	lerp (&bccd, &bc, &cd, t);       // point between bc and cd
	lerp (&dest, &abbc, &bccd, t);   // point on the bezier-curve
	return dest.y;
}

/*
 * Collect pad data about once per frame
 */
int JoystickUpdate(void *data)
{
	while (running) {
		SDL_SemWait(pad_sem);
		sceCtrlPeekBufferPositive(&pad, 1); 
		SDL_SemPost(pad_sem);
		/* Delay 1/60th of a second */
		sceKernelDelayThread(1000000 / 60);  
	}
	return 0;
}



/* Function to scan the system for joysticks.
 * This function should set SDL_numjoysticks to the number of available
 * joysticks.  Joystick 0 should be the system default joystick.
 * It should return number of joysticks, or -1 on an unrecoverable fatal error.
 */
int SDL_SYS_JoystickInit(void)
{
	int i;

	SDL_numjoysticks = 1;

	/* Setup input */
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	/* Start thread to read data */
	if((pad_sem =  SDL_CreateSemaphore(1)) == NULL) {
		SDL_SetError("Can't create input semaphore\n");
		return -1;
	}
	running = 1;
	if((thread = SDL_CreateThread(JoystickUpdate, NULL)) == NULL) {
		SDL_SetError("Can't create input thread\n");
		return -1;
	}

	/* Create an accurate map from analog inputs (0 to 255) 
	   to SDL joystick positions (-32768 to 32767) */
	for (i = 0; i < 128; i++)
	{
		float t = (float)i/127.0f;
		analog_map[i+128] = calc_bezier_y(t);
		analog_map[127-i] = -1 * analog_map[i+128];
	}

	return 1;
}

/* Function to get the device-dependent name of a joystick */
const char *SDL_SYS_JoystickName(int index)
{
	if (index == 0)
		return "PSP controller";

	SDL_SetError("No joystick available with that index");
	return(NULL);
}

/* Function to open a joystick for use.
   The joystick to open is specified by the index field of the joystick.
   This should fill the nbuttons and naxes fields of the joystick structure.
   It returns 0, or -1 if there is an error.
 */
int SDL_SYS_JoystickOpen(SDL_Joystick *joystick)
{
	joystick->nbuttons = 14;
	joystick->naxes = 2;
	joystick->nhats = 0;

	return 0;
}

/* Function to update the state of a joystick - called as a device poll.
 * This function shouldn't update the joystick structure directly,
 * but instead should call SDL_PrivateJoystick*() to deliver events
 * and update joystick device state.
 */

void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick)
{
	int i;
	enum PspCtrlButtons buttons;
	enum PspCtrlButtons changed;
	unsigned char x, y;
	static enum PspCtrlButtons old_buttons = 0;
	static unsigned char old_x = 0, old_y = 0;

	SDL_SemWait(pad_sem);
	buttons = pad.Buttons;
	x = pad.Lx;
	y = pad.Ly;
	SDL_SemPost(pad_sem);

	/* Axes */
	if(old_x != x) {
		SDL_PrivateJoystickAxis(joystick, 0, analog_map[x]);
		old_x = x;
	}
	if(old_y != y) {
		SDL_PrivateJoystickAxis(joystick, 1, analog_map[y]);
		old_y = y;
	}

	/* Buttons */
	changed = old_buttons ^ buttons;
	old_buttons = buttons;
	if(changed) {
		for(i=0; i<sizeof(button_map)/sizeof(button_map[0]); i++) {
			if(changed & button_map[i]) {
				SDL_PrivateJoystickButton(
					joystick, i, 
					(buttons & button_map[i]) ? 
					SDL_PRESSED : SDL_RELEASED);
			}
		}
	}

	sceKernelDelayThread(0);
}

/* Function to close a joystick after use */
void SDL_SYS_JoystickClose(SDL_Joystick *joystick)
{
	/* Do nothing. */
}

/* Function to perform any system-specific joystick related cleanup */
void SDL_SYS_JoystickQuit(void)
{
	/* Cleanup Threads and Semaphore. */
	running = 0;
	SDL_WaitThread(thread, NULL);
	SDL_DestroySemaphore(pad_sem);
}

/* vim: ts=4 sw=4
 */
