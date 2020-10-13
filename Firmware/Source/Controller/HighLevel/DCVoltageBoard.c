// Header
#include "DCVoltageBoard.h"

// Includes
#include "BCCIMHighLevel.h"

// Functions
//
ExecutionResult DCV_Execute(NodeName Name)
{
	if(Name != NAME_DCVoltage1 && Name != NAME_DCVoltage2 && Name != NAME_DCVoltage3)
		return ER_WrongNode;

	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(Name);
	pDCVoltageBoardObject Settings = (pDCVoltageBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t VoltageLow = (uint16_t)(Settings->Setpoint.Voltage & 0xFFFF);
			uint16_t VoltageHigh = (uint16_t)(Settings->Setpoint.Voltage >> 16);
			uint16_t CurrentLow = (uint16_t)(Settings->Setpoint.Current & 0xFFFF);
			uint16_t CurrentHigh = (uint16_t)(Settings->Setpoint.Current >> 16);
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_WriteRegister(NodeID, DCV_REG_OUTPUT_LINE, Settings->OutputLine))
				if(BHL_WriteRegister(NodeID, DCV_REG_OUTPUT_TYPE, Settings->OutputType))
					if(BHL_WriteRegister(NodeID, DCV_REG_OUTPUT_MODE, Settings->OutputMode))
						if(BHL_WriteRegister(NodeID, DCV_REG_PULSE_LENGTH, Settings->PulseLength))
							if(BHL_WriteRegister(NodeID, DCV_REG_CURRENT_SETPOINT, CurrentLow))
								if(BHL_WriteRegister(NodeID, DCV_REG_CURRENT_SETPOINT_32, CurrentHigh))
									if(BHL_WriteRegister(NodeID, DCV_REG_VOLTAGE_SETPOINT, VoltageLow))
										if(BHL_WriteRegister(NodeID, DCV_REG_VOLTAGE_SETPOINT_32, VoltageHigh))
											if(BHL_Call(NodeID, DCV_ACT_START_PROCESS))
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

ExecutionResult DCV_ReadResult(NodeName Name)
{
	if(Name != NAME_DCVoltage1 && Name != NAME_DCVoltage2 && Name != NAME_DCVoltage3)
		return ER_WrongNode;

	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(Name);
	pDCVoltageBoardObject Settings = (pDCVoltageBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t CurrentLow = 0, CurrentHigh = 0, VoltageLow = 0, VoltageHigh = 0;
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_ReadRegister(NodeID, DCV_REG_CURRENT_RESULT, &CurrentLow))
				if(BHL_ReadRegister(NodeID, DCV_REG_CURRENT_RESULT_32, &CurrentHigh))
					if(BHL_ReadRegister(NodeID, DCV_REG_VOLTAGE_RESULT, &VoltageLow))
						if(BHL_ReadRegister(NodeID, DCV_REG_VOLTAGE_RESULT_32, &VoltageHigh))
						{
							Settings->Result.Current = CurrentLow;
							Settings->Result.Current |= (uint32_t)CurrentHigh << 16;
							Settings->Result.Voltage = VoltageLow;
							Settings->Result.Voltage |= (uint32_t)VoltageHigh << 16;
							return ER_NoError;
						}
		}
		else
		{
			Settings->Result.Current = DCV_EMULATION_RES_CURRENT;
			Settings->Result.Voltage = DCV_EMULATION_RES_VOLTAGE;

			return ER_NoError;
		}

		return ER_InterfaceError;
	}
	else
		return ER_SettingsError;
}
//-----------------------------

ExecutionResult DCV_Stop(NodeName Name)
{
	if(Name != NAME_DCVoltage1 && Name != NAME_DCVoltage2 && Name != NAME_DCVoltage3)
		return ER_WrongNode;

	return COMM_NodeCall(Name, DCV_ACT_STOP_PROCESS);
}
//-----------------------------

ExecutionResult DCV_IsVoltageReady(NodeName Name, bool *VoltageReady)
{
	if(Name != NAME_DCVoltage1 && Name != NAME_DCVoltage2 && Name != NAME_DCVoltage3)
		return ER_WrongNode;

	return COMM_NodeOutputReady(Name, DCV_REG_VOLTAGE_READY, VoltageReady);
}
//-----------------------------
