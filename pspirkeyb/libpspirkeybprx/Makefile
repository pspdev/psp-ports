TARGET_LIB = libpspirkeybprx.a
OBJS = IRKEYB.o

INCDIR = ./include
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

install: all
	cp libpspirkeybprx.a $(PSPSDK)/lib
	cp include/pspirkeybprx.h $(PSPSDK)/include

uninstall:
	rm -f $(PSPSDK)/lib/libpspirkeybprx.a
	rm -f $(PSPSDK)/include/pspirkeybprx.h
