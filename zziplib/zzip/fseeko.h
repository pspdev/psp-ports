#ifndef _ZZIP_FSEEKO_H_
#define _ZZIP_FSEEKO_H_
/*
 * simplified zip disk access using fseeko on a stdio FILE handle
 *
 * Author: 
 *      Guido Draheim <guidod@gmx.de>
 *
 * Copyright (c) 2003,2004 Guido Draheim
 *          All rights reserved,
 *          use under the restrictions of the 
 *          Lesser GNU General Public License
 *          or alternatively the restrictions 
 *          of the Mozilla Public License 1.1
 */


#include <zzip/types.h>
#include <stdio.h>

typedef struct zzip_entry ZZIP_ENTRY;
typedef struct zzip_entry_file ZZIP_ENTRY_FILE;

typedef int (*zzip_strcmp_fn_t)(char*, char*);
typedef int (*zzip_fnmatch_fn_t)(char*, char*, int);

#define zzip_entry_extern extern

zzip_off_t
zzip_entry_data_offset(ZZIP_ENTRY* entry);

char* _zzip_restrict
zzip_entry_strdup_name(ZZIP_ENTRY* entry);


zzip_entry_extern ZZIP_ENTRY* _zzip_restrict
zzip_entry_findfirst(FILE* file);

zzip_entry_extern ZZIP_ENTRY* _zzip_restrict
zzip_entry_findnext(ZZIP_ENTRY* _zzip_restrict entry);

#define zzip_entry_findlast      zzip_entry_free
#define zzip_entry_findlastfile  zzip_entry_free
#define zzip_entry_findlastmatch zzip_entry_free

zzip_entry_extern int
zzip_entry_free(ZZIP_ENTRY* entry);

char* _zzip_restrict
zzip_entry_strdup_name(ZZIP_ENTRY* entry);
char*
zzip_entry_to_data(ZZIP_ENTRY* entry);

zzip_entry_extern ZZIP_ENTRY* _zzip_restrict
zzip_entry_findfile(FILE* disk, char* filename, 
		    ZZIP_ENTRY* _zzip_restrict old,
		   zzip_strcmp_fn_t compare);
zzip_entry_extern ZZIP_ENTRY* _zzip_restrict
zzip_entry_findmatch(FILE* disk, char* filespec, 
		     ZZIP_ENTRY* _zzip_restrict old,
		    zzip_fnmatch_fn_t compare, int flags);

zzip_entry_extern ZZIP_ENTRY_FILE* _zzip_restrict
zzip_entry_fopen (ZZIP_ENTRY* entry, int takeover);

zzip_entry_extern ZZIP_ENTRY_FILE* _zzip_restrict
zzip_entry_ffile (FILE* disk, char* filename);

zzip_entry_extern _zzip_size_t
zzip_entry_fread (void* ptr, _zzip_size_t size, _zzip_size_t nmemb,
		 ZZIP_ENTRY_FILE* file);
zzip_entry_extern int
zzip_entry_fclose (ZZIP_ENTRY_FILE* file);
int
zzip_entry_feof (ZZIP_ENTRY_FILE* file);

#endif

