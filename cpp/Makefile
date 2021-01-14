#
# ©2015 Symbol Technologies LLC. All rights reserved.
#

CC = g++
CXXFLAGS = -Wall -g -I/usr/include/zebra-scanner -I./include/
LFLAGS = -L/usr/lib/zebra-scanner/corescanner -I./include/
LIBS = -lcs-client -lcs-common
SRCS = ./src/ConsoleMain.cpp ./src/ConsoleSampleEventListener.cpp ./src/ISO15434formatEnvelope.cpp
OBJS = $(SRCS:.cpp=.o)
MAIN = ./bin/scanner

.PHONY: depend clean

all:	$(MAIN)

$(MAIN): $(OBJS)
	mkdir bin
	$(CC) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)
	
depend: $(SRC)
	makedepend $(INCLUDES) $^
