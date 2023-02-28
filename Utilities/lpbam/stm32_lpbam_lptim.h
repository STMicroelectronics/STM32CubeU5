/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_lptim.h
  * @author  MCD Application Team
  * @brief   header for the stm32_lpbam_lptim.c file
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

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef STM32_LPBAM_LPTIM_H
#define STM32_LPBAM_LPTIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------- */
#include "stm32_platform_lpbam_lptim.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup LPTIM_Basic
  * @brief      LPTIM LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_LPTIM_Basic_Exported_Types LPBAM LPTIM Basic Exported Types
  * @brief    LPBAM LPTIM Exported Types
  * @{
  */

/**
  * @brief LPBAM LPTIM Configuration Structure definition.
  */
typedef struct
{
  uint32_t Channel;                      /*!< Specifies the LPTIM Channel.
                                              This parameter can be a value of @ref LPBAM_LPTIM_Channel        */

  uint32_t Mode;                         /*!< Specifies the LPTIM Mode.
                                              This parameter can be a value of @ref LPBAM_LPTIM_Stop_Mode      */

  uint32_t StartMode;                    /*!< Specifies the LPTIM Start Mode.
                                              This parameter can be a value of @ref LPBAM_LPTIM_Start_Mode     */

  LPBAM_FunctionalState UpdateStartMode; /*!< Specifies the start mode update state
                                              This parameter can be ENABLE or DISABLE                          */

  LPBAM_FunctionalState State;           /*!< Specifies the LPTIM.
                                              This parameter can be ENABLE or DISABLE                          */

  uint32_t WakeupIT;                     /*!< Specifies the wake up source interrupt
                                              This parameter can be one or a combination of
                                              @ref LPBAM_LPTIM_Wakeup_Interrupt                                */

  uint32_t Size;                         /*!< Specifies the LPTIM update number for PWM generation             */

  uint32_t CaptureCount;                 /*!< Specifies the LPTIM DMA access number for input capture          */

  uint32_t CountValue;                   /*!< Specifies the LPTIM
                                              this parameter can be read counter value                         */

} LPBAM_LPTIM_Config_t;

/**
  * @brief LPBAM LPTIM Configuration Node Structure definition.
  */
typedef struct
{
  LPBAM_Desc_t NodeDesc;       /*!< Defines the node descriptor fields   */

  LPTIM_TypeDef *pInstance;    /*!< Defines the LPTIM instance           */

  LPBAM_LPTIM_Config_t Config; /*!< Defines the LPTIM user configuration */

} LPBAM_LPTIM_ConfNode_t;

/**
  * @brief LPBAM LPTIM Configuration Node ID Enumeration definition.
  *
  */
typedef enum
{
  LPBAM_LPTIM_CONFIG_ID,            /*!< LPBAM LPTIM Configuration ID           */

  LPBAM_LPTIM_UPDATE_PERIOD_ID,     /*!< LPBAM LPTIM Update Period node ID      */

  LPBAM_LPTIM_UPDATE_PULSE_ID,      /*!< LPBAM LPTIM Update Pulse node ID       */

  LPBAM_LPTIM_UPDATE_REPETITION_ID, /*!< LPBAM LPTIM Update Repetition node ID  */

  LPBAM_LPTIM_GET_COUNTER_ID,       /*!< LPBAM LPTIM Get Counter node ID        */

  LPBAM_LPTIM_INPUT_CAPTURE_ID,     /*!< LPBAM LPTIM Input Capture node ID      */

  LPBAM_LPTIM_WAKEUP_IT_ID,         /*!< LPBAM LPTIM Wakeup Interrupt node ID   */

  LPBAM_LPTIM_CLEARFLAG_ID,         /*!< LPBAM LPTIM Clear Flag node ID         */

} LPBAM_LPTIM_ID_t;
/**
  * @}
  */

/* Include LPBAM LPTIM Advanced module */
#include "stm32_adv_lpbam_lptim.h"

/** @addtogroup LPBAM_LPTIM_Basic_Exported_Functions LPBAM LPTIM Basic Exported Functions
  * @{
  */

/**
  * @brief LPBAM_LPTIM_FillNodeConfig.
  *
  */
LPBAM_Status_t LPBAM_LPTIM_FillNodeConfig(LPBAM_LPTIM_ConfNode_t const *const pConfNode,
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

#endif /* STM32_LPBAM_LPTIM_H */
