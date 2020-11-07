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

LogicConfigError LOGIC_PrepareMeasurement();

bool LOGIC_IsDCControl();
bool LOGIC_IsDCLeakage();

void LOGIC_HandleControlExecResult(ExecutionResult Result);
ExecutionResult LOGIC_StartControl();
ExecutionResult LOGIC_StopControl();
ExecutionResult LOGIC_IsControlVoltageReady(bool *IsReady);
ExecutionResult LOGIC_ControlReadResult(uint16_t *OpResult, pVIPair Result);

void LOGIC_HandleLeakageExecResult(ExecutionResult Result);
ExecutionResult LOGIC_StartLeakage();
ExecutionResult LOGIC_StopLeakage();
ExecutionResult LOGIC_IsLeakageVoltageReady(bool *IsReady);
ExecutionResult LOGIC_LeakageReadResult(uint16_t *OpResult, pVIPair Result);
bool IsLeakageNodeReady();

bool LOGIC_IsControlInProblem();
bool LOGIC_IsLeakagelInProblem();
bool LOGIC_IsPowerSupply1InProblem();
bool LOGIC_IsPowerSupply2InProblem();

// Общие обёртки
void LOGIC_Wrapper_FaultControl();
void LOGIC_Wrapper_Start(DeviceSubState NextState);
void LOGIC_Wrapper_WaitAllNodesReady(DeviceSubState NextState);

// Обёртки коммутации
void LOGIC_Wrapper_Commutate(DeviceSubState NextState);
void LOGIC_Wrapper_UnCommutate(DeviceSubState NextState);

// Обёртки управления
void LOGIC_Wrapper_StartControl(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_IsControlReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_StopControl(DeviceSubState NextState);

// Обёртки формирователя тока
void LOGIC_Wrapper_PulseCurrent(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_WaitCurrentReady(DeviceSubState NextState, uint64_t Timeout);

#endif // __LOGIC_H
