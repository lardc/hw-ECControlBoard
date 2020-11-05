#ifndef __AC_VOLTAGE_BOARD_DICT_H
#define __AC_VOLTAGE_BOARD_DICT_H

// �������
#define ACV_ACT_START_PROCESS				100	// ������ �������� ������������ �����������
#define ACV_ACT_STOP_PROCESS				101	// ��������� �������� ������������ �����������

// ��������
#define ACV_REG_VOLTAGE_SETPOINT			128	// �������� ������� ���������� (� ��)
#define ACV_REG_VOLTAGE_SETPOINT_32			129
#define ACV_REG_CURRENT_SETPOINT			130	// �������� ������� ���� (� ���)
#define ACV_REG_CURRENT_SETPOINT_32			131
#define ACV_REG_OUTPUT_LINE					132	// ����� ������ ��������� ����������

#define ACV_REG_VOLTAGE_RESULT				200	// �������� ��������� ���������� (� ��)
#define ACV_REG_VOLTAGE_RESULT_32			201
#define ACV_REG_CURRENT_RESULT				202	// �������� ��������� ���� (� ���)
#define ACV_REG_CURRENT_RESULT_32			203
#define ACV_REG_VOLTAGE_READY				204	// ���� ������ �� ������� ����������

typedef enum __ACV_OutputLine
{
	ACV_BUS_LV = 1,
	ACV_CTRL = 2
} ACV_OutputLine;

#endif // __AC_VOLTAGE_BOARD_DICT_H
