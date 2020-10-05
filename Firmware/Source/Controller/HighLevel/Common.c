// Header
#include "Common.h"

// Types
typedef enum __NodeName
{
	NAME_Multiplexer = 1,
	NAME_DCCurrent = 2,
	NAME_DCHighVoltage = 3,
	NAME_DCVoltage1 = 4,
	NAME_DCVoltage2 = 5,
	NAME_DCVoltage3 = 6,
	NAME_ACVoltage1 = 7,
	NAME_ACVoltage2 = 8

} NodeName;

typedef struct __SlaveNode
{
	bool Emulation;
	uint16_t NodeID;

} SlaveNode, *pSlaveNode;

// Functions

//-----------------------------
