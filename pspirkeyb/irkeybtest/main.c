/*
    irkeybtest: PSP IR Keyboard Library test application (http://pspirkeyb.podzone.net)
    Copyright (C) 2007 Harald Fielker <harald.fielker@web.de>

    This program can be distributed under the terms of the GNU LGPL.
    See the file LICENSE.
*/

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>

#include <string.h>

#include <pspirkeyb.h>
#include <pspirkeyb_rawkeys.h>

#define CONFIG_FILE         "host0:/pspirkeyb/libpspirkeyb/config/pspirkeyb-test.ini"
//#define CONFIG_FILE     NULL    /* this will take ms0:/seplugins/pspirkeyb.ini */

#define KERNELMODE          0     /* 1 is untested but required for some keyboards to change baudrate */

#define printf pspDebugScreenPrintf

/* Define the module info section */
PSP_MODULE_INFO("irkeybtest", PSP_MODULE_USER, 1, 1);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
{
	sceKernelExitGame();

	return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();

	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}

/*
 *  this will get VT100 chars from the keyboard
 */
void outputVT100Test()
{
    unsigned char buffer[255];
    unsigned char termchar = 0x03;
    int i, length;

    printf( "outputVT100Test\n" );
    printf("\npress ^C on keyboard to exit\n" );

    while(1) {
        if( pspIrKeybReadinput(buffer, &length) >= 0 )
        {
            for( i=0; i < length; i++ )
                printf( "%c", buffer[i] ); 
            if( length == 1 && buffer[0] == termchar )
                break;
        } 
        else 
        {
            // tick(); do what you want here... keyboard is not blocking
        }
    }
}

/*
 *  this will get ascii chars from the keyboard
 */
void outputAsciiTest()
{
    unsigned char termchar = 'X';
    unsigned char buffer[255];
    int i, length=0;
    
    printf( "outputAsciiTest\n" );
    printf("\npress %c on keyboard to exit\n", termchar );

    while(1) {
        if( pspIrKeybReadinput(buffer, &length) >= 0 )
        {
            for( i=0; i < length; i++ )
                printf( "%c", buffer[i] ); 
            if( length == 1 && buffer[0] == termchar )
                break;
        } 
        else 
        {
            // tick(); do what you want here... keyboard is not blocking
        }
    }
}

/*
 *  this will get raw chars from the keyboard
 */
void outputRawTest()
{
    unsigned char pressed, raw, termchar = KEY_X;
    unsigned char buffer[255];
    SIrKeybRawCodeData * rawData;
    int i, count, length;

    printf( "outputRawTest\n" );
    printf("\npress %s on keyboard to exit\n", pspIrKeybRawkeyName( termchar ) );

    while(1) {
        if( pspIrKeybReadinput(buffer, &length) >= 0 )
        {
            if( ( length % sizeof(SIrKeybRawCodeData) ) > 0 ) {
                printf( "error: read length is %d\n", length );
                break;
            }
            count = length / sizeof(SIrKeybRawCodeData);
            for( i=0; i < count; i++ )
            {
                rawData = (SIrKeybRawCodeData*)buffer+i;
                pressed = rawData->pressed;
                raw = rawData->raw;

                if( raw == KEY_RESERVED )
                {
                    printf( "DEAD KEY - please check driver\n" );
                }
                else if( raw >= (unsigned char)KEY_MAX )
                {
                    printf( "!illegal KEY - please check driver\n" );
                }
                else
                {
                    printf( "raw: %3d - '%18s' (%8s)\n", raw, 
                             pspIrKeybRawkeyName( raw ), pressed ? "pressed" : "released" );
                    if( raw == termchar )
                        return;
                }
            }
        } 
        else 
        {
            // tick(); do what you want here... keyboard is not blocking
        }
    }
}

/*
 *  this will get raw chars from the keyboard
 */
void outputScanCodeTest()
{
    unsigned char shift, ctrl, alt, pressed, raw, termchar = KEY_X;
    unsigned char buffer[255];
    SIrKeybScanCodeData * scanData;
    int i, length, count;

    printf( "outputScanCodeTest\n" );
    printf("\npress %s on keyboard to exit\n", pspIrKeybRawkeyName( termchar ) );

    while(1) {
        if( pspIrKeybReadinput(buffer, &length) >= 0 )
        {
            if( ( length % sizeof(SIrKeybScanCodeData) ) > 0 ) {
                printf( "error: read length is %d\n", length );
                break;
            }
            count = length / sizeof(SIrKeybScanCodeData);
            for( i=0; i < count; i++ )
            {
                scanData=(SIrKeybScanCodeData*) buffer+i;

                raw = scanData->raw;
                shift = scanData->shift;
                ctrl = scanData->ctrl;
                alt = scanData->alt;
                pressed = scanData->pressed;
                
                if( raw == KEY_RESERVED )
                {
                    printf( "DEAD KEY - please check driver\n" );
                }
                else if( raw >= (unsigned char)KEY_MAX )
                {
                    printf( "!illegal KEY - please check driver\n" );
                }
                else
                {
                    printf( "scan: %3d - '%18s' (%8s) %s %s %s\n", raw, 
                             pspIrKeybRawkeyName( raw ), pressed ? "pressed" : "released",
                             shift ? "shift" : "", alt ? "alt" : "", ctrl ? "ctrl" : "" );
                    if( raw == termchar )
                        return;
                }
            }
        } 
        else 
        {
            // tick(); do what you want here... keyboard is not blocking
        }
    }
}

int main(int argc, char *argv[])
{
    SceCtrlData pad;
    u32 buttonsold = 0;
    int ret;
    int outputmode = PSP_IRKBD_OUTPUT_MODE_VT100;

    /*
        possible output modes
                PSP_IRKBD_OUTPUT_MODE_ASCII
                PSP_IRKBD_OUTPUT_MODE_RAW
                PSP_IRKBD_OUTPUT_MODE_SCANCODE
                PSP_IRKBD_OUTPUT_MODE_VT100    
    */

	SetupCallbacks();
	pspDebugScreenInit();
    printf("PSP Irda Keyboard test\n");

    ret = pspIrKeybInit( CONFIG_FILE, KERNELMODE );
    if( ret == PSP_IRKBD_RESULT_OK )
    {
        pspIrKeybOutputMode( outputmode );

        if( outputmode == PSP_IRKBD_OUTPUT_MODE_ASCII )
            outputAsciiTest();
        else if( outputmode == PSP_IRKBD_OUTPUT_MODE_RAW )
            outputRawTest();
        else if( outputmode == PSP_IRKBD_OUTPUT_MODE_SCANCODE )
            outputScanCodeTest();
        else if( outputmode == PSP_IRKBD_OUTPUT_MODE_VT100 )
            outputVT100Test();

        if( pspIrKeybFinish() == 0 )
            printf( "\nclosed IR keyboard\n" );
        else
            printf( "\nerror: closing IR keyboard\n" );
    } else {
        switch( ret )
        {
            case PSP_IRKBD_RESULT_CANT_OPEN_DEVICE:
                printf( "error: can't open device\n" );
                break;
            case PSP_IRKBD_RESULT_CANT_OPEN_MAPFILE:
                printf( "error: can't open mapfile\n" );
                break;
            case PSP_IRKBD_RESULT_MAPFILE_MAXDEPTHLEVEL:
                printf( "error: mapfile max include level reached - recursion?\n" );
                break;
            case PSP_IRKBD_RESULT_CANT_OPEN_MAPFILE_INCLUDE:
                printf( "error: can't open include in mapfile\n" );
                break;
            case PSP_IRKBD_RESULT_CANT_SET_BAUDTATE:
                printf( "error: can't set baudrate - you need kernel support\n" );
                break; 
            case PSP_IRKBD_RESULT_CONFIG_FILE_NOT_FOUND:
                printf( "error: can't read config file\n" );
                break; 
            case PSP_IRKBD_RESULT_UNKNOW_KEYBOARD:
                printf( "error: unknown keyboard\n" );
                break;
            case PSP_IRKBD_RESULT_FAILED:
            default:
                printf( "error: init failed\n" );
                break;
        }
    }

    printf( "\n bye... (PRESS psp button to quit)\n" );

    while (1) {
        sceCtrlReadBufferPositive(&pad, 1);
        if (pad.Buttons != buttonsold) {
            /* Exit */
            sceKernelExitGame();
        }
    }


	return 0;
}
