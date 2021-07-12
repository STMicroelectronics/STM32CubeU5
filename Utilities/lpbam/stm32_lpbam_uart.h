/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_uart.h
  * @author  MCD Application Team
  * @brief   header for the stm32_lpbam_uart.c file
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
#ifndef  STM32_LPBAM_UART_H
#define  STM32_LPBAM_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_platform_lpbam_uart.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup UART_Basic
  * @brief      UART LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_UART_Basic_Exported_Types LPBAM UART Basic Exported Types
  * @brief    LPBAM UART Basic Exported Types
  * @{
  */

/**
  * @brief LPBAM UART Configuration Structure definition.
  */
typedef struct
{
  LPBAM_FunctionalState State;                  /*!< Specifies the UART enable state.
                                                     This parameter can be ENABLE or DISABLE                          */


  LPBAM_UART_AutonomousModeConf_t AutoModeConf; /*!< Specifies the autonomous mode configuration
                                                     Please refer to LPBAM_UART_AutonomousModeConf_t definition in
                                                     stm32_platform_lpbam_uart.h for more information.                */

  uint32_t Mode;                                /*!< Specifies whether the Receive or Transmit mode is enabled
                                                     or disabled.
                                                     This parameter can be a value of @ref LPBAM_UART_Mode            */

  uint32_t WakeupIT;                            /*!< Specifies the wake up source interrupt                           */

  uint8_t *pData;                               /*!< Specifies the address of data buffer                             */

  uint16_t Size;                                /*!< Specifies the UART data number of bytes to
                                                     transfer or to receive                                           */
} LPBAM_UART_Config_t;

/**
  * @brief LPBAM UART Configuration Node Structure definition.
  */
typedef struct
{
  LPBAM_Desc_t        NodeDesc;   /*!< Defines the node descriptor fields  */

  USART_TypeDef       *pInstance; /*!< Defines the UART instance           */

  LPBAM_UART_Config_t Config;     /*!< Defines the UART user configuration */

} LPBAM_UART_ConfNode_t;

/**
  * @brief LPBAM UART Configuration Node ID Enumeration definition.
  *
  */
typedef enum
{
  LPBAM_UART_CONFIG_ID,         /*!< LPBAM UART config state node ID           */

  LPBAM_UART_AUTONOMOUSMODE_ID, /*!< LPBAM UART config autonomous mode node ID */

  LPBAM_UART_RECEIVE_DATA_ID,   /*!< LPBAM UART receive data transfer node ID  */

  LPBAM_UART_TRANSMIT_DATA_ID,  /*!< LPBAM UART transmit data transfer node ID */

  LPBAM_UART_CLEARFLAG_ID       /*!< LPBAM UART clear flag node ID             */

} LPBAM_UART_ID_t;
/**
  * @}
  */

/* Include LPBAM UART Advanced module */
#include "stm32_adv_lpbam_uart.h"

/** @addtogroup LPBAM_UART_Basic_Exported_Functions LPBAM UART Basic Exported Functions
  * @brief    LPBAM UART Basic Exported Functions
  * @{
  */

/**
  * @brief LPBAM_UART_FillNodeConfig.
  */
LPBAM_Status_t LPBAM_UART_FillNodeConfig(LPBAM_UART_ConfNode_t const *const pConfNode,
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

#endif /* STM32_LPBAM_UART_H */
