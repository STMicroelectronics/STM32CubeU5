/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_i2c.h
  * @author  MCD Application Team
  * @brief   header for the stm32_lpbam_i2c.c file
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
#ifndef STM32_LPBAM_I2C_H
#define STM32_LPBAM_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_platform_lpbam_i2c.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup I2C_Basic
  * @brief      I2C LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_I2C_Basic_Exported_Types LPBAM I2C Basic Exported Types
  * @brief    LPBAM I2C Basic Exported Types
  * @{
  */

/**
  * @brief LPBAM I2C Configuration Structure definition.
  */
typedef struct
{
  uint32_t AddressingMode;                      /*!< Specifies the I2C addressing mode.
                                                     This parameter can be a value of @ref LPBAM_I2C_Addressing_Mode  */

  uint32_t Timing;                              /*!< Specifies the I2C timing values.                                 */

  uint32_t WakeupIT;                            /*!< Specifies the wake up source interrupt
                                                     This parameter can be a value of @ref LPBAM_I2C_Wakeup_Interrupt */

  LPBAM_I2C_AutonomousModeConf_t AutoModeConf;  /*!< Specifies the master autonomous mode configuration.
                                                     Please refer to LPBAM_I2C_AutonomousModeConf_t definition in
                                                     stm32_platform_lpbam_i2c.h for more information.
                                                     This field is useless for slave device                           */

  uint16_t DevAddress;                          /*!< Specifies the I2C slave address.
                                                     This parameter can be a value between 0 and 0x3FF                */

  uint16_t Size;                                /*!< Specifies the I2C data number of bytes to transfer               */

  LPBAM_FunctionalState State;                  /*!< Specifies the I2C enable state.
                                                     This parameter can be ENABLE or DISABLE                          */

  LPBAM_FunctionalState ReloadDataState;        /*!< Specifies the I2C reload data transfer state.
                                                     This parameter can be ENABLE or DISABLE                          */

  LPBAM_FunctionalState SequentialDataState;    /*!< Specifies the I2C sequential data transfer state.
                                                     This parameter can be ENABLE or DISABLE                          */

  uint8_t Mode;                                 /*!< Specifies the I2C mode.
                                                     This parameter can be a value of @ref LPBAM_I2C_Mode             */

  uint8_t IsFirstReloadData;                    /*!< Specifies whether the current data to transfer is the first reload
                                                     data or not.
                                                     This parameter can be a value of
                                                     @ref LPBAM_I2C_First_Reload_Data                                 */

  uint8_t IsLastReloadData;                     /*!< Specifies whether the current data to transfer is the last reload
                                                     data or not.
                                                     This parameter can be a value of @ref LPBAM_I2C_Last_Reload_Data */

  uint8_t IsFirstSequentialData;                /*!< Specifies whether the current data to transfer is the first
                                                     sequential data or not.
                                                     This parameter can be a value of
                                                     @ref LPBAM_I2C_First_Sequential_Data                             */

  uint8_t IsLastSequentialData;                 /*!< Specifies whether the current data to transfer is the first
                                                     sequential data or not.
                                                     This parameter can be a value of
                                                     @ref LPBAM_I2C_Last_Sequential_Data                              */
} LPBAM_I2C_Config_t;

/**
  * @brief LPBAM I2C Configuration Node Structure definition.
  */
typedef struct
{
  LPBAM_Desc_t       NodeDesc;   /*!< Defines the node descriptor fields */

  I2C_TypeDef        *pInstance; /*!< Defines the I2C instance           */

  LPBAM_I2C_Config_t Config;     /*!< Defines the I2C user configuration */

} LPBAM_I2C_ConfNode_t;

/**
  * @brief LPBAM I2C Configuration Node ID Enumeration definition.
  *
  */
typedef enum
{
  LPBAM_I2C_STATE_ID,              /*!< LPBAM I2C State node ID                  */

  LPBAM_I2C_CONFIG_TRANSACTION_ID, /*!< LPBAM I2C configuration transfer node ID */

  LPBAM_I2C_TIMING_ID,             /*!< LPBAM I2C  timing node ID                */

  LPBAM_I2C_TRIG_ID,               /*!< LPBAM I2C  trigger node ID               */

  LPBAM_I2C_TRANSMIT_DATA_ID,      /*!< LPBAM I2C transmit data node ID          */

  LPBAM_I2C_RECEIVE_DATA_ID,       /*!< LPBAM I2C receive data node ID           */

} LPBAM_I2C_ID_t;
/**
  * @}
  */

/* Include LPBAM I2C Advanced module */
#include "stm32_adv_lpbam_i2c.h"

/** @addtogroup LPBAM_I2C_Basic_Exported_Functions LPBAM I2C Basic Exported Functions
  * @{
  */

/**
  * @brief LPBAM_I2C_FillNodeConfig.
  */
LPBAM_Status_t LPBAM_I2C_FillNodeConfig(LPBAM_I2C_ConfNode_t const *const pConfNode,
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


#endif /* STM32_LPBAM_I2C_H */
