#ifndef __DC_VOLTAGE_BOARD_H
#define __DC_VOLTAGE_BOARD_H

// Include
#include "stdinc.h"
#include "Common.h"
#include "DCVoltageBoardDictionary.h"

// Types
typedef struct __DCVoltageBoardObject
{
	pSlaveNode SlaveNode;
	DCV_OutputLine OutputLine;
	DCV_OutputType OutputType;
	DCV_OutputMode OutputMode;
	uint16_t PulseLength;
	VIPair Setpoint;
	VIPair Result;
} DCVoltageBoardObject, *pDCVoltageBoardObject;

// Functions
//
bool DCV_Execute(pDCVoltageBoardObject Settings);
bool DCV_Stop(pDCVoltageBoardObject Settings);
bool DCV_ReadResult(pDCVoltageBoardObject Settings);

#endif // __DC_VOLTAGE_BOARD_H
