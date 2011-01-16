TARGET = asciidemo
OBJS = main.o

INCDIR = $(PSPDEV)/SDK/include ../libpspirkeyb/include
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = $(PSPDEV)/SDK/lib ../libpspirkeyb
LDFLAGS =

LIBS = -lpspirkeyb -lpsppower

BUILD_PRX = 1
PSP_FW_VERSION = 401
PSP_LARGE_MEMORY = 1

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = IR Keyb ASCii Test

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
