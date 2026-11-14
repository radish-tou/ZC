/**
 * Ai_Acq.c  —— 修正版  (FZTY2026010 / TPC51701 8 通道模拟量采集)
 *
 */

#include "spi.h"
#include "gpio.h"
#include "delay.h"
#include "MySys.h"
#include <string.h>
#include "CAN_Comm.h"
#include "sensor.h"
#define TPC51701_CFG_VAL 0x3FCF

static uint16_t adc_buf[8];
float g_Aidata[8] = {0};

static inline void TPC51701_CNV_High(void)
{
    HAL_GPIO_WritePin(MCU_ADC_CNV_GPIO_Port, MCU_ADC_CNV_Pin, GPIO_PIN_SET);
}
static inline void TPC51701_CNV_Low(void)
{
    HAL_GPIO_WritePin(MCU_ADC_CNV_GPIO_Port, MCU_ADC_CNV_Pin, GPIO_PIN_RESET);
}

static inline void TPC51701_ConvOnce(void)
{
    TPC51701_CNV_High();
    delay_us(1);
    TPC51701_CNV_Low();
}

static void TPC51701_WriteCfg(uint16_t cfg_val)
{
    uint16_t spi_tx = cfg_val << 2; /* 14bit CFG 左移 2 位对齐到帧的高 14 位 */
    TPC51701_CNV_Low();
    HAL_SPI_Transmit(&hspi3, (uint8_t *)&spi_tx, 1, 100);
}

static uint16_t TPC51701_ReadResult(void)
{
    uint16_t spi_rx = 0;
    HAL_SPI_Receive(&hspi3, (uint8_t *)&spi_rx, 1, 100);
    return spi_rx;
}

void TPC51701_Init(void)
{
    TPC51701_CNV_Low();
    HAL_Delay(2); /* 内部 4.096V 基准建立时间 typ 2ms (CREF=10uF) */

    /* 手册 P13: 上电后 CFG 未定义，必须先做 2 次哑转换，CFG 写入才生效 */
    TPC51701_WriteCfg(TPC51701_CFG_VAL);
    for (uint8_t i = 0; i < 1; i++)
    {
        TPC51701_ConvOnce();
        (void)TPC51701_ReadResult();
    }
}

static void TPC51701_ScanAllChannel(void)
{
    for (uint8_t ch = 0; ch < 8; ch++)
    {
        TPC51701_ConvOnce();
        adc_buf[ch] = TPC51701_ReadResult();
    }
}

static float TPC51701_CodeToVolt(uint16_t raw_code)
{
    return (float)raw_code * 4.096f / 65535.0f;
}

//static void TPC51701_GetArrayValue(void) //获取模拟输入
//{
//    memset(g_Aidata, 0, sizeof(g_Aidata));

//    for (int ch = 0; ch < 6; ch++)
//    {
//        g_Aidata[ch] = TPC51701_CodeToVolt(adc_buf[ch]);
//    }
//}

static void TPC51701_GetArrayValue(void) //获取模拟输入
{
    for (int ch = 0; ch < 6; ch++)
    {
				float tempValue = TPC51701_CodeToVolt(adc_buf[ch]);
        forceSensor[ch].Value = tempValue;
//				if(tempValue > 3.5) //模拟输入异常判断
//					forceSensor[ch].commCnt = 0x0A;
    }		
}


void TPC51701_Poll(void)
{
    static TickType_t tick = 0;
    if (Millis_Elapsed(tick) > 50)
    {
        TPC51701_ScanAllChannel();
        TPC51701_GetArrayValue();
        tick = Millis();
    }
}

void TPC51701_ReportAiData(CAN_HandleTypeDef *hcan, uint8_t *ai_data, uint8_t len)
{
    uint8_t *pData = NULL;
    uint8_t ai_data_buf[8] = {0};

    if (len != 8 || ai_data == NULL)
    {
        return;
    }

    ai_data_buf[0] = 0x66;
    ai_data_buf[1] = 0xBB;
    ai_data_buf[7] = 0x00;

    for (int ch = 0; ch < 8; ch++)
    {
        ai_data_buf[2] = ch + 1;
        pData = &ai_data_buf[3];
        memcpy(pData, &g_Aidata[ch], sizeof(float));
        CAN_Write(hcan, 0x01020304, ai_data_buf, 8);
        delay_ms(2); /* 注意：测试，测试,实际工程不能这么用*/
    }
}
