// Header
#include "LogicLeakage.h"

// Includes
#include "Logic.h"
#include "Controller.h"
#include "CommonDictionary.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Global.h"
#include "Multiplexer.h"
#include "DCVoltageBoard.h"
#include "ACVoltageBoard.h"
#include "DCHighVoltageBoard.h"

// Functions
void LEAK_HandleMeasurement()
{
	static Int64U Timeout;
	static uint16_t Problem = PROBLEM_NONE;

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
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_WaitCommutation);
					else
						CONTROL_SwitchToFault(res, FAULT_EXT_GR_MUX);
				}
				break;

			case DSS_Leakage_WaitCommutation:
				{
					if(COMM_AreSlavesInStateX(CDS_Ready))
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_StartControl);
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
						else if(LOGIC_IsControlInProblem())
						{
							Problem = PROBLEM_CONTROL_NODE;
							CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_UnCommutate);
						}
					}
					else
						LOGIC_HandleControlExecResult(res);
				}
				break;

			case DSS_Leakage_StartOutVoltage:
				{
					res = LOGIC_StartLeakage();
					if(res == ER_NoError)
					{
						Timeout = DataTable[REG_GENERAL_LOGIC_TIMEOUT] + CONTROL_TimeCounter;
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_WaitOutVoltageReady);
					}
					else
						LOGIC_HandleLeakageExecResult(res);
				}
				break;

			case DSS_Leakage_WaitOutVoltageReady:
				{
					bool IsVoltageReady;
					res = LOGIC_IsLeakageVoltageReady(&IsVoltageReady);
					if(res == ER_NoError)
					{
						if(IsVoltageReady)
							CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_StopOutVoltage);
						else if(CONTROL_TimeCounter > Timeout)
							LOGIC_HandleLeakageExecResult(ER_ChangeStateTimeout);
						else if(LOGIC_IsLeakagelInProblem())
						{
							Problem = PROBLEM_LEAKAGE_NODE;
							CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_UnCommutate);
						}
					}
					else
						LOGIC_HandleLeakageExecResult(res);
				}
				break;

			case DSS_Leakage_StopOutVoltage:
				{
					res = LOGIC_StopLeakage();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_WaitStopOutVoltage);
					else
						LOGIC_HandleLeakageExecResult(res);
				}
				break;

			case DSS_Leakage_WaitStopOutVoltage:
				{
					if(IsLeakageNodeReady())
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_StopControl);
				}
				break;

			case DSS_Leakage_StopControl:
				{
					res = LOGIC_StopControl();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_WaitStopControl);
					else
						LOGIC_HandleControlExecResult(res);
				}
				break;

			case DSS_Leakage_WaitStopControl:
				{
					if(COMM_AreSlavesInStateX(CDS_Ready))
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_UnCommutate);
				}
				break;

			case DSS_Leakage_UnCommutate:
				{
					res = MUX_Disconnect();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_WaitUnCommutate);
					else
						CONTROL_SwitchToFault(res, FAULT_EXT_GR_MUX);
				}
				break;

			case DSS_Leakage_WaitUnCommutate:
				{
					if(COMM_AreSlavesInStateX(CDS_Ready))
						CONTROL_SetDeviceState(DS_InProcess, DSS_Leakage_ReadResult);
				}
				break;

			case DSS_Leakage_ReadResult:
				{
					if(Problem == PROBLEM_NONE)
					{
						VIPair Result;
						uint16_t OpResult;
						res = LOGIC_LeakageReadResult(&OpResult, &Result);

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
							LOGIC_HandleLeakageExecResult(res);
					}
					else
					{
						DataTable[REG_OP_RESULT] = OPRESULT_FAIL;
						DataTable[REG_PROBLEM] = Problem;
						CONTROL_SetDeviceState(DS_Ready, DSS_None);
					}
				}
				break;

			default:
				break;
		}
	}
}
//-----------------------------
