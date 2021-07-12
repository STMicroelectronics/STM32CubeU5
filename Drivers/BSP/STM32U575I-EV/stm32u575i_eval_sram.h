/**
  ******************************************************************************
  * @file    stm32u575i_eval_sram.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32u575i_eval_sram.c driver.
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

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U575I_EVAL
  * @{
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32U575I_EVAL_SRAM_H
#define STM32U575I_EVAL_SRAM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u575i_eval_conf.h"
#include "stm32u575i_eval_errno.h"

/** @addtogroup STM32U575I_EVAL_SRAM
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup STM32U575I_EVAL_SRAM_Exported_Types SRAM Exported Types
  * @{
  */
#if (USE_HAL_SRAM_REGISTER_CALLBACKS > 0)
typedef struct
{
  pSRAM_CallbackTypeDef  pMspInitCb;
  pSRAM_CallbackTypeDef  pMspDeInitCb;
} BSP_SRAM_Cb_t;
#endif /* (USE_HAL_SRAM_REGISTER_CALLBACKS > 0) */
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup STM32U575I_EVAL_SRAM_Exported_Constants SRAM Exported Constants
  * @{
  */

#define SRAM_INSTANCES_NBR    1U

#define SRAM_DEVICE_ADDR      0x60000000U
#define SRAM_DEVICE_SIZE      0x200000U  /* SRAM device size : 1024 words of 16-bits */

/* DMA definitions for SRAM DMA transfer */
#define SRAM_DMAx_CLK_ENABLE  __HAL_RCC_GPDMA1_CLK_ENABLE
#define SRAM_DMAx_CLK_DISABLE __HAL_RCC_GPDMA1_CLK_DISABLE
#define SRAM_DMAx_CHANNEL     GPDMA1_Channel0
#define SRAM_DMAx_IRQn        GPDMA1_Channel0_IRQn
#define SRAM_DMAx_IRQHandler  GPDMA1_Channel0_IRQHandler

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/

/** @addtogroup STM32U575I_EVAL_SRAM_Exported_Variables
  * @{
  */
extern SRAM_HandleTypeDef hsram[SRAM_INSTANCES_NBR];
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @addtogroup STM32U575I_EVAL_SRAM_Exported_Functions
  * @{
  */
int32_t BSP_SRAM_Init(uint32_t Instance);
int32_t BSP_SRAM_DeInit(uint32_t Instance);
#if (USE_HAL_SRAM_REGISTER_CALLBACKS > 0)
int32_t BSP_SRAM_RegisterDefaultMspCallbacks(uint32_t Instance);
int32_t BSP_SRAM_RegisterMspCallbacks(uint32_t Instance, BSP_SRAM_Cb_t *CallBacks);
#endif /* (USE_HAL_SRAM_REGISTER_CALLBACKS > 0) */
void    BSP_SRAM_IRQHandler(uint32_t Instance);
void    BSP_SRAM_ErrorCallback(uint32_t Instance);

/* These functions can be modified in case the current settings
   need to be changed for specific application needs */
HAL_StatusTypeDef MX_SRAM_BANK1_Init(SRAM_HandleTypeDef *hSram);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32U575I_EVAL_SRAM_H */

/**
  * @}
  */

/**
  * @}
  */
