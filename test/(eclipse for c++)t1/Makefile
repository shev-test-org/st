CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		t1.o

COMMON_OBJS = bin/abc.o

LIBS =

TARGET =	t1

$(TARGET):	$(OBJS) $(COMMON_OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(COMMON_OBJS) $(LIBS)

$(COMMON_OBJS): common/abc.cpp
	$(CXX) -c -o $(COMMON_OBJS) common/abc.cpp
	
all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
