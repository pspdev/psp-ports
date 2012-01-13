PSPSDK=$(shell psp-config --pspsdk-path)
PSPDIR=$(shell psp-config --psp-prefix)
TARGET_LIB = libz.a
OBJS = adler32.o compress.o crc32.o deflate.o gzclose.o gzlib.o gzread.o \
       gzwrite.o infback.o inffast.o inflate.o inftrees.o trees.o uncompr.o\
       zutil.o

CFLAGS = -O2 -G0

include $(PSPSDK)/lib/build.mak

install: $(TARGET_LIB)
	@echo "Installing libz into $(PSPDIR)"
	@mkdir -p $(PSPDIR)/include $(PSPDIR)/lib
	@cp zlib.h zconf.h $(PSPDIR)/include
	@cp libz.a  $(PSPDIR)/lib
	@echo "Done"

