// Header
#include "CurrentBoard.h"

// Includes
#include "CurrentBoardDictionary.h"
#include "BCCIMHighLevel.h"

// Functions
//
bool CURR_Execute(pCurrentBoardObject Settings)
{
	bool result = false;

	uint16_t CurrentLow = (uint16_t)(Settings->Setpoint.Current & 0xFFFF);
	uint16_t CurrentHigh = (uint16_t)(Settings->Setpoint.Current >> 16);
	uint16_t NodeID = Settings->SlaveNode->NodeID;

	if(BHL_WriteRegister(NodeID, CURR_REG_CURRENT_SETPOINT, CurrentLow))
		if(BHL_WriteRegister(NodeID, CURR_REG_CURRENT_SETPOINT_32, CurrentHigh))
			if(BHL_WriteRegister(NodeID, CURR_REG_VOLTAGE_DUT_LIM, Settings->Setpoint.Voltage))
				if(BHL_Call(NodeID, CURR_ACT_START_PROCESS))
					result = true;

	return result;
}
//-----------------------------

bool CURR_ReadResult(pCurrentBoardObject Settings)
{
	bool result = false;
	uint16_t CurrentLow = 0, CurrentHigh = 0, Voltage = 0;
	uint16_t NodeID = Settings->SlaveNode->NodeID;

	if(BHL_ReadRegister(NodeID, CURR_REG_RESULT_CURRENT, &CurrentLow))
		if(BHL_ReadRegister(NodeID, CURR_REG_RESULT_CURRENT_32, &CurrentHigh))
			if(BHL_ReadRegister(NodeID, CURR_REG_RESULT_VOLTAGE, &Voltage))
			{
				Settings->Result.Current = CurrentLow;
				Settings->Result.Current |= (uint32_t)CurrentHigh << 16;
				Settings->Result.Voltage = Voltage;
				result = true;
			}

	return result;
}
//-----------------------------
