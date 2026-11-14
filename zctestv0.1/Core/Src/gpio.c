/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, FLASH_SPI_CS_Pin|FLASH_SPI_CLK_Pin|FLASH_SPI_MOSI_Pin|PHY1_RST_Pin
                          |RS485_EN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PHY1_SPI_CS_Pin|RS485_EN1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, LED_RUN_Pin|IO_OUT1_Pin|IO_OUT2_Pin|IO_OUT3_Pin
                          |IO_OUT4_Pin|IO_OUT5_Pin|IO_OUT6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PHY2_RST_GPIO_Port, PHY2_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PHY2_SPI_CS_Pin|MCU_ADC_CNV_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, RE_422_1_Pin|DE_422_1_Pin|RE_422_2_Pin|DE_422_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : FLASH_SPI_CS_Pin FLASH_SPI_CLK_Pin FLASH_SPI_MOSI_Pin PHY1_RST_Pin
                           RS485_EN2_Pin */
  GPIO_InitStruct.Pin = FLASH_SPI_CS_Pin|FLASH_SPI_CLK_Pin|FLASH_SPI_MOSI_Pin|PHY1_RST_Pin
                          |RS485_EN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : FLASH_SPI_MISO_Pin */
  GPIO_InitStruct.Pin = FLASH_SPI_MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(FLASH_SPI_MISO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PHY1_SPI_CS_Pin RS485_EN1_Pin */
  GPIO_InitStruct.Pin = PHY1_SPI_CS_Pin|RS485_EN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PHY1_INT_Pin */
  GPIO_InitStruct.Pin = PHY1_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PHY1_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_RUN_Pin IO_OUT1_Pin IO_OUT2_Pin IO_OUT3_Pin
                           IO_OUT4_Pin IO_OUT5_Pin IO_OUT6_Pin */
  GPIO_InitStruct.Pin = LED_RUN_Pin|IO_OUT1_Pin|IO_OUT2_Pin|IO_OUT3_Pin
                          |IO_OUT4_Pin|IO_OUT5_Pin|IO_OUT6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PHY2_INT_Pin */
  GPIO_InitStruct.Pin = PHY2_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PHY2_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PHY2_RST_Pin */
  GPIO_InitStruct.Pin = PHY2_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PHY2_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PHY2_SPI_CS_Pin MCU_ADC_CNV_Pin */
  GPIO_InitStruct.Pin = PHY2_SPI_CS_Pin|MCU_ADC_CNV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : MCU_DI1_Pin MCU_DI2_Pin MCU_DI3_Pin MCU_DI4_Pin
                           MCU_DI5_Pin */
  GPIO_InitStruct.Pin = MCU_DI1_Pin|MCU_DI2_Pin|MCU_DI3_Pin|MCU_DI4_Pin
                          |MCU_DI5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : RE_422_1_Pin DE_422_1_Pin RE_422_2_Pin DE_422_2_Pin */
  GPIO_InitStruct.Pin = RE_422_1_Pin|DE_422_1_Pin|RE_422_2_Pin|DE_422_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
