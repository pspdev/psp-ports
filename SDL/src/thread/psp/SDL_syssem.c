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

/* PSP port contributed by Marcus R. Brown <mrbrown@ocgnet.org>. */

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_syssem.c,v 1.6 2004/01/04 16:49:19 slouken Exp $";
#endif

/* Semaphore functions for the PSP. */

#include <stdio.h>
#include <stdlib.h>

#include "SDL_error.h"
#include "SDL_thread.h"

#include <pspkerneltypes.h>
#include <pspthreadman.h>
#include <pspkerror.h>

struct SDL_semaphore {
	SceUID	semid;
};


/* Create a semaphore */
SDL_sem *SDL_CreateSemaphore(Uint32 initial_value)
{
	SDL_sem *sem;

	sem = (SDL_sem *) malloc(sizeof(*sem));
	if (sem != NULL) {
		/* TODO: Figure out the limit on the maximum value. */
		sem->semid = sceKernelCreateSema("SDL sema", 0, initial_value, 255, NULL);
		if (sem->semid < 0) {
			SDL_SetError("Couldn't create semaphore");
			free(sem);
			sem = NULL;
		}
	} else {
		SDL_OutOfMemory();
	}

	return sem;
}

/* Free the semaphore */
void SDL_DestroySemaphore(SDL_sem *sem)
{
	if (sem != NULL) {
		if (sem->semid > 0) {
			sceKernelDeleteSema(sem->semid);
			sem->semid = 0;
		}

		free(sem);
	}
}

/* TODO: This routine is a bit overloaded.
 * If the timeout is 0 then just poll the semaphore; if it's SDL_MUTEX_MAXWAIT, pass
 * NULL to sceKernelWaitSema() so that it waits indefinitely; and if the timeout
 * is specified, convert it to microseconds. */
int SDL_SemWaitTimeout(SDL_sem *sem, Uint32 timeout)
{
	Uint32 *pTimeout;
       unsigned int res;

	if (sem == NULL) {
		SDL_SetError("Passed a NULL sem");
		return 0;
	}

	if (timeout == 0) {
		res = sceKernelPollSema(sem->semid, 1);
		if (res < 0) {
			return SDL_MUTEX_TIMEDOUT;
		}
		return 0;
	}

	if (timeout == SDL_MUTEX_MAXWAIT) {
		pTimeout = NULL;
	} else {
		timeout *= 1000;  /* Convert to microseconds. */
		pTimeout = &timeout;
	}

	res = sceKernelWaitSema(sem->semid, 1, pTimeout);
       switch (res) {
               case SCE_KERNEL_ERROR_OK:
                       return 0;
               case SCE_KERNEL_ERROR_WAIT_TIMEOUT:
                       return SDL_MUTEX_TIMEDOUT;
               default:
                       SDL_SetError("WaitForSingleObject() failed");
                       return -1;
	}
}

int SDL_SemTryWait(SDL_sem *sem)
{
	return SDL_SemWaitTimeout(sem, 0);
}

int SDL_SemWait(SDL_sem *sem)
{
	return SDL_SemWaitTimeout(sem, SDL_MUTEX_MAXWAIT);
}

/* Returns the current count of the semaphore */
Uint32 SDL_SemValue(SDL_sem *sem)
{
	SceKernelSemaInfo info;

	if (sem == NULL) {
		SDL_SetError("Passed a NULL sem");
		return 0;
	}

	if (sceKernelReferSemaStatus(sem->semid, &info) >= 0) {
		return info.currentCount;
	}

	return 0;
}

int SDL_SemPost(SDL_sem *sem)
{
	int res;

	if (sem == NULL) {
		SDL_SetError("Passed a NULL sem");
		return -1;
	}

	res = sceKernelSignalSema(sem->semid, 1);
	if (res < 0) {
		SDL_SetError("sceKernelSignalSema() failed");
		return -1;
	}

	return 0;
}

/* vim: ts=4 sw=4
 */
