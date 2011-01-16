ARCH = psp-
CC = $(ARCH)gcc
PSP_INSTALL = ../tools/psp-install
RM = rm -f

TARGET = test-vfpu 
OBJS = main.o

PSPPATH := $(shell psp-config --pspsdk-path)
LIBS = -lpspdebug -lpspdisplay -lpspge -lpspsdk -lpspctrl -lm -lc -lpspuser -lpsputility -lpspkernel
CFLAGS = -g -Wall -O2 -MD -I$(PSPPATH)/include 
LFLAGS = -g -Wall -O2 -DMODULE_NAME=$(TARGET) psp-setup.c -L$(PSPPATH)/lib $(LIBS)

BUILDDATE = $(shell date "+%Y/%m/%d %k:%M:%S")


all: $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJS) psp-setup.c
	$(CC) $(OBJS) $(LFLAGS) -o $@

install: all
	$(PSP_INSTALL) $(TARGET) --eboot-title="$(TARGET) $(BUILDDATE)"

clean:
	$(RM) $(TARGET) *.d *.o *.a *.elf *.sfo EBOOT.PBP

-include $(wildcard *.d) dummy

