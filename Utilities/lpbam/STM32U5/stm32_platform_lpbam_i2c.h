/**
  **********************************************************************************************************************
  * @file    stm32_platform_lpbam_i2c.h
  * @author  MCD Application Team
  * @brief   Header file provides the LPBAM I2C defines
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
#ifndef  STM32_PLATFORM_LPBAM_I2C_H
#define  STM32_PLATFORM_LPBAM_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup I2C_Platform
  * @brief      I2C Platform LPBAM module driver
  * @{
  */

/* Exported types ----------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_I2C_Platform_Exported_Types LPBAM I2C Platform Exported Types
  * @{
  */

/**
  * @brief  LPBAM I2C Autonomous Mode Configuration structure definition
  */
typedef struct
{
  uint32_t TriggerState;     /*!< Specifies the trigger state.
                                  This parameter can be a value of @ref LPBAM_I2CEx_AutonomousMode_FunctionalState  */

  uint32_t TriggerSelection; /*!< Specifies the autonomous mode trigger signal selection.
                                  This parameter can be a value of @ref LPBAM_I2CEx_AutonomousMode_TriggerSelection */

  uint32_t TriggerPolarity;  /*!< Specifies the autonomous mode trigger signal polarity sensitivity.
                                  This parameter can be a value of @ref LPBAM_I2CEx_AutonomousMode_TriggerPolarity  */

} LPBAM_I2C_AutonomousModeConf_t;
/**
  * @}
  */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_I2C_Platform_Exported_Constants LPBAM I2C Platform Exported Constants
  * @{
  */

/** @defgroup LPBAM_I2CEx_AutonomousMode_FunctionalState LPBAM I2C Autonomous Mode State
  * @{
  */
#define LPBAM_I2C_AUTO_MODE_DISABLE (0x00U) /*!< LPBAM I2C Autonomous Mode disable */
#define LPBAM_I2C_AUTO_MODE_ENABLE  (0x01U) /*!< LPBAM I2C Autonomous Mode enable  */
/**
  * @}
  */

/** @defgroup LPBAM_I2CEx_AutonomousMode_TriggerSelection LPBAM I2C Autonomous Mode Trigger Selection
  * @{
  */
#define LPBAM_I2C_GRP2_LPDMA_CH0_TCF_TRG (0x00U) /*!< LPBAM I2C HW Trigger signal is LPDMA_CH0_TRG  */
#define LPBAM_I2C_GRP2_LPDMA_CH1_TCF_TRG (0x01U) /*!< LPBAM I2C HW Trigger signal is LPDMA_CH1_TRG  */
#define LPBAM_I2C_GRP2_LPDMA_CH2_TCF_TRG (0x02U) /*!< LPBAM I2C HW Trigger signal is LPDMA_CH2_TRG  */
#define LPBAM_I2C_GRP2_LPDMA_CH3_TCF_TRG (0x03U) /*!< LPBAM I2C HW Trigger signal is LPDMA_CH3_TRG  */
#define LPBAM_I2C_GRP2_EXTI5_TRG         (0x04U) /*!< LPBAM I2C HW Trigger signal is EXTI5_TRG      */
#define LPBAM_I2C_GRP2_EXTI8_TRG         (0x05U) /*!< LPBAM I2C HW Trigger signal is EXTI8_TRG      */
#define LPBAM_I2C_GRP2_LPTIM1_CH1_TRG    (0x06U) /*!< LPBAM I2C HW Trigger signal is LPTIM1_CH1_TRG */
#define LPBAM_I2C_GRP2_LPTIM3_CH1_TRG    (0x07U) /*!< LPBAM I2C HW Trigger signal is LPTIM3_CH1_TRG */
#define LPBAM_I2C_GRP2_COMP1_TRG         (0x08U) /*!< LPBAM I2C HW Trigger signal is COMP1_TRG      */
#if defined(COMP2)
#define LPBAM_I2C_GRP2_COMP2_TRG         (0x09U) /*!< LPBAM I2C HW Trigger signal is COMP2_TRG      */
#endif /* defined(COMP2) */
#define LPBAM_I2C_GRP2_RTC_ALRA_TRG      (0x0AU) /*!< LPBAM I2C HW Trigger signal is RTC_ALRA_TRG   */
#define LPBAM_I2C_GRP2_RTC_WUT_TRG       (0x0BU) /*!< LPBAM I2C HW Trigger signal is RTC_WUT_TRG    */
/**
  * @}
  */

/** @defgroup LPBAM_I2CEx_AutonomousMode_TriggerPolarity LPBAM I2C Autonomous Mode Trigger Polarity
  * @{
  */
#define LPBAM_I2C_TRIG_POLARITY_RISING  (0x00U) /*!< LPBAM I2C Trigger Signal on Rising Edge  */
#define LPBAM_I2C_TRIG_POLARITY_FALLING (0x01U) /*!< LPBAM I2C Trigger Signal on Falling Edge */
/**
  * @}
  */

/** @defgroup LPBAM_I2C_Wakeup_Interrupt LPBAM I2C Wakeup Interrupt
  * @{
  */
#define LPBAM_I2C_IT_NONE  (0x00U)    /*!< LPBAM I2C wake up interrupt none               */
#define LPBAM_I2C_IT_STOP  (0x20U)    /*!< LPBAM I2C Stop detection interrupt enable      */
#define LPBAM_I2C_IT_TC    (0x40U)    /*!< LPBAM I2C transfer complete interrupt enable   */
#define LPBAM_I2C_IT_RXI   (0x04U)    /*!< LPBAM I2C Rx buffer not empty interrupt enable */
#define LPBAM_I2C_IT_TXI   (0x02U)    /*!< LPBAM I2C Tx buffer status interrupt enable    */
#define LPBAM_I2C_IT_ADDRI (0x08U)    /*!< LPBAM I2C Address match interrupt enable       */
#define LPBAM_I2C_IT_NACKI (0x10U)    /*!< LPBAM I2C NACK received interrupt enable       */
/**
  * @}
  */

/** @defgroup LPBAM_I2C_Addressing_Mode LPBAM I2C Addressing Mode
  * @{
  */
#define LPBAM_I2C_ADDRESSINGMODE_7BIT  (0x00U) /*!< LPBAM I2C Addressing Mode 7 bit  */
#define LPBAM_I2C_ADDRESSINGMODE_10BIT (0x01U) /*!< LPBAM I2C Addressing Mode 10 bit */
/**
  * @}
  */

/** @defgroup LPBAM_I2C_Mode LPBAM I2C Mode
  * @{
  */
#define LPBAM_I2C_MASTER_TRANSMIT_MODE (0x01U) /*!< LPBAM I2C master transmit mode */
#define LPBAM_I2C_MASTER_RECEIVE_MODE  (0x02U) /*!< LPBAM I2C master receive mode  */
#define LPBAM_I2C_SLAVE_TRANSMIT_MODE  (0x04U) /*!< LPBAM I2C slave transmit mode  */
#define LPBAM_I2C_SLAVE_RECEIVE_MODE   (0x08U) /*!< LPBAM I2C slave receive mode   */
/**
  * @}
  */

/** @defgroup LPBAM_I2C_First_Reload_Data LPBAM I2C First Reload Data
  * @{
  */
#define LPBAM_I2C_IS_FIRST_RELOADED_DATA     (0x00U) /*!< LPBAM I2C is first reloaded data     */
#define LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA (0x01U) /*!< LPBAM I2C is not first reloaded data */
/**
  * @}
  */

/** @defgroup LPBAM_I2C_Last_Reload_Data LPBAM I2C Last Reload Data
  * @{
  */
#define LPBAM_I2C_IS_LAST_RELOADED_DATA     (0x00U) /*!< LPBAM I2C is last reloaded data     */
#define LPBAM_I2C_IS_NOT_LAST_RELOADED_DATA (0x01U) /*!< LPBAM I2C is not last reloaded data */
/**
  * @}
  */

/** @defgroup LPBAM_I2C_First_Sequential_Data LPBAM I2C First Sequential Data
  * @{
  */
#define LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA     (0x00U) /*!< LPBAM I2C is first sequential data     */
#define LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA (0x01U) /*!< LPBAM I2C is not first sequential data */
/**
  * @}
  */

/** @defgroup LPBAM_I2C_Last_Sequential_Data LPBAM I2C Last Sequential Data
  * @{
  */
#define LPBAM_I2C_IS_LAST_SEQUENTIAL_DATA     (0x00U) /*!< LPBAM I2C is last sequential data     */
#define LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA (0x01U) /*!< LPBAM I2C is not last sequential data */
/**
  * @}
  */

/** @defgroup LPBAM_I2C_Advanced_MaxData_Size LPBAM I2C Advanced Max Data Size
  * @{
  */
#define LPBAM_I2C_MAX_DATA_SIZE (0xFFU) /*!< LPBAM I2C Max Data Size */
/**
  * @}
  */

/**
  * @}
  */

/* Private constants -------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_I2C_Platform_Private_Constants LPBAM I2C Platform Private Constants
  * @{
  */
#define I2C_SLAVE_DATA_NODEIDX  (0x03U)             /*!< LPBAM I2C Slave data node index                                */
#define I2C_MASTER_DATA_NODEIDX (0x04U)             /*!< LPBAM I2C master data node index                               */
#define LPBAM_I2C_TCDMAEN       I2C_AUTOCR_TCDMAEN  /*!< LPBAM I2C DMA request enable on transfer complete event        */
#define LPBAM_I2C_TCRDMAEN      I2C_AUTOCR_TCRDMAEN /*!< LPBAM I2C DMA request enable on transfer complete reload event */
#define LPBAM_I2C_RXDMAEN       I2C_CR1_RXDMAEN     /*!< LPBAM I2C Rx DMA request enable                                */
#define LPBAM_I2C_TXDMAEN       I2C_CR1_TXDMAEN     /*!< LPBAM I2C Tx DMA request enable                                */
#define LPBAM_I2C_STOPFACLR     I2C_CR1_STOPFACLR   /*!< LPBAM I2C STOP flag auto clear enable                          */
#define LPBAM_I2C_ADDRACLR      I2C_CR1_ADDRACLR    /*!< LPBAM I2C address match auto clear enable enable               */
#define LPBAM_I2C_WUPEN         I2C_CR1_WUPEN       /*!< LPBAM I2C Wakeup from STOP enable                              */
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

#endif /* STM32_PLATFORM_LPBAM_I2C_H */
