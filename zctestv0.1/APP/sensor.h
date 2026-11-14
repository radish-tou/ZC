#ifndef __SENSOR_H
#define __SENSOR_H

#include "App.h"

//陀螺状态
typedef struct
{		
	float Value;		  //力
	float amp;        //力传感器反馈的原始电流
	uint16_t commCnt;	//离线计时器
}forceSensor_TypeDef;

typedef struct
{		
	float raw;		//方位角
	float pitch;  //俯仰角
	uint16_t commCnt;	//离线计时器
}angleSensor_TypeDef;

extern angleSensor_TypeDef angleSensor;

extern forceSensor_TypeDef forceSensor[6];


void sensor_Poll(void);



#endif