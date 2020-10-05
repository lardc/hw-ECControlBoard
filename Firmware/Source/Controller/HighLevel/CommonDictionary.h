#ifndef __COMMON_DICTIONARY_H
#define __COMMON_DICTIONARY_H

// Actions
#define COMM_ACT_ENABLE_POWER				1	// Включение питания
#define COMM_ACT_DISABLE_POWER				2	// Отключение питания
#define COMM_ACT_FAULT_CLEAR				3	// Очистка fault
#define COMM_ACT_WARNING_CLEAR				4	// Очистка warning

// Registers
#define COMM_REG_DEV_STATE					192	// Регистр состояния
#define COMM_REG_FAULT_REASON				193	// Регистр Fault
#define COMM_REG_DISABLE_REASON				194	// Регистр Disable
#define COMM_REG_WARNING					195	// Регистр Warning
#define COMM_REG_PROBLEM					196	// Регистр Problem
#define COMM_REG_OP_RESULT					197	// Регистр результата операции

#endif // __COMMON_DICTIONARY_H
