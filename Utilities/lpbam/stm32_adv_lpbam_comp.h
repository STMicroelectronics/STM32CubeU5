/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_comp.h
  * @author  MCD Application Team
  * @brief   header for the stm32_adv_lpbam_comp.c file
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
#ifndef STM32_ADV_LPBAM_COMP_H
#define STM32_ADV_LPBAM_COMP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_platform_lpbam_comp.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup COMP_Advanced COMP_Advanced
  * @brief    COMP Advanced LPBAM module driver
  * @{
  */

/* Exported types ----------------------------------------------------------------------------------------------------*/

/** @defgroup LPBAM_COMP_Advanced_Exported_Types LPBAM COMP Advanced Exported Types
  * @brief    LPBAM COMP Advanced Exported Types
  * @{
  */

/**
  * @brief LPBAM start comparator advanced configuration structure definition.
  */
typedef struct
{
  uint32_t OutputPol;  /*!< Specifies the COMP polarity.
                            This parameter can be a value of @ref LPBAM_COMP_OutputPolarity                   */

  uint32_t InputPlus;  /*!< Specifies the COMP input plus.
                            Note:  When the comparator is configured as input window mode, the input plus will
                                   be shared between the two comparator instances. For the comparator instance
                                   which is not driving the window mode InputPlus parameter has no effect.
                                   This parameter can be a value of @ref LPBAM_COMP_InputPlus                 */

  uint32_t InputMinus; /*!< Specifies the COMP input minus.
                            This parameter can be a value of @ref LPBAM_COMP_InputMinus                       */

} LPBAM_COMP_StartFullAdvConf_t;

/**
  * @brief LPBAM output level advanced information structure definition.
  */
typedef struct
{
  uint32_t *pData; /*!< Specifies the data buffer address     */

  uint16_t Size;   /*!< Specifies the size of data to be read */

} LPBAM_COMP_OutLevelFullAdvConf_t;

/**
  * @brief LPBAM startand stop descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DMA queue execution : only one node is
                                   needed                                                                             */

  uint32_t pReg[1U];          /*!< Specifies the content of register to be updated : only one value is needed         */

} LPBAM_COMP_StartFullDesc_t, LPBAM_COMP_StopFullDesc_t;

/**
  * @brief LPBAM output level descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DMA queue execution : only one node is
                                   needed                                                                             */

} LPBAM_COMP_OutLevelFullDesc_t;
/**
  * @}
  */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_COMP_Advanced_Exported_Functions  LPBAM COMP Advanced Exported Functions
  * @{
  */
/**
  * @brief ADV_LPBAM_COMP_Start_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_COMP_Start_SetFullQ(COMP_TypeDef                  *const pInstance,
                                             LPBAM_DMAListInfo_t           const *const pDMAListInfo,
                                             LPBAM_COMP_StartFullAdvConf_t const *const pStartFull,
                                             LPBAM_COMP_StartFullDesc_t    *const pDescriptor,
                                             DMA_QListTypeDef              *const pQueue);
/**
  * @brief ADV_LPBAM_COMP_Stop_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_COMP_Stop_SetFullQ(COMP_TypeDef              *const pInstance,
                                            LPBAM_DMAListInfo_t       const *const pDMAListInfo,
                                            LPBAM_COMP_StopFullDesc_t *const pDescriptor,
                                            DMA_QListTypeDef          *const pQueue);
/**
  * @brief ADV_LPBAM_COMP_OutputLevel_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_COMP_OutputLevel_SetFullQ(COMP_TypeDef                     *const pInstance,
                                                   LPBAM_DMAListInfo_t              const *const pDMAListInfo,
                                                   LPBAM_COMP_OutLevelFullAdvConf_t const *const pOutLevelFull,
                                                   LPBAM_COMP_OutLevelFullDesc_t    *const pDescriptor,
                                                   DMA_QListTypeDef                 *const pQueue);
/**
  * @}
  */

/* Exported macro ----------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_COMP_Advanced_Exported_Macros LPBAM COMP Advanced Exported Macros
  * @brief    LPBAM COMP Advanced Exported macro
  * @{
  */

/** @brief  Check COMP output level.
  * @param  __OUTPUTLEVEL__ : Specifies the output level.
  * @retval The output level (TRUE or FALSE).
  */
#define __LPBAM_COMP_GET_OUTPUTLEVEL(__OUTPUTLEVEL__) \
  ((((__OUTPUTLEVEL__) & LPBAM_COMP_OUTPUT_MASK) == LPBAM_COMP_OUTPUT_MASK) ? 1U : 0U)
/**
  * @}
  */

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

#endif /* STM32_ADV_LPBAM_COMP_H */
