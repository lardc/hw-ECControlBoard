// Header
#include "LowLevel.h"
// Include
#include "Board.h"
#include "Delay.h"

// Functions
//
void LL_ToggleBoardLED()
{
	GPIO_Toggle(GPIO_INT_LED);
}
//-----------------------------

void LL_SetStateExtLed(bool State)
{
	GPIO_SetState(GPIO_EXT_LED, State);
}
//-----------------------------

void LL_SetStateFan(bool State)
{
	GPIO_SetState(GPIO_FAN, State);
}
//-----------------------------

void LL_SetStateExtLineSynce1(bool State)
{
	GPIO_SetState(GPIO_EXT_SYNC1_OUT, State);
}
//-----------------------------

void LL_SetStateExtLineSynce2(bool State)
{
	GPIO_SetState(GPIO_EXT_SYNC2_OUT, State);
}
//-----------------------------

void LL_SetStateIntLineSynce1(bool State)
{
	GPIO_SetState(GPIO_INT_SYNC1_OUT, State);
}
//-----------------------------

void LL_SetStateIntLineSynce2(bool State)
{
	GPIO_SetState(GPIO_INT_SYNC2_OUT, State);
}
//-----------------------------

bool LL_GetStateIntLineSynce1()
{
	return GPIO_GetState(GPIO_INT_SYNC1_IN);
}
//-----------------------------

bool LL_GetStateIntLineSynce2()
{
	return GPIO_GetState(GPIO_INT_SYNC2_IN);
}
//-----------------------------
