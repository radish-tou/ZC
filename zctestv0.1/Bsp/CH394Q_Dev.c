#include "CH394Q_SPI.h"
#include "CH394Q_NET.h"
#include "spi.h"
#include "gpio.h"

CH394Q_Dev_t g_ch394_dev[2];
uint8_t TX_BUFF[SOCKET_NUM] = {2, 2, 2, 2, 2, 2, 2, 2};
uint8_t RX_BUFF[SOCKET_NUM] = {2, 2, 2, 2, 2, 2, 2, 2};

void CH394Q_Init(void)
{
  /* --- Channel 0: PHY1 on SPI1 --- */
  g_ch394_dev[0].spi = &hspi1;
  g_ch394_dev[0].spi_reg = SPI1;
  g_ch394_dev[0].cs_port = PHY1_SPI_CS_GPIO_Port;
  g_ch394_dev[0].cs_pin = PHY1_SPI_CS_Pin;
  g_ch394_dev[0].rst_port = PHY1_RST_GPIO_Port;
  g_ch394_dev[0].rst_pin = PHY1_RST_Pin;
  g_ch394_dev[0].int_port = PHY1_INT_GPIO_Port;
  g_ch394_dev[0].int_pin = PHY1_INT_Pin;
  /* Network config: 192.168.0.200 */
  g_ch394_dev[0].ip[0] = 192;
  g_ch394_dev[0].ip[1] = 168;
  g_ch394_dev[0].ip[2] = 0;
  g_ch394_dev[0].ip[3] = 200;
  g_ch394_dev[0].gw[0] = 192;
  g_ch394_dev[0].gw[1] = 168;
  g_ch394_dev[0].gw[2] = 0;
  g_ch394_dev[0].gw[3] = 1;
  g_ch394_dev[0].mask[0] = 255;
  g_ch394_dev[0].mask[1] = 255;
  g_ch394_dev[0].mask[2] = 255;
  g_ch394_dev[0].mask[3] = 0;
  g_ch394_dev[0].port = 1000;
  //  g_ch394_dev[0].mac[0] = 0x02;
  //  g_ch394_dev[0].mac[1] = 0x00;
  //  g_ch394_dev[0].mac[2] = 0x00;
  //  g_ch394_dev[0].mac[3] = 0x00;
  //  g_ch394_dev[0].mac[4] = 0x00;
  //  g_ch394_dev[0].mac[5] = 0x01;

  /* --- Channel 1: PHY2 on SPI2 --- */
  g_ch394_dev[1].spi = &hspi2;
  g_ch394_dev[1].spi_reg = SPI2;
  g_ch394_dev[1].cs_port = PHY2_SPI_CS_GPIO_Port;
  g_ch394_dev[1].cs_pin = PHY2_SPI_CS_Pin;
  g_ch394_dev[1].rst_port = PHY2_RST_GPIO_Port;
  g_ch394_dev[1].rst_pin = PHY2_RST_Pin;
  g_ch394_dev[1].int_port = PHY2_INT_GPIO_Port;
  g_ch394_dev[1].int_pin = PHY2_INT_Pin;
  /* Network config: 192.168.0.199 */
  g_ch394_dev[1].ip[0] = 192;
  g_ch394_dev[1].ip[1] = 168;
  g_ch394_dev[1].ip[2] = 0;
  g_ch394_dev[1].ip[3] = 199;
  g_ch394_dev[1].gw[0] = 192;
  g_ch394_dev[1].gw[1] = 168;
  g_ch394_dev[1].gw[2] = 0;
  g_ch394_dev[1].gw[3] = 1;
  g_ch394_dev[1].mask[0] = 255;
  g_ch394_dev[1].mask[1] = 255;
  g_ch394_dev[1].mask[2] = 255;
  g_ch394_dev[1].mask[3] = 0;
  g_ch394_dev[1].port = 1020;
  //  g_ch394_dev[1].mac[0] = 0x02;
  //  g_ch394_dev[1].mac[1] = 0x00;
  //  g_ch394_dev[1].mac[2] = 0x00;
  //  g_ch394_dev[1].mac[3] = 0x00;
  //  g_ch394_dev[1].mac[4] = 0x00;
  //  g_ch394_dev[1].mac[5] = 0x02;

  /* --- Initialize both channels --- */
  for (int ch = 0; ch < 2; ch++)
  {
    g_cur_dev = &g_ch394_dev[ch]; /* select active device */

    CH394Q_ResetHW();                                            /* hardware reset */
    CH394Q_GetCHIPV();                                           /* read chip version */
    CH394Q_PHY_Check();                                          /* wait for PHY link up */
    CH394Q_InfParam();                                           /* set IP/MAC/GW/Mask */
    CH394Q_SocketBuf_Init(TX_BUFF, RX_BUFF);                     /* configure socket buffer sizes */
    CH394Q_SetGINTE(0xFF);                                       /* enable global interrupt */
    CH394Q_SetSINTE(0xFF);                                       /* enable socket interrupt */
    CH394Q_TCPServerSocketInit(0, Sn_MODE_TCP, g_cur_dev->port); /* TCP server on socket 0 */
  }
  g_cur_dev = &g_ch394_dev[0]; /* default to channel 0 */
}

void CH394Q_Poll(void)
{
  for (int ch = 0; ch < 2; ch++)
  {
    if (g_ch394_dev[ch].stCH394Q_IntUB)
    {
      g_ch394_dev[ch].stCH394Q_IntUB = 0;
      g_cur_dev = &g_ch394_dev[ch]; /* switch to this channel */
      CH394Q_GlobalInterrupt();     /* handle socket/global interrupts */
    }
  }
}

/**
 * @brief GPIO EXTI callback ¡ª sets the interrupt flag for the matching CH394Q channel.
 *        PHY1_INT_Pin (PE9) ¡ú channel 0
 *        PHY2_INT_Pin (PC4) ¡ú channel 1
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == PHY1_INT_Pin)
  {
    g_ch394_dev[0].stCH394Q_IntUB = 1;
  }
  else if (GPIO_Pin == PHY2_INT_Pin)
  {
    g_ch394_dev[1].stCH394Q_IntUB = 1;
  }
}