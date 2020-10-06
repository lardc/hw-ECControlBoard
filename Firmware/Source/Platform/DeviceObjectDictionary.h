#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// �������
#define ACT_ENABLE_POWER				1	// ������� � ��������� ��������
#define ACT_DISABLE_POWER				2	// ���������� �����
#define ACT_FAULT_CLEAR					3	// ������� fault
#define ACT_WARNING_CLEAR				4	// ������� warning

#define ACT_FAN_ON						48	// �������� ����������
#define ACT_FAN_OFF						49	// ��������� ����������

#define ACT_DBG_PULSE_EXT_SYNC1			50	// ��������� ������� �� ������� ���� SYNC_1
#define ACT_DBG_PULSE_EXT_SYNC2			51	// ��������� ������� �� ������� ���� SYNC_2
#define ACT_DBG_PULSE_INT_SYNC1			52	// ��������� ������� �� ���������� ���� SYNC_1
#define ACT_DBG_PULSE_INT_SYNC2			53	// ��������� ������� �� ���������� ���� SYNC_2
#define ACT_DBG_IS_STATE_INT_SYNC_1		54	// ��������� ���������� ���� SYNC_1
#define ACT_DBG_IS_STATE_INT_SYNC_2		55	// ��������� ���������� ���� SYNC_2
#define ACT_DBG_PULSE_EXT_LED			56	// ��������� ������� �������� ����������

#define ACT_SAVE_TO_ROM					200	// ���������� ���������������� ������ �� FLASH ����������
#define ACT_RESTORE_FROM_ROM			201	// �������������� ������ �� FLASH
#define ACT_RESET_TO_DEFAULT			202	// ����� DataTable � ��������� �� ���������

#define ACT_BOOT_LOADER_REQUEST			320	// ���������� ���������� � ����� ��������������������
// -----------------------------

// ��������
// ����������� ��������
#define REG_SLAVE_NID_MULTIPLEXER		0	// Can NodeID ��� ECDCMultiplexerBoard
#define REG_SLAVE_NID_DC_CURRENT		1	// Can NodeID ��� ECDCCurrentBoard
#define REG_SLAVE_NID_DC_HV				2	// Can NodeID ��� ECDCHighVoltageBoard
#define REG_SLAVE_NID_DC_VOLTAGE1		3	// Can NodeID ��� ECDCVoltageBoard �1
#define REG_SLAVE_NID_DC_VOLTAGE2		4	// Can NodeID ��� ECDCVoltageBoard �2
#define REG_SLAVE_NID_DC_VOLTAGE3		5	// Can NodeID ��� ECDCVoltageBoard �3
#define REG_SLAVE_NID_AC_VOLTAGE1		6	// Can NodeID ��� ECACVoltageBoard �1
#define REG_SLAVE_NID_AC_VOLTAGE2		7	// Can NodeID ��� ECACVoltageBoard �2
// 8 - 9
#define REG_EMULATE_MULTIPLEXER			10	// ����� �������� ��� ECDCMultiplexerBoard
#define REG_EMULATE_DC_CURRENT			11	// ����� �������� ��� ECDCCurrentBoard
#define REG_EMULATE_DC_HV				12	// ����� �������� ��� ECDCHighVoltageBoard
#define REG_EMULATE_DC_VOLTAGE1			13	// ����� �������� ��� ECDCVoltageBoard �1
#define REG_EMULATE_DC_VOLTAGE2			14	// ����� �������� ��� ECDCVoltageBoard �2
#define REG_EMULATE_DC_VOLTAGE3			15	// ����� �������� ��� ECDCVoltageBoard �3
#define REG_EMULATE_AC_VOLTAGE1			16	// ����� �������� ��� ECACVoltageBoard �1
#define REG_EMULATE_AC_VOLTAGE2			17	// ����� �������� ��� ECACVoltageBoard �2
// 18 - 19
#define REG_POWER_ENABLE_TIMEOUT		20	// ������� �������� ��������� ������� (� ���.)

// ������������ �������� ������-������
#define REG_MEASUREMENT_TYPE			128	// ��� ������������ ���������
#define REG_DUT_CASE_TYPE				129	// ��� ������� ����������� �������
#define REG_DUT_POSITION_NUMBER			130	// ����� ������� �� �������
#define REG_INPUT_CONTROL_TYPE			131	// ��� �������� ������� ����������
#define REG_I_LEAK_VOLTAGE_TYPE			132	// ��� ��������� ���������� ��������� ���� ������

#define REG32S_INPUT_VOLTAGE			135	// ���������� ����� ����������
#define REG32S_INPUT_VOLTAGE_32			136
#define REG32S_INPUT_CURRENT			137	// ��� ����� ����������
#define REG32S_INPUT_CURRENT_32			138

#define REG32S_OUTPUT_VOLTAGE			139	// ���������� ����� ����������
#define REG32S_OUTPUT_VOLTAGE_32		140
#define REG32S_OUTPUT_CURRENT			141	// ��� ����� ����������
#define REG32S_OUTPUT_CURRENT_32		142

#define REG_AUX_PS1_VOLTAGE				143	// ���������� ���������������� ��������� ������� 1
#define REG_AUX_PS1_CURRENT				144	// ��� ���������������� ��������� ������� 1
#define REG_AUX_PS2_VOLTAGE				145	// ���������� ���������������� ��������� ������� 2
#define REG_AUX_PS2_CURRENT				146	// ��� ���������������� ��������� ������� 2

#define REG_DBG_STATE					190	// ������� ������ �������

// �������� ������ ������
#define REG_DEV_STATE					192	// ������� ���������
#define REG_FAULT_REASON				193	// ������� Fault
#define REG_DISABLE_REASON				194	// ������� Disable
#define REG_WARNING						195	// ������� Warning
#define REG_PROBLEM						196	// ������� Problem
#define REG_OP_RESULT					197	// ������� ���������� ��������
#define REG_DEV_SUB_STATE				198	// ������� ���������������� ���������

#define REG_BHL_ERROR_CODE				210	// ������ ���������� ��: ��� ������
#define REG_BHL_DEVICE					211	// ������ ���������� ��: NID ����������
#define REG_BHL_FUNCTION				212	// ������ ���������� ��: ��� �������
#define REG_BHL_EXT_DATA				213	// ������ ���������� ��: ����������� ����������

#define REG_DBG_INT_SYNC1				251	// ������� ��������� ����������� ����� SYNC1
#define REG_DBG_INT_SYNC2				252	// ������� ��������� ����������� ����� SYNC2
#define REG_FAN_STATE					253	// ������� ���������� ������������
// -----------------------------

// Operation results
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

//  Fault and disable codes
#define DF_NONE							0
#define DF_INTERFACE					1	// ������ ������-����������
#define DF_LOGIC_WRONG_STATE			2	// ������� ���� ��������� � ������������ ���������
#define DF_LOGIC_STATE_TIMEOUT			3	// ������� ����� ��������� ������� �����

// Problem
#define PROBLEM_NONE					0

//  Warning
#define WARNING_NONE					0

//  User Errors
#define ERR_NONE						0
#define ERR_CONFIGURATION_LOCKED		1	//  ���������� �������� �� ������
#define ERR_OPERATION_BLOCKED			2	//  �������� �� ����� ���� ��������� � ������� ��������� ����������
#define ERR_DEVICE_NOT_READY			3	//  ���������� �� ������ ��� ����� ���������
#define ERR_WRONG_PWD					4	//  ������������ ����

#endif //  __DEV_OBJ_DIC_H
