PSPSDK=$(shell psp-config --pspsdk-path)
PSPDIR = $(shell psp-config --psp-prefix)

TARGET_LIB = ../lib/libmad.a
OBJS = bit.o decoder.o fixed.o frame.o huffman.o layer12.o layer3.o \
	stream.o synth.o timer.o version.o

CFLAGS += -DHAVE_CONFIG_H -DHAVE_SYS_TYPES_H -DHAVE_ERRNO_H -DHAVE_UNISTD_H \
	-DHAVE_FCNTL_H -I$(PSPSDK)/include/sys/  -I../include

include $(PSPSDK)/lib/build.mak

install: $(PSP_LIB)
	@echo "Installing libmad into $(PSPLIBSDIR)/libmad"
	@mkdir -p $(PSPDIR)/include $(PSPDIR)/lib
	@cp ../include/mad.h $(PSPDIR)/include
	@cp ../lib/libmad.a  $(PSPDIR)/lib
	@echo "Done."
	


