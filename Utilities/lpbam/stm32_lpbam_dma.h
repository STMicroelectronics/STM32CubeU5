/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_dma.h
  * @author  MCD Application Team
  * @brief   header for the stm32_lpbam_dma.c file
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

/* Define to prevent recursive inclusion ---------------------------------------------------------------------------- */
#ifndef STM32_LPBAM_DMA_H
#define STM32_LPBAM_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_platform_lpbam_dma.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup DMA_Basic
  * @brief      DMA LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_DMA_Basic_Exported_Types LPBAM DMA Basic Exported Types
  * @brief    LPBAM DMA Basic Exported Types
  * @{
  */

/**
  * @brief LPBAM DMA Configuration Structure definition.
  */
typedef struct
{
  LPBAM_FunctionalState State; /*!< Specifies the DMA channel state.
                                    This parameter can be ENABLE or DISABLE                                       */

  uint32_t HeadQAddress;       /*!< Specifies the head node address of queue to be executed                       */

  uint32_t WakeupIT;           /*!< Specifies the wake up source interrupt
                                    This parameter can be one or a combination of @ref LPBAM_DMA_Wakeup_Interrupt */

} LPBAM_DMA_Config_t;

/**
  * @brief LPBAM DMA Configuration Node Structure definition.
  */
typedef struct
{
  LPBAM_Desc_t        NodeDesc;   /*!< Defines the node descriptor fields */

  DMA_Channel_TypeDef *pInstance; /*!< Defines the DMA channel instance   */

  LPBAM_DMA_Config_t  Config;     /*!< Defines the DMA user configuration */

} LPBAM_DMA_ConfNode_t;

/**
  * @brief LPBAM DMA Configuration Node ID Enumeration definition.
  *
  */
typedef enum
{
  LPBAM_DMA_CONFIG_ID,              /*!< LPBAM DMA configuration node ID   */

  LPBAM_DMA_QUEUE_BASEADDRESS_ID,   /*!< LPBAM DMA base address node ID    */

  LPBAM_DMA_QUEUE_OFFSETADDRESS_ID, /*!< LPBAM DMA offset address node ID  */

  LPBAM_DMA_CLEARDATASIZE_ID,       /*!< LPBAM DMA clear data size node ID */

  LPBAM_DMA_CLEARFLAG_ID            /*!< LPBAM DMA clear flag node ID      */

} LPBAM_DMA_ID_t;
/**
  * @}
  */

/* Include LPTIM LPBAM advanced module */
#include "stm32_adv_lpbam_dma.h"

/** @addtogroup LPBAM_DMA_Basic_Exported_Functions LPBAM DMA Basic Exported Functions
  * @brief      LPBAM DMA Basic Exported Functions
  * @{
  */

/**
  * @brief LPBAM_DMA_FillNodeConfig.
  */
LPBAM_Status_t LPBAM_DMA_FillNodeConfig(LPBAM_DMA_ConfNode_t const *const pConfNode,
                                        DMA_NodeConfTypeDef  *const pDMANodeConfig);
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

#endif /* STM32_LPBAM_DMA_H */
