CC=g++
LDFLAGS=-lpng
CPPFLAGS=-c -std=c++11 -Os -Wall -g3 -ggdb
SOURCES=bitmap.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: png2raw raw2png npl2rsd clean

png2raw: $(OBJECTS) png2raw.o
	$(CC) $(LDFLAGS) png2raw.o $(OBJECTS) -o $@

raw2png: $(OBJECTS) raw2png.o
	$(CC) $(LDFLAGS) raw2png.o $(OBJECTS) -o $@

npl2rsd: npl2rsd.o
	$(CC) npl2rsd.o -o $@

.cpp.o:
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	rm -rf *o
