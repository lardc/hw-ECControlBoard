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
}
//-----------------------------

void LL_SetStateFan(bool State)
{
}
//-----------------------------

void LL_SetStateExtLineSync1(bool State)
{
}
//-----------------------------

void LL_SetStateExtLineSync2(bool State)
{
}
//-----------------------------

void LL_SetStateIntLineSync1(bool State)
{
}
//-----------------------------

void LL_SetStateIntLineSync2(bool State)
{
}
//-----------------------------

bool LL_GetStateIntLineSync1()
{
	return false;
}
//-----------------------------

bool LL_GetStateIntLineSync2()
{
	return false;
}
//-----------------------------
