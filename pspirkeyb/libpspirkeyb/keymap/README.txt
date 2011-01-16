#
# How to write "map" files.
#

The mapfile is defined via pspirtty.ini. Also the map-directory is defined
here. 

Mapfiles can include other mapfiles. E.g. a base "querty-include.ini" template.

If you need a new map, you can derive from an include.ini file and add support
to your new map.

###########################################
# Structucture of a mapfile
###########################################

###########################################
# 1) Include files
#

#[GLOBAL]
include = 'querty-include.ini';

- avoid recursion! max include nestlevel is 1
- only one include per map file
- includes gets loaded before any other keymaps

###########################################
# 2) Keymaps
#

#[KEY_XXX]
#normal  = 'x';
#shift   = 'X';
#ctrl    = ;
#alt     = ;
#compose = ;
#meta    = ;
#num     = ;


#
# U+XXXX        - unicode -         "U+00AE"
# 'x'           - latin1 encoded characters
############### 0xAE          - latin1 encoded characters in hex
# KEY_FOO       - KEY_XXX  or BTN_YYY key constant (emulate a raw key)
#

Every entry is optional. 
Some KEY_* have default "map" meanings, e.g. CRTL*, SHIFT*, META*, COMPOSE*.

Keymaps can fully replace entries from includes, e.g.
[KEY_Y]
normal = 'z';
shift = 'Z';

Do this when you want some other layout.


######## KEY names #######

#KEY_RESERVED
#KEY_ESC
#KEY_1
#KEY_2
#KEY_3
#KEY_4
#KEY_5
#KEY_6
#KEY_7
#KEY_8
#KEY_9
#KEY_0
#KEY_MINUS
#KEY_EQUAL
#KEY_BACKSPACE
#KEY_TAB
#KEY_Q
#KEY_W
#KEY_E
#KEY_R
#KEY_T
#KEY_Y
#KEY_U
#KEY_I
#KEY_O
#KEY_P
#KEY_LEFTBRACE
#KEY_RIGHTBRACE
#KEY_ENTER
#KEY_LEFTCTRL
#KEY_A
#KEY_S
#KEY_D
#KEY_F
#KEY_G
#KEY_H
#KEY_J
#KEY_K
#KEY_L
#KEY_SEMICOLON
#KEY_APOSTROPHE
#KEY_GRAVE
#KEY_LEFTSHIFT
#KEY_BACKSLASH
#KEY_Z
#KEY_X
#KEY_C
#KEY_V
#KEY_B
#KEY_N
#KEY_M
#KEY_COMMA
#KEY_DOT
#KEY_SLASH
#KEY_RIGHTSHIFT
#KEY_KPASTERISK
#KEY_LEFTALT
#KEY_SPACE
#KEY_CAPSLOCK
#KEY_F1
#KEY_F2
#KEY_F3
#KEY_F4
#KEY_F5
#KEY_F6
#KEY_F7
#KEY_F8
#KEY_F9
#KEY_F10
#KEY_NUMLOCK
#KEY_SCROLLLOCK
#KEY_KP7
#KEY_KP8
#KEY_KP9
#KEY_KPMINUS
#KEY_KP4
#KEY_KP5
#KEY_KP6
#KEY_KPPLUS
#KEY_KP1
#KEY_KP2
#KEY_KP3
#KEY_KP0
#KEY_KPDOT
#KEY_103RD
#KEY_F13
#KEY_102ND
#KEY_F11
#KEY_F12
#KEY_F14
#KEY_F15
#KEY_F16
#KEY_F17
#KEY_F18
#KEY_F19
#KEY_F20
#KEY_KPENTER
#KEY_RIGHTCTRL
#KEY_KPSLASH
#KEY_SYSRQ
#KEY_RIGHTALT
#KEY_LINEFEED
#KEY_HOME
#KEY_UP
#KEY_PAGEUP
#KEY_LEFT
#KEY_RIGHT
#KEY_END
#KEY_DOWN
#KEY_PAGEDOWN
#KEY_INSERT
#KEY_DELETE
#KEY_MACRO
#KEY_MUTE
#KEY_VOLUMEDOWN
#KEY_VOLUMEUP
#KEY_POWER
#KEY_KPEQUAL
#KEY_KPPLUSMINUS
#KEY_PAUSE
#KEY_F21
#KEY_F22
#KEY_F23
#KEY_F24
#KEY_KPCOMMA
#KEY_LEFTMETA
#KEY_RIGHTMETA
#KEY_COMPOSE
#KEY_STOP
#KEY_AGAIN
#KEY_PROPS
#KEY_UNDO
#KEY_FRONT
#KEY_COPY
#KEY_OPEN
#KEY_PASTE
#KEY_FIND
#KEY_CUT
#KEY_HELP
#KEY_MENU
#KEY_CALC
#KEY_SETUP
#KEY_SLEEP
#KEY_WAKEUP
#KEY_FILE
#KEY_SENDFILE
#KEY_DELETEFILE
#KEY_XFER
#KEY_PROG1
#KEY_PROG2
#KEY_WWW
#KEY_MSDOS
#KEY_COFFEE
#KEY_DIRECTION
#KEY_CYCLEWINDOWS
#KEY_MAIL
#KEY_BOOKMARKS
#KEY_COMPUTER
#KEY_BACK
#KEY_FORWARD
#KEY_CLOSECD
#KEY_EJECTCD
#KEY_EJECTCLOSECD
#KEY_NEXTSONG
#KEY_PLAYPAUSE
#KEY_PREVIOUSSONG
#KEY_STOPCD
#KEY_RECORD
#KEY_REWIND
#KEY_PHONE
#KEY_ISO
#KEY_CONFIG
#KEY_HOMEPAGE
#KEY_REFRESH
#KEY_EXIT
#KEY_MOVE
#KEY_EDIT
#KEY_SCROLLUP
#KEY_SCROLLDOWN
#KEY_KPLEFTPAREN
#KEY_KPRIGHTPAREN
#KEY_INTL1
#KEY_INTL2
#KEY_INTL3
#KEY_INTL4
#KEY_INTL5
#KEY_INTL6
#KEY_INTL7
#KEY_INTL8
#KEY_INTL9
#KEY_LANG1
#KEY_LANG2
#KEY_LANG3
#KEY_LANG4
#KEY_LANG5
#KEY_LANG6
#KEY_LANG7
#KEY_LANG8
#KEY_LANG9
#KEY_PLAYCD
#KEY_PAUSECD
#KEY_PROG3
#KEY_PROG4
#KEY_SUSPEND
#KEY_CLOSE
#KEY_UNKNOWN
#KEY_BRIGHTNESSDOWN
#KEY_BRIGHTNESSUP
#BTN_MISC
#BTN_0
#BTN_1
#BTN_2
#BTN_3
#BTN_4
#BTN_5
#BTN_6
#BTN_7
#BTN_8
#BTN_9
#BTN_MOUSE
#BTN_LEFT
#BTN_RIGHT
#BTN_MIDDLE
#BTN_SIDE
#BTN_EXTRA
#BTN_FORWARD
#BTN_BACK
#BTN_JOYSTICK
#BTN_TRIGGER
#BTN_THUMB
#BTN_THUMB2
#BTN_TOP
#BTN_TOP2
#BTN_PINKIE
#BTN_BASE
#BTN_BASE2
#BTN_BASE3
#BTN_BASE4
#BTN_BASE5
#BTN_BASE6
#BTN_DEAD
#BTN_GAMEPAD
#BTN_A
#BTN_B
#BTN_C
#BTN_X
#BTN_Y
#BTN_Z
#BTN_TL
#BTN_TR
#BTN_TL2
#BTN_TR2
#BTN_SELECT
#BTN_START
#BTN_MODE
#BTN_THUMBL
#BTN_THUMBR
#BTN_DIGI
#BTN_TOOL_PEN
#BTN_TOOL_RUBBER
#BTN_TOOL_BRUSH
#BTN_TOOL_PENCIL
#BTN_TOOL_AIRBRUSH
#BTN_TOOL_FINGER
#BTN_TOOL_MOUSE
#BTN_TOOL_LENS
#BTN_TOUCH
#BTN_STYLUS
#BTN_STYLUS2
#KEY_MAX


