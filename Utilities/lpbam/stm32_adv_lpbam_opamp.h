/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_opamp.h
  * @author  MCD Application Team
  * @brief   header for the stm32_adv_lpbam_opamp.c file
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
#ifndef STM32_ADV_LPBAM_OPAMP_H
#define STM32_ADV_LPBAM_OPAMP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_platform_lpbam_opamp.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup OPAMP_Advanced OPAMP_Advanced
  * @brief    OPAMP Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_OPAMP_Advanced_Exported_Types LPBAM OPAMP Advanced Exported Types
  * @brief    Advanced LPBAM OPAMP Exported Types
  * @{
  */

/**
  * @brief LPBAM specific advanced OPAMP information structure definition.
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

} LPBAM_OPAMP_StartFullAdvConf_t;

/**
  * @brief LPBAM start descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DMA queue execution : 1 node is needed */

  uint32_t pReg[1U];          /*!< Specifies the content of register to be updated : 1 value is needed                */

} LPBAM_OPAMP_StartFullDesc_t;
/**
  * @}
  */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_OPAMP_Advanced_Exported_Functions LPBAM OPAMP Advanced Exported Functions
  * @{
  */

/**
  * @brief ADV_LPBAM_OPAMP_Start_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_OPAMP_Start_SetFullQ(OPAMP_TypeDef                  *const pInstance,
                                              LPBAM_DMAListInfo_t            const *const pDMAListInfo,
                                              LPBAM_OPAMP_StartFullAdvConf_t const *const pStartFull,
                                              LPBAM_OPAMP_StartFullDesc_t    *const pDescriptor,
                                              DMA_QListTypeDef               *const pQueue);
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

#endif /* STM32_ADV_LPBAM_OPAMP_H */
