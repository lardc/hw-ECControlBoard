#ifndef __MULTIPLEXER_DICTIONARY_H
#define __MULTIPLEXER_DICTIONARY_H

// �������
#define MUX_ACT_SET_RELAY_GROUP				20 	// ���������� ������ ���� (�� ��������� ������� � �.�.)
#define MUX_ACT_SET_RELAY_NONE				21 	// ��������� ��� �����
#define MUX_ACT_SET_RELAY_TABLE				22 	// ���������� ������ ���� �� ������ �������

// ��������
#define MUX_REG_TYPE_MEASURE				130	// ��� ���������
#define MUX_REG_TYPE_CASE					131	// ��� �������
#define MUX_REG_POSITION_OF_CASE			132	// �������
#define MUX_REG_TYPE_SIGNAL_CTRL			133	// ��� ����������
#define MUX_REG_TYPE_LEAKAGE				134	// ��� ������� ��� ������
#define MUX_REG_TYPE_POLARITY				135	// ���������� �����������

// ���� ���������
#define MUX_MEASURE_LEAKAGE_CURRENT			1	// ��� ������ � �������� ���������
#define MUX_MEASURE_ON_VOLTAGE				2	// ���������� ���������� � �������� ���������
#define MUX_MEASURE_INPUT_VOLTAGE			3	// ������� ����������
#define MUX_MEASURE_INHIBIT_VOLTAGE			4	// ���������� �������

// ���� �������� ��������
#define MUX_CASE_A1							1
#define MUX_CASE_I1							2
#define MUX_CASE_I6							3
#define MUX_CASE_B1							4
#define MUX_CASE_B2							5
#define MUX_CASE_B5							6
#define MUX_CASE_V1							7
#define MUX_CASE_V2							8
#define MUX_CASE_V104						9
#define MUX_CASE_V108						10
#define MUX_CASE_L1							11
#define MUX_CASE_L2							12
#define MUX_CASE_D1							13
#define MUX_CASE_D2							14
#define MUX_CASE_D192						15

// ������ �������
#define MUX_POSITION_1						1
#define MUX_POSITION_2						2
#define MUX_POSITION_3						3

// ��� ����� �� ����������
#define MUX_CTRL_DC_V						1
#define MUX_CTRL_DC_I						2
#define MUX_CTRL_AC_V						3

// ������ ��������� ������ � �������� ���������
#define MUX_LEAKAGE_DC						1
#define MUX_LEAKAGE_AC						2

// ���������� ����������
#define MUX_DIRECTION_FORWARD				1
#define MUX_DIRECTION_REVERSE				2

#endif // __MULTIPLEXER_DICTIONARY_H
