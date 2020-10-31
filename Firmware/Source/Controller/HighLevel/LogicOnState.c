// Header
#include "LogicOnState.h"

// Includes
#include "Logic.h"
#include "Controller.h"
#include "Common.h"
#include "CommonDictionary.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Global.h"
#include "Multiplexer.h"
#include "CurrentBoard.h"
#include "DCVoltageBoard.h"
#include "ACVoltageBoard.h"

// Functions
void ONSTATE_HandleMeasurement()
{
	static Int64U Timeout;

	if(CONTROL_State == DS_InProcess)
	{
		if(COMM_IsSlaveInFaultOrDisabled())
			CONTROL_SwitchToFault(ER_WrongState, FAULT_EXT_GR_COMMON);

		ExecutionResult res;
		switch(CONTROL_SubState)
		{
			case DSS_OnVoltage_StartTest:
				if(COMM_AreSlavesInStateX(CDS_Ready))
					CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_Commutate);
				else
					CONTROL_SwitchToFault(ER_WrongState, FAULT_EXT_GR_COMMON);
				break;

			case DSS_OnVoltage_Commutate:
				{
					res = MUX_Connect();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_StartControl);
					else
						CONTROL_SwitchToFault(res, FAULT_EXT_GR_MUX);
				}
				break;

			case DSS_OnVoltage_StartControl:
				{
					res = LOGIC_StartControl();
					if(res == ER_NoError)
					{
						Timeout = DataTable[REG_GENERAL_LOGIC_TIMEOUT] + CONTROL_TimeCounter;
						CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_WaitControlReady);
					}
					else
						LOGIC_HandleControlExecResult(res);
				}
				break;

			case DSS_OnVoltage_WaitControlReady:
				{
					bool IsVoltageReady;
					res = LOGIC_IsControlVoltageReady(&IsVoltageReady);
					if(res == ER_NoError)
					{
						if(IsVoltageReady)
							CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_PulseCurrent);
						else if(CONTROL_TimeCounter > Timeout)
							LOGIC_HandleControlExecResult(ER_ChangeStateTimeout);
					}
					else
						LOGIC_HandleControlExecResult(res);
				}
				break;

			case DSS_OnVoltage_PulseCurrent:
				{
					res = CURR_Execute();
					if(res == ER_NoError)
					{
						Timeout = DataTable[REG_GENERAL_LOGIC_TIMEOUT] + CONTROL_TimeCounter;
						CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_WaitCurrentReady);
					}
					else
						CONTROL_SwitchToFault(res, FAULT_EXT_GR_DC_CURRENT);
				}
				break;

			case DSS_OnVoltage_WaitCurrentReady:
				{
					if(COMM_AreSlavesInStateX(CDS_Ready))
						CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_StopControl);
					else if(CONTROL_TimeCounter > Timeout)
						CONTROL_SwitchToFault(ER_ChangeStateTimeout, FAULT_EXT_GR_DC_CURRENT);
				}
				break;

			case DSS_OnVoltage_StopControl:
				{
					res = LOGIC_StopControl();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_UnCommutate);
					else
						LOGIC_HandleControlExecResult(res);
				}
				break;

			case DSS_OnVoltage_UnCommutate:
				{
					res = MUX_Disconnect();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_ReadResult);
					else
						CONTROL_SwitchToFault(res, FAULT_EXT_GR_MUX);
				}
				break;

			case DSS_OnVoltage_ReadResult:
				{
					res = CURR_ReadResult();
					if(res == ER_NoError)
					{
						pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_DCCurrent);
						pCurrentBoardObject Settings = (pCurrentBoardObject)NodeData->Settings;

						if(NodeData->OpResult == OPRESULT_OK)
						{
							DataTable[REG_OP_RESULT] = OPRESULT_OK;
							DT_Write32(REG_RESULT_ON_VOLTAGE, REG_RESULT_ON_VOLTAGE_32, Settings->Result.Voltage);
							DT_Write32(REG_RESULT_ON_CURRENT, REG_RESULT_ON_CURRENT_32, Settings->Result.Current);
						}
						else
							DataTable[REG_OP_RESULT] = OPRESULT_FAIL;

						CONTROL_SetDeviceState(DS_Ready, DSS_None);
					}
					else
						CONTROL_SwitchToFault(res, FAULT_EXT_GR_DC_CURRENT);
				}
				break;

			default:
				break;
		}
	}
}
//-----------------------------
