/**
  **********************************************************************************************************************
  * @file    stm32_platform_lpbam_spi.h
  * @author  MCD Application Team
  * @brief   Header file provides the LPBAM SPI defines
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
#ifndef  STM32_PLATFORM_LPBAM_SPI_H
#define  STM32_PLATFORM_LPBAM_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup SPI_Platform
  * @brief      SPI Platform LPBAM module driver
  * @{
  */

/* Exported types ----------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_SPI_Platform_Exported_Types LPBAM SPI Platform Exported Types
  * @{
  */

/**
  * @brief  LPBAM SPI Autonomous Mode Configuration structure definition
  */
typedef struct
{
  uint32_t TriggerState;     /*!< Specifies the trigger state.
                                  This parameter can be a value of @ref LPBAM_SPI_AutonomousMode_State            */

  uint32_t TriggerSelection; /*!< Specifies the autonomous mode trigger signal selection.
                                  This parameter can be a value of @ref LPBAM_SPI_AutonomousMode_TriggerSelection */

  uint32_t TriggerPolarity;  /*!< Specifies the autonomous mode trigger signal polarity sensitivity.
                                  This parameter can be a value of @ref LPBAM_SPI_AutonomousMode_TriggerPolarity  */

} LPBAM_SPI_AutonomousModeConf_t;
/**
  * @}
  */


/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_SPI_Platform_Exported_Constants LPBAM SPI Platform Exported Constants
  * @{
  */

/** @defgroup LPBAM_SPI_ClockPolarity LPBAM SPI Clock Polarity
  * @{
  */
#define LPBAM_SPI_POLARITY_LOW  (0x00U) /*!< LPBAM SPI Clock Polarity Low  */
#define LPBAM_SPI_POLARITY_HIGH (0x01U) /*!< LPBAM SPI Clock Polarity High */
/**
  * @}
  */

/** @defgroup LPBAM_SPI_ClockPhase LPBAM SPI Clock Phase
  * @{
  */
#define LPBAM_SPI_PHASE_1EDGE (0x00U) /*!< LPBAM SPI Clock Phase First Edge  */
#define LPBAM_SPI_PHASE_2EDGE (0x01U) /*!< LPBAM SPI Clock Phase Second Edge */
/**
  * @}
  */

/** @defgroup LPBAM_SPI_FirstBit LPBAM SPI First Bit
  * @{
  */
#define LPBAM_SPI_FIRSTBIT_MSB (0x00U) /*!< LPBAM SPI First Bit MSB */
#define LPBAM_SPI_FIRSTBIT_LSB (0x01U) /*!< LPBAM SPI First Bit LSB */
/**
  * @}
  */

/** @defgroup LPBAM_SPI_BaudRate_Prescaler LPBAM SPI BaudRate Prescaler
  * @{
  */
#define LPBAM_SPI_BAUDRATEPRESCALER_BYPASS (0x08U) /*!< LPBAM SPI BaudRate Prescaler Bypassed */
#define LPBAM_SPI_BAUDRATEPRESCALER_2      (0x00U) /*!< LPBAM SPI BaudRate Prescaler 2        */
#define LPBAM_SPI_BAUDRATEPRESCALER_4      (0x01U) /*!< LPBAM SPI BaudRate Prescaler 4        */
#define LPBAM_SPI_BAUDRATEPRESCALER_8      (0x02U) /*!< LPBAM SPI BaudRate Prescaler 8        */
#define LPBAM_SPI_BAUDRATEPRESCALER_16     (0x03U) /*!< LPBAM SPI BaudRate Prescaler 16       */
#define LPBAM_SPI_BAUDRATEPRESCALER_32     (0x04U) /*!< LPBAM SPI BaudRate Prescaler 32       */
#define LPBAM_SPI_BAUDRATEPRESCALER_64     (0x05U) /*!< LPBAM SPI BaudRate Prescaler 64       */
#define LPBAM_SPI_BAUDRATEPRESCALER_128    (0x06U) /*!< LPBAM SPI BaudRate Prescaler 128      */
#define LPBAM_SPI_BAUDRATEPRESCALER_256    (0x07U) /*!< LPBAM SPI BaudRate Prescaler 256      */
/**
  * @}
  */

/** @defgroup LPBAM_SPI_Data_Size LPBAM SPI Data Size
  * @{
  */
#define LPBAM_SPI_DATASIZE_8BIT  (0x07U) /*!< LPBAM SPI Data Size 8bit  */
#define LPBAM_SPI_DATASIZE_16BIT (0x0FU) /*!< LPBAM SPI Data Size 16bit */
/**
  * @}
  */

/** @defgroup LPBAM_SPI_AutonomousMode_State LPBAM SPI Autonomous Mode State
  * @{
  */
#define LPBAM_SPI_AUTO_MODE_DISABLE (0x00U) /*!< LPBAM SPI Trigger Disable */
#define LPBAM_SPI_AUTO_MODE_ENABLE  (0x01U) /*!< LPBAM SPI Trigger Enable  */
/**
  * @}
  */

/** @defgroup LPBAM_SPI_AutonomousMode_TriggerSelection LPBAM SPI Autonomous Mode Trigger Selection
  * @{
  */
#define LPBAM_SPI_GRP2_LPDMA_CH0_TCF_TRG (0x00U) /*!< LPBAM SPI HW Trigger signal is LPDMA_CH0_TRG  */
#define LPBAM_SPI_GRP2_LPDMA_CH1_TCF_TRG (0x01U) /*!< LPBAM SPI HW Trigger signal is LPDMA_CH1_TRG  */
#define LPBAM_SPI_GRP2_LPDMA_CH2_TCF_TRG (0x02U) /*!< LPBAM SPI HW Trigger signal is LPDMA_CH2_TRG  */
#define LPBAM_SPI_GRP2_LPDMA_CH3_TCF_TRG (0x03U) /*!< LPBAM SPI HW Trigger signal is LPDMA_CH3_TRG  */
#define LPBAM_SPI_GRP2_EXTI4_TRG         (0x04U) /*!< LPBAM SPI HW Trigger signal is EXTI4_TRG      */
#define LPBAM_SPI_GRP2_EXTI8_TRG         (0x05U) /*!< LPBAM SPI HW Trigger signal is EXTI8_TRG      */
#define LPBAM_SPI_GRP2_LPTIM1_CH1_TRG    (0x06U) /*!< LPBAM SPI HW Trigger signal is LPTIM1_CH1_TRG */
#define LPBAM_SPI_GRP2_LPTIM3_CH1_TRG    (0x07U) /*!< LPBAM SPI HW Trigger signal is LPTIM3_CH1_TRG */
#define LPBAM_SPI_GRP2_COMP1_TRG         (0x08U) /*!< LPBAM SPI HW Trigger signal is COMP1_TRG      */
#if defined(COMP2)
#define LPBAM_SPI_GRP2_COMP2_TRG         (0x09U) /*!< LPBAM SPI HW Trigger signal is COMP2_TRG      */
#endif /* defined(COMP2) */
#define LPBAM_SPI_GRP2_RTC_ALRA_TRG      (0x0AU) /*!< LPBAM SPI HW Trigger signal is RTC_ALRA_TRG   */
#define LPBAM_SPI_GRP2_RTC_WUT_TRG       (0x0BU) /*!< LPBAM SPI HW Trigger signal is RTC_WUT_TRG    */
/**
  * @}
  */

/** @defgroup LPBAM_SPI_AutonomousMode_TriggerPolarity LPBAM SPI Autonomous Mode Trigger Polarity
  * @{
  */
#define LPBAM_SPI_TRIG_POLARITY_RISING  (0x00U) /*!< LPBAM SPI HW Trigger signal on rising edge  */
#define LPBAM_SPI_TRIG_POLARITY_FALLING (0x01U) /*!< LPBAM SPI HW Trigger signal on falling edge */
/**
  * @}
  */

/** @defgroup LPBAM_SPI_Wakeup_Interrupt LPBAM SPI Wakeup Interrupt
  * @{
  */
#define LPBAM_SPI_IT_NONE (0x00U) /*!< LPBAM SPI wake up interrupt none                      */
#define LPBAM_SPI_IT_RXP  (0x01U) /*!< LPBAM SPI Rx Packet available wake up interrupt       */
#define LPBAM_SPI_IT_TXP  (0x02U) /*!< LPBAM SPI Tx Packet space available wake up interrupt */
#define LPBAM_SPI_IT_DXP  (0x04U) /*!< LPBAM SPI Duplex Packet wake up interrupt             */
#define LPBAM_SPI_IT_EOT  (0x08U) /*!< LPBAM SPI End of transfer wake up interrupt           */
/**
  * @}
  */

/** @defgroup LPBAM_SPI_CommunicationMode LPBAM SPI Communication Mode
  * @{
  */
#define LPBAM_SPI_DIRECTION_2LINES        (0x00U) /*!< LPBAM SPI Communication Full Duplex      */
#define LPBAM_SPI_DIRECTION_2LINES_TXONLY (0x01U) /*!< LPBAM SPI Communication Simplex Transmit */
#define LPBAM_SPI_DIRECTION_2LINES_RXONLY (0x02U) /*!< LPBAM SPI Communication Simplex Receive  */
/**
  * @}
  */

/**
  * @}
  */


/* Private constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_SPI_Platform_Private_Constants LPBAM SPI Platform Private Constants
  * @{
  */
#define LPBAM_SPI_TXDMAEN SPI_CFG1_TXDMAEN /*!< SPI Tx DMA transfer enable */
#define LPBAM_SPI_RXDMAEN SPI_CFG1_RXDMAEN /*!< SPI Rx DMA transfer enable */
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

#endif /* STM32_PLATFORM_LPBAM_SPI_H */
