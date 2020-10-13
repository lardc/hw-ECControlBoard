// Header
#include "Logic.h"

// Includes
#include "Controller.h"
#include "CommonDictionary.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Global.h"
#include "Multiplexer.h"
#include "CurrentBoard.h"
#include "DCVoltageBoard.h"
#include "ACVoltageBoard.h"
#include "DCHighVoltageBoard.h"

// Variables
static volatile MuxObject Multiplexer;
static volatile CurrentBoardObject CurrentBoard;
static volatile DCVoltageBoardObject DCVoltageBoard1, DCVoltageBoard2, DCVoltageBoard3;
static volatile ACVoltageBoardObject ACVoltageBoard1, ACVoltageBoard2;
static volatile DCHVoltageBoardObject DCHighVoltageBoard;

// Forward functions
void LOGIC_AttachSettings(NodeName Name, void *SettingsPointer);
LogicConfigError LOGIC_CacheMuxSettings();
void LOGIC_CacheCurrentBoardSettings();
void LOGIC_CacheControlSettings();
void LOGIC_CacheLeakageSettings();

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

LogicConfigError LOGIC_PrepareMeasurement()
{
	LogicConfigError err = LOGIC_CacheMuxSettings();
	if(err == LCE_None)
	{
		switch(Multiplexer.MeasureType)
		{
			case MT_LeakageCurrent:
				{
					LOGIC_CacheLeakageSettings();
					LOGIC_CacheControlSettings();
					CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_StartTest);
				}
				break;

			case MT_OnVoltage:
				{
					LOGIC_CacheCurrentBoardSettings();
					LOGIC_CacheControlSettings();
					CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_StartTest);
				}
				break;

			case MT_InputVoltageCurrent:
				break;

			case MT_InhibitVoltage:
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
	if((Multiplexer.Position == Position2 && DUTConfig->OutputPositionsNum == OneOutput) ||
		(Multiplexer.Position == Position3 && DUTConfig->OutputPositionsNum != ThreeOutputs))
		return LCE_PositionMissmatch;

	return LCE_None;
}
//-----------------------------

void LOGIC_CacheCurrentBoardSettings()
{
	CurrentBoard.Setpoint.Current = DataTable[REG_COMM_CURRENT];
	CurrentBoard.Setpoint.Voltage = DataTable[REG_COMM_VOLTAGE];
}
//-----------------------------

void LOGIC_CacheControlSettings()
{
	VIPair Setpoint;
	Setpoint.Voltage = DataTable[REG_CONTROL_VOLTAGE];
	Setpoint.Current = DataTable[REG_CONTROL_CURRENT];

	if(LOGIC_IsDCControl())
	{
		DCVoltageBoard1.Setpoint = Setpoint;
		DCVoltageBoard1.OutputLine = DC_CTRL;
		DCVoltageBoard1.OutputType = Multiplexer.InputType;
		DCVoltageBoard1.OutputMode = Continuous;
	}
	else
	{
		ACVoltageBoard1.Setpoint = Setpoint;
		ACVoltageBoard1.OutputLine = AC_CTRL;
	}
}
//-----------------------------

void LOGIC_CacheLeakageSettings()
{
	VIPair Setpoint;
	Setpoint.Voltage = DataTable[REG_COMM_VOLTAGE];
	Setpoint.Current = DataTable[REG_COMM_CURRENT];

	if(LOGIC_IsDCLeakage())
	{
		DCHighVoltageBoard.Setpoint = Setpoint;
	}
	else
	{
		ACVoltageBoard2.Setpoint = Setpoint;
		ACVoltageBoard2.OutputLine = AC_BUS_LV;
	}
}
//-----------------------------

bool LOGIC_IsDCControl()
{
	return Multiplexer.InputType == ControlIDC || Multiplexer.InputType == ControlVDC;
}
//-----------------------------

bool LOGIC_IsDCLeakage()
{
	return Multiplexer.LeakageType == LeakageDC;
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
	return LOGIC_IsDCControl() ? DCV_Execute(NAME_DCVoltage1) : ACV_Execute(NAME_ACVoltage1);
}
//-----------------------------

ExecutionResult LOGIC_StopControl()
{
	return LOGIC_IsDCControl() ? DCV_Stop(NAME_DCVoltage1) : ACV_Stop(NAME_ACVoltage1);
}
//-----------------------------

ExecutionResult LOGIC_IsControlVoltageReady(bool *IsReady)
{
	return LOGIC_IsDCControl() ? DCV_IsVoltageReady(NAME_DCVoltage1, IsReady) :
		ACV_IsVoltageReady(NAME_ACVoltage1, IsReady);
}
//-----------------------------
