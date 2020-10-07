// Header
#include "Multiplexer.h"

// Includes
#include "MultiplexerDictionary.h"
#include "BCCIMHighLevel.h"
#include "Common.h"

// Functions
//
bool MUX_Commutate(uint16_t NodeID, MeasurementType MeasureType, DL_Case Case, uint16_t Position,
		DL_InputType InputType, uint16_t LeakageType, uint16_t Polarity)
{
	bool result = false;

	if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_MEASURE, MeasureType))
		if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_CASE, Case))
			if(BHL_WriteRegister(NodeID, MUX_REG_POSITION_OF_CASE, Position))
				if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_SIGNAL_CTRL, InputType))
					if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_LEAKAGE, LeakageType))
						if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_POLARITY, Polarity))
							result = true;

	return result;
}
//-----------------------------

bool MUX_CommutateObject(MuxObject Settings)
{
	return MUX_Commutate(Settings.NodeID, Settings.MeasureType, Settings.Case,
			Settings.Position, Settings.InputType, Settings.LeakageType, Settings.Polarity);
}
//-----------------------------
