/*
    libpspirkeyb: PSP IR Keyboard Library (http://pspirkeyb.podzone.net)
    Copyright (C) 2007 Harald Fielker <harald.fielker@web.de>

    This program can be distributed under the terms of the GNU LGPL.
    See the file LICENSE.
*/

#ifndef __KEYMAP_H
#define __KEYMAP_H

#ifdef __cplusplus
extern "C" {
#endif

/* reset state e.g. after power resume */
void keymap_reset_state();

/* decoder for raw keys */
void keymap_decode( int outputmode, unsigned char raw, unsigned char pressed, unsigned char * buffer, int *length );

#ifdef __cplusplus
}
#endif

#endif
