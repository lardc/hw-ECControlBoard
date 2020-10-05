#ifndef __COMMON_DICTIONARY_H
#define __COMMON_DICTIONARY_H

// Actions
#define COMM_ACT_ENABLE_POWER				1	// ��������� �������
#define COMM_ACT_DISABLE_POWER				2	// ���������� �������
#define COMM_ACT_FAULT_CLEAR				3	// ������� fault
#define COMM_ACT_WARNING_CLEAR				4	// ������� warning

// Registers
#define COMM_REG_DEV_STATE					192	// ������� ���������
#define COMM_REG_FAULT_REASON				193	// ������� Fault
#define COMM_REG_DISABLE_REASON				194	// ������� Disable
#define COMM_REG_WARNING					195	// ������� Warning
#define COMM_REG_PROBLEM					196	// ������� Problem
#define COMM_REG_OP_RESULT					197	// ������� ���������� ��������

#endif // __COMMON_DICTIONARY_H
