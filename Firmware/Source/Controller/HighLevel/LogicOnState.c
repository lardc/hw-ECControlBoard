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
	static uint16_t Problem = PROBLEM_NONE;

	if(CONTROL_State == DS_InProcess)
	{
		ExecutionResult res;
		LOGIC_Wrapper_FaultControl();

		switch(CONTROL_SubState)
		{
			case DSS_OnVoltage_StartTest:
				{
					Problem = PROBLEM_NONE;
					LOGIC_Wrapper_Start(DSS_OnVoltage_Commutate);
				}
				break;

			case DSS_OnVoltage_Commutate:
				{
					res = MUX_Connect();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_WaitCommutation);
					else
						CONTROL_SwitchToFault(res, FAULT_EXT_GR_MUX);
				}
				break;

			case DSS_OnVoltage_WaitCommutation:
				{
					if(COMM_AreSlavesInStateX(CDS_Ready))
						CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_StartControl);
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
						else if(LOGIC_IsControlInProblem())
						{
							Problem = PROBLEM_CONTROL_NODE;
							CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_UnCommutate);
						}
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
					if(COMM_IsSlaveInStateX(NAME_DCCurrent, CDS_Ready))
						CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_StopControl);
					else if(CONTROL_TimeCounter > Timeout)
						CONTROL_SwitchToFault(ER_ChangeStateTimeout, FAULT_EXT_GR_DC_CURRENT);
				}
				break;

			case DSS_OnVoltage_StopControl:
				{
					res = LOGIC_StopControl();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_WaitStopControl);
					else
						LOGIC_HandleControlExecResult(res);
				}
				break;

			case DSS_OnVoltage_WaitStopControl:
				{
					if(COMM_AreSlavesInStateX(CDS_Ready))
						CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_UnCommutate);
				}
				break;

			case DSS_OnVoltage_UnCommutate:
				{
					res = MUX_Disconnect();
					if(res == ER_NoError)
						CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_WaitUnCommutate);
					else
						CONTROL_SwitchToFault(res, FAULT_EXT_GR_MUX);
				}
				break;

			case DSS_OnVoltage_WaitUnCommutate:
				{
					if(COMM_AreSlavesInStateX(CDS_Ready))
						CONTROL_SetDeviceState(DS_InProcess, DSS_OnVoltage_ReadResult);
				}
				break;

			case DSS_OnVoltage_ReadResult:
				{
					if(Problem == PROBLEM_NONE)
					{
						VIPair ControlResult;
						uint16_t ControlOpResult;

						res = CURR_ReadResult();
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
							CONTROL_SwitchToFault(res, FAULT_EXT_GR_DC_CURRENT);
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
