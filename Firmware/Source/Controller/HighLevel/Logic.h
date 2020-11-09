#ifndef __LOGIC_H
#define __LOGIC_H

// Include
#include "stdinc.h"
#include "Common.h"
#include "BCCIMHighLevel.h"
#include "Controller.h"

// Types
typedef enum __LogicConfigError
{
	LCE_None = 0,
	LCE_UnknownCase = 1,
	LCE_PositionMissmatch = 2,
	LCE_InvalidMeasurement = 3
} LogicConfigError;

// Functions
//
void LOGIC_InitEntities();
void LOGIC_HandleStateUpdate();
void LOGIC_HandlePowerOn();
void LOGIC_HandlePowerOff();
void LOGIC_HandleFault();

LogicConfigError LOGIC_PrepareMeasurement(pMeasurementType OverrideMeasurement);

ExecutionResult LOGIC_ControlReadResult(uint16_t *OpResult, pVIPair Result);
ExecutionResult LOGIC_LeakageReadResult(uint16_t *OpResult, pVIPair Result);

void LOGIC_HandleMuxExecResult(ExecutionResult Result);
void LOGIC_HandleControlExecResult(ExecutionResult Result);
void LOGIC_HandleLeakageExecResult(ExecutionResult Result);
void LOGIC_HandleCurrentExecResult(ExecutionResult Result);
void LOGIC_HandleCalibrationExecResult(ExecutionResult Result);

// Общие обёртки
void LOGIC_Wrapper_FaultControl();
void LOGIC_Wrapper_Start(DeviceSubState NextState);
void LOGIC_Wrapper_WaitAllNodesReady(DeviceSubState NextState);
void LOGIC_Wrapper_SetStateAfterDelay(DeviceSubState NextState, uint64_t Timeout);

// Обёртки коммутации
void LOGIC_Wrapper_Commutate(DeviceSubState NextState, DeviceSubState StopState, uint16_t *Problem);
void LOGIC_Wrapper_UnCommutate(DeviceSubState NextState);

// Обёртки управления
void LOGIC_Wrapper_StartControl(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_IsControlReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_StopControl(DeviceSubState NextState);
void LOGIC_Wrapper_ControlSetDelay(DeviceSubState NextState, DeviceSubState NextStateNoDelay, uint64_t *Timeout);

// Обёртки формирователя тока
void LOGIC_Wrapper_PulseCurrent(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_WaitCurrentFinished(DeviceSubState NextState, uint64_t Timeout);

// Обёртки утечки
void LOGIC_Wrapper_StartLeakage(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_IsLeakageReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_StopLeakage(DeviceSubState NextState);
void LOGIC_Wrapper_WaitLeakageFinished(DeviceSubState NextState, uint64_t Timeout);

// Обёртки калибровки
void LOGIC_Wrapper_StartCalibration(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_IsCalibrationReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_StopCalibration(DeviceSubState NextState);

#endif // __LOGIC_H
