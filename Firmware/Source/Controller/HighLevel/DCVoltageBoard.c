// Header
#include "DCVoltageBoard.h"

// Includes
#include "DCVoltageBoardDictionary.h"
#include "BCCIMHighLevel.h"

// Functions
//
bool CURR_Execute(pDCVoltageBoardObject Settings)
{
	bool result = false;

	uint16_t VoltageLow = (uint16_t)(Settings->Setpoint.Voltage & 0xFFFF);
	uint16_t VoltageHigh = (uint16_t)(Settings->Setpoint.Voltage >> 16);
	uint16_t CurrentLow = (uint16_t)(Settings->Setpoint.Current & 0xFFFF);
	uint16_t CurrentHigh = (uint16_t)(Settings->Setpoint.Current >> 16);

	if(BHL_WriteRegister(NodeID, DCV_REG_OUTPUT_LINE, Settings->OutputLine))
		if(BHL_WriteRegister(NodeID, DCV_REG_OUTPUT_TYPE, Settings->OutputType))
			if(BHL_WriteRegister(NodeID, DCV_REG_OUTPUT_MODE, Settings->OutputMode))
				if(BHL_WriteRegister(NodeID, DCV_REG_PULSE_LENGTH, Settings->PulseLength))

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
