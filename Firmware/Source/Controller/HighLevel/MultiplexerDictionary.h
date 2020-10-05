#ifndef __MULTIPLEXER_DICTIONARY_H
#define __MULTIPLEXER_DICTIONARY_H

// Action
#define ACT_MUX_SET_RELAY_GROUP				20 	// Подключить группу реле (на основании корпуса и т.д.)
#define ACT_MUX_SET_RELAY_NONE				21 	// Отключить все релле
#define ACT_MUX_SET_RELAY_TABLE				22 	// Подключить группу реле по номеру таблицы

// Registers
#define REG_MUX_TYPE_MEASURE				130	// Тип измерения
#define REG_MUX_TYPE_CASE					131	// Тип корпуса
#define REG_MUX_POSITION_OF_CASE			132	// Позиция
#define REG_MUX_TYPE_SIGNAL_CTRL			133	// Тип управления
#define REG_MUX_TYPE_SIGNAL_AT_LEAKAGE		134	// Тип сигнала при утечке
#define REG_MUX_TYPE_POLARITY				135	// Полярность подключения

#endif // __MULTIPLEXER_DICTIONARY_H
