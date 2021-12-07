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
      It is strongly recommended to read carefully the LPBAM_Utility_GettingStarted.html document before starting
      developing an LPBAM application.

    *** Driver description ***
    ==========================
    [..]
      This driver is dedicated for UART that can change configuration and operate in low power mode.
      This advanced LPBAM module counts 3 files :
          (+) stm32_adv_lpbam_uart.c
              (++) This file provides the UART advanced files body.
          (+) stm32_adv_lpbam_uart.h
              (++) This file is the header file of stm32_adv_lpbam_uart.c. It provides advanced used types.
          (+) stm32_platform_lpbam_uart.h
              (++) This header file contains all defines to be used in applicative side.

    *** Driver features ***
    =======================
    [..]
      This driver provides the following list of features :
          (+) Configure the UART transfers in simplex and full duplex modes.
          (+) Starts the UART data transfers in simplex and full duplex modes.
          (+) Configure and starts the UART data transfers in simplex and full duplex modes.

    *** Functional description ***
    ==============================
    [..]
      The UART peripheral is configured and started through a DMA channel thanks to a built DMA linked-list queue in
      simplex and full duplex mode.
      The UART peripheral transfer is started through a DMA channel thanks to a build DMA linked-list queue in simplex
      and full duplex mode.
      The UART peripheral transfer is configured and started through a DMA channel thanks to a build DMA linked-list
      queue in simplex and full duplex mode.

      The output of each API is one linked-list queue. For simplex transfer, it is performed by executing one API
      (Transmit or receive) according application needs. For the full duplex transfer, it is performed by executing two
      APIs (one Transmit and one receive) then link each one to different DMA channel. (Please refer to Driver APIs
      description and user sequence section for more information).

    *** Driver APIs description ***
    ===============================
    [..]
      Use ADV_LPBAM_UART_Transmit_SetConfigQ() API to build a linked-list queue that setup the UART transmission
      configuration operation according to parameters in the LPBAM_UART_ConfigAdvConf_t structure.
      Configuration parameters are :
          (+) Mode         : Specifies the UART mode.
                             This parameter can be a value of @ref LPBAM_UART_Mode.
          (+) AutoModeConf : Specifies the autonomous mode parameters.
                             (++) AutonomousModeState : Specifies the autonomous mode state.
                                                        This parameter can be a value
                                                        of @ref LPBAM_UART_AutonomousMode_State.
                             (++) TriggerSelection    : Specifies the autonomous mode trigger signal selection.
                                                        This parameter can be a value
                                                        of @ref LPBAM_UART_AutonomousMode_TriggerSelection.
                             (++) TriggerPolarity     : Specifies the autonomous mode trigger signal polarity
                                                        sensitivity.
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
      starting according to parameters in the LPBAM_UART_DataAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

      Configuration parameters are :
          (+) pData : Specifies the data source buffer.
          (+) Size  : Specifies the data number of bytes to be sent.

    [..]
      Use ADV_LPBAM_UART_Transmit_SetFullQ() API to build a linked-list queue that setup the UART configuration and
      transmission starting according to parameters in the LPBAM_UART_FullAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

      Configuration parameters are :
          (+) Mode         : Specifies the UART mode.
                             This parameter can be a value of @ref LPBAM_UART_Mode.
          (+) AutoModeConf : Specifies the autonomous mode parameters.
                             (++) AutonomousModeState : Specifies the autonomous mode state.
                                                        This parameter can be a value
                                                        of @ref LPBAM_UART_AutonomousMode_State.
                             (++) TriggerSelection    : Specifies the autonomous mode trigger signal selection.
                                                        This parameter can be a value
                                                        of @ref LPBAM_UART_AutonomousMode_TriggerSelection.
                             (++) TriggerPolarity     : Specifies the autonomous mode trigger signal polarity
                                                        sensitivity.
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

    [..]
      Use ADV_LPBAM_UART_Receive_SetConfigQ() API to build a linked-list queue that setup the UART reception
      configuration operation according to parameters in the LPBAM_UART_ConfigAdvConf_t structure.
      Configuration parameters are :
          (+) Mode     : Specifies the UART mode.
                         This parameter can be a value of @ref LPBAM_UART_Mode.
          (+) WakeupIT : Specifies the wake up source.
                         UART wake up IT can be:
                         (++) NONE  : UART wake up interrupt none.
                         (++) RXFNE : UART RX FIFO not empty interrupt.
                         (++) RXFF  : UART RX FIFO full interrupt.

    [..]
      Use ADV_LPBAM_UART_Receive_SetDataQ() API to build a linked-list queue that setup the UART reception operation
      starting according to parameters in the LPBAM_UART_DataAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

      Configuration parameters are :
          (+) pData : Specifies the data source buffer.
          (+) Size  : Specifies the data number of bytes to be sent.

    [..]
      Use ADV_LPBAM_UART_Receive_SetFullQ() API to build a linked-list queue that setup the UART configuration and
      reception starting according to parameters in the LPBAM_UART_FullAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

      Configuration parameters are :
          (+) Mode     : Specifies the UART mode.
                         This parameter can be a value of @ref LPBAM_UART_Mode.
          (+) WakeupIT : Specifies the wake up source.
                         UART wake up IT can be:
                         (++) NONE  : UART wake up interrupt none.
                         (++) RXFNE : UART RX FIFO not empty interrupt.
                         (++) RXFF  : UART RX FIFO full interrupt.
          (+) pData    : Specifies the data source buffer.
          (+) Size     : Specifies the data number of bytes to be sent.

      These APIs must be called when the UART is well initialized.
           (+) Recommended UART initialization sequence
              (++) Call HAL_UART_Init() to initialize the UART. (Mandatory)
                   Initialization parameters can be :
                   (+++) BaudRate       : Any.
                   (+++) WordLength     : Any.
                   (+++) StopBits       : Any.
                   (+++) Parity         : Any.
                   (+++) HwFlowCtl      : UART_HWCONTROL_NONE.
                   (+++) Mode           : Any.
                   (+++) ClockPrescaler : Any.
                   (+++) AdvFeatureInit : UART_ADVFEATURE_NO_INIT.
              (++) Call HAL_UARTEx_SetConfigAutonomousMode() to configure the autonomous mode.(Mandatory only for
                   transmission)
                   Autonomous mode parameters can be :
                   (+++) AutonomousModeState : UART_AUTONOMOUS_MODE_ENABLE.
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
      Simplex user sequence :
          (+) Initialize the UART (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_UART_EnableDMARequests() to enable the UART DMA requests. (Mandatory)
          (+) Repeat calling ADV_LPBAM_UART_{Transfer}_SetConfigQ() and/or ADV_LPBAM_UART_{Transfer}_SetDataQ() and/or
              ADV_LPBAM_UART_{Transfer}_SetFullQ until completing LPBAM scenario. (Mandatory)
          (+) Call ADV_LPBAM_Q_SetTriggerConfig() to add hardware trigger condition for executing output
              queues. (Optional)
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call ADV_LPBAM_Q_SetCircularMode() to circularize your linked-list queue for infinite scenarios
              cases. (Optional)
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode (Using HAL/LL). (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)

    [..]
      Full duplex user sequence :
          (+) Initialize the UART (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_UART_EnableDMARequests() to enable the UART DMA requests. (Mandatory)
          (+) Repeat calling ADV_LPBAM_UART_Receive_SetConfigQ() and/or ADV_LPBAM_UART_Receive_SetDataQ() and/or
              ADV_LPBAM_UART_Receive_SetFullQ until completing LPBAM scenario. (Mandatory)
          (+) Repeat calling ADV_LPBAM_UART_Transmit_SetConfigQ() and/or ADV_LPBAM_UART_Transmit_SetDataQ() and/or
              ADV_LPBAM_UART_Transmit_SetFullQ until completing LPBAM scenario. (Mandatory)
          (+) Call ADV_LPBAM_Q_SetTriggerConfig() to add hardware trigger condition for executing output
              queues. (Optional)
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call ADV_LPBAM_Q_SetCircularMode() to circularize your linked-list queue for infinite scenarios
              cases. (Optional)
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call HAL_DMAEx_List_Init() to initialize the receive DMA channel in linked-list
              mode (Using HAL/LL). (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the receive output queue to the initialized
              receive DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the receive DMA channel linked-list execution. (Mandatory)
          (+) Call HAL_DMAEx_List_Init() to initialize the transmit DMA channel in linked-list
              mode (Using HAL/LL). (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the transmit output queue to the initialized
              transmit DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the transmit DMA channel linked-list execution. (Mandatory)


    *** Recommendation ***
    ======================
    [..]
      It's useless to call ADV_LPBAM_UART_{Transfer}_SetConfigQ() consecutively as the first configuration will be
      overwritten.

    [..]
      It's useless to call ADV_LPBAM_UART_{Transfer}_SetConfigQ() before ADV_LPBAM_UART_{Transfer}_SetFullQ() as
      ADV_LPBAM_UART_{Transfer}_SetFullQ() APIs contains configuration process.

    [..]
      It's mandatory to starts the receive DMA channel before the transmit DMA channel in full duplex mode.

    [..]
      It's strongly not recommended to execute the same linked-list queue that contains UART transfers configuration
      by two different DMA channels as unexpected behavior can appear.

    [..]
      It's strongly not recommended to call the UART advanced APIs with the same instance by more than
      one linked-list queue. When the UART nodes will be executed simultaneously unexpected behavior will appear.

    [..]
      It's mandatory to use two DMA channels for full duplex transfers and only one DMA channels for simplex transfers.

    *** Driver status description ***
    =================================
    [..]
      This driver detects and reports any detected issue.
          (+) returned values are :
              (++) LPBAM_OK when no error is detected.
              (++) LPBAM_ERROR when error is detected.
              (++) LPBAM_INVALID_ID when an invalid node ID is detected. This error value is specific for LPBAM basic
                   layer.

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
  * @brief  Build the UART transmit configuration nodes in DMA linked-list queue according to configured parameters in
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
  * @brief  Build the UART transmit data nodes in DMA linked-list queue according to configured parameters in
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
  * @brief  Build the UART transmit full nodes in DMA linked-list queue according to configured parameters in
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
  * @brief  Build the UART receive configuration nodes in DMA linked-list queue according to configured parameters in
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
  * @brief  Build the UART receive data nodes in DMA linked-list queue according to configured parameters in
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
  * @brief  Build the UART receive full nodes in DMA linked-list queue according to configured parameters in
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
  * @brief  Enable the uart TX and Rx DMA requests.
  * @param  pInstance    : [IN]  Pointer to a USART_TypeDef structure that selects UART instance.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_UART_EnableDMARequests(USART_TypeDef *const pInstance)
{
  /* Enable UART TX/Rx DMA requests */
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
