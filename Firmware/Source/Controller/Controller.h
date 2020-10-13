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
	DS_InProcess = 4,
	DS_SafetyTrig = 5
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
	DSS_OnVoltage_ReadResult = 12,

	DSS_Leakage_StartTest = 13,
	DSS_Leakage_Commutate = 14,
	DSS_Leakage_StartControl = 15,
	DSS_Leakage_WaitControlReady = 16,
	DSS_Leakage_StartOutVoltage = 17,
	DSS_Leakage_WaitOutVoltageReady = 18,
	DSS_Leakage_StopOutVoltage = 19,
	DSS_Leakage_StopControl = 20,
	DSS_Leakage_UnCommutate = 21,
	DSS_Leakage_ReadResult = 22,

	DSS_Control_StartTest = 23,
	DSS_Control_Commutate = 24,
	DSS_Control_StartControl = 25,
	DSS_Control_WaitControlReady = 26,
	DSS_Control_StopControl = 27,
	DSS_Control_UnCommutate = 28,
	DSS_Control_ReadResult = 29
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
