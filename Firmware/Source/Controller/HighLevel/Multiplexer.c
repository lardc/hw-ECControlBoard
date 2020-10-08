// Header
#include "Multiplexer.h"

// Includes
#include "MultiplexerDictionary.h"
#include "BCCIMHighLevel.h"

// Forward functions
//
bool MUX_ConnectObject(MuxObject Settings)
{
	bool result = false;
	uint16_t NodeID = Settings.NodeID;

	if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_MEASURE, Settings.MeasureType))
		if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_CASE, Settings.Case))
			if(BHL_WriteRegister(NodeID, MUX_REG_POSITION_OF_CASE, Settings.Position))
				if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_SIGNAL_CTRL, Settings.InputType))
					if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_LEAKAGE, Settings.LeakageType))
						if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_POLARITY, Settings.Polarity))
							if(BHL_Call(NodeID, MUX_ACT_SET_RELAY_GROUP))
								result = true;

	return result;
}
//-----------------------------

bool MUX_Disconnect(MuxObject Settings)
{
	return BHL_Call(Settings.NodeID, MUX_ACT_SET_RELAY_NONE);
}
//-----------------------------
