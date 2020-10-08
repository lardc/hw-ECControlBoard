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
	VIPair Result;
} CurrentBoardObject, *pCurrentBoardObject;

// Functions
//
bool CURR_Execute(pCurrentBoardObject Settings);
bool CURR_ReadResult(pCurrentBoardObject Settings);

#endif // __CURRENT_BOARD_H
