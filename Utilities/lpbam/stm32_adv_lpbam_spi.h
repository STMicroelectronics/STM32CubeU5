/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_spi.h
  * @author  MCD Application Team
  * @brief   header for the stm32_adv_lpbam_spi.c file
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
#ifndef STM32_ADV_LPBAM_SPI_H
#define STM32_ADV_LPBAM_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_platform_lpbam_spi.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup SPI_Advanced SPI_Advanced
  * @brief    SPI Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_SPI_Advanced_Exported_Types LPBAM SPI Advanced Exported Types
  * @brief    Advanced LPBAM SPI Exported Types
  * @{
  */

/**
  * @brief LPBAM specific advanced SPI information structure definition.
  */
typedef struct
{
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
                                                    This parameter can be one or a combination of
                                                    @ref LPBAM_SPI_Wakeup_Interrupt                                   */

} LPBAM_SPI_ConfigAdvConf_t;

/**
  * @brief LPBAM specific advanced SPI information structure definition.
  */
typedef struct
{
  LPBAM_SPI_AutonomousModeConf_t AutoModeConf; /*!< Specifies only the autonomous mode trigger state
                                                    Only TriggerState field is mandatory.
                                                    Please refer to LPBAM_SPI_AutonomousModeConf_t definition in
                                                    stm32_platform_lpbam_spi.h for more information.                  */

  uint32_t DataSize;                           /*!< Specifies the SPI data size.
                                                    This parameter can be a value of @ref LPBAM_SPI_Data_Size         */

  uint16_t Size;                               /*!< Specifies the element number.
                                                    This parameter can be a value between 1 and 0x400                 */

  uint8_t *pTxData;                            /*!< Specifies the Tx transfer data buffer address.                    */

  uint8_t *pRxData;                            /*!< Specifies the Rx transfer data buffer address.                    */

} LPBAM_SPI_DataAdvConf_t;

/**
  * @brief LPBAM specific advanced SPI information structure definition.
  */
typedef struct
{
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

  LPBAM_SPI_AutonomousModeConf_t AutoModeConf; /*!< Specifies the autonomous mode configuration
                                                    Please refer to LPBAM_SPI_AutonomousModeConf_t definition in
                                                    stm32_platform_lpbam_spi.h for more information.                  */

  uint32_t WakeupIT;                           /*!< Specifies the wake up source interrupt.
                                                    This parameter can be a value of @ref LPBAM_SPI_Wakeup_Interrupt  */

  uint16_t Size;                               /*!< Specifies the element number.
                                                    This parameter can be a value between 1 and 0x400                 */

  uint8_t *pTxData;                            /*!< Specifies the pointer to SPI Tx transfer data buffer.             */

  uint8_t *pRxData;                            /*!< Specifies the pointer to SPI Rx transfer data buffer.             */

} LPBAM_SPI_FullAdvConf_t;

/**
  * @brief LPBAM SPI simplex transmit configuration descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[7U]; /*!< Specifies the content of nodes required for SPI transfers : 7 different nodes are
                                   needed                                                                             */

  uint32_t pReg[7U];          /*!< Specifies the content of register to be updated : 7 different values are needed    */

} LPBAM_SPI_TxConfigDesc_t;

/**
  * @brief LPBAM SPI simplex transmit data descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[6U]; /*!< Specifies the content of nodes required for SPI transfers : 6 different nodes are
                                  needed                                                                              */

  uint32_t pReg[5U];          /*!< Specifies the content of register to be updated : 5 different values are needed    */

} LPBAM_SPI_TxDataDesc_t;

/**
  * @brief LPBAM SPI simplex transmit full descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[10U]; /*!< Specifies the content of nodes required for SPI transfers : 10 different nodes
                                    are needed                                                                        */

  uint32_t pReg[9U];           /*!< Specifies the content of register to be updated : 9 different values are needed   */

} LPBAM_SPI_TxFullDesc_t;


/**
  * @brief LPBAM SPI simplex receive configuration descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[7U]; /*!< Specifies the content of nodes required for SPI transfers : 7 different nodes are
                                   needed                                                                             */

  uint32_t pReg[7U];          /*!< Specifies the content of register to be updated : 7 different values are needed    */

} LPBAM_SPI_RxConfigDesc_t;

/**
  * @brief LPBAM SPI simplex receive data descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[6U]; /*!< Specifies the content of nodes required for SPI transfers : 6 different nodes are
                                   needed                                                                             */

  uint32_t pReg[5U];          /*!< Specifies the content of register to be updated : 5 different values are needed    */

} LPBAM_SPI_RxDataDesc_t;

/**
  * @brief LPBAM SPI simplex receive full descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[10U]; /*!< Specifies the content of nodes required for SPI transfers : 10 different nodes
                                    are needed                                                                        */

  uint32_t pReg[9U];           /*!< Specifies the content of register to be updated : 9 different values are needed   */

} LPBAM_SPI_RxFullDesc_t;


/**
  * @brief LPBAM SPI full duplex transmit and receive configuration descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pNodes[7U]; /*!< Specifies the content of nodes required for SPI transfers : 7 different nodes are
                                   needed                                                                             */

  uint32_t pReg[7U];          /*!< Specifies the content of register to be updated : 7 different values are needed    */

} LPBAM_SPI_TxRx_ConfigDesc_t;

/**
  * @brief LPBAM SPI full duplex transmit data descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pTxNodes[6U]; /*!< Specifies the content of nodes required for SPI transfers : 6 different nodes are
                                     needed                                                                           */

  uint32_t pTxReg[5U];          /*!< Specifies the content of register to be updated : 5 different values are needed  */

} LPBAM_SPI_TxRx_TxDataDesc_t;

/**
  * @brief LPBAM SPI full duplex receive data descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pRxNodes[1U]; /*!< Specifies the content of nodes required for SPI transfers : only one node is
                                     needed                                                                           */
} LPBAM_SPI_TxRx_RxDataDesc_t;

/**
  * @brief LPBAM SPI full duplex transmit full descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pTxNodes[10U]; /*!< Specifies the content of nodes required for SPI transfers : 10 different nodes
                                      are needed                                                                      */

  uint32_t pTxReg[9U];           /*!< Specifies the content of register to be updated : 9 different values are needed */

} LPBAM_SPI_TxRx_TxFullDesc_t;

/**
  * @brief LPBAM SPI full duplex receive full descriptor structure definition.
  */
typedef struct
{
  DMA_NodeTypeDef pRxNodes[1U];  /*!< Specifies the content of nodes required for SPI transfers : only one node is
                                      needed                                                                          */
} LPBAM_SPI_TxRx_RxFullDesc_t;
/**
  * @}
  */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_SPI_Advanced_Exported_Functions LPBAM SPI Advanced Exported Functions
  * @brief      LPBAM SPI Advanced Exported Functions
  * @{
  */

/**
  * @brief  ADV_LPBAM_SPI_Tx_SetConfigQ.
  */
LPBAM_Status_t ADV_LPBAM_SPI_Tx_SetConfigQ(SPI_TypeDef               *const pInstance,
                                           LPBAM_DMAListInfo_t       const *const pDMAListInfo,
                                           LPBAM_SPI_ConfigAdvConf_t const *const pTxConfig,
                                           LPBAM_SPI_TxConfigDesc_t  *const pDescriptor,
                                           DMA_QListTypeDef          *const pQueue);
/**
  * @brief  ADV_LPBAM_SPI_Tx_SetDataQ.
  */
LPBAM_Status_t ADV_LPBAM_SPI_Tx_SetDataQ(SPI_TypeDef             *const pInstance,
                                         LPBAM_DMAListInfo_t     const *const pDMAListInfo,
                                         LPBAM_SPI_DataAdvConf_t const *const pTxData,
                                         LPBAM_SPI_TxDataDesc_t  *const pDescriptor,
                                         DMA_QListTypeDef        *const pQueue);
/**
  * @brief  ADV_LPBAM_SPI_Tx_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_SPI_Tx_SetFullQ(SPI_TypeDef             *const pInstance,
                                         LPBAM_DMAListInfo_t     const *const pDMAListInfo,
                                         LPBAM_SPI_FullAdvConf_t const *const pTxFull,
                                         LPBAM_SPI_TxFullDesc_t  *const pDescriptor,
                                         DMA_QListTypeDef        *const pQueue);
/**
  * @brief  ADV_LPBAM_SPI_Rx_SetConfigQ.
  */
LPBAM_Status_t ADV_LPBAM_SPI_Rx_SetConfigQ(SPI_TypeDef               *const pInstance,
                                           LPBAM_DMAListInfo_t       const *const pDMAListInfo,
                                           LPBAM_SPI_ConfigAdvConf_t const *const pRxConfig,
                                           LPBAM_SPI_RxConfigDesc_t  *const pDescriptor,
                                           DMA_QListTypeDef          *const pQueue);
/**
  * @brief  ADV_LPBAM_SPI_Rx_SetDataQ.
  */
LPBAM_Status_t ADV_LPBAM_SPI_Rx_SetDataQ(SPI_TypeDef             *const pInstance,
                                         LPBAM_DMAListInfo_t     const *const pDMAListInfo,
                                         LPBAM_SPI_DataAdvConf_t const *const pRxData,
                                         LPBAM_SPI_RxDataDesc_t  *const pDescriptor,
                                         DMA_QListTypeDef        *const pQueue);
/**
  * @brief  ADV_LPBAM_SPI_Rx_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_SPI_Rx_SetFullQ(SPI_TypeDef             *const pInstance,
                                         LPBAM_DMAListInfo_t     const *const pDMAListInfo,
                                         LPBAM_SPI_FullAdvConf_t const *const pRxFull,
                                         LPBAM_SPI_RxFullDesc_t  *const pDescriptor,
                                         DMA_QListTypeDef        *const pQueue);
/**
  * @brief  ADV_LPBAM_SPI_TxRx_SetConfigQ.
  */
LPBAM_Status_t ADV_LPBAM_SPI_TxRx_SetConfigQ(SPI_TypeDef                 *const pInstance,
                                             LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                             LPBAM_SPI_ConfigAdvConf_t   const *const pTxRxConfig,
                                             LPBAM_SPI_TxRx_ConfigDesc_t *const pDescriptor,
                                             DMA_QListTypeDef            *const pQueue);
/**
  * @brief  ADV_LPBAM_SPI_TxRx_SetDataQ.
  */
LPBAM_Status_t ADV_LPBAM_SPI_TxRx_SetDataQ(SPI_TypeDef                 *const pInstance,
                                           LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                           LPBAM_SPI_DataAdvConf_t     const *const pTxRxData,
                                           LPBAM_SPI_TxRx_TxDataDesc_t *const pTxDescriptor,
                                           LPBAM_SPI_TxRx_RxDataDesc_t *const pRxDescriptor,
                                           DMA_QListTypeDef            *const pTxQueue,
                                           DMA_QListTypeDef            *const pRxQueue);
/**
  * @brief  ADV_LPBAM_SPI_TxRx_SetFullQ.
  */
LPBAM_Status_t ADV_LPBAM_SPI_TxRx_SetFullQ(SPI_TypeDef                 *const pInstance,
                                           LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                           LPBAM_SPI_FullAdvConf_t     const *const pTxRxFull,
                                           LPBAM_SPI_TxRx_TxFullDesc_t *const pTxDescriptor,
                                           LPBAM_SPI_TxRx_RxFullDesc_t *const pRxDescriptor,
                                           DMA_QListTypeDef            *const pTxQueue,
                                           DMA_QListTypeDef            *const pRxQueue);
/**
  * @brief  ADV_LPBAM_SPI_EnableDMARequests.
  */
LPBAM_Status_t ADV_LPBAM_SPI_EnableDMARequests(SPI_TypeDef *const pInstance);
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

#endif /* STM32_ADV_LPBAM_SPI_H */
