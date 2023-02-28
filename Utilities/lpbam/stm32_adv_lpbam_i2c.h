/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_i2c.h
  * @author  MCD Application Team
  * @brief   header for the stm32_adv_lpbam_i2c.c file
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
#ifndef STM32_ADV_LPBAM_I2C_H
#define STM32_ADV_LPBAM_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_platform_lpbam_i2c.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup I2C_Advanced I2C_Advanced
  * @brief    I2C Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_I2C_Advanced_Exported_Types LPBAM I2C Advanced Exported Types
  * @brief    LPBAM I2C Advanced Exported Types
  * @{
  */

/**
  * @brief LPBAM specific advanced I2C information structure definition.
  */
typedef struct
{
  LPBAM_I2C_AutonomousModeConf_t AutoModeConf; /*!< Specifies the master autonomous mode configuration.
                                                    Please refer to LPBAM_I2C_AutonomousModeConf_t definition in
                                                    stm32_platform_lpbam_i2c.h for more information.
                                                    This field is useless for slave device                       */

  uint32_t Timing;                             /*!< Specifies the I2C_TIMINGR_register value.
                                                    This parameter calculated by referring to I2C initialization
                                                    section in Reference manual                                  */

  uint32_t WakeupIT;                           /*!< Specifies the wake up source interrupt.
                                                    This parameter can be one or a combination of
                                                    @ref LPBAM_I2C_Wakeup_Interrupt                              */

} LPBAM_I2C_ConfigAdvConf_t;

/**
  * @brief LPBAM specific advanced I2C information structure definition.
  */
typedef struct
{
  LPBAM_I2C_AutonomousModeConf_t AutoModeConf; /*!< Specifies the master autonomous mode configuration.
                                                    Please refer to LPBAM_I2C_AutonomousModeConf_t definition in
                                                    stm32_platform_lpbam_i2c.h for more information.
                                                    This field is useless for slave device
                                                    Only TriggerState field is mandatory                              */

  uint32_t AddressingMode;                     /*!< Specifies if 7-bit or 10-bit addressing mode is selected.
                                                    This parameter can be a value of @ref LPBAM_I2C_Addressing_Mode   */

  uint32_t SequenceNumber;                     /*!< Specifies the number of frames to be transferred when sequential
                                                    transfer is activated.
                                                    This parameter shall be equal to 1 when transfer is not
                                                    sequential                                                        */

  uint8_t *pData;                              /*!< Specifies the transfer data buffer                                */

  uint16_t DevAddress;                         /*!< Specifies the target device address
                                                    This field is useless for slave device                            */

  uint16_t Size;                               /*!< Specifies the I2C data number of bytes to transfer                */

} LPBAM_I2C_DataAdvConf_t;

/**
  * @brief LPBAM specific advanced I2C full information structure definition.
  */
typedef struct
{
  LPBAM_I2C_AutonomousModeConf_t AutoModeConf; /*!< Specifies the autonomous mode configuration
                                                    Please refer to LPBAM_I2C_AutonomousModeConf_t definition in
                                                    stm32_platform_lpbam_i2c.h for more information.                 */

  uint32_t Timing;                             /*!< Specifies the I2C_TIMINGR_register value.
                                                    This parameter calculated by referring to I2C initialization
                                                    section in Reference manual                                      */

  uint32_t WakeupIT;                           /*!< Specifies the wake up source interrupt.
                                                    This parameter can be a value of @ref LPBAM_I2C_Wakeup_Interrupt */

  uint32_t AddressingMode;                     /*!< Specifies if 7-bit or 10-bit addressing mode is selected.
                                                    This parameter can be a value of @ref LPBAM_I2C_Addressing_Mode  */

  uint32_t SequenceNumber;                     /*!< Specifies the number of frames to be transferred when sequential
                                                    transfer is activated.
                                                    This parameter is equal to 1 when sequential transfer
                                                    is not activated.                                                */

  uint8_t *pData;                              /*!< Specifies the transfer data buffer.                              */

  uint16_t DevAddress;                         /*!< Specifies the target device address.
                                                    This field is useless for slave device                           */

  uint16_t Size;                               /*!< Specifies the I2C data number of bytes to transfer               */

} LPBAM_I2C_FullAdvConf_t;

/**
  * @brief LPBAM I2C master transmit configuration descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[4U]; /*!< Specifies the content of nodes required for I2C transfers : 4 different nodes are
                                   needed                                                                             */

  uint32_t pReg[4U];          /*!< Specifies the content of register to be updated : 4 different values are needed    */

} LPBAM_I2C_MasterTxConfigDesc_t;

/**
  * @brief LPBAM I2C master transmit data descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[2U]; /*!< Specifies the content of nodes required for I2C transfers : 2 different nodes are
                                   needed                                                                             */

  uint32_t pReg[1U];          /*!< Specifies the content of register to be updated : only one value is needed         */

} LPBAM_I2C_MasterTxDataDesc_t;

/**
  * @brief LPBAM I2C master transmit full descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[6U]; /*!< Specifies the content of nodes required for I2C transfers : 6 different nodes are
                                   needed                                                                             */

  uint32_t pReg[5U];          /*!< Specifies the content of register to be updated : 5 different values are needed    */

} LPBAM_I2C_MasterTxFullDesc_t;

/**
  * @brief LPBAM I2C slave transmit configuration descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[3U]; /*!< Specifies the content of nodes required for I2C transfers : 3 different nodes are
                                   needed                                                                             */

  uint32_t pReg[3U];          /*!< Specifies the content of register to be updated : 3 different values are needed    */

} LPBAM_I2C_SlaveTxConfigDesc_t;

/**
  * @brief LPBAM I2C slave transmit data descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[2U]; /*!< Specifies the content of nodes required for I2C transfers : 2 different nodes are
                                   needed                                                                             */

  uint32_t pReg[1U];          /*!< Specifies the content of register to be updated : only one value is needed         */

} LPBAM_I2C_SlaveTxDataDesc_t;

/**
  * @brief LPBAM I2C slave transmit full descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[5U]; /*!< Specifies the content of nodes required for I2C transfers : 5 different nodes are
                                   needed                                                                             */

  uint32_t pReg[4U];          /*!< Specifies the content of register to be updated : 4 different values are needed    */

} LPBAM_I2C_SlaveTxFullDesc_t;

/**
  * @brief LPBAM I2C master receive configuration descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[4U]; /*!< Specifies the content of nodes required for I2C transfers : 4 different nodes are
                                   needed                                                                             */

  uint32_t pReg[4U];          /*!< Specifies the content of register to be updated : 4 different values are needed    */

} LPBAM_I2C_MasterRxConfigDesc_t;

/**
  * @brief LPBAM I2C master receive data descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[2U]; /*!< Specifies the content of nodes required for I2C transfers : 2 different nodes are
                                   needed                                                                             */

  uint32_t pReg[1U];          /*!< Specifies the content of register to be updated : only one value is needed         */

} LPBAM_I2C_MasterRxDataDesc_t;

/**
  * @brief LPBAM I2C master receive full descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[6U]; /*!< Specifies the content of nodes required for I2C transfers : 6 different nodes are
                                   needed                                                                             */

  uint32_t pReg[5U];          /*!< Specifies the content of register to be updated : 5 different values are needed    */

} LPBAM_I2C_MasterRxFullDesc_t;

/**
  * @brief LPBAM I2C slave receive configuration descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[3U]; /*!< Specifies the content of nodes required for I2C transfers : 3 different nodes are
                                   needed                                                                             */

  uint32_t pReg[3U];          /*!< Specifies the content of register to be updated : 3 different values are needed    */

} LPBAM_I2C_SlaveRxConfigDesc_t;

/**
  * @brief LPBAM I2C slave receive data descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[2U]; /*!< Specifies the content of nodes required for I2C transfers : 2 different nodes are
                                   needed                                                                             */

  uint32_t pReg[1U];          /*!< Specifies the content of register to be updated : only one value is needed         */

} LPBAM_I2C_SlaveRxDataDesc_t;

/**
  * @brief LPBAM I2C slave receive full descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[5U]; /*!< Specifies the content of nodes required for I2C transfers : 5 different nodes are
                                   needed                                                                             */

  uint32_t pReg[4U];          /*!< Specifies the content of register to be updated : 4 different values are needed    */

} LPBAM_I2C_SlaveRxFullDesc_t;
/**
  * @}
  */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_I2C_Advanced_Exported_Functions LPBAM I2C Advanced Exported Functions
  * @brief      LPBAM I2C Advanced Exported Functions
  * @{
  */

/**
  * @brief ADV_LPBAM_I2C_MasterTx_SetConfigQ.
  */
LPBAM_Status_t ADV_LPBAM_I2C_MasterTx_SetConfigQ(I2C_TypeDef                    *const pInstance,
                                                 LPBAM_DMAListInfo_t            const *const pDMAListInfo,
                                                 LPBAM_I2C_ConfigAdvConf_t      const *const pTxConfig,
                                                 LPBAM_I2C_MasterTxConfigDesc_t *const pDescriptor,
                                                 DMA_QListTypeDef               *const pQueue);
/**
  * @brief ADV_LPBAM_I2C_MasterTx_SetDataQ.
  */
LPBAM_Status_t ADV_LPBAM_I2C_MasterTx_SetDataQ(I2C_TypeDef                  *const pInstance,
                                               LPBAM_DMAListInfo_t          const *const pDMAListInfo,
                                               LPBAM_I2C_DataAdvConf_t      const *const pTxData,
                                               LPBAM_I2C_MasterTxDataDesc_t *const pDescriptor,
                                               DMA_QListTypeDef             *const pQueue);
/**
  * @brief ADV_LPBAM_I2C_MasterTx_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_I2C_MasterTx_SetFullQ(I2C_TypeDef                  *const pInstance,
                                               LPBAM_DMAListInfo_t          const *const pDMAListInfo,
                                               LPBAM_I2C_FullAdvConf_t      const *const pTxFull,
                                               LPBAM_I2C_MasterTxFullDesc_t *const pDescriptor,
                                               DMA_QListTypeDef             *const pQueue);
/**
  * @brief ADV_LPBAM_I2C_SlaveTx_SetConfigQ.
  */
LPBAM_Status_t ADV_LPBAM_I2C_SlaveTx_SetConfigQ(I2C_TypeDef                   *const pInstance,
                                                LPBAM_DMAListInfo_t           const *const pDMAListInfo,
                                                LPBAM_I2C_ConfigAdvConf_t     const *const pTxConfig,
                                                LPBAM_I2C_SlaveTxConfigDesc_t *const pDescriptor,
                                                DMA_QListTypeDef              *const pQueue);
/**
  * @brief ADV_LPBAM_I2C_SlaveTx_SetDataQ.
  */
LPBAM_Status_t ADV_LPBAM_I2C_SlaveTx_SetDataQ(I2C_TypeDef                 *const pInstance,
                                              LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                              LPBAM_I2C_DataAdvConf_t     const *const pTxData,
                                              LPBAM_I2C_SlaveTxDataDesc_t *const pDescriptor,
                                              DMA_QListTypeDef            *const pQueue);
/**
  * @brief ADV_LPBAM_I2C_SlaveTx_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_I2C_SlaveTx_SetFullQ(I2C_TypeDef                 *const pInstance,
                                              LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                              LPBAM_I2C_FullAdvConf_t     const *const pTxFull,
                                              LPBAM_I2C_SlaveTxFullDesc_t *const pDescriptor,
                                              DMA_QListTypeDef            *const pQueue);
/**
  * @brief ADV_LPBAM_I2C_MasterRx_SetConfigQ.
  */
LPBAM_Status_t ADV_LPBAM_I2C_MasterRx_SetConfigQ(I2C_TypeDef                    *const pInstance,
                                                 LPBAM_DMAListInfo_t            const *const pDMAListInfo,
                                                 LPBAM_I2C_ConfigAdvConf_t      const *const pRxConfig,
                                                 LPBAM_I2C_MasterRxConfigDesc_t *const pDescriptor,
                                                 DMA_QListTypeDef               *const pQueue);
/**
  * @brief ADV_LPBAM_I2C_MasterRx_SetDataQ.
  */
LPBAM_Status_t ADV_LPBAM_I2C_MasterRx_SetDataQ(I2C_TypeDef                  *const pInstance,
                                               LPBAM_DMAListInfo_t          const *const pDMAListInfo,
                                               LPBAM_I2C_DataAdvConf_t      const *const pRxData,
                                               LPBAM_I2C_MasterRxDataDesc_t *const pDescriptor,
                                               DMA_QListTypeDef             *const pQueue);
/**
  * @brief ADV_LPBAM_I2C_MasterRx_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_I2C_MasterRx_SetFullQ(I2C_TypeDef                  *const pInstance,
                                               LPBAM_DMAListInfo_t          const *const pDMAListInfo,
                                               LPBAM_I2C_FullAdvConf_t      const *const pRxFull,
                                               LPBAM_I2C_MasterRxFullDesc_t *const pDescriptor,
                                               DMA_QListTypeDef             *const pQueue);
/**
  * @brief ADV_LPBAM_I2C_SlaveRx_SetConfigQ.
  */
LPBAM_Status_t ADV_LPBAM_I2C_SlaveRx_SetConfigQ(I2C_TypeDef                   *const pInstance,
                                                LPBAM_DMAListInfo_t           const *const pDMAListInfo,
                                                LPBAM_I2C_ConfigAdvConf_t     const *const pRxConfig,
                                                LPBAM_I2C_SlaveRxConfigDesc_t *const pDescriptor,
                                                DMA_QListTypeDef              *const pQueue);
/**
  * @brief ADV_LPBAM_I2C_SlaveRx_SetDataQ.
  */
LPBAM_Status_t ADV_LPBAM_I2C_SlaveRx_SetDataQ(I2C_TypeDef                 *const pInstance,
                                              LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                              LPBAM_I2C_DataAdvConf_t     const *const pRxData,
                                              LPBAM_I2C_SlaveRxDataDesc_t *const pDescriptor,
                                              DMA_QListTypeDef            *const pQueue);
/**
  * @brief ADV_LPBAM_I2C_SlaveRx_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_I2C_SlaveRx_SetFullQ(I2C_TypeDef                 *const pInstance,
                                              LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                              LPBAM_I2C_FullAdvConf_t     const *const pRxFull,
                                              LPBAM_I2C_SlaveRxFullDesc_t *const pDescriptor,
                                              DMA_QListTypeDef            *const pQueue);
/**
  * @brief ADV_LPBAM_I2C_EnableDMARequests.
  */
LPBAM_Status_t ADV_LPBAM_I2C_EnableDMARequests(I2C_TypeDef *const pInstance);

#if defined (HAL_I2C_MODULE_ENABLED)
/**
  * @brief ADV_LPBAM_I2C_RegisterISR.
  */
LPBAM_Status_t ADV_LPBAM_I2C_RegisterISR(I2C_HandleTypeDef *const hi2c);
#endif /* defined (HAL_I2C_MODULE_ENABLED) */
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

#endif /* STM32_ADV_LPBAM_I2C_H */
