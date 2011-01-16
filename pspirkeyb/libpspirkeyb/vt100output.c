/*
    libpspirkeyb: PSP IR Keyboard Library (http://pspirkeyb.podzone.net)
    Copyright (C) 2007 Harald Fielker <harald.fielker@web.de>

    This program can be distributed under the terms of the GNU LGPL.
    See the file LICENSE.
*/

#include <pspkernel.h>
#include <pspdebug.h>

#include <stdlib.h>
#include <string.h>
#include "pspirkeyb_rawkeys.h"
#include "vt100output.h"


#define printf pspDebugScreenPrintf

struct Vt100Entry
{
    unsigned char raw;
    char shift;
    char ctrl;
    char *sequence;
    unsigned char c;
};

/*
 *          -1 = don't care ... 0 = not set ... 1 = set
 */
static struct Vt100Entry g_vt100_map[] =
{
    /*key*/             /*shift*/            /*ctrl*/        /*sequence*/           /* char code */
    { KEY_UP,           -1,                  -1,              "\e[A",               0x00    },
    { KEY_DOWN,         -1,                  -1,              "\e[B",               0x00     },
    { KEY_RIGHT,        -1,                  -1,              "\e[C",               0x00     },
    { KEY_LEFT,         -1,                  -1,              "\e[D",               0x00     },

    { KEY_ENTER,        -1,                  -1,              NULL,                 0x0D     },
    { KEY_KPENTER,      -1,                  -1,              NULL,                 0x0D     },
    { KEY_BACKSPACE,    -1,                  -1,              NULL,                 0x08     },

    { KEY_INSERT,       -1,                  -1,              "\e[[2~",               0x00     },
    { KEY_DELETE,       -1,                  -1,              "\e[[3~",               0x00     },
    { KEY_END,          -1,                  -1,              "\e[OF",                0x00      },
    { KEY_HOME,         -1,                  -1,              "\e[OH~",               0x00     },

    { KEY_PAGEUP,       -1,                  -1,              "\e[[5~",               0x00     },
    { KEY_PAGEDOWN,     -1,                  -1,              "\e[[6~",               0x00     },

    { KEY_TAB,           0,                  -1,              NULL,                  0x09     },
    { KEY_TAB,           1,                  -1,              "\e[[Z",               0x00     },

    { KEY_A,            -1,                   1,              NULL,                  0x01     },
    { KEY_B,            -1,                   1,              NULL,                  0x02     },
    { KEY_C,            -1,                   1,              NULL,                  0x03     },
    { KEY_D,            -1,                   1,              NULL,                  0x04     },
    { KEY_E,            -1,                   1,              NULL,                  0x05     },
    { KEY_F,            -1,                   1,              NULL,                  0x06     },
    { KEY_G,            -1,                   1,              NULL,                  0x07     },
    { KEY_H,            -1,                   1,              NULL,                  0x08     },
    { KEY_I,            -1,                   1,              NULL,                  0x09     },
    { KEY_J,            -1,                   1,              NULL,                  0x0A     },
    { KEY_K,            -1,                   1,              NULL,                  0x0B     },
    { KEY_L,            -1,                   1,              NULL,                  0x0C     },
    { KEY_M,            -1,                   1,              NULL,                  0x0D     },
    { KEY_N,            -1,                   1,              NULL,                  0x0E     },
    { KEY_O,            -1,                   1,              NULL,                  0x0F     },
    { KEY_P,            -1,                   1,              NULL,                  0x10     },
    { KEY_Q,            -1,                   1,              NULL,                  0x11     },
    { KEY_R,            -1,                   1,              NULL,                  0x12     },
    { KEY_S,            -1,                   1,              NULL,                  0x13     },
    { KEY_T,            -1,                   1,              NULL,                  0x14     },
    { KEY_U,            -1,                   1,              NULL,                  0x15     },
    { KEY_V,            -1,                   1,              NULL,                  0x16     },
    { KEY_W,            -1,                   1,              NULL,                  0x17     },
    { KEY_X,            -1,                   1,              NULL,                  0x18     },
    { KEY_Y,            -1,                   1,              NULL,                  0x19     },
    { KEY_Z,            -1,                   1,              NULL,                  0x1A     },

    { KEY_SPACE,        -1,                   0,              " ",                   0x00     },
    { KEY_ESC,          -1,                  -1,              NULL,                  0x1B     },

    { KEY_F1,           -1,                   0,              "\e[OP",               0x00     },
    { KEY_F2,           -1,                   0,              "\e[OQ",               0x00     },
    { KEY_F3,           -1,                   0,              "\e[OR",               0x00     },
    { KEY_F4,           -1,                   0,              "\e[OS",               0x00     },
};

static void vt100_map( unsigned char raw, unsigned char shift, unsigned char ctrl, unsigned char alt, unsigned char * buffer, int *length );

void vt100_output_map( unsigned char *data, int size, unsigned char is_rawcode, unsigned char shift, unsigned char ctrl, unsigned char alt, unsigned char pressed, unsigned char * buffer, int *length )
{
    if( !buffer || !length || !data || !pressed || size < 1 )
        return;

    /* now rawcode */
    if( !is_rawcode )
    {
        buffer += (*length);
        memcpy(buffer, data, size );
        (*length) += size;
        return;
    }

    if( size != 1 )
        return;

    vt100_map( *data, shift, ctrl, alt, buffer, length );
}


static void vt100_map( unsigned char raw, unsigned char shift, unsigned char ctrl, unsigned char alt, unsigned char * buffer, int *length )
{
    struct Vt100Entry * entry;
    int count = sizeof( g_vt100_map ) / sizeof( struct Vt100Entry );
    int i, len;

    /* we don't care about left/right modifiers - only down */
    if( shift > 0 )
        shift = 1;
    if( ctrl > 0 )
        ctrl = 1;


    for( i=0; i < count; i++ )
    {
        entry = g_vt100_map + i;

        if( entry->raw != raw )
            continue;
        if( entry->shift > -1 && entry->shift != shift )
                continue;
        if( entry->ctrl > -1 && entry->ctrl != ctrl )
                continue;

        buffer += (*length);
        
        if( entry->sequence ) {
            /* add sequence */
            len = strlen(entry->sequence);       
            memcpy(buffer, entry->sequence, len);
        } else {
            /* add single char */
            len = 1;
            *buffer=entry->c;
        }

        (*length) += len;

        break;
    }
}

