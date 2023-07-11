/**
  ******************************************************************************
  * @file    OSPI/OSPI_NOR_MemoryMapped/Src/stm32u5xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"
#include "main.h"

/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

/** @defgroup HAL_MSP HAL MSP module driver
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief OSPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for requests by peripheral
  *           - NVIC configuration for DMA and OSPI interrupts
  * @param hospi: OSPI handle pointer
  * @retval None
  */
void HAL_XSPI_MspInit(XSPI_HandleTypeDef *hospi)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable the OctoSPI memory interface clocks */
  OSPI_CLK_ENABLE();
  /* Reset the OctoSPI memory interface */
  OSPI_FORCE_RESET();
  OSPI_RELEASE_RESET();

  /* Enable GPIO clocks */
  OSPI_CLK_GPIO_CLK_ENABLE();
  OSPI_DQS_GPIO_CLK_ENABLE();
  OSPI_CS_GPIO_CLK_ENABLE();
  OSPI_D0_GPIO_CLK_ENABLE();
  OSPI_D1_GPIO_CLK_ENABLE();
  OSPI_D2_GPIO_CLK_ENABLE();
  OSPI_D3_GPIO_CLK_ENABLE();
  OSPI_D4_GPIO_CLK_ENABLE();
  OSPI_D5_GPIO_CLK_ENABLE();
  OSPI_D6_GPIO_CLK_ENABLE();
  OSPI_D7_GPIO_CLK_ENABLE();

  /*  Activate HSLV  */
  HAL_GPIO_EnableHighSPeedLowVoltage(OSPI_CS_GPIO_PORT, OSPI_CS_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(OSPI_CLK_GPIO_PORT, OSPI_CLK_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(OSPI_D0_GPIO_PORT, OSPI_D0_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(OSPI_D1_GPIO_PORT, OSPI_D1_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(OSPI_D2_GPIO_PORT, OSPI_D3_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(OSPI_D3_GPIO_PORT, OSPI_D3_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(OSPI_D4_GPIO_PORT, OSPI_D4_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(OSPI_D5_GPIO_PORT, OSPI_D5_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(OSPI_D6_GPIO_PORT, OSPI_D6_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(OSPI_D7_GPIO_PORT, OSPI_D7_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(OSPI_DQS_GPIO_PORT, OSPI_DQS_PIN);

  /* OctoSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = OSPI_CS_PIN_AF;
  HAL_GPIO_Init(OSPI_CS_GPIO_PORT, &GPIO_InitStruct);

  /* OctoSPI CLK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_CLK_PIN;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Alternate = OSPI_CLK_PIN_AF;
  HAL_GPIO_Init(OSPI_CLK_GPIO_PORT, &GPIO_InitStruct);

  /* OctoSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D0_PIN;
  GPIO_InitStruct.Alternate = OSPI_D0_PIN_AF;
  HAL_GPIO_Init(OSPI_D0_GPIO_PORT, &GPIO_InitStruct);

  /* OctoSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D1_PIN;
  GPIO_InitStruct.Alternate = OSPI_D1_PIN_AF;
  HAL_GPIO_Init(OSPI_D1_GPIO_PORT, &GPIO_InitStruct);

  /* OctoSPI D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D2_PIN;
  GPIO_InitStruct.Alternate = OSPI_D2_PIN_AF;
  HAL_GPIO_Init(OSPI_D2_GPIO_PORT, &GPIO_InitStruct);

  /* OctoSPI D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D3_PIN;
  GPIO_InitStruct.Alternate = OSPI_D3_PIN_AF;
  HAL_GPIO_Init(OSPI_D3_GPIO_PORT, &GPIO_InitStruct);

  /* OctoSPI D4 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D4_PIN;
  GPIO_InitStruct.Alternate = OSPI_D4_PIN_AF;
  HAL_GPIO_Init(OSPI_D4_GPIO_PORT, &GPIO_InitStruct);

  /* OctoSPI D5 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D5_PIN;
  GPIO_InitStruct.Alternate = OSPI_D5_PIN_AF;
  HAL_GPIO_Init(OSPI_D5_GPIO_PORT, &GPIO_InitStruct);

  /* OctoSPI D6 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D6_PIN;
  GPIO_InitStruct.Alternate = OSPI_D6_PIN_AF;
  HAL_GPIO_Init(OSPI_D6_GPIO_PORT, &GPIO_InitStruct);

  /* OctoSPI D7 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_D7_PIN;
  GPIO_InitStruct.Alternate = OSPI_D7_PIN_AF;
  HAL_GPIO_Init(OSPI_D7_GPIO_PORT, &GPIO_InitStruct);

  /* OctoSPI DQS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = OSPI_DQS_PIN;
  GPIO_InitStruct.Alternate = OSPI_DQS_PIN_AF;
  HAL_GPIO_Init(OSPI_DQS_GPIO_PORT, &GPIO_InitStruct);

  /*##-2- Configure the NVIC for OSPI #########################################*/
  /* NVIC configuration for OSPI interrupt */
  HAL_NVIC_SetPriority(OCTOSPI1_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(OCTOSPI1_IRQn);

}

/**
  * @brief OSPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hospi: OSPI handle pointer
  * @retval None
  */
void HAL_XSPI_MspDeInit(XSPI_HandleTypeDef *hospi)
{
  /*##-1- Disable the NVIC for OSPI and DMA ##################################*/
  HAL_NVIC_DisableIRQ(OCTOSPI1_IRQn);

  /*##-2- Disable peripherals ################################################*/

  /*  Deactivate HSLV  */
  HAL_GPIO_DisableHighSPeedLowVoltage(OSPI_CS_GPIO_PORT, OSPI_CS_PIN);
  HAL_GPIO_DisableHighSPeedLowVoltage(OSPI_CLK_GPIO_PORT, OSPI_CLK_PIN);
  HAL_GPIO_DisableHighSPeedLowVoltage(OSPI_D0_GPIO_PORT, OSPI_D0_PIN);
  HAL_GPIO_DisableHighSPeedLowVoltage(OSPI_D2_GPIO_PORT, OSPI_D1_PIN);
  HAL_GPIO_DisableHighSPeedLowVoltage(OSPI_D5_GPIO_PORT, OSPI_D3_PIN);
  HAL_GPIO_DisableHighSPeedLowVoltage(OSPI_D5_GPIO_PORT, OSPI_D3_PIN);
  HAL_GPIO_DisableHighSPeedLowVoltage(OSPI_D5_GPIO_PORT, OSPI_D4_PIN);
  HAL_GPIO_DisableHighSPeedLowVoltage(OSPI_D5_GPIO_PORT, OSPI_D5_PIN);
  HAL_GPIO_DisableHighSPeedLowVoltage(OSPI_D5_GPIO_PORT, OSPI_D6_PIN);
  HAL_GPIO_DisableHighSPeedLowVoltage(OSPI_D5_GPIO_PORT, OSPI_D7_PIN);
  HAL_GPIO_DisableHighSPeedLowVoltage(OSPI_DQS_GPIO_PORT, OSPI_DQS_PIN);

  /* De-Configure OSPI pins */
  HAL_GPIO_DeInit(OSPI_CS_GPIO_PORT, OSPI_CS_PIN);
  HAL_GPIO_DeInit(OSPI_CLK_GPIO_PORT, OSPI_CLK_PIN);
  HAL_GPIO_DeInit(OSPI_D0_GPIO_PORT, OSPI_D0_PIN);
  HAL_GPIO_DeInit(OSPI_D1_GPIO_PORT, OSPI_D1_PIN);
  HAL_GPIO_DeInit(OSPI_D2_GPIO_PORT, OSPI_D2_PIN);
  HAL_GPIO_DeInit(OSPI_D3_GPIO_PORT, OSPI_D3_PIN);
  HAL_GPIO_DeInit(OSPI_D4_GPIO_PORT, OSPI_D4_PIN);
  HAL_GPIO_DeInit(OSPI_D5_GPIO_PORT, OSPI_D5_PIN);
  HAL_GPIO_DeInit(OSPI_D6_GPIO_PORT, OSPI_D6_PIN);
  HAL_GPIO_DeInit(OSPI_D7_GPIO_PORT, OSPI_D7_PIN);
  HAL_GPIO_DeInit(OSPI_DQS_GPIO_PORT, OSPI_DQS_PIN);

  /*##-3- Reset peripherals ##################################################*/
  /* Reset the OctoSPI memory interface */
  OSPI_FORCE_RESET();
  OSPI_RELEASE_RESET();

  /* Disable the OctoSPI memory interface clocks */
  OSPI_CLK_DISABLE();
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
