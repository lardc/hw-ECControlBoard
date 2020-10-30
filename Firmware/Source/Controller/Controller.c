// Header
#include "Controller.h"
//
// Includes
#include "Board.h"
#include "Delay.h"
#include "DeviceProfile.h"
#include "Interrupts.h"
#include "Global.h"
#include "LowLevel.h"
#include "SysConfig.h"
#include "Diagnostic.h"
#include "BCCIMHighLevel.h"
#include "BCCIxParams.h"
#include "Logic.h"
#include "LogicLeakage.h"
#include "LogicOnState.h"
#include "LogicControl.h"
#include "LogicInhibit.h"

// Types
//
typedef void (*FUNC_AsyncDelegate)();

// Variables
//
DeviceState CONTROL_State = DS_None;
DeviceSubState CONTROL_SubState = DSS_None;
static Boolean CycleActive = false, SafetyMonitorActive = false;
static Int16U CONTROL_EPCounter, CONTROL_EPArray[XCCI_EP_SIZE];

volatile Int64U CONTROL_TimeCounter = 0;

/// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_UpdateWatchDog();
void CONTROL_ResetToDefaultState();
void CONTROL_ResetOutputRegisters();

// Functions
//
void CONTROL_Init()
{
	// Переменные для конфигурации EndPoint
	Int16U EPIndexes[EP_COUNT] = {EP_SLAVE_DATA};
	Int16U EPSized[EP_COUNT] = {XCCI_EP_SIZE};
	pInt16U EPCounters[EP_COUNT] = {&CONTROL_EPCounter};
	pInt16U EPDatas[EP_COUNT] = {CONTROL_EPArray};

	// Конфигурация сервиса работы Data-table и EPROM
	EPROMServiceConfig EPROMService = {(FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT};
	// Инициализация data table
	DT_Init(EPROMService, false);
	DT_SaveFirmwareInfo(CAN_SLAVE_NID, CAN_MASTER_NID);
	// Инициализация device profile
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_InitEPService(EPIndexes, EPSized, EPCounters, EPDatas);
	// Сброс значений
	DEVPROFILE_ResetControlSection();
	CONTROL_ResetToDefaultState();

	// Инициализация сущностей логики верхнего уровня
	LOGIC_InitEntities();
}
//------------------------------------------

void CONTROL_ResetOutputRegisters()
{
	DataTable[REG_FAULT_REASON] = DF_NONE;
	DataTable[REG_DISABLE_REASON] = DF_NONE;
	DataTable[REG_WARNING] = WARNING_NONE;
	DataTable[REG_PROBLEM] = PROBLEM_NONE;
	DataTable[REG_OP_RESULT] = OPRESULT_NONE;
	DataTable[REG_CONFIG_ERR] = LCE_None;
	
	DataTable[REG_RESULT_LEAKAGE_CURRENT] = 0;
	DataTable[REG_RESULT_ON_VOLTAGE] = 0;
	DataTable[REG_RESULT_CONTROL_CURRENT] = 0;
	DataTable[REG_RESULT_CONTROL_VOLTAGE] = 0;
	DataTable[REG_RESULT_INHIBIT_VOLTAGE] = 0;
	DataTable[REG_RESULT_AUX_CURRENT1] = 0;
	DataTable[REG_RESULT_AUX_CURRENT2] = 0;

	DataTable[REG_RESULT_LEAKAGE_CURRENT_32] = 0;
	DataTable[REG_RESULT_ON_VOLTAGE_32] = 0;
	DataTable[REG_RESULT_CONTROL_CURRENT_32] = 0;
	DataTable[REG_RESULT_CONTROL_VOLTAGE_32] = 0;
	DataTable[REG_RESULT_INHIBIT_VOLTAGE_32] = 0;
	DataTable[REG_RESULT_AUX_CURRENT1_32] = 0;
	DataTable[REG_RESULT_AUX_CURRENT2_32] = 0;

	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();
}
//------------------------------------------

void CONTROL_ResetToDefaultState()
{
	CONTROL_ResetOutputRegisters();
	CONTROL_SetDeviceState(DS_None, DSS_None);
}
//------------------------------------------

void CONTROL_Idle()
{
	DEVPROFILE_ProcessRequests();
	CONTROL_UpdateWatchDog();

	LOGIC_HandleStateUpdate();
	LOGIC_HandlePowerOn();
	LOGIC_HandlePowerOff();

	LEAK_HandleMeasurement();
	ONSTATE_HandleMeasurement();
	CTRL_HandleMeasurement();
	INHIBIT_HandleMeasurement();
}
//------------------------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError)
{
	*pUserError = ERR_NONE;
	
	switch (ActionID)
	{
		case ACT_ENABLE_POWER:
			{
				if(CONTROL_State == DS_None)
					CONTROL_SetDeviceState(DS_InProcess, DSS_PowerEnable);
				else if(CONTROL_State != DS_Ready)
					*pUserError = ERR_DEVICE_NOT_READY;
			}
			break;
			
		case ACT_DISABLE_POWER:
			{
				if(CONTROL_State == DS_Ready)
					CONTROL_SetDeviceState(DS_None, DSS_PowerOff);
				else if(CONTROL_State != DS_None)
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_SAFETY_SET_ACTIVE:
			{
				if(CONTROL_State == DS_Ready)
					SafetyMonitorActive = true;
				else
					*pUserError = ERR_DEVICE_NOT_READY;
			}
			break;

		case ACT_SAFETY_SET_INACTIVE:
			{
				if(CONTROL_State == DS_SafetyTrig)
				{
					SafetyMonitorActive = false;
					CONTROL_SetDeviceState(DS_Ready, DSS_None);
				}
				else if(CONTROL_State != DS_Ready)
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_START_TEST:
			{
				if(CONTROL_State == DS_Ready)
				{
					CONTROL_ResetOutputRegisters();
					LogicConfigError err = LOGIC_PrepareMeasurement();
					DataTable[REG_CONFIG_ERR] = err;

					if(err != LCE_None)
						*pUserError = ERR_BAD_CONFIG;
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_FAULT_CLEAR:
			{
				if(CONTROL_State == DS_Fault)
				{
					CONTROL_SetDeviceState(DS_None, DSS_None);
					if(!COMM_SlavesClearFault())
						CONTROL_SwitchToFault(ER_InterfaceError, FAULT_EXT_GR_COMMON);
				}
			}
			break;

		case ACT_WARNING_CLEAR:
			DataTable[REG_WARNING] = WARNING_NONE;
			break;

		case ACT_DIAG_READ_REG:
			{
				uint16_t val;
				bool ret = BHL_ReadRegister(DataTable[REG_DIAG_NID], DataTable[REG_DIAG_IN_1], &val);
				if(ret)
				{
					DataTable[REG_DIAG_OUT_1] = ERR_NO_ERROR;
					DataTable[REG_DIAG_OUT_2] = val;
				}
				else
				{
					BHLError err = BHL_GetError();
					DataTable[REG_DIAG_OUT_1] = err.ErrorCode;
				}
			}
			break;

		case ACT_DIAG_WRITE_REG:
			{
				bool ret = BHL_WriteRegister(DataTable[REG_DIAG_NID], DataTable[REG_DIAG_IN_1], DataTable[REG_DIAG_IN_2]);
				if(ret)
					DataTable[REG_DIAG_OUT_1] = ERR_NO_ERROR;
				else
				{
					BHLError err = BHL_GetError();
					DataTable[REG_DIAG_OUT_1] = err.ErrorCode;
				}
			}
			break;

		case ACT_DIAG_CALL:
			{
				bool ret = BHL_Call(DataTable[REG_DIAG_NID], DataTable[REG_DIAG_IN_1]);
				if(ret)
					DataTable[REG_DIAG_OUT_1] = ERR_NO_ERROR;
				else
				{
					BHLError err = BHL_GetError();
					DataTable[REG_DIAG_OUT_1] = err.ErrorCode;
				}
			}
			break;

		case ACT_DIAG_READ_EP:
			{
				DEVPROFILE_ResetEPReadState();
				DEVPROFILE_ResetScopes(0);

				bool ret = BHL_ReadArray16(DataTable[REG_DIAG_NID], DataTable[REG_DIAG_IN_1], CONTROL_EPArray, XCCI_EP_SIZE, &CONTROL_EPCounter);
				if (ret)
				{
					DataTable[REG_DIAG_OUT_1] = ERR_NO_ERROR;
					DataTable[REG_DIAG_OUT_2] = CONTROL_EPCounter;
				}
				else
				{
					BHLError err = BHL_GetError();
					DataTable[REG_DIAG_OUT_1] = err.ErrorCode;
				}
			}
			break;

		default:
			return DIAG_HandleDiagnosticAction(ActionID, pUserError);
	}

	return true;
}
//-----------------------------------------------

void CONTROL_SwitchToFault(ExecutionResult Result, Int16U Group)
{
	if(Result != ER_NoError)
	{
		if(Result == ER_InterfaceError)
		{
			BHLError Error = BHL_GetError();
			DataTable[REG_BHL_ERROR_CODE] = Error.ErrorCode;
			DataTable[REG_BHL_DEVICE] = Error.Device;
			DataTable[REG_BHL_FUNCTION] = Error.Func;
			DataTable[REG_BHL_EXT_DATA] = Error.ExtData;
		}

		CONTROL_SetDeviceState(DS_Fault, DSS_None);
		DataTable[REG_FAULT_REASON] = Result + Group;
	}
}
//------------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState, DeviceSubState NewSubState)
{
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;

	CONTROL_SubState = NewSubState;
	DataTable[REG_DEV_SUB_STATE] = NewSubState;
}
//------------------------------------------

void CONTROL_UpdateWatchDog()
{
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
//------------------------------------------
