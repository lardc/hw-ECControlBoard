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
typedef bool (*xIsProblemFunction)();
typedef ExecutionResult (*xReadResultFunction)(uint16_t *OpResult, pVIPair Result);

// Variables
static volatile MuxObject Multiplexer;
static volatile CurrentBoardObject CurrentBoard;
static volatile DCVoltageBoardObject DCVoltageBoard1, DCVoltageBoard2, DCVoltageBoard3;
static volatile ACVoltageBoardObject ACVoltageBoard1, ACVoltageBoard2;
static volatile DCHVoltageBoardObject DCHighVoltageBoard;
static uint16_t GeneralLogicTimeout, ControlWaitDelay;
static CalibrateNodeIndex CachedNode;
static DL_AuxPowerSupply CachedPowerSupply;

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
void LOGIC_Wrapper_TerminateX(DeviceSubState NextState, xExecFunction TerminateFunc, xHandleFaultFunction FaultFunc);
void LOGIC_Wrapper_IsOutputReadyX(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem, xIsReadyFunction MainReadyFunc, xIsProblemFunction IsInProblemFunc,
		uint16_t ReadyProblem, uint16_t TimeoutProblem, xHandleFaultFunction FaultFunc);
void LOGIC_Wrapper_ReadResultX(DeviceSubState NextState, uint16_t *Problem, uint16_t ProblemCode,
		xIsProblemFunction IsInProblemFunc, xReadResultFunction ResultFunc, pVIPair Result, xHandleFaultFunction FaultFunc);

LogicConfigError LOGIC_CacheMuxSettings(bool Calibration, pDL_AuxPowerSupply PowerSupply);
void LOGIC_CacheCurrentBoardSettings();
void LOGIC_CacheControlSettings(DCV_OutputMode Mode);
void LOGIC_CacheLeakageSettings();
void LOGIC_CachePowerSupplySettings(DL_AuxPowerSupply Mode);
void LOGIC_CacheCalibrationSettings();

bool LOGIC_IsDCControl();
bool LOGIC_IsDCLeakage();

ExecutionResult LOGIC_StartControl();
ExecutionResult LOGIC_StopControl();
ExecutionResult LOGIC_IsControlVoltageReady(bool *IsReady);
bool LOGIC_IsControlNodeReady();

ExecutionResult LOGIC_StartLeakage();
ExecutionResult LOGIC_StopLeakage();
ExecutionResult LOGIC_IsLeakageVoltageReady(bool *IsReady);
bool LOGIC_IsLeakageNodeReady();

ExecutionResult LOGIC_StartPowerSupply();
ExecutionResult LOGIC_IsPowerSupplyReady(bool *IsReady);
ExecutionResult LOGIC_StopPowerSupply();

ExecutionResult LOGIC_StartCalibration();
ExecutionResult LOGIC_StopCalibration();

bool LOGIC_IsControlInProblem();
bool LOGIC_IsCurrentInProblem();
bool LOGIC_IsLeakagelInProblem();
bool LOGIC_IsCalibrationInProblem();
bool LOGIC_IsPowerSupplyInProblem();

ExecutionResult LOGIC_ControlReadResult(uint16_t *OpResult, pVIPair Result);
ExecutionResult LOGIC_CurrentReadResult(uint16_t *OpResult, pVIPair Result);
ExecutionResult LOGIC_LeakageReadResult(uint16_t *OpResult, pVIPair Result);
ExecutionResult LOGIC_CalibrationReadResult(uint16_t *OpResult, pVIPair Result);
ExecutionResult LOGIC_PowerSupply1ReadResult(uint16_t *OpResult, pVIPair Result);
ExecutionResult LOGIC_PowerSupply2ReadResult(uint16_t *OpResult, pVIPair Result);

void LOGIC_HandleMuxExecResult(ExecutionResult Result);
void LOGIC_HandleControlExecResult(ExecutionResult Result);
void LOGIC_HandleLeakageExecResult(ExecutionResult Result);
void LOGIC_HandleCurrentExecResult(ExecutionResult Result);
void LOGIC_HandleCalibrationExecResult(ExecutionResult Result);
void LOGIC_HandlePowerSupplyExecResult(ExecutionResult Result);

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

void LOGIC_HandleFaultAndStop()
{
	static DeviceSubState SavedRequest = DSS_None;

	if(CONTROL_State == DS_InProcess)
	{
		switch(CONTROL_SubState)
		{
			case DSS_Fault_Request:
			case DSS_Stop_Request:
				{
					SavedRequest = CONTROL_SubState;
					CONTROL_SetDeviceState(DS_InProcess, DSS_FaultStop_StopDCCurrent);
				}
				break;

			case DSS_FaultStop_StopDCCurrent:
				{
					if(COMM_IsSlaveInStateX(NAME_DCCurrent, CDS_InProcess))
						CURR_Stop();
					CONTROL_SetDeviceState(DS_InProcess, DSS_FaultStop_StopHV);
				}
				break;

			case DSS_FaultStop_StopHV:
				{
					if(COMM_IsSlaveInStateX(NAME_DCHighVoltage, CDS_InProcess))
						DCHV_Stop();
					CONTROL_SetDeviceState(DS_InProcess, DSS_FaultStop_StopDC1);
				}
				break;

			case DSS_FaultStop_StopDC1:
				{
					if(COMM_IsSlaveInStateX(NAME_DCVoltage1, CDS_InProcess))
						DCV_Stop(NAME_DCVoltage1);
					CONTROL_SetDeviceState(DS_InProcess, DSS_FaultStop_StopDC2);
				}
				break;

			case DSS_FaultStop_StopDC2:
				{
					if(COMM_IsSlaveInStateX(NAME_DCVoltage2, CDS_InProcess))
						DCV_Stop(NAME_DCVoltage2);
					CONTROL_SetDeviceState(DS_InProcess, DSS_FaultStop_StopDC3);
				}
				break;

			case DSS_FaultStop_StopDC3:
				{
					if(COMM_IsSlaveInStateX(NAME_DCVoltage3, CDS_InProcess))
						DCV_Stop(NAME_DCVoltage3);
					CONTROL_SetDeviceState(DS_InProcess, DSS_FaultStop_StopAC1);
				}
				break;

			case DSS_FaultStop_StopAC1:
				{
					if(COMM_IsSlaveInStateX(NAME_ACVoltage1, CDS_InProcess))
						ACV_Stop(NAME_ACVoltage1);
					CONTROL_SetDeviceState(DS_InProcess, DSS_FaultStop_StopAC2);
				}
				break;

			case DSS_FaultStop_StopAC2:
				{
					if(COMM_IsSlaveInStateX(NAME_ACVoltage2, CDS_InProcess))
						ACV_Stop(NAME_ACVoltage2);
					CONTROL_SetDeviceState(DS_InProcess, DSS_FaultStop_StopMux);
				}
				break;

			case DSS_FaultStop_StopMux:
				{
					MUX_Disconnect();

					if(SavedRequest == DSS_Fault_Request)
						CONTROL_SetDeviceState(DS_Fault, DSS_None);
					else
						CONTROL_SetDeviceState(DS_Ready, DSS_None);
				}
				break;

			default:
				break;
		}
	}
}
//-----------------------------

LogicConfigError LOGIC_PrepareMeasurement(bool Calibration)
{
	LogicConfigError err = LOGIC_CacheMuxSettings(Calibration, &CachedPowerSupply);

	if(err == LCE_None)
	{
		GeneralLogicTimeout = DataTable[REG_GENERAL_LOGIC_TIMEOUT];
		ControlWaitDelay = DataTable[REG_CTRL_HOLD_DELAY];

		if(Calibration)
			CachedNode = DataTable[REG_CALIBRATION_NODE];

		LOGIC_CachePowerSupplySettings(CachedPowerSupply);

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

			case MT_CalibrateDCControl:
			case MT_CalibratePS1:
			case MT_CalibratePS2:
			case MT_CalibrateDCLeakage:
			case MT_CalibrateACLeakage:
			case MT_CalibrateACControl:
			case MT_CalibrateCurrent:
				{
					LOGIC_CacheCalibrationSettings();
					CONTROL_SetDeviceState(DS_InProcess, DSS_Calibrate_Start);
				}
				break;

			default:
				return LCE_InvalidMeasurement;
		}
	}

	return err;
}
//-----------------------------

void LOGIC_CacheCalibrationSettings()
{
	VIPair Setpoint;
	Setpoint.Voltage = DT_Read32(REG_CALIBRATION_VSET, REG_CALIBRATION_VSET_32);
	Setpoint.Current = DT_Read32(REG_CALIBRATION_ISET, REG_CALIBRATION_ISET_32);

	switch(CachedNode)
	{
		case CN_DC1:
			{
				DCVoltageBoard1.Setpoint = Setpoint;
				DCVoltageBoard1.OutputLine = DCV_CTRL;
				DCVoltageBoard1.OutputType = DataTable[REG_CALIBRATION_TYPE];
				DCVoltageBoard1.OutputMode = DCV_Continuous;
				DCVoltageBoard1.PulseLength = DataTable[REG_CTRL_PULSE_LENGTH];
			}
			break;

		case CN_DC2:
			{
				DCVoltageBoard2.Setpoint = Setpoint;
				DCVoltageBoard2.OutputLine = DCV_PS1;
				DCVoltageBoard2.OutputType = DCV_Voltage;
				DCVoltageBoard2.OutputMode = DCV_Continuous;
				DCVoltageBoard2.PulseLength = DataTable[REG_CTRL_PULSE_LENGTH];
			}
			break;

		case CN_DC3:
			{
				DCVoltageBoard3.Setpoint = Setpoint;
				DCVoltageBoard3.OutputLine = DCV_PS2;
				DCVoltageBoard3.OutputType = DCV_Voltage;
				DCVoltageBoard3.OutputMode = DCV_Continuous;
				DCVoltageBoard3.PulseLength = DataTable[REG_CTRL_PULSE_LENGTH];
			}
			break;

		case CN_HVDC:
			{
				DCHighVoltageBoard.Setpoint = Setpoint;
			}
			break;

		case CN_AC1:
			{
				ACVoltageBoard1.Setpoint = Setpoint;
				ACVoltageBoard1.OutputLine = ACV_CTRL;
			}
			break;

		case CN_AC2:
			{
				ACVoltageBoard2.Setpoint = Setpoint;
				ACVoltageBoard2.OutputLine = ACV_BUS_LV;
			}
			break;

		case CN_CB:
			{
				CurrentBoard.Setpoint = Setpoint;
			}
			break;

		default:
			break;
	}
}
//-----------------------------

LogicConfigError LOGIC_CacheMuxSettings(bool Calibration, pDL_AuxPowerSupply PowerSupply)
{
	*PowerSupply = NoSupply;

	if(Calibration)
	{
		switch(CachedNode)
		{
			case CN_DC1:
				Multiplexer.MeasureType = MT_CalibrateDCControl;
				break;

			case CN_DC2:
				Multiplexer.MeasureType = MT_CalibratePS1;
				break;

			case CN_DC3:
				Multiplexer.MeasureType = MT_CalibratePS2;
				break;

			case CN_HVDC:
				Multiplexer.MeasureType = MT_CalibrateDCLeakage;
				break;

			case CN_AC1:
				Multiplexer.MeasureType = MT_CalibrateACControl;
				break;

			case CN_AC2:
				Multiplexer.MeasureType = MT_CalibrateACLeakage;
				break;

			case CN_CB:
				Multiplexer.MeasureType = MT_CalibrateCurrent;
				break;

			default:
				break;
		}
	}
	else
		Multiplexer.MeasureType = DataTable[REG_MEASUREMENT_TYPE];

	Multiplexer.Case = DataTable[REG_DUT_CASE_TYPE];
	Multiplexer.Position = DataTable[REG_DUT_POSITION_NUMBER];
	Multiplexer.InputType = DataTable[REG_INPUT_CONTROL_TYPE];
	Multiplexer.LeakageType = DataTable[REG_COMM_VOLTAGE_TYPE_LEAKAGE];
	Multiplexer.Polarity = DataTable[REG_COMM_POLARITY];
	Multiplexer.SafetyMute = DataTable[REG_DIAG_SAFETY_MUTE];

	// ��������� ������������
	if(!Calibration)
	{
		const DL_DUTConfiguration* DUTConfig = DUTLIB_ExtractConfiguration(Multiplexer.Case);

		// �������� ������� ���������� �������
		if(DUTConfig == NULL)
			return LCE_UnknownCase;

		// �������� ������������ ������� �������
		if((Multiplexer.Position == MX_Position2 && DUTConfig->OutputPositionsNum == OneOutput) ||
			(Multiplexer.Position == MX_Position3 && DUTConfig->OutputPositionsNum != ThreeOutputs))
			return LCE_PositionMissmatch;

		// ��������� �������� ���������������� �������
		*PowerSupply = DUTConfig->AuxPowerSupply;
	}

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

void LOGIC_CachePowerSupplySettings(DL_AuxPowerSupply Mode)
{
	if(Mode == SingleDCSupply || Mode == DoubleDCSupply)
	{
		VIPair SetpointPS1;
		SetpointPS1.Voltage = (uint32_t)DataTable[REG_AUX_PS1_VOLTAGE] * 1000;
		SetpointPS1.Current = DataTable[REG_AUX_PS1_CURRENT];

		DCVoltageBoard2.Setpoint = SetpointPS1;
		DCVoltageBoard2.OutputLine = DCV_PS1;
		DCVoltageBoard2.OutputType = DCV_Voltage;
		DCVoltageBoard2.OutputMode = DCV_Continuous;
		DCVoltageBoard2.PulseLength = DataTable[REG_CTRL_PULSE_LENGTH];

		if(Mode == DoubleDCSupply)
		{
			VIPair SetpointPS2;
			SetpointPS2.Voltage = (uint32_t)DataTable[REG_AUX_PS2_VOLTAGE] * 1000;
			SetpointPS2.Current = DataTable[REG_AUX_PS2_CURRENT];

			DCVoltageBoard3.Setpoint = SetpointPS2;
			DCVoltageBoard3.OutputLine = DCV_PS2;
			DCVoltageBoard3.OutputType = DCV_Voltage;
			DCVoltageBoard3.OutputMode = DCV_Continuous;
			DCVoltageBoard3.PulseLength = DataTable[REG_CTRL_PULSE_LENGTH];
		}
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

void LOGIC_HandlePowerSupplyExecResult(ExecutionResult Result)
{
	CONTROL_SwitchToFault(Result, FAULT_EXT_GR_POWER_SUPPLY);
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

bool LOGIC_IsControlNodeReady()
{
	return COMM_IsSlaveInStateX(LOGIC_IsDCControl() ? ControlDCNode : ControlACNode, CDS_Ready);
}
//-----------------------------

ExecutionResult LOGIC_CurrentReadResult(uint16_t *OpResult, pVIPair Result)
{
	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_DCCurrent);
	pCurrentBoardObject Settings = (pCurrentBoardObject)NodeData->Settings;

	ExecutionResult res = CURR_ReadResult();
	*Result = Settings->Result;
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

bool LOGIC_IsLeakageNodeReady()
{
	return COMM_IsSlaveInStateX(LOGIC_IsDCLeakage() ? LeakageDCNode : LeakageACNode, CDS_Ready);
}
//-----------------------------

ExecutionResult LOGIC_StartCalibration()
{
	switch(CachedNode)
	{
		case CN_DC1:
			return DCV_Execute(NAME_DCVoltage1);

		case CN_DC2:
			return DCV_Execute(NAME_DCVoltage2);

		case CN_DC3:
			return DCV_Execute(NAME_DCVoltage3);

		case CN_HVDC:
			return DCHV_Execute();

		case CN_AC1:
			return ACV_Execute(NAME_ACVoltage1);

		case CN_AC2:
			return ACV_Execute(NAME_ACVoltage2);

		case CN_CB:
			return CURR_Execute();

		default:
			return ER_WrongNode;
	}
}
//-----------------------------

ExecutionResult LOGIC_StopCalibration()
{
	switch(CachedNode)
	{
		case CN_DC1:
			return DCV_Stop(NAME_DCVoltage1);

		case CN_DC2:
			return DCV_Stop(NAME_DCVoltage2);

		case CN_DC3:
			return DCV_Stop(NAME_DCVoltage3);

		case CN_HVDC:
			return DCHV_Stop();

		case CN_AC1:
			return ACV_Stop(NAME_ACVoltage1);

		case CN_AC2:
			return ACV_Stop(NAME_ACVoltage2);

		case CN_CB:
			return CURR_Stop();

		default:
			return ER_WrongNode;
	}
}
//-----------------------------

void LOGIC_HandleCalibrationExecResult(ExecutionResult Result)
{
	uint16_t Group = FAULT_EXT_GR_COMMON;

	switch(CachedNode)
	{
		case CN_DC1:
			Group = FAULT_EXT_GR_DC_VOLTAGE1;
			break;

		case CN_DC2:
			Group = FAULT_EXT_GR_DC_VOLTAGE2;
			break;

		case CN_DC3:
			Group = FAULT_EXT_GR_DC_VOLTAGE3;
			break;

		case CN_HVDC:
			Group = FAULT_EXT_GR_DC_HV;
			break;

		case CN_AC1:
			Group = FAULT_EXT_GR_AC_VOLTAGE1;
			break;

		case CN_AC2:
			Group = FAULT_EXT_GR_AC_VOLTAGE2;
			break;

		case CN_CB:
			Group = FAULT_EXT_GR_DC_CURRENT;
			break;

		default:
			break;
	}

	CONTROL_SwitchToFault(Result, Group);
}
//-----------------------------

ExecutionResult LOGIC_IsCalibrationReady(bool *IsReady)
{
	switch(CachedNode)
	{
		case CN_DC1:
			return DCV_IsVoltageReady(NAME_DCVoltage1, IsReady);

		case CN_DC2:
			return DCV_IsVoltageReady(NAME_DCVoltage2, IsReady);

		case CN_DC3:
			return DCV_IsVoltageReady(NAME_DCVoltage3, IsReady);

		case CN_HVDC:
			 *IsReady = COMM_IsSlaveInStateX(NAME_DCHighVoltage, CDS_Ready);
			return ER_NoError;

		case CN_AC1:
			return ACV_IsVoltageReady(NAME_ACVoltage1, IsReady);

		case CN_AC2:
			return ACV_IsVoltageReady(NAME_ACVoltage2, IsReady);

		case CN_CB:
			 *IsReady = COMM_IsSlaveInStateX(NAME_DCCurrent, CDS_Ready);
			return ER_NoError;

		default:
			return ER_WrongNode;
	}
}
//-----------------------------

ExecutionResult LOGIC_CalibrationReadResult(uint16_t *OpResult, pVIPair Result)
{
	if(CachedNode == CN_DC1 || CachedNode == CN_DC2 || CachedNode == CN_DC3)
	{
		NodeName Name = NAME_DCVoltage1;

		if(CachedNode == CN_DC2)
			Name = NAME_DCVoltage2;
		else if(CachedNode == CN_DC3)
			Name = NAME_DCVoltage3;

		pSlaveNode NodeData = COMM_GetSlaveDevicePointer(Name);
		pDCVoltageBoardObject Settings = (pDCVoltageBoardObject)NodeData->Settings;

		ExecutionResult res = DCV_ReadResult(Name);
		*Result = Settings->Result;
		*OpResult = COMM_GetSlaveOpResult(Name);

		return res;
	}
	else if(CachedNode == CN_AC1 || CachedNode == CN_AC2)
	{
		NodeName Name = NAME_ACVoltage1;

		if(CachedNode == CN_AC2)
			Name = NAME_ACVoltage2;

		pSlaveNode NodeData = COMM_GetSlaveDevicePointer(Name);
		pACVoltageBoardObject Settings = (pACVoltageBoardObject)NodeData->Settings;

		ExecutionResult res = ACV_ReadResult(Name);
		*Result = Settings->Result;
		*OpResult = COMM_GetSlaveOpResult(Name);

		return res;
	}
	else if(CachedNode == CN_HVDC)
	{
		pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_DCHighVoltage);
		pDCHVoltageBoardObject Settings = (pDCHVoltageBoardObject)NodeData->Settings;

		ExecutionResult res = DCHV_ReadResult();
		*Result = Settings->Result;
		*OpResult = COMM_GetSlaveOpResult(NAME_DCHighVoltage);

		return res;
	}
	else if(CachedNode == CN_CB)
	{
		pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_DCCurrent);
		pCurrentBoardObject Settings = (pCurrentBoardObject)NodeData->Settings;

		ExecutionResult res = CURR_ReadResult();
		*Result = Settings->Result;
		*OpResult = COMM_GetSlaveOpResult(NAME_DCCurrent);

		return res;
	}

	return ER_WrongNode;
}
//-----------------------------

ExecutionResult LOGIC_StartPowerSupply()
{
	switch(CachedPowerSupply)
	{
		case NoSupply:
			return ER_NoError;

		case SingleDCSupply:
			return DCV_Execute(PowerSupply1Node);

		case DoubleDCSupply:
			{
				ExecutionResult res = DCV_Execute(PowerSupply1Node);
				if(res == ER_NoError)
					return DCV_Execute(PowerSupply2Node);
				else
					return res;
			}

		default:
			return ER_BadHighLevelConfig;
	}
}
//-----------------------------

ExecutionResult LOGIC_IsPowerSupplyReady(bool *IsReady)
{
	switch(CachedPowerSupply)
	{
		case NoSupply:
			*IsReady = true;
			return ER_NoError;

		case SingleDCSupply:
			return DCV_IsVoltageReady(PowerSupply1Node, IsReady);

		case DoubleDCSupply:
			{
				bool Ready1, Ready2;
				ExecutionResult res = DCV_IsVoltageReady(PowerSupply1Node, &Ready1);
				if(res == ER_NoError)
				{
					res = DCV_IsVoltageReady(PowerSupply2Node, &Ready2);
					*IsReady = Ready1 & Ready2;
					return res;
				}
				else
					return res;
			}

		default:
			return ER_BadHighLevelConfig;
	}
}
//-----------------------------

ExecutionResult LOGIC_StopPowerSupply()
{
	switch(CachedPowerSupply)
	{
		case NoSupply:
			return ER_NoError;

		case SingleDCSupply:
			return DCV_Stop(NAME_DCVoltage2);

		case DoubleDCSupply:
			{
				ExecutionResult res = DCV_Stop(PowerSupply1Node);
				if(res == ER_NoError)
					return DCV_Stop(PowerSupply2Node);
				else
					return res;
			}

		default:
			return ER_BadHighLevelConfig;
	}
}
//-----------------------------

ExecutionResult LOGIC_PowerSupply1ReadResult(uint16_t *OpResult, pVIPair Result)
{
	switch(CachedPowerSupply)
	{
		case NoSupply:
			*OpResult = COMM_OPRESULT_OK;
			return ER_NoError;

		case SingleDCSupply:
		case DoubleDCSupply:
			{
				pSlaveNode NodeData = COMM_GetSlaveDevicePointer(PowerSupply1Node);
				pDCVoltageBoardObject Settings = (pDCVoltageBoardObject)NodeData->Settings;

				ExecutionResult res = DCV_ReadResult(PowerSupply1Node);
				*Result = Settings->Result;
				*OpResult = NodeData->OpResult;

				return res;
			}

		default:
			return ER_BadHighLevelConfig;
	}
}
//-----------------------------

ExecutionResult LOGIC_PowerSupply2ReadResult(uint16_t *OpResult, pVIPair Result)
{
	switch(CachedPowerSupply)
	{
		case NoSupply:
		case SingleDCSupply:
			*OpResult = COMM_OPRESULT_OK;
			return ER_NoError;

		case DoubleDCSupply:
			{
				pSlaveNode NodeData = COMM_GetSlaveDevicePointer(PowerSupply2Node);
				pDCVoltageBoardObject Settings = (pDCVoltageBoardObject)NodeData->Settings;

				ExecutionResult res = DCV_ReadResult(PowerSupply2Node);
				*Result = Settings->Result;
				*OpResult = NodeData->OpResult;

				return res;
			}

		default:
			return ER_BadHighLevelConfig;
	}
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

bool LOGIC_IsCurrentInProblem()
{
	return LOGIC_IsNodeInProblem(NAME_DCCurrent);
}
//-----------------------------

bool LOGIC_IsLeakagelInProblem()
{
	return LOGIC_IsNodeInProblem(LOGIC_IsDCLeakage() ? LeakageDCNode : LeakageACNode);
}
//-----------------------------

bool LOGIC_IsCalibrationInProblem()
{
	switch(CachedNode)
	{
		case CN_DC1:
			return LOGIC_IsNodeInProblem(NAME_DCVoltage1);

		case CN_DC2:
			return LOGIC_IsNodeInProblem(NAME_DCVoltage2);

		case CN_DC3:
			return LOGIC_IsNodeInProblem(NAME_DCVoltage3);

		case CN_HVDC:
			return LOGIC_IsNodeInProblem(NAME_DCHighVoltage);

		case CN_AC1:
			return LOGIC_IsNodeInProblem(NAME_ACVoltage1);

		case CN_AC2:
			return LOGIC_IsNodeInProblem(NAME_ACVoltage2);

		case CN_CB:
			return LOGIC_IsNodeInProblem(NAME_DCCurrent);

		default:
			return ER_WrongNode;
	}
}
//-----------------------------

bool LOGIC_IsPowerSupplyInProblem()
{
	switch(CachedPowerSupply)
	{
		case NoSupply:
			return false;

		case SingleDCSupply:
			return LOGIC_IsNodeInProblem(PowerSupply1Node);

		case DoubleDCSupply:
			{
				bool ProblemPS1 = LOGIC_IsNodeInProblem(PowerSupply1Node);
				bool ProblemPS2 = LOGIC_IsNodeInProblem(PowerSupply2Node);
				return (ProblemPS1 || ProblemPS2);
			}
	}

	return true;
}
//-----------------------------

void LOGIC_Wrapper_SafetyMonitor()
{
	if(COMM_IsSlaveInStateX(NAME_Multiplexer, MUX_STATE_SAFETY_TRIG))
	{
		if(CONTROL_SubState >= DSS_InterruptableStatesBegin)
		{
			CONTROL_SetDeviceState(DS_InProcess, DSS_Stop_Request);

			DataTable[REG_PROBLEM] = PROBLEM_SAFETY_TRIG;
			DataTable[REG_OP_RESULT] = OPRESULT_FAIL;
		}
	}
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

void LOGIC_Wrapper_TerminateX(DeviceSubState NextState, xExecFunction TerminateFunc, xHandleFaultFunction FaultFunc)
{
	ExecutionResult res = TerminateFunc();
	if(res == ER_NoError)
		CONTROL_SetDeviceState(DS_InProcess, NextState);
	else
		LOGIC_HandleMuxExecResult(res);
}
//-----------------------------

void LOGIC_Wrapper_ReadResultX(DeviceSubState NextState, uint16_t *Problem, uint16_t ProblemCode,
		xIsProblemFunction IsInProblemFunc, xReadResultFunction ResultFunc, pVIPair Result, xHandleFaultFunction FaultFunc)
{
	if(*Problem == PROBLEM_NONE)
	{
		uint16_t OpResult;
		ExecutionResult res = ResultFunc(&OpResult, Result);

		if(res == ER_NoError)
		{
			if(IsInProblemFunc() || (OpResult != COMM_OPRESULT_OK))
				*Problem = ProblemCode;
		}
		else
		{
			FaultFunc(res);
			return;
		}
	}

	CONTROL_SetDeviceState(DS_InProcess, NextState);
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
	LOGIC_Wrapper_TerminateX(NextState, &MUX_Disconnect, &LOGIC_HandleMuxExecResult);
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
	LOGIC_Wrapper_TerminateX(NextState, &LOGIC_StopControl, &LOGIC_HandleControlExecResult);
}
//-----------------------------

void LOGIC_Wrapper_IsOutputReadyX(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem, xIsReadyFunction MainReadyFunc, xIsProblemFunction IsInProblemFunc,
		uint16_t ReadyProblem, uint16_t TimeoutProblem, xHandleFaultFunction FaultFunc)
{
	bool IsVoltageReady;
	ExecutionResult res = MainReadyFunc(&IsVoltageReady);

	if(res == ER_NoError)
	{
		if(IsVoltageReady)
			CONTROL_SetDeviceState(DS_InProcess, NextState);
		else if(IsInProblemFunc())
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

void LOGIC_Wrapper_IsControlOutputReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem)
{
	LOGIC_Wrapper_IsOutputReadyX(NextState, StopState, Timeout, Problem, &LOGIC_IsControlVoltageReady, &LOGIC_IsControlInProblem,
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

void LOGIC_Wrapper_IsControlNodeReady(DeviceSubState NextState)
{
	if(LOGIC_IsControlNodeReady())
		CONTROL_SetDeviceState(DS_InProcess, NextState);
}
//-----------------------------

void LOGIC_Wrapper_ControlReadResult(DeviceSubState NextState, pVIPair Result, uint16_t *Problem)
{
	LOGIC_Wrapper_ReadResultX(NextState, Problem, PROBLEM_CONTROL_RESULT,
			&LOGIC_IsControlInProblem, &LOGIC_ControlReadResult, Result, LOGIC_HandleControlExecResult);
}
//-----------------------------

void LOGIC_Wrapper_ControlSaveResult(VIPair Result)
{
	DT_Write32(REG_RESULT_CONTROL_VOLTAGE, REG_RESULT_CONTROL_VOLTAGE_32, Result.Voltage);
	DT_Write32(REG_RESULT_CONTROL_CURRENT, REG_RESULT_CONTROL_CURRENT_32, Result.Current);
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

void LOGIC_Wrapper_CurrentReadResult(DeviceSubState NextState, pVIPair Result, uint16_t *Problem)
{
	LOGIC_Wrapper_ReadResultX(NextState, Problem, PROBLEM_CURRENT_RESULT,
			&LOGIC_IsCurrentInProblem, &LOGIC_CurrentReadResult, Result, LOGIC_HandleCurrentExecResult);
}
//-----------------------------

void LOGIC_Wrapper_CurrentSaveResult(VIPair Result)
{
	DT_Write32(REG_RESULT_ON_VOLTAGE, REG_RESULT_ON_VOLTAGE_32, Result.Voltage);
	DT_Write32(REG_RESULT_ON_CURRENT, REG_RESULT_ON_CURRENT_32, Result.Current);
}
//-----------------------------

void LOGIC_Wrapper_StartLeakage(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem)
{
	LOGIC_Wrapper_ExecuteX(NextState, StopState, Timeout, Problem,
			&LOGIC_StartLeakage, PROBLEM_LEAKAGE_CONFIG, &LOGIC_HandleLeakageExecResult);
}
//-----------------------------

void LOGIC_Wrapper_IsLeakageOutputReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem)
{
	LOGIC_Wrapper_IsOutputReadyX(NextState, StopState, Timeout, Problem, &LOGIC_IsLeakageVoltageReady, &LOGIC_IsLeakagelInProblem,
			PROBLEM_LEAKAGE_IN_PROBLEM, PROBLEM_LEAKAGE_READY_TIMEOUT, &LOGIC_HandleLeakageExecResult);
}
//-----------------------------

void LOGIC_Wrapper_StopLeakage(DeviceSubState NextState)
{
	LOGIC_Wrapper_TerminateX(NextState, &LOGIC_StopLeakage, &LOGIC_HandleLeakageExecResult);
}
//-----------------------------

void LOGIC_Wrapper_IsLeakageNodeReady(DeviceSubState NextState)
{
	if(LOGIC_IsLeakageNodeReady())
		CONTROL_SetDeviceState(DS_InProcess, NextState);
}
//-----------------------------

void LOGIC_Wrapper_LeakageReadResult(DeviceSubState NextState, pVIPair Result, uint16_t *Problem)
{
	LOGIC_Wrapper_ReadResultX(NextState, Problem, PROBLEM_LEAKAGE_RESULT,
			&LOGIC_IsLeakagelInProblem, &LOGIC_LeakageReadResult, Result, LOGIC_HandleLeakageExecResult);
}
//-----------------------------

void LOGIC_Wrapper_LeakageSaveResult(VIPair Result)
{
	DT_Write32(REG_RESULT_LEAKAGE_CURRENT, REG_RESULT_LEAKAGE_CURRENT_32, Result.Current);
}
//-----------------------------

void LOGIC_Wrapper_StartCalibration(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem)
{
	LOGIC_Wrapper_ExecuteX(NextState, StopState, Timeout, Problem,
			&LOGIC_StartCalibration, PROBLEM_CAL_CONFIG, &LOGIC_HandleCalibrationExecResult);
}
//-----------------------------

void LOGIC_Wrapper_IsCalibrationOutputReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem)
{
	LOGIC_Wrapper_IsOutputReadyX(NextState, StopState, Timeout, Problem, &LOGIC_IsCalibrationReady, &LOGIC_IsCalibrationInProblem,
			PROBLEM_CAL_IN_PROBLEM, PROBLEM_CAL_READY_TIMEOUT, &LOGIC_HandleCalibrationExecResult);
}
//-----------------------------

void LOGIC_Wrapper_StopCalibration(DeviceSubState NextState)
{
	LOGIC_Wrapper_TerminateX(NextState, &LOGIC_StopCalibration, &LOGIC_HandleCalibrationExecResult);
}
//-----------------------------

void LOGIC_Wrapper_CalibrationReadResult(DeviceSubState NextState, pVIPair Result, uint16_t *Problem)
{
	LOGIC_Wrapper_ReadResultX(NextState, Problem, PROBLEM_CAL_RESULT,
			&LOGIC_IsCalibrationInProblem, &LOGIC_CalibrationReadResult, Result, LOGIC_HandleCalibrationExecResult);
}
//-----------------------------

void LOGIC_Wrapper_CalibrationSaveResult(VIPair Result)
{
	DT_Write32(REG_CALIBRATION_VOLTAGE, REG_CALIBRATION_VOLTAGE_32, Result.Voltage);
	DT_Write32(REG_CALIBRATION_CURRENT, REG_CALIBRATION_CURRENT_32, Result.Current);
}
//-----------------------------

void LOGIC_Wrapper_StartPowerSupply(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem)
{
	LOGIC_Wrapper_ExecuteX(NextState, StopState, Timeout, Problem,
			&LOGIC_StartPowerSupply, PROBLEM_PS_CONFIG, &LOGIC_HandlePowerSupplyExecResult);
}
//-----------------------------

void LOGIC_Wrapper_IsPowerSupplyOutputReady(DeviceSubState NextState, DeviceSubState StopState,
		uint64_t *Timeout, uint16_t *Problem)
{
	LOGIC_Wrapper_IsOutputReadyX(NextState, StopState, Timeout, Problem, &LOGIC_IsPowerSupplyReady, &LOGIC_IsPowerSupplyInProblem,
			PROBLEM_PS_IN_PROBLEM, PROBLEM_PS_READY_TIMEOUT, &LOGIC_HandlePowerSupplyExecResult);
}
//-----------------------------

void LOGIC_Wrapper_StopPowerSupply(DeviceSubState NextState)
{
	LOGIC_Wrapper_TerminateX(NextState, &LOGIC_StopPowerSupply, &LOGIC_HandlePowerSupplyExecResult);
}
//-----------------------------

void LOGIC_Wrapper_PowerSupply1ReadResult(DeviceSubState NextState, pVIPair Result, uint16_t *Problem)
{
	LOGIC_Wrapper_ReadResultX(NextState, Problem, PROBLEM_PS_RESULT,
			&LOGIC_IsPowerSupplyInProblem, &LOGIC_PowerSupply1ReadResult, Result, LOGIC_HandlePowerSupplyExecResult);
}
//-----------------------------

void LOGIC_Wrapper_PowerSupply2ReadResult(DeviceSubState NextState, pVIPair Result, uint16_t *Problem)
{
	LOGIC_Wrapper_ReadResultX(NextState, Problem, PROBLEM_PS_RESULT,
			&LOGIC_IsPowerSupplyInProblem, &LOGIC_PowerSupply1ReadResult, Result, LOGIC_HandlePowerSupplyExecResult);
}
//-----------------------------

void LOGIC_Wrapper_PowerSupply1SaveResult(VIPair Result)
{
	DT_Write32(REG_RESULT_AUX_CURRENT1, REG_RESULT_AUX_CURRENT1_32, Result.Current);
}
//-----------------------------

void LOGIC_Wrapper_PowerSupply2SaveResult(VIPair Result)
{
	DT_Write32(REG_RESULT_AUX_CURRENT2, REG_RESULT_AUX_CURRENT2_32, Result.Current);
}
//-----------------------------
