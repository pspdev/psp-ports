/*
    libpspirkeyb: PSP IR Keyboard Library (http://pspirkeyb.podzone.net)
    Copyright (C) 2007 Harald Fielker <harald.fielker@web.de>

    This program can be distributed under the terms of the GNU LGPL.
    See the file LICENSE.
*/

#ifndef __VT100OUTPUT_H
#define __VT100OUTPUT_H

#ifdef __cplusplus
extern "C" {
#endif

void vt100_output_map( unsigned char *data, int size, unsigned char is_rawcode, unsigned char shift, unsigned char ctrl, unsigned char alt, unsigned char pressed, unsigned char * buffer, int *length );


#ifdef __cplusplus
}
#endif

#endif
