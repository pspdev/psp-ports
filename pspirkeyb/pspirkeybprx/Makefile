TARGET = irkeyb
OBJS = main.o

# Define to build this as a prx (instead of a static elf)
BUILD_PRX=1

# Define the name of our custom exports (minus the .exp extension)
PRX_EXPORTS=exports.exp

# no idea if this works... but the iniparser uses scanf and atoi
#USE_PSPSDK_LIBC = 1

INCDIR = ../libpspirkeyb/include
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = ../libpspirkeyb

LIBS = -lpspirkeyb -lpsppower

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

exp_symbols: all
	$(shell psp-config --pspdev-path)/bin/psp-build-exports -s exports.exp
	cp IRKEYB.S ../libpspirkeybprx
