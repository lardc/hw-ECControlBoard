#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// �������
#define ACT_ENABLE_POWER				1	// ������� � ��������� ��������
#define ACT_DISABLE_POWER				2	// ���������� �����
#define ACT_FAULT_CLEAR					3	// ������� fault
#define ACT_WARNING_CLEAR				4	// ������� warning
#define ACT_SAFETY_CLEAR				5	// ������� ��������� ������� ������������

#define ACT_DIAG_READ_REG				10	// ������ �������� slave-���������� �� CAN-����
#define ACT_DIAG_WRITE_REG				11	// ������ �������� slave-���������� �� CAN-����
#define ACT_DIAG_CALL					12	// ����� ������� slave-���������� �� CAN-����
#define ACT_DIAG_READ_EP				13	// ������ EP slave-���������� �� CAN-����

#define ACT_DBG_PULSE_EXT_SYNC1			50	// ��������� ������� �� ������� ���� SYNC_1
#define ACT_DBG_PULSE_EXT_SYNC2			51	// ��������� ������� �� ������� ���� SYNC_2
#define ACT_DBG_PULSE_INT_SYNC1			52	// ��������� ������� �� ���������� ���� SYNC_1
#define ACT_DBG_PULSE_INT_SYNC2			53	// ��������� ������� �� ���������� ���� SYNC_2
#define ACT_DBG_IS_STATE_INT_SYNC_1		54	// ��������� ���������� ���� SYNC_1
#define ACT_DBG_IS_STATE_INT_SYNC_2		55	// ��������� ���������� ���� SYNC_2
#define ACT_DBG_PULSE_EXT_LED			56	// ��������� ������� �������� ����������
#define ACT_FAN_ON						57	// �������� ����������
#define ACT_FAN_OFF						58	// ��������� ����������

#define ACT_START_TEST					100	// ������ ���������
#define ACT_STOP_TEST					101	// ������ ���������
#define ACT_SAFETY_SET_ACTIVE			102	// ��������� ������� ������������
#define ACT_SAFETY_SET_INACTIVE			103	// ����������� ������� ������������
#define ACT_CALIBRATE					104	// ������ ����������

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
#define REG_GENERAL_LOGIC_TIMEOUT		21	// ������� �������� ������� ���������� �������� (� ���.)
#define REG_FAN_OPERATE_PERIOD			22	// ������ ��������� ����������� �� ������� (� ���.)
#define REG_FAN_OPERATE_MIN_TIME		23	// ����������� ����� ������ ����������� (� ���.)
#define REG_ALTER_SCAN_STATE_DELAY		24	// ������������ �������������� �������� ������ ��������� (� ��)

// ������������ �������� ������-������
#define REG_MEASUREMENT_TYPE			128	// ��� ������������ ���������
#define REG_DUT_CASE_TYPE				129	// ��� ������� ����������� �������
#define REG_DUT_POSITION_NUMBER			130	// ����� ������� �� �������
#define REG_INPUT_CONTROL_TYPE			131	// ��� �������� ������� ����������

#define REG_CONTROL_VOLTAGE				132	// ���������� ����� ����������
#define REG_CONTROL_CURRENT				133	// ��� ����� ����������

#define REG_COMM_VOLTAGE_TYPE_LEAKAGE	134	// ��� �������������� ���������� ��� ��������� ������
#define REG_COMM_POLARITY				135	// ���������� ���������� ����������� �������������� ����������
#define REG_COMM_CURRENT_SHAPE			136	// ����� �������� �������������� ����
#define REG_COMM_CURRENT_TIME			137	// ������������ �������� �������������� ����
#define REG_COMM_CURRENT				138	// ������������� ��� (���)
#define REG_COMM_VOLTAGE				139	// �������������� ���������� (���)

#define REG_AUX_PS1_VOLTAGE				140	// ���������� ���������������� ��������� ������� 1
#define REG_AUX_PS1_CURRENT				141	// ��� ���������������� ��������� ������� 1
#define REG_AUX_PS2_VOLTAGE				142	// ���������� ���������������� ��������� ������� 2
#define REG_AUX_PS2_CURRENT				143	// ��� ���������������� ��������� ������� 2

#define REG_MODE_OUTPUT_RES				144	// ��������� ������������� �������� ���� (1 - ������������ �����)

#define REG_CTRL_PULSE_LENGTH			145	// ������������ �������� ���������� � ���������� ������ (� ��)
#define REG_CTRL_HOLD_DELAY				146	// �������� ��� ����������� �������� ����� ���������� ���� ���������� (� ��)

#define REG_CALIBRATION_HOLD_DELAY		147	// ����� ��������� ������� ����� ���������� ��� ���������� (� ��)

// ������� ����� 32������ ����� �������
#define REG_CONTROL_VOLTAGE_32			150	// ���������� ����� ����������
#define REG_CONTROL_CURRENT_32			151	// ��� ����� ����������
#define REG_COMM_CURRENT_32				152	// ������������� ��� (���)
#define REG_COMM_VOLTAGE_32				153	// �������������� ���������� (���)
#define REG_AUX_PS1_VOLTAGE_32			154	// ���������� ���������������� ��������� ������� 1
#define REG_AUX_PS1_CURRENT_32			155	// ��� ���������������� ��������� ������� 1
#define REG_AUX_PS2_VOLTAGE_32			156	// ���������� ���������������� ��������� ������� 2
#define REG_AUX_PS2_CURRENT_32			157	// ��� ���������������� ��������� ������� 2

#define REG_CALIBRATION_NODE			160	// ����������� ����
#define REG_CALIBRATION_TYPE			161	// ��� ����������
#define REG_CALIBRATION_VSET			162	// ������� ���������� ����������
#define REG_CALIBRATION_VSET_32			163
#define REG_CALIBRATION_ISET			164	// ������� ���� ����������
#define REG_CALIBRATION_ISET_32			165

#define REG_DIAG_NID					185	// CAN: ������� NodeID ��� ������� � slave-�����������
#define REG_DIAG_IN_1					186	// CAN: ������� ���������� ������� 1
#define REG_DIAG_IN_2					187	// CAN: ������� ���������� ������� 2
#define REG_DIAG_FAN_MUTE				188	// ���������� �����������
#define REG_DIAG_SAFETY_MUTE			189	// ���������� ������� ������������

#define REG_DBG_STATE					190	// ������� ������ �������

// �������� ������ ������
#define REG_DEV_STATE					192	// ������� ���������
#define REG_FAULT_REASON				193	// ������� Fault
#define REG_DISABLE_REASON				194	// ������� Disable
#define REG_WARNING						195	// ������� Warning
#define REG_PROBLEM						196	// ������� Problem
#define REG_OP_RESULT					197	// ������� ���������� ��������

#define REG_RESULT_LEAKAGE_CURRENT		198	// ��� ������ �� ������ (� ���)
#define REG_RESULT_ON_VOLTAGE			199	// ���������� ���������� �� ������ (� ���)
#define REG_RESULT_CONTROL_CURRENT		200	// ��� ���������� (� ���)
#define REG_RESULT_CONTROL_VOLTAGE		201	// ���������� ���������� (� ���)
#define REG_RESULT_INHIBIT_VOLTAGE		202	// ���������� ������� (� ���)
#define REG_RESULT_AUX_CURRENT1			203	// ��� ���������������� ��������� 1 (� ���)
#define REG_RESULT_AUX_CURRENT2			204	// ��� ���������������� ��������� 2 (� ���)
#define REG_RESULT_OUTPUT_RES			205	// �������� ������������� (� ����)

#define REG_RESULT_LEAKAGE_VOLTAGE		206	// ���������� ��������� ������ (� ���)
#define REG_RESULT_LEAKAGE_VOLTAGE_32	207
#define REG_RESULT_ON_CURRENT			208	// ��� ��������� ����������� ���������� (� ���)
#define REG_RESULT_ON_CURRENT_32		209

#define REG_DEV_SUB_STATE				220	// ������� ���������������� ���������
#define REG_FAILED_DEV_SUB_STATE		221	// ������� ������������ ���������������� ���������
#define REG_CONFIG_ERR					222	// ������� �������� ���� ������ ����������������

#define REG_DIAG_OUT_1					223	// CAN: ���������� �������� ������� 1
#define REG_DIAG_OUT_2					224	// CAN: ���������� �������� ������� 2

#define REG_BHL_ERROR_CODE				225	// ������ ���������� ��: ��� ������
#define REG_BHL_DEVICE					226	// ������ ���������� ��: NID ����������
#define REG_BHL_FUNCTION				227	// ������ ���������� ��: ��� �������
#define REG_BHL_EXT_DATA				228	// ������ ���������� ��: ����������� ����������
#define REG_BHL_DETAILS					229	// ������ ���������� ��: ������

// ������� ����� 32������ ����� �����������
#define REG_RESULT_LEAKAGE_CURRENT_32	230	// ��� ������ �� ������ (������� �����) (� ���)
#define REG_RESULT_ON_VOLTAGE_32		231	// ���������� ���������� �� ������ (������� �����) (� ���)
#define REG_RESULT_CONTROL_CURRENT_32	232	// ��� ���������� (������� �����) (� ���)
#define REG_RESULT_CONTROL_VOLTAGE_32	233	// ���������� ���������� (������� �����) (� ���)
#define REG_RESULT_INHIBIT_VOLTAGE_32	234	// ���������� ������� (������� �����) (� ���)
#define REG_RESULT_AUX_CURRENT1_32		235	// ��� ���������������� ��������� 1 (������� �����) (� ���)
#define REG_RESULT_AUX_CURRENT2_32		236	// ��� ���������������� ��������� 2 (������� �����) (� ���)
#define REG_RESULT_OUTPUT_RES_32		237	// �������� ������������� (������� �����) (� ����)

#define REG_CALIBRATION_VOLTAGE			240	// ��������� ����������: ���������� (� ���)
#define REG_CALIBRATION_VOLTAGE_32		241
#define REG_CALIBRATION_CURRENT			242	// ��������� ����������: ��� (� ���)
#define REG_CALIBRATION_CURRENT_32		243

#define REG_RESULT_AUX_VOLTAGE1			244	// ���������� ���������������� ��������� 1 (� ���)
#define REG_RESULT_AUX_VOLTAGE1_32		245
#define REG_RESULT_AUX_VOLTAGE2			246	// ���������� ���������������� ��������� 2 (� ���)
#define REG_RESULT_AUX_VOLTAGE2_32		247

#define REG_DBG_INT_SYNC1				251	// ������� ��������� ����������� ����� SYNC1
#define REG_DBG_INT_SYNC2				252	// ������� ��������� ����������� ����� SYNC2
#define REG_FAN_STATE					253	// ������� ���������� ������������
// -----------------------------
#define REG_FWINFO_SLAVE_NID			256	// Device CAN slave node ID
#define REG_FWINFO_MASTER_NID			257	// Device CAN master node ID (if presented)
// 258 - 259
#define REG_FWINFO_STR_LEN				260	// Length of the information string record
#define REG_FWINFO_STR_BEGIN			261	// Begining of the information string record

// Operation results
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

// Fault and disable codes
#define DF_NONE							0

// ������ ����� ������ ����������
// ��� ������ ������� ��� FAULT_EXT_GR + ExecutionResult
#define FAULT_EXT_GR_COMMON				0	// ����� ������
#define FAULT_EXT_GR_MUX				100	// Multiplexer
#define FAULT_EXT_GR_DC_CURRENT			200	// DCCurrentBoard
#define FAULT_EXT_GR_DC_HV				300	// DCHighVoltageBoard
#define FAULT_EXT_GR_DC_VOLTAGE1		400	// DCVoltageBoard1
#define FAULT_EXT_GR_DC_VOLTAGE2		500	// DCVoltageBoard2
#define FAULT_EXT_GR_DC_VOLTAGE3		600	// DCVoltageBoard3
#define FAULT_EXT_GR_AC_VOLTAGE1		700	// ACVoltageBoard1
#define FAULT_EXT_GR_AC_VOLTAGE2		800	// ACVoltageBoard2
#define FAULT_EXT_GR_POWER_SUPPLY		900	// PowerSupply

// Problem
#define PROBLEM_NONE					0
#define PROBLEM_COMMAND_STOP			1	// �������� ������� ������ �� ���������
#define PROBLEM_SAFETY_TRIG				2	// ��������� �� ������������ ������� ������������

#define PROBLEM_MUX_CONFIG				10	// ������������ ������������ ��������������
#define PROBLEM_MUX_FAST_CONFIG			11	// ������������ ������������ ������� ���������� ��������������

#define PROBLEM_CAL_CONFIG				20	// ������������ ������������ ������������ ����
#define PROBLEM_CAL_READY_TIMEOUT		21	// ���������� ������� ���������� ������������ ����
#define PROBLEM_CAL_IN_PROBLEM			22	// ����������� ���� � ��������
#define PROBLEM_CAL_RESULT				23	// ���������� ��������� ���������

#define PROBLEM_CONTROL_CONFIG			30	// ������������ ������������ ���� ����������
#define PROBLEM_CONTROL_READY_TIMEOUT	31	// ���������� ������� ���������� ����������
#define PROBLEM_CONTROL_IN_PROBLEM		32	// �������� ������� ���������� � ��������
#define PROBLEM_CONTROL_RESULT			33	// ���������� ��������� ���������

#define PROBLEM_CURRENT_CONFIG			40	// ������������ ������������ ��������� ����
#define PROBLEM_CURRENT_RESULT			41	// ���������� ��������� ���������

#define PROBLEM_LEAKAGE_CONFIG			50	// ������������ ������������ ��������� ���������� ������
#define PROBLEM_LEAKAGE_READY_TIMEOUT	51	// ���������� ������� ���������� ������
#define PROBLEM_LEAKAGE_IN_PROBLEM		52	// �������� ������� ������ � ��������
#define PROBLEM_LEAKAGE_RESULT			53	// ���������� ��������� ���������
#define PROBLEM_LEAKAGE_NEXT_CONFIG		54	// ������������ ������������ ��������� ���������� ������ (��������� �����)
#define PROBLEM_LEAKAGE_NEXT_TIMEOUT	55	// ���������� ������� ���������� ������ (��������� �����)
#define PROBLEM_LEAKAGE_NEXT_IN_PROBLEM	56	// �������� ������� ������ � �������� (��������� �����)

#define PROBLEM_PS_CONFIG				60	// ������������ ������������ ���������������� �������
#define PROBLEM_PS_READY_TIMEOUT		61	// ���������� ������� ���������� ���������������� �������
#define PROBLEM_PS_IN_PROBLEM			62	// ���������������� ������� � ��������
#define PROBLEM_PS_RESULT				63	// ���������� ��������� ���������

//  Warning
#define WARNING_NONE					0

// Endpoints
#define EP_SLAVE_DATA					1	// ������ ���������� �� Slave-���������

//  User Errors
#define ERR_NONE						0
#define ERR_CONFIGURATION_LOCKED		1	//  ���������� �������� �� ������
#define ERR_OPERATION_BLOCKED			2	//  �������� �� ����� ���� ��������� � ������� ��������� ����������
#define ERR_DEVICE_NOT_READY			3	//  ���������� �� ������ ��� ����� ���������
#define ERR_WRONG_PWD					4	//  ������������ ����
#define ERR_BAD_CONFIG					5	//  ������� ������� ��������� � �������� �������������

#endif //  __DEV_OBJ_DIC_H
