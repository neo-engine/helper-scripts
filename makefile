CC=g++
LDFLAGS= -L /opt/homebrew/lib/ -lpng
CPPFLAGS=-c -std=c++20 -I/opt/homebrew/include/ -Wall -DNUM_LANGUAGES=2 -DMAX_ITEMS_PER_DIR=30 -g3 -ggdb
SOURCES=bitmap.cpp fsdata.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: normalizepkmnicon ampal2p2l csvjoin csvjoin2 png2raw 128x32raw plat2raw sprite2raw pkmnsprite2raw pkmnicon2rsd raw2png raw2sprite typeicon2raw maprender am95mapto92map pkmnowsprite2rsd

am95mapto92map: $(OBJECTS) am95mapto92map.o
	$(CC) $(LDFLAGS) am95mapto92map.o $(OBJECTS) -o $@

maprender: $(OBJECTS) maprender.o
	$(CC) $(LDFLAGS) maprender.o $(OBJECTS) -o $@

normalizepkmnicon: $(OBJECTS) normalizepkmnicon.o
	$(CC) $(LDFLAGS) normalizepkmnicon.o $(OBJECTS) -o $@

p2ldns: $(OBJECTS) p2ldns.o
	$(CC) p2ldns.o -o $@

ampal2p2l: $(OBJECTS) ampal2p2l.o
	$(CC) ampal2p2l.o -o $@

csvjoin: $(OBJECTS) csvjoin.o
	$(CC) csvjoin.o -o $@

csvjoin2: $(OBJECTS) csvjoin2.o
	$(CC) csvjoin2.o -o $@

png2raw: $(OBJECTS) png2raw.o
	$(CC) $(LDFLAGS) png2raw.o $(OBJECTS) -o $@

128x32raw: $(OBJECTS) 128x32raw.o
	$(CC) $(LDFLAGS) 128x32raw.o $(OBJECTS) -o $@

plat2raw: $(OBJECTS) plat2raw.o
	$(CC) $(LDFLAGS) plat2raw.o $(OBJECTS) -o $@

sprite2raw: $(OBJECTS) sprite2raw.o
	$(CC) $(LDFLAGS) sprite2raw.o $(OBJECTS) -o $@

door2raw: $(OBJECTS) door2raw.o
	$(CC) $(LDFLAGS) door2raw.o $(OBJECTS) -o $@

typeicon2raw: $(OBJECTS) typeicon2raw.o
	$(CC) $(LDFLAGS) typeicon2raw.o $(OBJECTS) -o $@

pkmnowsprite2rsd: $(OBJECTS) pkmnowsprite2rsd.o
	$(CC) $(LDFLAGS) pkmnowsprite2rsd.o $(OBJECTS) -o $@

pkmnsprite2raw: $(OBJECTS) pkmnsprite2raw.o
	$(CC) $(LDFLAGS) pkmnsprite2raw.o $(OBJECTS) -o $@

pkmnicon2rsd: $(OBJECTS) pkmnicon2rsd.o
	$(CC) $(LDFLAGS) pkmnicon2rsd.o $(OBJECTS) -o $@

raw2png: $(OBJECTS) raw2png.o
	$(CC) $(LDFLAGS) raw2png.o $(OBJECTS) -o $@

raw2sprite: $(OBJECTS) raw2sprite.o
	$(CC) $(LDFLAGS) raw2sprite.o $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	rm -rf *o
