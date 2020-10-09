#ifndef __MULTIPLEXER_H
#define __MULTIPLEXER_H

// Include
#include "stdinc.h"
#include "Common.h"
#include "MultiplexerDictionary.h"

// Types
typedef struct __MuxObject
{
	MeasurementType MeasureType;
	DL_Case Case;
	MUX_Position Position;
	MUX_Polarity Polarity;
	InputType InputType;
	LeakageType LeakageType;
} MuxObject, *pMuxObject;

// Functions
//
ExecutionResult MUX_Connect();
ExecutionResult MUX_Disconnect();

#endif // __MULTIPLEXER_H
