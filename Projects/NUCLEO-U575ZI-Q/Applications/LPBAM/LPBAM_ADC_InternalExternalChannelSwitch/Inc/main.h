/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_ADC_InetrnalExternalChannelSwitch/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"
#include "stm32u5xx_nucleo.h"
#include "stm32_lpbam.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* #define DEBUG_CONFIGURATION */

/* Definition of ADC conversions data table size */
#define ADC_CONVERTED_DATA_BUFFER_SIZE   (32U)

/* Definition for ADC clock resources */
#define ADC_Instance                  ADC4
#define ADC_CLK_ENABLE()              __HAL_RCC_ADC4_CLK_ENABLE()
#define ADC_CHANNEL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()

#define ADC_FORCE_RESET()             __HAL_RCC_ADC4_FORCE_RESET()
#define ADC_RELEASE_RESET()           __HAL_RCC_ADC4_RELEASE_RESET()

/* Definition for ADC Channel Pin */
#define ADC_CHANNEL_PIN_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define ADC_CHANNEL_PIN              GPIO_PIN_3
#define ADC_CHANNEL_GPIO_PORT        GPIOC

/* Definition for ADC's Channel */
#define ADC_CHANNEL ADC_CHANNEL_VREFINT

/* Definition for DMA */
#define DMA_CLK_ENABLE() __HAL_RCC_LPDMA1_CLK_ENABLE()

/* Definition the Autonomous Mode utilities */
#define LPDMA_CLKAM_ENABLE() __HAL_RCC_LPDMA1_CLKAM_ENABLE()
#define ADC_CLKAM_ENABLE()   __HAL_RCC_ADC4_CLKAM_ENABLE()
#define LPTIM_CLKAM_ENABLE() __HAL_RCC_LPTIM1_CLKAM_ENABLE()
#define SRAM_CLKAM_ENABLE()  __HAL_RCC_SRAM4_CLKAM_ENABLE()

#define ANALOGWATCHDOG_1 (0x00U)
#define ANALOGWATCHDOG_2 (0x01U)
#define ANALOGWATCHDOG_3 (0x02U)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

#endif /* MAIN_H */
