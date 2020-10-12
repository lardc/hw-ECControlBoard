#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// Команды
#define ACT_ENABLE_POWER				1	// Переход в состояние ожидания
#define ACT_DISABLE_POWER				2	// Отключение блока
#define ACT_FAULT_CLEAR					3	// Очистка fault
#define ACT_WARNING_CLEAR				4	// Очистка warning
#define ACT_SAFETY_CLEAR				5	// Очистка нарушения контура безопасности

#define ACT_FAN_ON						48	// Включить вентилятор
#define ACT_FAN_OFF						49	// Отключить вентилятор

#define ACT_DBG_PULSE_EXT_SYNC1			50	// Одиночный импусль во внешнюю цепь SYNC_1
#define ACT_DBG_PULSE_EXT_SYNC2			51	// Одиночный импусль во внешнюю цепь SYNC_2
#define ACT_DBG_PULSE_INT_SYNC1			52	// Одиночный импусль во внутреннюю цепь SYNC_1
#define ACT_DBG_PULSE_INT_SYNC2			53	// Одиночный импусль во внутреннюю цепь SYNC_2
#define ACT_DBG_IS_STATE_INT_SYNC_1		54	// Состояние внутренней цепи SYNC_1
#define ACT_DBG_IS_STATE_INT_SYNC_2		55	// Состояние внутренней цепи SYNC_2
#define ACT_DBG_PULSE_EXT_LED			56	// Одиночный импусль внешнего светодиода

#define ACT_START_TEST					100	// Запуск измерения
#define ACT_STOP_TEST					101	// Запуск измерения
#define ACT_SAFETY_SET_ACTIVE			102	// Активация контура безопасности
#define ACT_SAFETY_SET_INACTIVE			103	// Деактивация контура безопасности

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

#define REG_CONTROL_VOLTAGE				132	// Напряжение входа управления
#define REG_CONTROL_CURRENT				133	// Ток входа управления

#define REG_COMM_VOLTAGE_TYPE_LEAKAGE	134	// Тип коммутируемого напряжения при измерении утечки
#define REG_COMM_POLARITY				135	// Полярность приложения постоянного коммутируемого напряжения
#define REG_COMM_CURRENT_SHAPE			136	// Форма импульса коммутируемого тока
#define REG_COMM_CURRENT_TIME			137	// Длительность импульса коммутируемого тока
#define REG_COMM_CURRENT				138	// Коммутируемый ток
#define REG_COMM_VOLTAGE				139	// Коммутируемого напряжение

#define REG_AUX_PS1_VOLTAGE				140	// Напряжение вспомогательного источника питания 1
#define REG_AUX_PS1_CURRENT				141	// Ток вспомогательного источника питания 1
#define REG_AUX_PS2_VOLTAGE				142	// Напряжение вспомогательного источника питания 2
#define REG_AUX_PS2_CURRENT				143	// Ток вспомогательного источника питания 2

#define REG_DBG_STATE					190	// Регистр режима Отладки

// Регистры только чтение
#define REG_DEV_STATE					192	// Регистр состояния
#define REG_FAULT_REASON				193	// Регистр Fault
#define REG_DISABLE_REASON				194	// Регистр Disable
#define REG_WARNING						195	// Регистр Warning
#define REG_PROBLEM						196	// Регистр Problem
#define REG_OP_RESULT					197	// Регистр результата операции
#define REG_DEV_SUB_STATE				198	// Регистр вспомогательного состояния
#define REG_CONFIG_ERR					199	// Регистр хранения кода ошибки конфигурирования
#define REG_FAULT_EXT_CODE				200	// Регистр расширенного кода Fault

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

// Fault and disable codes
#define DF_NONE							0
#define DF_INTERFACE					1	// Ошибка мастер-интерфейса
#define DF_LOGIC_WRONG_STATE			2	// Ведомый узел находится в некорректном состоянии
#define DF_LOGIC_STATE_TIMEOUT			3	// Таймаут смены состояния ведомым узлом
#define DF_LOGIC_EXEC					4	// Общий фолт выполнения логики (см. значение расширенного кода Fault)

// Группы кодов ошибок выполнения
// Код ошибки задаётся как FAULT_EXT_GR + ExecutionResult
#define FAULT_EXT_GR_MUX				10	// Multiplexer
#define FAULT_EXT_GR_DC_CURRENT			20	// DCCurrentBoard
#define FAULT_EXT_GR_DC_HV				30	// DCHighVoltageBoard
#define FAULT_EXT_GR_DC_VOLTAGE1		40	// DCVoltageBoard1
#define FAULT_EXT_GR_DC_VOLTAGE2		50	// DCVoltageBoard2
#define FAULT_EXT_GR_DC_VOLTAGE3		60	// DCVoltageBoard3

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
#define ERR_BAD_CONFIG					5	//  Попытка запуска измерения с неверной конфигурацией

#endif //  __DEV_OBJ_DIC_H
