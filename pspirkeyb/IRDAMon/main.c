/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * main.c - IrDA Monitor
 *
 * Copyright (c) 2005 Christopher Phillips <chris@drvegetable.com> (aka Dr. Vegetable)
 * Based on the work of Frank Buss <fb@frank-buss.de> (aka Shine)
 *
 * $Id$
 */

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspsircs.h>
#include <stdlib.h>
#include <string.h>

/* Define the module info section */
PSP_MODULE_INFO("IRDAMon", 0, 1, 1);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

/* Define printf, just to make typing easier */
#define printf	pspDebugScreenPrintf

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

void clearScreen(void)
{
	pspDebugScreenInit();
	printf("    IrDA Monitor by Dr. Vegetable, based on IrDA sample by Shine\n");
        printf("      CROSS: insert newline, SQUARE: clear screen, CIRCLE: quit\n");
        printf("--------------------------------------------------------------------");
}

int main(void)
{
	SceCtrlData pad;
	u32 buttonsold = 0;

	SetupCallbacks();

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

	clearScreen();
	
	//Load the IRDA driver:
	
	u32 mod_id = sceKernelLoadModule("flash0:/kd/irda.prx", 0, NULL); 
	sceKernelStartModule(mod_id, 0, NULL, NULL, NULL);
	
	int fd = sceIoOpen("irda0:", PSP_O_RDWR, 0);

	while (1)
	{
		// read pad
		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons != buttonsold)
		{
			if (pad.Buttons & PSP_CTRL_CROSS)
				printf("\n");
			if (pad.Buttons & PSP_CTRL_SQUARE)
				clearScreen();
			if (pad.Buttons & PSP_CTRL_CIRCLE)
				sceKernelExitGame();

			buttonsold = pad.Buttons;
		}

		// check for IrDA input:
		unsigned char data;
		int len = sceIoRead(fd, &data, 1);
		if (len == 1)
			printf("%02X ", data);
		
		sceDisplayWaitVblankStart(); 
	}

	return 0;
}
