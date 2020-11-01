// Header
#include "LogicControl.h"

// Includes
#include "Logic.h"
#include "Controller.h"
#include "Common.h"
#include "CommonDictionary.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Global.h"
#include "Multiplexer.h"
#include "DCVoltageBoard.h"
#include "ACVoltageBoard.h"

// Forward functions
ExecutionResult CTRL_ControlResult(uint16_t *OpResult, pVIPair Result);

// Functions
void CTRL_HandleMeasurement()
{
	static Int64U Timeout;

	if(CONTROL_State == DS_InProcess)
	{
		if(COMM_IsSlaveInFaultOrDisabled())
			CONTROL_SwitchToFault(ER_WrongState, FAULT_EXT_GR_COMMON);

		ExecutionResult res;
		switch(CONTROL_SubState)
		{
			case DSS_Control_StartTest:
				{
					if(COMM_AreSlavesInStateX(CDS_Ready))
						CONTROL_SetDeviceState(DS_InProcess, DSS_Control_Commutate);
					else
						CONTROL_SwitchToFault(ER_WrongState, FAULT_EXT_GR_COMMON);
				}
				break;

			case DSS_Control_Commutate:
				{
					res = MUX_Connect();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_Control_WaitCommutation);
					else
						CONTROL_SwitchToFault(res, FAULT_EXT_GR_MUX);
				}
				break;

			case DSS_Control_WaitCommutation:
				{
					if(COMM_AreSlavesInStateX(CDS_Ready))
							CONTROL_SetDeviceState(DS_InProcess, DSS_Control_StartControl);
				}
				break;

			case DSS_Control_StartControl:
				{
					res = LOGIC_StartControl();
					if(res == ER_NoError)
					{
						Timeout = DataTable[REG_GENERAL_LOGIC_TIMEOUT] + CONTROL_TimeCounter;
						CONTROL_SetDeviceState(DS_InProcess, DSS_Control_WaitControlReady);
					}
					else
						LOGIC_HandleControlExecResult(res);
				}
				break;

			case DSS_Control_WaitControlReady:
				{
					bool IsVoltageReady;
					res = LOGIC_IsControlVoltageReady(&IsVoltageReady);
					if(res == ER_NoError)
					{
						if(IsVoltageReady)
							CONTROL_SetDeviceState(DS_InProcess, DSS_Control_StopControl);
						else if(CONTROL_TimeCounter > Timeout)
							LOGIC_HandleControlExecResult(ER_ChangeStateTimeout);
					}
					else
						LOGIC_HandleControlExecResult(res);
				}
				break;

			case DSS_Control_StopControl:
				{
					res = LOGIC_StopControl();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_Control_WaitStopControl);
					else
						LOGIC_HandleControlExecResult(res);
				}
				break;

			case DSS_Control_WaitStopControl:
				{
					if(COMM_AreSlavesInStateX(CDS_Ready))
							CONTROL_SetDeviceState(DS_InProcess, DSS_Control_UnCommutate);
				}
				break;

			case DSS_Control_UnCommutate:
				{
					res = MUX_Disconnect();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_Control_WaitUnCommutate);
					else
						CONTROL_SwitchToFault(res, FAULT_EXT_GR_MUX);
				}
				break;

			case DSS_Control_WaitUnCommutate:
				{
					if(COMM_AreSlavesInStateX(CDS_Ready))
							CONTROL_SetDeviceState(DS_InProcess, DSS_Control_ReadResult);
				}
				break;

			case DSS_Control_ReadResult:
				{
					VIPair Result;
					uint16_t OpResult;
					res = CTRL_ControlResult(&OpResult, &Result);

					if(res == ER_NoError)
					{
						if(OpResult == OPRESULT_OK)
						{
							DataTable[REG_OP_RESULT] = OPRESULT_OK;
							DT_Write32(REG_RESULT_CONTROL_VOLTAGE, REG_RESULT_CONTROL_VOLTAGE_32, Result.Voltage);
							DT_Write32(REG_RESULT_CONTROL_CURRENT, REG_RESULT_CONTROL_CURRENT_32, Result.Current);
						}
						else
							DataTable[REG_OP_RESULT] = OPRESULT_FAIL;

						CONTROL_SetDeviceState(DS_Ready, DSS_None);
					}
					else
						LOGIC_HandleControlExecResult(res);
				}
				break;

			default:
				break;
		}
	}
}
//-----------------------------

ExecutionResult CTRL_ControlResult(uint16_t *OpResult, pVIPair Result)
{
	ExecutionResult res;
	pSlaveNode NodeData;

	if(LOGIC_IsDCControl())
	{
		NodeData = COMM_GetSlaveDevicePointer(NAME_DCVoltage1);
		pDCVoltageBoardObject Settings = (pDCVoltageBoardObject)NodeData->Settings;

		res = DCV_ReadResult(NAME_DCVoltage1);
		*Result = Settings->Result;
	}
	else
	{
		NodeData = COMM_GetSlaveDevicePointer(NAME_ACVoltage1);
		pACVoltageBoardObject Settings = (pACVoltageBoardObject)NodeData->Settings;

		res = ACV_ReadResult(NAME_ACVoltage1);
		*Result = Settings->Result;
	}

	*OpResult = NodeData->OpResult;
	return res;
}
//-----------------------------
