PSPSDK=$(shell psp-config --pspsdk-path)
PSPDIR=$(shell psp-config --psp-prefix)
TARGET_LIB = libz.a
OBJS = adler32.o compress.o crc32.o deflate.o gzclose.o gzlib.o gzread.o \
       gzwrite.o infback.o inffast.o inflate.o inftrees.o trees.o uncompr.o\
       zutil.o

CFLAGS = -O2 -G0

include $(PSPSDK)/lib/build.mak

install: $(TARGET_LIB)
	@echo "Installing libz into $(DESTDIR)$(PSPDIR)"
	@install -d $(DESTDIR)$(PSPDIR)/include
	@install -d $(DESTDIR)$(PSPDIR)/lib
	@install -m644 zlib.h zconf.h $(DESTDIR)$(PSPDIR)/include
	@install -m644 libz.a $(DESTDIR)$(PSPDIR)/lib
	@echo "Done"

