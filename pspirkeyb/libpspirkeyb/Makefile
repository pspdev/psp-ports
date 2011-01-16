TARGET_LIB = libpspirkeyb.a
OBJS = iniparser.o pspirkeyb.o maptable.o novaetsmap.o keymap.o rawkeys.o vt100output.o

INCDIR = ./include
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

install: all
	cp libpspirkeyb.a $(PSPSDK)/lib
	cp include/pspirkeyb.h $(PSPSDK)/include
	cp include/pspirkeyb_rawkeys.h $(PSPSDK)/include

uninstall:
	rm -f $(PSPSDK)/lib/libpspirkeyb.a
	rm -f $(PSPSDK)/include/pspirkeyb.h
	rm -f $(PSPSDK)/include/pspirkeyb_rawkeys.h
