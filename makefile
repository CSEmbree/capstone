
CC = gcc
CXX = g++
DEBUG = -g -Wall
RM = rm -rf
CPPFLAGS = -pthread 
#LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS = -pthread

SRCS=raraa.cpp src/audio_recorder.cpp src/config_handler.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: raraa

raraa: $(OBJS)
	$(CXX) $(DEBUG) -o  $@ $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(DEBUG) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) ./.libs ./.obj *.lo *.o *.la  *~
