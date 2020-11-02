#ifndef __MULTIPLEXER_DICTIONARY_H
#define __MULTIPLEXER_DICTIONARY_H

// Команды
#define MUX_ACT_SET_RELAY_GROUP				100	// Подключить группу реле (на основании корпуса и т.д.)
#define MUX_ACT_SET_RELAY_NONE				101	// Отключить все релле

// Регистры
#define MUX_REG_TYPE_MEASURE				130	// Тип измерения
#define MUX_REG_TYPE_CASE					131	// Тип корпуса
#define MUX_REG_POSITION_OF_CASE			132	// Позиция
#define MUX_REG_TYPE_SIGNAL_CTRL			133	// Тип управления
#define MUX_REG_TYPE_LEAKAGE				134	// Тип сигнала при утечке
#define MUX_REG_TYPE_POLARITY				135	// Полярность подключения

typedef enum __MUX_Position
{
	MX_Position1 = 1,
	MX_Position2 = 2,
	MX_Position3 = 3
} MUX_Position;

typedef enum __MUX_Polarity
{
	MX_PolarityDirect = 1,
	MX_PolarityReverse = 2
} MUX_Polarity;

typedef enum __MUX_ControlSupply
{
	MUX_ControlDC = 1,
	MUX_ControlAC = 2
} MUX_ControlSupply;

#endif // __MULTIPLEXER_DICTIONARY_H
