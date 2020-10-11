// Header
#include "ACVoltageBoard.h"

// Includes
#include "BCCIMHighLevel.h"

// Functions
//
ExecutionResult ACV_Execute(NodeName Name)
{
	if(Name != NAME_ACVoltage1 && Name != NAME_ACVoltage2)
		return ER_WrongNode;

	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(Name);
	pACVoltageBoardObject Settings = (pACVoltageBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t VoltageLow = (uint16_t)(Settings->Setpoint.Voltage & 0xFFFF);
			uint16_t CurrentLow = (uint16_t)(Settings->Setpoint.Current & 0xFFFF);
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_WriteRegister(NodeID, ACV_REG_OUTPUT_LINE, Settings->OutputLine))
				if(BHL_WriteRegister(NodeID, ACV_REG_CURRENT_SETPOINT, CurrentLow))
					if(BHL_WriteRegister(NodeID, ACV_REG_VOLTAGE_SETPOINT, VoltageLow))
						if(BHL_Call(NodeID, ACV_ACT_START_PROCESS))
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

ExecutionResult ACV_ReadResult(NodeName Name)
{
	if(Name != NAME_ACVoltage1 && Name != NAME_ACVoltage2)
		return ER_WrongNode;

	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(Name);
	pACVoltageBoardObject Settings = (pACVoltageBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t CurrentLow = 0, VoltageLow = 0;
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_ReadRegister(NodeID, ACV_REG_CURRENT_RESULT, &CurrentLow))
				if(BHL_ReadRegister(NodeID, ACV_REG_VOLTAGE_RESULT, &VoltageLow))
				{
					Settings->Result.Current = CurrentLow;
					Settings->Result.Voltage = VoltageLow;
					return ER_NoError;
				}
		}
		else
		{
			Settings->Result.Current = ACV_EMULATION_RES_CURRENT;
			Settings->Result.Voltage = ACV_EMULATION_RES_VOLTAGE;

			return ER_NoError;
		}

		return ER_InterfaceError;
	}
	else
		return ER_SettingsError;
}
//-----------------------------

ExecutionResult ACV_Stop(NodeName Name)
{
	if(Name != NAME_ACVoltage1 && Name != NAME_ACVoltage2)
		return ER_WrongNode;

	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(Name);
	pACVoltageBoardObject Settings = (pACVoltageBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t NodeID = NodeData->NodeID;
			if(BHL_Call(NodeID, ACV_ACT_STOP_PROCESS))
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
