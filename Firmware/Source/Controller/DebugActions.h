#ifndef __DEBUGACTIONS_H
#define __DEBUGACTIONS_H

// Include
//
#include "ZwBase.h"

// Functions
//
void DBGACT_BlinkExtLed();
void DBGACT_GenerateImpulseToExtLineSync1();
void DBGACT_GenerateImpulseToExtLineSync2();
void DBGACT_GenerateImpulseToIntLineSync1();
void DBGACT_GenerateImpulseToIntLineSync2();
bool DBGACT_ReadStateIntLineSync1();
bool DBGACT_ReadStateIntLineSync2();

#endif //__DEBUGACTIONS_H
