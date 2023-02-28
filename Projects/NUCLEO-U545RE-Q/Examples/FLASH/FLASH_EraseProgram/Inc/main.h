/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    FLASH/FLASH_EraseProgram/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32u5xx_nucleo.h"
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

/* USER CODE BEGIN Private defines */
/* Base address of the Flash pages */

#define ADDR_FLASH_PAGE_0     ((uint32_t)0x08000000) /* Base @ of Page 0, 8 Kbytes */
#define ADDR_FLASH_PAGE_1     ((uint32_t)0x08002000) /* Base @ of Page 1, 8 Kbytes */
#define ADDR_FLASH_PAGE_2     ((uint32_t)0x08004000) /* Base @ of Page 2, 8 Kbytes */
#define ADDR_FLASH_PAGE_3     ((uint32_t)0x08006000) /* Base @ of Page 3, 8 Kbytes */
#define ADDR_FLASH_PAGE_4     ((uint32_t)0x08008000) /* Base @ of Page 4, 8 Kbytes */
#define ADDR_FLASH_PAGE_5     ((uint32_t)0x0800A000) /* Base @ of Page 5, 8 Kbytes */
#define ADDR_FLASH_PAGE_6     ((uint32_t)0x0800C000) /* Base @ of Page 6, 8 Kbytes */
#define ADDR_FLASH_PAGE_7     ((uint32_t)0x0800E000) /* Base @ of Page 7, 8 Kbytes */
#define ADDR_FLASH_PAGE_8     ((uint32_t)0x08010000) /* Base @ of Page 8, 8 Kbytes */
#define ADDR_FLASH_PAGE_9     ((uint32_t)0x08012000) /* Base @ of Page 9, 8 Kbytes */
#define ADDR_FLASH_PAGE_10    ((uint32_t)0x08014000) /* Base @ of Page 10, 8 Kbytes */
#define ADDR_FLASH_PAGE_11    ((uint32_t)0x08016000) /* Base @ of Page 11, 8 Kbytes */
#define ADDR_FLASH_PAGE_12    ((uint32_t)0x08018000) /* Base @ of Page 12, 8 Kbytes */
#define ADDR_FLASH_PAGE_13    ((uint32_t)0x0801A000) /* Base @ of Page 13, 8 Kbytes */
#define ADDR_FLASH_PAGE_14    ((uint32_t)0x0801C000) /* Base @ of Page 14, 8 Kbytes */
#define ADDR_FLASH_PAGE_15    ((uint32_t)0x0801E000) /* Base @ of Page 15, 8 Kbytes */
#define ADDR_FLASH_PAGE_16    ((uint32_t)0x08020000) /* Base @ of Page 16, 8 Kbytes */
#define ADDR_FLASH_PAGE_17    ((uint32_t)0x08022000) /* Base @ of Page 17, 8 Kbytes */
#define ADDR_FLASH_PAGE_18    ((uint32_t)0x08024000) /* Base @ of Page 18, 8 Kbytes */
#define ADDR_FLASH_PAGE_19    ((uint32_t)0x08026000) /* Base @ of Page 19, 8 Kbytes */
#define ADDR_FLASH_PAGE_20    ((uint32_t)0x08028000) /* Base @ of Page 20, 8 Kbytes */
#define ADDR_FLASH_PAGE_21    ((uint32_t)0x0802A000) /* Base @ of Page 21, 8 Kbytes */
#define ADDR_FLASH_PAGE_22    ((uint32_t)0x0802C000) /* Base @ of Page 22, 8 Kbytes */
#define ADDR_FLASH_PAGE_23    ((uint32_t)0x0802E000) /* Base @ of Page 23, 8 Kbytes */
#define ADDR_FLASH_PAGE_24    ((uint32_t)0x08030000) /* Base @ of Page 24, 8 Kbytes */
#define ADDR_FLASH_PAGE_25    ((uint32_t)0x08032000) /* Base @ of Page 25, 8 Kbytes */
#define ADDR_FLASH_PAGE_26    ((uint32_t)0x08034000) /* Base @ of Page 26, 8 Kbytes */
#define ADDR_FLASH_PAGE_27    ((uint32_t)0x08036000) /* Base @ of Page 27, 8 Kbytes */
#define ADDR_FLASH_PAGE_28    ((uint32_t)0x08038000) /* Base @ of Page 28, 8 Kbytes */
#define ADDR_FLASH_PAGE_29    ((uint32_t)0x0803A000) /* Base @ of Page 29, 8 Kbytes */
#define ADDR_FLASH_PAGE_30    ((uint32_t)0x0803C000) /* Base @ of Page 30, 8 Kbytes */
#define ADDR_FLASH_PAGE_31    ((uint32_t)0x0803E000) /* Base @ of Page 31, 8 Kbytes */
      
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
