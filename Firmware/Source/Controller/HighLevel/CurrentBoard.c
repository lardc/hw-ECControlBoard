// Header
#include "CurrentBoard.h"

// Includes
#include "CurrentBoardDictionary.h"
#include "BCCIMHighLevel.h"

// Functions
//
bool CURR_Execute(uint16_t NodeID, VIPair Setpoint)
{
	bool result = false;

	uint16_t CurrentLow = (uint16_t)(Setpoint.Current & 0xFFFF);
	uint16_t CurrentHigh = (uint16_t)(Setpoint.Current >> 16);

	if(BHL_WriteRegister(NodeID, CURR_REG_CURRENT_SETPOINT, CurrentLow))
		if(BHL_WriteRegister(NodeID, CURR_REG_CURRENT_SETPOINT_32, CurrentHigh))
			if(BHL_WriteRegister(NodeID, CURR_REG_VOLTAGE_DUT_LIM, Setpoint.Voltage))
				if(BHL_Call(NodeID, CURR_ACT_START_PROCESS))
					result = true;

	return result;
}
//-----------------------------

bool CURR_ReadResult(uint16_t NodeID, pVIPair Result)
{
	bool result = false;
	uint16_t CurrentLow = 0, CurrentHigh = 0, Voltage = 0;

	if(BHL_ReadRegister(NodeID, CURR_REG_RESULT_CURRENT, &CurrentLow))
		if(BHL_ReadRegister(NodeID, CURR_REG_RESULT_CURRENT_32, &CurrentHigh))
			if(BHL_ReadRegister(NodeID, CURR_REG_RESULT_VOLTAGE, &Voltage))
			{
				Result->Current = CurrentLow;
				Result->Current |= (uint32_t)CurrentHigh << 16;
				Result->Voltage = Voltage;
				result = true;
			}

	return result;
}
//-----------------------------
