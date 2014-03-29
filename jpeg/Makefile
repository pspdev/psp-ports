PSPSDK=$(shell psp-config --pspsdk-path)
PSPDIR=$(shell psp-config --psp-prefix)
TARGET_LIB = libjpeg.a
OBJS = jaricom.o jcomapi.o jutils.o jerror.o jmemmgr.o jmemnobs.o jcapimin.o   \
       jcapistd.o jcarith.o jctrans.o jcparam.o jdatadst.o jcinit.o jcmaster.o \
       jcmarker.o jcmainct.o jcprepct.o jccoefct.o jccolor.o jcsample.o        \
       jchuff.o jcdctmgr.o jfdctfst.o jfdctflt.o jfdctint.o jdapimin.o         \
       jdapistd.o jdarith.o jdtrans.o jdatasrc.o jdmaster.o jdinput.o          \
       jdmarker.o jdhuff.o jdmainct.o jdcoefct.o jdpostct.o jddctmgr.o         \
       jidctfst.o jidctflt.o jidctint.o jdsample.o jdcolor.o jquant1.o         \
       jquant2.o jdmerge.o

CFLAGS = -O2 -G0

include $(PSPSDK)/lib/build.mak

install: $(TARGET_LIB)
	@echo "Installing libjpeg into $(DESTDIR)$(PSPDIR)"
	@install -d $(DESTDIR)$(PSPDIR)/include
	@install -m644 jconfig.h jpeglib.h jmorecfg.h jerror.h $(DESTDIR)$(PSPDIR)/include
	@install -d $(DESTDIR)$(PSPDIR)/lib
	@install -m644 libjpeg.a $(DESTDIR)$(PSPDIR)/lib
	@echo "Done"
