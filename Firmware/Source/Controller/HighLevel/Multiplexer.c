// Header
#include "Multiplexer.h"

// Includes
#include "MultiplexerDictionary.h"
#include "BCCIMHighLevel.h"

// Forward functions
//
bool MUX_Connect(uint16_t NodeID, MeasurementType MeasureType, DL_Case Case, MUX_Position Position,
		DL_InputType InputType, MUX_LeakageType LeakageType, MUX_Polarity Polarity);

// Functions
//
bool MUX_Connect(uint16_t NodeID, MeasurementType MeasureType, DL_Case Case, MUX_Position Position,
		DL_InputType InputType, MUX_LeakageType LeakageType, MUX_Polarity Polarity)
{
	bool result = false;

	if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_MEASURE, MeasureType))
		if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_CASE, Case))
			if(BHL_WriteRegister(NodeID, MUX_REG_POSITION_OF_CASE, Position))
				if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_SIGNAL_CTRL, InputType))
					if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_LEAKAGE, LeakageType))
						if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_POLARITY, Polarity))
							if(BHL_Call(NodeID, MUX_ACT_SET_RELAY_GROUP))
								result = true;

	return result;
}
//-----------------------------

bool MUX_Disconnect(uint16_t NodeID)
{
	return BHL_Call(NodeID, MUX_ACT_SET_RELAY_NONE);
}
//-----------------------------

bool MUX_ConnectObject(MuxObject Settings)
{
	return MUX_Connect(Settings.NodeID, Settings.MeasureType, Settings.Case,
			Settings.Position, Settings.InputType, Settings.LeakageType, Settings.Polarity);
}
//-----------------------------
