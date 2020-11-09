// Header
#include "LogicCalibrateLeakage.h"

// Includes
#include "Logic.h"
#include "Controller.h"
#include "Common.h"
#include "CommonDictionary.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Global.h"
#include "Multiplexer.h"
#include "ACVoltageBoard.h"
#include "DCHighVoltageBoard.h"

// Functions
void CLEAK_HandleMeasurement()
{
	static Int64U Timeout;
	static uint16_t Problem = PROBLEM_NONE;

	if(CONTROL_State == DS_InProcess)
	{
		LOGIC_Wrapper_FaultControl();

		switch(CONTROL_SubState)
		{
			case DSS_CalLeakage_StartTest:
				Problem = PROBLEM_NONE;
				LOGIC_Wrapper_Start(DSS_CalLeakage_Commutate);
				break;

			case DSS_CalLeakage_Commutate:
				LOGIC_Wrapper_Commutate(DSS_CalLeakage_WaitCommutation, DSS_CalLeakage_ReadResult, &Problem);
				break;

			case DSS_CalLeakage_WaitCommutation:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_CalLeakage_StartOutVoltage);
				break;

			case DSS_CalLeakage_StartOutVoltage:
				LOGIC_Wrapper_StartLeakage(DSS_CalLeakage_WaitOutVoltageReady, DSS_CalLeakage_UnCommutate,
						&Timeout, &Problem);
				break;

			case DSS_CalLeakage_WaitOutVoltageReady:
				LOGIC_Wrapper_IsLeakageReady(DSS_CalLeakage_StopOutVoltage, DSS_CalLeakage_StopOutVoltage,
						&Timeout, &Problem);
				break;

			case DSS_CalLeakage_StopOutVoltage:
				LOGIC_Wrapper_StopLeakage(DSS_CalLeakage_WaitStopOutVoltage);
				break;

			case DSS_CalLeakage_WaitStopOutVoltage:
				LOGIC_Wrapper_WaitLeakageFinished(DSS_CalLeakage_UnCommutate, Timeout);
				break;

			case DSS_CalLeakage_UnCommutate:
				LOGIC_Wrapper_UnCommutate(DSS_CalLeakage_WaitUnCommutate);
				break;

			case DSS_CalLeakage_WaitUnCommutate:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_CalLeakage_ReadResult);
				break;

			case DSS_CalLeakage_ReadResult:
				{
					if(Problem == PROBLEM_NONE)
					{
						VIPair Result;
						uint16_t OpResult;

						ExecutionResult res = LOGIC_LeakageReadResult(&OpResult, &Result);

						if(res == ER_NoError)
						{
							if(OpResult == OPRESULT_OK)
							{
								DataTable[REG_OP_RESULT] = OPRESULT_OK;
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
