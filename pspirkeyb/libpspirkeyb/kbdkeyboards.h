/*
    libpspirkeyb: PSP IR Keyboard Library (http://pspirkeyb.podzone.net)
    Copyright (C) 2007 Harald Fielker <harald.fielker@web.de>

    This program can be distributed under the terms of the GNU LGPL.
    See the file LICENSE.
*/

#include "pspirkeyb_rawkeys.h"

/* buttons on IPAQ 5450 (others too?) */
#define IPAQ_BUTTON_CALENDAR    219    /* keycode of calendar button */
#define IPAQ_BUTTON_CONTACTS    220    /* keycode of contacts button */
#define IPAQ_BUTTON_INBOX    155    /* keycode of email button */
#define IPAQ_BUTTON_ITASK    183    /* keycode of task (flip arrow) button */

#if 0
/***********************************************************************************
 *   iConcepts
 *
 *  9600 baud, 8N1
 *
 *  Notes:
 *   The shift, control, alt, and fn keys are the only keys that can be held
 *      down while pressing another key.
 *   All other keys generate keycode for down and "0x85 0x85" for up.
 *
 *  It seems to want an ACK of some kind?
 *
 *              Down       Up
 *   Control    1          9e 9e
 *   Function   2          9d 9d       (we use this as AltGr)
 *   Shift      3          9c 9c
 *   Alt        4          9b 9b       (we use this as Meta or Alt)
 *   OTHERS     keycode    85 85
 *
 ***********************************************************************************/

 static unsigned char iconcepts_normal[128] = {
        0, 29, 100, 42, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 10, 34, 24, 4, 30, 23, 16, 6, 46, 37, 31, 8, 18, 50, 22,
        0, 0, 0, 0, 0, 0, 0, 0, 45, 39, 28, 105, 44, 51, 1, 123,
        13, 53, 111, 125, 26, 15, 108, 87, 0, 0, 0, 0, 0, 0, 0, 0,
        47, 49, 33, 9, 20, 38, 32, 7, 19, 36, 48, 5, 25, 35, 11, 3,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        122, 110, 52, 12, 124, 57, 40, 21, 106, 58, 43, 17, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 103, 41, 27,  };

 static unsigned char iconcepts_numlock[128] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 73, 0, 77, 0, 0, 76, 0, 0, 0, 80, 0, 75, 0, 82, 75,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 74, 0, 0, 0, 51, 0, 0,
        0, 78, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 72, 0, 81, 0, 0, 0, 79, 0, 0, 55, 0, 98, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 83, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  };
#endif

/***********************************************************************************
 *   Snap N Type keyboard
 *
 * Key down sends single byte: KEY
 * Key up sends two bytes:     (KEY | 0x80), 0x7f
 *
 ***********************************************************************************/

#if 0
static unsigned char snapntype_normal[128] = {
      /* 000, 001, 002, 003, 004, 005, 006, 007, 008, 009 */
/*000*/    0,   0,   0,   0,   0,  36,   0,   0,  22,   0,
/*010*/   47,  34,  48,   0,   0, 115,  50,   0,   0,   0,
/*020*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*030*/    0,   0,  65,   0,   0,   0,   0,   0,   0,   0,
/*040*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*050*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*060*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*070*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*080*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*090*/    0,   0,   0,   0,   0,   0,   0,  38,  56,  54,
/*100*/   40,  26,  41,  42,  43,  31,  44,  45,  46,  58,
/*110*/   57,  32,  33,  24,  27,  39,  28,  30,  55,  25,
/*120*/   53,  29,  52,   0,   0,   0,   0,   0 };

#else
static unsigned char snapntype_normal[128] = {
        0, 0, 0, 0, 0, 29, 0, 0, 14, 0, 28, 104, 109, 0, 0, 1,
        42, 0, 122, 123, 124, 125, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0,
        57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 30, 48, 46, 32, 18, 33, 34, 35, 23, 36, 37, 38, 50, 49, 24,
        25, 16, 19, 31, 20, 22, 47, 17, 45, 21, 44, 0, 0, 0, 0, 0,  };

static unsigned char snapntype_symbol[128] = {
      /*   0, 001, 002, 003, 004, 005, 006, 007, 008, 009 */
/*000*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*010*/    0,  99, 105,   0,   0,   0,  66,   0,   0,   0,
/*020*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*030*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*040*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*050*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*060*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*070*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*080*/    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*090*/    0,   0,   0,   0,   0,   0,   0,   0,  59,   0,
/*100*/    0,  12,   0,   0,   0,  17, 100,  98, 102, 104,
/*110*/   60,  18,  19,  10,  13,   0,  14,  16,   0,  11,
/*120*/    0,  15,  61,   0,   0,   0,   0,   0 };
#endif


/***********************************************************************************
 *   Compaq Microkeyboard
 *
 * 4800 baud, 8N1
 *
 * Key down sends two bytes:  KEY          ~KEY (complement of KEY)
 * Key up sends two bytes:    (KEY | 0x80) ~KEY
 ***********************************************************************************/

 static unsigned char compaq_normal[128] = {
        0, 0, 100, 0, IPAQ_BUTTON_INBOX, IPAQ_BUTTON_CALENDAR, IPAQ_BUTTON_CONTACTS, 0, 0, 0, 0, 0, IPAQ_BUTTON_ITASK, 0, 0, 0,
        0, 0, 42, 0, 0, 16, 0, 0, 0, 29, 44, 31, 30, 17, 0, 0,
        0, 46, 45, 32, 18, 0, 0, 0, 103, 0, 47, 33, 20, 19, 0, 106,
        0, 49, 48, 35, 34, 21, 0, 0, 0, 0, 50, 36, 22, 0, 0, 0,
        0, 0, 37, 23, 24, 0, 0, 0, 0, 0, 0, 38, 0, 25, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 58, 0, 28, 0, 57, 0, 105, 0,
        108, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  };

static unsigned char compaq_function[128] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 42, 0, 0, KEY_1, 0, 0, 0, KEY_ESC, 44, KEY_SLASH, 30, KEY_2, 0, 0,
        0, 46, 45, 32, KEY_3, 0, 0, 0, 103, 0, 47, KEY_MINUS, KEY_5, KEY_4, 0, 106,
        0, KEY_COMMA, 48, KEY_SEMICOLON, KEY_EQUAL, KEY_6, 0, 0, 0, 0, KEY_DOT, KEY_APOSTROPHE, KEY_7, 0, 0, 0,
        0, 0, 37, KEY_8, KEY_9, 0, 0, 0, 0, 0, 0, 38, 0, KEY_0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, KEY_NUMLOCK, 0, KEY_TAB, 0, 57, 0, 105, 0,
        108, 0, 0, 0, 0, 0, KEY_DELETE, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  };

#if 0
/***********************************************************************************
 *   Stowaway XT
 *
 *  9600 baud, 8N1
 *
 *  Notes:
 *   the green function key is basically shift + scancode - handled elsewhere
 *
 ***********************************************************************************/
static unsigned char stowawayxt_normal[128] = {
  /* 000 */ 0, 0, 0, KEY_Z, 0, 0, 0, 0, KEY_LEFTMETA, KEY_Q,
  /* 010 */ KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, 0,KEY_X, KEY_A, KEY_S, KEY_D,
  /* 020 */ KEY_F, KEY_G, KEY_H, KEY_SPACE, KEY_CAPSLOCK, KEY_TAB, KEY_LEFTCTRL, 0, 0, 0,
  /* 030 */ 0, 0, 0, 0, 0, KEY_LEFTALT, 0, 0, 0, 0,
  /* 040 */ 0, 0, 0, 0, KEY_C, KEY_V, KEY_B, KEY_N, 0, 0,
  /* 050 */ KEY_BACKSPACE, 0, 0, 0, 0, KEY_SPACE, KEY_MINUS, KEY_EQUAL, KEY_SLASH, 0,
  /* 060 */ KEY_U, KEY_I, KEY_O, KEY_P, KEY_APOSTROPHE, KEY_ENTER, 0, 0, KEY_J, KEY_K,
  /* 070 */ KEY_L, KEY_SEMICOLON, KEY_UP, 0, 0, 0, KEY_M, KEY_COMMA, KEY_DOT, 0,
  /* 080 */ KEY_DELETE, KEY_LEFT, KEY_DOWN, KEY_RIGHT, 0, 0, 0, 0, KEY_LEFTSHIFT, KEY_RIGHTSHIFT,
  /* 090 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 100 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 110 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 120 */ 0, 0, 0, 0, 0, 0, 0, 0
};

static unsigned stowawayxt_function[128] = {
  /* 000 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, KEY_1,
  /* 010 */ KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, 0, 0, KEY_F9, KEY_F10, KEY_F11,
  /* 020 */ KEY_F12, 0, 0, 0, KEY_NUMLOCK, KEY_ESC, 0, 0, 0, 0,
  /* 030 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 040 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 050 */ 0, 0, 0, 0, 0, 0, KEY_LEFTBRACE, KEY_RIGHTBRACE, KEY_BACKSLASH, 0,
  /* 060 */ KEY_7, KEY_8, KEY_9, KEY_0, 0, 0, 0, 0, 0, 0,
  /* 070 */ 0, KEY_WWW, KEY_UP, 0, 0, 0, 0, KEY_INTL1, KEY_INTL2, 0,
  /* 080 */ 0, KEY_LEFT, KEY_DOWN, KEY_RIGHT, 0, 0, 0, 0, 0, 0,
  /* 090 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 100 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 110 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 120 */ 0, 0, 0, 0, 0, 0, 0, 0
};
#endif

/***********************************************************************************
 *   HP foldable keyboard
 *
 * 4800 baud, 8N1
 *
 * Key down sends two bytes:  KEY          ~KEY (complement of KEY)
 * Key up sends two bytes:    (KEY | 0x80) ~KEY
 ***********************************************************************************/
#if 0
unsigned char foldable_normal[128] = {
      /* 000,   001,   002,   003,   004,   005,   006,   007,   008,   009 */
/*000*/  000,   000,   000,   000,   000,   000,   000,   115,   000,   000,
/*010*/  000,   000,   000,    23,    49,   000,   000,   113,    50,   000,
/*020*/   37,    24, KEY_1,   000,   000,   000,    52,    39,    38,    25,
/*030*/  KEY_2, 000,   000,    54,    53,    40,    26, KEY_4, KEY_3,   000,
/*040*/   98,   000,    55,    41,    28,    27, KEY_5,   102,   000,    57,
/*050*/   56,    43,    42,    29, KEY_6,   000,   000,   000,    58,    44,
/*060*/   30, KEY_7, KEY_8,   000,   000,    59,    45,    31,    32, KEY_0,
/*070*/  KEY_9, 000,   000,    60,    61,    46,    47,    33,    20,   000,
/*080*/  000,   000,    51,   000,    34,    21,   000,   000,    66,    62,
/*090*/   36,    35,    65,    48,   100,   000,   104,   000,   000,   000,
/*100*/  000,   000,   107,   000,   000,   000,   000,   000,   000,   000,
/*110*/  000,   000,   112,    22,   000,   109,    94,   000,   000,   000,
/*120*/  000,   000,   000,   000,   000,   000,   000,   000 };

 static unsigned char foldable_function[128] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 124, 59, 0, 0, 0, 0, 0, 123, 85, 60, 0,
        0, 0, 0, 0, 89, 62, 61, 0, 104, 0, 0, 0, 125, 122, 63, 107,
        0, 0, 0, 0, 0, 90, 64, 0, 0, 0, 0, 0, 0, 65, 66, 0,
        0, 0, 0, 0, 0, 68, 67, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 69, 0, 91, 0, 0, 0, 102, 0,
        109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        125, 88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  };

 static unsigned char foldable_numlock[128] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 79, 75, 71, 72, 0,
        0, 0, 80, 76, 77, 55, 73, 0, 0, 83, 98, 81, 78, 74, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  };
#else
static unsigned short foldable_normal[128] = {
        0,  0, 128, 0, 0, 0, 0, 87, 0, 0, 0, 0, 0, 15, 41, 0,
        0, 134 /*100*/ /*AltGR*/, 42, 0, 29, 16, 2, 0, 0, 0, 44, 31, 30, 17, 3, 0,
        0, 46, 45, 32, 18, 5, 4, 0, 144/*103*/, 0, 47, 33, 20, 19, 6, 151/*106*/,
        0, 49, 48, 35, 34, 21, 7, 0, 0, 0, 50, 36, 22, 8, 9, 0,
        0, 51, 37, 23, 24, 11, 10, 0, 0, 52, 53, 38, 39, 25, 12, 0,
        0, 0, 43, 0, 26, 13, 0, 0, 58, 54, 28, 27, 57, 40, 146 /*105 lft*/, 0,
        161/*108*/, 0, 0, 0, 0, 0, 111, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        92, 14, 0, 97, 86, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  };

static unsigned short foldable_function[128] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 124, 59, 0, 0, 0, 0, 0, 123, 85, 60, 0,
        0, 0, 0, 0, 89, 62, 61, 0, 104, 0, 0, 0, 125, 122, 63, 107,
        0, 0, 0, 0, 0, 90, 64, 0, 0, 0, 0, 0, 0, 65, 66, 0,
        0, 0, 0, 0, 0, 68, 67, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 69, 0, 91, 0, 0, 0, 102, 0,
        109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        125, 88, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  };


#if 0
static unsigned short foldable_numlock[128] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 79, 75, 71, 72, 0,
        0, 0, 80, 76, 77, 55, 73, 0, 0, 83, 98, 81, 78, 74, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  };
#endif
#endif

/*
 *  map based on Palm 3169WW IR Keyboard
 *
 *  9600 baud, 8N1
 *
 *  key down sends: 0xFF 0xC0 key ~key key' 0xC1
 *  key up sends: 0xFF 0xC0 key+0x80 ~(key+0x80) key'+0x80 0xC1
 *
 */

static unsigned short palmuw_normal[128] = {
        0,  0,  KEY_COMPOSE,  KEY_LEFTMETA,  0,  0,  0,  0, // 00 -07
        0,  0,  0,  0,  0,  KEY_TAB,  KEY_GRAVE,  0, // 08 - 0F
        0,  KEY_LEFTALT,  KEY_LEFTSHIFT,  0,  KEY_LEFTCTRL,  KEY_Q,  KEY_1,  0, // 10 - 17
        0,  0,  KEY_Z,  KEY_S,  KEY_A,  KEY_W,  KEY_2,  KEY_DELETE, // 18 - 1F
        0, KEY_C,  KEY_X,  KEY_D,  KEY_E,  KEY_4,  KEY_3,  0, // 20 - 27
        KEY_UP,  KEY_SPACE,  KEY_V,  KEY_F,  KEY_T,  KEY_R,  KEY_5,  KEY_RIGHT, // 28 -2F
        KEY_RIGHTALT,  KEY_N,  KEY_B,  KEY_H,  KEY_G,  KEY_Y,  KEY_6,  0, // 30 - 37
        0,  0,  KEY_M,  KEY_J,  KEY_U,  KEY_7, KEY_8 , 0 , // 38 - 3F
        0,  0,  KEY_K,  KEY_I,  KEY_O,  KEY_0,  KEY_9,  0,  // 40 - 47
        KEY_COMMA,  KEY_DOT,  KEY_SLASH,  KEY_L,  KEY_SEMICOLON,  KEY_P,  KEY_MINUS,  0, // 48 - 4F
        0,  0,  KEY_APOSTROPHE,  0,  KEY_LEFTBRACE,  KEY_EQUAL,  0,  0, // 50 - 57
        KEY_CAPSLOCK,  KEY_RIGHTSHIFT,  KEY_ENTER,  KEY_RIGHTBRACE,  0,  KEY_BACKSLASH, KEY_LEFT,  0, // 58 - 5F
        KEY_DOWN,  0,  0,  0,  0,  0,  KEY_BACKSPACE,  0, // 60 - 67
        0,  0,  0,  0,  0,  0,  0,  0, // 68 - 6F
        0,  0,  0,  0,  0,  0,  0,  0, // 70 - 77
        0,  0,  0,  0,  0,  0,  0,  0  // 78 - 7F
};

/*
 *  map based on Palm One P10946-PROMO IR Keyboard
 *
 *  9600 baud, 8N1
 *
 *  key down sends: 0xFF 0xC0 key code1 code2 0xC1
 *  key up sends: 0xFF 0xC0 key+0x80 code3 code4 0xC1
 *
 */

static unsigned short palm1_normal[128] = {
        0,  0,  0,  KEY_Z,  0,  0,  0,  0, // 00 - 07
        KEY_LEFTMETA,  KEY_Q,  KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  0, // 08 - 0F
        KEY_X,  KEY_A,  KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_SPACE, // 10 - 17
        KEY_CAPSLOCK,  0,  KEY_LEFTCTRL,  KEY_TAB,  0,  0,  0,  0, // 18 - 1F
        0,  0,  0,  KEY_LEFTALT,  0,  0,  0,  0, // 20 - 27
        0,  0,  0,  0,  KEY_C,  KEY_V,  KEY_B,  KEY_N, // 28 - 2F
        0,  0,  KEY_BACKSPACE,  0,  0,  0,  0,  KEY_SPACE, // 30 - 37
        KEY_MINUS,  KEY_EQUAL,  KEY_SLASH,  0,  KEY_U,  KEY_I,  KEY_O,  KEY_P, // 38 - 3F
        0,  0,  0,  0,  KEY_J,  KEY_K,  KEY_L,  KEY_SEMICOLON, // 40 - 47
        KEY_UP,  0,  0,  0,  KEY_M,  KEY_COMMA,  KEY_DOT,  0, // 48 - 4F
        KEY_DELETE,  KEY_LEFT,  KEY_DOWN,  KEY_RIGHT,  0,  0,  KEY_APOSTROPHE,  KEY_ENTER, // 50 - 57
        KEY_LEFTSHIFT,  KEY_RIGHTSHIFT,  0,  0,  0,  0,  0,  0, // 58 - 5F
        0,  0,  0,  0,  0,  0,  0,  0, // 60 - 67
        0,  0,  0,  0,  0,  0,  0,  0, // 68 - 6F
        0,  0,  0,  0,  0,  0,  0,  0, // 70 - 77
        0,  0,  0,  0,  0,  0,  0,  0  // 78 - 7F
};

static unsigned short palm1_blue[128] = {
        0,  0,  0,  KEY_Z,  0,  0,  0,  0, // 00 - 07
        KEY_LEFTMETA,  KEY_1,  KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6,  0, // 08 - 0F
        KEY_X,  KEY_A,  KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_SPACE, // 10 - 17
        KEY_NUMLOCK,  0,  KEY_LEFTCTRL,  KEY_TAB,  0,  0,  0,  0, // 18 - 1F
        0,  0,  0,  KEY_LEFTALT,  0,  0,  0,  0, // 20 - 27
        0,  0,  0,  0,  KEY_C,  KEY_V,  KEY_B,  KEY_N, // 28 - 2F
        0,  0,  KEY_BACKSPACE,  0,  0,  0,  0,  KEY_SPACE, // 30 - 37
        KEY_LEFTBRACE,  KEY_RIGHTBRACE,  KEY_BACKSLASH,  0,  KEY_7,  KEY_8,  KEY_9,  KEY_0, // 38 - 3F
        0,  0,  0,  0,  KEY_J,  KEY_K,  KEY_L,  KEY_SEMICOLON, // 40 - 47
        KEY_PAGEUP,  0,  0,  0,  KEY_M,  KEY_COMMA,  KEY_DOT,  0, // 48 - 4F
        KEY_DELETE,  KEY_HOME,  KEY_PAGEDOWN,  KEY_END,  0,  0,  KEY_GRAVE,  KEY_ENTER, // 50 - 57
        KEY_LEFTSHIFT,  KEY_RIGHTSHIFT,  0,  0,  0,  0,  0,  0, // 58 - 5F
        0,  0,  0,  0,  0,  0,  0,  0, // 60 - 67
        0,  0,  0,  0,  0,  0,  0,  0, // 68 - 6F
        0,  0,  0,  0,  0,  0,  0,  0, // 70 - 77
        0,  0,  0,  0,  0,  0,  0,  0  // 78 - 7F
};

static unsigned short palm1_green[128] = {
        0,  0,  0,  KEY_Z,  0,  0,  0,  0, // 00 - 07
        KEY_RIGHTMETA,  KEY_F1,  KEY_F2,  KEY_F3,  KEY_F4,  KEY_F5,  KEY_F6,  0, // 08 - 0F
        KEY_X,  KEY_A,  KEY_S,  KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_SPACE, // 10 - 17
        KEY_CAPSLOCK,  0,  KEY_RIGHTCTRL,  KEY_TAB,  0,  0,  0,  0, // 18 - 1F
        0,  0,  0,  KEY_RIGHTALT,  0,  0,  0,  0, // 20 - 27
        0,  0,  0,  0,  KEY_C,  KEY_V,  KEY_B,  KEY_N, // 28 - 2F
        0,  0,  KEY_BACKSPACE,  0,  0,  0,  0,  KEY_SPACE, // 30 - 37
        KEY_F11,  KEY_F12,  KEY_SLASH,  0,  KEY_F7,  KEY_F8,  KEY_F9,  KEY_F10, // 38 - 3F
        0,  0,  0,  0,  KEY_J,  KEY_K,  KEY_L,  KEY_SEMICOLON, // 40 - 47
        KEY_PAGEUP,  0,  0,  0,  KEY_M,  KEY_COMMA,  KEY_DOT,  0, // 48 - 4F
        KEY_DELETE,  KEY_HOME,  KEY_PAGEDOWN,  KEY_END,  0,  0,  KEY_ESC,  KEY_ENTER, // 50 - 57
        KEY_LEFTSHIFT,  KEY_RIGHTSHIFT,  0,  0,  0,  0,  0,  0, // 58 - 5F
        0,  0,  0,  0,  0,  0,  0,  0, // 60 - 67
        0,  0,  0,  0,  0,  0,  0,  0, // 68 - 6F
        0,  0,  0,  0,  0,  0,  0,  0, // 70 - 77
        0,  0,  0,  0,  0,  0,  0,  0  // 78 - 7F
};

/*
 *  map based on Targus PA870v2.0 IR Keyboard
 *
 *  9600 baud, 8N1
 *
 *  key down sends: n * 0xFF 0xC0 key 0xC1
 *  key up sends: n * 0xFF 0xC0 key+0x80 0xC1
 *  note - the number of 0xFF bytes varies
 *
 */

static unsigned short targus_normal[128] = {
        0,  KEY_ESC,  KEY_1,  KEY_2,  KEY_3,  KEY_4,  KEY_5,  KEY_6, // 00 - 07
        KEY_7,  KEY_8,  KEY_9,  KEY_0,  KEY_MINUS,  KEY_EQUAL,  KEY_BACKSPACE,  KEY_TAB, // 08 - 0F
        KEY_Q,  KEY_W,  KEY_E,  KEY_R,  KEY_T,  KEY_Y,  KEY_U,  KEY_I, // 10 - 17
        KEY_O,  KEY_P,  KEY_LEFTBRACE,  KEY_RIGHTBRACE,  KEY_ENTER,  KEY_LEFTCTRL,  KEY_A,  KEY_S, // 18 - 1F
        KEY_D,  KEY_F,  KEY_G,  KEY_H,  KEY_J,  KEY_K,  KEY_L,  KEY_SEMICOLON, // 20 - 27
        KEY_APOSTROPHE,  KEY_GRAVE,  KEY_LEFTSHIFT,  KEY_BACKSLASH,  KEY_Z,  KEY_X,  KEY_C,  KEY_V, // 28 - 2F
        KEY_B,  KEY_N,  KEY_M,  KEY_COMMA,  KEY_DOT,  KEY_SLASH,  KEY_RIGHTSHIFT,  0, // 30 - 37
        KEY_LEFTALT,  KEY_SPACE,  KEY_CAPSLOCK,  KEY_HOME,  KEY_END,  KEY_PAGEUP,  KEY_PAGEDOWN,  0, // 38 - 3F
        0,  0,  0,  0,  0,  0,  0,  0, // 40 - 47
        KEY_UP,  0,  0,  KEY_LEFT,  0,  KEY_RIGHT,  0,  0, // 48 - 4F
        KEY_DOWN,  0,  0,  0,  0,  0,  0,  0, // 50 - 57
        0,  0,  0,  0,  0,  0,  0,  0, // 58 - 5F
        0,  KEY_COMPOSE,  KEY_LEFTMETA,  KEY_SPACE,  KEY_RIGHTMETA,  KEY_RIGHTALT,  KEY_DELETE,  0, // 60 - 67
        KEY_ENTER,  0,  0,  0,  0,  0,  0,  0, // 68 - 6F
        0,  0,  0,  0,  0,  0,  0,  0, // 70 - 77
        0,  0,  0,  0,  0,  0,  0,  0  // 78 - 7F
};

#if 0

/***********************************************************************************
 * Eagle-Touch Portable PDA keyboard
 *
 * 9600 baud, 8N1
 *
 * Key down sends:  KEY
 * Key up sends    ~KEY
 *
 * After a delay, keyboard sends 74 to indicate it's going low power
 * When you press a key after the delay, it sends:  75 6d 61 78
 *   followed by the normal KEY, ~KEY
 ***********************************************************************************/

 static unsigned char eagletouch_normal[128] = {
        0, 0, 128, 122, 88, 87, 124, 0, 85, 0, 89, 123, 90, 15, 40, 0,
        0, 125, 42, 0, 29, 16, 2, 0, 0, 0, 44, 31, 30, 17, 3, 111,
        0, 46, 45, 32, 18, 5, 4, 0, 103, 57, 47, 33, 20, 19, 6, 106,
        100, 49, 48, 35, 34, 21, 7, 0, 91, 0, 50, 36, 22, 8, 9, 0,
        56, 51, 37, 23, 24, 11, 10, 1, 0, 52, 53, 38, 39, 25, 12, 0,
        0, 0, 40, 0, 26, 13, 0, 0, 58, 54, 28, 27, 0, 43, 105, 0,
        108, 129, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 129, 0, 0,
        0, 0, 0, 0, 129, 129, 0, 0, 129, 0, 0, 0, 0, 0, 0, 0,  };

 static unsigned char eagletouch_function[128] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 59, 0, 0, 0, 0, 0, 0, 0, 60, 0,
        0, 0, 0, 0, 0, 62, 61, 0, 104, 0, 0, 0, 0, 0, 63, 107,
        0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 65, 66, 0,
        0, 0, 0, 0, 0, 68, 67, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 0,
        109, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  };

#endif

/***********************************************************************************
 * Micro Innovations IR keyboard
 *
 * 9600 baud, 8N1
 *
 * Key down sends one byte:  KEY
 * Key up sends one byte:    KEY | 0x80
 * Last key up repeats key up
 ***********************************************************************************/
static unsigned char micro_innovations_normal[128] = {
/* 0 */	0, KEY_Q, 0, KEY_Z, 0, 0, 0, KEY_A,
/* 8 */	0, KEY_W, 0, KEY_X, 0, 0, 0, KEY_S,
/* 16*/	0, KEY_E, 0, KEY_C, 0, 0, 0, KEY_D,
/* 24*/	0, KEY_R, 0, KEY_V, 0, 0, 0, KEY_F,
/* 32*/	0, KEY_T, 0, KEY_B, 0, 0, 0, KEY_G,
/* 40*/	0, KEY_Y, 0, KEY_SPACE, 0, 0, 0, KEY_H,
/* 48*/	0, KEY_U, KEY_J, KEY_N, KEY_SPACE, 0, 0, 0,
/* 56*/	0, KEY_I, KEY_K, KEY_M, 0, 0, 0, 0,
/* 64*/	KEY_F11, KEY_O, KEY_L, KEY_COMMA, KEY_LEFT, 0, 0, 0,
/* 72*/	KEY_F12, KEY_P, KEY_SEMICOLON, KEY_DOT, KEY_DOWN, 0, 0, 0,
/* 80*/	KEY_F13, KEY_MINUS, KEY_APOSTROPHE, KEY_UP, KEY_RIGHT, 0, 0, 0,
/* 88*/	KEY_F14, KEY_BACKSPACE, KEY_ENTER, KEY_DELETE, 0, 0, 0, 0,
/* 96*/	0, 0, 0, 0, 0, KEY_LEFTSHIFT, KEY_LEFTALT, 0,
/*104*/	0, KEY_TAB, 0, 0, 0, KEY_LEFTCTRL, 0, 0,
/*112*/	0, 0, 0, 0, 0, KEY_LEFTMETA, 0, KEY_CAPSLOCK,
/*120*/	0, 0, 0, 0, 0, KEY_RIGHTSHIFT, 0, 0
};

static unsigned char micro_innovations_num[128] = {
/* 0 */	0, KEY_1, 0, KEY_Z, 0, 0, 0, KEY_A,
/* 8 */	0, KEY_2, 0, KEY_X, 0, 0, 0, KEY_S,
/* 16*/	0, KEY_3, 0, KEY_C, 0, 0, 0, KEY_D,
/* 24*/	0, KEY_4, 0, KEY_V, 0, 0, 0, KEY_F,
/* 32*/	0, KEY_5, 0, KEY_B, 0, 0, 0, KEY_G,
/* 40*/	0, KEY_6, 0, KEY_SPACE, 0, 0, 0, KEY_H,
/* 48*/	0, KEY_7, KEY_4, KEY_N, KEY_SPACE, 0, 0, 0,
/* 56*/	0, KEY_8, KEY_5, KEY_1, KEY_0, 0, 0, 0,
/* 64*/	KEY_F11, KEY_9, KEY_6, KEY_2, KEY_DOT, 0, 0, 0,
/* 72*/	KEY_F12, KEY_0, KEY_KPASTERISK, KEY_3, KEY_KPPLUS, 0, 0, 0,
/* 80*/	KEY_F13, KEY_KPSLASH, KEY_APOSTROPHE, KEY_KPMINUS, KEY_EQUAL, 0, 0, 0,
/* 88*/	KEY_F14, KEY_BACKSPACE, KEY_ENTER, KEY_DELETE, 0, 0, 0, 0,
/* 96*/	0, 0, 0, 0, 0, KEY_LEFTSHIFT, KEY_LEFTALT, 0,
/*104*/	0, KEY_TAB, 0, 0, 0, KEY_LEFTCTRL, 0, 0,
/*112*/	0, 0, 0, 0, 0, KEY_NUMLOCK, 0, KEY_CAPSLOCK,
/*120*/	0, 0, 0, 0, 0, KEY_RIGHTSHIFT, 0, 0
};

/* Send shift before key */
#define FKEY_SHIFT 0x8000

static unsigned short micro_innovations_punct[128] = {
/* 0 */	0, KEY_1 | FKEY_SHIFT, 0, KEY_Z, 0, 0, 0, KEY_GRAVE,
/* 8 */	0, KEY_2 | FKEY_SHIFT, 0, KEY_X, 0, 0, 0, KEY_GRAVE | FKEY_SHIFT,
/* 16*/	0, KEY_3 | FKEY_SHIFT, 0, KEY_C, 0, 0, 0, KEY_SLASH,
/* 24*/	0, KEY_4 | FKEY_SHIFT, 0, KEY_V, 0, 0, 0, KEY_SLASH | FKEY_SHIFT,
/* 32*/	0, KEY_5 | FKEY_SHIFT, 0, KEY_B, 0, 0, 0, KEY_LEFTBRACE,
/* 40*/	0, KEY_6 | FKEY_SHIFT, 0, KEY_SPACE, 0, 0, 0, KEY_RIGHTBRACE,
/* 48*/	0, KEY_7 | FKEY_SHIFT, KEY_LEFTBRACE | FKEY_SHIFT, KEY_N, KEY_SPACE, 0, 0, 0,
/* 56*/	0, KEY_8 | FKEY_SHIFT, KEY_RIGHTBRACE | FKEY_SHIFT, KEY_M, 0, 0, 0, 0,
/* 64*/	KEY_F11, KEY_9 | FKEY_SHIFT, KEY_BACKSLASH, KEY_COMMA, KEY_LEFT, 0, 0, 0,
/* 72*/	KEY_F12, KEY_0 | FKEY_SHIFT, KEY_BACKSLASH | FKEY_SHIFT, KEY_DOT, KEY_DOWN, 0, 0, 0,
/* 80*/	KEY_F13, KEY_EQUAL | FKEY_SHIFT, KEY_EQUAL, KEY_UP, KEY_RIGHT, 0, 0, 0,
/* 88*/	KEY_F14, KEY_BACKSPACE, KEY_ENTER, KEY_DELETE, 0, 0, 0, 0,
/* 96*/	0, 0, 0, 0, 0, KEY_LEFTSHIFT, KEY_LEFTALT, 0,
/*104*/	0, KEY_TAB, 0, 0, 0, KEY_LEFTCTRL, 0, 0,
/*112*/	0, 0, 0, 0, 0, KEY_LEFTMETA, 0, KEY_CAPSLOCK,
/*120*/	0, 0, 0, 0, 0, KEY_RIGHTSHIFT, 0, 0
};

static unsigned char micro_innovations_func[128] = {
/* 0 */	0, KEY_F1, 0, KEY_Z, 0, 0, 0, KEY_A,
/* 8 */	0, KEY_F2, 0, KEY_X, 0, 0, 0, KEY_S,
/* 16*/	0, KEY_F3, 0, KEY_C, 0, 0, 0, KEY_D,
/* 24*/	0, KEY_F4, 0, KEY_V, 0, 0, 0, KEY_F,
/* 32*/	0, KEY_F5, 0, KEY_B, 0, 0, 0, KEY_G,
/* 40*/	0, KEY_F6, 0, KEY_SPACE, 0, 0, 0, KEY_H,
/* 48*/	0, KEY_F7, KEY_J, KEY_N, KEY_SPACE, 0, 0, 0,
/* 56*/	0, KEY_F8, KEY_K, KEY_M, 0, 0, 0, 0,
/* 64*/	KEY_F11, KEY_F9, KEY_L, KEY_COMMA, KEY_HOME, 0, 0, 0,
/* 72*/	KEY_F12, KEY_F10, KEY_SEMICOLON, KEY_DOT, KEY_PAGEDOWN, 0, 0, 0,
/* 80*/	KEY_F13, KEY_MINUS, KEY_APOSTROPHE, KEY_PAGEUP, KEY_END, 0, 0, 0,
/* 88*/	KEY_F14, KEY_BACKSPACE, KEY_ENTER, KEY_INSERT, 0, 0, 0, 0,
/* 96*/	0, 0, 0, 0, 0, KEY_LEFTSHIFT, KEY_LEFTALT, 0,
/*104*/	0, KEY_ESC, 0, 0, 0, KEY_LEFTCTRL, 0, 0,
/*112*/	0, 0, 0, 0, 0, KEY_LEFTMETA, 0, KEY_CAPSLOCK,
/*120*/	0, 0, 0, 0, 0, KEY_RIGHTSHIFT, 0, 0
};

/***********************************************************************************
 * Micro Innovations Foldaway keyboard
 *
 * 9600 baud, 8N1
 *
 * Key down sends one byte:  KEY
 * Key up sends one byte:    KEY | 0x80
 * Last key up repeats key up
 ***********************************************************************************/

static unsigned char micro_foldaway_normal[128] = {
  /* 00 */ 0,              KEY_LEFTCTRL,  0,              KEY_LEFTSHIFT, KEY_LEFTALT,
  /* 05 */ 0,              0,             0,              0,             0,
  /* 10 */ 0,              0,             0,              0,             0,
  /* 15 */ 0,              KEY_1,         KEY_9,          KEY_G,         KEY_O,
  /* 20 */ KEY_3,          KEY_A,         KEY_I,          KEY_Q,         KEY_5,
  /* 25 */ KEY_C,          KEY_K,         KEY_S,          KEY_7,         KEY_E,
  /* 30 */ KEY_M,          KEY_U,         0,              0,             0,
  /* 35 */ 0,              0,             0,              0,             0,
  /* 40 */ KEY_X,          KEY_SEMICOLON, KEY_ENTER,      KEY_LEFT,      KEY_Z,
  /* 45 */ KEY_COMMA,      KEY_SPACE,     IPAQ_BUTTON_CONTACTS, KEY_EQUAL, KEY_SLASH,
  /* 50 */ KEY_DELETE,     IPAQ_BUTTON_ITASK, KEY_LEFTBRACE, KEY_TAB,    KEY_DOWN,
  /* 55 */ KEY_LEFTMETA,   0,             0,              0,             0,
  /* 60 */ 0,              0,             0,              0,             KEY_V,
  /* 65 */ KEY_N,          KEY_F,         KEY_8,          KEY_T,         KEY_L,
  /* 70 */ KEY_D,          KEY_6,         KEY_R,          KEY_J,         KEY_B,
  /* 75 */ KEY_4,          KEY_P,         KEY_H,          KEY_0,         KEY_2,
  /* 80 */ 0,              0,             0,              0,             0,
  /* 85 */ 0,              0,             0,              0,             0,
  /* 90 */ 0,              0,             0,              0,             0,
  /* 95 */ 0,              IPAQ_BUTTON_CALENDAR, 0,       KEY_DOT,       KEY_MINUS,
  /*100 */ IPAQ_BUTTON_INBOX, KEY_SPACE,   KEY_APOSTROPHE, KEY_Y,        KEY_RIGHT,
  /*105 */ KEY_CAPSLOCK,   KEY_BACKSLASH, KEY_W,          0,             0,
  /*110 */ 0,              0,             0,              0,             0,
  /*115 */ 0,              0,             0,              0,             0,
  /*120 */ 0,              0,             0,              0,             KEY_BACKSPACE,
  /*125 */ KEY_UP,         KEY_GRAVE,     KEY_RIGHTBRACE
};

/***********************************************************************************
 * Micro Innovations Datapad
 *
 * 9600 baud, 8N1
 *
 * Same protocol as MicroInnovations Foldaway
 ***********************************************************************************/

static unsigned char micro_datapad_normal[128] = {
  /* 00 */ 0,              0,             0,              0,             0,
  /* 05 */ 0,              0,             0,              0,             0,
  /* 10 */ 0,              0,             0,              0,             0,
  /* 15 */ 0,              KEY_A,         KEY_B,          KEY_C,         KEY_D,
  /* 20 */ KEY_Q,          KEY_R,         KEY_S,          KEY_T,         KEY_RIGHT,
  /* 25 */ KEY_LEFTMETA,   0,             KEY_SPACE,      0,             0,
  /* 30 */ 0,              0,             0,              0,             0,
  /* 35 */ 0,              0,             0,              0,             0,
  /* 40 */ KEY_M,          KEY_N,         KEY_O,          KEY_P,         KEY_ENTER,
  /* 45 */ KEY_UP,         KEY_DOWN,      KEY_LEFT,       KEY_LEFTBRACE, KEY_RIGHTBRACE,
  /* 50 */ KEY_CAPSLOCK,   0,             0,              0,             0,
  /* 55 */ 0,              0,             0,              0,             0,
  /* 60 */ 0,              0,             0,              0,             0,
  /* 65 */ 0,              0,             0,   IPAQ_BUTTON_ITASK, IPAQ_BUTTON_CALENDAR,
  /* 70 */ IPAQ_BUTTON_CONTACTS, IPAQ_BUTTON_INBOX, KEY_DELETE, KEY_BACKSPACE, KEY_Z,
  /* 75 */ KEY_Y,          KEY_L,         KEY_K,          KEY_J,         KEY_I,
  /* 80 */ 0,              0,             0,              0,             0,
  /* 85 */ 0,              0,             0,              0,             0,
  /* 90 */ 0,              0,             0,              0,             0,
  /* 95 */ 0,              KEY_TAB,       KEY_APOSTROPHE, KEY_GRAVE,     KEY_SEMICOLON,
  /*100 */ KEY_X,          KEY_W,         KEY_V,          KEY_U,         KEY_H,
  /*105 */ KEY_G,          KEY_F,         KEY_E,          0,             0,
  /*110 */ 0,              0,             0,              0,             0,
  /*115 */ 0,              0,             0,              0,             0,
  /*120 */ 0,              0,             0,              0,             0,
  /*125 */ 0,              0,             0
};

#if 0
/***********************************************************************************
 * Grandtec PocketVIK     (www.grandtec.com)
 *
 * 57600 baud, 8N1
 *
 *  This keyboard autorepeats
 *  It does not generate key up
 *
 *  It may have a low power sleep mode that needs to be cycled????
 *
 *  Set 1:   ShiftL, ShiftR, CtrlL, AltL, AltR, CtrlR, Fn
 *  Set 2:   All others
 *
 *  Set 1 keys do not generate keystrokes.  They set fields in a bit mask.
 *
 *  MASK = 0x80 | (sum of active modifiers)
 *
 *  On keydown, you either get
 *
 *  Case 1 (no bit mask field)        KEY
 *  Case 2 (bitmask field)         MASK, KEY
 ***********************************************************************************/

 static unsigned char pocketvik_normal[128] = {
        0, 108, 105, 51, 0, 0, 57, 46, 0, 50, 103, 53, 52, 0, 106, 47,
        0, 44, 87, 28, 40, 0, 37, 35, 34, 45, 31, 0, 27, 39, 11, 36,
        21, 33, 32, 30, 16, 26, 25, 23, 22, 20, 19, 18, 17, 15, 13, 58,
        10, 9, 7, 6, 4, 3, 41, 14, 24, 12, 8, 5, 0, 122, 2, 38,
        111, 1, 48, 43, 14, 125, 123, 124, 49, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  };

 static unsigned char pocketvik_function[128] = {
        0, 109, 102, 0, 0, 0, 0, 0, 0, 0, 104, 0, 0, 0, 107, 0,
        0, 0, 0, 91, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 89, 0, 0,
        88, 0, 0, 0, 0, 92, 85, 90, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  };

#define POCKETVIK_Fn        0x40
struct {
    unsigned char mask;
    unsigned char key;
} pocketvik_modifiers[] = {
    { 0x01, KEY_LEFTSHIFT },
    { 0x02, KEY_RIGHTSHIFT },
    { 0x04, KEY_LEFTCTRL },
    { 0x08, KEY_RIGHTCTRL },
    { 0x10, KEY_LEFTALT },
    { 0x20, KEY_RIGHTALT }
};
#endif

/***********************************************************************************
 * Flexis FX-100
 *
 * 9600 baud, 8N1
 *
 * This keyboard autorepeats
 * It does not generate key up for most keys
 *
 * Set 1:   ShiftL, ShiftR, Ctrl, Alt, Fn
 * Set 2:   All others
 *
 *             Down             Up
 *             ====          ==
 *  Set 1      KEY | 0x80      KEY
 *  Set 2      KEY | 0x80, KEY    <none>
 *
 * If you press two keys from Set 1 you get the following:
 *
 * Press Key #1            KEY1_DOWN
 * Press Key #2            KEY1_UP
 * Release Key #1        KEY2_DOWN
 * Release Key #2        KEY2_UP
 ***********************************************************************************/

 static unsigned char flexis_fx100_normal[128] = {
        30, 31, 32, 33, 35, 34, 44, 45, 46, 47, 1, 48, 16, 17, 18, 19,
        21, 20, 2, 3, 4, 5, 7, 6, 13, 10, 8, 12, 9, 11, 27, 24,
        22, 26, 23, 25, 28, 38, 36, 40, 37, 39, 43, 51, 53, 49, 50, 52,
        15, 57, 41, 14, 0, 0, 0, 56, 54, 58, 128, 29, 42, 87, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 111, 105, 106, 108, 103, 0,  };

static  unsigned char flexis_fx100_function[128] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 123, 88, 85, 89, 0, 0, 90, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 91, 0, 0, 0, 92, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102, 107, 109, 104, 0,  };

/***********************************************************************************
 * HP Slim  keyboard
 *
 * 4800 baud, 8N1
 *
 * Key down sends one byte:  KEY
 * Key up sends one byte:    KEY | 0x80
 * Keys don't need translation (except for arrow keys).
 * If Fn key is pressed, then hpslim_symbol maps is used to translate
 * key codes.  Shift must be added to some symbol key codes.
 * For example Fn-Z is shown on the keyboard as a dollar sign,
 * so we translate it to  shift-4.
 * Haven't yet figured out the euro symbol, or the British pound.
 *
 * HP Slim Keyboard stuff blame to Tom Swiss tms@infamous.net
 * More stuff added by Gordon Maclean (maclean at ucar edu).
 ****************************************************************************/

 /* translations for keys pressed without Fn.
  * Not much translation necessary except for arrow keys
  * (KP_4,6,2,8)=(75,77,80,72)->(146,151,161,144)
  * and IPAQ buttons (113,114,117,116)->(219,220,155,183)
  */
 static unsigned char hpslim_normal[128] = {
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
        44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
        55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
        70, 71,144, 73, 74,146, 76,151, 78, 79,
       161, 81, 82, 83, 84, 85, 86, 87, 88, 89,
        90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
       100,101,102,103,104,105,106,107,108,109,
       110,111,112,219,220,115,155,183,118,119,
       120,121,122,123,124,125,126,127
};

 /* translations for Fn-key sequences */
 static unsigned char hpslim_symbol[128] = {
/*code   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15 */
/*key                                                            BS     */
/*sym                                                           DEL     */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,111,  0,

/*code  16  17  18  19  20  21  22  23  24  25  26  27  28  29 */
/*key    Q   W   E   R   T   Y   U   I   O   P  [   ]   CR  LC */
/*sym    1   2   3   4   5   6   7   8   9   0          TB ESC */
         2,  3,  4,  5,  6,  7,  8,  9, 10, 11,  0,  0, 15,  1,

/*code  30  31  32  33  34  35  36  37  38  39  40  41  42  43 */
/*key    A   S   D   F   G   H   J   K   L   ;  AP GRAV LS   \ */
/*sym    *   /   +   -    =  :   '   "   @                     */
        55, 53, 78, 12, 13, 39, 41, 40,  3,  0,  0,  0,  0,  0,
/*shft                       y       y   y                     */

/*code  44  45  46  47  48  49  50  51  52  53  54  */
/*key    Z   X   C   V   B   N   M   ,   .   /  RS  */
/*sym    $   (   )   ?   !   ,   .  NUM             */
         5, 10, 11, 53,  2, 51, 52, 69,  0,  0,  0,
/*shft   y   y   y   y   y                          */

/*code  55  56  57  58  59  60  61  62  63  64  65  66  67  68  69 */
/*key              CAP                                             */
/*sym              NUM                                             */
         0,  0,  0, 69,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

/*code  70  71  72  73  74  75  76  77  78  79 */
/*key          KP8         KP4     KP6         */
/*sym            _           %      \          */
         0,  0, 12,  0,  0,  6,  0, 43,  0,  0,
/*shft           y           y                 */

/*code  80  81  82  83  84  85  86  87  88  89 */
/*key  KP2                                     */
/*sym  euro                                   */
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

/*code  90  91  92  93  94  95  96  97  98  99 */
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

/*code 100 101 102 103 104 105 106 107 108 109 */
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0};

 /* whether we need to add a shft to get a symbol */
 unsigned char hpslim_symshft[128] = {
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,/*0-15*/
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    /*16-29*/
         0,  0,  0,  0,  0,  1,  0,  1,  1,  0,  0,  0,  0,  0, /*30-43*/
         1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,        /*44-54*/
         0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,/*55-69*/
         0,  0,  1,  0,  0,  1,  0,  0,  0,  0,        /*70-79*/
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,        /*80-89*/
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,        /*90-99*/
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,        /*100-109*/
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,        /*110-119*/
     0,  0,  0,  0,  0,  0,  0,  0};


#define MKBD_HPS_LOCKKEY 0x3a
#define MKBD_HPS_NUMLOCKSCAN 0x45
#define MKBD_HPS_FNKEY 0x73


/***********************************************************************************
 *    Freedom keyboard
 *
 *    David Lapetina ntic at lapetina dot org
 *
 *    Key down sends one byte: (KEY)
 *    Key up sends  one byte:  (KEY+63(!!))
 ***********************************************************************************/
static unsigned char freedom_kbd[63] = {
/* keys without modificators */
/* 00 */ KEY_RESERVED,     KEY_1,         KEY_2,         KEY_3,         KEY_4,
/* 05 */ KEY_5,         KEY_6,      KEY_A,         KEY_Z,         KEY_E,
/* 10 */ KEY_R,         KEY_T,         KEY_Q,         KEY_S,         KEY_D,
/* 15 */ KEY_F,         KEY_G,         KEY_W,         KEY_X,         KEY_C,
/* 20 */ KEY_V,         KEY_B,         KEY_SPACE,     KEY_TAB,     KEY_CAPSLOCK,
/* 25 */ KEY_MENU/*HOME*/,    KEY_7,         KEY_8,         KEY_9,         KEY_0,
/* 30 */ KEY_RIGHTBRACE,    KEY_EQUAL,    KEY_BACKSPACE,    KEY_Y,         KEY_U,
/* 35 */ KEY_I,         KEY_O,         KEY_P,         KEY_APOSTROPHE,    KEY_SPACE/*$*/,
/* 40 */ KEY_H,         KEY_J,         KEY_K,         KEY_L,         KEY_M,
/* 45 */ KEY_SPACE/*.*/,    KEY_ENTER,     KEY_N,         KEY_COMMA,     KEY_SEMICOLON,
/* 50 */ KEY_SLASH,     0/*!*/,     KEY_UP,     KEY_SPACE,     KEY_ESC /*key with "circle"*/,
/* 55 */ KEY_LEFT,     KEY_DOWN,     KEY_RIGHT,     KEY_LEFTCTRL,     KEY_RIGHTSHIFT,
/* 60 */ KEY_LEFTSHIFT,     KEY_DELETE,     0
};

/***********************************************************************************
 *    Belkin Wireless PDA IRDA Type keyboard
 *
 *    Alex Lange
 *    chicken@handhelds.org
 *
 *
 ***********************************************************************************/

static unsigned char belkin_irda_normal[128] = {
        0, 0, 0, 0, 0, 0, 0, 0, 14, 15, 0, 0, 0, 28, 0, 0,
        42, 29, 0, 111, 58, 0, 0, 0, 0, 0, 0, 0, 0, 56, 100, 0,
        57, 57, 0, 0, 0, 106, 103, 105, 108, 0, 0, 0, 40, 12, 52, 53,
        11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 39, 51, 13, 0, 0,
        0, 30, 0, 46, 32, 18, 33, 34, 35, 23, 36, 37, 38, 50, 49, 24,
        25, 16, 19, 31, 20, 22, 47, 17, 45, 21, 44, 26, 43, 27, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 11, 19, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  };


/***********************************************************************************
 *	Belkin F8U1500 IR Keyboard
 *
 *	Angelo Alexandrou
 *	unofficial.piKey.Support@hotmail.co.uk
 *
 ***********************************************************************************/

static unsigned char Belkin_F8U1500_IR_Keyboard[128] = {

/* 00  */ 0,                0,             0,             0,              0,
/* 05  */ 0,                0,             0,             KEY_BACKSPACE,  KEY_TAB,
/* 10  */ 0,                0,             0,             KEY_KPENTER,    0,
/* 15  */ 0,                KEY_LEFTSHIFT, KEY_LEFTCTRL,  KEY_RIGHTSHIFT, KEY_DELETE,
/* 20  */ KEY_CAPSLOCK,     0,             0,             0,              0,
/* 25  */ 0,                0,             0,             KEY_INTL1,      KEY_LEFTALT,
/* 30  */ KEY_INTL2,        0,             KEY_SPACE,     KEY_SPACE,      0,
/* 35  */ 0,                0,             KEY_RIGHT,     KEY_UP,         KEY_LEFT,
/* 40  */ KEY_DOWN,         0,             0,             KEY_0,          KEY_APOSTROPHE,
/* 45  */ KEY_KPMINUS,      KEY_DOT,       KEY_SLASH,     0,              KEY_1,
/* 50  */ KEY_2,            KEY_3,         KEY_4,         KEY_5,          KEY_6,
/* 55  */ KEY_7,            KEY_8,         KEY_9,         0,              KEY_SEMICOLON,
/* 60  */ KEY_COMMA,        KEY_EQUAL,     0,             0,              0,
/* 65  */ 0,                KEY_B,         KEY_C,         KEY_D,          KEY_E,
/* 70  */ KEY_F,            KEY_G,         KEY_H,         KEY_I,          KEY_J,
/* 75  */ KEY_K,            KEY_L,         KEY_M,         KEY_N,          KEY_O,
/* 80  */ KEY_P,            KEY_Q,         KEY_R,         KEY_S,          KEY_T,
/* 85  */ KEY_U,            KEY_V,         KEY_W,         KEY_X,          KEY_Y,
/* 90  */ KEY_Z,            KEY_LEFTBRACE, KEY_BACKSLASH, KEY_RIGHTBRACE, KEY_A,
/* 95  */ 0,                0,             0,             0,              0,
/* 100 */ 0,                0,             0,             0,              0,
/* 105 */ 0,                0,             0,             0,              0,
/* 110 */ 0,                0,             0,             KEY_INTL3,      KEY_INTL4,
/* 115 */ KEY_INTL5,        KEY_INTL6,     0,             0,              0,
/* 120 */ 0,                0,             0,             0,              0,
/* 125 */ 0,                0,             0
};

/***********************************************************************************
 *	Sprint PDA IRDA Type keyboard
 *
 *	Craig Lee
 *	kraigly@gmail.com
 *
 ***********************************************************************************/

static unsigned char sprint_irda_normal[128] = {
/*0*/   0, 1, 2, 3, 4, 5, 6, 7,
/*8*/	KEY_BACKSPACE, KEY_TAB, 10, 11, 12, KEY_ENTER, KEY_CAPSLOCK, 15,
/*16*/  KEY_LEFTSHIFT,KEY_LEFTCTRL,KEY_RIGHTSHIFT,KEY_DELETE,20,21,22,23,
/*24*/	24,25,26,KEY_GRAVE,KEY_LEFTMETA,KEY_LEFTALT,KEY_MACRO,31,
/*32*/  KEY_SPACE,KEY_SPACE,34,35,36,KEY_RIGHT,KEY_UP,KEY_LEFT,
/*40*/	KEY_DOWN,41,42,43,KEY_APOSTROPHE,KEY_MINUS,KEY_DOT,KEY_SLASH,
/*48*/  KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,
/*56*/	KEY_8,KEY_9,58,KEY_SEMICOLON,KEY_COMMA,KEY_EQUAL,62,63,
/*64*/  64,KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,
/*72*/	KEY_H,KEY_I,KEY_J,KEY_K,KEY_L,KEY_M,KEY_N,KEY_O,
/*80*/  KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,
/*88*/	KEY_X,KEY_Y,KEY_Z,KEY_LEFTBRACE,KEY_BACKSLASH,KEY_RIGHTBRACE,94,95,
/*96*/  96,97,98,99,100,101,102,103,
/*104*/	104,105,106,107,108,109,110,111,
/*112*/ 112,KEY_COMPUTER,KEY_CYCLEWINDOWS,KEY_CALC,KEY_FIND,117,118,119,
/*120*/	120,121,122,123,124,125,126,127 };


/***********************************************************************************
    *
    *  Benq G250 Gamepad
    *
    *  4800 baud, 8N1
    *  Not strictly a keyboard, but it's serial and works in much the same way
    *
    *  Sends two bytes on press/release, the first byte being OR 0x80 if it is a release
    *
 ***********************************************************************************/

    /* Up, Down, Left, Right, Circle, Cross, Square, Triangle, Stop, Play */
    static unsigned char benq_gamepad_map[10][2] = {
        {40, KEY_UP},     /* Up */
        {96, KEY_DOWN},   /* Down */
        {94, KEY_LEFT},   /* Left */
        {47, KEY_RIGHT},  /* Right */
        {90, KEY_W},      /* Circle */
        {28, KEY_A},      /* Cross */
        {50, KEY_S},      /* Square */
        {92, KEY_D},      /* Triangle */
        {33, KEY_ESC},    /* Stop */
        {35, KEY_ENTER}   /* Play */
    };

        const unsigned char hama_keymap[128] = {
  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 30, 48, 46, 32, 18, 33,
 34, 35, 23, 36, 37, 38, 50, 49, 24, 25, 16, 19, 31, 20, 22, 47,
 17, 45, 44, 21, 43, 40, 55, 22, KEY_O, 30, 51, 52, 74, 14,110, 15,
 58, 96, 57, 57,112,111,106,105,103,108, 1, 42, 56,100,106,  0,
  29,  42,  127,  56,  56,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};


