/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_common.h
  * @author  MCD Application Team
  * @brief   header for the stm32_adv_lpbam_common.c file
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
#ifndef STM32_ADV_LPBAM_COMMON_H
#define STM32_ADV_LPBAM_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_platform_lpbam_common.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup COMMON_Advanced COMMON_Advanced
  * @brief    COMMON Advanced LPBAM module driver
  * @{
  */
/** @defgroup LPBAM_COMMON_Advanced_Exported_Types LPBAM COMMON Advanced Exported Types
  * @brief    LPBAM COMMON Advanced Exported Types
  * @{
  */
/**
  * @brief LPBAM COMMON Transfer Configuration Structure definition.
  */
typedef struct
{
  LPBAM_DMA_NodeConfTypeDef TransferConfig;      /*!< Specifies the transfer configuration parameters.                */

  LPBAM_FunctionalState UpdateSrcInc;            /*!< Specifies whether the data source increment to be updated or not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring SrcInc in TransferConfig
                                                      is mandatory                                                    */
  LPBAM_FunctionalState UpdateDestInc;           /*!< Specifies whether the data destination increment to be updated
                                                      or not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring DestInc in
                                                      TransferConfig is mandatory                                     */
  LPBAM_FunctionalState UpdateSrcDataWidth;      /*!< Specifies whether the data source data width to be updated or
                                                      not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring SrcDataWidth in
                                                      TransferConfig is mandatory                                     */
  LPBAM_FunctionalState UpdateDestDataWidth;     /*!< Specifies whether the destination data width to be updated or
                                                      not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring DestDataWidth in
                                                      TransferConfig is mandatory                                     */
  LPBAM_FunctionalState UpdateTransferEventMode; /*!< Specifies whether the transfer event mode to be updated or not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring TransferEventMode in
                                                      TransferConfig is mandatory                                     */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  LPBAM_FunctionalState UpdateSrcSecure;         /*!< Specifies whether the source security to be updated or not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring SrcSecure in
                                                      TransferConfig is mandatory                                     */
  LPBAM_FunctionalState UpdateDestSecure;        /*!< Specifies whether the destination security to be updated or not.
                                                      This parameter can be ENABLE or DISABLE
                                                      When this field is enabled, configuring DestSecure in
                                                      TransferConfig is mandatory                                     */
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

} LPBAM_COMMON_DataAdvConf_t;

/**
  * @brief LPBAM COMMON Trigger Configuration Structure definition.
  */
typedef struct
{
  LPBAM_DMA_TriggerConfTypeDef TriggerConfig; /*!< Specifies the transfer trigger parameters. */

} LPBAM_COMMON_TrigAdvConf_t;
/**
  * @}
  */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_COMMON_Advanced_Exported_Constants LPBAM COMMON Advanced Exported Constants
  * @brief    LPBAM COMMON Advanced Exported Constants.
  * @{
  */

/** @addtogroup LPBAM_Q_Config_Node_Selection LPBAM Q Configuration Node Selection
  * @{
  */
/* ADC configuration nodes defines */
#define LPBAM_ADC_CONVERSION_CONFIGQ_CONFIG_NODE  (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_ADC_Conversion_SetConfigQ().
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_ADC_CONVERSION_DATAQ_CONFIG_NODE    (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_ADC_Conversion_SetDataQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_ADC_CONVERSION_FULLQ_CONFIG_NODE    (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_ADC_Conversion_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_ADC_ANALOGWDG_FULLQ_CONFIG_NODE     (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_ADC_STOP_FULLQ_CONFIG_NODE          (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_ADC_Stop_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */

/* COMP configuration nodes defines */
#define LPBAM_COMP_START_FULLQ_CONFIG_NODE        (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_COMP_Start_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_COMP_STOP_FULLQ_CONFIG_NODE         (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_COMP_Stop_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */

/* DAC configuration nodes defines */
#define LPBAM_DAC_CONVERSION_CONFIGQ_CONFIG_NODE  (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_DAC_Conversion_SetConfigQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_DAC_CONVERSION_DATAQ_CONFIG_NODE    (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_DAC_Conversion_SetDataQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_DAC_CONVERSION_FULLQ_CONFIG_NODE    (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_DAC_Conversion_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_DAC_STOP_FULLQ_CONFIG_NODE          (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_DAC_Stop_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */

/* DMA configuration nodes defines */
#define LPBAM_DMA_START_FULLQ_CONFIG_NODE         (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_DMA_Start_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_DMA_STOP_FULLQ_CONFIG_NODE          (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_DMA_Stop_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */

/* I2C configuration nodes defines */
#define LPBAM_I2C_MASTERTX_CONFIGQ_CONFIG_NODE    (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_I2C_MasterTx_SetConfigQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_I2C_MASTERTX_DATAQ_CONFIG_NODE      (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_I2C_MasterTx_SetDataQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_I2C_MASTERTX_FULLQ_CONFIG_NODE      (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_I2C_MasterTx_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_I2C_SLAVETX_CONFIGQ_CONFIG_NODE     (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_I2C_SlaveTx_SetConfigQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_I2C_SLAVETX_DATAQ_CONFIG_NODE       (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_I2C_SlaveTx_SetDataQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_I2C_SLAVETX_FULLQ_CONFIG_NODE       (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_I2C_SlaveTx_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_I2C_MASTERRX_CONFIGQ_CONFIG_NODE    (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_I2C_Masterx_SetConfigQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_I2C_MASTERRX_DATAQ_CONFIG_NODE      (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_I2C_MasterRx_SetDataQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_I2C_MASTERRX_FULLQ_CONFIG_NODE      (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_I2C_MasterRx_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_I2C_SLAVERX_CONFIGQ_CONFIG_NODE     (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_I2C_SlaveRx_SetConfigQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_I2C_SLAVERX_DATAQ_CONFIG_NODE       (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_I2C_SlaveRx_SetDataQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_I2C_SLAVERX_FULLQ_CONFIG_NODE       (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_I2C_SlaveRx_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */

/* LPTIM configuration nodes defines */
#define LPBAM_LPTIM_PWM_FULLQ_CONFIG_NODE         (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_LPTIM_PWM_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_LPTIM_UE_FULLQ_CONFIG_NODE          (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_LPTIM_UE_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_LPTIM_GETCOUNTER_FULLQ_CONFIG_NODE  (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_LPTIM_GetCounter_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_LPTIM_START_FULLQ_CONFIG_NODE       (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_LPTIM_Start_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_LPTIM_STOP_FULLQ_CONFIG_NODE        (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_LPTIM_Stop_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */

/* OPAMP configuration nodes defines */
#define LPBAM_OPAMP_START_FULLQ_CONFIG_NODE       (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_OPAMP_Start_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_OPAMP_STOP_FULLQ_CONFIG_NODE        (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_OPAMP_Stop_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */

/* SPI configuration nodes defines */
#define LPBAM_SPI_TX_CONFIGQ_CONFIG_NODE          (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_SPI_Tx_SetConfigQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_SPI_TX_DATAQ_CONFIG_NODE            (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_SPI_Tx_SetDataQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_SPI_TX_FULLQ_CONFIG_NODE            (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_SPI_Tx_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_SPI_RX_CONFIGQ_CONFIG_NODE          (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_SPI_Rx_SetConfigQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_SPI_RX_DATAQ_CONFIG_NODE            (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_SPI_Rx_SetDataQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_SPI_RX_FULLQ_CONFIG_NODE            (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_SPI_Rx_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_SPI_TXRX_CONFIGQ_CONFIG_NODE        (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_SPI_TxRx_SetConfigQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_SPI_TXRX_DATAQ_CONFIG_NODE          (0x00U) /*!< Specifies the config nodes of the queues built by
                                                               ADV_LPBAM_SPI_TxRx_SetDataQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig() : to
                                                                           be applied only for TX queue.
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_SPI_TXRX_FULLQ_CONFIG_NODE          (0x00U) /*!< Specifies the config nodes of the queues built by
                                                               ADV_LPBAM_SPI_TxRx_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig() : to
                                                                           be applied only for TX queue.
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */

/* UART configuration nodes defines */
#define LPBAM_UART_TX_CONFIGQ_CONFIG_NODE         (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_UART_Transmit_SetConfigQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_UART_TX_DATAQ_CONFIG_NODE           (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_UART_Transmit_SetDataQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_UART_TX_FULLQ_CONFIG_NODE           (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_UART_Transmit_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_UART_RX_CONFIGQ_CONFIG_NODE         (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_UART_Receive_SetConfigQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_UART_RX_DATAQ_CONFIG_NODE           (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_UART_Receive_SetDataQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
#define LPBAM_UART_RX_FULLQ_CONFIG_NODE           (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_UART_Receive_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */

/* VREFBUF configuration nodes defines */
#define LPBAM_VREFBUF_BUFMODE_CONFIGQ_CONFIG_NODE (0x00U) /*!< Specifies the config node of the queue built by
                                                               ADV_LPBAM_VREFBUF_BufferMode_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                         + ADV_LPBAM_Q_SetTriggerConfig()
                                                                         + ADV_LPBAM_Q_SetCircularMode()        */
/**
  * @}
  */

/** @addtogroup LPBAM_Q_Data_Node_Selection LPBAM Q Data Node Selection
  * @{
  */
/* ADC data nodes defines */
#define LPBAM_ADC_CONVERSION_DATAQ_DATA_NODE   (0x01U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_ADC_Conversion_SetDataQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetCircularMode()
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_ADC_CONVERSION_FULLQ_DATA_NODE   (0x0DU) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_ADC_Conversion_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetCircularMode()
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
/* COMP data nodes defines */
#define LPBAM_COMP_OUTPUTLEVEL_FULLQ_DATA_NODE (0x00U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_COMP_OutputLevel_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetTriggerConfig()
                                                                      + ADV_LPBAM_Q_SetCircularMode()
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
/* DAC data nodes defines */
#define LPBAM_DAC_CONVERSION_DATAQ_DATA_NODE   (0x00U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_DAC_Conversion_SetDataQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetCircularMode()
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_DAC_CONVERSION_FULLQ_DATA_NODE   (0x02U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_DAC_Conversion_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetCircularMode()
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */

/* GPIO data nodes defines */
#define LPBAM_GPIO_WRITEPIN_FULLQ_DATA_NODE    (0x00U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_GPIO_WritePin_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetTriggerConfig()
                                                                      + ADV_LPBAM_Q_SetCircularMode()
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_GPIO_WRITEPINSEQ_FULLQ_DATA_NODE (0x00U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_GPIO_WritePinSequence_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetTriggerConfig()
                                                                      + ADV_LPBAM_Q_SetCircularMode()
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_GPIO_READPINSEQ_FULLQ_DATA_NODE  (0x00U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_GPIO_ReadPinSequence_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetTriggerConfig()
                                                                      + ADV_LPBAM_Q_SetCircularMode()
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */

/* I2C data nodes defines */
#define LPBAM_I2C_MASTERTX_DATAQ_DATA_NODE     (0x01U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_I2C_MasterTx_SetDataQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_I2C_MASTERTX_FULLQ_DATA_NODE     (0x05U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_I2C_MasterTx_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_I2C_SLAVETX_DATAQ_DATA_NODE      (0x01U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_I2C_SlaveTx_SetDataQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_I2C_SLAVETX_FULLQ_DATA_NODE      (0x04U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_I2C_SlaveTx_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_I2C_MASTERRX_DATAQ_DATA_NODE     (0x01U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_I2C_MasterRx_SetDataQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_I2C_MASTERRX_FULLQ_DATA_NODE     (0x05U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_I2C_MasterRx_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_I2C_SLAVERX_DATAQ_DATA_NODE      (0x01U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_I2C_SlaveRx_SetDataQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_I2C_SLAVERX_FULLQ_DATA_NODE      (0x04U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_I2C_SlaveRx_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */

/* LPTIM data nodes defines */
#define LPBAM_LPTIM_IC_FULLQ_DATA_NODE         (0x00U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_LPTIM_IC_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetCircularMode()
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */

/* SPI data nodes defines */
#define LPBAM_SPI_TX_DATAQ_DATA_NODE           (0x05U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_SPI_Tx_SetDataQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_SPI_TX_FULLQ_DATA_NODE           (0x09U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_SPI_Tx_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_SPI_RX_DATAQ_DATA_NODE           (0x05U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_SPI_Rx_SetDataQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_SPI_RX_FULLQ_DATA_NODE           (0x09U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_SPI_Rx_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_SPI_TXRX_DATAQ_TXDATA_NODE       (0x05U) /*!< Specifies the data nodes of the queues built by
                                                            ADV_LPBAM_SPI_TxRx_SetDataQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_SPI_TXRX_DATAQ_RXDATA_NODE       (0x00U) /*!< Specifies the data nodes of the queues built by
                                                            ADV_LPBAM_SPI_TxRx_SetDataQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_SPI_TXRX_FULLQ_TXDATA_NODE       (0x09U) /*!< Specifies the data nodes of the queues built by
                                                            ADV_LPBAM_SPI_TxRx_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_SPI_TXRX_FULLQ_RXDATA_NODE       (0x00U) /*!< Specifies the data nodes of the queues built by
                                                            ADV_LPBAM_SPI_TxRx_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */

/* UART data nodes defines */
#define LPBAM_UART_TX_DATAQ_DATA_NODE          (0x00U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_UART_Transmit_SetDataQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetCircularMode()
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_UART_TX_FULLQ_DATA_NODE          (0x04U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_UART_Transmit_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetCircularMode()
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_UART_RX_DATAQ_DATA_NODE          (0x00U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_UART_Receive_SetDataQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetCircularMode()
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
#define LPBAM_UART_RX_FULLQ_DATA_NODE          (0x02U) /*!< Specifies the data node of the queue built by
                                                            ADV_LPBAM_UART_Receive_SetFullQ()
                                                            This define can be used as a parameter for :
                                                                      + ADV_LPBAM_Q_SetCircularMode()
                                                                      + ADV_LPBAM_Q_SetDataConfig()        */
/**
  * @}
  */

/** @addtogroup LPBAM_Q_LastNode_Selection LPBAM Q Last Node Selection
  * @{
  */
/* ADC last nodes defines */
#define LPBAM_ADC_CONVERSION_CONFIGQ_LAST_NODE  (0x0BU) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_DAC_Conversion_SetConfigQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_ADC_ANALOGWDG_FULLQ_LAST_NODE     (0x09U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_ADC_AnalogWDGConfig_SetFullQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_ADC_STOP_FULLQ_LAST_NODE          (0x00U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_ADC_Stop_SetFullQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */

/* COMP last nodes defines */
#define LPBAM_COMP_START_FULLQ_LAST_NODE        (0x00U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_COMP_Start_SetFullQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_COMP_STOP_FULLQ_LAST_NODE         (0x00U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_COMP_Stop_SetFullQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */

/* DAC last nodes defines */
#define LPBAM_DAC_CONVERSION_CONFIGQ_LAST_NODE  (0x01U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_DAC_Conversion_SetConfigQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_DAC_STOP_FULLQ_LAST_NODE          (0x00U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_DAC_Stop_SetFullQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */

/* I2C last nodes defines */
#define LPBAM_I2C_MASTERRX_CONFIGQ_LAST_NODE    (0x03U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_I2C_MasterRx_SetConfigQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_I2C_SLAVERX_CONFIGQ_LAST_NODE     (0x02U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_I2C_SlaveRx_SetConfigQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_I2C_MASTERTX_CONFIGQ_LAST_NODE    (0x03U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_I2C_MasterTx_SetConfigQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_I2C_SLAVETX_CONFIGQ_LAST_NODE     (0x02U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_I2C_SlaveTx_SetConfigQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */

/* DMA last nodes defines */
#define LPBAM_DMA_START_FULLQ_LAST_NODE         (0x05U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_DMA_Start_SetFullQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_DMA_STOP_FULLQ_LAST_NODE          (0x00U) /*!< Specifies the last node of the queue built by
                                                               ADV_LPBAM_DMA_Stop_SetFullQ()
                                                               This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */

/* LPTIM last nodes defines */
#define LPBAM_LPTIM_START_FULLQ_LAST_NODE       (0x02U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_LPTIM_Start_SetFullQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_LPTIM_STOP_FULLQ_LAST_NODE        (0x00U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_LPTIM_Stop_SetFullQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_LPTIM_PWM_FULLQ_LAST_NODE         (0x02U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_LPTIM_PWM_SetFullQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_LPTIM_UE_FULLQ_LAST_NODE          (0x01U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_LPTIM_UE_SetFullQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */

/* UART last nodes defines */
#define LPBAM_UART_TX_CONFIGQ_LAST_NODE         (0x03U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_UART_Transmit_SetConfigQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_UART_RX_CONFIGQ_LAST_NODE         (0x01U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_UART_Receive_SetConfigQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */

/* OPAMP last nodes defines */
#define LPBAM_OPAMP_START_FULLQ_LAST_NODE       (0x00U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_OPAMP_Start_SetFullQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_OPAMP_STOP_FULLQ_LAST_NODE        (0x00U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_OPAMP_Stop_SetFullQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */

/* SPI last nodes defines */
#define LPBAM_SPI_TX_CONFIGQ_LAST_NODE          (0x06U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_SPI_Tx_SetConfigQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_SPI_RX_CONFIGQ_LAST_NODE          (0x06U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_SPI_Rx_SetConfigQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
#define LPBAM_SPI_TXRX_CONFIGQ_LAST_NODE        (0x06U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_SPI_TxRx_SetConfigQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */

/* VREFBUF last nodes defines */
#define LPBAM_VREFBUF_BUFMODE_CONFIGQ_LAST_NODE (0x00U) /*!< Specifies the last node of the queue built by
                                                             ADV_LPBAM_VREFBUF_BufferMode_SetFullQ()
                                                             This define can be used as a parameter for :
                                                                       + ADV_LPBAM_Q_SetTransferEventGeneration() */
/**
  * @}
  */

/**
  * @}
  */


/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_COMMON_Advanced_Exported_Functions LPBAM COMMON Advanced Exported Functions
  * @brief      LPBAM COMMON Advanced Exported Functions
  * @{
  */

/**
  * @brief ADV_LPBAM_Q_SetDataConfig.
  */
LPBAM_Status_t ADV_LPBAM_Q_SetDataConfig(LPBAM_COMMON_DataAdvConf_t const *const pDataConfig,
                                         uint32_t                   DataNode,
                                         void                       const *const pDescriptor);
/**
  * @brief ADV_LPBAM_Q_SetTransferEventGeneration.
  */
LPBAM_Status_t ADV_LPBAM_Q_SetTransferEventGeneration(uint32_t TransferEvent,
                                                      uint32_t TransferEventNode,
                                                      void     const *const pDescriptor);
/**
  * @brief ADV_LPBAM_Q_SetTriggerConfig.
  */
LPBAM_Status_t ADV_LPBAM_Q_SetTriggerConfig(LPBAM_COMMON_TrigAdvConf_t const *const pTrigConfig,
                                            uint32_t                   NodeLevel,
                                            void                       const *const pDescriptor);
/**
  * @brief ADV_LPBAM_Q_SetCircularMode.
  */
LPBAM_Status_t ADV_LPBAM_Q_SetCircularMode(void             const *const pDescriptor,
                                           uint32_t         NodeLevel,
                                           DMA_QListTypeDef *const pQueue);
/**
  * @brief ADV_LPBAM_Q_ConvertQ.
  */
LPBAM_Status_t ADV_LPBAM_Q_ConvertQ(uint32_t         Type,
                                    DMA_QListTypeDef *const pQueue);
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

#endif /* STM32_ADV_LPBAM_COMMON_H */
