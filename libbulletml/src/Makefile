CC = psp-g++
AR = psp-ar
PSPDIR = $(shell psp-config --psp-prefix)
INCLUDES = -I. -I tinyxml
LIBS = tinyxml/tinyxml.o tinyxml/tinyxmlparser.o tinyxml/tinyxmlerror.o
#CXXFLAGS = -g -W -Wall -ansi -pedantic
#CXXFLAGS = -pg -g -W -Wall -ansi -pedantic
CXXFLAGS = -g -O2 -G0 -W -Wall -fsingle-precision-constant -fno-exceptions
CFLAGS = -g -O2 -G0 -fsingle-precision-constant
OBJS = bulletmlparser-tinyxml.o bulletmlparser.o bulletmltree.o calc.o formula-variables.o bulletmlrunner.o bulletmlrunnerimpl.o

all: libbulletml.a

libbulletml.a: $(OBJS)
	$(MAKE) -C tinyxml
	$(AR) -r libbulletml.a *.o tinyxml/tiny*.o

clean:
	rm -f *.o *.a
	$(MAKE) -C tinyxml clean

calc.cpp: calc.yy
	bison -y   calc.yy && mv y.tab.c calc.cpp

$(OBJS): %.o: %.cpp
	$(CC) -c $(CXXFLAGS) $(INCLUDES) $<

doxy:
	doxygen

distdir:

install: all
	mkdir -p $(PSPDIR)/include/bulletml $(PSPDIR)/lib
	mkdir -p $(PSPDIR)/include/boost/config
	mkdir -p $(PSPDIR)/include/boost/config/stdlib
	mkdir -p $(PSPDIR)/include/boost/config/compiler
	mkdir -p $(PSPDIR)/include/boost/config/platform
	mkdir -p $(PSPDIR)/include/boost/detail
	cp bulletml*.h formula.h tree.h tinyxml/tinyxml.h $(PSPDIR)/include/bulletml
	find boost -name *.hpp -exec cp -R {} $(PSPDIR)/include/{} \;
	cp libbulletml.a $(PSPDIR)/lib
