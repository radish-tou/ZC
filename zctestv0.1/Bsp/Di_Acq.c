#include "gpio.h"
#include "Di_Acq.h"
#include "MySys.h"

#define DIO_MASK (uint16_t)0x3ff
Di_infoDisIn_TYPE Di_infoDisIn;

static void Di_ScanDi(void)
{
	static uint16_t val[5] = {0};
	for (uint8_t i = 0; i < 5; i++)
	{
		val[i] <<= 1;
	}
	if (HAL_GPIO_ReadPin(MCU_DI1_GPIO_Port, MCU_DI1_Pin) == GPIO_PIN_RESET)
		val[0] |= 1;
	if (HAL_GPIO_ReadPin(MCU_DI2_GPIO_Port, MCU_DI2_Pin) == GPIO_PIN_RESET)
		val[1] |= 1;
	if (HAL_GPIO_ReadPin(MCU_DI3_GPIO_Port, MCU_DI3_Pin) == GPIO_PIN_RESET)
		val[2] |= 1;
	if (HAL_GPIO_ReadPin(MCU_DI4_GPIO_Port, MCU_DI4_Pin) == GPIO_PIN_RESET)
		val[3] |= 1;
	if (HAL_GPIO_ReadPin(MCU_DI5_GPIO_Port, MCU_DI5_Pin) == GPIO_PIN_RESET)
		val[4] |= 1;

	if ((val[0] & DIO_MASK) == DIO_MASK)
	{
		Di_infoDisIn.bit.stCh1UB = 1;
	}
	else if ((val[0] & DIO_MASK) == 0)
	{
		Di_infoDisIn.bit.stCh1UB = 0;
	}

	if ((val[1] & DIO_MASK) == DIO_MASK)
	{
		Di_infoDisIn.bit.stCh2UB = 1;
	}
	else if ((val[1] & DIO_MASK) == 0)
	{
		Di_infoDisIn.bit.stCh2UB = 0;
	}

	if ((val[2] & DIO_MASK) == DIO_MASK)
	{
		Di_infoDisIn.bit.stCh3UB = 1;
	}
	else if ((val[2] & DIO_MASK) == 0)
	{
		Di_infoDisIn.bit.stCh3UB = 0;
	}

	if ((val[3] & DIO_MASK) == DIO_MASK)
	{
		Di_infoDisIn.bit.stCh4UB = 1;
	}
	else if ((val[3] & DIO_MASK) == 0)
	{
		Di_infoDisIn.bit.stCh4UB = 0;
	}

	if ((val[4] & DIO_MASK) == DIO_MASK)
	{
		Di_infoDisIn.bit.stCh5UB = 1;
	}
	else if ((val[4] & DIO_MASK) == 0)
	{
		Di_infoDisIn.bit.stCh5UB = 0;
	}
}

void Di_Poll(void)
{
	static TickType_t tick = 0;
	if (Millis_Elapsed(tick) > 20)
	{
		Di_ScanDi();
		tick = Millis();
	}
}

void Di_DataAcq(uint8_t *data)
{
	if (data == NULL)
		return;
	if (Di_infoDisIn.bit.stCh1UB)
		data[2] = 1;
	else
		data[2] = 0;
	if (Di_infoDisIn.bit.stCh2UB)
		data[3] = 1;
	else
		data[3] = 0;
	if (Di_infoDisIn.bit.stCh3UB)
		data[4] = 1;
	else
		data[4] = 0;
	if (Di_infoDisIn.bit.stCh4UB)
		data[5] = 1;
	else
		data[5] = 0;
	if (Di_infoDisIn.bit.stCh5UB)
		data[6] = 1;
	else
		data[6] = 0;
}
