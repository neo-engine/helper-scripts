/*
  Functions for the ARM7 to process the commands from the ARM9.Based
  on code from the MOD player example posted to the GBADEV forums.
*/
#include <nds.h>
#include "command.h"

static void CommandPlayOneShotSample(PlaySampleSoundCommand* ps)
{
  int channel = ps->channel;

  SCHANNEL_CR(channel) = 0;
  SCHANNEL_TIMER(channel) = SOUND_FREQ(ps->frequency);
  SCHANNEL_SOURCE(channel) = (uint32)ps->data;
  SCHANNEL_LENGTH(channel) = ps->length >> 2;  
  SCHANNEL_CR(channel) = 
    SCHANNEL_ENABLE | 
    SOUND_ONE_SHOT | 
    SOUND_8BIT | 
    SOUND_VOL(ps->volume);
}

void CommandProcessCommands() {
  static int currentCommand = -1;
  
  while(currentCommand != commandControl->currentCommand) {
    Command* command = &commandControl->command[currentCommand];
    switch(command->commandType) {
    case PLAY_ONE_SHOT_SAMPLE:
      CommandPlayOneShotSample(&command->playSample);
      break;      
    }
    currentCommand++;
    currentCommand &= MAX_COMMANDS-1;
  }
}
