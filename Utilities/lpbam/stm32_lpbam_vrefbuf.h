/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_vrefbuf.h
  * @author  MCD Application Team
  * @brief   header for the stm32_lpbam_vrefbuf.c file
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
#ifndef STM32_LPBAM_VREFBUF_H
#define STM32_LPBAM_VREFBUF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_platform_lpbam_vrefbuf.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup VREFBUF_Basic
  * @brief      VREFBUF LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_VREFBUF_Basic_Exported_Types LPBAM VREFBUF Basic Exported Types
  * @brief    LPBAM VREFBUF Basic Exported Types
  * @{
  */

/**
  * @brief LPBAM VREFBUF Configuration Structure definition.
  */
typedef struct
{
  uint16_t BufferMode; /*!< Specifies the VREFBUF mode.
                            This parameter can be a value of @ref LPBAM_VREFBUF_BufferMode */

} LPBAM_VREFBUF_Config_t;

/**
  * @brief LPBAM VREFBUF Configuration Node Structure definition.
  */
typedef struct
{
  LPBAM_Desc_t NodeDesc;         /*!< Defines the node descriptor fields     */

  VREFBUF_TypeDef *pInstance;    /*!< Defines the VREFBUF instance           */

  LPBAM_VREFBUF_Config_t Config; /*!< Defines the VREFBUF user configuration */

} LPBAM_VREFBUF_ConfNode_t;

/**
  * @brief LPBAM VREFBUF Configuration Node ID Enumeration definition.
  *
  */
typedef enum
{
  LPBAM_VREFBUF_MODE_ID, /*!< LPBAM VREFBUF mode selection node ID  */

} LPBAM_VREFBUF_ID_t;
/**
  * @}
  */

/* Include LPBAM VREFBUF advanced module */
#include "stm32_adv_lpbam_vrefbuf.h"

/** @addtogroup LPBAM_VREFBUF_Basic_Exported_Functions LPBAM VREFBUF Basic Exported Functions
  * @{
  */

/**
  * @brief LPBAM_VREFBUF_FillNodeConfig.
  */
LPBAM_Status_t LPBAM_VREFBUF_FillNodeConfig(LPBAM_VREFBUF_ConfNode_t const *const pConfNode,
                                            DMA_NodeConfTypeDef      *const pDMANodeConfig);
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

#endif /* STM32_LPBAM_VREFBUF_H */
