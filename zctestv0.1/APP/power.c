#include "power.h"

static uint8_t powerTxBuf[32];

uint8_t powerRxBuf[POWERRXBUFFERSIZE];
powerState_TypeDef powerState;

void powerPoll(){
	static TickType_t powerReqtick = 0;
   if (Millis_Elapsed(powerReqtick) > 500)
    {
			powerGetState(); //每500ms发送一次电池状态查询指令
      powerReqtick = Millis();
    }
		powerStateUpdata(); //电源状态更新
}

void powerStateUpdata(){

			if(powerRxBuf[0] == 0x01 && powerRxBuf[1] == 0x03 && powerState.updateFlag){
					powerState.inputVoltage = powerRxBuf[3]<<8 & powerRxBuf[4];
					powerState.inputCurrent = powerRxBuf[5]<<8 & powerRxBuf[6];
					powerState.outputVoltage1 = powerRxBuf[7]<<8 & powerRxBuf[8];
					powerState.outputVoltage2 = powerRxBuf[9]<<8 & powerRxBuf[10];
					powerState.outputCurrent1 = powerRxBuf[11]<<8 & powerRxBuf[12];
					powerState.outputCurrent2 = powerRxBuf[13]<<8 & powerRxBuf[14];
					powerState.temperature1 = powerRxBuf[15]<<8 & powerRxBuf[16];
					powerState.temperature2 = powerRxBuf[17]<<8 & powerRxBuf[18];
					powerState.inputPowerSwitchState = powerRxBuf[19]<<8 & powerRxBuf[20];
					powerState.powerSwitchState1 = powerRxBuf[21]<<8 & powerRxBuf[22];
					powerState.powerSwitchState2 = powerRxBuf[23]<<8 & powerRxBuf[24];
					powerState.alarmState = powerRxBuf[25]<<8 & powerRxBuf[26];
					powerState.updateFlag = 0;
			}
}

void powerSwitchControl(uint8_t ch,uint8_t state){  //ch 电源1/2  state 0 关闭电源 1 开启电源
	memset(powerTxBuf,0,sizeof(powerTxBuf));
	powerTxBuf[0] = 0x01; //从机地址
	powerTxBuf[1] = 0x06; //写单个寄存器功能码
	powerTxBuf[2] = 0x20; //写入地址高位
	powerTxBuf[3] = 0x00+ch;
	powerTxBuf[4] = 0x00; //写入数据高位
	powerTxBuf[5] = 0x01; 
	uint16_t crc = modbus_crc16(powerTxBuf,6);
	powerTxBuf[6] = (uint8_t)(crc & 0xFF);
	powerTxBuf[7] = (uint8_t)(crc>>8);
	
	HAL_UART_Transmit(Rs422_1_HandleS, powerTxBuf, 8, 0xFFFF);
}

void powerGetState(void){  //获取电源状态
	
	memset(powerTxBuf,0,sizeof(powerTxBuf));
	powerTxBuf[0] = 0x01; //从机地址
	powerTxBuf[1] = 0x03; //读多个寄存器功能码
	powerTxBuf[2] = 0x00; //起始地址
	powerTxBuf[3] = 0x00; 
	powerTxBuf[4] = 0x00; //寄存器数量高位
	powerTxBuf[5] = 0x0c; //寄存器数量低位
	uint16_t crc = modbus_crc16(powerTxBuf,6);
	powerTxBuf[6] = (uint8_t)(crc & 0xFF);
	powerTxBuf[7] = (uint8_t)(crc>>8);
	
	HAL_UART_Transmit(Rs422_1_HandleS, powerTxBuf, 8, 0xFFFF);
}



/**
 * @brief 计算 Modbus RTU CRC-16
 * @param data   输入数据
 * @param length 数据长度（字节）
 * @return 16 位 CRC 数值
 */
uint16_t modbus_crc16(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xFFFFu;
    size_t i;
    if ((data == NULL) && (length != 0u))
    {
        return 0u;
    }

    for (i = 0u; i < length; ++i)
    {
        uint8_t bit;

        /* 当前字节与 CRC 的低 8 位异或 */
        crc ^= (uint16_t)data[i];

        /* 处理 8 个位，LSB 先移出 */
        for (bit = 0u; bit < 8u; ++bit)
        {
            if ((crc & 0x0001u) != 0u)
            {
                crc = (uint16_t)((crc >> 1) ^ 0xA001u);
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}