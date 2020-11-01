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

	DSS_Leakage_StartTest = 100,
	DSS_Leakage_Commutate,
	DSS_Leakage_StartControl,
	DSS_Leakage_WaitControlReady,
	DSS_Leakage_StartOutVoltage,
	DSS_Leakage_WaitOutVoltageReady,
	DSS_Leakage_StopOutVoltage,
	DSS_Leakage_StopControl,
	DSS_Leakage_UnCommutate,
	DSS_Leakage_ReadResult,

	DSS_OnVoltage_StartTest = 200,
	DSS_OnVoltage_Commutate,
	DSS_OnVoltage_WaitCommutation,
	DSS_OnVoltage_StartControl,
	DSS_OnVoltage_WaitControlReady,
	DSS_OnVoltage_PulseCurrent,
	DSS_OnVoltage_WaitCurrentReady,
	DSS_OnVoltage_StopControl,
	DSS_OnVoltage_WaitStopControl,
	DSS_OnVoltage_UnCommutate,
	DSS_OnVoltage_WaitUnCommutate,
	DSS_OnVoltage_ReadResult,

	DSS_Control_StartTest = 300,
	DSS_Control_Commutate,
	DSS_Control_WaitCommutation,
	DSS_Control_StartControl,
	DSS_Control_WaitControlReady,
	DSS_Control_StopControl,
	DSS_Control_WaitStopControl,
	DSS_Control_UnCommutate,
	DSS_Control_WaitUnCommutate,
	DSS_Control_ReadResult,

	DSS_Inhibit_StartTest = 400,
	DSS_Inhibit_ReadResult
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
