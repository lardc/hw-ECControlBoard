// Header
#include "LogicLeakage.h"

// Includes
#include "Logic.h"
#include "Controller.h"
#include "CommonDictionary.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Global.h"

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

			case DSS_Leakage_StartOutVoltage:
				LOGIC_Wrapper_StartLeakage(DSS_Leakage_WaitOutVoltageReady, DSS_Leakage_StopControl,
						&Timeout, &Problem);
				break;

			case DSS_Leakage_WaitOutVoltageReady:
				LOGIC_Wrapper_IsLeakageReady(DSS_Leakage_StopOutVoltage, DSS_Leakage_StopOutVoltage,
						&Timeout, &Problem);
				break;

			case DSS_Leakage_StopOutVoltage:
				LOGIC_Wrapper_StopLeakage(DSS_Leakage_WaitStopOutVoltage);
				break;

			case DSS_Leakage_WaitStopOutVoltage:
				LOGIC_Wrapper_WaitLeakageFinished(DSS_Leakage_StopControl, Timeout);
				break;

			case DSS_Leakage_StopControl:
				LOGIC_Wrapper_StopControl(DSS_Leakage_WaitStopControl);
				break;

			case DSS_Leakage_WaitStopControl:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Leakage_UnCommutate);
				break;

			case DSS_Leakage_UnCommutate:
				LOGIC_Wrapper_UnCommutate(DSS_Leakage_WaitUnCommutate);
				break;

			case DSS_Leakage_WaitUnCommutate:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Leakage_ReadResult);
				break;

			case DSS_Leakage_ReadResult:
				{
					if(Problem == PROBLEM_NONE)
					{
						VIPair Result, ControlResult;
						uint16_t OpResult, ControlOpResult;

						ExecutionResult res = LOGIC_LeakageReadResult(&OpResult, &Result);
						if(res == ER_NoError)
							res = LOGIC_ControlReadResult(&ControlOpResult, &ControlResult);

						if(res == ER_NoError)
						{
							if(OpResult == OPRESULT_OK)
							{
								DataTable[REG_OP_RESULT] = OPRESULT_OK;

								DT_Write32(REG_RESULT_LEAKAGE_VOLTAGE, REG_RESULT_LEAKAGE_VOLTAGE_32, Result.Voltage);
								DT_Write32(REG_RESULT_LEAKAGE_CURRENT, REG_RESULT_LEAKAGE_CURRENT_32, Result.Current);

								DT_Write32(REG_RESULT_CONTROL_VOLTAGE, REG_RESULT_CONTROL_VOLTAGE_32, ControlResult.Voltage);
								DT_Write32(REG_RESULT_CONTROL_CURRENT, REG_RESULT_CONTROL_CURRENT_32, ControlResult.Current);
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
