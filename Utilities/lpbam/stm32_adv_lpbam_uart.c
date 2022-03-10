/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_uart.c
  * @author  MCD Application Team
  * @brief   This file provides the advanced LPBAM layer for UART peripheral.
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
  @verbatim
  ======================================================================================================================
                                 ############### How to use this driver ###############
  ======================================================================================================================
    [..]
      It is recommended to read the LPBAM_Utility_GettingStarted.html document, available at the root of LPBAM utility
      folder, prior to any LPBAM application development start.

    *** Driver description ***
    ==========================
    [..]
      This section provide description of the driver files content (refer to LPBAM_Utility_GettingStarted.html document
      for more information)

    [..]
      It is composed of 3 files :
          (+) stm32_adv_lpbam_uart.c file
              (++) This file provides the implementation of the advanced LPBAM UART functions.
          (+) stm32_adv_lpbam_uart.h file
              (++) This file is the header file of stm32_adv_lpbam_lptim.c. It provides advanced LPBAM UART functions
                   prototypes and the declaration of their needed exported types and structures.
          (+) STM32xx/stm32_platform_lpbam_uart.h file
              (++) This header file contains all defines to be used on applicative side.
                   (+++) STM32xx stands for the device supporting LPBAM sub-system.

    *** Driver functions model ***
    ==============================
    [..]
      This section precises this module supported advanced functions model (refer to LPBAM_Utility_GettingStarted.html
      document for function model definition).

    [..]
      This driver provides 3 model of APIs :
          (+) ADV_LPBAM_{Module}_{Mode}_SetConfigQ() : provides one peripheral configuration queue.
          (+) ADV_LPBAM_{Module}_{Mode}_SetDataQ() : provides one peripheral data transfer queue.
          (+) ADV_LPBAM_{Module}_{Mode}_SetFullQ() : provides one peripheral configuration and one data transfer queue.

    *** Driver features ***
    =======================
    [..]
      This section describes this LPBAM module supported features.

    [..]
      This driver provides services covering the LPBAM management of the following UART features :
          (+) Configure the UART transfers in simplex or full duplex modes.
          (+) Starts the UART data transfers in simplex or full duplex modes.
          (+) Configure and starts the UART data transfers in simplex or full duplex modes.

    *** Functional description ***
    ==============================
    [..]
      This section describes the peripheral features covered by this LPBAM module.

    [..]
      The output of each API is one linked-list queue. For simplex transfer, it is performed by executing one API
      (Transmit or Receive) according to application needs. For the full duplex transfer, it is performed by executing
      two APIs (one Transmit and one Receive) then link each one to a different DMA channel. (Please refer to Driver
      APIs description and user sequence section for more information).

      The UART peripheral transfer is configured in simplex or full duplex mode.
      The UART peripheral transfer is started in simplex or full duplex mode.
      The UART peripheral transfer is configured and started in simplex or full duplex mode.

    *** Driver APIs description ***
    ===============================
    [..]
      This section provides LPBAM module exhaustive APIs description without considering application user call sequence.
      For user call sequence information, please refer to 'Driver user sequence' section below.

    [..]
      Use ADV_LPBAM_UART_Transmit_SetConfigQ() API to build a linked-list queue that setup the UART transmission
      configuration according to parameters in the LPBAM_UART_ConfigAdvConf_t structure.
      The configuration parameters are :
          (+) Mode         : Specifies the UART mode.
                             This parameter can be a LPBAM_UART_MODE_TX or LPBAM_UART_MODE_TX_RX.
          (+) AutoModeConf : Specifies the autonomous mode.
                             (++) AutonomousModeState : Specifies the autonomous mode state.
                                                        This parameter shall be LPBAM_UART_AUTONOMOUS_MODE_ENABLE.
                             (++) TriggerSelection    : Specifies the autonomous mode trigger signal selection.
                                                        This parameter can be a value
                                                        of @ref LPBAM_UART_AutonomousMode_TriggerSelection.
                             (++) TriggerPolarity     : Specifies the autonomous mode trigger signal polarity.
                                                        This parameter can be a value
                                                        of @ref LPBAM_UART_AutonomousMode_TriggerPolarity.
                             (++) DataSize            : Specifies the UART data number of bytes to transfer in each
                                                        packet.
          (+) WakeupIT    : Specifies the wake up source.
                            UART wake up IT can be:
                            (++) NONE  : UART wake up interrupt none.
                            (++) IDLE  : UART idle interrupt.
                            (++) TC    : UART transfer complete interrupt.
                            (++) TXFNF : UART TX FIFO not full interrupt.
                            (++) TXFE  : UART TX FIFO empty interrupt.

    [..]
      Use ADV_LPBAM_UART_Transmit_SetDataQ() API to build a linked-list queue that setup the UART transmission operation
      according to parameters in the LPBAM_UART_DataAdvConf_t structure.
      The configuration parameters are :
          (+) pData : Specifies the data source buffer.
          (+) Size  : Specifies the data number of bytes to be sent.

      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_UART_Transmit_SetDataQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c
      file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) DestInc           : DMA_DINC_FIXED.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.

    [..]
      Use ADV_LPBAM_UART_Transmit_SetFullQ() API to build a linked-list queue that setup the UART configuration and
      transmission according to parameters in the LPBAM_UART_FullAdvConf_t structure.
      The configuration parameters are :
          (+) Mode         : Specifies the UART mode.
                             This parameter can be a LPBAM_UART_MODE_TX or LPBAM_UART_MODE_TX_RX.
          (+) AutoModeConf : Specifies the autonomous mode.
                             (++) AutonomousModeState : Specifies the autonomous mode state.
                                                        This parameter shall be LPBAM_UART_AUTONOMOUS_MODE_ENABLE.
                             (++) TriggerSelection    : Specifies the autonomous mode trigger signal selection.
                                                        This parameter can be a value
                                                        of @ref LPBAM_UART_AutonomousMode_TriggerSelection.
                             (++) TriggerPolarity     : Specifies the autonomous mode trigger signal polarity.
                                                        This parameter can be a value
                                                        of @ref LPBAM_UART_AutonomousMode_TriggerPolarity.
                             (++) DataSize            : Specifies the UART data number of bytes to transfer in each
                                                        packet.
          (+) WakeupIT    : Specifies the wake up source.
                            UART wake up IT can be:
                            (++) NONE  : UART wake up interrupt none.
                            (++) IDLE  : UART idle interrupt.
                            (++) TC    : UART transfer complete interrupt.
                            (++) TXFNF : UART TX FIFO not full interrupt.
                            (++) TXFE  : UART TX FIFO empty interrupt.
          (+) pData       : Specifies the data source buffer.
          (+) Size        : Specifies the data number of bytes to be sent.

      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_UART_Transmit_SetFullQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c
      file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) DestInc           : DMA_DINC_FIXED.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.

    [..]
      Use ADV_LPBAM_UART_Receive_SetConfigQ() API to build a linked-list queue that sets up the UART reception
      configuration according to parameters in the LPBAM_UART_ConfigAdvConf_t structure.
      The configuration parameters are :
          (+) Mode     : Specifies the UART mode.
                         This parameter can be a LPBAM_UART_MODE_RX or LPBAM_UART_MODE_TX_RX.
          (+) WakeupIT : Specifies the wake up source.
                         UART wake up IT can be:
                         (++) NONE  : UART wake up interrupt none.
                         (++) IDLE  : UART idle interrupt.
                         (++) RXFNE : UART RX FIFO not empty interrupt.
                         (++) RXFF  : UART RX FIFO full interrupt.

    [..]
      Use ADV_LPBAM_UART_Receive_SetDataQ() API to build a linked-list queue that sets up the UART reception operation
      according to parameters in the LPBAM_UART_DataAdvConf_t structure.
      The configuration parameters are :
          (+) pData : Specifies the data source buffer.
          (+) Size  : Specifies the data number of bytes to be sent.

      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_UART_Receive_SetDataQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c
      file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.

    [..]
      Use ADV_LPBAM_UART_Receive_SetFullQ() API to build a linked-list queue that sets up the UART configuration and
      reception according to parameters in the LPBAM_UART_FullAdvConf_t structure.
      The configuration parameters are :
          (+) Mode     : Specifies the UART mode.
                         This parameter can be a LPBAM_UART_MODE_RX or LPBAM_UART_MODE_TX_RX.
          (+) WakeupIT : Specifies the wake up source.
                         UART wake up IT can be:
                         (++) NONE  : UART wake up interrupt none.
                         (++) IDLE  : UART idle interrupt.
                         (++) RXFNE : UART RX FIFO not empty interrupt.
                         (++) RXFF  : UART RX FIFO full interrupt.
          (+) pData    : Specifies the data source buffer.
          (+) Size     : Specifies the data number of bytes to be sent.

      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_UART_Receive_SetFullQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c
      file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.

      These APIs must be called when the UART is initialized.
           (+) Recommended UART initialization sequence
              (++) Call HAL_UART_Init() to initialize the UART. (Mandatory)
                   Initialization parameters can be :
                   (+++) BaudRate       : Any.
                   (+++) WordLength     : Any.
                   (+++) StopBits       : Any.
                   (+++) Parity         : Any.
                   (+++) HwFlowCtl      : UART_HWCONTROL_NONE. (Mandatory)
                   (+++) Mode           : Any.
                   (+++) ClockPrescaler : Any.
                   (+++) AdvFeatureInit : UART_ADVFEATURE_NO_INIT.
              (++) Call HAL_UARTEx_SetConfigAutonomousMode() to configure the autonomous mode.(Mandatory only for
                   transmission)
                   Autonomous mode parameters can be :
                   (+++) AutonomousModeState : UART_AUTONOMOUS_MODE_ENABLE. (Mandatory for transmit APIs)
                   (+++) IdleFrame           : Any.
                   (+++) TriggerPolarity     : Any.
                   (+++) TriggerSelection    : Any.
                   (+++) DataSize            : Any.
              (++) Call HAL_UARTEx_EnableStopMode() to enable the UART in stop mode. (Mandatory)
              (++) Call __HAL_UART_ENABLE_IT() to enable error interrupts.
                   UART error interrupts can be :
                   (+++) UART_IT_ERR : overrun error, framing error, noise error.
                   (+++) UART_IT_PE  : Parity error (When parity bit is enabled).

    *** Driver user sequence ***
    ============================
    [..]
      This section provides the steps to follow to build an LPBAM application based on HAL/LL and LPBAM drivers. (refer
      to LPBAM_Utility_GettingStarted.html for linked-list feature description).

    [..]
      Simplex user sequence :
          (+) Initialize all needed GPIOs. (Depends on application needs)
          (+) Initialize the UART (Using HAL/LL) in simplex mode. (Mandatory)
          (+) Call ADV_LPBAM_UART_EnableDMARequests() to enable the UART DMA requests. (Mandatory)
          (+) Call ADV_LPBAM_UART_{Transfer}_SetConfigQ() and/or ADV_LPBAM_UART_{Transfer}_SetDataQ() and/or
              ADV_LPBAM_UART_{Transfer}_SetFullQ. (Mandatory)
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_UART_{Transfer}_SetConfigQ(), ADV_LPBAM_UART_{Transfer}_SetDataQ() or
              ADV_LPBAM_UART_{Transfer}_SetFullQ() output queue.
          (+) Call, optionally, ADV_LPBAM_Q_SetCircularMode() in stm32_adv_lpbam_common.c to circularize your
              linked-list queue for infinite scenarios cases.
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode. (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)

    [..]
      Full duplex user sequence :
          (+) Initialize all needed GPIOs. (Depends on application needs)
          (+) Initialize the UART (Using HAL/LL) in full duplex mode. (Mandatory)
          (+) Call ADV_LPBAM_UART_EnableDMARequests() to enable the UART DMA requests. (Mandatory)
          (+) Call ADV_LPBAM_UART_Receive_SetConfigQ() and/or ADV_LPBAM_UART_Receive_SetDataQ() and/or
              ADV_LPBAM_UART_Receive_SetFullQ(). (Mandatory)
          (+) Call ADV_LPBAM_UART_Transmit_SetConfigQ() and/or ADV_LPBAM_UART_Transmit_SetDataQ() and/or
              ADV_LPBAM_UART_Transmit_SetFullQ(). (Mandatory)
          (+) Call ADV_LPBAM_UART_Transmit_SetConfigQ() and/or ADV_LPBAM_UART_Transmit_SetDataQ() and/or
              ADV_LPBAM_UART_Transmit_SetFullQ(). (Mandatory)
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_UART_{Transfer}_SetConfigQ(), ADV_LPBAM_UART_{Transfer}_SetDataQ() or
              ADV_LPBAM_UART_{Transfer}_SetFullQ() output queue.
          (+) Call, optionally, ADV_LPBAM_Q_SetCircularMode() in stm32_adv_lpbam_common.c to circularize your
              linked-list queues for infinite scenarios cases.
          (+) Call HAL_DMAEx_List_Init() to initialize the Receive DMA channel in linked-list mode. (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the Receive output queue to the initialized
              Receive DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the Receive DMA channel linked-list execution. (Mandatory)
          (+) Call HAL_DMAEx_List_Init() to initialize the Transmit DMA channel in linked-list
              mode. (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the Transmit output queue to the initialized
              Transmit DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the Transmit DMA channel linked-list execution. (Mandatory)

    *** Recommendation ***
    ======================
    [..]
      This section provides tips and tricks to consider while using LPBAM module drivers to build a user application.

    [..]
      For memory optimization purpose, when a re-configuration is necessary, it's recommended to use
      ADV_LPBAM_UART_{Transfer}_SetFullQ() instead of ADV_LPBAM_UART_{Transfer}_SetConfigQ() then
      ADV_LPBAM_UART_{Transfer}_SetDataQ().

    [..]
      For memory optimization purpose, when no re-configuration is necessary, it's recommended to use
      ADV_LPBAM_UART_{Transfer}_SetDataQ().

    [..]
      It's mandatory to use two linked-list queues (One for the transmission and another one for the reception) which
      will be executed simultaneously when UART is used in full duplex mode.

    [..]
      To avoid missing Rx DMA request transfers in full duplex, it's mandatory to start the DMA channel execution of Rx
      linked-list queue before starting the DMA channel execution of Tx linked-list queue.

    [..]
      For memory optimization purpose in full duplex transfers, Rx queue shall contains only data transfer ensured by
      ADV_LPBAM_UART_Receive_SetDataQ() API.

    [..]
      It's forbidden to execute simultaneously the same linked-list queue with different DMA channels.

    [..]
      It's forbidden to execute simultaneously two linked-list queue to configure the same UART instance.

    *** Driver status description ***
    =================================
    [..]
      This section provides reported LPBAM module status.

    [..]
      This advanced module reports any detected issue.
          (+) returned values are :
              (++) LPBAM_OK when no error is detected.
              (++) LPBAM_ERROR when any error is detected.

    @endverbatim
  **********************************************************************************************************************
  */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32_lpbam.h"

#ifdef LPBAM_UART_MODULE_ENABLED

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup UART_Advanced UART_Advanced
  * @brief    UART Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_UART_Advanced_Exported_Functions LPBAM UART Advanced Exported Functions
  * @{
  */

/**
  * @brief  Build DMA linked-list queue to setup the configuration of UART transmit according to parameters in
  *         LPBAM_UART_ConfigAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a USART_TypeDef structure that selects UART instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxConfig    : [IN]  Pointer to a LPBAM_UART_ConfigAdvConf_t structure that contains transmit configuration
  *                              nodes information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_UART_TxConfigDesc_t structure that contains transmit configuration
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_UART_Transmit_SetConfigQ(USART_TypeDef              *const pInstance,
                                                  LPBAM_DMAListInfo_t        const *const pDMAListInfo,
                                                  LPBAM_UART_ConfigAdvConf_t const *const pTxConfig,
                                                  LPBAM_UART_TxConfigDesc_t  *const pDescriptor,
                                                  DMA_QListTypeDef           *const pQueue)
{
  LPBAM_UART_ConfNode_t config_node;
  DMA_NodeConfTypeDef   dma_node_conf;

  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /*
   *               ######## UART disable node ########
   */

  /* Set UART instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_UART_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Set UART configuration */
  config_node.Config.State               = DISABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build config state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert config state node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## UART clear flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_UART_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert clear flag node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## UART autonomous mode node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID                 = (uint32_t)LPBAM_UART_AUTONOMOUSMODE_ID;
  config_node.NodeDesc.pSrcVarReg                      = &pDescriptor->pReg[reg_idx];

  /* Set UART configuration */
  config_node.Config.AutoModeConf.AutonomousModeState = pTxConfig->AutoModeConf.AutonomousModeState;
  config_node.Config.AutoModeConf.TriggerSelection    = pTxConfig->AutoModeConf.TriggerSelection;
  config_node.Config.AutoModeConf.TriggerPolarity     = pTxConfig->AutoModeConf.TriggerPolarity;
  config_node.Config.AutoModeConf.DataSize            = pTxConfig->AutoModeConf.DataSize;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build config packet size node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert config packet size node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## UART enable node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_UART_CONFIG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set UART configuration */
  config_node.Config.State             = ENABLE;
  config_node.Config.WakeupIT          = pTxConfig->WakeupIT;

  /* Check UART mode */
  if (pTxConfig->Mode == LPBAM_UART_MODE_TX)
  {
    config_node.Config.Mode = LPBAM_UART_MODE_TX;
  }
  else
  {
    config_node.Config.Mode = LPBAM_UART_MODE_TX_RX;
  }

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build config state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert config state node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the data of UART transmit according to parameters in
  *         LPBAM_UART_DataAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a USART_TypeDef structure that selects UART instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxData      : [IN]  Pointer to a LPBAM_UART_DataAdvConf_t structure that contains transmit data nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_UART_TxDataDesc_t structure that contains transmit data descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_UART_Transmit_SetDataQ(USART_TypeDef            *const pInstance,
                                                LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                                LPBAM_UART_DataAdvConf_t const *const pTxData,
                                                LPBAM_UART_TxDataDesc_t  *const pDescriptor,
                                                DMA_QListTypeDef         *const pQueue)
{
  LPBAM_UART_ConfNode_t config_node;
  DMA_NodeConfTypeDef   dma_node_conf;

  uint32_t node_idx = 0U;

  uint32_t buffer_address;

  /*
   *               ######## UART Transmit data node ########
   */

  /* Set UART instance */
  config_node.pInstance                  = pInstance;

  /* Get buffer address */
  buffer_address                         = (uint32_t)pTxData->pData;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_UART_TRANSMIT_DATA_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pBuff             = (uint32_t *)buffer_address;

  /* Set UART configuration */
  config_node.Config.Size                = pTxData->Size;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build Transmit data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Transmit data node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the configuration and data of UART transmit according to parameters in
  *         LPBAM_UART_FullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a USART_TypeDef structure that selects UART instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxFull      : [IN]  Pointer to a LPBAM_UART_FullAdvConf_t structure that contains transmit full nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_UART_TxFullDesc_t structure that contains transmit full descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_UART_Transmit_SetFullQ(USART_TypeDef            *const pInstance,
                                                LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                                LPBAM_UART_FullAdvConf_t const *const pTxFull,
                                                LPBAM_UART_TxFullDesc_t  *const pDescriptor,
                                                DMA_QListTypeDef         *const pQueue)
{
  LPBAM_UART_ConfNode_t config_node;
  DMA_NodeConfTypeDef   dma_node_conf;

  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  uint32_t buffer_address;

  /*
   *               ######## UART disable node ########
   */

  /* Set UART instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_UART_CONFIG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Set UART configuration */
  config_node.Config.State               = DISABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build config state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert config state node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## UART clear flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_UART_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert clear flag node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## UART autonomous mode node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID                = (uint32_t)LPBAM_UART_AUTONOMOUSMODE_ID;
  config_node.NodeDesc.pSrcVarReg                     = &pDescriptor->pReg[reg_idx];

  /* Set UART configuration */
  config_node.Config.AutoModeConf.AutonomousModeState = pTxFull->AutoModeConf.AutonomousModeState;
  config_node.Config.AutoModeConf.TriggerSelection    = pTxFull->AutoModeConf.TriggerSelection;
  config_node.Config.AutoModeConf.TriggerPolarity     = pTxFull->AutoModeConf.TriggerPolarity;
  config_node.Config.AutoModeConf.DataSize            = pTxFull->AutoModeConf.DataSize;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build config packet size node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert config packet size node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## UART enable node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_UART_CONFIG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set UART configuration */
  config_node.Config.State             = ENABLE;
  config_node.Config.WakeupIT          = pTxFull->WakeupIT;

  /* Check UART mode */
  if (pTxFull->Mode == LPBAM_UART_MODE_TX)
  {
    config_node.Config.Mode = LPBAM_UART_MODE_TX;
  }
  else
  {
    config_node.Config.Mode = LPBAM_UART_MODE_TX_RX;
  }

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build config state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert config state node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;


  /*
   *               ######## UART Transmit data node ########
   */

  /* Get buffer address */
  buffer_address                       = (uint32_t)pTxFull->pData;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_UART_TRANSMIT_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Set UART configuration */
  config_node.Config.Size              = pTxFull->Size;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build Transmit data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert Transmit data node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the configuration of UART receive according to parameters in
  *         LPBAM_UART_ConfigAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a USART_TypeDef structure that selects UART instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pRxConfig    : [IN]  Pointer to a LPBAM_UART_ConfigAdvConf_t structure that contains receive configuration
  *                              nodes information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_UART_RxConfigDesc_t structure that contains receive configuration
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_UART_Receive_SetConfigQ(USART_TypeDef              *const pInstance,
                                                 LPBAM_DMAListInfo_t        const *const pDMAListInfo,
                                                 LPBAM_UART_ConfigAdvConf_t const *const pRxConfig,
                                                 LPBAM_UART_RxConfigDesc_t  *const pDescriptor,
                                                 DMA_QListTypeDef           *const pQueue)
{
  LPBAM_UART_ConfNode_t config_node;
  DMA_NodeConfTypeDef   dma_node_conf;

  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /*
   *               ######## UART clear flag node ########
   */

  /* Set UART instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_UART_CLEARFLAG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert clear flag node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /* Check UART mode */
  if (pRxConfig->Mode == LPBAM_UART_MODE_RX)
  {
    /*
     *               ######## UART Wake up Interrupt node ########
     */

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_UART_CONFIG_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

    /* Set UART configuration */
    config_node.Config.State             = ENABLE;
    config_node.Config.Mode              = LPBAM_UART_MODE_RX;
    config_node.Config.WakeupIT          = pRxConfig->WakeupIT;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build config interrupt node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert config interrupt node to UART queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the data of UART receive according to parameters in
  *         LPBAM_UART_DataAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a USART_TypeDef structure that selects UART instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pRxData      : [IN]  Pointer to a LPBAM_UART_DataAdvConf_t structure that contains receive data nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_UART_RxDataDesc_t structure that contains receive data descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_UART_Receive_SetDataQ(USART_TypeDef            *const pInstance,
                                               LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                               LPBAM_UART_DataAdvConf_t const *const pRxData,
                                               LPBAM_UART_RxDataDesc_t  *const pDescriptor,
                                               DMA_QListTypeDef         *const pQueue)
{
  LPBAM_UART_ConfNode_t config_node;
  DMA_NodeConfTypeDef   dma_node_conf;

  uint32_t node_idx = 0U;

  uint32_t buffer_address;

  /*
   *               ######## UART Receive data node ########
   */

  /* Set UART instance */
  config_node.pInstance                  = pInstance;

  /* Get period buffer address */
  buffer_address                         = (uint32_t)pRxData->pData;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_UART_RECEIVE_DATA_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pBuff             = (uint32_t *)buffer_address;

  /* Set UART configuration */
  config_node.Config.Size = pRxData->Size;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build receive data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert receive data node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the configuration and data of UART receive according to parameters in
  *         LPBAM_UART_FullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a USART_TypeDef structure that selects UART instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pRxFull      : [IN]  Pointer to a LPBAM_UART_FullAdvConf_t structure that contains receive full nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_UART_RxFullDesc_t structure that contains receive full descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_UART_Receive_SetFullQ(USART_TypeDef            *const pInstance,
                                               LPBAM_DMAListInfo_t      const *const pDMAListInfo,
                                               LPBAM_UART_FullAdvConf_t const *const pRxFull,
                                               LPBAM_UART_RxFullDesc_t  *const pDescriptor,
                                               DMA_QListTypeDef         *const pQueue)
{
  LPBAM_UART_ConfNode_t config_node;
  DMA_NodeConfTypeDef   dma_node_conf;

  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  uint32_t buffer_address;

  /*
   *               ######## UART clear flag node ########
   */

  /* Set UART instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_UART_CLEARFLAG_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert clear flag node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /* Check UART mode */
  if (pRxFull->Mode == LPBAM_UART_MODE_RX)
  {
    /*
     *               ######## UART Wake up Interrupt node ########
     */

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_UART_CONFIG_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

    /* Set UART configuration */
    config_node.Config.State             = ENABLE;
    config_node.Config.Mode              = LPBAM_UART_MODE_RX;
    config_node.Config.WakeupIT          = pRxFull->WakeupIT;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build config interrupt node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert config interrupt node to UART queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }
  }

  /* Update node index and register index */
  node_idx++;


  /*
   *               ######## UART Receive data node ########
   */

  /* Get period buffer address */
  buffer_address                       = (uint32_t)pRxFull->pData;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_UART_RECEIVE_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Set UART configuration */
  config_node.Config.Size              = pRxFull->Size;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_UART_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build receive data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert receive data node to UART queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Enable the UART transmit and receive DMA requests.
  * @param  pInstance    : [IN]  Pointer to a USART_TypeDef structure that selects UART instance.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_UART_EnableDMARequests(USART_TypeDef *const pInstance)
{
  /* Enable UART TX and Rx DMA requests */
  pInstance->CR3 |= (LPBAM_UART_DMAT | LPBAM_UART_DMAR);

  return LPBAM_OK;
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* LPBAM_UART_MODULE_ENABLED */
