# Makefile for gbfs_demo1.nds
# chris.double@double.co.nz
NDSLIB_INCLUDE=$(DEVKITPRO)/libnds/include
NDSLIB_LIB=$(DEVKITPRO)/libnds/lib

all: gbfs_demo1.nds.gba

BaseHit.raw: wav/BaseHit.wav
	sox wav/BaseHit.wav BaseHit.raw

InvHit.raw: wav/InvHit.wav
	sox wav/InvHit.wav InvHit.raw

Shot.raw: wav/Shot.wav
	sox wav/Shot.wav Shot.raw

Ufo.raw: wav/Ufo.wav
	sox wav/Ufo.wav Ufo.raw

UfoHit.raw: wav/UfoHit.wav
	sox wav/UfoHit.wav UfoHit.raw

Walk1.raw: wav/Walk1.wav
	sox wav/Walk1.wav Walk1.raw

Walk2.raw: wav/Walk2.wav
	sox wav/Walk2.wav Walk2.raw

Walk3.raw: wav/Walk3.wav
	sox wav/Walk3.wav Walk3.raw

Walk4.raw: wav/Walk4.wav
	sox wav/Walk4.wav Walk4.raw

libgbfs.o: libgbfs.c gbfs.h
	arm-elf-g++ -g -Wall -O2 -mcpu=arm7tdmi -mtune=arm7tdmi -fomit-frame-pointer -ffast-math -mthumb-interwork -I$(NDSLIB_INCLUDE) -DARM7 -c libgbfs.c -olibgbfs.o

arm7_main.o: arm7_main.cpp
	arm-elf-g++ -g -Wall -O2 -mcpu=arm7tdmi -mtune=arm7tdmi -fomit-frame-pointer -ffast-math -mthumb-interwork -I$(NDSLIB_INCLUDE) -DARM7 -c arm7_main.cpp -oarm7_main.o

command7.o: command7.cpp
	arm-elf-g++ -g -Wall -O2 -mcpu=arm7tdmi -mtune=arm7tdmi -fomit-frame-pointer -ffast-math -mthumb-interwork -I$(NDSLIB_INCLUDE) -DARM7 -c command7.cpp -ocommand7.o

arm7.elf: arm7_main.o command7.o
	arm-elf-g++ -g -mthumb-interwork -mno-fpu -specs=ds_arm7.specs arm7_main.o command7.o -L$(NDSLIB_LIB) -lnds7 -oarm7.elf

arm7.bin: arm7.elf 
	arm-elf-objcopy -O binary arm7.elf arm7.bin

arm9_main.o: arm9_main.cpp
	arm-elf-g++ -g -Wall -O2 -mcpu=arm9tdmi -mtune=arm9tdmi -fomit-frame-pointer -ffast-math -mthumb-interwork -I$(NDSLIB_INCLUDE) -DARM9 -c arm9_main.cpp -oarm9_main.o

command9.o: command9.cpp
	arm-elf-g++ -g -Wall -O2 -mcpu=arm9tdmi -mtune=arm9tdmi -fomit-frame-pointer -ffast-math -mthumb-interwork -I$(NDSLIB_INCLUDE) -DARM9 -c command9.cpp -ocommand9.o

arm9.elf: arm9_main.o libgbfs.o command9.o 
	arm-elf-g++ -g -mthumb-interwork -mno-fpu -specs=ds_arm9.specs arm9_main.o command9.o libgbfs.o  -L$(NDSLIB_LIB) -lnds9 -o arm9.elf

arm9.bin: arm9.elf
	arm-elf-objcopy -O binary arm9.elf arm9.bin

sounds.gbfs: BaseHit.raw InvHit.raw Shot.raw Ufo.raw UfoHit.raw Walk1.raw Walk2.raw Walk3.raw Walk4.raw 
	gbfs sounds.gbfs BaseHit.raw InvHit.raw Shot.raw Ufo.raw UfoHit.raw Walk1.raw Walk2.raw Walk3.raw Walk4.raw 

gbfs_demo1.nds: arm7.bin arm9.bin sounds.gbfs
	ndstool -c gbfs_demo1.nds -9 arm9.bin -7 arm7.bin
	padbin 256 gbfs_demo1.nds
	cat gbfs_demo1.nds sounds.gbfs >gbfs_demo1_tmp.nds
	mv gbfs_demo1_tmp.nds gbfs_demo1.nds

gbfs_demo1.nds.gba: gbfs_demo1.nds
	dsbuild gbfs_demo1.nds -o gbfs_demo1.nds.gba

clean:
	rm -f *.bin
	rm -f *.elf
	rm -f *.o
	rm -f *~
	rm -f *.raw
	rm -f *.gbfs

