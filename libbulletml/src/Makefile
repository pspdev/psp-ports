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
	install -d $(DESTDIR)$(PSPDIR)/include/bulletml
	install -d $(DESTDIR)$(PSPDIR)/lib
	install -d $(DESTDIR)$(PSPDIR)/include/boost/config
	install -d $(DESTDIR)$(PSPDIR)/include/boost/config/stdlib
	install -d $(DESTDIR)$(PSPDIR)/include/boost/config/compiler
	install -d $(DESTDIR)$(PSPDIR)/include/boost/config/platform
	install -d $(DESTDIR)$(PSPDIR)/include/boost/detail
	install -m644 bulletml*.h formula.h tree.h tinyxml/tinyxml.h $(DESTDIR)$(PSPDIR)/include/bulletml
	find boost -name *.hpp -exec cp -R {} $(DESTDIR)$(PSPDIR)/include/{} \;
	install -m644 libbulletml.a $(DESTDIR)$(PSPDIR)/lib
