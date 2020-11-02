#ifndef __CURRENT_BOARD_DICTIONARY_H
#define __CURRENT_BOARD_DICTIONARY_H

// �������
#define CURR_ACT_START_PROCESS				100	// ������ �������� ������������ ��������
#define CURR_ACT_STOP_PROCESS				101	// ��������� �������� ������������ ��������

// ��������
#define CURR_REG_CURRENT_SETPOINT			128	// �������� ������� ���� (� ���)
#define CURR_REG_CURRENT_SETPOINT_32		129
#define CURR_REG_VOLTAGE_DUT_LIM			130	// ���������� �������� ������� ���������� (� ���)
#define CURR_REG_VOLTAGE_DUT_LIM_32			131

#define CURR_REG_RESULT_CURRENT				250	// ���������� �������� ���� (� ���)
#define CURR_REG_RESULT_CURRENT_32			251
#define CURR_REG_RESULT_VOLTAGE				252	// ���������� �������� ���������� (� ���)
#define CURR_REG_RESULT_VOLTAGE_32			253

typedef enum __CurrentShape
{
	CB_CurrentTrapeze = 1,
	CB_CurrentSin = 2
} CurrentShape;

#endif // __CURRENT_BOARD_DICTIONARY_H
