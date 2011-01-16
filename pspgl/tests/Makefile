ARCH = psp-
CC = $(ARCH)gcc
AS = $(ARCH)as
SIZE = $(ARCH)size
PSP_INSTALL = ../tools/psp-install
RM = rm -f

PSPPATH := $(shell psp-config --pspsdk-path)
PSPGL_LFLAGS = -lglut -lGLU -lGL -lm -lc -lpsputility -lpspdebug -lpspge -lpspdisplay -lpspctrl -lpspsdk \
	-lpspvfpu -lpsplibc -lpspuser -lpspkernel -lpsprtc
CFLAGS = -std=gnu99 -fsingle-precision-constant -g -Wall -O2 \
	-MD -MF $(DEPDIR)/$(basename $@).d -I.. -I$(PSPPATH)/include
LDFLAGS = -g -Wall -L.. -L$(PSPPATH)/lib

TESTS=bezier copytex depthtest eglcube glut spots \
	tex texsubimage varray vertexblend mipmaps viewports \
	thrashalloc readdepth eglpbuffers texmat
#TESTS+=idxtex

BUILDDATE := $(shell date "+%Y/%m/%d %k:%M:%S")
PSP_EBOOT_TITLE = $(TARGET) $(BUILDDATE)
PSP_EBOOT_ICON = firefox.png
PSP_EBOOT_ICON1 = NULL
PSP_EBOOT_UNKPNG = NULL
PSP_EBOOT_PIC1 = NULL
PSP_EBOOT_SND0 = NULL
PSP_EBOOT_PSAR = NULL

DEPDIR = .deps
PSPSDK := $(shell psp-config --pspsdk-path)

all: $(DEPDIR) $(TESTS:%=%.elf)

syms: $(TESTS:%=%.sym)

%.sym: %.elf
	prxtool -y $< > $@

install: $(TESTS:%=install-%)

-include install-settings

install-%: %.elf
	while [ ! -d $(PSP_MOUNTDIR)/psp/game ]; do mount $(PSP_MOUNTDIR); sleep 1; done
	PSP_MOUNTDIR=$(PSP_MOUNTDIR) \
		$(PSP_INSTALL) $(INSTALLFLAGS) $< \
		--eboot-title="$* $(BUILDDATE)" --eboot-icon="firefox.png"
	umount $(PSP_MOUNTDIR)

DXTTEX:=$(wildcard dxt3/*.dxt3)
DXTOBJ:=$(DXTTEX:%.dxt3=%.o)

bezier.elf: firefox.o
copytex.elf: firefox.o
eglcube.elf: logo.o
idxtex.elf: firefox-mono.o textab.h $(DXTOBJ)
spots.elf: firefox.o
tex.elf: firefox.o
texsubimage.elf: one.o two.o three.o four.o
varray.elf: firefox.o
viewports.elf: firefox.o
mipmaps.elf: sky.o ground.o

psp-setup-%.o: psp-setup.c 
	$(CC) $(CFLAGS) -o $@ -c -DMODULE_NAME="$(patsubst psp-setup-%.o,%,$@)" psp-setup.c

%.elf: %.o perfmeter.o screenshot.o psp-setup-%.o ../libGL.a ../libGLU.a ../libglut.a
	$(CC) -o $@ $(LDFLAGS) $(filter %.o,$^)  -lpng -lz $(PSPGL_LFLAGS)

%.sys: %-sys.o
	cc -o $@ $^ -lglut -lGL

%.raw: %.png
	convert $< rgba:$@

%-mono.raw: %.png
	convert $< gray:$@

%.o: %.raw
	(sym=`echo $(notdir $*) | tr '-' '_'`; \
	 echo -e ".data\n.global $${sym}_start\n$${sym}_start:\n\t.incbin \"$<\"" | $(AS) -o $@)

%.o: %.dxt3
	(sym=`echo $(notdir $*) | tr '-' '_'`; \
	 echo -e ".data\n.global $${sym}_start\n.global $${sym}_end\n$${sym}_start:\n\t.incbin \"$<\"\n$${sym}_end:\n" | $(AS) -o $@)

textab.h: $(DXTTEX) Makefile
	(for i in $(DXTTEX); do b=`basename $$i .dxt3`; echo "extern unsigned char $${b}_start[], $${b}_end[];"; done; \
	 echo "static const struct { const unsigned char *start, *end; } dxt[] = {"; \
	 for i in $(DXTTEX); do b=`basename $$i .dxt3`; echo "     { $${b}_start, $${b}_end },"; done; \
         echo "};" )> $@ || rm -f $@


%.o: $(DEPDIR) %.c
	$(CC) $(CFLAGS) -c $*.c

%-sys.o: %.c
	cc -g -DSYS -c -o $@ $<

$(DEPDIR):
	mkdir $(DEPDIR)

clean:
	$(RM) $(DEPDIR)/*.d *.o $(DXTOBJ) *.a *.elf *.sys *~

-include $(wildcard *.d) dummy

