// Header
#include "DCHighVoltageBoard.h"

// Includes
#include "BCCIMHighLevel.h"

// Functions
//
ExecutionResult DCHV_Execute()
{
	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_DCHighVoltage);
	pDCHVoltageBoardObject Settings = (pDCHVoltageBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t VoltageLow = (uint16_t)(Settings->Setpoint.Voltage & 0xFFFF);
			uint16_t CurrentLow = (uint16_t)(Settings->Setpoint.Current & 0xFFFF);
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_WriteRegister(NodeID, DCHV_REG_CURRENT_SETPOINT, CurrentLow))
				if(BHL_WriteRegister(NodeID, DCHV_REG_VOLTAGE_SETPOINT, VoltageLow))
					if(BHL_Call(NodeID, DCHV_ACT_START_PROCESS))
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

ExecutionResult DCHV_ReadResult()
{
	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_DCHighVoltage);
	pDCHVoltageBoardObject Settings = (pDCHVoltageBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t CurrentLow = 0, VoltageLow = 0;
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_ReadRegister(NodeID, DCHV_REG_CURRENT_RESULT, &CurrentLow))
				if(BHL_ReadRegister(NodeID, DCHV_REG_VOLTAGE_RESULT, &VoltageLow))
				{
					Settings->Result.Current = CurrentLow;
					Settings->Result.Voltage = VoltageLow;
					return ER_NoError;
				}
		}
		else
		{
			Settings->Result.Current = DCHV_EMULATION_RES_CURRENT;
			Settings->Result.Voltage = DCHV_EMULATION_RES_VOLTAGE;

			return ER_NoError;
		}

		return ER_InterfaceError;
	}
	else
		return ER_SettingsError;
}
//-----------------------------

ExecutionResult DCHV_IsVoltageReady(bool *VoltageReady)
{
	return COMM_NodeOutputReady(NAME_DCHighVoltage, DCHV_REG_VOLTAGE_READY, VoltageReady);
}
//-----------------------------

ExecutionResult DCHV_Stop()
{
	return COMM_NodeCall(NAME_DCHighVoltage, DCHV_ACT_STOP_PROCESS);
}
//-----------------------------
