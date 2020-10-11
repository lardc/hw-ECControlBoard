#ifndef __LOGIC_H
#define __LOGIC_H

// Include
#include "stdinc.h"
#include "BCCIMHighLevel.h"

// Types
typedef enum __LogicConfigError
{
	LCE_None = 0,
	LCE_UnknownCase = 1,
	LCE_PositionMissmatch = 2
} LogicConfigError;

// Functions
//
void LOGIC_InitEntities();
void LOGIC_HandleStateUpdate();
void LOGIC_HandlePowerOn();
void LOGIC_HandlePowerOff();

LogicConfigError LOGIC_PrepareOnStateMeasurement();

#endif // __LOGIC_H
