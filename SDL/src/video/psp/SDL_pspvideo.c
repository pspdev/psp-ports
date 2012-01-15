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
    Matthew H <matthewh@webone.com.au>
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_nullvideo.c,v 1.7 2004/01/04 16:49:24 slouken Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "SDL.h"
#include "SDL_error.h"
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_pspvideo.h"
#include "SDL_pspevents_c.h"
#include "SDL_pspgl_c.h"
#include "SDL_pspmouse_c.h"

#define PSPVID_DRIVER_NAME "psp"

#define PSP_SLICE_SIZE	(32)
#define PSP_LINE_SIZE (512)
#define SCREEN_WIDTH (480)
#define SCREEN_HEIGHT (272)

#define IS_SWSURFACE(flags) ((flags & SDL_HWSURFACE) == SDL_SWSURFACE) 
#define IS_HWSURFACE(flags) ((flags & SDL_HWSURFACE) == SDL_HWSURFACE) 

static unsigned int list[4096] __attribute__((aligned(16)));

struct texVertex
{
	unsigned short u, v;
	short x, y, z;
};

struct rectVertex
{
	short x, y, z;
};

/* Initialization/Query functions */
static int PSP_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **PSP_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *PSP_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int PSP_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static void PSP_VideoQuit(_THIS);
void PSP_EventInit(_THIS);
void PSP_EventQuit(_THIS);

/* Hardware surface functions */
static int PSP_AllocHWSurface(_THIS, SDL_Surface *surface);
static int PSP_LockHWSurface(_THIS, SDL_Surface *surface);
static void PSP_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void PSP_FreeHWSurface(_THIS, SDL_Surface *surface);
static int PSP_FlipHWSurface(_THIS, SDL_Surface *surface);
static void PSP_UpdateRects(_THIS, int numrects, SDL_Rect *rects);
static void PSP_GuInit(_THIS, SDL_Surface *current, int bpp);
static int PSP_CheckHWBlit(_THIS, SDL_Surface *src, SDL_Surface *dst);
static int HWAccelBlit(SDL_Surface *src, SDL_Rect *srcrect,
                       SDL_Surface *dst, SDL_Rect *dstrect);
static int PSP_GuStretchBlit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Rect *dstrect);
static int PSP_FillHWRect(_THIS, SDL_Surface *dst, SDL_Rect *rect, Uint32 color);

/* Software surface function */
static void PSP_GuUpdateRects(_THIS, int numrects, SDL_Rect *rects);

/* return a suitable psm value for a particular bpp */
static inline int psm_value(int bpp) {
	if (bpp == 15)
		return GU_PSM_5551;
	if (bpp == 16)
		return GU_PSM_5650;
	return GU_PSM_8888;
}

/* PSP driver bootstrap functions */

static int PSP_Available(void)
{
	return 1;
}

static void PSP_DeleteDevice(SDL_VideoDevice *device)
{
	if (device) {
		if (device->hidden) {
			free(device->hidden);
		}
#ifdef HAVE_OPENGL
		if (device->gl_data) {
			free(device->gl_data);
		}
#endif
		free(device);
	}
}

static SDL_VideoDevice *PSP_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	/* Initialize all variables that we clean on shutdown */
	device = (SDL_VideoDevice *)malloc(sizeof(SDL_VideoDevice));
	if ( device ) {
		memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				malloc((sizeof *device->hidden));
#ifdef HAVE_OPENGL
		device->gl_data = (struct SDL_PrivateGLData *)
				malloc((sizeof *device->gl_data));
#endif
	}
	if ( (device == NULL) || (device->hidden == NULL) 
#ifdef HAVE_OPENGL
			|| (device->gl_data == NULL)
#endif
			) {
		SDL_OutOfMemory();
		if ( device ) {
			free(device);
		}
		return(0);
	}
	memset(device->hidden, 0, (sizeof *device->hidden));
#ifdef HAVE_OPENGL
	memset(device->gl_data, 0, (sizeof *device->gl_data));
#endif

	/* Set the function pointers */
	device->VideoInit = PSP_VideoInit;
	device->ListModes = PSP_ListModes;
	device->SetVideoMode = PSP_SetVideoMode;
	device->CreateYUVOverlay = NULL;
	device->SetColors = PSP_SetColors;
	device->UpdateRects = PSP_UpdateRects;
	device->VideoQuit = PSP_VideoQuit;
	device->AllocHWSurface = PSP_AllocHWSurface;
	device->CheckHWBlit = PSP_CheckHWBlit;
	device->FillHWRect = PSP_FillHWRect;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = PSP_LockHWSurface;
	device->UnlockHWSurface = PSP_UnlockHWSurface;
	device->FlipHWSurface = PSP_FlipHWSurface;
	device->FreeHWSurface = PSP_FreeHWSurface;
#ifdef HAVE_OPENGL
	device->GL_GetProcAddress = PSP_GL_GetProcAddress;
	device->GL_GetAttribute = PSP_GL_GetAttribute;
	device->GL_MakeCurrent = PSP_GL_MakeCurrent;
	device->GL_SwapBuffers = PSP_GL_SwapBuffers;
#endif
	device->SetCaption = NULL;
	device->SetIcon = NULL;
	device->IconifyWindow = NULL;
	device->GrabInput = NULL;
	device->GetWMInfo = NULL;
	device->InitOSKeymap = PSP_InitOSKeymap;
	device->PumpEvents = PSP_PumpEvents;

	device->free = PSP_DeleteDevice;

	return device;
}

VideoBootStrap PSP_bootstrap = {
	PSPVID_DRIVER_NAME, "PSP video driver",
	PSP_Available, PSP_CreateDevice
};

const static SDL_Rect RECT_480x272 = { .x = 0, .y = 0, .w = 480, .h = 272 };
const static SDL_Rect *modelist[] = {
	&RECT_480x272,
	NULL
};

int PSP_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	/* Default for pspsdk is 8888 ABGR */
	vformat->BitsPerPixel = 32;
	vformat->BytesPerPixel = 4;

	this->info.wm_available = 0;
	this->info.hw_available = 1;
	this->info.blit_fill = 0; /* todo: fixme */
	this->info.blit_hw = 1;
	this->info.blit_hw_CC = 1;
	this->info.blit_hw_A = 0; /* todo: implement me */

	PSP_EventInit(this);
	
	return(0);
}

SDL_Rect **PSP_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
	/* x dimension should be a multiple of PSP_SLICE_SIZE */
	if (IS_SWSURFACE(flags))
		return (SDL_Rect **)-1;

	switch(format->BitsPerPixel) {
	case 8: /* proxied by a shadow surface */
	case 15:
	case 16:
	case 32:
		return (SDL_Rect **)modelist;
	default:
		return NULL;
	}
}

static void PSP_GuInit(_THIS, SDL_Surface *current, int bpp) {
	void *dispbuffer = (void*) 0;
	void *drawbuffer = (void*) this->hidden->frame_offset;

	sceGuInit();
	sceGuStart(GU_DIRECT, list); 
	sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, dispbuffer, PSP_LINE_SIZE);
	if (IS_SWSURFACE(current->flags) || (current->flags & SDL_HWPALETTE)) {
		sceGuClutMode(GU_PSM_8888, 0, 255, 0);
		sceGuDrawBuffer(GU_PSM_8888, drawbuffer, PSP_LINE_SIZE);
	} else {
		sceGuDrawBuffer(psm_value(bpp), drawbuffer, PSP_LINE_SIZE);
	}
	sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
	sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
	sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
	sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuFrontFace(GU_CW);

	sceGuFinish();
	sceGuSync(0, 0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(1);
}

/*
vidmem handling from Holger Waechtler's pspgl
*/
struct vidmem_chunk {
	void *ptr;
	unsigned long len;
};


static struct vidmem_chunk *vidmem_map = NULL;
static unsigned long vidmem_map_len = 0;

static
void *vidmem_map_insert_new (unsigned long idx, unsigned long adr, unsigned long size)
{
	void *tmp = realloc(vidmem_map, (vidmem_map_len + 1) * sizeof(vidmem_map[0]));

	if (!tmp)
		return NULL;

	vidmem_map = tmp;
	memmove(&vidmem_map[idx+1], &vidmem_map[idx], (vidmem_map_len-idx) * sizeof(vidmem_map[0]));
	vidmem_map_len++;
	vidmem_map[idx].ptr = (void*) adr;
	vidmem_map[idx].len = size;

	return vidmem_map[idx].ptr;
}


void* vidmem_alloc (unsigned long size)
{
	unsigned long start = (unsigned long) sceGeEdramGetAddr();
	unsigned long adr = start;
	unsigned long i;

	/* round the size up to the nearest 16 bytes and all hwsurfaces are safe to 
	   use as textures */
	if (size % 16 != 0)
		size += 16 - (size % 16); 

	for (i=0; i<vidmem_map_len; i++) {
		if (vidmem_map[i].ptr != NULL) {
			unsigned long new_adr = (unsigned long) vidmem_map[i].ptr;
			if (size <= new_adr - adr)
				return vidmem_map_insert_new(i, adr, size);
			adr = new_adr + vidmem_map[i].len;
		}
	}

	if (adr + size > start + sceGeEdramGetSize())
		return NULL;

	return vidmem_map_insert_new(vidmem_map_len, adr, size);
}


void  vidmem_free (void * ptr)
{
	int i;

	for (i=0; i<vidmem_map_len; i++) {
		if (vidmem_map[i].ptr == ptr) {
			void *tmp;
			
			vidmem_map_len--;
			memmove(&vidmem_map[i], &vidmem_map[i+1], (vidmem_map_len-i) * sizeof(vidmem_map[0]));
			tmp = realloc(vidmem_map, vidmem_map_len * sizeof(vidmem_map[0]));
			if (tmp)
				vidmem_map = tmp;
		}
	}
}

static inline int roundUpToPowerOfTwo (int x)
{
	return 1 << (32 - __builtin_allegrex_clz(x - 1));
}

SDL_Surface *PSP_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	int disp_pitch = 512, draw_pitch = 512, pixel_format;
	Uint32 Amask, Rmask, Gmask, Bmask;

	if (IS_HWSURFACE(flags) &&
		(width != SCREEN_WIDTH || height != SCREEN_HEIGHT)) 
	{
		SDL_SetError("Couldn't find requested mode");
		return NULL;
	}

	switch(bpp) {
	case 8: /* indexed, uses a shadow buffer for hwsurfaces */
		disp_pitch *= 4;
		Amask = 0;
		Rmask = 0;
		Gmask = 0;
		Bmask = 0;
		pixel_format = PSP_DISPLAY_PIXEL_FORMAT_8888; 
		break;
	case 15: /* 5-5-5-1 */
		disp_pitch *= 2;
		draw_pitch *= 2;
		Amask = 0x00008000;
		Rmask = 0x0000001f;
		Gmask = 0x000003e0;
		Bmask = 0x00007c00;
		pixel_format = PSP_DISPLAY_PIXEL_FORMAT_5551;
		break;
	case 16: /* 5-6-5 */
		disp_pitch *= 2;
		draw_pitch *= 2;
		Amask = 0;
		Rmask = 0x0000001f;
		Gmask = 0x000007e0;
		Bmask = 0x0000f800;
		pixel_format = PSP_DISPLAY_PIXEL_FORMAT_565;
		break;
	case 32: /* 8-8-8-8 */
		disp_pitch *= 4;
		draw_pitch *= 4;
		Amask = 0xff000000;
		Rmask = 0x000000ff;
		Gmask = 0x0000ff00;
		Bmask = 0x00ff0000;
		pixel_format = PSP_DISPLAY_PIXEL_FORMAT_8888;
		break;
	default:
		SDL_SetError("Couldn't find requested mode");
		return(NULL);
	}
	
	if ( ! SDL_ReallocFormat(current, bpp, Rmask, Gmask, Bmask, 0/*Amask*/) ) {
		SDL_SetError("Couldn't allocate color format");
		return(NULL);
	}

	current->flags = flags | SDL_FULLSCREEN;
	current->w = width;
	current->h = height;

#ifdef HAVE_OPENGL
	if (flags & SDL_OPENGL)
	{
		this->gl_config.driver_loaded = 1;
		current->flags = SDL_FULLSCREEN | SDL_OPENGL;
		/* HACK: What does this need to be? */
		current->pixels = memalign(16, draw_pitch * height);

		if (!PSP_GL_Init(this))
		{
			/* Don't set an error here as PSP_GL_Init() will set one. */
			return NULL;
		}
		return current;
	}
#endif

	this->hidden->psm = psm_value(bpp);
	this->hidden->tpsm = this->hidden->psm; 
	this->hidden->pixel_format = pixel_format;
	this->hidden->frame = 0;
	this->hidden->frame_offset = 0;

	/* allocate display buffer */
	this->hidden->vram_base = vidmem_alloc(disp_pitch * SCREEN_HEIGHT); 

	sceDisplaySetMode(0, SCREEN_WIDTH, SCREEN_HEIGHT);
	sceDisplaySetFrameBuf(this->hidden->vram_base, 512, pixel_format, 
		PSP_DISPLAY_SETBUF_NEXTFRAME);

	if (IS_HWSURFACE(flags)) {

		current->pitch = draw_pitch;

		if (flags & SDL_DOUBLEBUF) {
			this->hidden->frame_offset = disp_pitch * height;

			/* Set the draw buffer to the second frame. */
			this->hidden->frame = 1;

			/* allocate drawbuffer */
			vidmem_alloc(disp_pitch * height); 
		}

		if (bpp == 8) {
			/* create a shadow surface */
			current->pixels = memalign(16, current->pitch * height); 

			/* can't hwaccel 8bpp hwsurface */
			this->info.blit_fill = 0;
			this->info.blit_hw = 0;
		} else {
	    	current->pixels = (void *) ((Uint32) this->hidden->vram_base + 
				this->hidden->frame_offset);
			current->flags |= SDL_PREALLOC; /* so SDL doesn't free ->pixels */
		}

    } else if (IS_SWSURFACE(flags)) {
		char *aspect_ratio;

		aspect_ratio = getenv("SDL_ASPECT_RATIO");

		if (aspect_ratio && !strcmp(aspect_ratio, "4:3")) {
			this->hidden->hw_rect.w = 360;
			this->hidden->hw_rect.h = 272;
			this->hidden->hw_rect.x = 60;
			this->hidden->hw_rect.y = 0;
		} else {
			this->hidden->hw_rect.w = 480;
			this->hidden->hw_rect.h = 272;
			this->hidden->hw_rect.x = 0;
			this->hidden->hw_rect.y = 0;
		}

		this->hidden->sw_rect.x = 0;
		this->hidden->sw_rect.y = 0;
		this->hidden->sw_rect.w = width;
		this->hidden->sw_rect.h = height;

		current->pitch = (width > 512) ? width * (bpp/8) : 512 * (bpp/8);
		current->pixels = memalign(16, current->pitch * height); 

		this->UpdateRects = PSP_GuUpdateRects;
    }

	PSP_GuInit(this, current, bpp);

	/* We're done */
	return(current);
}

static int PSP_AllocHWSurface(_THIS, SDL_Surface *surface)
{
	int pitch;

	pitch = roundUpToPowerOfTwo(surface->pitch);
	surface->pixels = vidmem_alloc(pitch * surface->h);

	if (!surface->pixels)
		return -1;

	surface->pitch = pitch;
	surface->flags |= SDL_HWSURFACE;
	surface->hwdata = (void*)1; /* Hack to make SDL realize it's a HWSURFACE when freeing */
	return 0;
}
static void PSP_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	vidmem_free(surface->pixels);
	surface->pixels = NULL;
	return;
}

/* We need to wait for vertical retrace on page flipped displays */
static int PSP_LockHWSurface(_THIS, SDL_Surface *surface)
{
	return(0);
}

static void PSP_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	/* Flush video RAM */
	sceKernelDcacheWritebackAll();

	return;
}

/* Show the draw buffer as the display buffer, and setup the next draw buffer. */
static int PSP_FlipHWSurface(_THIS, SDL_Surface *surface)
{
	void *new_pixels;

	sceKernelDcacheWritebackAll();
	if (surface->format->BitsPerPixel == 8) {
		/* blit the shadow surface */
		PSP_GuStretchBlit(surface, (SDL_Rect *)&RECT_480x272, (SDL_Rect *)&RECT_480x272);
		sceGuSync(0, 0);
	} else { 
		this->hidden->frame ^= 1;
		new_pixels = (void *) ((Uint32) this->hidden->vram_base +
				(this->hidden->frame_offset * this->hidden->frame));
		surface->pixels = new_pixels;
	}
	sceGuSwapBuffers();

	return 0;
}

static void PSP_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
}

static int PSP_CheckHWBlit(_THIS, SDL_Surface *src, SDL_Surface *dst)
{
	src->flags |= SDL_HWACCEL;
	src->map->hw_blit = HWAccelBlit;
	return 1;
}

static int HWAccelBlit(SDL_Surface *src, SDL_Rect *srcrect,
                       SDL_Surface *dst, SDL_Rect *dstrect)
{
	sceKernelDcacheWritebackAll();

	/* when rendering to the screen from a 16 byte aligned address,
	   use GuStretchBlit without stretching */
	if ((dst == current_video->screen) && IS_HWSURFACE(src->flags)) {

		PSP_GuStretchBlit(src, srcrect, dstrect); 

	} else {

		sceGuStart(GU_DIRECT,list);

		sceGuCopyImage(current_video->hidden->psm,
			srcrect->x, srcrect->y, srcrect->w, srcrect->h, 
			src->pitch / src->format->BytesPerPixel, src->pixels,
			dstrect->x, dstrect->y, dst->pitch / dst->format->BytesPerPixel, 
			dst->pixels);

		sceGuFinish();

	}

	sceGuSync(0, 0);

	return 0;
}

/** 
 * update screen from 16 byte aligned src surface
 */
static int PSP_GuStretchBlit(SDL_Surface *src, SDL_Rect *srcrect, SDL_Rect *dstrect)
{
	unsigned short old_slice = 0; /* set when we load 2nd tex */
	unsigned int slice, num_slices, width, height, tbw, off_x, off_bytes;
	struct texVertex *vertices;
	void *pixels;

	off_bytes = (int)(src->pixels + srcrect->x * src->format->BytesPerPixel) & 0xf;
	off_x = off_bytes / src->format->BytesPerPixel;
	width = roundUpToPowerOfTwo(srcrect->w + off_bytes);
	height = roundUpToPowerOfTwo(srcrect->h);
	tbw = src->pitch / src->format->BytesPerPixel;

	/* Align the texture prior to srcrect->x */
	pixels = src->pixels + (srcrect->x - off_x) * src->format->BytesPerPixel + 
		src->pitch * srcrect->y;
	num_slices = (srcrect->w + (PSP_SLICE_SIZE - 1)) / PSP_SLICE_SIZE;

	/* GE doesn't appreciate textures wider than 512 */
	if (width > 512)
		width = 512; 

	sceGuStart(GU_DIRECT,list);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuTexMode(current_video->hidden->tpsm,0,0,0);
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGB);
	sceGuTexFilter(GU_LINEAR, GU_LINEAR);
	sceGuTexImage(0, width, height, tbw, pixels);
	sceGuTexSync();

	for (slice = 0; slice < num_slices; slice++) {

		vertices = (struct texVertex*)sceGuGetMemory(2 * sizeof(struct texVertex));

		if ((slice * PSP_SLICE_SIZE) < width) {
			vertices[0].u = slice * PSP_SLICE_SIZE + off_x;
		} else {
			if (!old_slice) {
				/* load another texture (src width > 512) */
				pixels += width * src->format->BytesPerPixel;
				sceGuTexImage(0, roundUpToPowerOfTwo(srcrect->w - width), 
					height, tbw, pixels);
				sceGuTexSync();
				old_slice = slice;
			}
			vertices[0].u = (slice - old_slice) * PSP_SLICE_SIZE + off_x;
		}
		vertices[1].u = vertices[0].u + PSP_SLICE_SIZE;
		if (vertices[1].u > (off_x + srcrect->w))
			vertices[1].u = off_x + srcrect->w;

		vertices[0].v = 0;
		vertices[1].v = vertices[0].v + srcrect->h;

		vertices[0].x = dstrect->x + slice * PSP_SLICE_SIZE * dstrect->w / srcrect->w;
		vertices[1].x = vertices[0].x + PSP_SLICE_SIZE * dstrect->w / srcrect->w;
		if (vertices[1].x > (dstrect->x + dstrect->w))
			vertices[1].x = dstrect->x + dstrect->w;

		vertices[0].y = dstrect->y;
		vertices[1].y = vertices[0].y + dstrect->h;

		vertices[0].z = 0;
		vertices[1].z = 0;

		sceGuDrawArray(GU_SPRITES,GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D,
			2,0,vertices);
	}

	sceGuFinish();

	return 0;
}

static int PSP_FillHWRect(_THIS, SDL_Surface *dst, SDL_Rect *rect, Uint32 color)
{
	struct rectVertex *vertices;
	void *gu_offset;

	gu_offset = (void *)((int)current_video->screen->pixels & 0x00ffffff);

	sceGuStart(GU_DIRECT,list);

	vertices = sceGuGetMemory(2 * sizeof(struct rectVertex));

	vertices[0].x = rect->x;
	vertices[1].x = rect->x + rect->w;
	vertices[0].y = rect->y;
	vertices[1].y = rect->y + rect->h;

	sceGuDrawBuffer(psm_value(dst->format->BitsPerPixel),
		(void *)((int)dst->pixels & 0x00ffffff), 
		dst->pitch / dst->format->BytesPerPixel);

	sceGuColor(color);
	sceGuDrawArray(GU_SPRITES,GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertices);
	
	sceGuDrawBuffer(this->hidden->psm, gu_offset, PSP_LINE_SIZE);

	sceGuFinish();
	sceGuSync(0, 0);

	return 0;
}

/**
 * Update the screen from SDL_SWSURFACE (and scale).
 */
static void PSP_GuUpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	SDL_Surface *src = SDL_VideoSurface;

	sceKernelDcacheWritebackAll();

	/* if the screen dimensions are unusual, do a single update */
	if ((src->w != 480) || (src->h != 272)) {

		PSP_GuStretchBlit(src, &this->hidden->sw_rect, &this->hidden->hw_rect);

	} else {
		/* update the specified rects */
		while(numrects--) {
			PSP_GuStretchBlit(src, rects, rects);
			rects++;
		} 
	}

	sceGuSync(0, 0);
}

int PSP_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	int i, j, ret = 1;
	unsigned int *palette;

	if (ncolors > 256) {
		ncolors = 256;
		ret = 0;
	}

	if (this->hidden->gu_palette == NULL) {
		this->hidden->gu_palette = memalign(16, 4 * 256);
		this->hidden->tpsm = GU_PSM_T8;
	}

	palette = this->hidden->gu_palette;

	for (i=firstcolor, j=0; j < ncolors; i++, j++) {
		palette[i] = (colors[j].b << 16) | (colors[j].g << 8) | (colors[j].r);
	}

	sceKernelDcacheWritebackAll();
	sceGuStart(GU_DIRECT, list);
	sceGuClutLoad(32, this->hidden->gu_palette);
	sceGuFinish();
	sceGuSync(0,0);

	return ret;
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
void PSP_VideoQuit(_THIS)
{
	if (this->hidden->gu_palette != NULL) {
		free(this->hidden->gu_palette);
		this->hidden->gu_palette = NULL;
	}

	sceGuTerm();

	PSP_EventQuit(this);

	vidmem_free(this->hidden->vram_base);

	if (this->screen->flags & SDL_DOUBLEBUF)
		vidmem_free(this->hidden->vram_base + this->hidden->frame_offset);

	return;
}

/* vim: ts=4 sw=4
 */
