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

PSPDIR = $(shell psp-config --psp-prefix)

install: all
	install -d $(PSPDIR)/lib
	cp libpspirkeyb.a $(PSPDIR)/lib
	install -d $(PSPDIR)/include
	cp include/pspirkeyb.h $(PSPDIR)/include
	cp include/pspirkeyb_rawkeys.h $(PSPDIR)/include

uninstall:
	rm -f $(PSPDIR)/lib/libpspirkeyb.a
	rm -f $(PSPDIR)/include/pspirkeyb.h
	rm -f $(PSPDIR)/include/pspirkeyb_rawkeys.h
