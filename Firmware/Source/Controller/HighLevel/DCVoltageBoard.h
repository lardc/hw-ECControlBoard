#ifndef __DC_VOLTAGE_BOARD_H
#define __DC_VOLTAGE_BOARD_H

// Include
#include "stdinc.h"
#include "Common.h"

// Types
typedef struct __DCVoltageBoardObject
{
	uint16_t NodeID;
	DCV_OutputLine OutputLine;
	DCV_OutputType OutputType;
	DCV_OutputMode OutputMode;
	uint16_t PulseLength;
	VIPair Setpoint;
	VIPair Result;

} DCVoltageBoardObject, *pDCVoltageBoardObject;

// Functions
//
bool CURR_Execute(pDCVoltageBoardObject Settings);
bool CURR_Stop(pDCVoltageBoardObject Settings);
bool CURR_ReadResult(pDCVoltageBoardObject Settings);

#endif // __DC_VOLTAGE_BOARD_H
