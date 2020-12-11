#ifndef __MULTIPLEXER_DICTIONARY_H
#define __MULTIPLEXER_DICTIONARY_H

// �������
#define MUX_ACT_SET_RELAY_GROUP				100	// ���������� ������ ���� (�� ��������� ������� � �.�.)
#define MUX_ACT_SET_RELAY_NONE				101	// ��������� ��� �����
#define MUX_ACT_SET_RELAY_GROUP_FAST_HV		102	// ������� ����������� �������������� ����

// ��������
#define MUX_REG_TYPE_MEASURE				130	// ��� ���������
#define MUX_REG_TYPE_CASE					131	// ��� �������
#define MUX_REG_POSITION_OF_CASE			132	// �������
#define MUX_REG_TYPE_SIGNAL_CTRL			133	// ��� ����������
#define MUX_REG_TYPE_LEAKAGE				134	// ��� ������� ��� ������
#define MUX_REG_TYPE_POLARITY				135	// ���������� �����������
#define MUX_REG_ENABLE_SAFETY				136	// ���������� ������� ������������ ��� ���������� ����������

#define MUX_REG_BUTTON_START				210	// ������� ��������� ������ �����

// �������������� ���������
#define MUX_STATE_SAFETY_TRIG				5

typedef enum __MUX_Position
{
	MXP_Position1 = 1,
	MXP_Position2 = 2,
	MXP_Position3 = 3
} MUX_Position;

typedef enum __MUX_OutputPolarity
{
	MXOP_Direct = 1,
	MXOP_Reverse = 2
} MUX_Polarity;

typedef enum __MUX_Control
{
	MXC_ControlDC = 1,
	MXC_ControlAC = 2,
	MXC_ControlDCReverse = 3
} MUX_Control;

#endif // __MULTIPLEXER_DICTIONARY_H
