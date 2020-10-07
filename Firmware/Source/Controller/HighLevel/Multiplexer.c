// Header
#include "Multiplexer.h"

// Includes
#include "MultiplexerDictionary.h"
#include "BCCIMHighLevel.h"
#include "Common.h"

// Functions
//
bool MUX_ExecuteCommutation(uint16_t NodeID, MeasurementType MeasureType, DL_Case Case, uint16_t Position,
		DL_InputType InputType)
{
	bool result = false;

	if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_MEASURE, MeasureType))
		if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_CASE, Case))
			if(BHL_WriteRegister(NodeID, MUX_REG_POSITION_OF_CASE, Position))
				if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_SIGNAL_CTRL, InputType));

	return result;
}
//-----------------------------
