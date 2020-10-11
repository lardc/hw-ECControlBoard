#ifndef __AC_VOLTAGE_BOARD_DICT_H
#define __AC_VOLTAGE_BOARD_DICT_H

// �������
#define ACV_ACT_START_PROCESS				100	// ������ �������� ������������ �����������
#define ACV_ACT_STOP_PROCESS				101	// ��������� �������� ������������ �����������

// ��������
#define ACV_REG_OUTPUT_LINE					128	// ����� ������ ��������� ����������
#define ACV_REG_CURRENT_SETPOINT			129	// �������� ������� ����
#define ACV_REG_VOLTAGE_SETPOINT			130	// �������� ������� ����������

#define ACV_REG_CURRENT_RESULT				200	// �������� ��������� ����
#define ACV_REG_VOLTAGE_RESULT				201	// �������� ��������� ����������

typedef enum __ACV_OutputLine
{
	AC_BUS_LV = 1,
	AC_CTRL = 2
} ACV_OutputLine;

#endif // __AC_VOLTAGE_BOARD_DICT_H
