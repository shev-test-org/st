CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		fuse-daemon.o

LIBS =

TARGET =	fuse-daemon

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
