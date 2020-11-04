#ifndef __LOGIC_H
#define __LOGIC_H

// Include
#include "stdinc.h"
#include "Common.h"
#include "BCCIMHighLevel.h"

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

bool LOGIC_IsControlInProblem();
bool LOGIC_IsLeakagelInProblem();
bool LOGIC_IsPowerSupply1InProblem();
bool LOGIC_IsPowerSupply2InProblem();

void LOGIC_HandleMeasurementOnState();

LogicConfigError LOGIC_PrepareMeasurement();

#endif // __LOGIC_H
