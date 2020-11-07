// Header
#include "Logic.h"

// Includes
#include "CommonDictionary.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Global.h"
#include "Multiplexer.h"
#include "CurrentBoard.h"
#include "DCVoltageBoard.h"
#include "ACVoltageBoard.h"
#include "DCHighVoltageBoard.h"

// Types
typedef ExecutionResult (*xExecFunction)();
typedef ExecutionResult (*xIsReadyFunction)(bool *Result);
typedef void (*xHandleFaultFunction)(ExecutionResult Result);

// Variables
static volatile MuxObject Multiplexer;
static volatile CurrentBoardObject CurrentBoard;
static volatile DCVoltageBoardObject DCVoltageBoard1, DCVoltageBoard2, DCVoltageBoard3;
static volatile ACVoltageBoardObject ACVoltageBoard1, ACVoltageBoard2;
static volatile DCHVoltageBoardObject DCHighVoltageBoard;
static uint16_t GeneralLogicTimeout, ControlWaitDelay;

static const NodeName ControlDCNode = NAME_DCVoltage1;
static const NodeName ControlACNode = NAME_ACVoltage1;

static const NodeName PowerSupply1Node = NAME_DCVoltage2;
static const NodeName PowerSupply2Node = NAME_DCVoltage3;

static const NodeName LeakageDCNode = NAME_DCHighVoltage;
static const NodeName LeakageACNode = NAME_ACVoltage2;

// Forward functions
void LOGIC_AttachSettings(NodeName Name, void *SettingsPointer);
bool LOGIC_IsNodeInProblem(NodeName Name);
void LOGIC_Wrapper_ExecuteX(DeviceSubState NextState, DeviceSubState StopState, uint64_t *Timeout, uint16_t *Problem,
		xExecFunction MainEventFunc, uint16_t BadConfigProblem, xHandleFaultFunction FaultFunc);
void LOGIC_Wrapper_IsReadyX(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem, xIsReadyFunction MainReadyFunc,
		uint16_t ReadyProblem, uint16_t TimeoutProblem, xHandleFaultFunction FaultFunc);

LogicConfigError LOGIC_CacheMuxSettings();
void LOGIC_CacheCurrentBoardSettings();
void LOGIC_CacheControlSettings(DCV_OutputMode Mode);
void LOGIC_CacheLeakageSettings();

bool LOGIC_IsDCControl();
bool LOGIC_IsDCLeakage();

ExecutionResult LOGIC_StartControl();
ExecutionResult LOGIC_StopControl();
ExecutionResult LOGIC_IsControlVoltageReady(bool *IsReady);

ExecutionResult LOGIC_StartLeakage();
ExecutionResult LOGIC_StopLeakage();
ExecutionResult LOGIC_IsLeakageVoltageReady(bool *IsReady);
bool IsLeakageNodeReady();

bool LOGIC_IsControlInProblem();
bool LOGIC_IsLeakagelInProblem();
bool LOGIC_IsPowerSupply1InProblem();
bool LOGIC_IsPowerSupply2InProblem();

// Functions
void LOGIC_InitEntities()
{
	COMM_InitSlaveArray();

	LOGIC_AttachSettings(NAME_Multiplexer, (pMuxObject)&Multiplexer);
	LOGIC_AttachSettings(NAME_DCCurrent, (pCurrentBoardObject)&CurrentBoard);
	LOGIC_AttachSettings(NAME_DCHighVoltage, (pDCHVoltageBoardObject)&DCHighVoltageBoard);
	LOGIC_AttachSettings(NAME_DCVoltage1, (pDCVoltageBoardObject)&DCVoltageBoard1);
	LOGIC_AttachSettings(NAME_DCVoltage2, (pDCVoltageBoardObject)&DCVoltageBoard2);
	LOGIC_AttachSettings(NAME_DCVoltage3, (pDCVoltageBoardObject)&DCVoltageBoard3);
	LOGIC_AttachSettings(NAME_ACVoltage1, (pACVoltageBoardObject)&ACVoltageBoard1);
	LOGIC_AttachSettings(NAME_ACVoltage2, (pACVoltageBoardObject)&ACVoltageBoard2);
}
//-----------------------------

void LOGIC_AttachSettings(NodeName Name, void *SettingsPointer)
{
	pSlaveNode NodePointer = COMM_GetSlaveDevicePointer(Name);
	NodePointer->Settings = SettingsPointer;
}
//-----------------------------

void LOGIC_HandleStateUpdate()
{
	static Int64U TimeCounter = 0;

	if(CONTROL_State == DS_InProcess && CONTROL_TimeCounter > TimeCounter)
	{
		TimeCounter = CONTROL_TimeCounter + TIME_SLAVE_STATE_UPDATE;
		if(!COMM_SlavesReadState())
			CONTROL_SwitchToFault(ER_InterfaceError, FAULT_EXT_GR_COMMON);
	}
}
//-----------------------------

void LOGIC_HandlePowerOn()
{
	static Int64U TimeCounter = 0;

	if(CONTROL_State == DS_InProcess)
	{
		switch (CONTROL_SubState)
		{
			case DSS_PowerEnable:
				{
					if(COMM_SlavesEnablePower())
					{
						TimeCounter = CONTROL_TimeCounter + DataTable[REG_POWER_ENABLE_TIMEOUT];
						CONTROL_SetDeviceState(DS_InProcess, DSS_PowerWaitReady);
					}
					else
						CONTROL_SwitchToFault(ER_InterfaceError, FAULT_EXT_GR_COMMON);
				}
				break;

			case DSS_PowerWaitReady:
				{
					if(COMM_AreSlavesInStateX(CDS_Ready))
					{
						CONTROL_SetDeviceState(DS_Ready, DSS_None);
					}
					else if(COMM_IsSlaveInFaultOrDisabled())
					{
						CONTROL_SwitchToFault(ER_WrongState, FAULT_EXT_GR_COMMON);
					}
					else if(CONTROL_TimeCounter > TimeCounter)
						CONTROL_SwitchToFault(ER_ChangeStateTimeout, FAULT_EXT_GR_COMMON);
				}
				break;

			default:
				break;
		}
	}
}
//-----------------------------

void LOGIC_HandlePowerOff()
{
	if(CONTROL_State == DS_None && CONTROL_SubState == DSS_PowerOff)
	{
		if(COMM_SlavesDisablePower())
			CONTROL_SetDeviceState(DS_None, DSS_None);
		else
			CONTROL_SwitchToFault(ER_InterfaceError, FAULT_EXT_GR_COMMON);
	}
}
//-----------------------------

void LOGIC_HandleFault()
{
	if(CONTROL_State == DS_InProcess)
	{
		switch(CONTROL_SubState)
		{
			case DSS_Fault_Request:
				CONTROL_SetDeviceState(DS_InProcess, DSS_Fault_StopDCCurrent);
				break;

			case DSS_Fault_StopDCCurrent:
				{
					if(COMM_IsSlaveInStateX(NAME_DCCurrent, CDS_InProcess))
						CURR_Stop();
					CONTROL_SetDeviceState(DS_InProcess, DSS_Fault_StopHV);
				}
				break;

			case DSS_Fault_StopHV:
				{
					if(COMM_IsSlaveInStateX(NAME_DCHighVoltage, CDS_InProcess))
						DCHV_Stop();
					CONTROL_SetDeviceState(DS_InProcess, DSS_Fault_StopDC1);
				}
				break;

			case DSS_Fault_StopDC1:
				{
					if(COMM_IsSlaveInStateX(NAME_DCVoltage1, CDS_InProcess))
						DCV_Stop(NAME_DCVoltage1);
					CONTROL_SetDeviceState(DS_InProcess, DSS_Fault_StopDC2);
				}
				break;

			case DSS_Fault_StopDC2:
				{
					if(COMM_IsSlaveInStateX(NAME_DCVoltage2, CDS_InProcess))
						DCV_Stop(NAME_DCVoltage2);
					CONTROL_SetDeviceState(DS_InProcess, DSS_Fault_StopDC3);
				}
				break;

			case DSS_Fault_StopDC3:
				{
					if(COMM_IsSlaveInStateX(NAME_DCVoltage3, CDS_InProcess))
						DCV_Stop(NAME_DCVoltage3);
					CONTROL_SetDeviceState(DS_InProcess, DSS_Fault_StopAC1);
				}
				break;

			case DSS_Fault_StopAC1:
				{
					if(COMM_IsSlaveInStateX(NAME_ACVoltage1, CDS_InProcess))
						ACV_Stop(NAME_ACVoltage1);
					CONTROL_SetDeviceState(DS_InProcess, DSS_Fault_StopAC2);
				}
				break;

			case DSS_Fault_StopAC2:
				{
					if(COMM_IsSlaveInStateX(NAME_ACVoltage2, CDS_InProcess))
						ACV_Stop(NAME_ACVoltage2);
					CONTROL_SetDeviceState(DS_InProcess, DSS_Fault_StopMux);
				}
				break;

			case DSS_Fault_StopMux:
				{
					MUX_Disconnect();
					CONTROL_SetDeviceState(DS_Fault, DSS_None);
				}
				break;

			default:
				break;
		}
	}
}
//-----------------------------

LogicConfigError LOGIC_PrepareMeasurement()
{
	LogicConfigError err = LOGIC_CacheMuxSettings();

	GeneralLogicTimeout = DataTable[REG_GENERAL_LOGIC_TIMEOUT];
	ControlWaitDelay = DataTable[REG_CTRL_HOLD_DELAY];

	if(err == LCE_None)
	{
		switch(Multiplexer.MeasureType)
		{
			case MT_LeakageCurrent:
				{
					LOGIC_CacheLeakageSettings();
					LOGIC_CacheControlSettings(DCV_Continuous);
					CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_StartTest);
				}
				break;

			case MT_OnVoltage:
				{
					LOGIC_CacheCurrentBoardSettings();
					LOGIC_CacheControlSettings(DCV_Continuous);
					CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_StartTest);
				}
				break;

			case MT_InputVoltageCurrent:
				{
					LOGIC_CacheControlSettings(DCV_Continuous);
					CONTROL_SetDeviceState(DS_InProcess, DSS_Control_StartTest);
				}
				break;

			case MT_InhibitVoltage:
				{
					LOGIC_CacheControlSettings(DCV_Continuous);
					CONTROL_SetDeviceState(DS_InProcess, DSS_Inhibit_StartTest);
				}
				break;

			default:
				return LCE_InvalidMeasurement;
		}
	}
	return err;
}
//-----------------------------

LogicConfigError LOGIC_CacheMuxSettings()
{
	Multiplexer.MeasureType = DataTable[REG_MEASUREMENT_TYPE];
	Multiplexer.Case = DataTable[REG_DUT_CASE_TYPE];
	Multiplexer.Position = DataTable[REG_DUT_POSITION_NUMBER];
	Multiplexer.InputType = DataTable[REG_INPUT_CONTROL_TYPE];
	Multiplexer.LeakageType = DataTable[REG_COMM_VOLTAGE_TYPE_LEAKAGE];
	Multiplexer.Polarity = DataTable[REG_COMM_POLARITY];

	const DL_DUTConfiguration* DUTConfig = DUTLIB_ExtractConfiguration(Multiplexer.Case);

	// Валидация конфигурации
	// Проверка наличия требуемого корпуса
	if(DUTConfig == NULL)
		return LCE_UnknownCase;

	// Проверка соответствия номеров позиций
	if((Multiplexer.Position == MX_Position2 && DUTConfig->OutputPositionsNum == OneOutput) ||
		(Multiplexer.Position == MX_Position3 && DUTConfig->OutputPositionsNum != ThreeOutputs))
		return LCE_PositionMissmatch;

	return LCE_None;
}
//-----------------------------

void LOGIC_CacheCurrentBoardSettings()
{
	CurrentBoard.Setpoint.Current = DT_Read32(REG_COMM_CURRENT, REG_COMM_CURRENT_32);
	CurrentBoard.Setpoint.Voltage = DT_Read32(REG_COMM_VOLTAGE, REG_COMM_VOLTAGE_32);
}
//-----------------------------

void LOGIC_CacheControlSettings(DCV_OutputMode Mode)
{
	VIPair Setpoint;
	Setpoint.Voltage = DT_Read32(REG_CONTROL_VOLTAGE, REG_CONTROL_VOLTAGE_32);
	Setpoint.Current = DT_Read32(REG_CONTROL_CURRENT, REG_CONTROL_CURRENT_32);

	if(LOGIC_IsDCControl())
	{
		DCVoltageBoard1.Setpoint = Setpoint;
		DCVoltageBoard1.OutputLine = DCV_CTRL;
		DCVoltageBoard1.OutputType = (Multiplexer.InputType == IT_ControlVDC) ? DCV_Voltage : DCV_Current;
		DCVoltageBoard1.OutputMode = Mode;
		DCVoltageBoard1.PulseLength = DataTable[REG_CTRL_PULSE_LENGTH];
	}
	else
	{
		ACVoltageBoard1.Setpoint = Setpoint;
		ACVoltageBoard1.OutputLine = ACV_CTRL;
	}
}
//-----------------------------

void LOGIC_CacheLeakageSettings()
{
	VIPair Setpoint;
	Setpoint.Voltage = DT_Read32(REG_COMM_VOLTAGE, REG_COMM_VOLTAGE_32);
	Setpoint.Current = DT_Read32(REG_COMM_CURRENT, REG_COMM_CURRENT_32);

	if(LOGIC_IsDCLeakage())
	{
		DCHighVoltageBoard.Setpoint = Setpoint;
	}
	else
	{
		ACVoltageBoard2.Setpoint = Setpoint;
		ACVoltageBoard2.OutputLine = ACV_BUS_LV;
	}
}
//-----------------------------

bool LOGIC_IsDCControl()
{
	return Multiplexer.InputType == IT_ControlIDC || Multiplexer.InputType == IT_ControlVDC;
}
//-----------------------------

bool LOGIC_IsDCLeakage()
{
	return Multiplexer.LeakageType == LT_LeakageDC;
}
//-----------------------------

void LOGIC_HandleMuxExecResult(ExecutionResult Result)
{
	CONTROL_SwitchToFault(Result, FAULT_EXT_GR_MUX);
}
//-----------------------------

void LOGIC_HandleCurrentExecResult(ExecutionResult Result)
{
	CONTROL_SwitchToFault(Result, FAULT_EXT_GR_DC_CURRENT);
}
//-----------------------------

void LOGIC_HandleControlExecResult(ExecutionResult Result)
{
	CONTROL_SwitchToFault(Result,
		LOGIC_IsDCControl() ? FAULT_EXT_GR_DC_VOLTAGE1 : FAULT_EXT_GR_AC_VOLTAGE1);
}
//-----------------------------

ExecutionResult LOGIC_StartControl()
{
	return LOGIC_IsDCControl() ? DCV_Execute(ControlDCNode) : ACV_Execute(ControlACNode);
}
//-----------------------------

ExecutionResult LOGIC_StopControl()
{
	return LOGIC_IsDCControl() ? DCV_Stop(ControlDCNode) : ACV_Stop(ControlACNode);
}
//-----------------------------

ExecutionResult LOGIC_IsControlVoltageReady(bool *IsReady)
{
	return LOGIC_IsDCControl() ? DCV_IsVoltageReady(ControlDCNode, IsReady) :
		ACV_IsVoltageReady(ControlACNode, IsReady);
}
//-----------------------------

ExecutionResult LOGIC_ControlReadResult(uint16_t *OpResult, pVIPair Result)
{
	ExecutionResult res;
	pSlaveNode NodeData;

	if(LOGIC_IsDCControl())
	{
		NodeData = COMM_GetSlaveDevicePointer(ControlDCNode);
		pDCVoltageBoardObject Settings = (pDCVoltageBoardObject)NodeData->Settings;

		res = DCV_ReadResult(ControlDCNode);
		*Result = Settings->Result;
	}
	else
	{
		NodeData = COMM_GetSlaveDevicePointer(ControlACNode);
		pACVoltageBoardObject Settings = (pACVoltageBoardObject)NodeData->Settings;

		res = ACV_ReadResult(ControlACNode);
		*Result = Settings->Result;
	}

	*OpResult = NodeData->OpResult;
	return res;
}
//-----------------------------

void LOGIC_HandleLeakageExecResult(ExecutionResult Result)
{
	CONTROL_SwitchToFault(Result,
		LOGIC_IsDCLeakage() ? FAULT_EXT_GR_DC_HV : FAULT_EXT_GR_AC_VOLTAGE2);
}
//-----------------------------

ExecutionResult LOGIC_StartLeakage()
{
	return LOGIC_IsDCLeakage() ? DCHV_Execute() : ACV_Execute(LeakageACNode);
}
//-----------------------------

ExecutionResult LOGIC_StopLeakage()
{
	return LOGIC_IsDCLeakage() ? DCHV_Stop() : ACV_Stop(LeakageACNode);
}
//-----------------------------

ExecutionResult LOGIC_IsLeakageVoltageReady(bool *IsReady)
{
	if(LOGIC_IsDCLeakage())
	{
		*IsReady = COMM_IsSlaveInStateX(LeakageDCNode, CDS_Ready);
		return ER_NoError;
	}
	else
		return ACV_IsVoltageReady(LeakageACNode, IsReady);
}
//-----------------------------

ExecutionResult LOGIC_LeakageReadResult(uint16_t *OpResult, pVIPair Result)
{
	ExecutionResult res;
	pSlaveNode NodeData;

	if(LOGIC_IsDCLeakage())
	{
		NodeData = COMM_GetSlaveDevicePointer(LeakageDCNode);
		pDCHVoltageBoardObject Settings = (pDCHVoltageBoardObject)NodeData->Settings;

		res = DCHV_ReadResult();
		*Result = Settings->Result;
	}
	else
	{
		NodeData = COMM_GetSlaveDevicePointer(LeakageACNode);
		pACVoltageBoardObject Settings = (pACVoltageBoardObject)NodeData->Settings;

		res = ACV_ReadResult(LeakageACNode);
		*Result = Settings->Result;
	}

	*OpResult = NodeData->OpResult;
	return res;
}
//-----------------------------

bool IsLeakageNodeReady()
{
	return COMM_IsSlaveInStateX(LOGIC_IsDCLeakage() ? LeakageDCNode : LeakageACNode, CDS_Ready);
}
//-----------------------------

bool LOGIC_IsNodeInProblem(NodeName Name)
{
	return (COMM_GetSlaveOpResult(Name) == COMM_OPRESULT_FAIL)
			&& COMM_IsSlaveInStateX(Name, CDS_Ready);
}
//-----------------------------

bool LOGIC_IsControlInProblem()
{
	return LOGIC_IsNodeInProblem(LOGIC_IsDCControl() ? ControlDCNode : ControlACNode);
}
//-----------------------------

bool LOGIC_IsLeakagelInProblem()
{
	return LOGIC_IsNodeInProblem(LOGIC_IsDCLeakage() ? LeakageDCNode : LeakageACNode);
}
//-----------------------------

bool LOGIC_IsPowerSupply1InProblem()
{
	return LOGIC_IsNodeInProblem(PowerSupply1Node);
}
//-----------------------------

bool LOGIC_IsPowerSupply2InProblem()
{
	return LOGIC_IsNodeInProblem(PowerSupply2Node);
}
//-----------------------------

void LOGIC_Wrapper_FaultControl()
{
	if(COMM_IsSlaveInFaultOrDisabled())
		CONTROL_SwitchToFault(ER_WrongState, FAULT_EXT_GR_COMMON);
}
//-----------------------------

void LOGIC_Wrapper_Start(DeviceSubState NextState)
{
	if(COMM_AreSlavesInStateX(CDS_Ready))
		CONTROL_SetDeviceState(DS_InProcess, NextState);
	else
		CONTROL_SwitchToFault(ER_WrongState, FAULT_EXT_GR_COMMON);
}
//-----------------------------

void LOGIC_Wrapper_ExecuteX(DeviceSubState NextState, DeviceSubState StopState, uint64_t *Timeout, uint16_t *Problem,
		xExecFunction MainEventFunc, uint16_t BadConfigProblem, xHandleFaultFunction FaultFunc)
{
	ExecutionResult res = MainEventFunc();

	switch(res)
	{
		case ER_NoError:
			{
				if(Timeout)
					*Timeout = GeneralLogicTimeout + CONTROL_TimeCounter;
				CONTROL_SetDeviceState(DS_InProcess, NextState);
			}
			break;

		case ER_BadHighLevelConfig:
			{
				*Problem = BadConfigProblem;
				CONTROL_SetDeviceState(DS_InProcess, StopState);
			}
			break;

		default:
			FaultFunc(res);
			break;
	}
}
//-----------------------------

void LOGIC_Wrapper_Commutate(DeviceSubState NextState, DeviceSubState StopState, uint16_t *Problem)
{
	LOGIC_Wrapper_ExecuteX(NextState, StopState, NULL, Problem,
			&MUX_Connect, PROBLEM_MUX_CONFIG, &LOGIC_HandleMuxExecResult);
}
//-----------------------------

void LOGIC_Wrapper_UnCommutate(DeviceSubState NextState)
{
	ExecutionResult res = MUX_Disconnect();
	if(res == ER_NoError)
		CONTROL_SetDeviceState(DS_InProcess, NextState);
	else
		LOGIC_HandleMuxExecResult(res);
}
//-----------------------------

void LOGIC_Wrapper_WaitAllNodesReady(DeviceSubState NextState)
{
	if(COMM_AreSlavesInStateX(CDS_Ready))
		CONTROL_SetDeviceState(DS_InProcess, NextState);
}
//-----------------------------

void LOGIC_Wrapper_SetStateAfterDelay(DeviceSubState NextState, uint64_t Timeout)
{
	if(CONTROL_TimeCounter > Timeout)
		CONTROL_SetDeviceState(DS_InProcess, NextState);
}
//-----------------------------

void LOGIC_Wrapper_StartControl(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem)
{
	LOGIC_Wrapper_ExecuteX(NextState, StopState, Timeout, Problem,
			&LOGIC_StartControl, PROBLEM_CONTROL_CONFIG, &LOGIC_HandleControlExecResult);
}
//-----------------------------

void LOGIC_Wrapper_StopControl(DeviceSubState NextState)
{
	ExecutionResult res = LOGIC_StopControl();
	if(res == ER_NoError)
		CONTROL_SetDeviceState(DS_InProcess, NextState);
	else
		LOGIC_HandleControlExecResult(res);
}
//-----------------------------

void LOGIC_Wrapper_IsReadyX(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem, xIsReadyFunction MainReadyFunc,
		uint16_t ReadyProblem, uint16_t TimeoutProblem, xHandleFaultFunction FaultFunc)
{
	bool IsVoltageReady;
	ExecutionResult res = MainReadyFunc(&IsVoltageReady);

	if(res == ER_NoError)
	{
		if(IsVoltageReady)
			CONTROL_SetDeviceState(DS_InProcess, NextState);
		else if(LOGIC_IsControlInProblem())
		{
			*Problem = ReadyProblem;
			CONTROL_SetDeviceState(DS_InProcess, StopState);
		}
		else if(Timeout)
		{
			if(CONTROL_TimeCounter > *Timeout)
			{
				*Problem = TimeoutProblem;
				CONTROL_SetDeviceState(DS_InProcess, StopState);
			}
		}
	}
	else
		FaultFunc(res);
}
//-----------------------------

void LOGIC_Wrapper_IsControlReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem)
{
	LOGIC_Wrapper_IsReadyX(NextState, StopState, Timeout, Problem, &LOGIC_IsControlVoltageReady,
			PROBLEM_CONTROL_IN_PROBLEM, PROBLEM_CONTROL_READY_TIMEOUT, &LOGIC_HandleControlExecResult);
}
//-----------------------------

void LOGIC_Wrapper_ControlSetDelay(DeviceSubState NextState, DeviceSubState NextStateNoDelay, uint64_t *Timeout)
{
	if(ControlWaitDelay)
	{
		*Timeout = CONTROL_TimeCounter + ControlWaitDelay;
		CONTROL_SetDeviceState(DS_InProcess, NextState);
	}
	else
		CONTROL_SetDeviceState(DS_InProcess, NextStateNoDelay);
}
//-----------------------------

void LOGIC_Wrapper_PulseCurrent(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem)
{
	LOGIC_Wrapper_ExecuteX(NextState, StopState, Timeout, Problem,
			&CURR_Execute, PROBLEM_CURRENT_CONFIG, &LOGIC_HandleCurrentExecResult);
}
//-----------------------------

void LOGIC_Wrapper_WaitCurrentFinished(DeviceSubState NextState, uint64_t Timeout)
{
	if(COMM_IsSlaveInStateX(NAME_DCCurrent, CDS_Ready))
		CONTROL_SetDeviceState(DS_InProcess, NextState);
	else if(CONTROL_TimeCounter > Timeout)
		LOGIC_HandleCurrentExecResult(ER_ChangeStateTimeout);
}
//-----------------------------

void LOGIC_Wrapper_StartLeakage(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem)
{
	LOGIC_Wrapper_ExecuteX(NextState, StopState, Timeout, Problem,
			&LOGIC_StartLeakage, PROBLEM_LEAKAGE_CONFIG, &LOGIC_HandleLeakageExecResult);
}
//-----------------------------

void LOGIC_Wrapper_IsLeakageReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem)
{
	LOGIC_Wrapper_IsReadyX(NextState, StopState, Timeout, Problem, &LOGIC_IsLeakageVoltageReady,
			PROBLEM_LEAKAGE_IN_PROBLEM, PROBLEM_LEAKAGE_READY_TIMEOUT, &LOGIC_HandleLeakageExecResult);
}
//-----------------------------

void LOGIC_Wrapper_StopLeakage(DeviceSubState NextState)
{
	ExecutionResult res = LOGIC_StopLeakage();
	if(res == ER_NoError)
		CONTROL_SetDeviceState(DS_InProcess, NextState);
	else
		LOGIC_HandleLeakageExecResult(res);
}
//-----------------------------

void LOGIC_Wrapper_WaitLeakageFinished(DeviceSubState NextState, uint64_t Timeout)
{
	if(IsLeakageNodeReady())
		CONTROL_SetDeviceState(DS_InProcess, NextState);
	else if(CONTROL_TimeCounter > Timeout)
		LOGIC_HandleLeakageExecResult(ER_ChangeStateTimeout);
}
//-----------------------------
