// Header
#include "LogicLeakage.h"

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
#include "DCHighVoltageBoard.h"

// Forward functions
void LEAK_HandleExecResult(ExecutionResult Result);
ExecutionResult LEAK_Start();
ExecutionResult LEAK_Stop();
ExecutionResult LEAK_IsVoltageReady(bool *IsReady);
ExecutionResult LEAK_ReadResult(uint16_t *OpResult, pVIPair Result);

// Functions
void LEAK_HandleMeasurement()
{
	static Int64U Timeout;

	if(CONTROL_State == DS_InProcess)
	{
		if(COMM_IsSlaveInFaultOrDisabled())
			CONTROL_SwitchToFault(ER_WrongState, FAULT_EXT_GR_COMMON);

		ExecutionResult res;
		switch(CONTROL_SubState)
		{
			case DSS_Leakage_StartTest:
				if(COMM_AreSlavesInStateX(CDS_Ready))
					CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_Commutate);
				else
					CONTROL_SwitchToFault(ER_WrongState, FAULT_EXT_GR_COMMON);
				break;

			case DSS_Leakage_Commutate:
				{
					res = MUX_Connect();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_StartControl);
					else
						CONTROL_SwitchToFault(res, FAULT_EXT_GR_MUX);
				}
				break;

			case DSS_Leakage_StartControl:
				{
					res = LOGIC_StartControl();
					if(res == ER_NoError)
					{
						Timeout = DataTable[REG_GENERAL_LOGIC_TIMEOUT] + CONTROL_TimeCounter;
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_WaitControlReady);
					}
					else
						LOGIC_HandleControlExecResult(res);
				}
				break;

			case DSS_Leakage_WaitControlReady:
				{
					bool IsVoltageReady;
					res = LOGIC_IsControlVoltageReady(&IsVoltageReady);
					if(res == ER_NoError)
					{
						if(IsVoltageReady)
							CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_StartOutVoltage);
						else if(CONTROL_TimeCounter > Timeout)
							LOGIC_HandleControlExecResult(ER_ChangeStateTimeout);
					}
					else
						LOGIC_HandleControlExecResult(res);
				}
				break;

			case DSS_Leakage_StartOutVoltage:
				{
					res = LEAK_Start();
					if(res == ER_NoError)
					{
						Timeout = DataTable[REG_GENERAL_LOGIC_TIMEOUT] + CONTROL_TimeCounter;
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_WaitOutVoltageReady);
					}
					else
						LEAK_HandleExecResult(res);
				}
				break;

			case DSS_Leakage_WaitOutVoltageReady:
				{
					bool IsVoltageReady;
					res = LEAK_IsVoltageReady(&IsVoltageReady);
					if(res == ER_NoError)
					{
						if(IsVoltageReady)
							CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_StopOutVoltage);
						else if(CONTROL_TimeCounter > Timeout)
							LEAK_HandleExecResult(ER_ChangeStateTimeout);
					}
					else
						LEAK_HandleExecResult(res);
				}
				break;

			case DSS_Leakage_StopOutVoltage:
				{
					res = LEAK_Stop();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_StopControl);
					else
						LEAK_HandleExecResult(res);
				}
				break;

			case DSS_Leakage_StopControl:
				{
					res = LOGIC_StopControl();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_UnCommutate);
					else
						LOGIC_HandleControlExecResult(res);
				}
				break;

			case DSS_Leakage_UnCommutate:
				{
					res = MUX_Disconnect();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_ReadResult);
					else
						CONTROL_SwitchToFault(res, FAULT_EXT_GR_MUX);
				}
				break;

			case DSS_Leakage_ReadResult:
				{
					VIPair Result;
					uint16_t OpResult;
					res = LEAK_ReadResult(&OpResult, &Result);

					if(res == ER_NoError)
					{
						if(OpResult == OPRESULT_OK)
						{
							DataTable[REG_OP_RESULT] = OPRESULT_OK;
							DataTable[REG_RESULT_LEAKAGE_CURRENT] = Result.Current;
						}
						else
							DataTable[REG_OP_RESULT] = OPRESULT_FAIL;

						CONTROL_SetDeviceState(DS_Ready, DSS_None);
					}
					else
						LEAK_HandleExecResult(res);
				}
				break;

			default:
				break;
		}
	}
}
//-----------------------------

void LEAK_HandleExecResult(ExecutionResult Result)
{
	CONTROL_SwitchToFault(Result,
		LOGIC_IsDCLeakage() ? FAULT_EXT_GR_DC_HV : FAULT_EXT_GR_AC_VOLTAGE2);
}
//-----------------------------

ExecutionResult LEAK_Start()
{
	return LOGIC_IsDCLeakage() ? DCHV_Execute() : ACV_Execute(NAME_ACVoltage2);
}
//-----------------------------

ExecutionResult LEAK_Stop()
{
	return LOGIC_IsDCLeakage() ? DCHV_Stop() : ACV_Stop(NAME_ACVoltage2);
}
//-----------------------------

ExecutionResult LEAK_IsVoltageReady(bool *IsReady)
{
	return LOGIC_IsDCLeakage() ? DCHV_IsVoltageReady(IsReady) : ACV_IsVoltageReady(NAME_ACVoltage2, IsReady);
}
//-----------------------------

ExecutionResult LEAK_ReadResult(uint16_t *OpResult, pVIPair Result)
{
	ExecutionResult res;
	pSlaveNode NodeData;

	if(LOGIC_IsDCLeakage())
	{
		NodeData = COMM_GetSlaveDevicePointer(NAME_DCHighVoltage);
		pDCHVoltageBoardObject Settings = (pDCHVoltageBoardObject)NodeData->Settings;

		res = DCHV_ReadResult();
		*Result = Settings->Result;
	}
	else
	{
		NodeData = COMM_GetSlaveDevicePointer(NAME_ACVoltage2);
		pACVoltageBoardObject Settings = (pACVoltageBoardObject)NodeData->Settings;

		res = ACV_ReadResult(NAME_ACVoltage2);
		*Result = Settings->Result;
	}

	*OpResult = NodeData->OpResult;
	return res;
}
//-----------------------------
