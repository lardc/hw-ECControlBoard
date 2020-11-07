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
	static Int64U Timeout = 0;
	static uint16_t Problem = PROBLEM_NONE;

	if(CONTROL_State == DS_InProcess)
	{
		LOGIC_Wrapper_FaultControl();

		switch(CONTROL_SubState)
		{
			case DSS_OnVoltage_StartTest:
				Problem = PROBLEM_NONE;
				LOGIC_Wrapper_Start(DSS_OnVoltage_Commutate);
				break;

			case DSS_OnVoltage_Commutate:
				LOGIC_Wrapper_Commutate(DSS_OnVoltage_WaitCommutation, DSS_OnVoltage_ReadResult, &Problem);
				break;

			case DSS_OnVoltage_WaitCommutation:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_OnVoltage_StartControl);
				break;

			case DSS_OnVoltage_StartControl:
				LOGIC_Wrapper_StartControl(DSS_OnVoltage_WaitControlReady, DSS_OnVoltage_UnCommutate,
						&Timeout, &Problem);
				break;

			case DSS_OnVoltage_WaitControlReady:
				LOGIC_Wrapper_IsControlReady(DSS_OnVoltage_SetControlDelay, DSS_OnVoltage_StopControl,
						&Timeout, &Problem);
				break;

			case DSS_OnVoltage_SetControlDelay:
				LOGIC_Wrapper_ControlSetDelay(DSS_OnVoltage_WaitControlDelay, DSS_OnVoltage_PulseCurrent, &Timeout);
				break;

			case DSS_OnVoltage_WaitControlDelay:
				LOGIC_Wrapper_SetStateAfterDelay(DSS_OnVoltage_CheckReadyAfterDelay, Timeout);
				break;

			case DSS_OnVoltage_CheckReadyAfterDelay:
				LOGIC_Wrapper_IsControlReady(DSS_OnVoltage_PulseCurrent, DSS_OnVoltage_StopControl,
						NULL, &Problem);
				break;

			case DSS_OnVoltage_PulseCurrent:
				LOGIC_Wrapper_PulseCurrent(DSS_OnVoltage_WaitCurrentReady, DSS_OnVoltage_StopControl,
						&Timeout, &Problem);
				break;

			case DSS_OnVoltage_WaitCurrentReady:
				LOGIC_Wrapper_WaitCurrentFinished(DSS_OnVoltage_StopControl, Timeout);
				break;

			case DSS_OnVoltage_StopControl:
				LOGIC_Wrapper_StopControl(DSS_OnVoltage_WaitStopControl);
				break;

			case DSS_OnVoltage_WaitStopControl:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_OnVoltage_UnCommutate);
				break;

			case DSS_OnVoltage_UnCommutate:
				LOGIC_Wrapper_UnCommutate(DSS_OnVoltage_WaitUnCommutate);
				break;

			case DSS_OnVoltage_WaitUnCommutate:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_OnVoltage_ReadResult);
				break;

			case DSS_OnVoltage_ReadResult:
				{
					if(Problem == PROBLEM_NONE)
					{
						VIPair ControlResult;
						uint16_t ControlOpResult;

						ExecutionResult res = CURR_ReadResult();
						if(res == ER_NoError)
							res = LOGIC_ControlReadResult(&ControlOpResult, &ControlResult);

						if(res == ER_NoError)
						{
							pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_DCCurrent);
							pCurrentBoardObject Settings = (pCurrentBoardObject)NodeData->Settings;

							if(NodeData->OpResult == OPRESULT_OK)
							{
								DataTable[REG_OP_RESULT] = OPRESULT_OK;

								DT_Write32(REG_RESULT_ON_VOLTAGE, REG_RESULT_ON_VOLTAGE_32, Settings->Result.Voltage);
								DT_Write32(REG_RESULT_ON_CURRENT, REG_RESULT_ON_CURRENT_32, Settings->Result.Current);

								DT_Write32(REG_RESULT_CONTROL_VOLTAGE, REG_RESULT_CONTROL_VOLTAGE_32, ControlResult.Voltage);
								DT_Write32(REG_RESULT_CONTROL_CURRENT, REG_RESULT_CONTROL_CURRENT_32, ControlResult.Current);
							}
							else
							{
								DataTable[REG_OP_RESULT] = OPRESULT_FAIL;
								DataTable[REG_PROBLEM] = PROBLEM_CURRENT_NODE;
							}

							CONTROL_SetDeviceState(DS_Ready, DSS_None);
						}
						else
							LOGIC_HandleCurrentExecResult(res);
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
