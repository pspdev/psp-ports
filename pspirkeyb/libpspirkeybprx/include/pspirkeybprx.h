/*
    libpspirkeybprx: PSP IR Keyboard Library (http://pspirkeyb.podzone.net)
    Copyright (C) 2007 Harald Fielker <harald.fielker@web.de>

    This program can be distributed under the terms of the GNU LGPL.
    See the file LICENSE.
*/

#ifndef __PSPIRKEYBPRX_H
#define __PSPIRKEYBPRX_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PSPIRKEYB_HIDE_LIB_FUNCTIONS
#define __PSPIRKEYB_HIDE_LIB_FUNCTIONS
#endif
#include <pspirkeyb.h>

/*
 *          This is a wrapper around the prx.
 */

/**
 * Initalizes the keyboard from a ini file.
 *
 * @param inifile - Inifile. If this is NULL the default file 'ms0:/seplugins/pspirkeyb.ini' is used.
 * @param kernelmode - Initalize in kernel mode. 0 = no kernel - 1 = kernelmode. Kernel mode is needed for some keyboards to set the baudrate.
 *
 * @note the keyboard will be opened in ascii mode. If you set "kernelmode" to -1 and are not in kernel mode your application crashes.
 *
 * @returns PspIrKeyResults constant.
 */
int pspIrKeybInitPrx(const char* inifile, int kernelmode);

/**
 * Set the output mode of the skeyboard.
 *
 * @param outputmode - a PspIrKeyOutputModes constant.
 *
 * @returns PspIrKeyResults constant.
 */
int pspIrKeybOutputModePrx( int outputmode );

/**
 * Returns the current output mode of the keyboard.
 *
 * @returns PspIrKeyOutputModes constant.
 */
int pspIrKeybGetOutputModePrx();

/**
 * Nonblocking read data from the keyboard.
 *
 * The Keyboard input data goes to "buff". The result depends on the selected 
 * of pspIrKeybOutputMode().
 *
 * PSP_IRKBD_OUTPUT_MODE_ASCII
 *     returns unsigned char 
 * PSP_IRKBD_OUTPUT_MODE_VT100 
 *     returns unsigned char 
 * PSP_IRKBD_OUTPUT_MODE_SCANCODE
 *     returns SIrKeybScanCodeData pointers 
 * PSP_IRKBD_OUTPUT_MODE_RAW
 *     returns SIrKeybRawCodeData pointers
 *
 * @param buffer - Receive buffer. 
 * @param length - Length of the received data.
 *
 * @par Example:
 * @code
 * unsigned char buffer[255];
 * int length;
 * 
 * while( 1 ) {
 *      if( pspIrKeybReadinputÜrx( buffer, &length ) ==  PSP_IRKBD_RESULT_OK && length > 0)
 *          // Do something with the read keyboard data
 *          processKeyboardData( buffer, length );
 *      tick(); // this is a non blocking read
 * }
 * @endcode
 *
 * @returns PspIrKeyResults constant.
 */
int pspIrKeybReadinputPrx(void* buffer, int *length);

/**
 * Finishes the keyboard driver.
 *
 * @returns PspIrKeyResults constant.
 */
int pspIrKeybFinishPrx(void);

#ifdef __cplusplus
}
#endif

#endif

