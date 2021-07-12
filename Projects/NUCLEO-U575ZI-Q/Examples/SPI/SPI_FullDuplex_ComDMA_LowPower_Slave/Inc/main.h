/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "linked_list.h"
#include "stm32u5xx_nucleo.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern DMA_QListTypeDef Queue_tx;
extern DMA_QListTypeDef Queue_rx;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define SPIx_CLKAM_ENABLE()               __HAL_RCC_SPI3_CLKAM_ENABLE()
#define SPIx_CLK_SLEEP_ENABLE()           __HAL_RCC_SPI3_CLK_SLEEP_ENABLE()

#define DMAx_CLKAM_ENABLE()               __HAL_RCC_LPDMA1_CLKAM_ENABLE()
#define DMAx_CLK_SLEEP_ENABLE()           __HAL_RCC_LPDMA1_CLK_SLEEP_ENABLE()

#define SRAMx_CLK_ENABLE()                __HAL_RCC_SRAM4_CLK_ENABLE()
#define SRAMx_CLKAM_ENABLE()              __HAL_RCC_SRAM4_CLKAM_ENABLE()
#define SRAMx_CLK_SLEEP_ENABLE()          __HAL_RCC_SRAM4_CLK_SLEEP_ENABLE()
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define BUFFERSIZE                       (COUNTOF(aTxBuffer) - 1)
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
