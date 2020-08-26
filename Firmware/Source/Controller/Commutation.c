// Header
#include "Commutation.h"

// Includes
#include "Global.h"
#include "DataTable.h"
#include "ZwSPI.h"
#include "LowLevel.h"

#define SELECT_ALL_RELAY_IN_REGISTER 0xFF
// Variables
static uint8_t ShiftRegistersState[REGISTERS_NUM] = {0};
static uint8_t BistableBits[REGISTERS_NUM] = {0};

// Forward functions
void COMM_SwitchSimpleDevice(RegisterPin Device, bool State);
void COMM_SwitchBistableDevice(BistableSwitch Device, bool State);
void COMM_ApplyCommutation();
void COMM_DisableCtrlOfBistableRelay();
void COMM_CleanShiftRegister();

// Functions
void COMM_SwitchBistableRelay(uint8_t IndexRelay, bool NewState)
{
	COMM_SwitchBistableDevice(*COMM_BistableRelayArray[IndexRelay], NewState);

	COMM_ApplyCommutation();
	CONTROL_DelayMs(BISTABLE_SWITCH_DELAY);
	COMM_DisableCtrlOfBistableRelay();
}
// ----------------------------------------

void COMM_SwitchSimpleRelay(uint8_t IndexRelay, bool NewState)
{
	COMM_SwitchSimpleDevice(*COMM_SimpleRelayArray[IndexRelay], NewState);
	COMM_ApplyCommutation();
}
// ----------------------------------------

void COMM_DisconnectSimpleRelays()
{
	COMM_CleanShiftRegister();
	COMM_ApplyCommutation();
}
// ----------------------------------------

void COMM_SwitchSimpleDevice(RegisterPin Device, bool State)
{
	if(State)
		ShiftRegistersState[Device.Register] |= Device.Bit;
	else
		ShiftRegistersState[Device.Register] &= ~Device.Bit;
}
// ----------------------------------------

void COMM_SwitchBistableDevice(BistableSwitch Device, bool State)
{
	if(State)
	{
		ShiftRegistersState[Device.TurnOnPin.Register] |= Device.TurnOnPin.Bit;
		BistableBits[Device.TurnOnPin.Register] |= Device.TurnOnPin.Bit;
	}
	else
	{
		ShiftRegistersState[Device.TurnOffPin.Register] |= Device.TurnOffPin.Bit;
		BistableBits[Device.TurnOffPin.Register] |= Device.TurnOffPin.Bit;
	}
}
// ----------------------------------------

void COMM_ApplyCommutation()
{
	LL_WriteToShiftRegister(ShiftRegistersState, REGISTERS_NUM);
}
// ----------------------------------------

void COMM_DisconnectBistableRelays()
{
	for(uint8_t i = 0; i < BISTABLE_ARRAY_SIZE; i++)
		COMM_SwitchBistableRelay(i, false);
}
// ----------------------------------------

void COMM_DisableCtrlOfBistableRelay()
{
	for(uint8_t i = 0; i < REGISTERS_NUM; i++)
	{
		ShiftRegistersState[i] &= ~BistableBits[i];
		BistableBits[i] = 0;
	}
	COMM_ApplyCommutation();
}
// ----------------------------------------

void COMM_CleanShiftRegister()
{
	for(uint8_t i = 0; i < REGISTERS_NUM; i++)
	{
		ShiftRegistersState[i] = 0;
	}
}
// ----------------------------------------
