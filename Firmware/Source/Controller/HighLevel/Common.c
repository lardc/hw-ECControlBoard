// Header
#include "Common.h"

// Includes
#include "BCCIMHighLevel.h"
#include "DataTable.h"
#include "CommonDictionary.h"

// Variables
//
static SlaveNode NodeArray[MAX_SLAVE_NODES] = {0};

// Forward functions
//
void COMM_InitNode(NodeName Name, uint16_t RegNodeID, bool RegEmulation);
bool COMM_SlavesExecute(uint16_t Command);

// Functions
//
void COMM_InitSlaveArray()
{
	COMM_InitNode(NAME_Multiplexer,		REG_SLAVE_NID_MULTIPLEXER,	REG_EMULATE_MULTIPLEXER);
	COMM_InitNode(NAME_DCCurrent,		REG_SLAVE_NID_DC_CURRENT,	REG_EMULATE_DC_CURRENT);
	COMM_InitNode(NAME_DCHighVoltage,	REG_SLAVE_NID_DC_HV,		REG_EMULATE_DC_HV);
	COMM_InitNode(NAME_DCVoltage1,		REG_SLAVE_NID_DC_VOLTAGE1,	REG_EMULATE_DC_VOLTAGE1);
	COMM_InitNode(NAME_DCVoltage2,		REG_SLAVE_NID_DC_VOLTAGE2,	REG_EMULATE_DC_VOLTAGE2);
	COMM_InitNode(NAME_DCVoltage3,		REG_SLAVE_NID_DC_VOLTAGE3,	REG_EMULATE_DC_VOLTAGE3);
	COMM_InitNode(NAME_ACVoltage1,		REG_SLAVE_NID_AC_VOLTAGE1,	REG_EMULATE_AC_VOLTAGE1);
	COMM_InitNode(NAME_ACVoltage2,		REG_SLAVE_NID_AC_VOLTAGE2,	REG_EMULATE_AC_VOLTAGE2);
}
//-----------------------------

void COMM_InitNode(NodeName Name, uint16_t RegNodeID, bool RegEmulation)
{
	NodeArray[Name].Initialized = true;
	NodeArray[Name].Emulation = DataTable[RegEmulation];
	NodeArray[Name].NodeID = DataTable[RegNodeID];
}
//-----------------------------

bool COMM_SlavesExecute(uint16_t Command)
{
	for(uint8_t i = 0; i < MAX_SLAVE_NODES; ++i)
	{
		if(NodeArray[i].Initialized && !NodeArray[i].Emulation)
			if(!BHL_Call(NodeArray[i].NodeID, Command))
				return false;
	}

	return true;
}
//-----------------------------

bool COMM_SlavesReadState()
{
	uint16_t State, OpResult;
	bool result;

	for(uint8_t i = 0; i < MAX_SLAVE_NODES; ++i)
	{
		if(NodeArray[i].Initialized && !NodeArray[i].Emulation)
		{
			result = false;

			if(BHL_ReadRegister(NodeArray[i].NodeID, COMM_REG_DEV_STATE, &State))
				if(BHL_ReadRegister(NodeArray[i].NodeID, COMM_REG_OP_RESULT, &OpResult))
				{
					NodeArray[i].State = State;
					NodeArray[i].OpResult = OpResult;
					result = true;
				}

			if(!result)
				return false;
		}
	}

	return true;
}
//-----------------------------

bool COMM_AreSlavesInStateX(uint16_t State)
{
	bool result = true;

	for(uint16_t i = 0; i < MAX_SLAVE_NODES; ++i)
		if(NodeArray[i].Initialized && !NodeArray[i].Emulation)
		{
			if(NodeArray[i].State != State)
				result = false;
		}

	return result;
}
//-----------------------------

bool COMM_IsSlaveInFaultOrDisabled()
{
	for(uint8_t i = 0; i < MAX_SLAVE_NODES; ++i)
		if(NodeArray[i].Initialized && !NodeArray[i].Emulation)
		{
			if(NodeArray[i].State == CDS_Fault || NodeArray[i].State == CDS_Disabled)
				return true;
		}

	return false;
}
//-----------------------------

bool COMM_SlavesEnablePower()
{
	return COMM_SlavesExecute(COMM_ACT_ENABLE_POWER);
}
//-----------------------------

bool COMM_SlavesDisablePower()
{
	return COMM_SlavesExecute(COMM_ACT_DISABLE_POWER);
}
//-----------------------------

bool COMM_SlavesClearFault()
{
	return COMM_SlavesExecute(COMM_ACT_FAULT_CLEAR);
}
//-----------------------------

bool COMM_SlavesClearWarning()
{
	return COMM_SlavesExecute(COMM_ACT_WARNING_CLEAR);
}
//-----------------------------

pSlaveNode COMM_GetSlaveDevicePointer(NodeName Name)
{
	return &(NodeArray[Name]);
}
//-----------------------------
