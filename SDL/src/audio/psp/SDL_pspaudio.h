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
 "@(#) $Id: SDL_diskaudio.h,v 1.4 2004/01/04 16:49:13 slouken Exp $";
#endif

#ifndef _SDL_pspaudio_h
#define _SDL_pspaudio_h

#include "../SDL_sysaudio.h"

/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_AudioDevice *this

#define NUM_BUFFERS 2

struct SDL_PrivateAudioData {
	/* The hardware output channel. */
	int		channel;
	/* The raw allocated mixing buffer. */
	Uint8	*rawbuf;
	/* Individual mixing buffers. */
	Uint8	*mixbufs[NUM_BUFFERS];
	/* Index of the next available mixing buffer. */
	int		next_buffer;
};

#endif /* _SDL_pspaudio_h */
/* vim: ts=4 sw=4
 */
