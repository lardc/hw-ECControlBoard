// Header
#include "DCVoltageBoard.h"

// Includes
#include "BCCIMHighLevel.h"

// Functions
//
bool DCV_Execute(pDCVoltageBoardObject Settings)
{
	bool result = false;

	uint16_t VoltageLow = (uint16_t)(Settings->Setpoint.Voltage & 0xFFFF);
	uint16_t VoltageHigh = (uint16_t)(Settings->Setpoint.Voltage >> 16);
	uint16_t CurrentLow = (uint16_t)(Settings->Setpoint.Current & 0xFFFF);
	uint16_t CurrentHigh = (uint16_t)(Settings->Setpoint.Current >> 16);
	uint16_t NodeID = Settings->NodeID;

	if(BHL_WriteRegister(NodeID, DCV_REG_OUTPUT_LINE, Settings->OutputLine))
		if(BHL_WriteRegister(NodeID, DCV_REG_OUTPUT_TYPE, Settings->OutputType))
			if(BHL_WriteRegister(NodeID, DCV_REG_OUTPUT_MODE, Settings->OutputMode))
				if(BHL_WriteRegister(NodeID, DCV_REG_PULSE_LENGTH, Settings->PulseLength))
					if(BHL_WriteRegister(NodeID, DCV_REG_CURRENT_SETPOINT, CurrentLow))
						if(BHL_WriteRegister(NodeID, DCV_REG_CURRENT_SETPOINT_32, CurrentHigh))
							if(BHL_WriteRegister(NodeID, DCV_REG_VOLTAGE_SETPOINT, VoltageLow))
								if(BHL_WriteRegister(NodeID, DCV_REG_VOLTAGE_SETPOINT_32, VoltageHigh))
									if(BHL_Call(NodeID, DCV_ACT_START_PROCESS))
										result = true;

	return result;
}
//-----------------------------

bool DCV_ReadResult(pDCVoltageBoardObject Settings)
{
	bool result = false;
	uint16_t CurrentLow = 0, CurrentHigh = 0, VoltageLow = 0, VoltageHigh = 0;
	uint16_t NodeID = Settings->NodeID;

	if(BHL_ReadRegister(NodeID, DCV_REG_CURRENT_RESULT, &CurrentLow))
		if(BHL_ReadRegister(NodeID, DCV_REG_CURRENT_RESULT_32, &CurrentHigh))
			if(BHL_ReadRegister(NodeID, DCV_REG_VOLTAGE_RESULT, &VoltageLow))
				if(BHL_ReadRegister(NodeID, DCV_REG_VOLTAGE_RESULT_32, &VoltageHigh))
				{
					VIPair Result;
					Result.Current = CurrentLow;
					Result.Current |= (uint32_t)CurrentHigh << 16;
					Result.Voltage = VoltageLow;
					Result.Voltage |= (uint32_t)VoltageHigh << 16;
					Settings->Result = Result;
					result = true;
				}

	return result;
}
//-----------------------------

bool DCV_Stop(pDCVoltageBoardObject Settings)
{
	return BHL_Call(Settings->NodeID, DCV_ACT_STOP_PROCESS);
}
//-----------------------------
