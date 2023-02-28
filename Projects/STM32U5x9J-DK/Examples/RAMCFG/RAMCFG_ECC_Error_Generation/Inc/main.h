/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32u5x9j_discovery.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum {MATCH = 0, NOTMATCH = !MATCH} BufferContentStatus;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define SRAM3_SIGNLE_ERROR_ADDRESS_BASE (SRAM3_BASE_NS)
#define SRAM3_SIGNLE_ERROR_ADDRESS_SIZE (0x200U / 0x04U)
#define SRAM3_DOUBLE_ERROR_ADDRESS_BASE (SRAM3_BASE_NS + 0x200U)
#define SRAM3_DOUBLE_ERROR_ADDRESS_SIZE (0x200U / 0x04U)

#define DATA_VALUE                      (0xAA55AA55UL)
#define SINGLE_ERROR_DATA_VALUE         (0xAA55AA54UL)
#define DOUBLE_ERROR_DATA_VALUE         (0xAA55AA50UL)

#define EXPECTED_SINGLE_ERROR_NUM       SRAM3_SIGNLE_ERROR_ADDRESS_SIZE
#define EXPECTED_DOUBLE_ERROR_NUM       SRAM3_DOUBLE_ERROR_ADDRESS_SIZE
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
