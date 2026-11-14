#include "App.h"


Work_TypeDef work;
State_TypeDef State;

void InitApp(){
	work.workMode = 0xff; //进入初始化状态
	
}

void AnalysisCmd(){
	
	if(work.workMode == 0xff){ //初始化中
		work.workMode = 0x00;
		return ;
	}
	uint8_t curCmd = host.cmd;
	switch(curCmd){
		case 0x00://待机
			break;
		
		case 0x01:  //支撑
			if(work.workMode == 0x00){
				work.step = 0;
				work.workMode = 0x01;
				host.cmd = 0x00;
			}
			
		case 0x02:  //撤收
			if(work.workMode == 0x00){
				work.step = 0;
				work.workMode = 0x02;
				host.cmd = 0x00;
			}
			break;
			
		case 0x03:  //急停
				work.workMode = 0x03;
				work.step = 0;
				host.cmd = 0x00;
				break;
		
		case 0x04:  //二次调力
			if(work.workMode == 0x00){
				work.step = 0;
				work.workMode = 0x04;
				host.cmd = 0x00;
			}
			break;
			
		case 0x05:  //复位
				work.workMode = 0x01;
				work.step = 0;
				host.cmd = 0x00;
				break;
		
		case 0x06:  //本控远控切换
			  State.controlMode =  !State.controlMode;
				host.cmd = 0x00;
				break;
	}
}

void Action(){
	
	uint8_t cur_workMode = work.workMode;
	uint8_t cur_workStep = work.step;
	
	switch (cur_workMode)
	{
	case 0x00: // 待机模式
			idleMode(cur_workStep);
	break;

	case 0x01: // 支撑
		extensionMode(cur_workStep);
	break;

	case 0x02: // 撤收
		retractionMode(cur_workStep);
	break;

	case 0x03: // 急停
		stopMode(cur_workStep);
	break;

	case 0x04: // 二次调力
		secAdjust(cur_workStep);
	break;

	case 0x05: //复位
		resetMode(cur_workStep);
	break;
	}
}

void DoApp()  //转台工作流程
{
	
	AnalysisCmd(); //指令解析
    Action();      //指令执行
	powerPoll();   //电源状态更新
	alarmSystem(); //报警系统
}	


void idleMode(uint8_t step){
	
}



void alarmSystem(void){
	static uint8_t forceExceedCheckNum[MOTORNUM];
	static uint8_t forceAccuracyCheckNum[MOTORNUM];
	static uint8_t forceStateCheckNum[MOTORNUM];
	
	for(int i = 0; i<MOTORNUM; i++){
		
			//1、力传感器检测
		
		    //任意时刻进行力超限检测
			if(forceSensor[i].Value > para.maxForce){
				forceExceedCheckNum[i] ++;
			}
			else{
				State.forceExceed &= ~(0x02<<i);
				forceExceedCheckNum[i]  = 0;
			}
					
			if(forceExceedCheckNum[i] > ERRCHECKNUM){
					State.forceExceed |= (0x02<<i);
				}
				
			//  处于支撑中时进行支撑力精度检测
		  if(State.workState == EXTENDED){ 
				if((forceSensor[i].Value > para.standForce + para.forceOffset)&&(forceSensor[i].Value < para.standForce - para.forceOffset)){
					forceAccuracyCheckNum[i] ++;
				}
				else{
					forceExceedCheckNum[i] = 0;
					State.forceAccuracy &= ~(0x02<<i);
				}
				
				if(forceAccuracyCheckNum[i] > ERRCHECKNUM){
					State.forceAccuracy |= (0x02<<i);
				}	
			 }
			
			// 通信测试
		 if(forceSensor[i].commCnt == 0){
				State.forceSensorComm &= ~(0x02<<i);
		 }
		 else{
				State.forceSensorComm |=(0x02<<i);
		 }
			 
		   // 力传感器数据异常检测
		 if(forceSensor[i].commCnt != 0 && (forceSensor[i].amp > para.maxForceSensorAmp || forceSensor[i].amp < para.minForceSensorAmp)){
				forceStateCheckNum[i] ++;
			}
			else{
				State.forceExceed &= ~(0x02<<i);
				forceStateCheckNum[i]  = 0;
			}
					
			if(forceStateCheckNum[i] > ERRCHECKNUM){
				State.forceExceed |= (0x02<<i);
				}
	}

			//2、时间检测
			//2.1支撑完成时进行支撑时间检测
			if(State.workState == EXTENDED){ 
				if(State.extendTime > para.maxExtendTime){
					State.timeCheck = 0x02;
				}
				else{
					State.timeCheck = 0x01;
				}
			}
			//2.2撤收完成时进撤收时间检测
			if(State.workState == RETRACED){ 
				if(State.retraceTime > para.maxRetraceTime){
					State.timeCheck = 0x04;
				}
				else{
					State.timeCheck = 0x03;
				}
			 }
			 //3、操作盒检测
			 //3.1操作盒通信状态检测
			if( host.localComm == 0){
				State.localSystemComm = 0;
			}
			else{
				State.localSystemComm = 1;
			}
			
			 //4、远控系统监测
			 //4.1、远控系统通信状态检测
			if( host.remoteComm == 0){
				State.localSystemComm = 0;
			}
			else{
				State.localSystemComm = 1;
			}
		
			//7、远控系统通信状态检测
			if( host.remoteComm == 0){
				State.localSystemComm = 0;
			}
			else{
				State.localSystemComm = 1;
			}
	}
	
	
	


void extensionMode(uint8_t step){
	switch(step){
		
	case 0:
		State.controlMode = EXTENDING;
	break;
		
	case 1:
	break;	
	}
	
	uint8_t ch = 1;
	uint8_t mode = 1;
	MotorEnable(ch);
	delay_ms(10);
	MotorDisable(ch);
	delay_ms(10);
	MotorMode(ch, mode);
	delay_ms(10);
	MotorProfileSpeed(ch,20); //轮廓速度
	delay_ms(10);
	MotorPostion(ch, 21);      //位置设置
	delay_ms(10);
	MotorSpeed(ch,100);      //速度设置，rpm
	delay_ms(10);
	MotorStop(ch);                     //电机停止
	delay_ms(10);
	MotorAmp(ch,500);
	delay_ms(10);
	MotorAcce(ch,20,40);
	delay_ms(10);
	work.workMode = 0x0;
}
void retractionMode(uint8_t step){

} //撤收

void stopMode(uint8_t step){

}
void secAdjust(uint8_t step){

} //二次调力

void resetMode(uint8_t step){

} //复位