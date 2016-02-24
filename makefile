#http://stackoverflow.com/a/16125648

CPP        = g++
CPPFLAGS   = -Wall -pedantic -O2 -g -std=c++14

HEADERS    := ${wildcard *.hpp}
SOURCES    := ${wildcard *.cpp}
OBJECTS    := ${SOURCES:.cpp=.o}

.PHONY:    all clean

.SUFFIXES: .cpp .o

all:       main

main:      $(OBJECTS) $(HEADERS) makefile
	$(CPP) $(CPPFLAGS) $(OBJECTS) -o $@ -pthread

.cpp.o:
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	-rm -fv *.o hw3.tar main

tar:
	tar -cf hw3.tar makefile $(SOURCES) $(HEADERS)
