// Header
#include "LogicControl.h"

// Includes
#include "Logic.h"
#include "Controller.h"
#include "CommonDictionary.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Global.h"

// Functions
void CTRL_HandleMeasurement()
{
	static Int64U Timeout;
	static uint16_t Problem = PROBLEM_NONE;

	if(CONTROL_State == DS_InProcess)
	{
		LOGIC_Wrapper_FaultControl();
		LOGIC_Wrapper_SafetyMonitor();

		switch(CONTROL_SubState)
		{
			case DSS_Control_StartTest:
				Problem = PROBLEM_NONE;
				LOGIC_Wrapper_Start(DSS_Control_Commutate);
				break;

			case DSS_Control_Commutate:
				LOGIC_Wrapper_Commutate(DSS_Control_WaitCommutation, DSS_Control_ReadResult, &Problem);
				break;

			case DSS_Control_WaitCommutation:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Control_StartPowerSupply);
				break;

			case DSS_Control_StartPowerSupply:
				LOGIC_Wrapper_StartPowerSupply(DSS_Control_WaitStartPowerSupply, DSS_Control_UnCommutate,
						&Timeout, &Problem);
				break;

			case DSS_Control_WaitStartPowerSupply:
				LOGIC_Wrapper_IsPowerSupplyReady(DSS_Control_StartControl, DSS_Control_StopPowerSupply,
						&Timeout, &Problem);
				break;

			case DSS_Control_StartControl:
				LOGIC_Wrapper_StartControl(DSS_Control_WaitControlReady, DSS_Control_StopPowerSupply,
						&Timeout, &Problem);
				break;

			case DSS_Control_WaitControlReady:
				LOGIC_Wrapper_IsControlOutputReady(DSS_Control_StopControl, DSS_Control_StopControl,
						&Timeout, &Problem);
				break;

			case DSS_Control_StopControl:
				LOGIC_Wrapper_StopControl(DSS_Control_WaitStopControl);
				break;

			case DSS_Control_WaitStopControl:
				LOGIC_Wrapper_IsControlNodeReady(DSS_Control_StopPowerSupply);
				break;

			case DSS_Control_StopPowerSupply:
				LOGIC_Wrapper_StopPowerSupply(DSS_Control_WaitStopPowerSupply);
				break;

			case DSS_Control_WaitStopPowerSupply:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Control_UnCommutate);
				break;

			case DSS_Control_UnCommutate:
				LOGIC_Wrapper_UnCommutate(DSS_Control_WaitUnCommutate);
				break;

			case DSS_Control_WaitUnCommutate:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Control_ReadResult);
				break;

			case DSS_Control_ReadResult:
				{
					if(Problem == PROBLEM_NONE)
					{
						VIPair Result;
						uint16_t OpResult;
						ExecutionResult res = LOGIC_ControlReadResult(&OpResult, &Result);

						if(res == ER_NoError)
						{
							if(OpResult == OPRESULT_OK)
							{
								DataTable[REG_OP_RESULT] = OPRESULT_OK;
								DT_Write32(REG_RESULT_CONTROL_VOLTAGE, REG_RESULT_CONTROL_VOLTAGE_32, Result.Voltage);
								DT_Write32(REG_RESULT_CONTROL_CURRENT, REG_RESULT_CONTROL_CURRENT_32, Result.Current);
							}
							else
							{
								DataTable[REG_OP_RESULT] = OPRESULT_FAIL;
								DataTable[REG_PROBLEM] = PROBLEM_CONTROL_NODE;
							}

							CONTROL_SetDeviceState(DS_Ready, DSS_None);
						}
						else
							LOGIC_HandleControlExecResult(res);
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
