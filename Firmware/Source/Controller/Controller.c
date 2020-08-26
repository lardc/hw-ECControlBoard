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
#include "DebugActions.h"

// Types
//
typedef void (*FUNC_AsyncDelegate)();

// Variables
//
volatile DeviceState CONTROL_State = DS_None;
static Boolean CycleActive = false;

volatile Int64U CONTROL_TimeCounter = 0;

/// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_SetDeviceState(DeviceState NewState);
void CONTROL_SwitchToFault(Int16U Reason);
void CONTROL_DelayMs(uint32_t Delay);
void CONTROL_UpdateWatchDog();
void CONTROL_ResetToDefaultState();

// Functions
//
void CONTROL_Init()
{
	// Конфигурация сервиса работы Data-table и EPROM
	EPROMServiceConfig EPROMService = {(FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT};
	// Инициализация data table
	DT_Init(EPROMService, false);
	// Инициализация device profile
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	// Сброс значений
	DEVPROFILE_ResetControlSection();
	CONTROL_ResetToDefaultState();
}
//------------------------------------------

void CONTROL_ResetToDefaultState()
{
	DataTable[REG_FAULT_REASON] = DF_NONE;
	DataTable[REG_DISABLE_REASON] = DF_NONE;
	DataTable[REG_WARNING] = WARNING_NONE;
	DataTable[REG_PROBLEM] = PROBLEM_NONE;
	DataTable[REG_OP_RESULT] = OPRESULT_NONE;
	
	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();
	
	CONTROL_SetDeviceState(DS_None);
}

//------------------------------------------

void CONTROL_Idle()
{
	DEVPROFILE_ProcessRequests();
	CONTROL_UpdateWatchDog();
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
				{
					CONTROL_SetDeviceState(DS_Enabled);
				}
				else if(CONTROL_State != DS_Enabled)
				{
					*pUserError = ERR_DEVICE_NOT_READY;
				}
				break;
			}
			
		case ACT_DISABLE_POWER:
			if(CONTROL_State == DS_Enabled)
			{
				CONTROL_SetDeviceState(DS_None);
			}
			else
				*pUserError = ERR_OPERATION_BLOCKED;
			break;
			
		case ACT_DBG_PULSE_EXT_SYNC1:
			{
				if(DataTable[REG_DBG_STATE])
				{
					DBGACT_GenerateImpulseToExtLineSync1();
				}
				else
				{
					*pUserError = ERR_CONFIGURATION_LOCKED;
				}
			}
			break;

		case ACT_DBG_PULSE_EXT_SYNC2:
			{
				if(DataTable[REG_DBG_STATE])
				{
					DBGACT_GenerateImpulseToExtLineSync2();
				}
				else
				{
					*pUserError = ERR_CONFIGURATION_LOCKED;
				}
			}
			break;

		case ACT_DBG_PULSE_INT_SYNC1:
			{
				if(DataTable[REG_DBG_STATE])
				{
					DBGACT_GenerateImpulseToIntLineSync1();
				}
				else
				{
					*pUserError = ERR_CONFIGURATION_LOCKED;
				}
			}
			break;

		case ACT_DBG_PULSE_INT_SYNC2:
			{
				if(DataTable[REG_DBG_STATE])
				{
					DBGACT_GenerateImpulseToIntLineSync2();
				}
				else
				{
					*pUserError = ERR_CONFIGURATION_LOCKED;
				}
			}
			break;

		case ACT_DBG_IS_STATE_INT_SYNC_1:
			{
				if(DataTable[REG_DBG_STATE])
				{
					DataTable[REG_DBG_INT_SYNC1] = DBGACT_ReadStateIntLineSync1();
				}
				else
				{
					*pUserError = ERR_CONFIGURATION_LOCKED;
				}
			}
			break;

		case ACT_DBG_IS_STATE_INT_SYNC_2:
			{
				if(DataTable[REG_DBG_STATE])
				{
					DataTable[REG_DBG_INT_SYNC2] = DBGACT_ReadStateIntLineSync2();
				}
				else
				{
					*pUserError = ERR_CONFIGURATION_LOCKED;
				}
			}
			break;

		case ACT_DBG_PULSE_EXT_LED:
			{
				if(DataTable[REG_DBG_STATE])
				{
					DBGACT_BlinkExtLed();
				}
				else
				{
					*pUserError = ERR_CONFIGURATION_LOCKED;
				}
			}
			break;

		case ACT_FAN_ON:
			{
				LL_SetStateFan(true);
				DataTable[REG_FAN_STATE] = true;
			}
			break;

		case ACT_FAN_OFF:
			{
				LL_SetStateFan(false);
				DataTable[REG_FAN_STATE] = false;
			}
			break;

		default:
			return false;
			
	}
	return true;
}
//-----------------------------------------------

void CONTROL_SwitchToFault(Int16U Reason)
{
	CONTROL_SetDeviceState(DS_Fault);
	DataTable[REG_FAULT_REASON] = Reason;
}
//------------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState)
{
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;
}
//------------------------------------------

void CONTROL_DelayMs(uint32_t Delay)
{
	uint64_t Counter = (uint64_t)CONTROL_TimeCounter + Delay;
	while(Counter > CONTROL_TimeCounter)
		CONTROL_UpdateWatchDog();
}
//------------------------------------------

void CONTROL_UpdateWatchDog()
{
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
//------------------------------------------

