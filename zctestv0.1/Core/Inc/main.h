/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FLASH_SPI_CS_Pin GPIO_PIN_0
#define FLASH_SPI_CS_GPIO_Port GPIOC
#define FLASH_SPI_CLK_Pin GPIO_PIN_1
#define FLASH_SPI_CLK_GPIO_Port GPIOC
#define FLASH_SPI_MISO_Pin GPIO_PIN_2
#define FLASH_SPI_MISO_GPIO_Port GPIOC
#define FLASH_SPI_MOSI_Pin GPIO_PIN_3
#define FLASH_SPI_MOSI_GPIO_Port GPIOC
#define PHY1_SPI_CS_Pin GPIO_PIN_4
#define PHY1_SPI_CS_GPIO_Port GPIOA
#define PHY1_SPI_SCK_Pin GPIO_PIN_5
#define PHY1_SPI_SCK_GPIO_Port GPIOA
#define PHY1_SPI_MISO_Pin GPIO_PIN_6
#define PHY1_SPI_MISO_GPIO_Port GPIOA
#define PHY1_SPI_MOSI_Pin GPIO_PIN_7
#define PHY1_SPI_MOSI_GPIO_Port GPIOA
#define PHY1_INT_Pin GPIO_PIN_4
#define PHY1_INT_GPIO_Port GPIOC
#define PHY1_INT_EXTI_IRQn EXTI4_IRQn
#define PHY1_RST_Pin GPIO_PIN_5
#define PHY1_RST_GPIO_Port GPIOC
#define LED_RUN_Pin GPIO_PIN_0
#define LED_RUN_GPIO_Port GPIOG
#define PHY2_INT_Pin GPIO_PIN_9
#define PHY2_INT_GPIO_Port GPIOE
#define PHY2_INT_EXTI_IRQn EXTI9_5_IRQn
#define PHY2_RST_Pin GPIO_PIN_10
#define PHY2_RST_GPIO_Port GPIOE
#define PHY2_SPI_SCK_Pin GPIO_PIN_10
#define PHY2_SPI_SCK_GPIO_Port GPIOB
#define PHY2_SPI_CS_Pin GPIO_PIN_11
#define PHY2_SPI_CS_GPIO_Port GPIOB
#define PHY2_SPI_MISO_Pin GPIO_PIN_14
#define PHY2_SPI_MISO_GPIO_Port GPIOB
#define PHY2_SPI_MOSI_Pin GPIO_PIN_15
#define PHY2_SPI_MOSI_GPIO_Port GPIOB
#define TX1_232_Pin GPIO_PIN_8
#define TX1_232_GPIO_Port GPIOD
#define RX1_232_Pin GPIO_PIN_9
#define RX1_232_GPIO_Port GPIOD
#define MCU_DI1_Pin GPIO_PIN_11
#define MCU_DI1_GPIO_Port GPIOD
#define MCU_DI2_Pin GPIO_PIN_12
#define MCU_DI2_GPIO_Port GPIOD
#define MCU_DI3_Pin GPIO_PIN_13
#define MCU_DI3_GPIO_Port GPIOD
#define MCU_DI4_Pin GPIO_PIN_14
#define MCU_DI4_GPIO_Port GPIOD
#define MCU_DI5_Pin GPIO_PIN_15
#define MCU_DI5_GPIO_Port GPIOD
#define IO_OUT1_Pin GPIO_PIN_3
#define IO_OUT1_GPIO_Port GPIOG
#define IO_OUT2_Pin GPIO_PIN_4
#define IO_OUT2_GPIO_Port GPIOG
#define IO_OUT3_Pin GPIO_PIN_5
#define IO_OUT3_GPIO_Port GPIOG
#define IO_OUT4_Pin GPIO_PIN_6
#define IO_OUT4_GPIO_Port GPIOG
#define IO_OUT5_Pin GPIO_PIN_7
#define IO_OUT5_GPIO_Port GPIOG
#define IO_OUT6_Pin GPIO_PIN_8
#define IO_OUT6_GPIO_Port GPIOG
#define RS485_TX2_Pin GPIO_PIN_6
#define RS485_TX2_GPIO_Port GPIOC
#define RS485_RX2_Pin GPIO_PIN_7
#define RS485_RX2_GPIO_Port GPIOC
#define RS485_EN2_Pin GPIO_PIN_8
#define RS485_EN2_GPIO_Port GPIOC
#define RS485_EN1_Pin GPIO_PIN_8
#define RS485_EN1_GPIO_Port GPIOA
#define RS485_TX1_Pin GPIO_PIN_9
#define RS485_TX1_GPIO_Port GPIOA
#define RS485_RX1_Pin GPIO_PIN_10
#define RS485_RX1_GPIO_Port GPIOA
#define TX2_232_Pin GPIO_PIN_10
#define TX2_232_GPIO_Port GPIOC
#define RX2_232_Pin GPIO_PIN_11
#define RX2_232_GPIO_Port GPIOC
#define TXD_422_1_Pin GPIO_PIN_12
#define TXD_422_1_GPIO_Port GPIOC
#define RE_422_1_Pin GPIO_PIN_0
#define RE_422_1_GPIO_Port GPIOD
#define DE_422_1_Pin GPIO_PIN_1
#define DE_422_1_GPIO_Port GPIOD
#define RXD_422_1_Pin GPIO_PIN_2
#define RXD_422_1_GPIO_Port GPIOD
#define RE_422_2_Pin GPIO_PIN_3
#define RE_422_2_GPIO_Port GPIOD
#define DE_422_2_Pin GPIO_PIN_4
#define DE_422_2_GPIO_Port GPIOD
#define TXD_422_2_Pin GPIO_PIN_5
#define TXD_422_2_GPIO_Port GPIOD
#define RXD_422_2_Pin GPIO_PIN_6
#define RXD_422_2_GPIO_Port GPIOD
#define MCU_ADC_SCK_Pin GPIO_PIN_3
#define MCU_ADC_SCK_GPIO_Port GPIOB
#define MCU_ADC_SDO_Pin GPIO_PIN_4
#define MCU_ADC_SDO_GPIO_Port GPIOB
#define MCU_ADC_DIN_Pin GPIO_PIN_5
#define MCU_ADC_DIN_GPIO_Port GPIOB
#define MCU_ADC_CNV_Pin GPIO_PIN_6
#define MCU_ADC_CNV_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
