#ifndef RS232_422_485_H
#define RS232_422_485_H

#include "usart.h"
#include "gpio.h"
#include "power.h"
#include "can.h"
#include <string.h>
#include "Di_Acq.h"
#include "Do_Ctrl.h"
#include "CAN_Comm.h"
#include "Ai_Acq.h"
#include "w25q128.h"

extern UART_HandleTypeDef *Rs485_1_HandleS; //
extern UART_HandleTypeDef *Rs422_2_HandleS; //
extern UART_HandleTypeDef *Rs232_1_HandleS; //
extern UART_HandleTypeDef *Rs232_2_HandleS; //
extern UART_HandleTypeDef *Rs422_1_HandleS; //
extern UART_HandleTypeDef *Rs485_2_HandleS; //


void USART_CommInit(void);

#endif
