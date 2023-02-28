/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_lptim.h
  * @author  MCD Application Team
  * @brief   header for the stm32_adv_lpbam_lptim.c file
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
#ifndef STM32_ADV_LPBAM_LPTIM_H
#define STM32_ADV_LPBAM_LPTIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_platform_lpbam_lptim.h"

/* Exported types ----------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup LPTIM_Advanced LPTIM_Advanced
  * @brief    LPTIM Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_LPTIM_Advanced_Exported_Types LPBAM LPTIM Advanced Exported Types
  * @brief    LPBAM LPTIM Advanced Exported Types
  * @{
  */

/**
  * @brief LPBAM LPTIM PWM advanced information structure definition.
  */
typedef struct
{
  uint16_t PeriodValue;                   /*!< Specifies the period values             */

  LPBAM_FunctionalState UpdatePeriod;     /*!< Specifies the period update state
                                               This parameter can be ENABLE or DISABLE */

  uint16_t PulseValue;                    /*!< Specifies the pulse values              */

  LPBAM_FunctionalState UpdatePulse;      /*!< Specifies the pulse update state
                                               This parameter can be ENABLE or DISABLE */

  uint8_t RepetitionValue;                /*!< Specifies the repetition values         */

  LPBAM_FunctionalState UpdateRepetition; /*!< Specifies the repetition update state
                                               This parameter can be ENABLE or DISABLE */

} LPBAM_LPTIM_PWMFullAdvConf_t;

/**
  * @brief LPBAM LPTIM input capture and get counter advanced information structure definition.
  */
typedef struct
{
  uint16_t *pData; /*!< Specifies the address of data buffer  */

  uint16_t Size;   /*!< Specifies the size of data to be read */

} LPBAM_LPTIM_ICFullAdvConf_t, LPBAM_LPTIM_GetCounterFullAdvConf_t;

/**
  * @brief LPBAM LPTIM update event detection advanced information structure definition.
  */
typedef struct
{
  uint16_t Period;    /*!< Specifies the period value             */

  uint8_t Repetition; /*!< Specifies the period repetition number */

} LPBAM_LPTIM_UEFullAdvConf_t;

/**
  * @brief LPBAM LPTIM start advanced information structure definition.
  */
typedef struct
{
  uint32_t StartMode; /*!< Specifies the LPTIM Start Mode.
                           This parameter can be a value of @ref LPBAM_LPTIM_Start_Mode                    */

  uint32_t WakeupIT;  /*!< Specifies the wake up source interrupt
                           This parameter can be one or a combination of @ref LPBAM_LPTIM_Wakeup_Interrupt */

} LPBAM_LPTIM_StartFullAdvConf_t;

/**
  * @brief LPBAM LPTIM start advanced information structure definition.
  */
typedef struct
{
  uint32_t Mode; /*!< Specifies the LPTIM Mode.
                      This parameter can be a value of @ref LPBAM_LPTIM_Stop_Mode */

} LPBAM_LPTIM_StopFullAdvConf_t;

/**
  * @brief LPBAM LPTIM update PWM descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[3U]; /*!< Specifies the content of nodes required for DMA queue execution : 3 different nodes
                                   are needed                                                                         */

  uint32_t pReg[3U];          /*!< Specifies the content of register to be updated : 3 value is needed                */

} LPBAM_LPTIM_PWMFullDesc_t;

/**
  * @brief LPBAM LPTIM input capture and get counter descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DMA queue execution : only one node is
                                   needed                                                                             */

} LPBAM_LPTIM_ICFullDesc_t, LPBAM_LPTIM_GetCounterFullDesc_t;

/**
  * @brief LPBAM LPTIM update event detection descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[2U]; /*!< Specifies the content of nodes required for DMA queue execution : 2 node is needed */

  uint32_t pReg[2U];          /*!< Specifies the content of register to be updated : 2 value is needed                */

} LPBAM_LPTIM_UEFullDesc_t;

/**
  * @brief LPBAM LPTIM start descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[3U]; /*!< Specifies the content of nodes required for DMA queue execution : 3 nodes are
                                   needed                                                                             */

  uint32_t pReg[3U];          /*!< Specifies the content of register to be updated : 3 values is needed               */

} LPBAM_LPTIM_StartFullDesc_t;

/**
  * @brief LPBAM LPTIM stop descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DMA queue execution : only one node is
                                   needed                                                                             */

  uint32_t pReg[1U];          /*!< Specifies the content of register to be updated : only one value is needed         */

} LPBAM_LPTIM_StopFullDesc_t;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup LPBAM_LPTIM_Advanced_Exported_Functions LPBAM LPTIM Advanced Exported Functions
  * @{
  */

/**
  * @brief ADV_LPBAM_LPTIM_PWM_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_PWM_SetFullQ(LPTIM_TypeDef                *const pInstance,
                                            uint32_t                     Channel,
                                            LPBAM_DMAListInfo_t          const *const pDMAListInfo,
                                            LPBAM_LPTIM_PWMFullAdvConf_t const *const pPWMFull,
                                            LPBAM_LPTIM_PWMFullDesc_t    *const pDescriptor,
                                            DMA_QListTypeDef             *const pQueue);
/**
  * @brief ADV_LPBAM_LPTIM_IC_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_IC_SetFullQ(LPTIM_TypeDef               *const pInstance,
                                           uint32_t                    Channel,
                                           LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                           LPBAM_LPTIM_ICFullAdvConf_t const *const pICFull,
                                           LPBAM_LPTIM_ICFullDesc_t    *const pDescriptor,
                                           DMA_QListTypeDef            *const pQueue);
/**
  * @brief ADV_LPBAM_LPTIM_UE_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_UE_SetFullQ(LPTIM_TypeDef              *const pInstance,
                                           LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                           LPBAM_LPTIM_UEFullAdvConf_t const *const pUEFull,
                                           LPBAM_LPTIM_UEFullDesc_t    *const pDescriptor,
                                           DMA_QListTypeDef            *const pQueue);
/**
  * @brief ADV_LPBAM_LPTIM_GetCounter_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_GetCounter_SetFullQ(LPTIM_TypeDef                       *const pInstance,
                                                   LPBAM_DMAListInfo_t                 const *const pDMAListInfo,
                                                   LPBAM_LPTIM_GetCounterFullAdvConf_t const *const pGetCounterFull,
                                                   LPBAM_LPTIM_GetCounterFullDesc_t    *const pDescriptor,
                                                   DMA_QListTypeDef                    *const pQueue);

/**
  * @brief ADV_LPBAM_LPTIM_Start_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_Start_SetFullQ(LPTIM_TypeDef                  *const pInstance,
                                              LPBAM_DMAListInfo_t            const *const pDMAListInfo,
                                              LPBAM_LPTIM_StartFullAdvConf_t const *const pStartFull,
                                              LPBAM_LPTIM_StartFullDesc_t    *const pDescriptor,
                                              DMA_QListTypeDef               *const pQueue);
/**
  * @brief ADV_LPBAM_LPTIM_Stop_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_Stop_SetFullQ(LPTIM_TypeDef                 *const pInstance,
                                             LPBAM_DMAListInfo_t           const *const pDMAListInfo,
                                             LPBAM_LPTIM_StopFullAdvConf_t const *const pStopFull,
                                             LPBAM_LPTIM_StopFullDesc_t    *const pDescriptor,
                                             DMA_QListTypeDef              *const pQueue);
/**
  * @brief ADV_LPBAM_LPTIM_EnableDMARequests.
  */
LPBAM_Status_t ADV_LPBAM_LPTIM_EnableDMARequests(LPTIM_TypeDef *const pInstance,
                                                 uint32_t      Channel);
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

#endif /* STM32_ADV_LPBAM_LPTIM_H */
