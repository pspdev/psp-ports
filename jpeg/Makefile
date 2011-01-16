PSPSDK=$(shell psp-config --pspsdk-path)
PSPDIR=$(shell psp-config --psp-prefix)
TARGET_LIB = libjpeg.a
OBJS =	jcmainct.o jctrans.o jdhuff.o jdsample.o jidctint.o jcapimin.o \
	jcmarker.o jdapimin.o jdinput.o  jdtrans.o jidctred.o jcapistd.o \
	jcmaster.o jdapistd.o jdmainct.o jerror.o jmemmgr.o jccoefct.o \
	jcomapi.o jdatadst.o jdmarker.o jfdctflt.o jmemnobs.o jccolor.o \
	jcparam.o jdatasrc.o jdmaster.o jfdctfst.o jquant1.o jcdctmgr.o \
	jcphuff.o jdcoefct.o jdmerge.o jfdctint.o jquant2.o jchuff.o \
	jcprepct.o jdcolor.o jdphuff.o jidctflt.o jutils.o jcinit.o \
	jcsample.o jddctmgr.o jdpostct.o jidctfst.o

CFLAGS = -O2 -G0

include $(PSPSDK)/lib/build.mak

install: $(TARGET_LIB)
	@echo "Installing libjpeg into $(PSPDIR)"
	@mkdir -p $(PSPDIR)/include $(PSPDIR)/lib
	@cp jconfig.h jpeglib.h jmorecfg.h jerror.h $(PSPDIR)/include
	@cp libjpeg.a  $(PSPDIR)/lib
	@echo "Done"
