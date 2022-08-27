/*
   gbfs_demo1 ARM9 Code 
   Chris Double (chris.double@double.co.nz)
   http://www.double.co.nz/nintendo_ds
*/

#include <nds.h>
#include <nds/arm9/console.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "gbfs.h"

/* Pointers to sound data */
struct SoundFile {
  char const* filename;
  char const* name;
  int frequency;  
  uint8*  data;
  uint32 length;
};

static SoundFile soundFiles[] = {
  { "BaseHit.raw", "Base Hit", 11127, 0, 0 },
  { "InvHit.raw", "Invader Hit", 11127, 0, 0 },
  { "Shot.raw", "Shot", 11127, 0, 0 },
  { "Ufo.raw", "Ufo", 11127, 0, 0 },
  { "UfoHit.raw", "Ufo Hit", 11127, 0, 0 },
  { "Walk1.raw", "Walk1", 11127, 0, 0 },
  { "Walk2.raw", "Walk2", 11127, 0, 0 },
  { "Walk3.raw", "Walk3", 11127, 0, 0 },
  { "Walk4.raw", "Walk4", 11127, 0, 0 },
  { 0,0,0,0,0 }
};

static uint8* sound_buffer = 0;
static SoundFile* current_file = 0;
static unsigned int current_channel = 0;

static void InitSoundFiles()
{
  // Map Game Cartridge memory to ARM9
  WAIT_CR &= ~0x80;
  
  /* Start searching from the beginning of cartridge memory */
  GBFS_FILE const* gbfs_file = 
    find_first_gbfs_file((void*)0x08000000);

  unsigned int max_length = 0;
  SoundFile* file = soundFiles;
  while(file->filename) {
    file->data = (uint8*)gbfs_get_obj(gbfs_file, 
				      file->filename, 
				      &file->length);
    if(file->length > max_length)
      max_length = file->length;
    file++;
  }
  sound_buffer = (uint8*)malloc(max_length);
  current_file = soundFiles;
}

void on_irq() 
{	
  if(REG_IF & IRQ_VBLANK) {
    // Clear screen
    printf("\x1b[2J");

    printf("GBFS Demo Program\n\n");
    
    printf("Press 'A' to play current file.\n");
    printf("'left/right' changes file.\n");
    printf("'up/down' changes channel.\n\n");
    
    printf("File:    %s (%d)\n", 
		  current_file->name, 
		  current_file->length);
    printf("Channel: %d\n", current_channel);

    // Tell the DS we handled the VBLANK interrupt
    VBLANK_INTR_WAIT_FLAGS |= IRQ_VBLANK;
    REG_IF |= IRQ_VBLANK;
  }
  else {
    // Ignore all other interrupts
    REG_IF = REG_IF;
  }
}

void InitInterruptHandler()
{
  REG_IME = 0;
  IRQ_HANDLER = on_irq;
  REG_IE = IRQ_VBLANK;
  REG_IF = ~0;
  DISP_SR = DISP_VBLANK_IRQ;
  REG_IME = 1;
}

int main(void)
{
  powerON(POWER_ALL);  
  videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);
  vramSetBankA(VRAM_A_MAIN_BG);
  BG0_CR = BG_MAP_BASE(31);
  BG_PALETTE[255] = RGB15(31,31,31);
  lcdSwap();
  InitInterruptHandler();
  consoleInitDefault((u16*)SCREEN_BASE_BLOCK(31), (u16*)CHAR_BASE_BLOCK(0), 16);

  CommandInit();
  InitSoundFiles();

  while(1) {
    swiWaitForVBlank();
    scanKeys();

    if(keysDown() & KEY_UP) {
      if(++current_channel > 15)
	current_channel = 0;
    }
    if(keysDown() & KEY_DOWN) {
      if(current_channel == 0)
	current_channel = 15;
      else
	--current_channel;
    }
    if(keysDown() & KEY_LEFT) {
      if(--current_file < soundFiles)
	current_file = soundFiles;
    }
    if(keysDown() & KEY_RIGHT) {
      if((++current_file)->filename == 0) 
	--current_file;
    }
    if(keysDown() & KEY_A) {
      dmaCopy(current_file->data, sound_buffer, current_file->length);
      CommandPlayOneShotSample(current_channel, 
			       current_file->frequency, 
			       sound_buffer, 
			       current_file->length, 
			       0x3F);
    }
  }

  return 0;
}
