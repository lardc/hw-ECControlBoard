#ifndef __CURRENT_BOARD_H
#define __CURRENT_BOARD_H

// Include
#include "stdinc.h"
#include "Common.h"

// Types
typedef struct __CurrentBoardObject
{
	uint16_t NodeID;
	VIPair Setpoint;

} CurrentBoardObject, *pCurrentBoardObject;

// Functions
//
bool CURR_Execute(uint16_t NodeID, VIPair Setpoint);
bool CURR_ReadResult(uint16_t NodeID, pVIPair Result);

#endif // __CURRENT_BOARD_H
