// Header
#include "Common.h"

// Includes
#include "DataTable.h"
#include "DeviceObjectDictionary.h"

// Types
//
typedef enum __NodeName
{
	NAME_Multiplexer = 0,
	NAME_DCCurrent = 1,
	NAME_DCHighVoltage = 2,
	NAME_DCVoltage1 = 3,
	NAME_DCVoltage2 = 4,
	NAME_DCVoltage3 = 5,
	NAME_ACVoltage1 = 6,
	NAME_ACVoltage2 = 7

} NodeName;

typedef struct __SlaveNode
{
	bool Initialized;
	bool Emulation;
	uint16_t NodeID;

} SlaveNode, *pSlaveNode;

// Variables
//
static SlaveNode Nodes[MAX_SLAVE_NODES] = {0};

// Forward functions
//
void COMM_InitNode(NodeName Name, uint16_t RegNodeID, bool RegEmulation);

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
	Nodes[Name].Initialized = true;
	Nodes[Name].Emulation = DataTable[RegEmulation];
	Nodes[Name].NodeID = DataTable[RegNodeID];
}
//-----------------------------
