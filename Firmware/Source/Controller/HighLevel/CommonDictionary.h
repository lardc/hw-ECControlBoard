#ifndef __COMMON_DICTIONARY_H
#define __COMMON_DICTIONARY_H

// Actions
#define ACT_COMM_ENABLE_POWER				1	// Включение питания
#define ACT_COMM_DISABLE_POWER				2	// Отключение питания
#define ACT_COMM_FAULT_CLEAR				3	// Очистка fault
#define ACT_COMM_WARNING_CLEAR				4	// Очистка warning

// Registers
#define REG_COMM_DEV_STATE					192	// Регистр состояния
#define REG_COMM_FAULT_REASON				193	// Регистр Fault
#define REG_COMM_DISABLE_REASON				194	// Регистр Disable
#define REG_COMM_WARNING					195	// Регистр Warning
#define REG_COMM_PROBLEM					196	// Регистр Problem
#define REG_COMM_OP_RESULT					197	// Регистр результата операции

#endif // __COMMON_DICTIONARY_H
