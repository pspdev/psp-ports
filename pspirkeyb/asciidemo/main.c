/*
    asciidemo: PSP IR Keyboard Library ascii demo application (http://pspirkeyb.podzone.net)
    Copyright (C) 2007 Harald Fielker <harald.fielker@web.de>

    This program can be distributed under the terms of the GNU LGPL.
    See the file LICENSE.
*/

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>

#include <pspirkeyb.h>
#include <pspirkeyb_rawkeys.h>

#define printf pspDebugScreenPrintf


PSP_MODULE_INFO("asciidemo", PSP_MODULE_USER, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-256);


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

int main(int argc, char *argv[])
{
    SceCtrlData pad;
    u32 buttonsold = 0;
    int kernelmode = 0;       /* only 0 works for now - some keyboards need baud change */
    const char *config_file = NULL; /* this will force ms0:/seplugins/pspirkeyb.ini */

	SetupCallbacks();

    pspDebugScreenInit();
    printf("PSP Irda Keyboard test - ASCii input \n");

	if (sceKernelDevkitVersion() >= 0x03080010)
	{
        /* Load irda PRX for CFW >= 3.80 - Thanks, ZX81! */
        u32 mod_id = sceKernelLoadModule("flash0:/kd/irda.prx", 0, NULL);
        sceKernelStartModule(mod_id, 0, NULL, NULL, NULL);
	}

    if( pspIrKeybInit( config_file, kernelmode ) != 0 )
    {
        printf( "error: can't inialize the keyboard\n" );
        printf( "       check keyboard type/map in ms0:/seplugins/pspirkeyb.ini\n" );
    }
    else
    {
        unsigned char termchar = 'X';
        unsigned char buffer[255];
        int i, length=0;

        printf("\npress %c on keyboard or any PSP button to exit\n", termchar );

        /* setup output method to ASCii */
        pspIrKeybOutputMode( PSP_IRKBD_OUTPUT_MODE_ASCII );

        while(1) {
            length = 0;
            /* non blocking read */
            if( pspIrKeybReadinput(buffer, &length) >= 0 )
            {
                for( i=0; i < length; i++ )
                    printf( "%c", buffer[i] );
                if( length == 1 && buffer[0] == termchar )
                    break;
            }
            else
            {
                sceKernelDelayThread(10*1000);
                sceCtrlReadBufferPositive(&pad, 1);
                if (pad.Buttons != buttonsold)
                    break;
            }
        }

        /* bye keyboard */
        pspIrKeybFinish();
    }

    printf( "\n bye... (PRESS psp button to quit)\n" );

    buttonsold = 0;
    while (1) {
        sceCtrlReadBufferPositive(&pad, 1);
        if (pad.Buttons != buttonsold) {
            /* Exit */
            sceKernelExitGame();
        }
    }

	return 0;
}
