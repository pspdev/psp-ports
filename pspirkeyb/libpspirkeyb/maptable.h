/*
    libpspirkeyb: PSP IR Keyboard Library (http://pspirkeyb.podzone.net)
    Copyright (C) 2007 Harald Fielker <harald.fielker@web.de>

    This program can be distributed under the terms of the GNU LGPL.
    See the file LICENSE.
*/

#ifndef __MAPTABLE_H
#define __MAPTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

/* max ascii keys we are able to map */
#define     MAPTABLE_MAX_CHARS       4

#define     MAPTABLE_SECT_NORMAL     0
#define     MAPTABLE_SECT_SHIFT      1
#define     MAPTABLE_SECT_CTRL       2
#define     MAPTABLE_SECT_ALT        3
#define     MAPTABLE_SECT_COMPOSE    4
#define     MAPTABLE_SECT_META       5
#define     MAPTABLE_SECT_NUM        6
#define     MAPTABLE_SECT_LAST       MAPTABLE_SECT_NUM  /*must be last in list*/

/* this is what we map */
struct MaptableEntry
{
    unsigned char map[MAPTABLE_MAX_CHARS];
    unsigned char length;
    char is_rawcode;
};

/* map table entry for a single raw key in a given mode */
struct Maptable
{
    unsigned char raw;
    struct MaptableEntry e[MAPTABLE_SECT_LAST+1];
};

/* load a map */
int maptable_load_map( const char * file );

/* unload map */
int maptable_unload_map();

/* returns the maptable entry for the given rawcode */
struct Maptable * maptable_entry( unsigned char raw );


#ifdef __cplusplus
}
#endif

#endif
