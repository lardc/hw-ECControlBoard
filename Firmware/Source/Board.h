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

// ����������� ��� �������� ������
GPIO_PortPinSettingMacro GPIO_LED				= {GPIOB, Pin_7};

// ����������� ��� ������ �������������� �������
GPIO_PortPinSettingMacro GPIO_ALT_CAN_RX		= {GPIOA, Pin_11};
GPIO_PortPinSettingMacro GPIO_ALT_CAN_TX		= {GPIOA, Pin_12};
GPIO_PortPinSettingMacro GPIO_ALT_UART_RX		= {GPIOA, Pin_10};
GPIO_PortPinSettingMacro GPIO_ALT_UART_TX		= {GPIOA, Pin_9};

#endif // __BOARD_H
