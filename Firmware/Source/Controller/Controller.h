#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// Include
//
#include "stdinc.h"
#include "Common.h"

// Types
//
typedef enum __DeviceState
{
	DS_None = 0,
	DS_Fault = 1,
	DS_Disabled = 2,
	DS_Ready = 3,
	DS_InProcess = 4
} DeviceState;

typedef enum __DeviceSubState
{
	DSS_None = 0,

	DSS_PowerEnable = 1,
	DSS_PowerWaitReady = 2,
	DSS_PowerOff = 3,

	DSS_OnVoltage_StartTest = 4,
	DSS_OnVoltage_Commutate = 5,
	DSS_OnVoltage_StartControl = 6,
	DSS_OnVoltage_WaitControlReady = 7,
	DSS_OnVoltage_PulseCurrent = 8,
	DSS_OnVoltage_WaitCurrentReady = 9,
	DSS_OnVoltage_StopControl = 10,
	DSS_OnVoltage_UnCommutate = 11,
	DSS_OnVoltage_ReadResult = 12
} DeviceSubState;

// Variables
//
extern volatile Int64U CONTROL_TimeCounter;
extern DeviceState CONTROL_State;
extern DeviceSubState CONTROL_SubState;

// Functions
//
void CONTROL_Init();
void CONTROL_Idle();
void CONTROL_SwitchToFault(ExecutionResult Result, Int16U Group);
void CONTROL_SetDeviceState(DeviceState NewState, DeviceSubState NewSubState);

#endif // __CONTROLLER_H
