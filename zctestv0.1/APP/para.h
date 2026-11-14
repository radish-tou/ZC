#ifndef __PARA_H
#define __PARA_H

#include "App.h"


//默认参数
typedef struct
{	

	float maxSpeed;   //最大速度
	float maxTravel;  //最大行程
	float minTravel;  //最小行程
	
	
	float standForce;   //额定力
	float forceOffset;  //力偏差
	float maxForce;   //额定力
	
	float maxExtendTime;   //支撑超限时间
	float maxRetraceTime;   //撤收时间
	
	float maxForceSensorAmp; //力传感器反馈电流最小值
	float minForceSensorAmp; //力传感器反馈电流最大值
}Para_TypeDef;

typedef struct
{	
	volatile uint32_t head;	//上位机发送的数据帧头
	volatile uint32_t mid1;	//上位机发送的方位零位
	volatile uint32_t mid2;	//上位机发送的俯仰零位
	volatile uint32_t rightLimit;//上位机发送的方位右极限位置
	volatile uint32_t leftLimit;//上位机发送的方位左极限位置
	volatile uint32_t upLimit ;	
	volatile uint32_t downLimit ;//上位机发送的
	uint16_t right_xw_ks;  //方位最大速度右限位开始角度
	uint16_t left_xw_ks;   //方位最大速度左限位开始角度
	uint16_t right_xw_end;//方位最大速度右限位结束角度
  uint16_t left_xw_end;   //方位最大速度左限位结束角度	
	uint16_t up_xw_ks;    //上限位最大速度开始角度
	uint16_t down_xw_ks;   //下限位最大速度开始角度

	uint16_t up_xw_end;    //上限位最大速度限位结束角度
	uint16_t down_xw_end;   //下限位最大速度限位结束角度
	
	float right_k_;      //方位右限位参数
	float left_k;       //方位左限位参数
  float up_k;      //显旁俯仰上限位参数
	float down_k;       //显旁俯仰下限位参数
	//离线计时器
	volatile uint32_t sum;	//校验和
}Para_read_TypeDef;

extern Para_read_TypeDef para_read;
extern Para_TypeDef para;
void ParaInst(UART_HandleTypeDef *p);
void ReadPara(void);

#endif  // 
