#ifndef __AC_VOLTAGE_BOARD_DICT_H
#define __AC_VOLTAGE_BOARD_DICT_H

// Команды
#define ACV_ACT_START_PROCESS				100	// Страрт процесса формирования напряяжения
#define ACV_ACT_STOP_PROCESS				101	// Остановка процесса формирования напряяжения

// Регистры
#define ACV_REG_OUTPUT_LINE					128	// Линия подачи выходного напряжения
#define ACV_REG_CURRENT_SETPOINT			129	// Значение задания тока
#define ACV_REG_VOLTAGE_SETPOINT			130	// Значение задания напряжения

#define ACV_REG_CURRENT_RESULT				200	// Значение измерения тока
#define ACV_REG_VOLTAGE_RESULT				201	// Значение измерения напряжения

#define ACV_REG_VOLTAGE_READY				202	// Флаг выхода на уставку напряжения

typedef enum __ACV_OutputLine
{
	ACV_BUS_LV = 1,
	ACV_CTRL = 2
} ACV_OutputLine;

#endif // __AC_VOLTAGE_BOARD_DICT_H
