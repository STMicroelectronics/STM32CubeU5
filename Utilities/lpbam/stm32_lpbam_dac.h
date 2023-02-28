/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_dac.h
  * @author  MCD Application Team
  * @brief   header for the stm32_lpbam_dac.c file
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
#ifndef STM32_LPBAM_DAC_H
#define STM32_LPBAM_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_platform_lpbam_dac.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup DAC_Basic
  * @brief      DAC LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_DAC_Basic_Exported_Types LPBAM DAC Basic Exported Types
  * @brief    LPBAM DAC Basic Exported Types
  * @{
  */

/**
  * @brief LPBAM DAC Configuration Structure definition.
  */
typedef struct
{
  LPBAM_FunctionalState State; /*!< Specifies the DAC enable state.
                                    This parameter can be ENABLE or DISABLE                           */

  uint32_t Channel;            /*!< Specifies the selected channel.
                                    This parameter can be a value of @ref LPBAM_DAC_Channel           */

  uint32_t DAC_Trigger;        /*!< Specifies the external trigger for the selected DAC channel.
                                    This parameter can be a value of @ref LPBAM_DAC_Trigger_Selection */

  uint16_t Size;               /*!< Specifies the DAC DMA access number                               */

  uint32_t Alignment;          /*!< Specifies the data alignment form.
                                    This parameter can be a value of @ref LPBAM_DAC_DataAlignment     */

} LPBAM_DAC_Config_t;

/**
  * @brief LPBAM DAC Configuration Node Structure definition.
  */
typedef struct
{
  LPBAM_Desc_t NodeDesc;         /*!< Defines the node descriptor fields */

  DAC_TypeDef *pInstance;        /*!< Defines the DAC instance           */

  LPBAM_DAC_Config_t Config[2U]; /*!< Defines the DAC user configuration */

} LPBAM_DAC_ConfNode_t;

/**
  * @brief LPBAM DAC Configuration Node ID Enumeration definition.
  *
  */
typedef enum
{
  LPBAM_DAC_CONFIG_ID, /*!< LPBAM DAC configuration node ID */

  LPBAM_DAC_DATA_ID,   /*!< LPBAM DAC data node ID          */

} LPBAM_DAC_ID_t;
/**
  * @}
  */

/* Include LPBAM DAC Advanced module */
#include "stm32_adv_lpbam_dac.h"

/** @addtogroup LPBAM_DAC_Basic_Exported_Functions LPBAM DAC Basic Exported Functions
  * @{
  */

/**
  * @brief LPBAM_DAC_FillNodeConfig.
  */
LPBAM_Status_t LPBAM_DAC_FillNodeConfig(LPBAM_DAC_ConfNode_t const *const pConfNode,
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

#endif /* STM32_LPBAM_DAC_H */
