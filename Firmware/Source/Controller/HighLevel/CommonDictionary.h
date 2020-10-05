#ifndef __COMMON_DICTIONARY_H
#define __COMMON_DICTIONARY_H

// Actions
#define ACT_COMM_ENABLE_POWER				1	// ��������� �������
#define ACT_COMM_DISABLE_POWER				2	// ���������� �������
#define ACT_COMM_FAULT_CLEAR				3	// ������� fault
#define ACT_COMM_WARNING_CLEAR				4	// ������� warning

// Registers
#define REG_COMM_DEV_STATE					192	// ������� ���������
#define REG_COMM_FAULT_REASON				193	// ������� Fault
#define REG_COMM_DISABLE_REASON				194	// ������� Disable
#define REG_COMM_WARNING					195	// ������� Warning
#define REG_COMM_PROBLEM					196	// ������� Problem
#define REG_COMM_OP_RESULT					197	// ������� ���������� ��������

#endif // __COMMON_DICTIONARY_H
