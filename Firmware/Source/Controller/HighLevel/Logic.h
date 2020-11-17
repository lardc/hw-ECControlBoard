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

typedef enum __CalibrateNodeIndex
{
	CN_DC1 = 1,
	CN_DC2 = 2,
	CN_DC3 = 3,
	CN_HVDC = 4,
	CN_AC1 = 5,
	CN_AC2 = 6,
	CN_CB = 7
} CalibrateNodeIndex;

typedef enum __CalibrationType
{
	CT_Current = 1,
	CT_Voltage = 2
} CalibrationType;

// Functions
//
void LOGIC_InitEntities();
void LOGIC_HandleStateUpdate();
void LOGIC_HandlePowerOn();
void LOGIC_HandlePowerOff();
void LOGIC_HandleFaultAndStop();

LogicConfigError LOGIC_PrepareMeasurement(bool Calibration);

void LOGIC_HandleMuxExecResult(ExecutionResult Result);
void LOGIC_HandleControlExecResult(ExecutionResult Result);
void LOGIC_HandleLeakageExecResult(ExecutionResult Result);
void LOGIC_HandleCurrentExecResult(ExecutionResult Result);
void LOGIC_HandleCalibrationExecResult(ExecutionResult Result);
void LOGIC_HandlePowerSupplyExecResult(ExecutionResult Result);

// ����� ������
void LOGIC_Wrapper_SafetyMonitor();
void LOGIC_Wrapper_FaultControl();
void LOGIC_Wrapper_Start(DeviceSubState NextState);
void LOGIC_Wrapper_WaitAllNodesReady(DeviceSubState NextState);
void LOGIC_Wrapper_SetStateAfterDelay(DeviceSubState NextState, uint64_t Timeout);

// ������ ����������
void LOGIC_Wrapper_Commutate(DeviceSubState NextState, DeviceSubState StopState, uint16_t *Problem);
void LOGIC_Wrapper_UnCommutate(DeviceSubState NextState);

// ������ ����������
void LOGIC_Wrapper_StartControl(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_IsControlOutputReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_StopControl(DeviceSubState NextState);
void LOGIC_Wrapper_ControlSetDelay(DeviceSubState NextState, DeviceSubState NextStateNoDelay, uint64_t *Timeout);
void LOGIC_Wrapper_IsControlNodeReady(DeviceSubState NextState);
void LOGIC_Wrapper_ControlReadResult(DeviceSubState NextState, pVIPair Result, uint16_t *Problem);
void LOGIC_Wrapper_ControlSaveResult(VIPair Result);

// ������ ������������� ����
void LOGIC_Wrapper_PulseCurrent(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_WaitCurrentFinished(DeviceSubState NextState, uint64_t Timeout);

// ������ ������
void LOGIC_Wrapper_StartLeakage(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_IsLeakageOutputReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_StopLeakage(DeviceSubState NextState);
void LOGIC_Wrapper_IsLeakageNodeReady(DeviceSubState NextState);

// ������ ����������
void LOGIC_Wrapper_StartCalibration(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_IsCalibrationOutputReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_StopCalibration(DeviceSubState NextState);
void LOGIC_Wrapper_CalibrationReadResult(DeviceSubState NextState, pVIPair Result, uint16_t *Problem);
void LOGIC_Wrapper_CalibrationSaveResult(VIPair Result);

// ������ ���������������� �������
void LOGIC_Wrapper_StartPowerSupply(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_IsPowerSupplyOutputReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem);
void LOGIC_Wrapper_StopPowerSupply(DeviceSubState NextState);
void LOGIC_Wrapper_PowerSupply1ReadResult(DeviceSubState NextState, pVIPair Result, uint16_t *Problem);
void LOGIC_Wrapper_PowerSupply2ReadResult(DeviceSubState NextState, pVIPair Result, uint16_t *Problem);
void LOGIC_Wrapper_PowerSupply1SaveResult(VIPair Result);
void LOGIC_Wrapper_PowerSupply2SaveResult(VIPair Result);

#endif // __LOGIC_H
