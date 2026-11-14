/********************************** (C) COPYRIGHT *******************************
 * File Name          : CH394Q_SPI.C
 * Author             : WCH
 * Version            : V1.2
 * Date               : 2025/07/01
 * Description        : Hardware abstraction layer for CH39Q SPI V1.0.0
 *******************************************************************************/
#include <CH394Q_SPI.h>

/* Global pointer to the currently active CH394Q device */
CH394Q_Dev_t *g_cur_dev = 0;

/*******************************************************************************
 * Function Name  : SPI_WriteRead
 * Description    : SPI read and write one byte (uses g_cur_dev->spi)
 * Input          : uint8_t Data
 * Return         : None
 *******************************************************************************/
uint8_t SPI_WriteRead(uint8_t data)
{
    uint8_t RxData = 0x00;
    if (HAL_SPI_TransmitReceive(g_cur_dev->spi, &data, &RxData, 1, 10) != HAL_OK)
    {
        RxData = 0xFF;
    }
    return RxData;
}

/******************************************************************************
 * Function Name  : SPIWriteCH394QData
 * Description    : SPI write data
 * Input          : uint8_t mdata
 * Output         : None
 * Return         : None
 *******************************************************************************/
void SPIWriteCH394QData(uint8_t mdata)
{
    SPI_WriteRead(mdata);
}

/*******************************************************************************
 * Function Name  : SPIReadCH394QData
 * Description    : SPI read data
 * Input          : None
 * Output         : None
 * Return         : uint8_t i
 *******************************************************************************/
uint8_t SPIReadCH394QData(void)
{
    uint8_t i;
    i = SPI_WriteRead(0x00);
    return i;
}

/******************************************************************************
 * Function Name  : CH394Q_Write
 * Description    : Write a byte
 * Input          : addr: Write data address
                    data: Write data
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH394Q_Write(uint32_t addr, uint8_t data)
{
    SPICH394QStart();
    SPIWriteCH394QData((addr & 0x00ff0000) >> 16);
    SPIWriteCH394QData((addr & 0x0000ff00) >> 8);
    SPIWriteCH394QData((addr & 0x000000f8) + 4);
    SPIWriteCH394QData(data);
    SPIEndCH394Q();
#if IPv6_MODE
    Execute_IPv6();
#endif
}

/******************************************************************************
 * Function Name  : CH394Q_Read
 * Description    : Read a byte data
 * Input          : addr: Read data address
 * Output         : None
 * Return         : data
 *******************************************************************************/
uint8_t CH394Q_Read(uint32_t addr)
{
    uint8_t data = 0;
    SPICH394QStart();
    SPIWriteCH394QData((addr & 0x00ff0000) >> 16);
    SPIWriteCH394QData((addr & 0x0000ff00) >> 8);
    SPIWriteCH394QData((addr & 0x000000f8));
    data = SPIReadCH394QData();
    SPIEndCH394Q();
    return data;
}

/******************************************************************************
 * Function Name  : CH394Q_WriteBuf
 * Description    : Write len byte data
 * Input          : addr: Write data address
                    buf: Write data
                    len: Length
 * Output         : None
 * Return         : len
 *******************************************************************************/
uint16_t CH394Q_WriteBuf(uint32_t addr, uint8_t *buf, uint16_t len)
{
#if SPI_MODE
    uint16_t i = 0;
    SPICH394QStart();
    SPIWriteCH394QData((addr & 0x00ff0000) >> 16);
    SPIWriteCH394QData((addr & 0x0000ff00) >> 8);
    SPIWriteCH394QData((addr & 0x000000f8) + 4);
    for (i = 0; i < len; i++)
    {
        SPIWriteCH394QData(buf[i]);
    }
    SPIEndCH394Q();
#elif SPI_DMA_MODE
    {
        SPI_TypeDef *SPIx = g_cur_dev->spi_reg;
        SPICH394QStart();
        while ((SPIx->SR & SPI_I2S_FLAG_TXE) == 0)
            ;
        SPIx->DR = (addr & 0x00ff0000) >> 16;
        while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == 0)
            ;
        (void)(SPIx->DR);
        while ((SPIx->SR & SPI_I2S_FLAG_TXE) == 0)
            ;
        SPIx->DR = (addr & 0x0000ff00) >> 8;
        while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == 0)
            ;
        (void)(SPIx->DR);
        while ((SPIx->SR & SPI_I2S_FLAG_TXE) == 0)
            ;
        SPIx->DR = (addr & 0x000000f8) + 4;
        while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == 0)
            ;
        (void)(SPIx->DR);
        SPIx->CR2 |= SPI_I2S_DMAReq_Tx;
        DMA1_Channel3->CNDTR = len;
        DMA1_Channel3->CMAR = (uint32_t)buf;
        DMA1_Channel3->CCR |= DMA_CCR1_EN;
        while ((!DMA_GetFlagStatus(DMA1_FLAG_TC3)))
            ;
        while ((SPIx->SR) & SPI_I2S_FLAG_BSY)
            ;
        SPIEndCH394Q();
        DMA_ClearFlag(DMA1_FLAG_TC3);
        SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
        DMA_Cmd(DMA1_Channel3, DISABLE);
        (void)SPIx->DR;
    }
#endif
    return len;
}

/******************************************************************************
 * Function Name  : CH394Q_ReadBuf
 * Description    : Read len byte data
 * Input          : addr: Read data address
                    buf: Store read data
                    len: Length
 * Output         : None
 * Return         : len
 *******************************************************************************/
uint16_t CH394Q_ReadBuf(uint32_t addr, uint8_t *buf, uint16_t len)
{
#if SPI_MODE
    uint16_t i = 0;
    SPICH394QStart();
    SPIWriteCH394QData((addr & 0x00ff0000) >> 16);
    SPIWriteCH394QData((addr & 0x0000ff00) >> 8);
    SPIWriteCH394QData((addr & 0x000000f8));
    for (i = 0; i < len; i++)
    {
        buf[i] = SPIReadCH394QData();
    }
    SPIEndCH394Q();
#elif SPI_DMA_MODE
    {
        SPI_TypeDef *SPIx = g_cur_dev->spi_reg;
        SPICH394QStart();
        while ((SPIx->SR & SPI_I2S_FLAG_TXE) == 0)
            ;
        SPIx->DR = (addr & 0x00ff0000) >> 16;
        while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == 0)
            ;
        (void)(SPIx->DR);
        while ((SPIx->SR & SPI_I2S_FLAG_TXE) == 0)
            ;
        SPIx->DR = (addr & 0x0000ff00) >> 8;
        while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == 0)
            ;
        (void)(SPIx->DR);
        while ((SPIx->SR & SPI_I2S_FLAG_TXE) == 0)
            ;
        SPIx->DR = (addr & 0x000000f8);
        while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == 0)
            ;
        (void)(SPIx->DR);
        SPIx->CR2 |= SPI_I2S_DMAReq_Tx;
        SPIx->CR2 |= SPI_I2S_DMAReq_Rx;
        DMA1_Channel3->CNDTR = len;
        DMA1_Channel3->CMAR = (uint32_t)buf;
        DMA1_Channel2->CNDTR = len;
        DMA1_Channel2->CMAR = (uint32_t)buf;
        DMA1_Channel3->CCR |= DMA_CCR1_EN;
        DMA1_Channel2->CCR |= DMA_CCR1_EN;
        while ((!DMA_GetFlagStatus(DMA1_FLAG_TC2)) && (!DMA_GetFlagStatus(DMA1_FLAG_TC3)))
            ;
        while ((SPIx->SR) & SPI_I2S_FLAG_BSY)
            ;
        SPIEndCH394Q();
        DMA_ClearFlag(DMA1_FLAG_TC3);
        DMA_ClearFlag(DMA1_FLAG_TC2);
        SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
        SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
        DMA_Cmd(DMA1_Channel3, DISABLE);
        DMA_Cmd(DMA1_Channel2, DISABLE);
    }
#endif
    return len;
}
