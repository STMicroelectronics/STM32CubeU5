/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_comp.h
  * @author  MCD Application Team
  * @brief   header for the stm32_lpbam_comp.c file
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
#ifndef STM32_LPBAM_COMP_H
#define STM32_LPBAM_COMP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_platform_lpbam_comp.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup COMP_Basic
  * @brief      COMP LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_COMP_BASIC_Exported_Types LPBAM COMP Basic Exported Types
  * @brief    LPBAM COMP Basic Exported Types
  * @{
  */

/**
  * @brief LPBAM COMP Configuration Structure definition.
  */
typedef struct
{
  uint32_t OutputPol;          /*!< Specifies the COMP polarity.
                                    This parameter can be a value of @ref LPBAM_COMP_OutputPolarity   */

  uint32_t InputPlus;          /*!< Specifies the COMP input plus.
                                    This parameter can be a value of @ref LPBAM_COMP_InputPlus        */

  uint32_t InputMinus;         /*!< Specifies the COMP input minus.
                                    This parameter can be a value of @ref LPBAM_COMP_InputMinus       */

  LPBAM_FunctionalState State; /*!< Specifies the COMP enable state.
                                    This parameter can be ENABLE or DISABLE                           */


  uint16_t Size;               /*!< Specifies the COMP number of sample's output value                */

} LPBAM_COMP_Config_t;

/**
  * @brief LPBAM COMP Configuration Node Structure definition.
  */
typedef struct
{
  LPBAM_Desc_t NodeDesc;      /*!< Defines the node descriptor fields  */

  COMP_TypeDef *Instance;     /*!< Defines the COMP instance           */

  LPBAM_COMP_Config_t Config; /*!< Defines the COMP user configuration */

} LPBAM_COMP_ConfNode_t;

/**
  * @brief LPBAM COMP Configuration Node ID Enumeration definition.
  *
  */
typedef enum
{
  LPBAM_COMP_CONFIG_ID,    /*!< LPBAM COMP configuration node ID     */

  LPBAM_COMP_READOUTPUT_ID /*!< LPBAM COMP read output value node ID */

} LPBAM_COMP_ID_t;
/**
  * @}
  */

/* Include LPBAM COMP advanced module */
#include "stm32_adv_lpbam_comp.h"

/** @defgroup LPBAM_COMP_Basic_Exported_Functions LPBAM COMP Basic Exported Functions
  * @{
  */

/**
  * @brief LPBAM_COMP_FillNodeConfig.
  *
  */
LPBAM_Status_t LPBAM_COMP_FillNodeConfig(LPBAM_COMP_ConfNode_t const *const pConfNode,
                                         DMA_NodeConfTypeDef   *const pDMANodeConfig);

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

#endif /* STM32_LPBAM_COMP_H */
