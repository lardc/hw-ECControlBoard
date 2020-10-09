#ifndef __MULTIPLEXER_H
#define __MULTIPLEXER_H

// Include
#include "stdinc.h"
#include "Common.h"
#include "MultiplexerDictionary.h"

// Types
typedef struct __MuxObject
{
	uint16_t NodeID;
	MeasurementType MeasureType;
	DL_Case Case;
	MUX_Position Position;
	MUX_Polarity Polarity;
	InputType InputType;
	LeakageType LeakageType;
} MuxObject, *pMuxObject;

// Functions
//
bool MUX_ConnectObject(pMuxObject Settings);
bool MUX_Disconnect(pMuxObject Settings);

#endif // __MULTIPLEXER_H
