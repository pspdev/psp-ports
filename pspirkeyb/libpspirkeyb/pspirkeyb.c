/*
    libpspirkeyb: PSP IR Keyboard Library (http://pspirkeyb.podzone.net)
    Copyright (C) 2007 Harald Fielker <harald.fielker@web.de>

    This program can be distributed under the terms of the GNU LGPL.
    See the file LICENSE.
*/

#include <pspkernel.h>
#include <pspdebug.h>
#include <psppower.h>

#include <string.h>
#include <stdio.h>

#include "iniparser.h"
#include "maptable.h"
#include "keymap.h"
#include "kbdkeyboards.h"
#include "novaetsmap.h"

#include "pspirkeyb.h"

//#define printf pspDebugScreenPrintf

#define KEY_PRESSED     1
#define KEY_RELEASED    0

/* baudrates - currently only 9600 = default is suppored! */
#define B0      0
#define B2400   2400
#define B4800   4800
#define B9600   9600

/* config stuff */
#define DEFAULT_INI_FILE "ms0:/seplugins/pspirkeyb.ini"
#define IRDA_DEVICE      "irda0:"
//#define IRDA_DEVICE      "host0:/pspirkeyb/irdacapture.qwerty"    /*a raw dump from pspirdacapture*/

/* typedef for callback */
typedef int (*keyboard_input)(unsigned char* buffer, int *length);


static int g_keyboard = PSP_IRKBD_TYPE_NONE;
static int g_baudrate = B0;
static int g_irdafd   = -1;
static int g_kernelmode = 0;
static int g_outputmode = PSP_IRKBD_OUTPUT_MODE_ASCII;
static keyboard_input g_keyboard_input_CB = NULL;
static int g_keydelay = -1;
static int g_keyrate = -1;
int g_lastirkey = -1;

/* initalize the keyboard with a given type and mapfile */
static int pspIrKeybSetKeyboard(int keyboard, const char* mapfile);

/* keyboard specific callbacks */
static int compaq_foldable(unsigned char* buffer, int *length);
static int belkin_infrared(unsigned char* buffer, int *length);
static int belkin_F8U1500_ir_keyboard(unsigned char* buffer, int *length);
static int sprint_infrared(unsigned char* buffer, int *length);
static int freedom_keyboard(unsigned char* buffer, int *length);
static int novaets_kis2(unsigned char* buffer, int *length);
static int snapntype(unsigned char* buffer, int *length);
static int hpslim(unsigned char* buffer, int *length);
static int flexis(unsigned char* buffer, int *length);
static int micro_innovations(unsigned char* buffer, int *length);
static int micro_foldaway(unsigned char* buffer, int *length);
static int micro_datapad(unsigned char* buffer, int *length);
static int compaq_microkbd(unsigned char* buffer, int *length);
static int targus_infrared(unsigned char* buffer, int *length);
static int benqgamepad(unsigned char* buffer, int *length);
static int palmuw(unsigned char* buffer, int *length);
static int palm1(unsigned char* buffer, int *length);
static int hama(unsigned char* buffer, int *length);
static int pspIrdaSetBaud( int baudrate );

int pspIrKeybInit(const char* inifile, int kernelmode)
{
    const char *defaultfile = DEFAULT_INI_FILE;
    dictionary * ini;
    int keyboard=PSP_IRKBD_TYPE_NONE;
    char buffer[4096];
    char *p;
    char *str;

    /* check if already initalized */
    if( g_keyboard != PSP_IRKBD_TYPE_NONE )
        return PSP_IRKBD_RESULT_UNKNOW_KEYBOARD;

    if( !inifile )
        inifile = defaultfile;

    buffer[0] = '\0';
    p = buffer;

    ini = iniparser_new( inifile );
    if( ini == NULL )
        return PSP_IRKBD_RESULT_CONFIG_FILE_NOT_FOUND;

    g_kernelmode = kernelmode;

    /* keyboard type */
    keyboard = iniparser_getint( ini, "KEYBOARD:keyboard", PSP_IRKBD_TYPE_NONE );
    str = iniparser_getstring( ini, "KEYBOARD:mapfile_dir", NULL );
    if( str )
    {
        strcpy( p, str );
        p += strlen( str );
        if( *(p-1) != '/' )
        {
            *p='/';
            p++;
            *p='\0';
        }
    }
    str = iniparser_getstring( ini, "KEYBOARD:map", NULL );
    if( str )
        strcpy( p, str );

	/* key repeat vars */
    g_keydelay = iniparser_getint( ini, "KEYBOARD:delay", -1 );
    g_keyrate = iniparser_getint( ini, "KEYBOARD:rate", -1 );

    iniparser_free( ini );

    g_outputmode = PSP_IRKBD_OUTPUT_MODE_ASCII;

    return pspIrKeybSetKeyboard( keyboard, buffer );
}

int pspIrKeybOutputMode( int outputmode )
{
    switch( outputmode )
    {
        case PSP_IRKBD_OUTPUT_MODE_SCANCODE:
        case PSP_IRKBD_OUTPUT_MODE_ASCII:
        case PSP_IRKBD_OUTPUT_MODE_RAW:
        case PSP_IRKBD_OUTPUT_MODE_VT100:
            g_outputmode = outputmode;
            break;
        default:
            return -1;
    }

    return 0;
}

int pspIrKeybGetOutputMode()
{
    return g_outputmode;
}

int pspIrKeybReadinput(void* buffer, int *length)
{
	static int delay = -1;
	int err;

    if( !buffer || !length || !g_keyboard_input_CB )
        return -1;

    *length = 0;
    if( g_irdafd < 0 )
    {
        /* open irda */
        g_irdafd = sceIoOpen(IRDA_DEVICE, PSP_O_RDWR, 0);
        if (g_irdafd < 0) {
            return PSP_IRKBD_RESULT_FAILED;
        }

        /* change baudrate */
        if( pspIrdaSetBaud( g_baudrate ) < 0 )
            return PSP_IRKBD_RESULT_CANT_SET_BAUDTATE;
    }

    /* input from from keyboard */
    err = g_keyboard_input_CB( (unsigned char*)buffer, length );

	if (err == 0)
		// got key, reset key repeat
		delay = g_keydelay;

    if (err == -1)
    {
		delay--;

		if (delay == 0)
			if (g_lastirkey != -1)
			{
				// do key repeat
				keymap_decode( g_outputmode, g_lastirkey, KEY_PRESSED, buffer, length );
				err = 0;
				delay = g_keyrate; // delay until next repeat
			}

		if (delay < 0)
			delay = g_keydelay;
    }

    return err;
}

int pspIrKeybFinish(void)
{
    if (g_irdafd < 0)
        return PSP_IRKBD_RESULT_OK;

    /* Close irda */
    sceIoClose(g_irdafd);
    g_irdafd = -1;
    g_baudrate = B0;
    g_keyboard = PSP_IRKBD_TYPE_NONE;
    g_keyboard_input_CB = NULL;
    g_outputmode = PSP_IRKBD_OUTPUT_MODE_ASCII;

    maptable_unload_map();

    /* restore baud rate */
    pspIrdaSetBaud( B9600 );
    return PSP_IRKBD_RESULT_OK;
}


/* initalize the keyboard with a given type and mapfile */
static int pspIrKeybSetKeyboard(int keyboard, const char* mapfile)
{
    int ret = -1;

    if( !mapfile )
        return ret;

    switch( keyboard )
    {
        case PSP_IRKBD_TYPE_TARGUSIR:
            g_keyboard_input_CB = targus_infrared;
            g_baudrate = B9600;
            break;
        case PSP_IRKBD_TYPE_COMPAQ_MICROKBD:
            g_keyboard_input_CB = compaq_microkbd;
            g_baudrate = B4800;
            break;
        case PSP_IRKBD_TYPE_MICRO_DATAPAD:
            g_keyboard_input_CB = micro_datapad;
            g_baudrate = B9600;
            break;
        case PSP_IRKBD_TYPE_FREEDOM:
            g_keyboard_input_CB = freedom_keyboard;
            g_baudrate = B9600;
            break;
        case PSP_IRKBD_TYPE_MICRO_INNOVATIONS:
	    g_keyboard_input_CB = micro_innovations;
            g_baudrate = B9600;
            break;
        case PSP_IRKBD_TYPE_MICRO_FOLDAWAY:
            g_keyboard_input_CB = micro_foldaway;
            g_baudrate = B9600;
            break;
        case PSP_IRKBD_TYPE_FOLDABLE:
            g_keyboard_input_CB = compaq_foldable;
            g_baudrate = B4800;
            break;
        case PSP_IRKBD_TYPE_BELKINIR:
            g_keyboard_input_CB = belkin_infrared;
            g_baudrate = B9600;
            break;
		case PSP_IRKBD_TYPE_BELKINIRF8U1500:
            g_keyboard_input_CB = belkin_F8U1500_ir_keyboard;
            g_baudrate = B9600;
            break;
        case PSP_IRKBD_TYPE_SPRINT:
		    g_keyboard_input_CB = sprint_infrared;
            g_baudrate = B9600;
            break;
        case PSP_IRKBD_TYPE_BENQ_GAMEPAD:
            g_keyboard_input_CB = benqgamepad;
            g_baudrate = B4800;
            break;
        case PSP_IRKBD_TYPE_HPSLIM:
            g_keyboard_input_CB = hpslim;
            g_baudrate = B4800;
            break;
        case PSP_IRKBD_TYPE_SNAPNTYPE:
            g_keyboard_input_CB = snapntype;
            g_baudrate = B2400;
            break;
        case PSP_IRKBD_TYPE_NOVAETS_KIS2:
            g_keyboard_input_CB = novaets_kis2;
            g_baudrate = B9600;
            break;
        case PSP_IRKBD_TYPE_FLEXIS:
            g_keyboard_input_CB = flexis;
            g_baudrate = B9600;
            break;
        case PSP_IRKBD_TYPE_PALM_UW:
            g_keyboard_input_CB = palmuw;
            g_baudrate = B9600;
            break;
        case PSP_IRKBD_TYPE_HAMA:
            g_keyboard_input_CB = hama;
            g_baudrate = B9600;
            break;
        case PSP_IRKBD_TYPE_PALM_ONE:
            g_keyboard_input_CB = palm1;
            g_baudrate = B9600;
            break;
        default:
            return PSP_IRKBD_RESULT_UNKNOW_KEYBOARD;
    }

    g_keyboard = keyboard;
    ret = maptable_load_map( mapfile );

    return ret;
}

#define PSP_UART5_DIV1 0xBE540024
#define PSP_UART5_DIV2 0xBE540028
#define PSP_UART5_CTRL 0xBE54002C
#define PSP_UART_CLK   96000000

/* idea taken from : */
/* http://forums.ps2dev.org/viewtopic.php?t=7192&highlight=irda+baud */
/* don't know if it works... */
static int pspIrdaSetBaud( int baud )
{
    int div1, div2;

    /* this only works in kernel mode - but setting to "default" rate is ok*/
    if( baud == B9600 && !g_kernelmode )
        return PSP_IRKBD_RESULT_OK;

    /* rate set using the rough formula div1 = (PSP_UART_CLK / baud) >> 6 and
     * div2 = (PSP_UART_CLK / baud) & 0x3F
     * The uart4 driver actually uses a slightly different formula for div 2 (it
     * adds 32 before doing the AND, but it doesn't seem to make a difference
     */
    div1 = PSP_UART_CLK / baud;
    div2 = div1 & 0x3F;
    div1 >>= 6;

    _sw(div1, PSP_UART5_DIV1);
    _sw(div2, PSP_UART5_DIV2);
    _sw(0x60, PSP_UART5_CTRL);

    return PSP_IRKBD_RESULT_OK;
}

/* keyboard callbacks */

static int novaets_kis2(unsigned char* buffer, int *length)
{
    unsigned char buf[16];
    int row, col, ismod;
    unsigned char raw, pressed;

    /* Read 2 bytes */
    if( sceIoRead(g_irdafd, buf, 2) != 2 )
        return -1;

    /* resume display */
    scePowerTick(0);

    /* fancy novaets decode */
    if( novaets_kis2_cycle_decode( buf, &row, &col, &pressed, &ismod ) != 0 )
        return -1;

    raw = novaets_kis2_keycode_table[row][col];

    keymap_decode( g_outputmode, raw, pressed, buffer, length );

    if( !ismod && pressed )
    {
        // autogen a "release" for this braindead keyboard:
        // only "modifier" keys can be guessed
        keymap_decode( g_outputmode, raw, !pressed, buffer, length );
    }

    return 0;
}

/* the following keyboards are ported from keybd */

static int compaq_foldable(unsigned char* buffer, int *length)
{
    unsigned char buf[16];
    static char fn=0;

    if( sceIoRead(g_irdafd, buf, 2) != 2 )
        return (-1);

    /* resume display */
    scePowerTick(0);

    // fprintf(stderr, "0x%02x 0x%02x 0x%02x | ", buf[0], buf[1], ~buf[0]);
    if (buf[0] == (unsigned char)~buf[1]) {
        //if (debug) fprintf(stderr, "press: %d ", buf[0]);
        if (buf[0] == 0x02) {
            fn=1;
            return 0;
        }
        if (fn)
            buf[0] = foldable_function[buf[0]];
        else
            buf[0] = foldable_normal[buf[0]];
        //if (debug)
        //    fprintf(stderr,"= 0x%02x\n", buf[0]);
        if (buf[0] > 0)
        {
            keymap_decode( g_outputmode, (unsigned short)buf[0], KEY_PRESSED, buffer, length );
            return 0;
        }
    } else if (((unsigned char)buf[0] & (unsigned char)~0x80) == (unsigned char)~buf[1]) {
        //if (debug)
        //    fprintf(stderr, "rel. : %d ", buf[0] & ~0x80);
        if ((buf[0] & ~0x80) == 0x02) {
            fn = 0;
            return 0;
        }
        if (fn)
            buf[0] = foldable_function[(unsigned char)buf[0] & (unsigned char)~0x80];
        else
            buf[0] = foldable_normal[(unsigned char)buf[0] & (unsigned char)~0x80];
        //if (debug)
        //    fprintf(stderr,"= 0x%02x\n", buf[0]);
        if (buf[0] > 0)
        {
             keymap_decode( g_outputmode, (unsigned short)buf[0], KEY_RELEASED, buffer, length );
             return 0;
        }
    }

    return 0;
}

static int belkin_infrared(unsigned char* buffer, int *length)
{
    unsigned char buf[16];
    unsigned char key;
    unsigned int  key_down;
    unsigned char keycode;

    if( sceIoRead(g_irdafd, buf, 2) != 2 )
        return (-1);

    /* resume display */
    scePowerTick(0);

    key = buf[1] & 0x7f;
    key_down = !(buf[1] & 0x80);
    keycode = belkin_irda_normal[key];

    //if (debug)
    //    fprintf(stderr, "0x%02x 0x%02x 0x%02x\n", buf[0], buf[1], key);

       if ( key_down ) {
           //if (debug)
           //fprintf(stderr,"press %d\n", keycode);
        keymap_decode( g_outputmode, (unsigned short)keycode, KEY_PRESSED, buffer, length );
    } else {
           //if (debug)
           //fprintf(stderr,"release %d\n", keycode);
        keymap_decode( g_outputmode, (unsigned short)keycode, KEY_RELEASED, buffer, length );
    }

    return 0;
}

static int belkin_F8U1500_ir_keyboard(unsigned char* buffer, int *length)
{
    unsigned char keycode=0;
    unsigned int  key_down=0;
    unsigned char buf1;
    int len;
    static int pi = 0;
    static unsigned char pbuf[4] = {0,0,0,0};

loop:
    len = sceIoRead(g_irdafd, &buf1, 1);
    if (len <= 0)
        return -1;
    // must be 1... can't be anything else at this point
    if (pi == 0)
    {
        // wait on 0xC0
        if (buf1 == 0xC0)
        {
            pbuf[0] = 0xC0;
            pi++;
        }
        goto loop;
    }

    pbuf[pi] = buf1;
    pi++;
    if (pi < 4)
        goto loop;

    pi = 0;

    /* resume display */
    scePowerTick(0);

//  printf( "0  - %d\n",  buf[0] );
//  printf( "1  - %d\n",  buf[1] );
//  printf( "2  - %d\n",  buf[2] );
//  printf( "3  - %d\n",  buf[3] );

	// verify packet
    if( pbuf[0] != 0xc0 || pbuf[3] != 0xc1 )
        return -1;

    keycode = pbuf[2];

    if( keycode < 128 )
        key_down = 1;
    else
        keycode -= 128;

    if ( key_down ) {
        //if (debug)
            //fprintf(stdout,"press %d\n", keycode);
        keymap_decode( g_outputmode, Belkin_F8U1500_IR_Keyboard[keycode], KEY_PRESSED, buffer, length );
    } else {
        //if (debug)
            //fprintf(stderr,"release %d\n", keycode);
        keymap_decode( g_outputmode, Belkin_F8U1500_IR_Keyboard[keycode], KEY_RELEASED, buffer, length );
    }

    return 0;
}

static int sprint_infrared(unsigned char* buffer, int *length)
{
    unsigned char buf[16];
    unsigned char key;
    unsigned int  key_down;
    unsigned char keycode;

    //keyboard mentality... -CLEE
    //press: group key  release: key|0x80 key|0x80

    if( sceIoRead(g_irdafd, buf, 2) != 2 )
        return (-1);

    /* resume display */
    scePowerTick(0);

    key = buf[1] & 0x7f;
    key_down = !(buf[1] & 0x80);
    keycode = sprint_irda_normal[key];

    //if (debug)
	//fprintf(stderr, "0x%02x 0x%02x 0x%02x\n", buf[0], buf[1], key);

   	if ( key_down ) {
   	    //if (debug)
   	    //fprintf(stderr,"press %d\n", keycode);
        keymap_decode( g_outputmode, (unsigned short)keycode, KEY_PRESSED, buffer, length );
	} else {
   	    //if (debug)
   	    //fprintf(stderr,"release %d\n", keycode);
        keymap_decode( g_outputmode, (unsigned short)keycode, KEY_RELEASED, buffer, length );
	}

    return 0;
}

static int freedom_keyboard(unsigned char* buffer, int *length)
{
    unsigned char buf[16];
    unsigned char key;
    unsigned int  key_down;
    unsigned char keycode;

    if( sceIoRead(g_irdafd, buf, 1) != 1 )
        return (-1);

    /* resume display */
    scePowerTick(0);

    key =  buf[0];
    //keyboard sends n when pressing a key
    // and n+63 when releasing the key
    key_down = ( key < 63 );
    if (!key_down)
        key = (key-63)&0x3F; // convert key code for key up
    keycode = freedom_kbd[key];
    //if (debug)
    //fprintf(stdout, "0x%02x 0x%02x\n", buf[0], keycode);
       if ( key_down ) {
        //if (debug)
        //fprintf(stdout,"press %d\n", keycode);
        keymap_decode( g_outputmode, (unsigned short)keycode, KEY_PRESSED, buffer, length );
    } else {
           //if (debug)
           //fprintf(stderr,"release %d\n", keycode);
        keymap_decode( g_outputmode, (unsigned short)keycode, KEY_RELEASED, buffer, length );
    }

    return 0;
}


static int snapntype(unsigned char* buffer, int *length)
{
    unsigned char buf[16];
    static char symb=0;

    if( sceIoRead(g_irdafd, buf, 1) != 1  )
        return (-1);

    /* resume display */
    scePowerTick(0);

    //if (debug)
    //    fprintf(stderr, "got %d\n", buf[0]);
    if (buf[0] & 0x80) { /* release */
        if( sceIoRead(g_irdafd, buf+1, 1)  != 1  )
            return (-1);
        buf[0] = buf[0] & 0x7f;
        if (buf[0] == 27) {
            symb = 0;
            return 0;
        }
        if (symb)
            buf[0] = snapntype_symbol[buf[0]];
        else
            buf[0] = snapntype_normal[buf[0]];
        //if (debug)
        //    fprintf(stderr, " release %d\n", buf[0]);
        if (buf[0] != 0)
        {
            keymap_decode( g_outputmode, (unsigned short)buf[0], KEY_RELEASED, buffer, length );
            return 0;
        }
    } else { /* press */
        if (buf[0] == 27) {
            symb=1;
            return 0;
        }
        if (symb)
            buf[0] = snapntype_symbol[buf[0]];
        else
            buf[0] = snapntype_normal[buf[0]];
        //if (debug)
        //    fprintf(stderr, " press %d\n", buf[0]);
        if (buf[0] != 0)
        {
            keymap_decode( g_outputmode, (unsigned short)buf[0], KEY_PRESSED, buffer, length );
        }
    }

    return 0;
}

static int hpslim(unsigned char* buffer, int *length)
{
    unsigned char cin;
    unsigned char cnew;
    static char symb=0;
    static char shft=0;
    static char symshft=0;
    static char numlck=0;

    if( sceIoRead(g_irdafd, &cin, 1)  != 1  )
        return (-1);

    /* resume display */
    scePowerTick(0);

    //if (debug)
    //    fprintf(stderr, "got %d 0x%x\n", cin,cin);
    if (cin & 0x80) { /* release */
        cin &= 0x7f;
        switch (cin) {
            case MKBD_HPS_FNKEY:     /* Fn key release */
                symb = 0;
                return 0;
            case KEY_LEFTSHIFT:
            case KEY_RIGHTSHIFT:
                shft = 0;
                break;
        }
        if (symb)
            cnew = hpslim_symbol[cin];
        else {
            /* if numlock, convert QWERTYUIOP to 1-9,0 */
            if (numlck && cin >= KEY_Q && cin <= KEY_P)
                cnew = cin + KEY_1 - KEY_Q;
            else
                cnew = hpslim_normal[cin];
        }
        //if (debug)
        //    fprintf(stderr, " release cnew=%d 0x%x\n", cnew,cnew);
        if (cnew != 0) {
            keymap_decode( g_outputmode, (unsigned short)cnew, KEY_RELEASED, buffer, length );
            if (symshft)
                keymap_decode( g_outputmode, (unsigned short)KEY_RIGHTSHIFT, KEY_RELEASED, buffer, length );
            symshft = 0;
            // toggle numlck on release
            if (cnew == KEY_NUMLOCK) numlck = !numlck;
        }
    } else { /* press */
        switch (cin) {
            case MKBD_HPS_FNKEY:     /* Fn key press */
                symb = 1;
                return 0;
            case KEY_LEFTSHIFT:
            case KEY_RIGHTSHIFT:
                shft = 1;
                break;
        }
        if (symb) {
            /* if shift needed to get correct char */
            symshft =  hpslim_symshft[cin] && !shft;
            cnew = hpslim_symbol[cin];
        } else {
            /* if numlock, convert QWERTYUIOP to 1-9,0 */
            if (numlck && cin >= KEY_Q && cin <= KEY_P)
                cnew = cin + KEY_1 - KEY_Q;
            else
                cnew = hpslim_normal[cin];
        }
        //if (debug)
        //    fprintf(stderr, " press cnew=%d 0x%x\n", cnew,cnew);
        if (cnew != 0) {
            if (symshft)
                keymap_decode( g_outputmode, (unsigned short)KEY_RIGHTSHIFT, KEY_PRESSED, buffer, length );
            keymap_decode( g_outputmode, (unsigned short)cnew, KEY_PRESSED, buffer, length );
        }
    }

    return 0;
}


static int flexis(unsigned char* buffer, int *length)
{
    unsigned char buf[16];
    char symb=0;

    if( sceIoRead(g_irdafd, buf, 1) != 1 )
        return (-1);

    /* resume display */
    scePowerTick(0);

    //if (debug)
    //  fprintf(stderr, "got %d\n", buf[0]);

    if (buf[0] & 0x80) { /* key press */
        buf[0] = buf[0] & 0x7f;

        if (symb)
            buf[0] = flexis_fx100_function[buf[0]];
        else
            buf[0] = flexis_fx100_normal[buf[0]];

        /* FIXME: function key */
        /* FIXME: this code is a little broken, it should check scancode not keycode for set 1/2 */

        if (buf[0] == KEY_LEFTSHIFT || buf[0] == KEY_RIGHTSHIFT ||
                buf[0] == KEY_LEFTCTRL || buf[0] == KEY_RIGHTCTRL ||
                buf[0] == KEY_LEFTALT || buf[0] == KEY_RIGHTALT ) {
            keymap_decode( g_outputmode, (unsigned short)buf[0], KEY_PRESSED, buffer, length );
        } else {
            if( sceIoRead(g_irdafd, buf+1, 1) != 1 )
                return (-1);
            //if (debug)
            //    fprintf(stderr, "got %d\n", buf[1]);

            keymap_decode( g_outputmode, (unsigned short)buf[0], KEY_PRESSED, buffer, length );
            keymap_decode( g_outputmode, (unsigned short)buf[0], KEY_RELEASED, buffer, length );
        }
    } else { /* release of key from Set 2 */
        if (symb)
            buf[0] = flexis_fx100_function[buf[0]];
        else
            buf[0] = flexis_fx100_normal[buf[0]];
        if (buf[0] != 0)
            keymap_decode( g_outputmode, (unsigned short)buf[0], KEY_RELEASED, buffer, length );
    }

    return 0;
}

static int benqgamepad(unsigned char* buffer, int *length)
{
    unsigned char buf[16];
    unsigned char keycode=0;
    int i;

    if( sceIoRead(g_irdafd, buf, 2) != 2 )
        return (-1);

    /* resume display */
    scePowerTick(0);

    if (buf[0] & 0x80) { /* release */
        //if (debug)
        //    fprintf(stderr, "release: %d %d\n", buf[0], buf[1]);
        buf[0] = buf[0] & 0x7f;
        for (i=0; i<10; i++) {
            if (benq_gamepad_map[i][0] == buf[0]) {
                keycode = benq_gamepad_map[i][1];
                break;
            }
        }
        if (keycode != 0)
            keymap_decode( g_outputmode, (unsigned short)keycode, KEY_RELEASED, buffer, length );

    } else {
        //if (debug)
        //    fprintf(stderr, "press: %d %d\n", buf[0], buf[1]);
        for (i=0; i<10; i++) {
            if (benq_gamepad_map[i][0] == buf[0]) {
                keycode = benq_gamepad_map[i][1];
                break;
            }
        }
        if (keycode != 0)
            keymap_decode( g_outputmode, (unsigned short)keycode, KEY_PRESSED, buffer, length );
    }

    return 0;
}

static int micro_innovations(unsigned char* buffer, int *length)
{
    unsigned char buf[16];
    static char fkey_num = 0;
    static char fkey_punct = 0;
    static char fkey_func = 0;
    static int  key_count = 0;

    if( sceIoRead(g_irdafd, buf, 1) != 1 )
        return (-1);

    /* resume display */
    scePowerTick(0);

	//if (debug)
		//fprintf(stderr, "got: %d\n", buf[0]);
    if (buf[0] & 0x80) { /* possible release */
	key_count--;

	buf[0] &= 0x7f;

	if (buf[0] == 110)
	    fkey_num = 0;
	else if (buf[0] == 118)
	    fkey_punct = 0;
	else if (buf[0] == 60)
	    fkey_func = 0;
	else {
	    if (fkey_punct) {
		int shift;
		unsigned short scan;
		scan  = micro_innovations_punct[buf[0]] & 0x7f;
		shift = micro_innovations_punct[buf[0]] & FKEY_SHIFT;
        	keymap_decode( g_outputmode, scan, KEY_RELEASED, buffer, length );
		if (shift)
        	    keymap_decode( g_outputmode, (unsigned short) KEY_LEFTSHIFT, KEY_RELEASED, buffer, length );
	    } else if (fkey_num) {
		buf[0] = micro_innovations_num[buf[0]];
        	keymap_decode( g_outputmode, (unsigned short) buf[0], KEY_RELEASED, buffer, length );
	    } else if (fkey_func) {
		buf[0] = micro_innovations_func[buf[0]];
        	keymap_decode( g_outputmode, (unsigned short) buf[0], KEY_RELEASED, buffer, length );
	    } else {
		buf[0] = micro_innovations_normal[buf[0]];
        	keymap_decode( g_outputmode, (unsigned short) buf[0], KEY_RELEASED, buffer, length );
	    }
	}

	if (key_count == 0)
	    /* Eat last repeated code */
    	    if( sceIoRead(g_irdafd, buf, 1) != 1 )
        	return (-1);
        return 0;
    } else {
	if (buf[0] != 0) {
	    key_count++;

	    if (buf[0] == 110)
		fkey_num = 1;
	    else if (buf[0] == 118)
		fkey_punct = 1;
	    else if (buf[0] == 60)
		fkey_func = 1;
	    else {
		if (fkey_punct) {
		    int shift;
		    unsigned short scan;
		    scan  = micro_innovations_punct[buf[0]] & 0x7f;
		    shift = micro_innovations_punct[buf[0]] & FKEY_SHIFT;
		    if (shift)
        		keymap_decode( g_outputmode, (unsigned short) KEY_LEFTSHIFT, KEY_PRESSED, buffer, length );
        	    keymap_decode( g_outputmode, scan, KEY_PRESSED, buffer, length );
		} else if (fkey_num) {
		    buf[0] = micro_innovations_num[buf[0]];
        	    keymap_decode( g_outputmode, (unsigned short) buf[0], KEY_PRESSED, buffer, length );
		} else if (fkey_func) {
		    buf[0] = micro_innovations_func[buf[0]];
        	    keymap_decode( g_outputmode, (unsigned short) buf[0], KEY_PRESSED, buffer, length );
		} else {
		    buf[0] = micro_innovations_normal[buf[0]];
        	    keymap_decode( g_outputmode, (unsigned short) buf[0], KEY_PRESSED, buffer, length );
		}
	    }
        }
    }

    return 0;
}

static int micro_foldaway(unsigned char* buffer, int *length)
{
    unsigned char buf[16];
    static unsigned char lastkey = 0;
    // char symb=0;
    int checkkey = 0;

    if( sceIoRead(g_irdafd, buf, 1) != 1 )
        return (-1);

    /* resume display */
    scePowerTick(0);

    //if (debug)
    //    fprintf(stderr, "got: %d\n", buf[0]);
    if (buf[0] & 0x80) { /* possible release */
        checkkey = (159 - buf[0]);
        if (checkkey > 0 && checkkey <= 4) {
            buf[0] = micro_foldaway_normal[159 - buf[0]];
            keymap_decode( g_outputmode, (unsigned short) buf[0], KEY_RELEASED, buffer, length );
        } else if (buf[0] == 133 && lastkey > 0) {
            keymap_decode( g_outputmode, lastkey, KEY_RELEASED, buffer, length );
            lastkey = 0;
        }
        /* Eat repeated code */
        if( sceIoRead(g_irdafd, buf, 1) != 1 )
            return (-1);
        return 0;
    } else {
        checkkey = buf[0];
/*        if (symb)
            buf[0] = micro_foldaway_function[buf[0]];
        else*/
            buf[0] = micro_foldaway_normal[buf[0]];

        if (buf[0] != 0) {
            if (checkkey > 4)
                lastkey = buf[0];  /* Not a modifier, record key */
            keymap_decode( g_outputmode, (unsigned short) buf[0], KEY_PRESSED, buffer, length );
        }
    }

    return 0;
}

static int micro_datapad(unsigned char* buffer, int *length)
{
    unsigned char buf[16];
    static unsigned char lastkey = 0;
    int checkkey = 0;

    if( sceIoRead(g_irdafd, buf, 1) != 1 )
        return (-1);

    /* resume display */
    scePowerTick(0);

    //if (debug)
    //fprintf(stderr, "got: %d\n", buf[0]);
    if (buf[0] & 0x80) { /* possible release */
        checkkey = (159 - buf[0]);
        if (checkkey > 0 && checkkey <= 4) {
            buf[0] = micro_foldaway_normal[159 - buf[0]];
            keymap_decode( g_outputmode, (unsigned short) buf[0], KEY_RELEASED, buffer, length );
        } else if (buf[0] == 133 && lastkey > 0) {
            keymap_decode( g_outputmode, lastkey, KEY_RELEASED, buffer, length );
            lastkey = 0;
        }
        /* Eat repeated code */
        if( sceIoRead(g_irdafd, buf, 1) != 1 )
            return (-1);
        return 0;
    } else {
        checkkey = buf[0];
/*        if (symb)
            buf[0]=micro_datapad_function[buf[0]];
        else*/
            buf[0] = micro_datapad_normal[buf[0]];

        if (buf[0] != 0) {
            if (checkkey > 4)
                lastkey = buf[0];  /* Not a modifier, record key */
            keymap_decode( g_outputmode, (unsigned short) buf[0], KEY_PRESSED, buffer, length );
        }
    }

    return 0;
}


static int compaq_microkbd(unsigned char* buffer, int *length)
{
    unsigned char buf[2];
    unsigned char cnew;
    static char symb=0;

    if( sceIoRead(g_irdafd, buf, 2) != 2 )
        return (-1);

    /* resume display */
    scePowerTick(0);

    //if (debug)
    //    fprintf(stderr, "got %d %d (0x%x 0x%x)\n", buf[0], buf[1], buf[0], buf[1]);
    if (buf[0] & 0x80) { /* release */
        buf[0] &= 0x7f;
        if (buf[0] == 2) {
            symb = 0;
            return 0;
        }
        if (symb)
            cnew = compaq_function[buf[0]];
        else
            cnew = compaq_normal[buf[0]];

        //if (debug)
        //    fprintf(stderr, " release cnew=%d (0x%x)\n", cnew, cnew);
        if (cnew != 0)
            keymap_decode( g_outputmode, (unsigned short) cnew, KEY_RELEASED, buffer, length );
    } else { /* press */
        if (buf[0] == 2) {
            symb = 1;
            return 0;
        }
        if (symb)
            cnew = compaq_function[buf[0]];
        else
            cnew = compaq_normal[buf[0]];
        //if (debug)
        //    fprintf(stderr, " press cnew=%d (0x%x)\n", cnew, cnew);
        if (cnew != 0)
            keymap_decode( g_outputmode, (unsigned short) cnew, KEY_PRESSED, buffer, length );
    }

    return 0;
}

static int targus_infrared(unsigned char* buffer, int *length)
{
    unsigned char keycode=0;
    unsigned int  key_down=0;
    unsigned char buf1;
    int len;
    static int pi = 0;
    static unsigned char pbuf[3] = {0,0,0};

loop:
    len = sceIoRead(g_irdafd, &buf1, 1);
    if (len <= 0)
        return -1;
    // must be 1... can't be anything else at this point
    if (pi == 0)
    {
        // wait on 0xc0
        if (buf1 == 0xc0)
        {
            pbuf[0] = 0xc0;
            pi++;
        }
        goto loop;
    }

    pbuf[pi] = buf1;
    pi++;
    if (pi < 3)
        goto loop;

    pi = 0;

    /* resume display */
    scePowerTick(0);

    //printf( "0%c - %d\n", buf[0], buf[0] );
    //printf( "1%c - %d\n", buf[1], buf[1] );
    //printf( "5%c - %d\n", buf[2], buf[2] );

    if( pbuf[0] != 0xc0 || pbuf[2] != 0xc1 )
        return -1;

    /* 2rd pos is the key we need */
    keycode = pbuf[1];

    if( keycode < 128 )
        key_down = 1;
    else
        keycode -= 128;

    if ( key_down ) {
        //if (debug)
            //fprintf(stdout,"press %d\n", keycode);
        keymap_decode( g_outputmode, targus_normal[keycode], KEY_PRESSED, buffer, length );
    } else {
        //if (debug)
            //fprintf(stderr,"release %d\n", keycode);
        keymap_decode( g_outputmode, targus_normal[keycode], KEY_RELEASED, buffer, length );
    }

    return 0;
}

static int palmuw(unsigned char* buffer, int *length)
{
    unsigned char keycode=0;
    unsigned int  key_down=0;
    unsigned char buf1;
    int len;
    static int pi = 0;
    static unsigned char pbuf[6] = {0,0,0,0,0,0};

loop:
    len = sceIoRead(g_irdafd, &buf1, 1);
    if (len <= 0)
        return -1;
    // must be 1... can't be anything else at this point
    if (pi == 0)
    {
        // wait on 0xff
        if (buf1 == 0xff)
        {
            pbuf[0] = 0xff;
            pi++;
        }
        goto loop;
    }

    pbuf[pi] = buf1;
    pi++;
    if (pi < 6)
        goto loop;

    pi = 0;

    /* resume display */
    scePowerTick(0);

    //printf( "0%c - %d\n", buf[0], buf[0] );
    //printf( "1%c - %d\n", buf[1], buf[1] );
    //printf( "5%c - %d\n", buf[5], buf[5] );

    if( pbuf[0] != 0xff || pbuf[1] != 0xc0 || pbuf[5] != 0xc1 )
        return -1;

    //printf( "2%c - %d\n", buf[2], buf[2] );
    //printf( "3%c - %d\n", buf[3], buf[3] );

    if( ( pbuf[2] + pbuf[3] ) != 0xff )
        return -1;

    /* 3rd pos is the key we need */
    keycode = pbuf[2];

    if( keycode < 128 )
        key_down = 1;
    else
        keycode -= 128;

    if ( key_down ) {
        //if (debug)
            //fprintf(stdout,"press %d\n", keycode);
        keymap_decode( g_outputmode, palmuw_normal[keycode], KEY_PRESSED, buffer, length );
    } else {
        //if (debug)
            //fprintf(stderr,"release %d\n", keycode);
        keymap_decode( g_outputmode, palmuw_normal[keycode], KEY_RELEASED, buffer, length );
    }

    return 0;
}

static int palm1(unsigned char* buffer, int *length)
{
    unsigned char keycode=0;
    unsigned int  key_down=0;
    unsigned char buf1;
    int len;
    static int pi = 0;
    static int blue = 0;
    static int green = 0;
    static unsigned char pbuf[5] = {0,0,0,0,0};

loop:
    len = sceIoRead(g_irdafd, &buf1, 1);
    if (len <= 0)
        return -1;
    // must be 1... can't be anything else at this point
    if (pi == 0)
    {
        // wait on 0xc0
        if (buf1 == 0xc0)
        {
            pbuf[0] = 0xc0;
            pi++;
        }
        goto loop;
    }

    pbuf[pi] = buf1;
    pi++;
    if (pi < 5)
        goto loop;

    pi = 0;

    /* resume display */
    scePowerTick(0);

    //printf( "0%c - %d\n", buf[0], buf[0] );
    //printf( "1%c - %d\n", buf[1], buf[1] );
    //printf( "4%c - %d\n", buf[4], buf[4] );

    if( pbuf[0] != 0xc0 || pbuf[4] != 0xc1 )
        return -1;

    //printf( "2%c - %d\n", buf[2], buf[2] );
    //printf( "3%c - %d\n", buf[3], buf[3] );

// don't have any idea how 2 and 3 work at the moment
//    if( ( pbuf[2] + pbuf[3] ) != 0xff )
//        return -1;

    /* 2nd pos is the key we need */
    keycode = pbuf[1];

    switch (keycode)
    {
        case 0x22:
        // BlueFn pressed
        blue = 1;
        return -1;
        case 0xa2:
        // BlueFn released
        blue = 0;
        return -1;
        case 0x24:
        // GreenFn pressed
        green = 1;
        return -1;
        case 0xa4:
        // GreenFn released
        green = 0;
        return -1;
    }

    if( keycode < 128 )
        key_down = 1;
    else
        keycode -= 128;

    if (blue)
    {
        if ( key_down ) {
            //if (debug)
                //fprintf(stdout,"press %d\n", keycode);
            keymap_decode( g_outputmode, palm1_blue[keycode], KEY_PRESSED, buffer, length );
        } else {
            //if (debug)
                //fprintf(stderr,"release %d\n", keycode);
            keymap_decode( g_outputmode, palm1_blue[keycode], KEY_RELEASED, buffer, length );
        }
    }
    else if (green)
    {
        if ( key_down ) {
            //if (debug)
                //fprintf(stdout,"press %d\n", keycode);
            keymap_decode( g_outputmode, palm1_green[keycode], KEY_PRESSED, buffer, length );
        } else {
            //if (debug)
                //fprintf(stderr,"release %d\n", keycode);
            keymap_decode( g_outputmode, palm1_green[keycode], KEY_RELEASED, buffer, length );
        }
    }
    else
    {
        if ( key_down ) {
            //if (debug)
                //fprintf(stdout,"press %d\n", keycode);
            keymap_decode( g_outputmode, palm1_normal[keycode], KEY_PRESSED, buffer, length );
        } else {
            //if (debug)
                //fprintf(stderr,"release %d\n", keycode);
            keymap_decode( g_outputmode, palm1_normal[keycode], KEY_RELEASED, buffer, length );
        }
    }

    return 0;
}

static int hama(unsigned char* buffer, int *length)
{
    unsigned char keycode=0;
    unsigned int  key_down=0;
    unsigned char buf1;
    int len;
    static int pi = 0;
    static unsigned char pbuf[6] = {0,0,0,0,0,0};

loop:
    len = sceIoRead(g_irdafd, &buf1, 1);
    if (len <= 0)
        return -1;
    // must be 1... can't be anything else at this point
    if (pi == 0)
    {
        // wait on 0xC0
        if (buf1 == 0xC0)
        {
            pbuf[0] = 0xC0;
            pi++;
        }
        goto loop;
    }

    pbuf[pi] = buf1;
    pi++;
    if (pi < 5)
        goto loop;

    pi = 0;

    /* resume display */
    scePowerTick(0);

//  printf( "0  - %d\n",  buf[0] );
//  printf( "1  - %d\n",  buf[1] );
//  printf( "2  - %d\n",  buf[2] );
//  printf( "3  - %d\n",  buf[3] );
//  printf( "4  - %d\n",  buf[4] );
//  printf( "5  - %d\n",  buf[5] );

    /* 3rd pos is the key we need */
    keycode = pbuf[1];

    if( keycode < 72 )
        key_down = 1;
    else
        keycode -= 72;

    if ( key_down ) {
        //if (debug)
            //fprintf(stdout,"press %d\n", keycode);
        keymap_decode( g_outputmode, hama_keymap[keycode], KEY_PRESSED, buffer, length );
    } else {
        //if (debug)
            //fprintf(stderr,"release %d\n", keycode);
        keymap_decode( g_outputmode, hama_keymap[keycode], KEY_RELEASED, buffer, length );
    }

    return 0;
}
