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

typedef enum __MUX_Position
{
	Position1 = 1,
	Position2 = 2,
	Position3 = 3
} MUX_Position;

typedef enum __MUX_Polarity
{
	PolarityDirect = 1,
	PolarityReverse = 2
} MUX_Polarity;

#endif // __MULTIPLEXER_DICTIONARY_H
