/*
    libpspirkeyb: PSP IR Keyboard Library (http://pspirkeyb.podzone.net)
    Copyright (C) 2007 Harald Fielker <harald.fielker@web.de>

    This program can be distributed under the terms of the GNU LGPL.
    See the file LICENSE.
*/

#include <pspkernel.h>
#include <pspdebug.h>

#include <stdlib.h>
#include "iniparser.h"
#include "pspirkeyb.h"
#include "pspirkeyb_rawkeys.h"
#include "keymap.h"
#include "maptable.h"
#include "vt100output.h"

//#define DEBUG_MODIFIERS                         1
//#define printf pspDebugScreenPrintf

/* possible states */
#define     STATE_NONE      0
#define     STATE_DOWN      1
#define     STATE_LEFT      2
#define     STATE_RIGHT     3

#define     KEY_PRESSED     1

struct ModifierState
{
    unsigned char shift;
    unsigned char ctrl;
    unsigned char alt;
    unsigned char meta;
    unsigned char compose;
    unsigned char numlock;
    unsigned char capslock;
};

struct ModifierState g_modifierState;

extern int g_lastirkey;

static void keymap_decode_raw(  unsigned char raw, unsigned char pressed, unsigned char * buffer, int *length );
static void keymap_decode_maptable( int outputmode, unsigned char raw, unsigned char pressed, unsigned char * buffer, int *length );
static void keymap_decode_maptable_rec( int depth, int outputmode, unsigned char raw, unsigned char pressed, unsigned char * buffer, int *length );

static void decoded_to_buffer( int outputmode, unsigned char *data, int size, unsigned char is_rawcode, unsigned char shift, unsigned char ctrl, unsigned char alt, unsigned char pressed, unsigned char * buffer, int *length );
static void decoded_to_buffer_with_map( int outputmode, unsigned char raw, struct MaptableEntry *entry, unsigned char shift, unsigned char ctrl, unsigned char alt, unsigned char pressed, unsigned char * buffer, int *length );

static int decode_modifier( struct ModifierState *state, unsigned char raw, int pressed );

#ifdef DEBUG_MODIFIERS
static void dump_modifier( struct ModifierState *state );
#endif

/* reset state e.g. after power resume */
void keymap_reset_state()
{
    memset( &g_modifierState, 0, sizeof( struct ModifierState ) );
}

/* decoder for keyboard raw keys */
void keymap_decode( int outputmode, unsigned char raw, unsigned char pressed, unsigned char * buffer, int *length )
{
    if( !buffer || !length )
        return;

	if (pressed == KEY_PRESSED)
		g_lastirkey = raw;
	else
		g_lastirkey = -1;

    switch ( outputmode )
    {
        case PSP_IRKBD_OUTPUT_MODE_RAW:
            keymap_decode_raw( raw, pressed, buffer, length );
            break;
        case PSP_IRKBD_OUTPUT_MODE_SCANCODE:
        case PSP_IRKBD_OUTPUT_MODE_ASCII:
        case PSP_IRKBD_OUTPUT_MODE_VT100:
            keymap_decode_maptable( outputmode, raw, pressed, buffer, length );
            break;
        default:
            break;
    }
    return;
}

/* decodes to raw */
static void keymap_decode_raw( unsigned char raw, unsigned char pressed, unsigned char * buffer, int *length )
{
    unsigned char shift=0, alt=0, ctrl=0, is_rawcode=1;

    decoded_to_buffer( PSP_IRKBD_OUTPUT_MODE_RAW, &raw, sizeof( raw), is_rawcode, shift, ctrl, alt, pressed, buffer, length );
}

/* decodes with modifiers and maptable */
static void keymap_decode_maptable( int outputmode, unsigned char raw, unsigned char pressed, unsigned char * buffer, int *length )
{
    keymap_decode_maptable_rec( 0, outputmode, raw, pressed, buffer, length );
}

/* decodes with modifiers and maptable recursion */
static void keymap_decode_maptable_rec( int depth, int outputmode, unsigned char raw, unsigned char pressed, unsigned char * buffer, int *length )
{
    unsigned char normal, shift=0, alt=0, ctrl=0, caps, compose, meta, numlock, is_rawcode;
    struct ModifierState oldModifierState;
    struct Maptable *map = NULL;

    /* handle bad maps */
    if( depth > 3 )
        return;

    /* save global modifier table */
    memcpy( &oldModifierState, &g_modifierState, sizeof( struct ModifierState ) );

    /* check for modifiers (modify global table) */
    decode_modifier( &g_modifierState, raw, pressed );

    /* setup the effective modifiers */
    shift = g_modifierState.shift;
    alt = g_modifierState.alt;
    ctrl = g_modifierState.ctrl;
    compose = g_modifierState.compose;
    meta = g_modifierState.meta;
    numlock = g_modifierState.numlock;
		caps = g_modifierState.capslock;

#ifdef DEBUG_MODIFIERS
    dump_modifier( &g_modifierState );
#endif

    /* capslock handling - note that it is only like shift for alphas */
    if( caps )
    {
      if (((raw >= KEY_Q) && (raw <= KEY_P)) ||
					((raw >= KEY_A) && (raw <= KEY_L)) ||
					((raw >= KEY_Z) && (raw <= KEY_M)))
			{
				if (shift)
				{
					/* no caps for shift+capslock */
					shift = STATE_NONE;
				}
				else
				{
					/* force caps by pretending shift */
					shift = STATE_DOWN;
				}
			}
		}

    /* check if we have a maptable */
    map = maptable_entry( raw );
    if( map == NULL )
    {
        /* no! just write to buffer */
        is_rawcode = 1;
        decoded_to_buffer( outputmode, &raw, sizeof( raw ), is_rawcode, shift, ctrl, alt, pressed, buffer, length );
        return;
    }

    /* normal will be zero if no other modifier is down */
    normal = shift + alt + ctrl + compose + meta + numlock;


#define DO_DECODE(index) \
    { \
        if( map->e[index].is_rawcode == 0 ) { \
            decoded_to_buffer_with_map( outputmode, raw, &(map->e[index]), shift, ctrl, alt, pressed, buffer, length ); \
        } \
        else \
        { \
            if( map->e[index].length > 0 ) { \
                /* restore old state */ \
                memcpy( &g_modifierState, &oldModifierState, sizeof( struct ModifierState ) ); \
                /* recursion */ \
                keymap_decode_maptable_rec( depth +1, outputmode, *(map->e[index].map), pressed, buffer, length ); \
            } else { \
                is_rawcode = 1; \
                decoded_to_buffer( outputmode, &raw, sizeof( raw ), is_rawcode, shift, ctrl, alt, pressed, buffer, length ); \
            }\
        } \
    }

    /* no effective modifiers */
    if( normal == 0 )
        DO_DECODE(MAPTABLE_SECT_NORMAL);
    if( shift )
        DO_DECODE(MAPTABLE_SECT_SHIFT);
    if( ctrl )
        DO_DECODE(MAPTABLE_SECT_CTRL);
    if( alt )
        DO_DECODE(MAPTABLE_SECT_ALT);
    if( compose )
        DO_DECODE(MAPTABLE_SECT_COMPOSE);
    if( meta )
        DO_DECODE(MAPTABLE_SECT_META);
    if( numlock )
        DO_DECODE(MAPTABLE_SECT_NUM);

#undef DO_ENCODE
}


/* inserts decoded result to buffer */
static void decoded_to_buffer( int outputmode, unsigned char *data, int size, unsigned char is_rawcode, unsigned char shift, unsigned char ctrl, unsigned char alt, unsigned char pressed, unsigned char * buffer, int *length )
{
    char buf[4096];
    int len=0;

    if( outputmode == PSP_IRKBD_OUTPUT_MODE_RAW ) {
        SIrKeybRawCodeData * rawData;

        buffer += (*length);

        *length += sizeof(SIrKeybRawCodeData);
        rawData = (SIrKeybRawCodeData*) buffer;

        rawData->raw = *data;
        rawData->pressed = pressed;

    }
    else if( outputmode == PSP_IRKBD_OUTPUT_MODE_SCANCODE )
    {
        SIrKeybScanCodeData *scanData;

        buffer += (*length);

        *length += sizeof(SIrKeybScanCodeData);
        scanData = (SIrKeybScanCodeData*) buffer;

        scanData->raw = *data;
        scanData->shift = shift;
        scanData->ctrl = ctrl;
        scanData->alt = alt;
        scanData->pressed = pressed;
    }
    else if ( outputmode == PSP_IRKBD_OUTPUT_MODE_ASCII && size == 1 && pressed  )
    {
        /* we only support PSP sdk current charset - no backspace etc. */
        switch( *data )
        {
            case KEY_TAB:
                if( !shift ) {
                    buf[0] = '\t';
                    len = 1;
                }
                break;
            case KEY_SPACE:
                buf[0] = ' ';
                len = 1;
                break;
            case KEY_ENTER:
            case KEY_KPENTER:
                buf[0] = '\n';
                len = 1;
                break;
            default:
                break;
        }
        if( len > 0 ) {
            memcpy(buffer, buf, len );
            (*length) += len;
        }
    }
    else if( outputmode == PSP_IRKBD_OUTPUT_MODE_VT100 && size == 1 && pressed )
    {
        vt100_output_map( data, size, is_rawcode, shift, ctrl, alt, pressed, buffer, length );
    }
}

static void decoded_to_buffer_with_map( int outputmode, unsigned char raw, struct MaptableEntry *entry, unsigned char shift, unsigned char ctrl, unsigned char alt, unsigned char pressed, unsigned char * buffer, int *length )
{
    unsigned char is_rawcode=entry->is_rawcode;
/*
    printf( "raw: %s\n", pspIrKeybRawkeyName( raw ) );
    printf( "entry size: %d\n", entry->length );
    printf( "entry is raw: %d\n", entry->is_rawcode );
    if( entry->length > 0 )
        printf( "entry mapped raw: %s\n", pspIrKeybRawkeyName( *(entry->map) ) );
*/
    if( outputmode == PSP_IRKBD_OUTPUT_MODE_SCANCODE )
    {
        /* we don't need the map in scancode mode */
        SIrKeybScanCodeData *scanData;

        buffer += (*length);

        *length += sizeof(SIrKeybScanCodeData);
        scanData = (SIrKeybScanCodeData*) buffer;

        if( is_rawcode )
            scanData->raw = *(entry->map);
        else
            scanData->raw = raw;

        scanData->shift = shift;
        scanData->ctrl = ctrl;
        scanData->alt = alt;
        scanData->pressed = pressed;
    }
    else if( outputmode == PSP_IRKBD_OUTPUT_MODE_ASCII && pressed )
    {
        buffer += (*length);
        if( entry->length > 0 )
        {
            memcpy(buffer, entry->map, entry->length );
            (*length) += entry->length;
        }
    }
    else if( outputmode == PSP_IRKBD_OUTPUT_MODE_VT100 && pressed )
    {
        if( is_rawcode ) {
            vt100_output_map( &raw, sizeof( unsigned char), is_rawcode, shift, ctrl, alt, pressed, buffer, length );
        } else {
            if( entry->length > 0 ) {
                vt100_output_map( entry->map, entry->length, is_rawcode, shift, ctrl, alt, pressed, buffer, length );
            } else {

                /* we have no entry here - so use the raw code */
                is_rawcode = 1;
                vt100_output_map( &raw, sizeof( unsigned char), is_rawcode, shift, ctrl, alt, pressed, buffer, length );
            }
        }
    }
}


static int decode_modifier( struct ModifierState *state, unsigned char raw, int pressed )
{
    if( !state )
        return 0;

    switch( raw )
    {
        case KEY_LEFTSHIFT:
            if( !pressed ) state->shift = STATE_NONE;
            else state->shift = STATE_LEFT;
            return 1;
        case KEY_RIGHTSHIFT:
            if( !pressed ) state->shift = STATE_NONE;
            else state->shift = STATE_RIGHT;
            return 1;
        case KEY_LEFTCTRL:
            if( !pressed ) state->ctrl = STATE_NONE;
            else state->ctrl = STATE_LEFT;
            return 1;
        case KEY_RIGHTCTRL:
            if( !pressed ) state->ctrl = STATE_NONE;
            else state->ctrl = STATE_RIGHT;
            return 1;
        case KEY_LEFTALT:
            if( !pressed ) state->alt = STATE_NONE;
            else state->alt = STATE_LEFT;
            return 1;
        case KEY_RIGHTALT:
            if( !pressed ) state->alt = STATE_NONE;
            else state->alt = STATE_RIGHT;
            return 1;
        case KEY_LEFTMETA:
            if( !pressed ) state->meta = STATE_NONE;
            else state->meta = STATE_LEFT;
            return 1;
        case KEY_RIGHTMETA:
            if( !pressed ) state->meta = STATE_NONE;
            else state->meta = STATE_RIGHT;
            return 1;
        case KEY_COMPOSE:
            if( !pressed ) state->compose = STATE_NONE;
            else state->compose = STATE_DOWN;
            return 1;
        case KEY_NUMLOCK:
            if( !pressed )
                return 0;
            if( state->numlock )
                state->numlock = STATE_NONE;
            else
                state->numlock = STATE_DOWN;
            return 1;
        case KEY_CAPSLOCK:
            if( !pressed )
                return 0;
            if( state->capslock )
                state->capslock = STATE_NONE;
            else
                state->capslock = STATE_DOWN;
            return 1;
        default:
            break;
    }

    return 0;
}
#ifdef DEBUG_MODIFIERS
static void dump_modifier( struct ModifierState *state )
{
    char buffer[4096];
    if( state == NULL )
        return;
    buffer[0] = '\0';

    if( state->shift == STATE_DOWN )
        strcat(buffer, " SHIFT " );
    if( state->shift == STATE_LEFT )
        strcat(buffer, " SHIFTLEFT " );
    if( state->shift == STATE_RIGHT )
        strcat(buffer, " SHIFTRIGHT " );

    if( state->ctrl == STATE_DOWN )
        strcat(buffer, " CTRL " );
    if( state->ctrl == STATE_LEFT )
        strcat(buffer, " CTRLLEFT " );
    if( state->ctrl == STATE_RIGHT )
        strcat(buffer, " CTRLRIGHT " );

    if( state->alt == STATE_DOWN )
        strcat(buffer, " ALT " );
    if( state->alt == STATE_LEFT )
        strcat(buffer, " ALTLEFT " );
    if( state->alt == STATE_RIGHT )
        strcat(buffer, " ALTRIGHT " );

    if( state->meta == STATE_DOWN )
        strcat(buffer, " META " );
    if( state->meta == STATE_LEFT )
        strcat(buffer, " METALEFT " );
    if( state->meta == STATE_RIGHT )
        strcat(buffer, " METARIGHT " );

    if( state->compose == STATE_DOWN )
        strcat(buffer, " COMPOSE " );
    if( state->compose == STATE_LEFT )
        strcat(buffer, " COMPOSELEFT " );
    if( state->compose == STATE_RIGHT )
        strcat(buffer, " COMPOSERIGHT " );

    if( state->numlock > STATE_NONE )
        strcat(buffer, " NUMLOCK " );

    if( state->capslock > STATE_NONE )
        strcat(buffer, " CAPSLOCK " );

    if( buffer[0] != '\0' )
        printf( "modifiers: %s\n", buffer );
}
#endif
