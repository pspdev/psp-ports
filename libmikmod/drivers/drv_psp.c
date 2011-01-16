/*	MikMod sound library
	(c) 1998, 1999, 2000 Miodrag Vallat and others - see file AUTHORS for
	complete list.

	This library is free software; you can redistribute it and/or modify
	it under the terms of the GNU Library General Public License as
	published by the Free Software Foundation; either version 2 of
	the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
	02111-1307, USA.
*/

/*==============================================================================

  $Id: drv_win.c,v 1.1.1.1 2004/01/21 01:36:35 raph Exp $

  Output data to PSP audio device

==============================================================================*/

/*

By sweetlilmre 12 November 2005, (mikmod 3.1.11 port), original by Jim Shaw.
Public Domain
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mikmod_internals.h"

#ifdef DRV_PSP

#include <pspkernel.h>
#include <pspaudio.h>
#include <pspaudiolib.h>
#include <string.h>


static int audio_ready=0;
static int audio_handle = -1;
static volatile int audio_terminate=0;
static volatile int bufidx=0;

static short mkimod_sndbuf[2][PSP_NUM_AUDIO_SAMPLES][2];


static volatile int playing = 0;

static void PSP_Update(void)
{
	if (audio_terminate == 0)
  {
		void *bufptr=&mkimod_sndbuf[bufidx];
    if (playing)
    {
      VC_WriteBytes(bufptr, PSP_NUM_AUDIO_SAMPLES*4);
    }
    else
    {
      memset(bufptr, 0, PSP_NUM_AUDIO_SAMPLES*4);
    }
    if (audio_ready)
    {
      sceAudioOutputPannedBlocking(audio_handle, PSP_VOLUME_MAX, PSP_VOLUME_MAX, bufptr);
    }
		bufidx=(bufidx?0:1);
	}
}


static BOOL PSP_IsThere(void)
{
	return 1;
}


static BOOL PSP_Init(void)
{
	if (VC_Init())
		return 1;
    
	audio_terminate=0;
	audio_ready=0;
  audio_handle = -1;

  if ((audio_handle = sceAudioChReserve(-1,PSP_NUM_AUDIO_SAMPLES,0))<0) 
  {
    return 1;
	}
  
	audio_ready = 1;
	return 0;
}

static void PSP_Exit(void)
{
	audio_ready=0;
	audio_terminate=1;

  if (audio_handle != -1)
  {
    sceAudioChRelease(audio_handle);
    audio_handle = -1;
  }

	VC_Exit();
}


static BOOL PSP_Reset(void)
{
	VC_Exit();
	return VC_Init();
}

static BOOL PSP_PlayStart(void)
{
	VC_PlayStart();
	playing = 1;
	return 0;
}

static void PSP_PlayStop(void)
{
	playing = 0;
	VC_PlayStop();
}

MIKMODAPI MDRIVER drv_psp =
{
	NULL,
	"PSP Audio",
	"PSP Output Driver v1.1 - by sweetlilmre, original by Jim Shaw",
	0,255,
	"pspdrv",

	NULL,
	PSP_IsThere,
	VC_SampleLoad,
	VC_SampleUnload,
	VC_SampleSpace,
	VC_SampleLength,
  PSP_Init,
	PSP_Exit,
	PSP_Reset,
  
	VC_SetNumVoices,
	PSP_PlayStart,
	PSP_PlayStop,
	PSP_Update,
  NULL,
  
	VC_VoiceSetVolume,
	VC_VoiceGetVolume,
	VC_VoiceSetFrequency,
	VC_VoiceGetFrequency,
	VC_VoiceSetPanning,
	VC_VoiceGetPanning,
	VC_VoicePlay,
	VC_VoiceStop,
	VC_VoiceStopped,
	VC_VoiceGetPosition,
	VC_VoiceRealVolume
};

#else

MISSING(drv_psp);

#endif


