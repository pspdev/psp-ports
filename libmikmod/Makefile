PSPSDK=$(shell psp-config --pspsdk-path)
PSPDIR=$(shell psp-config --psp-prefix)

TARGET_LIB = ./libmikmod/libmikmod.a

OBJS = 	drivers/drv_psp_thread.o		\
	mmio/mmalloc.o			\
	mmio/mmerror.o			\
	mmio/mmio.o			\
	loaders/load_669.o		\
	loaders/load_amf.o		\
	loaders/load_dsm.o		\
	loaders/load_far.o		\
	loaders/load_gdm.o		\
	loaders/load_imf.o		\
	loaders/load_it.o		\
	loaders/load_m15.o		\
	loaders/load_med.o		\
	loaders/load_mod.o		\
	loaders/load_mtm.o		\
	loaders/load_okt.o		\
	loaders/load_s3m.o		\
	loaders/load_stm.o		\
	loaders/load_stx.o		\
	loaders/load_ult.o		\
	loaders/load_uni.o		\
	loaders/load_xm.o		\
	playercode/mdreg.o		\
	playercode/mdriver.o		\
	playercode/mdulaw.o		\
	playercode/mloader.o		\
	playercode/mlreg.o		\
	playercode/mlutil.o		\
	playercode/mplayer.o		\
	playercode/munitrk.o		\
	playercode/mwav.o		\
	playercode/npertab.o		\
	playercode/sloader.o		\
	playercode/virtch2.o		\
	playercode/virtch.o		\
	playercode/virtch_common.o	
	


INCDIR = ./include

CFLAGS = -G0 -DHAVE_CONFIG_H -g -O2

include $(PSPSDK)/lib/build.mak

install:
	@mkdir -p $(PSPDIR)/include $(PSPDIR)/lib
	@cp include/mikmod_build.h $(PSPDIR)/include/mikmod.h
	@cp libmikmod/libmikmod.a  $(PSPDIR)/lib
