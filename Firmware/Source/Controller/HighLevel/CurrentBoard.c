// Header
#include "CurrentBoard.h"

// Includes
#include "BCCIMHighLevel.h"

// Functions
//
ExecutionResult CURR_Execute()
{
	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_DCCurrent);
	pCurrentBoardObject Settings = (pCurrentBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t CurrentLow = (uint16_t)(Settings->Setpoint.Current & 0xFFFF);
			uint16_t CurrentHigh = (uint16_t)(Settings->Setpoint.Current >> 16);
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_WriteRegister(NodeID, CURR_REG_CURRENT_SETPOINT, CurrentLow))
				if(BHL_WriteRegister(NodeID, CURR_REG_CURRENT_SETPOINT_32, CurrentHigh))
					if(BHL_WriteRegister(NodeID, CURR_REG_VOLTAGE_DUT_LIM, Settings->Setpoint.Voltage))
						if(BHL_Call(NodeID, CURR_ACT_START_PROCESS))
							return ER_NoError;
		}
		else
			return ER_NoError;

		return ER_InterfaceError;
	}
	else
		return ER_SettingsError;
}
//-----------------------------

ExecutionResult CURR_ReadResult()
{
	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_DCCurrent);
	pCurrentBoardObject Settings = (pCurrentBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t CurrentLow = 0, CurrentHigh = 0, Voltage = 0;
			uint16_t NodeID = NodeData->NodeID;;

			if(BHL_ReadRegister(NodeID, CURR_REG_RESULT_CURRENT, &CurrentLow))
				if(BHL_ReadRegister(NodeID, CURR_REG_RESULT_CURRENT_32, &CurrentHigh))
					if(BHL_ReadRegister(NodeID, CURR_REG_RESULT_VOLTAGE, &Voltage))
					{
						Settings->Result.Current = CurrentLow;
						Settings->Result.Current |= (uint32_t)CurrentHigh << 16;
						Settings->Result.Voltage = Voltage;
						return ER_NoError;
					}
		}
		else
		{
			Settings->Result.Current = CURR_EMULATION_RES_CURRENT;
			Settings->Result.Voltage = CURR_EMULATION_RES_VOLTAGE;

			return ER_NoError;
		}

		return ER_InterfaceError;
	}
	else
		return ER_SettingsError;
}
//-----------------------------
