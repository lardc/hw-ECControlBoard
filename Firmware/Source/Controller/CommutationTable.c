// Header
#include "CommutationTable.h"

const RegisterPin* const COMM_SimpleRelayArray[] = {
		&COMM_SwitchPOTPlusToPOT1,					// 0
		&COMM_SwitchPOTPlusToPOT2,					// 1
		&COMM_SwitchPOTPlusToPOT3,					// 
		&COMM_SwitchPOTPlusToPOT4,					// 
		&COMM_SwitchPOTPlusToPOT5,					// 
		&COMM_SwitchPOTPlusToPOT6,					// 

		&COMM_SwitchPOTMinusToPOT1,					// 
		&COMM_SwitchPOTMinusToPOT2,					// 
		&COMM_SwitchPOTMinusToPOT3,					// 
		&COMM_SwitchPOTMinusToPOT4,					// 
		&COMM_SwitchPOTMinusToPOT5,					// 
		&COMM_SwitchPOTMinusToPOT6,					// 

		&COMM_SwitchPOTOutToPOT1,					// 
		&COMM_SwitchPOTOutToPOT2,					// 
		&COMM_SwitchPOTOutToPOT3,					// 
		&COMM_SwitchPOTOutToPOT4,					// 
		&COMM_SwitchPOTOutToPOT5,					// 
		&COMM_SwitchPOTOutToPOT6,					// 

		&COMM_SwitchBUSHVPlusToPOW1,				// 
		&COMM_SwitchBUSHVPlusToPOW2,				// 
		&COMM_SwitchBUSHVPlusToPOW3,				// 
		&COMM_SwitchBUSHVPlusToPOW4,				// 
		&COMM_SwitchBUSHVPlusToPOW5,				// 
		&COMM_SwitchBUSHVPlusToPOW6,				// 

		&COMM_SwitchBUSHVMinusToPOW1,				// 
		&COMM_SwitchBUSHVMinusToPOW2,				// 
		&COMM_SwitchBUSHVMinusToPOW3,				// 
		&COMM_SwitchBUSHVMinusToPOW4,				// 
		&COMM_SwitchBUSHVMinusToPOW5,				// 
		&COMM_SwitchBUSHVMinusToPOW6,				// 

		&COMM_SwitchCtrl1ToCtrl1,					// 
		&COMM_SwitchCtrl1ToCtrl2,					// 
		&COMM_SwitchCtrl1ToCtrl3,					// 
		&COMM_SwitchCtrl1ToCtrl4,					// 
		&COMM_SwitchCtrl2ToCtrl1,					// 
		&COMM_SwitchCtrl2ToCtrl2,					// 
		&COMM_SwitchCtrl2ToCtrl3,					// 
		&COMM_SwitchCtrl2ToCtrl4,					// 

		&COMM_SwitchPotCtrlPlusToCtrlPot1,			// 
		&COMM_SwitchPotCtrlPlusToCtrlPot2,			// 
		&COMM_SwitchPotCtrlPlusToCtrlPot3,			// 
		&COMM_SwitchPotCtrlPlusToCtrlPot4,			// 
		&COMM_SwitchPotCtrlMinusToCtrlPot1,			// 
		&COMM_SwitchPotCtrlMinusToCtrlPot2,			// 
		&COMM_SwitchPotCtrlMinusToCtrlPot3,			// 
		&COMM_SwitchPotCtrlMinusToCtrlPot4			// 
};

const BistableSwitch* const COMM_BistableRelayArray[] = {
		&COMM_SwitchBUSLVPlusToPOW1,				// 0
		&COMM_SwitchBUSLVPlusToPOW2,				// 1
		&COMM_SwitchBUSLVPlusToPOW3,				// 
		&COMM_SwitchBUSLVPlusToPOW4,				// 
		&COMM_SwitchBUSLVPlusToPOW5,				// 
		&COMM_SwitchBUSLVPlusToPOW6,				// 

		&COMM_SwitchBUSLVMinusToPOW1,				// 
		&COMM_SwitchBUSLVMinusToPOW2,				// 
		&COMM_SwitchBUSLVMinusToPOW3,				// 
		&COMM_SwitchBUSLVMinusToPOW4,				// 
		&COMM_SwitchBUSLVMinusToPOW5,				// 
		&COMM_SwitchBUSLVMinusToPOW6				// 
};
