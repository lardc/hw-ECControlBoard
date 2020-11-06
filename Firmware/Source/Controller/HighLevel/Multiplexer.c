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
			MUX_ControlSupply InputVoltageSupply = (Settings->InputType == IT_ControlVAC) ? MUX_ControlAC : MUX_ControlDC;

			if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_MEASURE, Settings->MeasureType))
				if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_CASE, Settings->Case))
					if(BHL_WriteRegister(NodeID, MUX_REG_POSITION_OF_CASE, Settings->Position))
						if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_SIGNAL_CTRL, InputVoltageSupply))
							if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_LEAKAGE, Settings->LeakageType))
								if(BHL_WriteRegister(NodeID, MUX_REG_TYPE_POLARITY, Settings->Polarity))
								{
									if(BHL_Call(NodeID, MUX_ACT_SET_RELAY_GROUP))
									{
										NodeData->StateIsUpToDate = false;
										return ER_NoError;
									}
									else
									{
										BHLError err = BHL_GetError();
										if(err.Func == FUNCTION_CALL && err.ErrorCode == ERR_USER &&
												err.ExtData == MUX_ACT_SET_RELAY_GROUP && err.Details == COMM_ERR_BAD_CONFIG)
										{
											return ER_BadHighLevelConfig;
										}
									}
								}
		}
		else
			return ER_NoError;

		return ER_InterfaceError;
	}
	else
		return ER_SettingsError;
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
			{
				NodeData->StateIsUpToDate = false;
				return ER_NoError;
			}
		}
		else
			return ER_NoError;

		return ER_InterfaceError;
	}
	else
		return ER_SettingsError;
}
//-----------------------------
