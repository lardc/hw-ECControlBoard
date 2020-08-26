#ifndef __BOARD_H
#define __BOARD_H

#include "stm32f30x.h"

#include "ZwRCC.h"
#include "ZwGPIO.h"
#include "ZwNCAN.h"
#include "ZwTIM.h"
#include "ZwDAC.h"
#include "ZwDMA.h"
#include "ZwADC.h"
#include "ZwEXTI.h"
#include "ZwSCI.h"
#include "ZwIWDG.h"
#include "ZwNFLASH.h"
#include "ZwSPI.h"

// Определения для выходных портов
GPIO_PortPinSettingMacro GPIO_LED_EXT_RED	= {GPIOA, Pin_0};
GPIO_PortPinSettingMacro GPIO_LED_EXT_GREEN	= {GPIOA, Pin_1};
GPIO_PortPinSettingMacro GPIO_SET			= {GPIOA, Pin_4};
GPIO_PortPinSettingMacro GPIO_CLK			= {GPIOA, Pin_5};
GPIO_PortPinSettingMacro GPIO_DATA			= {GPIOA, Pin_7};
GPIO_PortPinSettingMacro GPIO_CTRL_SYNC_2	= {GPIOA, Pin_15};

GPIO_PortPinSettingMacro GPIO_LOCK_2		= {GPIOB, Pin_0};
GPIO_PortPinSettingMacro GPIO_LOCK_1		= {GPIOB, Pin_1};
GPIO_PortPinSettingMacro GPIO_CTRL_SYNC_1	= {GPIOB, Pin_3};
GPIO_PortPinSettingMacro GPIO_RESET			= {GPIOB, Pin_10};
GPIO_PortPinSettingMacro GPIO_OE			= {GPIOB, Pin_11};
GPIO_PortPinSettingMacro GPIO_LED			= {GPIOB, Pin_15};

// Определения для портов альтернативных функций
GPIO_PortPinSettingMacro GPIO_ALT_CAN_RX	= {GPIOA, Pin_11};
GPIO_PortPinSettingMacro GPIO_ALT_CAN_TX	= {GPIOA, Pin_12};
GPIO_PortPinSettingMacro GPIO_ALT_UART_RX	= {GPIOA, Pin_10};
GPIO_PortPinSettingMacro GPIO_ALT_UART_TX	= {GPIOA, Pin_9};

// Входы
GPIO_PortPinSettingMacro GPIO_SYNC_1		= {GPIOB, Pin_4};
GPIO_PortPinSettingMacro GPIO_SYNC_2		= {GPIOB, Pin_5};
GPIO_PortPinSettingMacro GPIO_SFTY_1		= {GPIOB, Pin_6};
GPIO_PortPinSettingMacro GPIO_SFTY_2		= {GPIOB, Pin_7};
GPIO_PortPinSettingMacro GPIO_KEY_START		= {GPIOB, Pin_8};
GPIO_PortPinSettingMacro GPIO_KEY_STOP		= {GPIOB, Pin_9};

#endif // __BOARD_H
