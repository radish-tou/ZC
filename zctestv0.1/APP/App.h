//v1.0
#ifndef __APP_H
#define __APP_H

#include "stm32f4xx.h"
#include "host.h"
#include "Bsp.h"
#include "motor.h"
#include "delay.h"
#include "power.h"
#include "sensor.h"

#define ERRCHECKNUM 3
////#define DEBUG
//#ifdef DEBUG	//串口打印阻塞方式
//#define DebugP(str, ...) printf(str, ##__VA_ARGS__)
//#endif


//工作状态
typedef struct
{	
	uint8_t workMode;	//待机(0),支撑(1)，撤收(2),急停(3),二次支撑(4),复位(5),本控远控切换(6),初始化中(0xff)
	uint8_t step;		  //执行流程
}Work_TypeDef;


enum workStateEnum{
	//0：无信号，1：支撑中，2：支撑完成，3：支撑故障，4：撤收中，5：撤收完成，6：撤收故障
	NOSIGNAL,
	EXTENDING,
	EXTENDED,
	EXTENDEDERR,
	RETRACING,
	RETRACED,
	RETRACEERR
};


typedef struct
{
	uint8_t systemState 	     ;//系统状态，  0：无信号，1：正常工作，1：急停状态，2：故障状态 3：待机状态
	uint8_t checkSelfState     ;//自检状态，  0：无信号，1：自检中，2：自检正常，3：自检异常
	uint8_t workState		       ;//工作状态，  0：无信号，1：支撑中，2：支撑完成，3：支撑故障，4：撤收中，5：撤收完成，6：撤收故障
	uint8_t controlMode		     ;//控制模式，  0：远控模式，1：本控模式	
	uint8_t timeCheck	         ;//时间检查，  0: 无信号，1：支撑正常，2：支撑超时，3：撤收正常，4：撤收超时 
	uint8_t extendTime         ;//伸出时间，  单位秒
	uint8_t retraceTime	       ;//撤收时间，  单位秒
	uint8_t raw     	         ;//方位角度，  单位0.1°
	uint8_t pitch              ;//俯仰角度，  单位0.1°
	uint8_t localSystemComm    ;//操作盒通讯状态，  0：未连接，1：连接，
	uint8_t remoteSystemComm	 ;//远控系统通信状态，0：未连接，1：连接，
	uint8_t angleSensorComm;	 ;//倾角传感器状态，  0：未连接，1：连接，
	uint8_t forceSensorComm;	 ;//力传感器状态，    0：未连接，1：连接，bit1~6：分别对应通道1~6
	uint8_t amp[6];            ;//作动器1~6电流
	uint8_t force[6];          ;//作动器1~6触地力
	uint8_t ampMode            ;//电流模式          0:正常模式，1：电流模式
	uint8_t overTravelAlarm    ;//超行程报警 0：无信号，1：正常，bit2~7：分别对应通道1~6超行程
	uint8_t DIState;           ;//IO报警
	uint8_t extPowerLock ;     ;//外部供电互锁状态 0:正常，1：异常
	uint8_t lockState ;        ;//锁紧状态 bit1~6：0:锁紧到位，1：未到位
    uint8_t unlockState ;      ;//释放状态 bit1~6：0:释放到位，1：未到位
	uint16_t totalLockTime     ;//累计锁紧时间，  单位秒
	uint8_t driverAlarm1;      ;//瞬态可恢复类驱动器报警，bit1~6：0:正常，1：报警
	uint8_t driverAlarm2;      ;//条件可恢复类驱动器报警，bit1~6：0:正常，1：报警
	uint8_t driverAlarm3;      ;//不可现场恢复类驱动器报警，bit1~6：0:正常，1：报警
	uint8_t forceExceed;       ;//力超载报警，bit1~6：0:正常，1：报警
	uint8_t forceAccuracy      ;//力精度报警，bit1~6：0:正常，1：报警
	uint8_t forceState         ;//力传感器异常报警，bit1~6：0:正常，1：报警
}State_TypeDef;

extern Work_TypeDef work;
extern State_TypeDef State;

void InitApp(void);
void DoApp(void);
void alarmSystem(void); 
void AnalysisCmd(void);
void Action(void);

void idleMode(uint8_t step);  //待机模式
 
void extensionMode(uint8_t step); //伸出

void retractionMode(uint8_t step); //撤收

void stopMode(uint8_t step); //急停

void secAdjust(uint8_t step); //二次调力

void resetMode(uint8_t step); //复位

#endif  // !APP_H