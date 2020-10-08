#ifndef __MULTIPLEXER_H
#define __MULTIPLEXER_H

// Include
#include "stdinc.h"
#include "Common.h"

// Types
typedef struct __MuxObject
{
	uint16_t NodeID;
	MeasurementType MeasureType;
	DL_Case Case;
	DL_InputType InputType;
	MUX_Position Position;
	MUX_LeakageType LeakageType;
	MUX_Polarity Polarity;

} MuxObject, *pMuxObject;

// Functions
//
bool MUX_ConnectObject(pMuxObject Settings);
bool MUX_Disconnect(pMuxObject Settings);

#endif // __MULTIPLEXER_H
