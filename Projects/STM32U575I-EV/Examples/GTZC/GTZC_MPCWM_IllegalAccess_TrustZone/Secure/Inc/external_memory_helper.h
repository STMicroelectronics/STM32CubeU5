/* USER CODE BEGIN Header */
/**
  ******************************************************************************
* @file    : Secure/Inc/external_memory_helper.h
* @author  : MCD Application Team
* @brief   : This file contains the headers of the external memory helper.
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
#ifndef HAL_EXT_MEM_HELPER_H
#define HAL_EXT_MEM_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FMC_SRAM_BANKx             FMC_NORSRAM_BANK1
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

uint32_t extMemory_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* HAL_EXT_MEM_HELPER_H */

/**
* @}
*/

/**
* @}
*/

