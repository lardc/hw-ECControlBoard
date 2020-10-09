// Header
#include "Multiplexer.h"

// Includes
#include "BCCIMHighLevel.h"

// Forward functions
//
ExecutionResult MUX_Connect()
{
	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_Multiplexer);
	pMuxObject Settings = (pMuxObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_MEASURE, Settings->MeasureType))
				if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_CASE, Settings->Case))
					if(BHL_WriteRegister(NodeID, MUX_REG_POSITION_OF_CASE, Settings->Position))
						if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_SIGNAL_CTRL, Settings->InputType))
							if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_LEAKAGE, Settings->LeakageType))
								if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_POLARITY, Settings->Polarity))
									if(BHL_Call(NodeID, MUX_ACT_SET_RELAY_GROUP))
										return ER_NoError;
		}
		else
			return ER_NoError;

		return ER_InterfaceError;
	}
	else
		return ER_LogicError;
}
//-----------------------------

ExecutionResult MUX_Disconnect()
{
	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_Multiplexer);
	pMuxObject Settings = (pMuxObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t NodeID = NodeData->NodeID;
			if(BHL_Call(NodeID, MUX_ACT_SET_RELAY_NONE))
				return ER_NoError;
		}
		else
			return ER_NoError;

		return ER_InterfaceError;
	}
	else
		return ER_LogicError;
}
//-----------------------------
