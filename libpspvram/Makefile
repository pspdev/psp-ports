### 
## libpspvram library makefile
###

TARGET_LIB_VALLOC = libpspvalloc.a
TARGET_LIB_VRAM   = libpspvram.a
TARGET_LIB = $(TARGET_LIB_VRAM)

VALLOC_OBJS = valloc.o
VRAM_OBJS   = vram.o

INCDIR = 
CFLAGS = -G0 -mno-explicit-relocs -O2 -Wall -Werror -g

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LDFLAGS =
LIBS = 

all: pspvalloc pspvram

pspvalloc:
	rm -f *.o $(TARGET_LIB_VALLOC)
	make TARGET_LIB=$(TARGET_LIB_VALLOC) OBJS=$(VALLOC_OBJS) $(TARGET_LIB_VALLOC)

pspvram:
	rm -f *.o $(TARGET_LIB_VRAM)
	make TARGET_LIB=$(TARGET_LIB_VRAM) OBJS=$(VRAM_OBJS) $(TARGET_LIB_VRAM)

PSPSDK=$(shell psp-config --pspsdk-path)
PSPDIR=$(shell psp-config --psp-prefix)
include $(PSPSDK)/lib/build.mak

install: pspvalloc pspvram
	@install -d $(DESTDIR)$(PSPDIR)/include
	@install -d $(DESTDIR)$(PSPDIR)/lib
	@install -m644 $(TARGET_LIB_VRAM) $(TARGET_LIB_VALLOC) $(DESTDIR)$(PSPDIR)/lib
	@install -m644 *.h $(DESTDIR)$(PSPDIR)/include
	@echo "Done."

clobber:
	rm -f *.o *.a

