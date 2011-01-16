INCLUDES = -I../std -I..
LIBS = 
#CXXFLAGS = -g -W -Wall
#CXXFLAGS = -pg -g -W -Wall
CXXFLAGS = -O2 -W -Wall
CFLAGS = -O2
OBJS = ygg.o

all: $(OBJS)

clean:
	rm -f *.o *.a

$(OBJS): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $<

distdir:

