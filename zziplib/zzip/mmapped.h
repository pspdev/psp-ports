#ifndef _ZZIP_MMAPPED_H_
#define _ZZIP_MMAPPED_H_
/*
 * simplified zip disk access using a mmapping of the whole archive.
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

typedef struct zzip_disk_file  ZZIP_DISK_FILE;
typedef struct zzip_disk       ZZIP_DISK;
typedef struct zzip_disk_entry ZZIP_DISK_ENTRY;

/* we expose this structure so third party applications can augment
 * on them. The mmapped zip access usually just needs the two pointers
 * specifying the mmapped area, whereever you have that from.
 */
struct zzip_disk
{
    char* buffer;      /* start of mmapped area, the base of all seekvals */
    char* endbuf;      /* end of mmapped area, i.e. buffer + buflen */
    char* reserved;    /* - for later extensions (might be renamed) */
    char* user;        /* - free for applications (use this!) */
    long  flags;       /* bit 0: findfile searches case-insensitive */
    long  mapped;      /* used for mmap() wrappers of zzip/__mmap.h */
    long  unused;      /* - for later extensions (might be renamed) */
    long  code;        /* - free for applications (use this!) */
};

typedef int (*zzip_strcmp_fn_t)(char*, char*);
typedef int (*zzip_fnmatch_fn_t)(char*, char*, int);

#define zzip_disk_extern extern

zzip_disk_extern int
zzip_disk_init(ZZIP_DISK* disk, char* buffer, zzip_size_t buflen);

zzip_disk_extern ZZIP_DISK* _zzip_restrict
zzip_disk_new(void);

zzip_disk_extern ZZIP_DISK* _zzip_restrict
zzip_disk_mmap(int fd);

zzip_disk_extern int 
zzip_disk_munmap(ZZIP_DISK* disk);

zzip_disk_extern ZZIP_DISK*  _zzip_restrict
zzip_disk_open(char* filename);

zzip_disk_extern int
zzip_disk_close(ZZIP_DISK* disk);


zzip_disk_extern int
zzip_disk_init (ZZIP_DISK* disk, char* buffer, _zzip_size_t buflen);

zzip_disk_extern ZZIP_DISK_ENTRY*
zzip_disk_findfirst(ZZIP_DISK* disk);

zzip_disk_extern ZZIP_DISK_ENTRY*
zzip_disk_findnext(ZZIP_DISK* disk, ZZIP_DISK_ENTRY* entry);

char* _zzip_restrict
zzip_disk_entry_strdup_name(ZZIP_DISK* disk, ZZIP_DISK_ENTRY* entry);
struct zzip_file_header*
zzip_disk_entry_to_file_header(ZZIP_DISK* disk, ZZIP_DISK_ENTRY* entry);
char*
zzip_disk_entry_to_data(ZZIP_DISK* disk, ZZIP_DISK_ENTRY* entry);

zzip_disk_extern ZZIP_DISK_ENTRY*
zzip_disk_findfile(ZZIP_DISK* disk, 
		   char* filename, ZZIP_DISK_ENTRY* after,
		   zzip_strcmp_fn_t compare);
zzip_disk_extern ZZIP_DISK_ENTRY*
zzip_disk_findmatch(ZZIP_DISK* disk, 
		    char* filespec, ZZIP_DISK_ENTRY* after,
		    zzip_fnmatch_fn_t compare, int flags);


zzip_disk_extern ZZIP_DISK_FILE* _zzip_restrict
zzip_disk_entry_fopen (ZZIP_DISK* disk, ZZIP_DISK_ENTRY* entry);

zzip_disk_extern ZZIP_DISK_FILE* _zzip_restrict
zzip_disk_fopen (ZZIP_DISK* disk, char* filename);

zzip_disk_extern _zzip_size_t
zzip_disk_fread (void* ptr, _zzip_size_t size, _zzip_size_t nmemb,
		 ZZIP_DISK_FILE* file);
zzip_disk_extern int
zzip_disk_fclose (ZZIP_DISK_FILE* file);
int
zzip_disk_feof (ZZIP_DISK_FILE* file);

#endif

