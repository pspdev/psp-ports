/*
    SDL_image:  An example image loading library for use with SDL
    Copyright (C) 1997-2009 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#if !defined(__APPLE__) || defined(SDL_IMAGE_USE_COMMON_BACKEND)

/* This is a TIFF image file loading framework */

#include <stdio.h>

#include "SDL_image.h"

#ifdef LOAD_TIF

#include <tiffio.h>

static struct {
	int loaded;
	void *handle;
	TIFF* (*TIFFClientOpen)(const char*, const char*, thandle_t, TIFFReadWriteProc, TIFFReadWriteProc, TIFFSeekProc, TIFFCloseProc, TIFFSizeProc, TIFFMapFileProc, TIFFUnmapFileProc);
	void (*TIFFClose)(TIFF*);
	int (*TIFFGetField)(TIFF*, ttag_t, ...);
	int (*TIFFReadRGBAImage)(TIFF*, uint32, uint32, uint32*, int);
	TIFFErrorHandler (*TIFFSetErrorHandler)(TIFFErrorHandler);
} lib;

#ifdef LOAD_TIF_DYNAMIC
int IMG_InitTIF()
{
	if ( lib.loaded == 0 ) {
		lib.handle = SDL_LoadObject(LOAD_TIF_DYNAMIC);
		if ( lib.handle == NULL ) {
			return -1;
		}
		lib.TIFFClientOpen =
			(TIFF* (*)(const char*, const char*, thandle_t, TIFFReadWriteProc, TIFFReadWriteProc, TIFFSeekProc, TIFFCloseProc, TIFFSizeProc, TIFFMapFileProc, TIFFUnmapFileProc))
			SDL_LoadFunction(lib.handle, "TIFFClientOpen");
		if ( lib.TIFFClientOpen == NULL ) {
			SDL_UnloadObject(lib.handle);
			return -1;
		}
		lib.TIFFClose =
			(void (*)(TIFF*))
			SDL_LoadFunction(lib.handle, "TIFFClose");
		if ( lib.TIFFClose == NULL ) {
			SDL_UnloadObject(lib.handle);
			return -1;
		}
		lib.TIFFGetField =
			(int (*)(TIFF*, ttag_t, ...))
			SDL_LoadFunction(lib.handle, "TIFFGetField");
		if ( lib.TIFFGetField == NULL ) {
			SDL_UnloadObject(lib.handle);
			return -1;
		}
		lib.TIFFReadRGBAImage =
			(int (*)(TIFF*, uint32, uint32, uint32*, int))
			SDL_LoadFunction(lib.handle, "TIFFReadRGBAImage");
		if ( lib.TIFFReadRGBAImage == NULL ) {
			SDL_UnloadObject(lib.handle);
			return -1;
		}
		lib.TIFFSetErrorHandler =
			(TIFFErrorHandler (*)(TIFFErrorHandler))
			SDL_LoadFunction(lib.handle, "TIFFSetErrorHandler");
		if ( lib.TIFFSetErrorHandler == NULL ) {
			SDL_UnloadObject(lib.handle);
			return -1;
		}
	}
	++lib.loaded;

	return 0;
}
void IMG_QuitTIF()
{
	if ( lib.loaded == 0 ) {
		return;
	}
	if ( lib.loaded == 1 ) {
		SDL_UnloadObject(lib.handle);
	}
	--lib.loaded;
}
#else
int IMG_InitTIF()
{
	if ( lib.loaded == 0 ) {
		lib.TIFFClientOpen = TIFFClientOpen;
		lib.TIFFClose = TIFFClose;
		lib.TIFFGetField = TIFFGetField;
		lib.TIFFReadRGBAImage = TIFFReadRGBAImage;
		lib.TIFFSetErrorHandler = TIFFSetErrorHandler;
	}
	++lib.loaded;

	return 0;
}
void IMG_QuitTIF()
{
	if ( lib.loaded == 0 ) {
		return;
	}
	if ( lib.loaded == 1 ) {
	}
	--lib.loaded;
}
#endif /* LOAD_TIF_DYNAMIC */

/*
 * These are the thunking routine to use the SDL_RWops* routines from
 * libtiff's internals.
*/

static tsize_t tiff_read(thandle_t fd, tdata_t buf, tsize_t size)
{
	return SDL_RWread((SDL_RWops*)fd, buf, 1, size);
}

static toff_t tiff_seek(thandle_t fd, toff_t offset, int origin)
{
	return SDL_RWseek((SDL_RWops*)fd, offset, origin);
}

static tsize_t tiff_write(thandle_t fd, tdata_t buf, tsize_t size)
{
	return SDL_RWwrite((SDL_RWops*)fd, buf, 1, size);
}

static int tiff_close(thandle_t fd)
{
	/*
	 * We don't want libtiff closing our SDL_RWops*, but if it's not given
         * a routine to try, and if the image isn't a TIFF, it'll segfault.
	 */
	return 0;
}

static int tiff_map(thandle_t fd, tdata_t* pbase, toff_t* psize)
{
	return (0);
}

static void tiff_unmap(thandle_t fd, tdata_t base, toff_t size)
{
	return;
}

static toff_t tiff_size(thandle_t fd)
{
	Uint32 save_pos;
	toff_t size;

	save_pos = SDL_RWtell((SDL_RWops*)fd);
	SDL_RWseek((SDL_RWops*)fd, 0, RW_SEEK_END);
        size = SDL_RWtell((SDL_RWops*)fd);
	SDL_RWseek((SDL_RWops*)fd, save_pos, RW_SEEK_SET);
	return size;
}

int IMG_isTIF(SDL_RWops* src)
{
	int start;
	int is_TIF;
	Uint8 magic[4];

	if ( !src )
		return 0;
	start = SDL_RWtell(src);
	is_TIF = 0;
	if ( SDL_RWread(src, magic, 1, sizeof(magic)) == sizeof(magic) ) {
		if ( (magic[0] == 'I' &&
                      magic[1] == 'I' &&
		      magic[2] == 0x2a &&
                      magic[3] == 0x00) ||
		     (magic[0] == 'M' &&
                      magic[1] == 'M' &&
		      magic[2] == 0x00 &&
                      magic[3] == 0x2a) ) {
			is_TIF = 1;
		}
	}
	SDL_RWseek(src, start, RW_SEEK_SET);
	return(is_TIF);
}

SDL_Surface* IMG_LoadTIF_RW(SDL_RWops* src)
{
	int start;
	TIFF* tiff;
	SDL_Surface* surface = NULL;
	Uint32 img_width, img_height;
	Uint32 Rmask, Gmask, Bmask, Amask;
	Uint32 x, y;
	Uint32 half;

	if ( !src ) {
		/* The error message has been set in SDL_RWFromFile */
		return NULL;
	}
	start = SDL_RWtell(src);

	if ( !IMG_Init(IMG_INIT_TIF) ) {
		return NULL;
	}

	/* turn off memory mapped access with the m flag */
	tiff = lib.TIFFClientOpen("SDL_image", "rm", (thandle_t)src, 
		tiff_read, tiff_write, tiff_seek, tiff_close, tiff_size, tiff_map, tiff_unmap);
	if(!tiff)
		goto error;

	/* Retrieve the dimensions of the image from the TIFF tags */
	lib.TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &img_width);
	lib.TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &img_height);

	Rmask = 0x000000FF;
	Gmask = 0x0000FF00;
	Bmask = 0x00FF0000;
	Amask = 0xFF000000;
	surface = SDL_AllocSurface(SDL_SWSURFACE, img_width, img_height, 32,
		Rmask, Gmask, Bmask, Amask);
	if(!surface)
		goto error;
	
	if(!lib.TIFFReadRGBAImage(tiff, img_width, img_height, surface->pixels, 0))
		goto error;

	/* libtiff loads the image upside-down, flip it back */
	half = img_height / 2;
	for(y = 0; y < half; y++)
	{
	        Uint32 *top = (Uint32 *)surface->pixels + y * surface->pitch/4;
	        Uint32 *bot = (Uint32 *)surface->pixels
		              + (img_height - y - 1) * surface->pitch/4;
		for(x = 0; x < img_width; x++)
		{
		        Uint32 tmp = top[x];
			top[x] = bot[x];
			bot[x] = tmp;
		}
	}
	lib.TIFFClose(tiff);
	
	return surface;

error:
	SDL_RWseek(src, start, RW_SEEK_SET);
	if ( surface ) {
		SDL_FreeSurface(surface);
	}
	return NULL;
}

#else

int IMG_InitTIF()
{
	IMG_SetError("TIFF images are not supported");
	return(-1);
}

void IMG_QuitTIF()
{
}

/* See if an image is contained in a data source */
int IMG_isTIF(SDL_RWops *src)
{
	return(0);
}

/* Load a TIFF type image from an SDL datasource */
SDL_Surface *IMG_LoadTIF_RW(SDL_RWops *src)
{
	return(NULL);
}

#endif /* LOAD_TIF */

#endif /* !defined(__APPLE__) || defined(SDL_IMAGE_USE_COMMON_BACKEND) */
