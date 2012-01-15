/*
    showfont:  An example of using the SDL_ttf library with 2D graphics.
    Copyright (C) 1997-2009 Sam Lantinga

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

/* $Id: showfont.c 5141 2009-10-18 20:47:04Z slouken $ */

/* A simple program to test the text rendering feature of the TTF library */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef HAVE_ICONV
#include <iconv.h>
#endif

#include "SDL.h"
#include "SDL_ttf.h"

#define DEFAULT_PTSIZE	18
#define DEFAULT_TEXT	"The quick brown fox jumped over the lazy dog"
#define NUM_COLORS      256

static char *Usage =
"Usage: %s [-solid] [-utf8|-unicode] [-b] [-i] [-u] [-s] [-outline size] [-hintlight|-hintmono|-hintnone] [-nokerning] [-fgcol r,g,b] [-bgcol r,g,b] <font>.ttf [ptsize] [text]\n";

static void cleanup(int exitcode)
{
	TTF_Quit();
	SDL_Quit();
	exit(exitcode);
}

int main(int argc, char *argv[])
{
	char *argv0 = argv[0];
	SDL_Surface *screen;
	TTF_Font *font;
	SDL_Surface *text, *temp;
	int ptsize;
	int i, done;
	int rdiff, gdiff, bdiff;
	SDL_Color colors[NUM_COLORS];
	SDL_Color white = { 0xFF, 0xFF, 0xFF, 0 };
	SDL_Color black = { 0x00, 0x00, 0x00, 0 };
	SDL_Color *forecol;
	SDL_Color *backcol;
	SDL_Rect dstrect;
	SDL_Event event;
	int rendersolid;
	int renderstyle;
	int outline;
	int hinting;
	int kerning;
	int dump;
	enum {
		RENDER_LATIN1,
		RENDER_UTF8,
		RENDER_UNICODE
	} rendertype;
	char *message, string[128];

	/* Look for special execution mode */
	dump = 0;
	/* Look for special rendering types */
	rendersolid = 0;
	renderstyle = TTF_STYLE_NORMAL;
	rendertype = RENDER_LATIN1;
	outline = 0;
	hinting = TTF_HINTING_NORMAL;
	kerning = 1;
	/* Default is black and white */
	forecol = &black;
	backcol = &white;
	for ( i=1; argv[i] && argv[i][0] == '-'; ++i ) {
		if ( strcmp(argv[i], "-solid") == 0 ) {
			rendersolid = 1;
		} else
		if ( strcmp(argv[i], "-utf8") == 0 ) {
			rendertype = RENDER_UTF8;
		} else
		if ( strcmp(argv[i], "-unicode") == 0 ) {
			rendertype = RENDER_UNICODE;
		} else
		if ( strcmp(argv[i], "-b") == 0 ) {
			renderstyle |= TTF_STYLE_BOLD;
		} else
		if ( strcmp(argv[i], "-i") == 0 ) {
			renderstyle |= TTF_STYLE_ITALIC;
		} else
		if ( strcmp(argv[i], "-u") == 0 ) {
			renderstyle |= TTF_STYLE_UNDERLINE;
		} else
		if ( strcmp(argv[i], "-s") == 0 ) {
			renderstyle |= TTF_STYLE_STRIKETHROUGH;
		} else
		if ( strcmp(argv[i], "-outline") == 0 ) {
			if ( sscanf (argv[++i], "%d", &outline) != 1 ) {
				fprintf(stderr, Usage, argv0);
				return(1);
			}
		} else
		if ( strcmp(argv[i], "-hintlight") == 0 ) {
			kerning = TTF_HINTING_LIGHT;
		} else
		if ( strcmp(argv[i], "-hintmono") == 0 ) {
			kerning = TTF_HINTING_MONO;
		} else
		if ( strcmp(argv[i], "-hintnone") == 0 ) {
			kerning = TTF_HINTING_NONE;
		} else
		if ( strcmp(argv[i], "-nokerning") == 0 ) {
			kerning = 0;
		} else
		if ( strcmp(argv[i], "-dump") == 0 ) {
			dump = 1;
		} else
		if ( strcmp(argv[i], "-fgcol") == 0 ) {
			int r, g, b;
			if ( sscanf (argv[++i], "%d,%d,%d", &r, &g, &b) != 3 ) {
				fprintf(stderr, Usage, argv0);
				return(1);
			}
			forecol->r = (Uint8)r;
			forecol->g = (Uint8)g;
			forecol->b = (Uint8)b;
		} else
		if ( strcmp(argv[i], "-bgcol") == 0 ) {
			int r, g, b;
			if ( sscanf (argv[++i], "%d,%d,%d", &r, &g, &b) != 3 ) {
				fprintf(stderr, Usage, argv0);
				return(1);
			}
			backcol->r = (Uint8)r;
			backcol->g = (Uint8)g;
			backcol->b = (Uint8)b;
		} else {
			fprintf(stderr, Usage, argv0);
			return(1);
		}
	}
	argv += i;
	argc -= i;

	/* Check usage */
	if ( ! argv[0] ) {
		fprintf(stderr, Usage, argv0);
		return(1);
	}

	/* Initialize SDL */
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		return(2);
	}

	/* Initialize the TTF library */
	if ( TTF_Init() < 0 ) {
		fprintf(stderr, "Couldn't initialize TTF: %s\n",SDL_GetError());
		SDL_Quit();
		return(2);
	}

	/* Open the font file with the requested point size */
	ptsize = 0;
	if ( argc > 1 ) {
		ptsize = atoi(argv[1]);
	}
	if ( ptsize == 0 ) {
		i = 2;
		ptsize = DEFAULT_PTSIZE;
	} else {
		i = 3;
	}
	font = TTF_OpenFont(argv[0], ptsize);
	if ( font == NULL ) {
		fprintf(stderr, "Couldn't load %d pt font from %s: %s\n",
					ptsize, argv[0], SDL_GetError());
		cleanup(2);
	}
	TTF_SetFontStyle(font, renderstyle);
	TTF_SetFontOutline(font, outline);
	TTF_SetFontKerning(font, kerning);

	if( dump ) {
		for( i = 48; i < 123; i++ ) {
			SDL_Surface* glyph = NULL;

			glyph = TTF_RenderGlyph_Shaded( font, i, *forecol, *backcol );

			if( glyph ) {
				char outname[64];
				sprintf( outname, "glyph-%d.bmp", i );
				SDL_SaveBMP( glyph, outname );
			}

		}
		cleanup(0);
	}

	/* Set a 640x480x8 video mode */
	screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n",
							SDL_GetError());
		cleanup(2);
	}

	/* Set a palette that is good for the foreground colored text */
	rdiff = backcol->r - forecol->r;
	gdiff = backcol->g - forecol->g;
	bdiff = backcol->b - forecol->b;
	for ( i=0; i<NUM_COLORS; ++i ) {
		colors[i].r = forecol->r + (i*rdiff)/4;
		colors[i].g = forecol->g + (i*gdiff)/4;
		colors[i].b = forecol->b + (i*bdiff)/4;
	}
	SDL_SetColors(screen, colors, 0, NUM_COLORS);

	/* Clear the background to background color */
	SDL_FillRect(screen, NULL,
			SDL_MapRGB(screen->format, backcol->r, backcol->g, backcol->b));
	SDL_UpdateRect(screen, 0, 0, 0, 0);

	/* Show which font file we're looking at */
	sprintf(string, "Font file: %s", argv[0]);  /* possible overflow */
	if ( rendersolid ) {
		text = TTF_RenderText_Solid(font, string, *forecol);
	} else {
		text = TTF_RenderText_Shaded(font, string, *forecol, *backcol);
	}
	if ( text != NULL ) {
		dstrect.x = 4;
		dstrect.y = 4;
		dstrect.w = text->w;
		dstrect.h = text->h;
		SDL_BlitSurface(text, NULL, screen, &dstrect);
		SDL_FreeSurface(text);
	}
	
	/* Render and center the message */
	if ( argc > 2 ) {
		message = argv[2];
	} else {
		message = DEFAULT_TEXT;
	}
	switch (rendertype) {
	    case RENDER_LATIN1:
		if ( rendersolid ) {
			text = TTF_RenderText_Solid(font,message,*forecol);
		} else {
			text = TTF_RenderText_Shaded(font,message,*forecol,*backcol);
		}
		break;

	    case RENDER_UTF8:
		if ( rendersolid ) {
			text = TTF_RenderUTF8_Solid(font,message,*forecol);
		} else {
			text = TTF_RenderUTF8_Shaded(font,message,*forecol,*backcol);
		}
		break;

	    case RENDER_UNICODE:
		{
			Uint16 unicode_text[BUFSIZ];
			int index;
#ifdef HAVE_ICONV
			/* Use iconv to convert the message into utf-16.
			 * "char" and "" are aliases for the local 8-bit encoding */
			iconv_t cd;
			/*ICONV_CONST*/ char *from_str = message;
			char *to_str = (char*)unicode_text;
			size_t from_sz = strlen(message) + 1;
			size_t to_sz = sizeof(unicode_text);
			size_t res;
			int i;

			if ((cd = iconv_open("UTF-16", "char")) == (iconv_t)-1
			    && (cd = iconv_open("UTF-16", "")) == (iconv_t)-1) {
				perror("Couldn't open iconv");
				exit(1);
			}

			res = iconv(cd, &from_str, &from_sz, &to_str, &to_sz);
			if (res == -1) {
				perror("Couldn't use iconv");
				exit(1);
			}

			iconv_close(cd);
#else
			/* Convert the message from ascii into utf-16.
			 * This is unreliable as a test because it always
			 * gives the local ordering. */
			for (index = 0; message[index]; index++) {
				unicode_text[index] = message[index];
			}
			unicode_text[index] = 0;
#endif

			if ( rendersolid ) {
				text = TTF_RenderUNICODE_Solid(font,
					unicode_text, *forecol);
			} else {
				text = TTF_RenderUNICODE_Shaded(font,
					unicode_text, *forecol, *backcol);
			}
		}
		break;
	    default:
		text = NULL; /* This shouldn't happen */
		break;
	}
	if ( text == NULL ) {
		fprintf(stderr, "Couldn't render text: %s\n", SDL_GetError());
		TTF_CloseFont(font);
		cleanup(2);
	}
	dstrect.x = (screen->w - text->w)/2;
	dstrect.y = (screen->h - text->h)/2;
	dstrect.w = text->w;
	dstrect.h = text->h;
	printf("Font is generally %d big, and string is %hd big\n",
						TTF_FontHeight(font), text->h);

	/* Blit the text surface */
	if ( SDL_BlitSurface(text, NULL, screen, &dstrect) < 0 ) {
		fprintf(stderr, "Couldn't blit text to display: %s\n", 
								SDL_GetError());
		TTF_CloseFont(font);
		cleanup(2);
	}
	SDL_UpdateRect(screen, 0, 0, 0, 0);

	/* Set the text colorkey and convert to display format */
	if ( SDL_SetColorKey(text, SDL_SRCCOLORKEY|SDL_RLEACCEL, 0) < 0 ) {
		fprintf(stderr, "Warning: Couldn't set text colorkey: %s\n",
								SDL_GetError());
	}
	temp = SDL_DisplayFormat(text);
	if ( temp != NULL ) {
		SDL_FreeSurface(text);
		text = temp;
	}

	/* Wait for a keystroke, and blit text on mouse press */
	done = 0;
	while ( ! done ) {
		if ( SDL_WaitEvent(&event) < 0 ) {
			fprintf(stderr, "SDL_PullEvent() error: %s\n",
								SDL_GetError());
			done = 1;
			continue;
		}
		switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				dstrect.x = event.button.x - text->w/2;
				dstrect.y = event.button.y - text->h/2;
				dstrect.w = text->w;
				dstrect.h = text->h;
				if ( SDL_BlitSurface(text, NULL, screen,
							&dstrect) == 0 ) {
					SDL_UpdateRects(screen, 1, &dstrect);
				} else {
					fprintf(stderr,
					"Couldn't blit text to display: %s\n", 
								SDL_GetError());
				}
				break;
				
			case SDL_KEYDOWN:
			case SDL_QUIT:
				done = 1;
				break;
			default:
				break;
		}
	}
	SDL_FreeSurface(text);
	TTF_CloseFont(font);
	cleanup(0);

	/* Not reached, but fixes compiler warnings */
	return 0;
}
