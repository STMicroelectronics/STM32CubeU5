/**
  **********************************************************************************************************************
  * @file    stm32_platform_lpbam_uart.h
  * @author  MCD Application Team
  * @brief   Header file provides the LPBAM UART defines
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
#ifndef  STM32_PLATFORM_LPBAM_UART_H
#define  STM32_PLATFORM_LPBAM_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup UART_Platform
  * @brief      UART Platform LPBAM module driver
  * @{
  */

/* Exported types ----------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_UART_Platform_Exported_Types LPBAM UART Platform Exported Types
  * @{
  */

/**
  * @brief LPBAM UART Autonomous Mode Configuration structure definition
  */
typedef struct
{
  uint32_t AutonomousModeState; /*!< Specifies the autonomous mode state.
                                     This parameter can be a value of @ref LPBAM_UART_AutonomousMode_State            */

  uint32_t TriggerSelection;    /*!< Specifies the autonomous mode trigger signal selection.
                                     This parameter can be a value of @ref LPBAM_LPUART_AutonomousMode_TriggerSelection
                                                                                                                      */
  uint32_t TriggerPolarity;     /*!< Specifies the autonomous mode trigger signal polarity.
                                     This parameter can be a value of @ref LPBAM_UART_AutonomousMode_TriggerPolarity  */

  uint32_t DataSize;            /*!< Specifies the UART data number of bytes to transfer in each packet               */

} LPBAM_UART_AutonomousModeConf_t;
/**
  * @}
  */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_UART_Platform_Exported_Constants LPBAM UART Platform Exported Constants
  * @{
  */

/** @defgroup LPBAM_UART_Mode LPBAM UART Mode
  * @{
  */
#define LPBAM_UART_MODE_TX    (0x00U) /*!< LPBAM UART Direction Transmit         */
#define LPBAM_UART_MODE_RX    (0x01U) /*!< LPBAM UART Direction Receive          */
#define LPBAM_UART_MODE_TX_RX (0x02U) /*!< LPBAM UART Direction Transmit Receive */
/**
  * @}
  */

/** @defgroup LPBAM_UART_Wakeup_Interrupt LPBAM UART Wakeup Interrupt
  * @{
  */
#define LPBAM_UART_IT_NONE  (0x00U) /*!< LPBAM UART wake up interrupt none             */
#define LPBAM_UART_IT_IDLE  (0x01U) /*!< LPBAM UART idle interrupt enable              */
#define LPBAM_UART_IT_RXFNE (0x02U) /*!< LPBAM UART RX FIFO not empty interrupt enable */
#define LPBAM_UART_IT_TC    (0x04U) /*!< LPBAM UART transfer complete interrupt enable */
#define LPBAM_UART_IT_TXFNF (0x08U) /*!< LPBAM UART TX FIFO not full interrupt enable  */
#define LPBAM_UART_IT_TXFE  (0x10U) /*!< LPBAM UART TX FIFO empty interrupt enable     */
#define LPBAM_UART_IT_RXFF  (0x20U) /*!< LPBAM UART RX FIFO full interrupt enable      */
/**
  * @}
  */

/** @defgroup LPBAM_UART_AutonomousMode_State LPBAM UART Autonomous Mode State
  * @{
  */
#define LPBAM_UART_AUTONOMOUS_MODE_DISABLE (0x00U) /*!< LPBAM UART Autonomous mode disable */
#define LPBAM_UART_AUTONOMOUS_MODE_ENABLE  (0x01U) /*!< LPBAM UART Autonomous mode enable  */
/**
  * @}
  */

/** @defgroup LPBAM_UART_AutonomousMode_TriggerPolarity LPBAM UART Autonomous Mode Trigger Polarity
  * @{
  */
#define LPBAM_UART_TRIG_POLARITY_RISING  (0x00U) /*!< LPBAM UART HW Trigger signal on rising edge  */
#define LPBAM_UART_TRIG_POLARITY_FALLING (0x01U) /*!< LPBAM UART HW Trigger signal on falling edge */
/**
  * @}
  */

/** @defgroup LPBAM_LPUART_AutonomousMode_TriggerSelection LPBAM LPUART Autonomous Mode Trigger Selection
  * @{
  */
#define LPBAM_LPUART_LPDMA1_CH0_TCF_TRG (0x00U) /*!< LPBAM LPUART HW Trigger signal is LPDMA1 channel 0 */
#define LPBAM_LPUART_LPDMA1_CH1_TCF_TRG (0x01U) /*!< LPBAM LPUART HW Trigger signal is LPDMA1 channel 1 */
#define LPBAM_LPUART_LPDMA1_CH2_TCF_TRG (0x02U) /*!< LPBAM LPUART HW Trigger signal is LPDMA1 channel 2 */
#define LPBAM_LPUART_LPDMA1_CH3_TCF_TRG (0x03U) /*!< LPBAM LPUART HW Trigger signal is LPDMA1 channel 3 */
#define LPBAM_LPUART_EXTI_LINE6_TRG     (0x04U) /*!< LPBAM LPUART HW Trigger signal is EXTI line 6      */
#define LPBAM_LPUART_EXTI_LINE8_TRG     (0x05U) /*!< LPBAM LPUART HW Trigger signal is EXTI line 8      */
#define LPBAM_LPUART_LPTIM1_OUT_TRG     (0x06U) /*!< LPBAM LPUART HW Trigger signal is LPTIM1 out       */
#define LPBAM_LPUART_LPTIM3_OUT_TRG     (0x07U) /*!< LPBAM LPUART HW Trigger signal is LPTIM3 out       */
#define LPBAM_LPUART_COMP1_OUT_TRG      (0x08U) /*!< LPBAM LPUART HW Trigger signal is COMP1 out        */
#if defined(COMP2)
#define LPBAM_LPUART_COMP2_OUT_TRG      (0x09U) /*!< LPBAM LPUART HW Trigger signal is COMP2 out        */
#endif /* defined(COMP2) */
#define LPBAM_LPUART_RTC_ALRA_TRG       (0x0AU) /*!< LPBAM LPUART HW Trigger signal is RTC alarmA       */
#define LPBAM_LPUART_RTC_WUT_TRG        (0x0BU) /*!< LPBAM LPUART HW Trigger signal is RTC wakeup       */
/**
  * @}
  */

/**
  * @}
  */

/* Private constants -------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_UART_Platform_Private_Constants LPBAM UART Platform Private Constants
  * @{
  */
#define LPBAM_UART_DMAT USART_CR3_DMAT /*!< LPBAM UART enable Tx DMA request */
#define LPBAM_UART_DMAR USART_CR3_DMAR /*!< LPBAM UART enable Rx DMA request */
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

#endif /* STM32_PLATFORM_LPBAM_UART_H */
