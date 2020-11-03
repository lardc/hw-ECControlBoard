#ifndef __LOGIC_CONTROL_H
#define __LOGIC_CONTROL_H

// Include
#include "stdinc.h"
#include "Common.h"

// Functions
void CTRL_HandleMeasurement();
ExecutionResult CTRL_ControlResult(uint16_t *OpResult, pVIPair Result);

#endif // __LOGIC_CONTROL_H
