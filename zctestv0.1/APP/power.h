#ifndef __POWER_H
#define __POWER_H
#include "App.h"
#include "RS232_422_485.h"
#include "string.h"
#include <stddef.h>
#include <stdint.h>
#define POWERRXBUFFERSIZE 32

//电源状态
typedef struct
{		
	uint16_t inputVoltage;		      //输入电压,0.1V
	uint16_t inputCurrent;		      //输入电流,0.01A
	uint16_t outputVoltage1;		    //输出电压1,0.1V
	uint16_t outputCurrent1;		    //输出电流1,0.01A
	uint16_t outputVoltage2;		    //输出电压2,0.1V
	uint16_t outputCurrent2;		    //输出电压2,0.01A
	uint16_t temperature1;		      //温度1,0.1°C
	uint16_t temperature2;         	//温度2,0.1°C
	uint16_t inputPowerSwitchState;	//输入电源开关状态
	uint16_t powerSwitchState1;	    //电源1开关状态,0 关闭 1上电中,2上电完成
	uint16_t powerSwitchState2;	    //电源2开关状态,0 关闭 1上电中,2上电完成
	uint16_t alarmState;	          //故障状态
	uint16_t ID;	                  //机器身份ID,01：3050W设备,:02：900W设备,03：200W设备
	uint8_t  commcnt;	 							//离线计时器
	uint8_t  updateFlag;	          //电源状态更新标志
}powerState_TypeDef;

extern uint8_t powerRxBuf[POWERRXBUFFERSIZE];
extern powerState_TypeDef powerState;

void powerInit(void);
void powerPoll(void);
void powerSwitchControl(uint8_t ch,uint8_t state);
void powerGetState(void);
void powerStateUpdata(void);
uint16_t modbus_crc16(const uint8_t *data, size_t length);
//0:1	输入电压状态	0：正常 1：过压 2：欠压
//2	输入电流状态	0：正常 1：过流
//3:4	输出电压状态1	0：正常 1：过压 2：欠压
//5	输出电流状态1	0：正常 1：过流
//6:7	输出电压状态2	0：正常 1：过压 2：欠压
//8	输出电流状态2	0：正常 1：过流
//9	温度状态1	0：正常 1：过温
//10	温度状态2	0：正常 1：过温
//11:14	保留	默认0 无意义
//15	故障总状态	0：系统正常1：故障状态


#endif