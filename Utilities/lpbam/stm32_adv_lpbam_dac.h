/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_dac.h
  * @author  MCD Application Team
  * @brief   header for the stm32_adv_lpbam_dac.c file
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
#ifndef STM32_ADV_LPBAM_DAC_H
#define STM32_ADV_LPBAM_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_platform_lpbam_dac.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup DAC_Advanced DAC_Advanced
  * @brief    DAC Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_DAC_Advanced_Exported_Types LPBAM DAC Advanced Exported Types
  * @brief    LPBAM DAC Advanced Exported Types
  * @{
  */

/**
  * @brief  LPBAM DAC advanced configuration structure definition.
  */
typedef struct
{
  uint32_t DAC_Trigger; /*!< Specifies the external trigger for the selected DAC channel.
                             This parameter can be a value of @ref LPBAM_DAC_Trigger_Selection */

} LPBAM_DAC_ConfigAdvConf_t;


/**
  * @brief  LPBAM DAC advanced data structure definition.
  */
typedef struct
{
  uint32_t Alignment; /*!< Specifies the data alignment form.
                           This parameter can be a value of @ref LPBAM_DAC_DataAlignment */

  uint32_t *pData;    /*!< The source buffer address.                                    */

  uint16_t Size;      /*!< The size of data to be converted.                             */

} LPBAM_DAC_DataAdvConf_t;

/**
  * @brief  LPBAM DAC advanced full structure definition.
  */
typedef struct
{
  uint32_t DAC_Trigger; /*!< Specifies the external trigger for the selected DAC channel.
                             This parameter can be a value of @ref LPBAM_DAC_Trigger_Selection   */

  uint32_t Alignment;   /*!< Specifies the data alignment form.
                             This parameter can be a value of @ref LPBAM_DAC_DataAlignment       */

  uint32_t *pData;      /*!< The source buffer address.                                          */

  uint16_t Size;        /*!< The size of data to be converted.                                   */

} LPBAM_DAC_FullAdvConf_t;

/**
  * @brief  LPBAM DAC conversion configuration descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[2U]; /*!< Specifies the content of nodes required for DAC transfers : 2 different nodes are
                                  needed                                                                              */

  uint32_t pReg[2U];          /*!< Specifies the content of register to be updated : 2 different values are needed    */

} LPBAM_DAC_ConvConfigDesc_t;

/**
  * @brief  LPBAM DAC conversion data descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DAC transfers : only one node is needed
                                                                                                                      */

} LPBAM_DAC_ConvDataDesc_t;

/**
  * @brief  LPBAM DAC conversion full descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[3U]; /*!< Specifies the content of nodes required for DAC transfers : 3 different nodes are
                                  needed                                                                              */

  uint32_t pReg[2U];          /*!< Specifies the content of register to be updated : 2 different values are needed    */

} LPBAM_DAC_ConvFullDesc_t;

/**
  * @brief  LPBAM stop conversion descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DAC stop conversion transfers : only one
                                   node is needed                                                                     */

  uint32_t pReg[1U];          /*!< Specifies the content of register to be updated : only one value is needed         */

} LPBAM_DAC_StopConvDesc_t;

/**
  * @}
  */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_DAC_Advanced_Exported_Functions LPBAM DAC Advanced Exported Functions
  * @{
  */

/**
  * @brief  ADV_LPBAM_DAC_Conversion_SetConfigQ.
  */
LPBAM_Status_t ADV_LPBAM_DAC_Conversion_SetConfigQ(DAC_TypeDef                *const pInstance,
                                                   uint32_t                   Channel,
                                                   LPBAM_DMAListInfo_t        const *const pDMAListInfo,
                                                   LPBAM_DAC_ConfigAdvConf_t  const *const pConfig,
                                                   LPBAM_DAC_ConvConfigDesc_t *const pDescriptor,
                                                   DMA_QListTypeDef           *const pQueue);
/**
  * @brief  ADV_LPBAM_DAC_Conversion_SetDataQ.
  */
LPBAM_Status_t ADV_LPBAM_DAC_Conversion_SetDataQ(DAC_TypeDef              *const pInstance,
                                                 uint32_t                 Channel,
                                                 LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                                 LPBAM_DAC_DataAdvConf_t  const *const pData,
                                                 LPBAM_DAC_ConvDataDesc_t *const pDescriptor,
                                                 DMA_QListTypeDef         *const pQueue);
/**
  * @brief  ADV_LPBAM_DAC_Conversion_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_DAC_Conversion_SetFullQ(DAC_TypeDef              *const pInstance,
                                                 uint32_t                 Channel,
                                                 LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                                 LPBAM_DAC_FullAdvConf_t  const *const pFull,
                                                 LPBAM_DAC_ConvFullDesc_t *const pDescriptor,
                                                 DMA_QListTypeDef         *const pQueue);
/**
  * @brief  ADV_LPBAM_DAC_Stop_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_DAC_Stop_SetFullQ(DAC_TypeDef              *const pInstance,
                                           LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                           LPBAM_DAC_StopConvDesc_t *const pDescriptor,
                                           DMA_QListTypeDef         *const pQueue);
/**
  * @brief  ADV_LPBAM_DAC_EnableDMARequests.
  */
LPBAM_Status_t ADV_LPBAM_DAC_EnableDMARequests(DAC_TypeDef *const pInstance,
                                               uint32_t    Channel);
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

#endif /* STM32_ADV_LPBAM_DAC_H */
