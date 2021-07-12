/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_spi.h
  * @author  MCD Application Team
  * @brief   header for the stm32_lpbam_spi.c file
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
#ifndef STM32_LPBAM_SPI_H
#define STM32_LPBAM_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_platform_lpbam_spi.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup SPI_Basic
  * @brief      SPI LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_SPI_Basic_Exported_Types LPBAM SPI Basic Exported Types
  * @brief    LPBAM SPI Basic Exported Types
  * @{
  */

/**
  * @brief LPBAM SPI Configuration Structure definition.
  */
typedef struct
{
  LPBAM_FunctionalState State;                 /*!< Specifies the State of SPI instance.
                                                    This parameter can be ENABLE or DISABLE                          */

  uint8_t Direction;                           /*!< Specifies the Direction of SPI instance.
                                                    This parameter can be a value of @ref LPBAM_SPI_CommunicationMode */

  uint32_t CLKPolarity;                        /*!< Specifies the serial clock steady state.
                                                    This parameter can be a value of @ref LPBAM_SPI_ClockPolarity     */

  uint32_t CLKPhase;                           /*!< Specifies the clock active edge for the bit capture.
                                                    This parameter can be a value of @ref LPBAM_SPI_ClockPhase        */

  uint32_t FirstBit;                           /*!< Specifies whether data transfers start from MSB or LSB bit.
                                                    This parameter can be a value of @ref LPBAM_SPI_FirstBit          */

  uint32_t BaudRatePrescaler;                  /*!< Specifies the Baud Rate prescaler value which will be used to
                                                    configure the transmit and receive SCK clock.
                                                    This parameter can be a value of @ref LPBAM_SPI_BaudRate_Prescaler
                                                    @note The communication clock is derived from the master clock.
                                                    The slave clock does not need to be set.                          */

  uint32_t DataSize;                           /*!< Specifies the SPI data size.
                                                    This parameter can be a value of @ref LPBAM_SPI_Data_Size         */

  LPBAM_SPI_AutonomousModeConf_t AutoModeConf; /*!< Specifies the autonomous mode configuration.
                                                    Please refer to LPBAM_SPI_AutonomousModeConf_t definition in
                                                    stm32_platform_lpbam_spi.h for more information.                  */

  uint32_t WakeupIT;                           /*!< Specifies the wake up source interrupt.
                                                    This parameter can be a value of @ref LPBAM_SPI_Wakeup_Interrupt  */

  uint16_t Size;                               /*!< Specifies the SPI number of data to transfer.
                                                    This parameter can be a value between 1 and 0x400                 */

} LPBAM_SPI_Config_t;

/**
  * @brief LPBAM SPI Configuration Node Structure definition.
  */
typedef struct
{
  LPBAM_Desc_t       NodeDesc;   /*!< Defines the node descriptor fields */

  SPI_TypeDef        *pInstance; /*!< Defines the SPI instance           */

  LPBAM_SPI_Config_t Config;     /*!< Defines the SPI user configuration */

} LPBAM_SPI_ConfNode_t;

/**
  * @brief LPBAM SPI Configuration Node ID Enumeration definition.
  */
typedef enum
{
  LPBAM_SPI_STATE_ID,         /*!< LPBAM SPI State node ID                       */

  LPBAM_SPI_DATASIZE_ID,      /*!< LPBAM SPI Data Size node ID                   */

  LPBAM_SPI_CONFIG_FRAME_ID,  /*!< LPBAM SPI configuration Frame node ID         */

  LPBAM_SPI_CONFIG_COM_ID,    /*!< LPBAM SPI configuration Communication node ID */

  LPBAM_SPI_TRIG_ID,          /*!< LPBAM SPI Trigger node ID                     */

  LPBAM_SPI_WAKEUP_IT_ID,     /*!< LPBAM SPI Wakeup Interrupt ID                 */

  LPBAM_SPI_START_ID,         /*!< LPBAM SPI Start node ID                       */

  LPBAM_SPI_TRANSMIT_DATA_ID, /*!< LPBAM SPI Transmit Data Transfer node ID      */

  LPBAM_SPI_RECEIVE_DATA_ID,  /*!< LPBAM SPI Receive Data Transfer node ID       */

  LPBAM_SPI_CLEARFLAG_ID      /*!< LPBAM SPI Clear Flag node ID                  */

} LPBAM_SPI_ID_t;
/**
  * @}
  */

/* Include LPBAM SPI Advanced module */
#include "stm32_adv_lpbam_spi.h"

/** @defgroup LPBAM_SPI_Basic_Exported_Functions LPBAM SPI Basic Exported Functions
  * @{
  */

/**
  * @brief LPBAM_SPI_FillNodeConfig.
  */
LPBAM_Status_t LPBAM_SPI_FillNodeConfig(LPBAM_SPI_ConfNode_t const *const pConfNode,
                                        DMA_NodeConfTypeDef  *const pDMANodeConfig);
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

#endif /* STM32_LPBAM_SPI_H */
