#include "CAN_Comm.h"
#include "motor.h"
#include "sensor.h"
/*
55 AA 00 00 00 00 00 00：6路do控制指令，55 aa为帧头，每个do占1字节，0x00表示关闭，0x01表示打开；
66 BB 00 00 00 00 00 00：8路路ai查询指令；
66 BB 01 02 03 04 05 06：8路路ai查询指令回复，66 bb为帧头，第3字节代表通道ai通道号，4-7字节代表通道ai值，8字节为0；
77 CC 00 00 00 00 00 00：5路路di查询指令；
77 CC 00 00 00 00 00 00：5路路di查询指令回复，77 cc为帧头，3-7字节代表1-5路路di通道值，每个di占1字节，0x00表示关闭，0x01表示打开,8字节为0；
88 DD 00 00 00 00 00 00：写入FLASH 数据，88 DD为帧头，后面6个字节为写入到flash的数据
99 EE 00 00 00 00 00 00：读取写入的数据，99 EE 为帧头，后面的00位填充
99 EE 00 00 00 00 00 00：读取flash数据的回复，99 EE 为帧头，后面字节跟的是读出的数据
*/

static uint32_t txMailboxUI;

typedef struct
{
	uint32_t idxExtIdUI;
	uint32_t DLC_UI;
	uint8_t dataUB[8];
} CAN_infoCanData_TYPE;

static void CAN1_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rxHeaderS;
	CAN_infoCanData_TYPE tempDataS = {0};
	uint8_t dataUB[8];

	while (HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0) != 0)
	{
		if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeaderS, dataUB) == HAL_OK)
		{
			tempDataS.idxExtIdUI = rxHeaderS.StdId;
			tempDataS.DLC_UI = rxHeaderS.DLC;
			memcpy(tempDataS.dataUB, dataUB, tempDataS.DLC_UI);
			if (tempDataS.DLC_UI == 8)
			{
				if (tempDataS.dataUB[0] == 0x55 && tempDataS.dataUB[1] == 0xAA) // 6路do控制指令
				{
						host.cmd = 1;
//					Do_OutCtl(tempDataS.dataUB, tempDataS.DLC_UI);
//					CAN_Write(hcan, 0x01020304, tempDataS.dataUB, tempDataS.DLC_UI);
				}
				else if (tempDataS.dataUB[0] == 0x66 && tempDataS.dataUB[1] == 0xBB) // 8路路ai查询指令
				{
					TPC51701_ReportAiData(hcan, tempDataS.dataUB, tempDataS.DLC_UI);
				}
				else if (tempDataS.dataUB[0] == 0x77 && tempDataS.dataUB[1] == 0xCC) // 5路路di查询指令
				{
					Di_DataAcq(tempDataS.dataUB);
					tempDataS.dataUB[7] = 0;
					CAN_Write(hcan, 0x01020304, tempDataS.dataUB, tempDataS.DLC_UI);
				}
				else if (tempDataS.dataUB[0] == 0x88 && tempDataS.dataUB[1] == 0xDD) // 写入FLASH 数据
				{
					W25QXX_Write(tempDataS.dataUB + 2, 0, tempDataS.DLC_UI - 2);
				}
				else if (tempDataS.dataUB[0] == 0x99 && tempDataS.dataUB[1] == 0xEE) // 读取写入的数据
				{
					W25QXX_Read(tempDataS.dataUB + 2, 0, tempDataS.DLC_UI - 2);
					CAN_Write(hcan, 0x01020304, tempDataS.dataUB, tempDataS.DLC_UI);
				}
			}
		}
	}
}

static void CAN2_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rxHeaderS;
	CAN_infoCanData_TYPE tempDataS;
	uint8_t dataUB[8];

	while (HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO1) != 0)
	{
		if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &rxHeaderS, dataUB) == HAL_OK)
		{
			tempDataS.idxExtIdUI = rxHeaderS.ExtId;
			tempDataS.DLC_UI = rxHeaderS.DLC;
			memcpy(tempDataS.dataUB, dataUB, tempDataS.DLC_UI);

			if (tempDataS.idxExtIdUI == 0x605 && tempDataS.DLC_UI == 8)
			{
					uint32_t tempRaw   = tempDataS.dataUB[0] & (tempDataS.dataUB[1]<<8) & (tempDataS.dataUB[2]<16);
					angleSensor.raw = (float)(tempDataS.dataUB[3] == 0x00 ? tempRaw:-tempRaw)/10000;
				
				  uint32_t tempPitch = tempDataS.dataUB[4] & (tempDataS.dataUB[5]<<8) & (tempDataS.dataUB[6]<<16);
					angleSensor.pitch = (float)(tempDataS.dataUB[7] == 0x00 ? tempPitch:-tempPitch)/10000;
				
					angleSensor.commCnt = 0x0A;
				}
			}
		}
}

int CAN_Write(CAN_HandleTypeDef *hcan, uint32_t idxExtIdUI, const uint8_t *dataPUB, uint32_t DLC_UI)
{
	static CAN_TxHeaderTypeDef _TxHeaderS = {
		.IDE = CAN_ID_EXT,
		.RTR = CAN_RTR_DATA,
		.TransmitGlobalTime = DISABLE};

	if (DLC_UI > 8)
	{
		DLC_UI = 8;
	}
	if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) != 0)
	{
		_TxHeaderS.DLC = DLC_UI;
		_TxHeaderS.ExtId = idxExtIdUI;
		return HAL_CAN_AddTxMessage(hcan, &_TxHeaderS, (uint8_t *)dataPUB, &txMailboxUI);
	}
	return HAL_OK;
}

void CAN_CommInit(void)
{
	// CAN过滤器参数配置
	CAN_FilterTypeDef CAN_FilterTypeDefSture;
	CAN_FilterTypeDefSture.FilterBank = 0;						          // 配置过滤器0
	CAN_FilterTypeDefSture.FilterScale = CAN_FILTERSCALE_32BIT; // 配置为32位过滤器
	CAN_FilterTypeDefSture.FilterMode = CAN_FILTERMODE_IDMASK;	// 掩码模式
	CAN_FilterTypeDefSture.FilterIdLow = 0;
	CAN_FilterTypeDefSture.FilterMaskIdLow = 0;
	CAN_FilterTypeDefSture.FilterIdHigh = 0;						        //
	CAN_FilterTypeDefSture.FilterMaskIdHigh = 0;				      	//
	CAN_FilterTypeDefSture.FilterFIFOAssignment = CAN_FILTER_FIFO0; // 过滤器0关联到FIFO0
	CAN_FilterTypeDefSture.FilterActivation = ENABLE;				// 激活过滤器0
	CAN_FilterTypeDefSture.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterTypeDefSture);

	CAN_FilterTypeDefSture.FilterBank = 15;						// 配置过滤器15
	CAN_FilterTypeDefSture.FilterScale = CAN_FILTERSCALE_32BIT; // 配置为32位过滤器
	CAN_FilterTypeDefSture.FilterMode = CAN_FILTERMODE_IDMASK;	// 掩码模式
	CAN_FilterTypeDefSture.FilterIdLow = 0;
	CAN_FilterTypeDefSture.FilterMaskIdLow = 0;
	CAN_FilterTypeDefSture.FilterIdHigh = 0;
	CAN_FilterTypeDefSture.FilterMaskIdHigh = 0;
	CAN_FilterTypeDefSture.FilterFIFOAssignment = CAN_FILTER_FIFO1; // 过滤器15关联到FIFO1
	CAN_FilterTypeDefSture.FilterActivation = ENABLE;				// 激活过滤器15
	HAL_CAN_ConfigFilter(&hcan2, &CAN_FilterTypeDefSture);

	HAL_CAN_RegisterCallback(&hcan1, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, CAN1_RxFifo0MsgPendingCallback);
	HAL_CAN_RegisterCallback(&hcan2, HAL_CAN_RX_FIFO1_MSG_PENDING_CB_ID, CAN2_RxFifo1MsgPendingCallback);
	HAL_CAN_Start(&hcan1);
	HAL_CAN_Start(&hcan2);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING);
}



int CanSendStd(uint8_t ch,CanSendData_TypeDef *p)  //发送标准帧
{
	CAN_HandleTypeDef * pCan;
	static CAN_TxHeaderTypeDef	_TxHeader =	{
  											.IDE = CAN_ID_STD,
		                  	.RTR = CAN_RTR_DATA,
											.TransmitGlobalTime = DISABLE
											};
	_TxHeader.StdId = p->id;
	_TxHeader.DLC = p->dlc;
											
											
	if(ch == 1){
		pCan = &hcan1;
		if(HAL_CAN_GetTxMailboxesFreeLevel(pCan) != 0 )
			return HAL_CAN_AddTxMessage(pCan,&_TxHeader,p->data,&txMailboxUI);
	}		
	else if(ch == 2){
		pCan = &hcan2;
		if(HAL_CAN_GetTxMailboxesFreeLevel(pCan) != 0 )
				return HAL_CAN_AddTxMessage(pCan,&_TxHeader,p->data,&txMailboxUI);
	}
	return HAL_OK;
}

int CanSendExt(uint8_t ch,CanSendData_TypeDef *p)  //发送扩展帧
{
	CAN_HandleTypeDef * pCan;
	static CAN_TxHeaderTypeDef	_TxHeader =	{
  											.IDE = CAN_ID_EXT,
		                  	.RTR = CAN_RTR_DATA,
											.TransmitGlobalTime = DISABLE
											};
	_TxHeader.StdId = p->id;
	_TxHeader.DLC = p->dlc;
											
	if(ch == 1){
		pCan = &hcan1;
		if(HAL_CAN_GetTxMailboxesFreeLevel(pCan) != 0 )
			return HAL_CAN_AddTxMessage(pCan,&_TxHeader,p->data,&txMailboxUI);
	}		
	else if(ch == 2){
		pCan = &hcan2;
		if(HAL_CAN_GetTxMailboxesFreeLevel(pCan) != 0 )
			return HAL_CAN_AddTxMessage(pCan,&_TxHeader,p->data,&txMailboxUI);
	}
	return HAL_OK;
}