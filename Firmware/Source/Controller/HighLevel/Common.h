#ifndef __COMMON_H
#define __COMMON_H

// Include
#include "stdinc.h"
#include "CommonDictionary.h"
#include "DUTLibrary.h"

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

typedef enum __MeasurementType
{
	MT_LeakageCurrent = 1,
	MT_OnVoltage = 2,
	MT_InputVoltage = 3,
	MT_InhibitVoltage = 4

} MeasurementType;

typedef struct __SlaveNode
{
	bool Initialized;
	bool Emulation;
	uint16_t NodeID;
	uint16_t State;
	uint16_t OpResult;

} SlaveNode, *pSlaveNode;

typedef struct __VIPair
{
	float Voltage;
	float Current;

} VIPair, *pVIPair;

// Functions
//
void COMM_InitSlaveArray();

bool COMM_SlavesReadState();
bool COMM_AreSlavesInStateX(uint16_t State);
bool COMM_IsSlaveInFaultOrDisabled();

bool COMM_SlavesEnablePower();
bool COMM_SlavesDisablePower();
bool COMM_SlavesClearFault();
bool COMM_SlavesClearWarning();

pSlaveNode COMM_GetSlaveDevicePointer(NodeName Name);

#endif // __COMMON_H
