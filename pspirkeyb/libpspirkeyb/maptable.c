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
#include "maptable.h"

//#define DEBUG_MAP_ENTRIES                       1
//#define printf pspDebugScreenPrintf

static char * g_sections[] = { "normal", "shift", "ctrl", "alt", "compose", "meta", "num" };
static struct Maptable *g_map = NULL;
static int g_map_entries = -1;
static int g_map_entry_slots = 0;

static int maptable_load_map_rec( const char * file, int depth );
static void init_map_entry_array();

/* load a map */
int maptable_load_map( const char * file )
{
    if( g_map != NULL )
        maptable_unload_map();

    init_map_entry_array();

    if( g_map == NULL )
        return PSP_IRKBD_RESULT_FAILED;

    return maptable_load_map_rec( file, 0 );
}


/* unload map */
int maptable_unload_map()
{
    if( g_map == NULL )
        return -1;

    free( g_map );
    g_map = 0;
    g_map_entry_slots = 0;
    g_map_entries = 0;

    return 0;
}

/* returns the maptable entry for the given rawcode */
struct Maptable * maptable_entry( unsigned char raw )
{
    int i;
    struct Maptable * map = NULL;

    if( g_map_entries < 0 )
        return map;

    /* find map entry */
    for( i=0; i< g_map_entries+1; i++ )
    {
        map = g_map+i;
        if( map->raw == raw )
            return map;
    }

    return NULL;
}


/* read content of single ini section:keyname entry */
static char * get_ini_entry( dictionary * ini, const char * keyname, const char * section )
{
    char buffer[255];

    if( !ini || !keyname || !section )
        return NULL;

    sprintf( buffer, "%s:%s", keyname, section );
    return iniparser_getstring(ini, buffer, NULL);
}

/* realocs the map entry table */
static void init_map_entry_array()
{
    int top = 16;
    g_map = (struct Maptable*) malloc( top * sizeof( struct Maptable ) );
    g_map_entries = -1;
    g_map_entry_slots = top;
}

/* new entry for a given raw key - will recycle existing entries */
static struct Maptable * get_new_maptable_entryslot( unsigned char raw )
{
    struct Maptable * map = NULL;
    int i;

    if( g_map_entry_slots == 0 )
        init_map_entry_array();

    /* check if we already know a map for the given raw key */
    if( g_map_entries > -1 )
    {
        for( i=0; i< g_map_entries+1; i++ )
        {
            map = g_map+i;
            if( map->raw == raw )
                return map;
        }
        map = NULL;
    }

    /* go to next position */
    g_map_entries++;

    /* realloc if buffer is too small */
    if( g_map_entries+1 > g_map_entry_slots )
    {
        int inc = g_map_entry_slots/2;
        if( inc < 8 )
            inc = 8;
        g_map_entry_slots +=  inc;
        g_map = (struct Maptable*) realloc( g_map, g_map_entry_slots* sizeof( struct Maptable ));
    }

    if( g_map == NULL )
        return NULL;

    map = g_map + g_map_entries;
    memset( map, 0, sizeof(struct Maptable));
    map->raw = raw;

    return map;
}

/* inserts the text read from the ini parser to a new map entry content*/
static void prepare_map_key_entry( struct MaptableEntry * result, char * text )
{
    unsigned int c;
    int length;

    if( !result || !text )
        return;

    length = strlen(text);
    result->length = 0;
    result->is_rawcode = 0;

    if( length == 1 )
    {
        result->map[0] = text[0];
        result->length = 1;
    }

    if( length > 2 )
    {
        /* hex code */
        if( text[0] == '0' && ( text[1] == 'x' || text[1] == 'X' ) )
        {
            if( sscanf( text+2, "%x", &c ) )
            {
                result->map[0] = c;
                result->length = 1;
            }
        }
        /* unicode */
        if( ( text[0] == 'u' || text[0] == 'U' ) && text[1] == '+' )
        {
            int k;
            for(k=2; k < length; k += 2 )
            {
              /* Zx: bug fix */
              char buf_uni[3];
              buf_uni[0] = text[k];
              buf_uni[1] = text[k+1];
              buf_uni[2] = 0;
              if( sscanf( buf_uni, "%x", &c ) )
              {
                result->map[(k-2)/2] = c;
                result->length++;
              }
            }
        }
    }
    if( length > 4 )
    {
        // this is a keycode
        if( ( text[0] == 'K' && text [1] == 'E' && text[2] == 'Y' && text[3] == '_' ) ||
            ( text[0] == 'B' && text [1] == 'T' && text[2] == 'N' && text[3] == '_' ) )
        {
            int i;

            for( i=0; i < g_maxKeys; i++ )
            {
                struct KeyName *keyName = keys+i;
                
                if( strcmp( keyName->name, text ) == 0 )
                {
                    result->map[0] = keyName->raw;
                    result->length = 1;
                    result->is_rawcode = 1;
                    break;
                }
            }
        }
    }

    // cut length
    if( result->length > MAPTABLE_MAX_CHARS )
        result->length = MAPTABLE_MAX_CHARS;

}

#ifdef DEBUG_MAP_ENTRIES
static void dump_map_entry( struct Maptable * entry )
{
    int k;
    int i;

    if( entry == NULL )
        return;

    printf( "entry for: %s - %d\n", pspIrKeybRawkeyName( entry->raw ), entry->raw );
    
    for( i = 0; i < MAPTABLE_SECT_LAST+1; i ++ )
    {
        if( entry->e[i].length > 0 )
        {
            printf( "is raw code : %d\n", entry->e[i].is_rawcode );              
            printf( "%s size %d - code: '", g_sections[i],entry->e[i].length );       
            for( k=0; k < entry->e[i].length; k++ )
                printf( "%c", *(entry->e[i].map+k) );
            printf( "'\n" );
        }
    }

    printf( "\n" );

}
#endif

/* loader for nested maps */
static int maptable_load_map_rec( const char * file, int depth )
{
    int i;
    char buffer[4096];
    char *p;

    dictionary * ini;

    if( file == NULL || g_map == NULL )
        return PSP_IRKBD_RESULT_FAILED;

    if( depth > 3 )
        return PSP_IRKBD_RESULT_MAPFILE_MAXDEPTHLEVEL;

    ini = iniparser_new( file );
    if( ini == NULL )
    {
        if( depth == 0 )
            return PSP_IRKBD_RESULT_CANT_OPEN_MAPFILE;
        return PSP_IRKBD_RESULT_CANT_OPEN_MAPFILE_INCLUDE;
    }

    /* check for includes */
    strcpy(buffer,file);
    for(p=buffer+strlen(buffer)-1; p!=buffer; p--) {
        if( *p== '/' ) {
            p++;
            *p = '\0';
            break;
        }
    }

    /* we have an abs path */
    if( p!= buffer  )
    {
        /* do include */     
        char *include = iniparser_getstring(ini, "GLOBAL:include", NULL );
        if( include )
        {   
            int res;
            strcat( buffer, include );
            iniparser_free(ini); /* close current ini file */
            ini = NULL;

            /* recursion */
            res = maptable_load_map_rec( buffer, depth+1 );
            if( res < 0 )
                return res;

            /* reopen ini file */
            ini = iniparser_new( file );
        }
    }

    for( i=0; i < g_maxKeys; i++ )
    {
        struct KeyName *keyName = keys+i;
        struct Maptable *map = NULL;
        char * entry;
        int k;

        for( k=0; k < MAPTABLE_SECT_LAST+1; k ++ )
        {
            entry = get_ini_entry( ini, keyName->name, g_sections[k] );
            if( entry )
            {
                /* get new slot */
                if( map == NULL ) 
                    map = get_new_maptable_entryslot( keyName->raw );
                prepare_map_key_entry( &map->e[k], entry );
            }
        }

        #ifdef DEBUG_MAP_ENTRIES
        //if( keyName->raw == KEY_DOWN )
        dump_map_entry( map );
        #endif
    }

    iniparser_free( ini );

    if( g_map_entries > 0 && depth == 0)
    {
        /* free some memory */
        g_map_entry_slots = g_map_entries+1;
        g_map = (struct Maptable*) realloc( g_map, g_map_entry_slots* sizeof( struct Maptable ));
    }

	return PSP_IRKBD_RESULT_OK;
}

