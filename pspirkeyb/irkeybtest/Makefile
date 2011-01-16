TARGET = irkeybtest
OBJS = main.o

INCDIR = ../libpspirkeyb/include
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = ../libpspirkeyb
LDFLAGS =

LIBS = -lpspirkeyb -lpsppower -lpspaudiolib -lpspaudio

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = IR Keyb Test

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
