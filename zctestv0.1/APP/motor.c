/*伺服驱动器*/
/*version:V0.00.05*/
/*电机运动控制*/
#include "motor.h"

//定时器
typedef struct
{
  volatile uint8_t tim2_1;
  volatile uint8_t tim50_1;
	volatile uint8_t tim2_2;
  volatile uint8_t tim50_2;
	volatile uint8_t tim20_1;
	volatile uint8_t tim20_2;
	volatile uint8_t tim20_3;
} TimFlagTypeDef;


static volatile TimFlagTypeDef timFlag;


MotorState_TypeDef motor[MOTORNUM];//电机，



void MotorEnable(uint8_t ch)
{
	static CanSendData_TypeDef _canData ={
	.dlc = 8,
	.data[0] = 0x23,
	.data[1] = 0x03,
	.data[2] = 0x40,
	.data[3] = 0x00,	
	.data[4] = 0x55,
	.data[5] = 0xAA,	
	.data[6] = 0x00,
	.data[7] = 0x00,
	};
	_canData.id = 0x600 + ch;	
	CanSendStd(1,&_canData); //CAN1，标准帧发送
}


void MotorDisable(uint8_t ch)
{
	static CanSendData_TypeDef _canData ={
	.dlc = 8,
	.data[0] = 0x23,
	.data[1] = 0x03,
	.data[2] = 0x40,
	.data[3] = 0x00,	
	.data[4] = 0xAA,
	.data[5] = 0x55,	
	.data[6] = 0x00,
	.data[7] = 0x00,
	};
	_canData.id = 0x600 + ch;	
	CanSendStd(1,&_canData);
	
}

//mode形参:电流环(1),速度环(2),电流(3)
void MotorMode(uint8_t ch,uint8_t mode)
{
	static CanSendData_TypeDef _canData ={
	.dlc = 8,
	.data[0] = 0x2B,
	.data[1] = 0x13,
	.data[2] = 0x30,
	.data[3] = 0x00,
	.data[5] = 0x00,	
	.data[6] = 0x00,
	.data[7] = 0x00,
	};
	
	_canData.data[4] = mode,
	_canData.id = 0x600 + ch;
	
	CanSendStd(1,&_canData);

}
//实际固定设置为0.15RPM = 9°/s
void MotorProfileSpeed(uint8_t ch,float speed)
{
	static CanSendData_TypeDef _canData ={
	.dlc = 8,
	.data[0] = 0x2b,
	.data[1] = 0x51,
	.data[2] = 0x20,
	.data[3] = 0x00,		
	.data[6] = 0x00,
	.data[7] = 0x00,
	};
	
	uint16_t tempSpeed;	

	tempSpeed = (uint16_t)(speed);
	_canData.data[4] = tempSpeed & 0xff;
	_canData.data[5] = tempSpeed >> 8;
	
	_canData.id = 0x600 + ch;
	CanSendStd(1,&_canData);
	
}


void MotorPostion(uint8_t ch,float angle) // 绝对位置
{
	static CanSendData_TypeDef _canData ={
	.dlc = 8,
	.data[0] = 0x2B,
	.data[1] = 0x03,	
	.data[2] = 0x40,	
	.data[3] = 0x00	
	};
	
	int32_t tempPosition;
	
	if(tempPosition <= para.minTravel)
		tempPosition = para.minTravel;
	else if(tempPosition >= para.maxTravel)
		tempPosition = para.maxTravel;

}


//速度指令，单位rpm
static void _MotorSpeed(uint8_t ch,int16_t speed)
{
	static CanSendData_TypeDef _canData ={
	.dlc = 8,
	.data[0] = 0x2b,
	.data[1] = 0x02,	
	.data[2] = 0x40,	
	.data[3] = 0x00,
	.data[6] = 0x00,	
	.data[7] = 0x00
	};
	
	if(speed > para.maxSpeed)
		speed = para.maxSpeed;		
	else if(speed < -para.maxSpeed)
		speed = -para.maxSpeed;
		
	_canData.id = 0x600 + ch;
	_canData.data[4] = speed & 0xff;
	_canData.data[5] = speed >> 8;
	CanSendStd(1,&_canData);
}


//速度，单位rpm
void MotorSpeed(uint8_t ch,int16_t speed)
{
	int16_t tempSpeed = speed;
	_MotorSpeed(ch,tempSpeed);
	
}


//停止
void MotorStop(uint8_t ch)
{
	static CanSendData_TypeDef _canData ={
	.dlc = 8,
	.data[0] = 0x23,
	.data[1] = 0x02,	
	.data[2] = 0x40,	
	.data[3] = 0x00,
	.data[4] = 0x01,
	.data[5] = 0x00,
	.data[6] = 0x00,	
	.data[7] = 0x00
	};
	
	_canData.id = 0x600 + ch;
	CanSendStd(1,&_canData);
}

static void _MotorAmp(uint8_t ch,int16_t amp)
{
	static CanSendData_TypeDef _canData ={
	.dlc = 8,
	.data[0] = 0x2b,
	.data[1] = 0x01,	
	.data[2] = 0x40,	
	.data[3] = 0x00,
	.data[6] = 0x00,	
	.data[7] = 0x00
	};
		
	_canData.data[4] = amp & 0xff;
	_canData.data[5] = amp >> 8;
	_canData.id = 0x600 + ch;
	
	CanSendStd(1,&_canData);
}

//电流模式，mA
void MotorAmp(uint8_t ch,int16_t amp){
	int16_t tempAmp = amp;
	_MotorAmp(ch,tempAmp);
}



void MotorAcce(uint8_t ch,uint16_t acc,uint16_t deAcc)
{
	static CanSendData_TypeDef _canData ={
	.dlc = 8,
	.data[0] = 0x2b,
	.data[1] = 0x43,
	.data[2] = 0x20,
	.data[3] = 0x00,
	.data[6] = 0x00,	
	.data[7] = 0x00
	};
	
	_canData.id = 0x600 + ch;
	
	_canData.data[1] = 0x43;
	_canData.data[4] = acc & 0xff;
	_canData.data[5] = acc >> 8;
	CanSendStd(1,&_canData);
	
	HAL_Delay(10);
	_canData.data[1] = 0x44;
	_canData.data[4] = deAcc & 0xff;
	_canData.data[5] = deAcc >> 8;
	CanSendStd(1,&_canData);
	
}


void MotorInst(void)
{
//	HAL_TIM_RegisterCallback(&htim6,HAL_TIM_PERIOD_ELAPSED_CB_ID,TIM_PeriodElapsedCallback);
//	__HAL_TIM_CLEAR_FLAG(&htim6,TIM_FLAG_UPDATE);
//	HAL_TIM_Base_Start_IT(&htim6);
//	HAL_Delay(20);
//	MotorAccel(1,625);
//	HAL_Delay(20);
//	MotorAccel(2,625);
//	HAL_Delay(20);
//	MotorSpeed(2,0.f);
//	HAL_Delay(20);
//	MotorSpeed(2,0.f);
//	HAL_Delay(20);
////	MotorMode(1,2);
////	MotorMode(2,2);
//	MotorDisable(1);
//	HAL_Delay(20);
//	MotorDisable(2);
//	HAL_Delay(20);
}


void MotorPid1(void)
{


}



void MotorPoll(void)
{
	

}




