// Header
#include "LogicCalibrate.h"

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
void CALIBRATE_HandleMeasurement()
{
	static Int64U Timeout;
	static uint16_t Problem = PROBLEM_NONE;

	if(CONTROL_State == DS_InProcess)
	{
		LOGIC_Wrapper_FaultControl();

		switch(CONTROL_SubState)
		{
			case DSS_Calibrate_StartTest:
				Problem = PROBLEM_NONE;
				LOGIC_Wrapper_Start(DSS_Calibrate_Commutate);
				break;

			case DSS_Calibrate_Commutate:
				LOGIC_Wrapper_Commutate(DSS_Calibrate_WaitCommutation, DSS_Calibrate_ReadResult, &Problem);
				break;

			case DSS_Calibrate_WaitCommutation:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Calibrate_StartOutVoltage);
				break;

			case DSS_Calibrate_StartOutVoltage:
				/*LOGIC_Wrapper_StartLeakage(DSS_Calibrate_WaitOutVoltageReady, DSS_Calibrate_UnCommutate,
						&Timeout, &Problem);*/
				break;

			case DSS_Calibrate_WaitOutVoltageReady:
				/*LOGIC_Wrapper_IsLeakageReady(DSS_Calibrate_StopOutVoltage, DSS_Calibrate_StopOutVoltage,
						&Timeout, &Problem);*/
				break;

			case DSS_Calibrate_StopOutVoltage:
				//LOGIC_Wrapper_StopLeakage(DSS_Calibrate_WaitStopOutVoltage);
				break;

			case DSS_Calibrate_WaitStopOutVoltage:
				//LOGIC_Wrapper_WaitLeakageFinished(DSS_Calibrate_UnCommutate, Timeout);
				break;

			case DSS_Calibrate_UnCommutate:
				LOGIC_Wrapper_UnCommutate(DSS_Calibrate_WaitUnCommutate);
				break;

			case DSS_Calibrate_WaitUnCommutate:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Calibrate_ReadResult);
				break;

			case DSS_Calibrate_ReadResult:
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
