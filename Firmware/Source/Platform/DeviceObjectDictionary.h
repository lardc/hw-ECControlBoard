#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// Команды
#define ACT_ENABLE_POWER				1	// Переход в состояние ожидания
#define ACT_DISABLE_POWER				2	// Отключение блока
#define ACT_FAULT_CLEAR					3	// Очистка fault
#define ACT_WARNING_CLEAR				4	// Очистка warning

#define ACT_FAN_ON						48	// Включить вентилятор
#define ACT_FAN_OFF						49	// Отключить вентилятор

#define ACT_DBG_PULSE_EXT_SYNC1			50	// Одиночный импусль во внешнюю цепь SYNC_1
#define ACT_DBG_PULSE_EXT_SYNC2			51	// Одиночный импусль во внешнюю цепь SYNC_2
#define ACT_DBG_PULSE_INT_SYNC1			52	// Одиночный импусль во внутреннюю цепь SYNC_1
#define ACT_DBG_PULSE_INT_SYNC2			53	// Одиночный импусль во внутреннюю цепь SYNC_2
#define ACT_DBG_IS_STATE_INT_SYNC_1		54	// Состояние внутренней цепи SYNC_1
#define ACT_DBG_IS_STATE_INT_SYNC_2		55	// Состояние внутренней цепи SYNC_2
#define ACT_DBG_PULSE_EXT_LED			56	// Одиночный импусль внешнего светодиода

#define ACT_SAVE_TO_ROM					200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM			201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT			202	// Сброс DataTable в состояние по умолчанию

#define ACT_BOOT_LOADER_REQUEST			320	// Перезапуск процессора с целью перепрограммирования
// -----------------------------

// Регистры
// Сохраняемые регистры
#define REG_SLAVE_NID_MULTIPLEXER		0	// Can NodeID для ECDCMultiplexerBoard
#define REG_SLAVE_NID_DC_CURRENT		1	// Can NodeID для ECDCCurrentBoard
#define REG_SLAVE_NID_DC_HV				2	// Can NodeID для ECDCHighVoltageBoard
#define REG_SLAVE_NID_DC_VOLTAGE1		3	// Can NodeID для ECDCVoltageBoard №1
#define REG_SLAVE_NID_DC_VOLTAGE2		4	// Can NodeID для ECDCVoltageBoard №2
#define REG_SLAVE_NID_DC_VOLTAGE3		5	// Can NodeID для ECDCVoltageBoard №3
#define REG_SLAVE_NID_AC_VOLTAGE1		6	// Can NodeID для ECACVoltageBoard №1
#define REG_SLAVE_NID_AC_VOLTAGE2		7	// Can NodeID для ECACVoltageBoard №2
// 8 - 9
#define REG_EMULATE_MULTIPLEXER			10	// Режим эмуляции для ECDCMultiplexerBoard
#define REG_EMULATE_DC_CURRENT			11	// Режим эмуляции для ECDCCurrentBoard
#define REG_EMULATE_DC_HV				12	// Режим эмуляции для ECDCHighVoltageBoard
#define REG_EMULATE_DC_VOLTAGE1			13	// Режим эмуляции для ECDCVoltageBoard №1
#define REG_EMULATE_DC_VOLTAGE2			14	// Режим эмуляции для ECDCVoltageBoard №2
#define REG_EMULATE_DC_VOLTAGE3			15	// Режим эмуляции для ECDCVoltageBoard №3
#define REG_EMULATE_AC_VOLTAGE1			16	// Режим эмуляции для ECACVoltageBoard №1
#define REG_EMULATE_AC_VOLTAGE2			17	// Режим эмуляции для ECACVoltageBoard №2
// 18 - 19
#define REG_POWER_ENABLE_TIMEOUT		20	// Таймаут ожидания включения питания (в сек.)

// Несохраняемы регистры чтения-записи
#define REG_MEASUREMENT_TYPE			128	// Тип выполняемого измерения
#define REG_DUT_CASE_TYPE				129	// Тип корпуса испытуемого прибора
#define REG_DUT_POSITION_NUMBER			130	// Номер позиции на приборе
#define REG_INPUT_CONTROL_TYPE			131	// Тип входного сигнала управления
#define REG_I_LEAK_VOLTAGE_TYPE			132	// Тип выходного напряжения измерения тока утечки

#define REG32S_INPUT_VOLTAGE			135	// Напряжение входа управления
#define REG32S_INPUT_VOLTAGE_32			136
#define REG32S_INPUT_CURRENT			137	// Ток входа управления
#define REG32S_INPUT_CURRENT_32			138

#define REG32S_OUTPUT_VOLTAGE			139	// Напряжение входа управления
#define REG32S_OUTPUT_VOLTAGE_32		140
#define REG32S_OUTPUT_CURRENT			141	// Ток входа управления
#define REG32S_OUTPUT_CURRENT_32		142

#define REG_AUX_PS1_VOLTAGE				143	// Напряжение вспомогательного источника питания 1
#define REG_AUX_PS1_CURRENT				144	// Ток вспомогательного источника питания 1
#define REG_AUX_PS2_VOLTAGE				145	// Напряжение вспомогательного источника питания 2
#define REG_AUX_PS2_CURRENT				146	// Ток вспомогательного источника питания 2

#define REG_DBG_STATE					190	// Регистр режима Отладки

// Регистры только чтение
#define REG_DEV_STATE					192	// Регистр состояния
#define REG_FAULT_REASON				193	// Регистр Fault
#define REG_DISABLE_REASON				194	// Регистр Disable
#define REG_WARNING						195	// Регистр Warning
#define REG_PROBLEM						196	// Регистр Problem
#define REG_OP_RESULT					197	// Регистр результата операции
#define REG_DEV_SUB_STATE				198	// Регистр вспомогательного состояния

#define REG_BHL_ERROR_CODE				210	// Ошибка интерфейса ВУ: код ошибки
#define REG_BHL_DEVICE					211	// Ошибка интерфейса ВУ: NID устройства
#define REG_BHL_FUNCTION				212	// Ошибка интерфейса ВУ: код функции
#define REG_BHL_EXT_DATA				213	// Ошибка интерфейса ВУ: расширенная информация

#define REG_DBG_INT_SYNC1				251	// Регистр состояния внутреннейи линии SYNC1
#define REG_DBG_INT_SYNC2				252	// Регистр состояния внутреннейи линии SYNC2
#define REG_FAN_STATE					253	// Регистр управления вентилятором
// -----------------------------

// Operation results
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

//  Fault and disable codes
#define DF_NONE							0
#define DF_INTERFACE					1	// Ошибка мастер-интерфейса
#define DF_LOGIC_WRONG_STATE			2	// Ведомый узел находится в некорректном состоянии
#define DF_LOGIC_STATE_TIMEOUT			3	// Таймаут смены состояния ведомым узлом

// Problem
#define PROBLEM_NONE					0

//  Warning
#define WARNING_NONE					0

//  User Errors
#define ERR_NONE						0
#define ERR_CONFIGURATION_LOCKED		1	//  Устройство защищено от записи
#define ERR_OPERATION_BLOCKED			2	//  Операция не может быть выполнена в текущем состоянии устройства
#define ERR_DEVICE_NOT_READY			3	//  Устройство не готово для смены состояния
#define ERR_WRONG_PWD					4	//  Неправильный ключ

#endif //  __DEV_OBJ_DIC_H
