/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_uart.h
  * @author  MCD Application Team
  * @brief   header for the stm32_adv_lpbam_uart.c file
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
#ifndef STM32_ADV_LPBAM_UART_H
#define STM32_ADV_LPBAM_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_platform_lpbam_uart.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup UART_Advanced UART_Advanced
  * @brief    UART Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_UART_Advanced_Exported_Types LPBAM UART Advanced Exported Types
  * @brief    Advanced LPBAM UART Exported Types
  * @{
  */

/**
  * @brief LPBAM specific advanced UART information structure definition.
  */
typedef struct
{
  uint32_t Mode;                                /*!< Specifies whether the Receive or Transmit mode is enabled
                                                     or disabled.
                                                     This parameter can be a value of @ref LPBAM_UART_Mode         */

  LPBAM_UART_AutonomousModeConf_t AutoModeConf; /*!< Specifies the autonomous mode configuration
                                                     Please refer to LPBAM_UART_AutonomousModeConf_t definition in
                                                     stm32_platform_lpbam_uart.h for more information.
                                                     This field is used only for transmit APIs.                    */

  uint32_t WakeupIT;                            /*!< This parameter can be one or a combination of
                                                     @ref LPBAM_UART_Wakeup_Interrupt                              */

} LPBAM_UART_ConfigAdvConf_t;

/**
  * @brief LPBAM specific advanced UART information structure definition.
  */
typedef struct
{
  uint8_t *pData; /*!< Specifies the address of data buffer          */

  uint16_t Size;  /*!< Specifies the data number of bytes to be sent */

} LPBAM_UART_DataAdvConf_t;

/**
  * @brief LPBAM specific advanced UART information structure definition.
  */
typedef struct
{
  uint32_t Mode;                                /*!< Specifies whether the Receive or Transmit mode is enabled
                                                     or disabled.
                                                     This parameter can be a value of @ref LPBAM_UART_Mode         */

  LPBAM_UART_AutonomousModeConf_t AutoModeConf; /*!< Specifies the autonomous mode configuration
                                                     Please refer to LPBAM_UART_AutonomousModeConf_t definition in
                                                     stm32_platform_lpbam_uart.h for more information.
                                                     This field is used only for transmit APIs.                    */

  uint32_t WakeupIT;                            /*!< Specifies the wake up source interrupt                        */

  uint8_t *pData;                               /*!< Specifies the address of data buffer                          */

  uint16_t Size;                                /*!< Specifies the UART data number of bytes to
                                                     transfer or to receive                                        */

} LPBAM_UART_FullAdvConf_t;

/**
  * @brief LPBAM UART transmit configuration descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[4U]; /*!< Specifies the content of nodes required for DMA queue execution : 4 different nodes
                                   are needed                                                                         */

  uint32_t pReg[4U];          /*!< Specifies the content of register to be updated : 4 different values are needed    */

} LPBAM_UART_TxConfigDesc_t;

/**
  * @brief LPBAM UART transmit data descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DMA queue execution : only one node
                                   is needed                                                                  */

} LPBAM_UART_TxDataDesc_t;

/**
  * @brief LPBAM UART transmit full descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[5U]; /*!< Specifies the content of nodes required for DMA queue execution : 5 different nodes
                                   are needed                                                                         */

  uint32_t pReg[4U];          /*!< Specifies the content of register to be updated : 4 different values are needed    */

} LPBAM_UART_TxFullDesc_t;

/**
  * @brief LPBAM receive configuration descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[2U]; /*!< Specifies the content of nodes required for DMA queue execution : 2 different nodes
                                   are needed                                                                         */

  uint32_t pReg[2U];          /*!< Specifies the content of register to be updated : 2 different values are needed    */

} LPBAM_UART_RxConfigDesc_t;

/**
  * @brief LPBAM receive data descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[1U]; /*!< Specifies the content of nodes required for DMA queue execution : only one node is
                                   needed                                                                             */

} LPBAM_UART_RxDataDesc_t;

/**
  * @brief LPBAM receive full descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[3U]; /*!< Specifies the content of nodes required for DMA queue execution : 3 different nodes
                                   are needed                                                                         */

  uint32_t pReg[2U];          /*!< Specifies the content of register to be updated : 2 different values are needed    */

} LPBAM_UART_RxFullDesc_t;
/**
  * @}
  */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_UART_Advanced_Exported_Functions LPBAM UART Advanced Exported Functions
  * @{
  */

/**
  * @brief ADV_LPBAM_UART_Transmit_SetConfigQ
  */
LPBAM_Status_t ADV_LPBAM_UART_Transmit_SetConfigQ(USART_TypeDef              *const pInstance,
                                                  LPBAM_DMAListInfo_t        const *const pDMAListInfo,
                                                  LPBAM_UART_ConfigAdvConf_t const *const pTxConfig,
                                                  LPBAM_UART_TxConfigDesc_t  *const pDescriptor,
                                                  DMA_QListTypeDef           *const pQueue);
/**
  * @brief ADV_LPBAM_UART_Transmit_SetDataQ
  */
LPBAM_Status_t ADV_LPBAM_UART_Transmit_SetDataQ(USART_TypeDef            *const pInstance,
                                                LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                                LPBAM_UART_DataAdvConf_t const *const pTxData,
                                                LPBAM_UART_TxDataDesc_t  *const pDescriptor,
                                                DMA_QListTypeDef         *const pQueue);
/**
  * @brief ADV_LPBAM_UART_Transmit_SetFullQ
  */
LPBAM_Status_t ADV_LPBAM_UART_Transmit_SetFullQ(USART_TypeDef            *const pInstance,
                                                LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                                LPBAM_UART_FullAdvConf_t const *const pTxFull,
                                                LPBAM_UART_TxFullDesc_t  *const pDescriptor,
                                                DMA_QListTypeDef         *const pQueue);
/**
  * @brief ADV_LPBAM_UART_Receive_SetConfigQ
  */
LPBAM_Status_t ADV_LPBAM_UART_Receive_SetConfigQ(USART_TypeDef              *const pInstance,
                                                 LPBAM_DMAListInfo_t        const *const pDMAListInfo,
                                                 LPBAM_UART_ConfigAdvConf_t const *const pRxConfig,
                                                 LPBAM_UART_RxConfigDesc_t  *const pDescriptor,
                                                 DMA_QListTypeDef           *const pQueue);
/**
  * @brief ADV_LPBAM_UART_Receive_SetDataQ
  */
LPBAM_Status_t ADV_LPBAM_UART_Receive_SetDataQ(USART_TypeDef            *const pInstance,
                                               LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                               LPBAM_UART_DataAdvConf_t const *const pRxData,
                                               LPBAM_UART_RxDataDesc_t  *const pDescriptor,
                                               DMA_QListTypeDef         *const pQueue);
/**
  * @brief ADV_LPBAM_UART_Receive_SetFullQ
  */
LPBAM_Status_t ADV_LPBAM_UART_Receive_SetFullQ(USART_TypeDef            *const pInstance,
                                               LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                               LPBAM_UART_FullAdvConf_t const *const pRxFull,
                                               LPBAM_UART_RxFullDesc_t  *const pDescriptor,
                                               DMA_QListTypeDef         *const pQueue);
/**
  * @brief ADV_LPBAM_UART_EnableDMARequests
  */
LPBAM_Status_t ADV_LPBAM_UART_EnableDMARequests(USART_TypeDef *const pInstance);
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

#endif /* STM32_ADV_LPBAM_UART_H */
