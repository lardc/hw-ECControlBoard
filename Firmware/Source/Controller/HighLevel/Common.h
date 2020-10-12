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
	MT_InputVoltageCurrent = 3,
	MT_InhibitVoltage = 4
} MeasurementType;

typedef enum __ExecutionResult
{
	ER_NoError = 0,
	ER_InterfaceError = 1,
	ER_SettingsError = 2,
	ER_WrongNode = 3,
	ER_ChangeStateTimeout = 4,
	ER_WrongState = 5
} ExecutionResult;

typedef enum __LeakageType
{
	LeakageDC = 1,
	LeakageAC = 2
} LeakageType;

typedef enum __InputType
{
	ControlIDC = 1,
	ControlVDC = 2,
	ControlVAC = 3
} InputType;

typedef struct __SlaveNode
{
	bool Emulation;
	uint16_t NodeID;
	uint16_t State;
	uint16_t OpResult;
	void *Settings;
} SlaveNode, *pSlaveNode;

typedef struct __VIPair
{
	uint32_t Voltage;
	uint32_t Current;
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
