#ifndef __MULTIPLEXER_DICTIONARY_H
#define __MULTIPLEXER_DICTIONARY_H

// Команды
#define MUX_ACT_SET_RELAY_GROUP				20 	// Подключить группу реле (на основании корпуса и т.д.)
#define MUX_ACT_SET_RELAY_NONE				21 	// Отключить все релле
#define MUX_ACT_SET_RELAY_TABLE				22 	// Подключить группу реле по номеру таблицы

// Регистры
#define MUX_REG_TYPE_MEASURE				130	// Тип измерения
#define MUX_REG_TYPE_CASE					131	// Тип корпуса
#define MUX_REG_POSITION_OF_CASE			132	// Позиция
#define MUX_REG_TYPE_SIGNAL_CTRL			133	// Тип управления
#define MUX_REG_TYPE_LEAKAGE				134	// Тип сигнала при утечке
#define MUX_REG_TYPE_POLARITY				135	// Полярность подключения

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
