SOURCEDIR = ./quake3-1.32b
CODEDIR = $(SOURCEDIR)/code
Q3ZIP = $(SOURCEDIR)-source.zip
TARGET = Quake3

include $(wildcard files_*.inc)

OBJS := $(SRC_FILES:.c=.o)
DEPS := $(SRC_FILES:.c=.d)

ARCH = psp-
CC = $(ARCH)gcc
RM = rm -f

CFLAGS = -g -Wall -O0 -MD -DBOTLIB
LFLAGS = -g -Wall -O0

#CFLAGS += -ffunction-sections

ifeq ($(ARCH),psp-)
	SDKPATH := $(shell psp-config --pspsdk-path)
	# XXX FIXME: Toolchain quirk: this should get set by the compiler!!
	CFLAGS += -D__LITTLE_ENDIAN__
	CFLAGS += -I$(SDKPATH)/include -I$(SDKPATH)/include/GL
	# XXX FIXME: Do we still need -G0 ??
	CFLAGS += -G0 -fsingle-precision-constant
	# we place all missing libc bits in generic/...
	CFLAGS += -I ./generic
	LFLAGS += -DMODULE_NAME="Quake3" psp-setup.c
	LFLAGS += -L$(SDKPATH)/lib -lglut -lGL
	LFLAGS += -lpspdebug -lpspge -lpspdisplay -lpspctrl -lpspsdk -lm -lc -lpspuser -lpsputility -lpspkernel
else
	ifeq ($(shell uname),Darwin)
        	CFLAGS += -F OpenGL -I/System/Library/Frameworks/OpenGL.framework/Headers/ 
        	CFLAGS += -F GLUT -I/System/Library/Frameworks/GLUT.framework/Headers/ 
	        LFLAGS += -framework GLUT -framework OpenGL
	else
        	CFLAGS += -I/usr/X11R6/include/GL -I/usr/include/GL
	        LFLAGS += -L/usr/X11R6/lib -lGL -lGLU -lglut
	endif
endif


all: $(SOURCEDIR) $(TARGET)

$(SOURCEDIR):
	@test -e $(Q3ZIP) || ( echo "please copy $(Q3ZIP) into this directory!" ; exit 1 )
	unzip $(Q3ZIP)
	@echo "--------------------------------------------------------------------------"
	@echo " Source Package unpacked. Removing DOS CR/LF... "
	@echo "--------------------------------------------------------------------------"
	# leaving them as-is would cause troubles when patching...
	find $(SOURCEDIR) -iname "*.[hc]" -exec sh ./strip_cr.sh {} \;
	@echo "--------------------------------------------------------------------------"
	@echo " Patching... "
	@echo "--------------------------------------------------------------------------"
	patch -p0 < $(wildcard *.diff)
	@echo "--------------------------------------------------------------------------"
	@echo " Compiling... "
	@echo "--------------------------------------------------------------------------"
	@make

$(TARGET): $(SOURCEDIR) $(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)

.c.o: $(SOURCEDIR)
	@echo "compile: $<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "remove object, dependency files and built files..."
	@$(RM) $(OBJS) $(DEPS) $(TARGET)
	@echo "done."

-include $(DEPS) dummy

