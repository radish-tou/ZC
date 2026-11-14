#include "RS232_422_485.h"

/*
串口1：RS485_1 PA9/PA10 EN:PA8
串口2：RS422_2 PD5/PD6  DE:PD4 RE:PD3
串口3：RS232_1 PD8/PD9
串口4：RS232_2 PC10/PC11
串口5：RS422_1 PC12/PD2 DE:PD1 RE:PD0
串口6：RS485_2 PC6/PC7  EN:PC8
*/

#define UART_BUFF_SIZE 256

UART_HandleTypeDef *Rs485_1_HandleS; //
UART_HandleTypeDef *Rs422_2_HandleS; //
UART_HandleTypeDef *Rs232_1_HandleS; //
UART_HandleTypeDef *Rs232_2_HandleS; //
UART_HandleTypeDef *Rs422_1_HandleS; //
UART_HandleTypeDef *Rs485_2_HandleS; //

volatile uint8_t UART1RxBuf[UART_BUFF_SIZE];
volatile uint8_t UART2RxBuf[UART_BUFF_SIZE];
volatile uint8_t UART3RxBuf[UART_BUFF_SIZE];
volatile uint8_t UART4RxBuf[UART_BUFF_SIZE];
volatile uint8_t UART5RxBuf[UART_BUFF_SIZE];
volatile uint8_t UART6RxBuf[UART_BUFF_SIZE];

// 串口1  rs485-1
static void UART1_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    HAL_GPIO_WritePin(RS485_EN1_GPIO_Port, RS485_EN1_Pin, GPIO_PIN_SET);
	  //返回状态LocalSystemSend(uint8_t* data,uint16_t size);
    HAL_UART_Transmit(Rs485_1_HandleS, (uint8_t *)UART1RxBuf, size, 0xFFFF);
    HAL_GPIO_WritePin(RS485_EN1_GPIO_Port, RS485_EN1_Pin, GPIO_PIN_RESET);

    HAL_UARTEx_ReceiveToIdle_DMA(Rs485_1_HandleS, (uint8_t *)UART1RxBuf, UART_BUFF_SIZE);
}

static void UART1_TxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_GPIO_WritePin(RS485_EN1_GPIO_Port, RS485_EN1_Pin, GPIO_PIN_RESET);
}

static void UART1_ErrorCallback(UART_HandleTypeDef *huart)
{
    HAL_UARTEx_ReceiveToIdle_DMA(Rs485_1_HandleS, (uint8_t *)UART1RxBuf, UART_BUFF_SIZE);
    HAL_GPIO_WritePin(RS485_EN1_GPIO_Port, RS485_EN1_Pin, GPIO_PIN_RESET);
}

// 串口2 rs422-2
static void UART2_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    HAL_UART_Transmit(Rs422_2_HandleS, (uint8_t *)UART2RxBuf, size, 0xFFFF);

    HAL_UARTEx_ReceiveToIdle_DMA(Rs422_2_HandleS, (uint8_t *)UART2RxBuf, UART_BUFF_SIZE);
}

static void UART2_TxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_GPIO_WritePin(RE_422_2_GPIO_Port, RE_422_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DE_422_2_GPIO_Port, DE_422_2_Pin, GPIO_PIN_SET);
}

static void UART2_ErrorCallback(UART_HandleTypeDef *huart)
{
    HAL_UARTEx_ReceiveToIdle_DMA(Rs422_2_HandleS, (uint8_t *)UART2RxBuf, UART_BUFF_SIZE);
    HAL_GPIO_WritePin(RE_422_2_GPIO_Port, RE_422_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DE_422_2_GPIO_Port, DE_422_2_Pin, GPIO_PIN_SET);
}

// 串口3 rs232_1
static void UART3_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    HAL_UART_Transmit(Rs232_1_HandleS, (uint8_t *)UART3RxBuf, size, 0xFFFF);

    HAL_UARTEx_ReceiveToIdle_DMA(Rs232_1_HandleS, (uint8_t *)UART3RxBuf, UART_BUFF_SIZE);
}

static void UART3_TxCpltCallback(UART_HandleTypeDef *huart)
{
}

static void UART3_ErrorCallback(UART_HandleTypeDef *huart)
{
    HAL_UARTEx_ReceiveToIdle_DMA(Rs232_1_HandleS, (uint8_t *)UART3RxBuf, UART_BUFF_SIZE);
}

// 串口4 rs232_2
static void UART4_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    HAL_UART_Transmit(Rs232_2_HandleS, (uint8_t *)UART4RxBuf, size, 0xFFFF);

    HAL_UARTEx_ReceiveToIdle_DMA(Rs232_2_HandleS, (uint8_t *)UART4RxBuf, UART_BUFF_SIZE);
}

static void UART4_TxCpltCallback(UART_HandleTypeDef *huart)
{
}

static void UART4_ErrorCallback(UART_HandleTypeDef *huart)
{
    HAL_UARTEx_ReceiveToIdle_DMA(Rs232_2_HandleS, (uint8_t *)UART4RxBuf, UART_BUFF_SIZE);
}

// 串口5 rs422_1
extern float g_Aidata[];
static void UART5_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
//    HAL_UART_Transmit(Rs422_1_HandleS, (uint8_t *)UART5RxBuf, size, 0xFFFF);
    if (size >= 2)
    {
        if (UART5RxBuf[0] == 0x01 && UART5RxBuf[1] == 0x06) // 写指令返回
        {
						//不进行处理
						powerState.commcnt= 0x0A;
        }
        else if (UART5RxBuf[0] == 0x01 && UART5RxBuf[1] == 0x03) // 电源状态反馈
        {
					  //CRC校验
//						memset(powerRxBuf,0,sizeof(powerRxBuf));
						powerState.updateFlag = 1;
					  memcpy(powerRxBuf,(void *)UART5RxBuf,32);
					  powerState.commcnt= 0x0A;
        }
     
        HAL_UARTEx_ReceiveToIdle_DMA(Rs422_1_HandleS, (uint8_t *)UART5RxBuf, UART_BUFF_SIZE);
    }
}

static void UART5_TxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_GPIO_WritePin(RE_422_1_GPIO_Port, RE_422_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DE_422_1_GPIO_Port, DE_422_1_Pin, GPIO_PIN_SET);
}

static void UART5_ErrorCallback(UART_HandleTypeDef *huart)
{
    HAL_UARTEx_ReceiveToIdle_DMA(Rs422_1_HandleS, (uint8_t *)UART5RxBuf, UART_BUFF_SIZE);
    HAL_GPIO_WritePin(RE_422_1_GPIO_Port, RE_422_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DE_422_1_GPIO_Port, DE_422_1_Pin, GPIO_PIN_SET);
}

// 串口6 rs485-2
static void UART6_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
    HAL_GPIO_WritePin(RS485_EN2_GPIO_Port, RS485_EN2_Pin, GPIO_PIN_SET);
    HAL_UART_Transmit(Rs485_2_HandleS, (uint8_t *)UART6RxBuf, size, 0xFFFF);
    HAL_GPIO_WritePin(RS485_EN2_GPIO_Port, RS485_EN2_Pin, GPIO_PIN_RESET);

    HAL_UARTEx_ReceiveToIdle_DMA(Rs485_2_HandleS, (uint8_t *)UART6RxBuf, UART_BUFF_SIZE);
}

static void UART6_TxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_GPIO_WritePin(RS485_EN2_GPIO_Port, RS485_EN2_Pin, GPIO_PIN_RESET);
}

static void UART6_ErrorCallback(UART_HandleTypeDef *huart)
{
    HAL_UARTEx_ReceiveToIdle_DMA(Rs485_2_HandleS, (uint8_t *)UART6RxBuf, UART_BUFF_SIZE);
    HAL_GPIO_WritePin(RS485_EN2_GPIO_Port, RS485_EN2_Pin, GPIO_PIN_RESET);
}

// 串口初始化
void USART_CommInit(void)
{
    // 初始化串口1
    Rs485_1_HandleS = &huart1;
    HAL_GPIO_WritePin(RS485_EN1_GPIO_Port, RS485_EN1_Pin, GPIO_PIN_RESET);
    HAL_UART_RegisterRxEventCallback(Rs485_1_HandleS, UART1_RxEventCallback);
    HAL_UARTEx_ReceiveToIdle_DMA(Rs485_1_HandleS, (uint8_t *)UART1RxBuf, UART_BUFF_SIZE);
    HAL_UART_RegisterCallback(Rs485_1_HandleS, HAL_UART_ERROR_CB_ID, UART1_ErrorCallback);
    HAL_UART_RegisterCallback(Rs485_1_HandleS, HAL_UART_TX_COMPLETE_CB_ID, UART1_TxCpltCallback);

    // 初始化串口2
    Rs422_2_HandleS = &huart2;
    HAL_GPIO_WritePin(RE_422_2_GPIO_Port, RE_422_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DE_422_2_GPIO_Port, DE_422_2_Pin, GPIO_PIN_SET);
    HAL_UART_RegisterRxEventCallback(Rs422_2_HandleS, UART2_RxEventCallback);
    HAL_UARTEx_ReceiveToIdle_DMA(Rs422_2_HandleS, (uint8_t *)UART2RxBuf, UART_BUFF_SIZE);
    HAL_UART_RegisterCallback(Rs422_2_HandleS, HAL_UART_ERROR_CB_ID, UART2_ErrorCallback);
    HAL_UART_RegisterCallback(Rs422_2_HandleS, HAL_UART_TX_COMPLETE_CB_ID, UART2_TxCpltCallback);

    // 初始化串口3
    Rs232_1_HandleS = &huart3;
    HAL_UART_RegisterRxEventCallback(Rs232_1_HandleS, UART3_RxEventCallback);
    HAL_UARTEx_ReceiveToIdle_DMA(Rs232_1_HandleS, (uint8_t *)UART3RxBuf, UART_BUFF_SIZE);
    HAL_UART_RegisterCallback(Rs232_1_HandleS, HAL_UART_ERROR_CB_ID, UART3_ErrorCallback);
    HAL_UART_RegisterCallback(Rs232_1_HandleS, HAL_UART_TX_COMPLETE_CB_ID, UART3_TxCpltCallback);

    // 初始化串口4
    Rs232_2_HandleS = &huart4;
    HAL_UART_RegisterRxEventCallback(Rs232_2_HandleS, UART4_RxEventCallback);
    HAL_UARTEx_ReceiveToIdle_DMA(Rs232_2_HandleS, (uint8_t *)UART4RxBuf, UART_BUFF_SIZE);
    HAL_UART_RegisterCallback(Rs232_2_HandleS, HAL_UART_ERROR_CB_ID, UART4_ErrorCallback);
    HAL_UART_RegisterCallback(Rs232_2_HandleS, HAL_UART_TX_COMPLETE_CB_ID, UART4_TxCpltCallback);

    // 初始化串口5
    Rs422_1_HandleS = &huart5;
    HAL_GPIO_WritePin(RE_422_1_GPIO_Port, RE_422_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DE_422_1_GPIO_Port, DE_422_1_Pin, GPIO_PIN_SET);
    HAL_UART_RegisterRxEventCallback(Rs422_1_HandleS, UART5_RxEventCallback);
    HAL_UARTEx_ReceiveToIdle_DMA(Rs422_1_HandleS, (uint8_t *)UART5RxBuf, UART_BUFF_SIZE);
    HAL_UART_RegisterCallback(Rs422_1_HandleS, HAL_UART_ERROR_CB_ID, UART5_ErrorCallback);
    HAL_UART_RegisterCallback(Rs422_1_HandleS, HAL_UART_TX_COMPLETE_CB_ID, UART5_TxCpltCallback);

    // 初始化串口6
    Rs485_2_HandleS = &huart6;
    HAL_GPIO_WritePin(RS485_EN2_GPIO_Port, RS485_EN2_Pin, GPIO_PIN_RESET);
    HAL_UART_RegisterRxEventCallback(Rs485_2_HandleS, UART6_RxEventCallback);
    HAL_UARTEx_ReceiveToIdle_DMA(Rs485_2_HandleS, (uint8_t *)UART6RxBuf, UART_BUFF_SIZE);
    HAL_UART_RegisterCallback(Rs485_2_HandleS, HAL_UART_ERROR_CB_ID, UART6_ErrorCallback);
    HAL_UART_RegisterCallback(Rs485_2_HandleS, HAL_UART_TX_COMPLETE_CB_ID, UART6_TxCpltCallback);
}