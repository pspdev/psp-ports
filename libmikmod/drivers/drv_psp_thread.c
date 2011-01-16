/*

Name:
drv_psp.c
Output data to PSP audio device

By Jim Shaw 9 July 2005
Public Domain

*/

#include <pspkernel.h>
#include <pspaudiolib.h>

#include <string.h>

#include "mikmod.h"
//#include "mmio.h"

static int playing = 0;

static BOOL PSP_IsThere(void)
{
	return 1;
}

static void sound_callback(void *buf, unsigned int reqn, void *userdata)
{
	if (playing)
	{
		VC_WriteBytes(buf, reqn*4);
	}
	else
	{
		memset(buf, 0, reqn*4);
	}
}

static BOOL PSP_Init(void)
{
	if (VC_Init())
		return 1;

	pspAudioInit();
	pspAudioSetChannelCallback(0, (void *)sound_callback, NULL);

	return 0;
}

static void PSP_Exit(void)
{
	pspAudioEndPre();
	pspAudioEnd();
	VC_Exit();
}

static void PSP_Update(void)
{
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
	"PSP Output Driver v1.0 - by Jim Shaw",
	0,255,
	"psp",
	NULL,	//command line
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
	NULL,	//pause
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
/*
	NULL,
	"PSP Audio",
	"PSP Output Driver v1.0 - by Jim Shaw",
	0,255,
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
	VC_VoiceSetVolume,
	VC_VoiceSetFrequency,
	VC_VoiceSetPanning,
	VC_VoicePlay,
	VC_VoiceStop,
	VC_VoiceStopped,
	VC_VoiceReleaseSustain,
	VC_VoiceGetPosition,
	VC_VoiceRealVolume
*/
