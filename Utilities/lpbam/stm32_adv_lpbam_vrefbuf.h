/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_vrefbuf.h
  * @author  MCD Application Team
  * @brief   header for the stm32_adv_lpbam_vrefbuf.c file
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
#ifndef STM32_ADV_LPBAM_VREFBUF_H
#define STM32_ADV_LPBAM_VREFBUF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_platform_lpbam_vrefbuf.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup VREFBUF_Advanced VREFBUF_Advanced
  * @brief    VREFBUF Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_VREFBUF_Advanced_Exported_Types LPBAM VREFBUF Advanced Exported Types
  * @brief    LPBAM VREFBUF Advanced Exported Types
  * @{
  */

/**
  * @brief  LPBAM specific advanced VREFBUF information structure definition.
  */
typedef struct
{
  uint16_t BufferMode; /*!< Specifies the VREFBUF mode.
                            This parameter can be a value of @ref LPBAM_VREFBUF_BufferMode */

} LPBAM_VREFBUF_BufferModeFullAdvConf_t;

/**
  * @brief  LPBAM buffer mode configuration descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DMA queue execution : only one node is
                                   needed                                                                             */

  uint32_t pReg[1U];          /*!< Specifies the content of register to be updated : only one node value is needed    */

} LPBAM_VREFBUF_BufferModeFullDesc_t;
/**
  * @}
  */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_VREFBUF_Advanced_Exported_Functions LPBAM VREFBUF Advanced Exported Functions
  * @{
  */

/**
  * @brief  ADV_LPBAM_VREFBUF_BufferMode_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_VREFBUF_BufferMode_SetFullQ(VREFBUF_TypeDef                       *const pInstance,
                                                     LPBAM_DMAListInfo_t                   const *const pDMAListInfo,
                                                     LPBAM_VREFBUF_BufferModeFullAdvConf_t const *const pBufferModeFull,
                                                     LPBAM_VREFBUF_BufferModeFullDesc_t    *const pDescriptor,
                                                     DMA_QListTypeDef                      *const pQueue);
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

#endif /* STM32_ADV_LPBAM_VREFBUF_H */
