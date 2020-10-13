#ifndef __DC_HIGH_VOLTAGE_BOARD_DICT_H
#define __DC_HIGH_VOLTAGE_BOARD_DICT_H

// Команды
#define DCHV_ACT_START_PROCESS				100	// Страрт процесса формирования напряяжения
#define DCHV_ACT_STOP_PROCESS				101	// Остановка процесса формирования напряяжения

// Регистры
#define DCHV_REG_CURRENT_SETPOINT			128	// Значение задания тока
#define DCHV_REG_VOLTAGE_SETPOINT			129	// Значение задания напряжения

#define DCHV_REG_VOLTAGE_READY				199	// Флаг выхода на уставку напряжения
#define DCHV_REG_CURRENT_RESULT				200	// Значение измерения тока
#define DCHV_REG_VOLTAGE_RESULT				201	// Значение измерения напряжения

#endif // __DC_HIGH_VOLTAGE_BOARD_DICT_H
