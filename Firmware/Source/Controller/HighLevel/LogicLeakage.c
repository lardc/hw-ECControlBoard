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
		LOGIC_Wrapper_FaultControl();

		switch(CONTROL_SubState)
		{
			case DSS_Leakage_StartTest:
				Problem = PROBLEM_NONE;
				LOGIC_Wrapper_Start(DSS_Leakage_Commutate);
				break;

			case DSS_Leakage_Commutate:
				LOGIC_Wrapper_Commutate(DSS_Leakage_WaitCommutation, DSS_Leakage_ReadResult, &Problem);
				break;

			case DSS_Leakage_WaitCommutation:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Leakage_StartControl);
				break;

			case DSS_Leakage_StartControl:
				LOGIC_Wrapper_StartControl(DSS_Leakage_WaitControlReady, DSS_Leakage_UnCommutate,
						&Timeout, &Problem);
				break;

			case DSS_Leakage_WaitControlReady:
				LOGIC_Wrapper_IsControlReady(DSS_Leakage_SetControlDelay, DSS_Leakage_StopControl,
						&Timeout, &Problem);
				break;

			case DSS_Leakage_SetControlDelay:
				LOGIC_Wrapper_ControlSetDelay(DSS_Leakage_WaitControlDelay, DSS_Leakage_StartOutVoltage, &Timeout);
				break;

			case DSS_Leakage_WaitControlDelay:
				LOGIC_Wrapper_SetStateAfterDelay(DSS_Leakage_CheckReadyAfterDelay, Timeout);
				break;

			case DSS_Leakage_CheckReadyAfterDelay:
				LOGIC_Wrapper_IsControlReady(DSS_Leakage_StartOutVoltage, DSS_Leakage_StopControl,
						NULL, &Problem);
				break;
			/*
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
			*/
			default:
				break;
		}
	}
}
//-----------------------------
