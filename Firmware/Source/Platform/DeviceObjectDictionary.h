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

// Несохраняемы регистры чтения-записи
#define REG_DBG_STATE					150	// Регистр режима Отладки

// Регистры только чтение
#define REG_DEV_STATE					192	// Регистр состояния
#define REG_FAULT_REASON				193	// Регистр Fault
#define REG_DISABLE_REASON				194	// Регистр Disable
#define REG_WARNING						195	// Регистр Warning
#define REG_PROBLEM						196	// Регистр Problem
#define REG_OP_RESULT					197	// Регистр результата операции

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
