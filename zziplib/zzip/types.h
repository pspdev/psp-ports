/*
 * unlike in <zzip/conf.h> we are allowed to make up typedefs here,
 * while over there only #defines are allowed
 *
 * Author: 
 *	Guido Draheim <guidod@gmx.de>
 *
 *	Copyright (c) 2003,2004 Guido Draheim
 * 	    All rights reserved, 
 *          usage allowed under the restrictions of the
 *	    Lesser GNU General Public License 
 *          or alternatively the restrictions 
 *          of the Mozilla Public License 1.1
 *
 * if you see "unknown symbol" errors, check first that `-I ..` is part of
 * your compiler options - a special hint to VC/IDE users who tend to make up
 * their own workspace files. All includes look like #include <zzip|*.h>, so
 * you need to add an include path to the dir containing (!!) the ./zzip/ dir
 */

#ifndef _ZZIP_TYPES_H_
#define _ZZIP_TYPES_H_

#include <zzip/conf.h>
#include <fcntl.h>
#include <stddef.h> /* size_t and friends */
/* msvc6 has neither ssize_t (we assume "int") nor off_t (assume "long") */


typedef       _zzip_off_t       zzip_off_t;
typedef       _zzip_size_t      zzip_size_t;
typedef       _zzip_ssize_t     zzip_ssize_t;

#endif

