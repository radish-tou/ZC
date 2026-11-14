#include "Spi_BitBang.h"

#define W25QXX_SPI_Delay() \
    do                     \
    {                      \
        NOP_3();           \
    } while (0)

static void W25QXX_SPI_DelayLowSpeed(void)
{
    uint32_t i;

    for (i = 0; i < 10; i++)
    {
        W25QXX_SPI_Delay();
    }
}

void W25QXX_SPI_WriteByte(uint8_t dat)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
        {
            W25QXX_MOSI_SET();
        }
        else
        {
            W25QXX_MOSI_CLR();
        }
        dat <<= 1;
        W25QXX_SCK_CLR();
        W25QXX_SPI_Delay();

        W25QXX_SCK_SET();
        W25QXX_SPI_Delay();
    }
}

void W25QXX_SPI_MultiWriteByte(uint8_t *pBuffer, uint16_t len)
{
    while (len--)
    {
        W25QXX_SPI_WriteByte(*pBuffer++);
    }
}

void W25QXX_SPI_LowSpeed_WriteByte(uint8_t dat)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
        {
            W25QXX_MOSI_SET();
        }
        else
        {
            W25QXX_MOSI_CLR();
        }
        dat <<= 1;
        W25QXX_SCK_CLR();
        W25QXX_SPI_DelayLowSpeed();

        W25QXX_SCK_SET();
        W25QXX_SPI_DelayLowSpeed();
    }
}

uint8_t W25QXX_SPI_ReadByte(void)
{
    uint8_t dat = 0;
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        W25QXX_SCK_CLR();
        W25QXX_SPI_Delay();

        W25QXX_SCK_SET();
        W25QXX_SPI_Delay();

        dat <<= 1;
        if (W25QXX_MISO_GET())
        {
            dat++;
        }
    }

    return dat;
}

void W25QXX_SPI_MultiReadByte(uint8_t *pBuffer, uint16_t len)
{
    while (len--)
    {
        *pBuffer++ = W25QXX_SPI_ReadByte();
    }
}