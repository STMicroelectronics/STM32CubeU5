/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_dma.h
  * @author  MCD Application Team
  * @brief   header for the stm32_adv_lpbam_dma.c file
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef STM32_ADV_LPBAM_DMA_H
#define STM32_ADV_LPBAM_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_platform_lpbam_dma.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup DMA_Advanced DMA_Advanced
  * @brief    DMA Advanced LPBAM module driver
  * @{
  */

/* Exported types ----------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_DMA_Advanced_Exported_Types LPBAM DMA Advanced Exported Types
  * @brief    LPBAM DMA Advanced Exported Types
  * @{
  */

/**
  * @brief  LPBAM DMA start advanced configuration structure definition.
  */
typedef struct
{
  uint32_t HeadQAddress; /*!< Specifies the head node address of queue to be executed : (uint32_t)Queue.Head */

  uint32_t WakeupIT;     /*!< Specifies the wake up source interrupt
                              This parameter can be one or a combination of @ref LPBAM_DMA_Wakeup_Interrupt  */
} LPBAM_DMA_StartFullAdvConf_t;

/**
  * @brief  LPBAM DMA start descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[6U]; /*!< Specifies the content of nodes required for DMA queue execution : 6 different nodes
                                   are needed                                                                         */

  uint32_t pReg[6U];          /*!< Specifies the content of register to be updated : 6 different values are needed    */

} LPBAM_DMA_StartFullDesc_t;

/**
  * @brief  LPBAM DMA stop descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DMA queue execution : only one node is
                                   needed                                                                             */

  uint32_t pReg[1U];          /*!< Specifies the content of register to be updated : only one value is needed         */

} LPBAM_DMA_StopFullDesc_t;
/**
  * @}
  */
/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_DMA_Advanced_Exported_Functions LPBAM DMA Advanced Exported Functions
  * @brief      LPBAM DMA Advanced Exported Functions
  * @{
  */

/**
  * @brief  ADV_LPBAM_DMA_Start_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_DMA_Start_SetFullQ(DMA_Channel_TypeDef          *const pInstance,
                                            LPBAM_DMAListInfo_t          const *const pDMAListInfo,
                                            LPBAM_DMA_StartFullAdvConf_t const *const pStartFull,
                                            LPBAM_DMA_StartFullDesc_t    *const pDescriptor,
                                            DMA_QListTypeDef             *const pQueue);

/**
  * @brief  ADV_LPBAM_DMA_Stop_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_DMA_Stop_SetFullQ(DMA_Channel_TypeDef      *const pInstance,
                                           LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                           LPBAM_DMA_StopFullDesc_t *const pDescriptor,
                                           DMA_QListTypeDef         *const pQueue);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32_ADV_LPBAM_DMA_H */
