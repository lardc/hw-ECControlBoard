// Header
#include "LogicInhibit.h"

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

// Functions
void INHIBIT_HandleMeasurement()
{
	if(CONTROL_State == DS_InProcess)
	{
		if(COMM_IsSlaveInFaultOrDisabled())
			CONTROL_SwitchToFault(ER_WrongState, FAULT_EXT_GR_COMMON);

		switch(CONTROL_SubState)
		{
			case DSS_Inhibit_StartTest:
				if(COMM_AreSlavesInStateX(CDS_Ready))
					CONTROL_SetDeviceState(DS_InProcess, DSS_Inhibit_ReadResult);
				else
					CONTROL_SwitchToFault(ER_WrongState, FAULT_EXT_GR_COMMON);
				break;

			case DSS_Inhibit_ReadResult:
				{
					DataTable[REG_OP_RESULT] = OPRESULT_OK;
					DataTable[REG_RESULT_INHIBIT_VOLTAGE] = 228;
					CONTROL_SetDeviceState(DS_Ready, DSS_None);
				}
				break;

			default:
				break;
		}
	}
}
//-----------------------------
