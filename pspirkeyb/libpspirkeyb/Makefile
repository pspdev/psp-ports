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
	install -d $(DESTDIR)$(PSPSDK)/lib
	cp libpspirkeyb.a $(DESTDIR)$(PSPSDK)/lib
	install -d $(DESTDIR)$(PSPSDK)/include
	cp include/pspirkeyb.h $(DESTDIR)$(PSPSDK)/include
	cp include/pspirkeyb_rawkeys.h $(DESTDIR)$(PSPSDK)/include

uninstall:
	rm -f $(DESTDIR)$(PSPSDK)/lib/libpspirkeyb.a
	rm -f $(DESTDIR)$(PSPSDK)/include/pspirkeyb.h
	rm -f $(DESTDIR)$(PSPSDK)/include/pspirkeyb_rawkeys.h
