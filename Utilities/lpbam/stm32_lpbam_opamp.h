/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_opamp.h
  * @author  MCD Application Team
  * @brief   header for the stm32_lpbam_opamp.c file
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
#ifndef STM32_LPBAM_OPAMP_H
#define STM32_LPBAM_OPAMP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_platform_lpbam_opamp.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup OPAMP_Basic
  * @brief      OPAMP LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_OPAMP_Basic_Exported_Types LPBAM OPAMP Basic Exported Types
  * @brief    LPBAM OPAMP Exported Types
  * @{
  */

/**
  * @brief LPBAM OPAMP Configuration Structure definition.
  */
typedef struct
{
  uint32_t Mode;               /*!< Specifies the OPAMP Mode (Standalone, Follower or PGA).
                                    This parameter can be a value of @ref LPBAM_OPAMP_Mode                            */

  uint32_t NonInvertingInput;  /*!< Specifies the OPAMP non inverting input of the OPAMP.
                                    This parameter can be a value of @ref LPBAM_OPAMP_NonInvertingInput               */

  uint32_t InvertingInput;     /*!< Specifies the inverting input in Standalone & PGA modes. In Follower mode this
                                    parameter is Not Applicable.
                                    This parameter can be a value of @ref LPBAM_OPAMP_InvertingInput                  */

  uint32_t PgaGain;            /*!< Specifies the OPAMP PGA gain.
                                    This parameter can be a value of @ref LPBAM_OPAMP_PgaGain                         */

  LPBAM_FunctionalState State; /*!< Specifies the OPAMP enable state.
                                    This parameter can be ENABLE or DISABLE                                           */

} LPBAM_OPAMP_Config_t;

/**
  * @brief LPBAM OPAMP Configuration Node Structure definition.
  */
typedef struct
{
  LPBAM_Desc_t NodeDesc;       /*!< Defines the node descriptor fields   */

  OPAMP_TypeDef *Instance;     /*!< Defines the OPAMP instance           */

  LPBAM_OPAMP_Config_t Config; /*!< Defines the OPAMP user configuration */

} LPBAM_OPAMP_ConfNode_t;

/**
  * @brief LPBAM OPAMP Configuration Node ID Enumeration definition.
  *
  */
typedef enum
{
  LPBAM_OPAMP_CONFIG_ID, /*!< LPBAM OPAMP configuration node ID */

} LPBAM_OPAMP_ID_t;
/**
  * @}
  */

/* Include LPBAM OPAMP advanced module */
#include "stm32_adv_lpbam_opamp.h"

/** @defgroup LPBAM_OPAMP_Basic_Exported_Functions LPBAM OPAMP Basic Exported Functions
  * @{
  */

/**
  * @brief LPBAM_OPAMP_FillNodeConfig.
  */
LPBAM_Status_t LPBAM_OPAMP_FillNodeConfig(LPBAM_OPAMP_ConfNode_t const *const pConfNode,
                                          DMA_NodeConfTypeDef    *const pDMANodeConfig);
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

#endif /* STM32_LPBAM_OPAMP_H */
