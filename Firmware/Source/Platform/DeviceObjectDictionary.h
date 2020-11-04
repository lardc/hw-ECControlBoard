#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// Команды
#define ACT_ENABLE_POWER				1	// Переход в состояние ожидания
#define ACT_DISABLE_POWER				2	// Отключение блока
#define ACT_FAULT_CLEAR					3	// Очистка fault
#define ACT_WARNING_CLEAR				4	// Очистка warning
#define ACT_SAFETY_CLEAR				5	// Очистка нарушения контура безопасности

#define ACT_DIAG_READ_REG				10	// Чтение регистра slave-устройства по CAN-шине
#define ACT_DIAG_WRITE_REG				11	// Запись регистра slave-устройства по CAN-шине
#define ACT_DIAG_CALL					12	// Вызов команды slave-устройства по CAN-шине
#define ACT_DIAG_READ_EP				13	// Чтение EP slave-устройства по CAN-шине

#define ACT_DBG_PULSE_EXT_SYNC1			50	// Одиночный импусль во внешнюю цепь SYNC_1
#define ACT_DBG_PULSE_EXT_SYNC2			51	// Одиночный импусль во внешнюю цепь SYNC_2
#define ACT_DBG_PULSE_INT_SYNC1			52	// Одиночный импусль во внутреннюю цепь SYNC_1
#define ACT_DBG_PULSE_INT_SYNC2			53	// Одиночный импусль во внутреннюю цепь SYNC_2
#define ACT_DBG_IS_STATE_INT_SYNC_1		54	// Состояние внутренней цепи SYNC_1
#define ACT_DBG_IS_STATE_INT_SYNC_2		55	// Состояние внутренней цепи SYNC_2
#define ACT_DBG_PULSE_EXT_LED			56	// Одиночный импусль внешнего светодиода
#define ACT_FAN_ON						57	// Включить вентилятор
#define ACT_FAN_OFF						58	// Отключить вентилятор

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
#define REG_GENERAL_LOGIC_TIMEOUT		21	// Таймаут основных быстрых логических операций (в сек.)
#define REG_FAN_OPERATE_PERIOD			22	// Период активации вентилятора по простою (в сек.)
#define REG_FAN_OPERATE_MIN_TIME		23	// Минимальное время работы вентилятора (в сек.)

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
#define REG_COMM_CURRENT				138	// Коммутируемый ток (мкА)
#define REG_COMM_VOLTAGE				139	// Коммутируемого напряжение (мкВ)

#define REG_AUX_PS1_VOLTAGE				140	// Напряжение вспомогательного источника питания 1
#define REG_AUX_PS1_CURRENT				141	// Ток вспомогательного источника питания 1
#define REG_AUX_PS2_VOLTAGE				142	// Напряжение вспомогательного источника питания 2
#define REG_AUX_PS2_CURRENT				143	// Ток вспомогательного источника питания 2

#define REG_MODE_OUTPUT_RES				144	// Измерение сопротивления выходной цепи (1 - активировать режим)

#define REG_CTRL_PULSE_LENGTH			145	// Длительность импульса управления в импульсном режиме (в мс)

// Старшие слова 32битных чисел уставок
#define REG_CONTROL_VOLTAGE_32			150	// Напряжение входа управления
#define REG_CONTROL_CURRENT_32			151	// Ток входа управления
#define REG_COMM_CURRENT_32				152	// Коммутируемый ток (мкА)
#define REG_COMM_VOLTAGE_32				153	// Коммутируемого напряжение (мкВ)
#define REG_AUX_PS1_VOLTAGE_32			154	// Напряжение вспомогательного источника питания 1
#define REG_AUX_PS1_CURRENT_32			155	// Ток вспомогательного источника питания 1
#define REG_AUX_PS2_VOLTAGE_32			156	// Напряжение вспомогательного источника питания 2
#define REG_AUX_PS2_CURRENT_32			157	// Ток вспомогательного источника питания 2

#define REG_DIAG_NID					185	// CAN: регистр NodeID для доступа к slave-устройствам
#define REG_DIAG_IN_1					186	// CAN: входной отладочный регистр 1
#define REG_DIAG_IN_2					187	// CAN: входной отладочный регистр 2

#define REG_DBG_STATE					190	// Регистр режима Отладки

// Регистры только чтение
#define REG_DEV_STATE					192	// Регистр состояния
#define REG_FAULT_REASON				193	// Регистр Fault
#define REG_DISABLE_REASON				194	// Регистр Disable
#define REG_WARNING						195	// Регистр Warning
#define REG_PROBLEM						196	// Регистр Problem
#define REG_OP_RESULT					197	// Регистр результата операции

#define REG_RESULT_LEAKAGE_CURRENT		198	// Ток утечки на выходе
#define REG_RESULT_ON_VOLTAGE			199	// Остаточное напряжение на выходе (в мкВ)
#define REG_RESULT_CONTROL_CURRENT		200	// Ток управления
#define REG_RESULT_CONTROL_VOLTAGE		201	// Напряжение управления
#define REG_RESULT_INHIBIT_VOLTAGE		202	// Напряжение запрета
#define REG_RESULT_AUX_CURRENT1			203	// Ток вспомогательного источника 1
#define REG_RESULT_AUX_CURRENT2			204	// Ток вспомогательного источника 2
#define REG_RESULT_OUTPUT_RES			205	// Выходное сопротивление

#define REG_RESULT_LEAKAGE_VOLTAGE		206	// Напряжение измерения утечки
#define REG_RESULT_LEAKAGE_VOLTAGE_32	207
#define REG_RESULT_ON_CURRENT			208	// Ток измерения остаточного напряжения
#define REG_RESULT_ON_CURRENT_32		209

#define REG_DEV_SUB_STATE				220	// Регистр вспомогательного состояния
#define REG_CONFIG_ERR					221	// Регистр хранения кода ошибки конфигурирования

#define REG_BHL_ERROR_CODE				225	// Ошибка интерфейса ВУ: код ошибки
#define REG_BHL_DEVICE					226	// Ошибка интерфейса ВУ: NID устройства
#define REG_BHL_FUNCTION				227	// Ошибка интерфейса ВУ: код функции
#define REG_BHL_EXT_DATA				228	// Ошибка интерфейса ВУ: расширенная информация

#define REG_DIAG_OUT_1					230	// CAN: отладочный выходной регистр 1
#define REG_DIAG_OUT_2					231	// CAN: отладочный выходной регистр 2

// Старшие слова 32битных чисел результатов
#define REG_RESULT_LEAKAGE_CURRENT_32	230	// Ток утечки на выходе
#define REG_RESULT_ON_VOLTAGE_32		231	// Остаточное напряжение на выходе (в мкВ)
#define REG_RESULT_CONTROL_CURRENT_32	232	// Ток управления
#define REG_RESULT_CONTROL_VOLTAGE_32	233	// Напряжение управления
#define REG_RESULT_INHIBIT_VOLTAGE_32	234	// Напряжение запрета
#define REG_RESULT_AUX_CURRENT1_32		235	// Ток вспомогательного источника 1
#define REG_RESULT_AUX_CURRENT2_32		236	// Ток вспомогательного источника 2
#define REG_RESULT_OUTPUT_RES_32		237	// Выходное сопротивление

#define REG_DBG_INT_SYNC1				251	// Регистр состояния внутреннейи линии SYNC1
#define REG_DBG_INT_SYNC2				252	// Регистр состояния внутреннейи линии SYNC2
#define REG_FAN_STATE					253	// Регистр управления вентилятором
// -----------------------------
#define REG_FWINFO_SLAVE_NID			256	// Device CAN slave node ID
#define REG_FWINFO_MASTER_NID			257	// Device CAN master node ID (if presented)
// 258 - 259
#define REG_FWINFO_STR_LEN				260	// Length of the information string record
#define REG_FWINFO_STR_BEGIN			261	// Begining of the information string record

// Operation results
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

// Fault and disable codes
#define DF_NONE							0

// Группы кодов ошибок выполнения
// Код ошибки задаётся как FAULT_EXT_GR + ExecutionResult
#define FAULT_EXT_GR_COMMON				0	// Общие ошибки
#define FAULT_EXT_GR_MUX				100	// Multiplexer
#define FAULT_EXT_GR_DC_CURRENT			200	// DCCurrentBoard
#define FAULT_EXT_GR_DC_HV				300	// DCHighVoltageBoard
#define FAULT_EXT_GR_DC_VOLTAGE1		400	// DCVoltageBoard1
#define FAULT_EXT_GR_DC_VOLTAGE2		500	// DCVoltageBoard2
#define FAULT_EXT_GR_DC_VOLTAGE3		600	// DCVoltageBoard3
#define FAULT_EXT_GR_AC_VOLTAGE1		700	// ACVoltageBoard1
#define FAULT_EXT_GR_AC_VOLTAGE2		800	// ACVoltageBoard2

// Problem
#define PROBLEM_NONE					0
#define PROBLEM_CONTROL_NODE			1	// Problem узла формирования сигнала управления
#define PROBLEM_CURRENT_NODE			2	// Problem узла формирования тока

//  Warning
#define WARNING_NONE					0

// Endpoints
#define EP_SLAVE_DATA					1	// Данные вычитанные из Slave-устройств

//  User Errors
#define ERR_NONE						0
#define ERR_CONFIGURATION_LOCKED		1	//  Устройство защищено от записи
#define ERR_OPERATION_BLOCKED			2	//  Операция не может быть выполнена в текущем состоянии устройства
#define ERR_DEVICE_NOT_READY			3	//  Устройство не готово для смены состояния
#define ERR_WRONG_PWD					4	//  Неправильный ключ
#define ERR_BAD_CONFIG					5	//  Попытка запуска измерения с неверной конфигурацией

#endif //  __DEV_OBJ_DIC_H
