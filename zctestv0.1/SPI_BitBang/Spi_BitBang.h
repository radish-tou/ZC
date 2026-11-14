#ifndef SPI_BIT_BANG_H
#define SPI_BIT_BANG_H

#include "MySys.h"

/* SPI类型 */
#define W25QXX_SPI_SW 0
#define W25QXX_SPI_HW 1
#define W25QXX_SPI_TYPE W25QXX_SPI_SW

#if (W25QXX_SPI_TYPE == W25QXX_SPI_SW)
/* 引脚定义 — 全部在 GPIOC: CS=PC0, SCK=PC1, MISO=PC2, MOSI=PC3 */
/* STM32F4: BSRR 低16位=Set(HIGH), BSRR 高16位=Reset(LOW), IDR=读取输入 */
#define W25QXX_CS_SET() (FLASH_SPI_CS_GPIO_Port->BSRR = (uint32_t)FLASH_SPI_CS_Pin)
#define W25QXX_CS_CLR() (FLASH_SPI_CS_GPIO_Port->BSRR = ((uint32_t)FLASH_SPI_CS_Pin << 16))

#define W25QXX_SCK_SET() (FLASH_SPI_CLK_GPIO_Port->BSRR = (uint32_t)FLASH_SPI_CLK_Pin)
#define W25QXX_SCK_CLR() (FLASH_SPI_CLK_GPIO_Port->BSRR = ((uint32_t)FLASH_SPI_CLK_Pin << 16))

#define W25QXX_MOSI_SET() (FLASH_SPI_MOSI_GPIO_Port->BSRR = (uint32_t)FLASH_SPI_MOSI_Pin)
#define W25QXX_MOSI_CLR() (FLASH_SPI_MOSI_GPIO_Port->BSRR = ((uint32_t)FLASH_SPI_MOSI_Pin << 16))

#define W25QXX_MISO_GET() ((FLASH_SPI_MISO_GPIO_Port->IDR & FLASH_SPI_MISO_Pin) ? 1 : 0)
#else
#define W25QXX_CS_SET()
#define W25QXX_CS_CLR()
#endif /* (W25QXX_SPI_TYPE == W25QXX_SPI_SW) */

void W25QXX_SPI_WriteByte(uint8_t dat);
void W25QXX_SPI_MultiWriteByte(uint8_t *pBuffer, uint16_t len);
//void W25QXX_SPI_LowSpeed_WriteByte(uint8_t dat);

uint8_t W25QXX_SPI_ReadByte(void);
void W25QXX_SPI_MultiReadByte(uint8_t *pBuffer, uint16_t len);


#endif /* SPI_BIT_BANG_H */
