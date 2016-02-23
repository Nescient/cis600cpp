# hw3: cis600hw3.cpp ColumnCount.cpp ColumnCount.hpp
# 	g++ -o hw3 cis600hw3.cpp ColumnCount.cpp -std=c++11

# clean:
# 	rm hw3

#http://stackoverflow.com/a/16125648

CPP        = g++
CPPFLAGS   = -Wall -pedantic -O2 -g -c -std=c++11

HEADERS    := ${wildcard *.hpp}
SOURCES    := ${wildcard *.cpp}
OBJECTS    := ${SOURCES:.cpp=.o}

.PHONY:    all clean

.SUFFIXES: .cpp .o

all:       main

main:      $(OBJECTS) $(HEADERS)

.cpp.o:
	$(CPP) $(CPPFLAGS) $< -o $@

clean:
	-rm -fv *.o

tar:
	tar -cf hw3.tar makefile $(SOURCES) $(HEADERS)
