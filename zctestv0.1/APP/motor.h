#ifndef __MOTOR_H
#define __MOTOR_H
#include "App.h"
#include "CAN_Comm.h"
#include "para.h"
//电机参数

#define MOTORNUM 6

typedef struct
{		
	//电机反馈信息

	uint16_t err;		  //警告码
	uint16_t state;		//状态字	
	int16_t speed;		//实时速度 rpm/m
	int16_t amp;		  //母线电流 mA
	
	int32_t postion;	//位置
	int32_t voltage;	//母线电压，0.1V
	uint16_t temp;	  //电机温度°C
	
	uint16_t commCnt;	    //离线计时器
	uint8_t motorEnable;	//电机使能，使能(1),未使能(0)
	uint8_t motorMode;	  //电机工作模式，电流(1)	速度环(2),位置(3),
	uint8_t brakeState;  //抱闸状态，1：抱闸
	
	//电机控制信息
	float speedTag;		//目标速度
	int32_t postionTag;	//目标位置

}MotorState_TypeDef;

extern MotorState_TypeDef motor[MOTORNUM];//左上


void MotorEnable(uint8_t ch);
void MotorDisable(uint8_t ch);
void MotorMode(uint8_t ch,uint8_t mode);
void MotorProfileSpeed(uint8_t ch,float speed); //轮廓速度
void MotorPostion(uint8_t ch,float angle);      //位置设置

void MotorSpeed(uint8_t ch,int16_t speed);      //速度设置，rpm

void MotorStop(uint8_t ch);                     //电机停止

void MotorAmp(uint8_t ch,int16_t amp);

void MotorAcce(uint8_t ch,uint16_t acc,uint16_t deAcc);


void MotorInst(void);
void MotorPoll(void);


//驱动警告码
//bit0	自检
//bit1	过压
//bit2	欠压
//bit3	母线过流
//bit4	软件过流
//bit5	过载
//bit6	驱动器过温
//bit7	电机过温
//bit8	过速
//bit9	位置反馈异常
//Bit[14:10]	预留
//Bit15	其它故障

// 伺服状态码
//bit0	系统告警
//bit1	上电完成
//bit2	就绪
//bit3	运行
//bit4	上限
//bit5	下限
//bit6	解抱闸
//bit7	抱闸
//bit8	速度到达（速度到达设定速度）
//bit9	位置到达（位置到达设定位置）
//bit10	泄放（1：泄放开 0：泄放关）
//bit11	当前状态：电流环
//bit12	当前状态：速度环
//bit13	当前状态：位置环
//bit14	保留
//bit15	保留

//系统故障码
//bit0	相过流
//bit1	上限位
//bit2	下限位
//bit3	母线过流
//bit4	过速
//bit5	过载
//bit6	电机过温
//bit7	母线过压
//bit8	母线欠压
//bit9	DI功能配置错误
//Bit10	泄放过流
//Bit16	电流采样初始化异常
//Bit25	硬件检测过流
//Bit26	驱动器过温
//Bit27	编码器故障
//Bit28	硬件检测过压
//Bit29	抱闸故障



#endif  // 
