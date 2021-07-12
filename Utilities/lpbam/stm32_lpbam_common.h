/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_common.h
  * @author  MCD Application Team
  * @brief   header for the stm32_lpbam_common.c file
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
#ifndef STM32_LPBAM_COMMON_H
#define STM32_LPBAM_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_platform_lpbam_common.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup COMMON_Basic
  * @{
  */

/** @defgroup LPBAM_COMMON_Basic_Exported_Types LPBAM COMMON Basic Exported Types
  * @brief    LPBAM COMMON Basic Exported Types
  * @{
  */

/**
  * @brief LPBAM COMMON Transfer Configuration Structure definition.
  */
typedef struct
{
  LPBAM_DMA_NodeConfTypeDef TransferConfig;      /*!< Specifies the DMA transfer configuration                        */
  LPBAM_FunctionalState UpdateSrcInc;            /*!< Specifies whether the data source increment to be updated or not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring SrcInc in TransferConfig
                                                      is mandatory                                                    */
  LPBAM_FunctionalState UpdateDestInc;           /*!< Specifies whether the data destination increment to be updated
                                                      or not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring DestInc in
                                                      TransferConfig is mandatory                                     */
  LPBAM_FunctionalState UpdateSrcDataWidth;      /*!< Specifies whether the data source data width to be updated or
                                                      not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring SrcDataWidth in
                                                      TransferConfig is mandatory                                     */
  LPBAM_FunctionalState UpdateDestDataWidth;     /*!< Specifies whether the destination data width to be updated or
                                                      not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring DestDataWidth in
                                                      TransferConfig is mandatory                                     */
  LPBAM_FunctionalState UpdateTransferEventMode; /*!< Specifies whether the transfer event mode to be updated or not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring TransferEventMode in
                                                      TransferConfig is mandatory                                     */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  LPBAM_FunctionalState UpdateSrcSecure;         /*!< Specifies whether the source security to be updated or not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring SrcSecure in
                                                      TransferConfig is mandatory                                     */
  LPBAM_FunctionalState UpdateDestSecure;        /*!< Specifies whether the destination security to be updated or not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring DestSecure in
                                                      TransferConfig is mandatory                                     */
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

} LPBAM_COMMON_TransferConfig_t;

/**
  * @brief LPBAM COMMON Trigger Configuration Structure definition.
  */
typedef struct
{
  LPBAM_DMA_TriggerConfTypeDef TriggerConfig; /*!< Specifies the DMA trigger configuration */

} LPBAM_COMMON_TrigConfig_t;
/**
  * @}
  */

/* Include LPBAM COMMON Advanced module */
#include "stm32_adv_lpbam_common.h"

/** @addtogroup LPBAM_COMMON_Basic_Exported_Functions LPBAM COMMON Basic Exported Functions
  * @{
  */

/**
  * @brief LPBAM_SetDMATransferConfig.
  */
LPBAM_Status_t LPBAM_SetDMATransferConfig(LPBAM_COMMON_TransferConfig_t const *const pTransferConfig,
                                          DMA_NodeTypeDef               *const pDMANode);
/**
  * @brief LPBAM_SetDMATriggerConfig.
  */
LPBAM_Status_t LPBAM_SetDMATriggerConfig(LPBAM_COMMON_TrigConfig_t const *const pTrigConfig,
                                         DMA_NodeTypeDef           *const pDMANode);
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

#endif /* STM32_LPBAM_COMMON_H */
