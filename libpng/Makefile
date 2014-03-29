PSPSDK=$(shell psp-config --pspsdk-path)
PSPDIR=$(shell psp-config --psp-prefix)
TARGET_LIB = libpng.a
OBJS = png.o pngerror.o pngget.o pngmem.o pngpread.o pngread.o pngrio.o        \
       pngrtran.o pngrutil.o pngset.o pngtrans.o pngwio.o pngwrite.o pngwtran.o\
       pngwutil.o

CFLAGS = -O2 -G0

include $(PSPSDK)/lib/build.mak

install: $(TARGET_LIB)
	@echo "Installing libpng into $(DESTDIR)$(PSPDIR)"
	@install -d $(DESTDIR)$(PSPDIR)/include
	@install -d $(DESTDIR)$(PSPDIR)/lib
	@install -m644 png.h pngconf.h pnglibconf.h $(DESTDIR)$(PSPDIR)/include
	@install -m644 libpng.a $(DESTDIR)$(PSPDIR)/lib
	@echo "Done"
