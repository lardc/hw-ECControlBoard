// Header
#include "DebugActions.h"

// Include
//
#include "LowLevel.h"
#include "Board.h"
#include "Delay.h"
#include "Controller.h"
#include "DataTable.h"

// Functions
//
void DBGACT_BlinkExtLed()
{
	LL_SetStateExtLed(TRUE);
	CONTROL_DelayMs(1000);
	LL_SetStateExtLed(FALSE);
}
//-----------------------------

void DBGACT_GenerateImpulseToExtLineSync1()
{
	LL_SetStateExtLineSync1(TRUE);
	CONTROL_DelayMs(100);
	LL_SetStateExtLineSync1(FALSE);
}
//-----------------------------

void DBGACT_GenerateImpulseToExtLineSync2()
{
	LL_SetStateExtLineSync2(TRUE);
	CONTROL_DelayMs(100);
	LL_SetStateExtLineSync2(FALSE);
}
//-----------------------------

void DBGACT_GenerateImpulseToIntLineSync1()
{
	LL_SetStateIntLineSync1(TRUE);
	CONTROL_DelayMs(100);
	LL_SetStateIntLineSync1(FALSE);
}
//-----------------------------

void DBGACT_GenerateImpulseToIntLineSync2()
{
	LL_SetStateIntLineSync2(TRUE);
	CONTROL_DelayMs(100);
	LL_SetStateIntLineSync2(FALSE);
}
//-----------------------------

bool DBGACT_ReadStateIntLineSync1()
{
	return LL_GetStateIntLineSync1();
}
//-----------------------------

bool DBGACT_ReadStateIntLineSync2()
{
	return LL_GetStateIntLineSync2();
}
//-----------------------------
