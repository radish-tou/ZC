#ifndef HOST_H
#define HOST_H

#include "App.h"
#include "host.h"
#include "RS232_422_485.h"

typedef struct
{	
	volatile float cmd;		       //指令
	volatile uint8_t localComm;  //操作盒通信
	volatile uint8_t remoteComm; //远端通信

}Host_TypeDef;

//显旁手柄参数
typedef struct
{
	volatile int16_t angleSpeed1;	//方位速度
	volatile int16_t angleSpeed2;	//俯仰速度
	volatile uint8_t fault;			//故障标志
	volatile uint8_t trackEnable;	//跟踪启动
	volatile uint8_t lampEnable;	//装置选择
	volatile uint8_t lampSel ;		//S7的C位
	volatile uint8_t commCnt;		//离线计时器
	volatile uint16_t enableCnt;	//10分钟计时器
}HandAux_TypeDef;

typedef struct
{
	volatile uint32_t head;	//上位机发送的数据帧头
	volatile uint32_t mid1;	//上位机发送的方位零位
	volatile uint32_t mid2;	//上位机发送的俯仰零位
	volatile uint32_t rightLimit;//上位机发送的方位右极限位置
	volatile uint32_t leftLimit;//上位机发送的方位左极限位置
	volatile uint32_t upLimit ;	
	volatile uint32_t downLimit ;//上位机发送的
	//离线计时器
	volatile uint32_t sum;	//校验和
}can_TypeDef;

extern Host_TypeDef host;

extern can_TypeDef can;

void HostPoll(void);

void LocalSystemSend(uint8_t* data,uint16_t size);

void remoteSystemSend(uint8_t* data,uint16_t size);

#endif  // 
 