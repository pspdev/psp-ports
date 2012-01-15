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

/*  
    PSP port contributed by:
    Marcus R. Brown <mrbrown@ocgnet.org>
    Jim Paris <jim@jtan.com>
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_nullvideo.h,v 1.4 2004/01/04 16:49:24 slouken Exp $";
#endif

#ifndef _SDL_pspvideo_h
#define _SDL_pspvideo_h

#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "SDL_mutex.h"

#include <psptypes.h>
#include <pspge.h>
#include <pspkernel.h>
#include <psputils.h>
#include <pspdisplay.h>
#include <pspgu.h>

/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_VideoDevice *this


/* Private display data */

struct SDL_PrivateVideoData {
	void *vram_base;
	/* Current display's pixel format. */
	int pixel_format;
	/* The current draw frame for double-buffered displays. */
	int frame;
	/* Byte offset of the start of the second frame. */
	unsigned int frame_offset;
	/* Screen format */
	int psm;
	/* Texture format */
	int tpsm;
	/* Pointer to the CLUT. */
	void *gu_palette;
	/* Dimensions for stretched blit */
	SDL_Rect hw_rect;
	SDL_Rect sw_rect;
};

#endif /* _SDL_pspvideo_h */
