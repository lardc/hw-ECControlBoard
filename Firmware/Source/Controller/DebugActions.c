// Header
#include "DebugActions.h"

// Include
#include "LowLevel.h"
#include "Board.h"
#include "Delay.h"
#include "Controller.h"
#include "DataTable.h"

// Functions
//
void DBGACT_GenerateImpulseLedRed()
{
	LL_LedRed(TRUE);
	CONTROL_DelayMs(10);
	LL_LedRed(FALSE);
}
//-----------------------------

void DBGACT_GenerateImpulseLedGreen()
{
	LL_LedGreen(TRUE);
	CONTROL_DelayMs(10);
	LL_LedGreen(FALSE);
}
//-----------------------------

void DBGACT_GenerateImpulseLineSync1()
{
	LL_SetSync1State(TRUE);
	CONTROL_DelayMs(10);
	LL_SetSync1State(FALSE);
}
//-----------------------------

void DBGACT_GenerateImpulseLineSync2()
{
	LL_SetSync2State(TRUE);
	CONTROL_DelayMs(10);
	LL_SetSync2State(FALSE);
}
//-----------------------------

void DBGACT_GenerateImpulseLineLock1()
{
	LL_SetStateLock1(TRUE);
	CONTROL_DelayMs(10);
	LL_SetStateLock1(FALSE);
}
//-----------------------------

void DBGACT_GenerateImpulseLineLock2()
{
	LL_SetStateLock2(TRUE);
	CONTROL_DelayMs(10);
	LL_SetStateLock2(FALSE);
}
//-----------------------------
