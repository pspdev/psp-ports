/*
    libpspirkeyb: PSP IR Keyboard Library (http://pspirkeyb.podzone.net)
    Copyright (C) 2007 Harald Fielker <harald.fielker@web.de>

    This program can be distributed under the terms of the GNU LGPL.
    See the file LICENSE.
*/

#ifndef __NOVAETSMAP_H
#define __NOVAETSMAP_H

#ifdef __cplusplus
extern "C" {
#endif

/* rows/cols in the keyboard */
#define NOVAETS_KIS2_KEY_ROWS 5
#define NOVAETS_KIS2_KEY_COLS 14

/* 
    decodes the input cycle 
    return 0 if ok -1 if guessing fails
 */
int novaets_kis2_cycle_decode( unsigned char data[2], int *row, int *col, unsigned char *pressed, int *ismod );

extern unsigned char novaets_kis2_keycode_table[NOVAETS_KIS2_KEY_ROWS][NOVAETS_KIS2_KEY_COLS];

#ifdef __cplusplus
}
#endif

#endif
