/*
  Structures and functions to allow the ARM9 to send commands to the
  ARM7. Based on code from the MOD player example posted to the GBADEV
  forums.
*/
#include <nds.h>
#include <string.h>
#include "command.h"

void CommandInit() {
  memset(commandControl, 0, sizeof(CommandControl));  
}

void CommandPlayOneShotSample(int channel, int frequency, void* data, int length, int volume)
{
  Command* command = &commandControl->command[commandControl->currentCommand];
  PlaySampleSoundCommand* ps = &command->playSample;

  command->commandType = PLAY_ONE_SHOT_SAMPLE; 
  ps->channel = channel;
  ps->frequency = frequency;
  ps->data = data;
  ps->length = length;
  ps->volume = volume;

  commandControl->currentCommand++;
  commandControl->currentCommand &= MAX_COMMANDS-1;
}

