/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * main.c - Simple PRX example.
 *
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 *
 * $Id: main.c 1531 2005-12-07 18:27:12Z tyranid $
 */
#include <pspkernel.h>
#include <pspirkeyb.h>

PSP_MODULE_INFO("irkeyb", PSP_MODULE_USER, 1, 1);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int main(int argc, char **argv)
{
    /* do nothing at startup */
    sceKernelSleepThread();

	return 0;
}

/* Exported function */

int pspIrKeybInitPrx(const char* inifile, int kernelmode)
{
    return pspIrKeybInit(inifile, kernelmode);
}

int pspIrKeybOutputModePrx( int outputmode )
{
    return pspIrKeybOutputMode( outputmode );
}

int pspIrKeybGetOutputModePrx()
{
    return pspIrKeybGetOutputMode();
}

int pspIrKeybReadinputPrx(void* buffer, int *length)
{
    return pspIrKeybReadinput( buffer, length );
}

int pspIrKeybFinishPrx(void)
{
    return pspIrKeybFinish();
}
