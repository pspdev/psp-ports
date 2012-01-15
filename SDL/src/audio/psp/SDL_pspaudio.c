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
 "@(#) $Id: SDL_diskaudio.c,v 1.5 2004/01/04 16:49:13 slouken Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "SDL_audio.h"
#include "SDL_error.h"
#include "../SDL_audiomem.h"
#include "../SDL_audio_c.h"
#include "SDL_timer.h"
#include "../SDL_audiodev_c.h"
#include "SDL_pspaudio.h"

#include <pspaudio.h>
#include <pspthreadman.h>

/* The tag name used by PSP audio */
#define PSPAUD_DRIVER_NAME         "psp"

/* Audio driver functions */
static int PSPAUD_OpenAudio(_THIS, SDL_AudioSpec *spec);
static void PSPAUD_ThreadInit(_THIS);
static void PSPAUD_WaitAudio(_THIS);
static void PSPAUD_PlayAudio(_THIS);
static Uint8 *PSPAUD_GetAudioBuf(_THIS);
static void PSPAUD_CloseAudio(_THIS);

/* Audio driver bootstrap functions */
static int PSPAUD_Available(void)
{
	return 1;
}

static void PSPAUD_DeleteDevice(SDL_AudioDevice *device)
{
	free(device->hidden);
	free(device);
}

static SDL_AudioDevice *PSPAUD_CreateDevice(int devindex)
{
	SDL_AudioDevice *this;

	/* Initialize all variables that we clean on shutdown */
	this = (SDL_AudioDevice *)malloc(sizeof(SDL_AudioDevice));
	if ( this ) {
		memset(this, 0, (sizeof *this));
		this->hidden = (struct SDL_PrivateAudioData *)
				malloc((sizeof *this->hidden));
	}
	if ( (this == NULL) || (this->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( this ) {
			free(this);
		}
		return(0);
	}
	memset(this->hidden, 0, (sizeof *this->hidden));

	/* Set the function pointers */
	this->OpenAudio = PSPAUD_OpenAudio;
	this->ThreadInit = PSPAUD_ThreadInit;
	this->WaitAudio = PSPAUD_WaitAudio;
	this->PlayAudio = PSPAUD_PlayAudio;
	this->GetAudioBuf = PSPAUD_GetAudioBuf;
	this->CloseAudio = PSPAUD_CloseAudio;

	this->free = PSPAUD_DeleteDevice;

	return this;
}

AudioBootStrap PSPAUD_bootstrap = {
	PSPAUD_DRIVER_NAME, "PSP audio driver",
	PSPAUD_Available, PSPAUD_CreateDevice
};

/* This function waits until it is possible to write a full sound buffer */
static void PSPAUD_WaitAudio(_THIS)
{
	/* Because we block when sending audio, there's no need for this function to do anything. */
}

static void PSPAUD_PlayAudio(_THIS)
{
	Uint8 *mixbuf = this->hidden->mixbufs[this->hidden->next_buffer];

	if (this->spec.channels == 1) {
		sceAudioOutputBlocking(this->hidden->channel, PSP_AUDIO_VOLUME_MAX, mixbuf);
	} else {
		sceAudioOutputPannedBlocking(this->hidden->channel, PSP_AUDIO_VOLUME_MAX, PSP_AUDIO_VOLUME_MAX, mixbuf);
	}

	this->hidden->next_buffer = (this->hidden->next_buffer + 1) % NUM_BUFFERS;
}

static Uint8 *PSPAUD_GetAudioBuf(_THIS)
{
	return this->hidden->mixbufs[this->hidden->next_buffer];
}

static void PSPAUD_CloseAudio(_THIS)
{
	if (this->hidden->channel >= 0) {
		sceAudioChRelease(this->hidden->channel);
		this->hidden->channel = -1;
	}

	if (this->hidden->rawbuf != NULL) {
		free(this->hidden->rawbuf);
		this->hidden->rawbuf = NULL;
	}
}

static void PSPAUD_ThreadInit(_THIS)
{
	/* Increase the priority of this audio thread by 1 to put it
	   ahead of other SDL threads. */
	SceUID thid;
	SceKernelThreadInfo status;
	thid = sceKernelGetThreadId();
	status.size = sizeof(SceKernelThreadInfo);
	if (sceKernelReferThreadStatus(thid, &status) == 0) {
		sceKernelChangeThreadPriority(thid, status.currentPriority - 1);
	}
}

static int PSPAUD_OpenAudio(_THIS, SDL_AudioSpec *spec)
{
	int format, mixlen, i;

	switch (spec->format & 0xff) {
		case 8:
		case 16:
			spec->format = AUDIO_S16LSB;
			break;
		default:
			SDL_SetError("Unsupported audio format");
			return -1;
	}

	/* The sample count must be a multiple of 64. */
	spec->samples = PSP_AUDIO_SAMPLE_ALIGN(spec->samples);
	spec->freq = 44100;

	/* Update the fragment size as size in bytes. */
	SDL_CalculateAudioSpec(spec);

	/* Allocate the mixing buffer.  Its size and starting address must
	   be a multiple of 64 bytes.  Our sample count is already a multiple of
	   64, so spec->size should be a multiple of 64 as well. */
	mixlen = spec->size * NUM_BUFFERS;
	this->hidden->rawbuf = (Uint8 *) memalign(64, mixlen);
	if (this->hidden->rawbuf == NULL) {
		SDL_SetError("Couldn't allocate mixing buffer");
		return -1;
	}

	/* Setup the hardware channel. */
	if (spec->channels == 1) {
		format = PSP_AUDIO_FORMAT_MONO;
	} else {
		format = PSP_AUDIO_FORMAT_STEREO;
	}
	this->hidden->channel = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL, spec->samples, format);
	if (this->hidden->channel < 0) {
		SDL_SetError("Couldn't reserve hardware channel");
		free(this->hidden->rawbuf);
		this->hidden->rawbuf = NULL;
		return -1;
	}

	memset(this->hidden->rawbuf, 0, mixlen);
	for (i = 0; i < NUM_BUFFERS; i++) {
		this->hidden->mixbufs[i] = &this->hidden->rawbuf[i * spec->size];
	}

	this->hidden->next_buffer = 0;
	return 0;
}

/* vim: ts=4 sw=4
 */
