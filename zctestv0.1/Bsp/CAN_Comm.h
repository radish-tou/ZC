#ifndef CAN_COMM_H
#define CAN_COMM_H
#include "can.h"
#include <string.h>
#include "Di_Acq.h"
#include "Do_Ctrl.h"
#include "CAN_Comm.h"
#include "Ai_Acq.h"
#include "w25q128.h"
#include "host.h"
//CAN发送参数
typedef struct
{		
	uint32_t id;
	uint32_t dlc;
	uint8_t data[8];
}CanSendData_TypeDef;


void CAN_CommInit(void);


int CAN_Write(CAN_HandleTypeDef *hcan, uint32_t idxExtIdUI, const uint8_t *dataPUB, uint32_t DLC_UI);

int CanSendStd(uint8_t ch,CanSendData_TypeDef *p); //发送标准帧

int CanSendExt(uint8_t ch,CanSendData_TypeDef *p); //发送扩展帧

#endif
