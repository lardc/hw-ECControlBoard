#ifndef __GLOBAL_H
#define __GLOBAL_H

// Definitions
// 
#define	SCCI_TIMEOUT_TICKS			1000	// Таймаут интерфейса SCCI (в мс)
#define EP_WRITE_COUNT				0		// Количество массивов для записи
#define EP_COUNT					0		// Количество массивов для чтения
#define ENABLE_LOCKING				FALSE	// Защита NV регистров паролем

// Временные параметры
#define TIME_LED_BLINK				500		// Мигание светодиодом (в мс)

#define MAX_SLAVE_NODES				10		// Максимальное чилсо slave-узлов

#endif //  __GLOBAL_H
