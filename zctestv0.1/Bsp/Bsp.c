#include "CH394Q_Dev.h"
#include "CAN_Comm.h"
#include "Do_Ctrl.h"
#include "Ai_Acq.h"
#include "RS232_422_485.h"
#include "Di_Acq.h"
#include "w25q128.h"
#include "Bsp.h"

void Bsp_Init(void)
{
	CH394Q_Init();
	USART_CommInit();
	CAN_CommInit();
	TPC51701_Init();
	W25QXX_Init();
}

void Bsp_Poll(void)
{
	CH394Q_Poll();
	Di_Poll();
	TPC51701_Poll();
}