#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "stdinc.h"

// Functions
//
void LL_ToggleBoardLED();
void LL_SetStateExtLed(bool State);
void LL_SetStateFan(bool State);
void LL_SetStateExtLineSynce1(bool State);
void LL_SetStateExtLineSynce2(bool State);
void LL_SetStateIntLineSynce1(bool State);
void LL_SetStateIntLineSynce2(bool State);

bool LL_GetStateIntLineSynce1();
bool LL_GetStateIntLineSynce2();

#endif //__LOWLEVEL_H
