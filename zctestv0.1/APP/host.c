#include "host.h"

Host_TypeDef host;

uint8_t localHostTxBuf[128];
uint8_t remoteHostTxBuf[128];


void LocalSystemSend(uint8_t* data,uint16_t size){
	
	uint16_t crc = modbus_crc16(data,6);
	
	HAL_UART_Transmit(Rs485_1_HandleS, data, size, 0xFFFF);
}


void remoteSystemSend(uint8_t* data,uint16_t size){
	
	uint16_t crc = modbus_crc16(data,6);
	
	HAL_UART_Transmit(Rs485_1_HandleS, data, size, 0xFFFF);
}