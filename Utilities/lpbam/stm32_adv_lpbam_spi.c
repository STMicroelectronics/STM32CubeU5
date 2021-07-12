/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_spi.c
  * @author  MCD Application Team
  * @brief   This file provides the advanced LPBAM layer for SPI peripheral.
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
      It is strongly recommended to read carefully the GettingStarted.html document before starting developing an LPBAM
      application.

    *** Driver description ***
    ==========================
    [..]
      This advanced LPBAM module counts 3 files :
          (+) stm32_adv_lpbam_spi.c
              (++) This file provides the SPI advanced files body.
          (+) stm32_adv_lpbam_spi.h
              (++) This file is the header file of stm32_adv_lpbam_spi.c. It provides advanced used types.
          (+) stm32_platform_lpbam_spi.h
              (++) This header file contains all defines to be used in applicative side.

    *** Driver features ***
    =======================
    [..]
      This driver provides the following list of features :
          (+) Configure the SPI transfers in simplex and full duplex.
          (+) Starts the SPI data transfers in simplex and full duplex.
          (+) Configure and starts the SPI data transfers in simplex and full duplex.

    *** Functional description ***
    ==============================
    [..]
      The SPI peripheral transfer is configured through a DMA channel thanks to a build DMA linked-list queue in simplex
      and full duplex mode.
      The SPI peripheral transfer is started through a DMA channel thanks to a build DMA linked-list queue in simplex
      and full duplex mode.
      The SPI peripheral transfer is configured and started through a DMA channel thanks to a build DMA linked-list
      queue in simplex and full duplex mode.

      The output of this driver can be only one queue to be executed by one DMA channel for simplex operations and can
      be two queues to be executed by two DMA channel for full duplex operations.

    *** Driver APIs description ***
    ===============================
    [..]
      Use ADV_LPBAM_SPI_Transmit_SetConfigQ() API to build a linked-list queue that setup the SPI simplex configuration
      transmission operation according to parameters in the LPBAM_SPI_ConfigAdvConf_t structure.

    [..]
      Use ADV_LPBAM_SPI_Transmit_SetDataQ() API to build a linked-list queue that setup the SPI simplex transmission
      starting operation according to parameters in the LPBAM_SPI_DataAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_SPI_Transmit_SetFullQ() API to build a linked-list queue that setup the SPI simplex configuration
      and transmission starting operation according to parameters in the LPBAM_SPI_FullAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_SPI_Receive_SetConfigQ() API to build a linked-list queue that setup the SPI simplex configuration
      reception operation according to parameters in the LPBAM_SPI_ConfigAdvConf_t structure.

    [..]
      Use ADV_LPBAM_SPI_Receive_SetDataQ() API to build a linked-list queue that setup the SPI simplex reception
      starting operation according to parameters in the LPBAM_SPI_DataAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_SPI_Receive_SetFullQ() API to build a linked-list queue that setup the SPI simplex configuration
      and reception starting operation according to parameters in the LPBAM_SPI_FullAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_SPI_TransmitReceive_SetConfigQ() API to build a linked-list queue that setup the SPI full duplex
      configuration transmission and reception operation according to parameters in the LPBAM_SPI_ConfigAdvConf_t
      structure. The output of this API is one queue that must be executed by the DMA transmit channel.

    [..]
      Use ADV_LPBAM_SPI_TransmitReceive_SetDataQ() API to build a linked-list queue that setup the SPI full duplex
      transmission and reception starting operation according to parameters in the LPBAM_SPI_DataAdvConf_t structure.
      The output of this API is two queues that must be executed by two different DMA channel.
      The transmit data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)
      The receive data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_SPI_TransmitReceive_SetFullQ() API to build a linked-list queue that setup the SPI full duplex
      configuration, transmission and reception starting operation according to parameters in the
      LPBAM_SPI_FullAdvConf_t structure. The output of this API is two queues that must be executed by two different DMA
      channel.
      The transmit data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)
      The receive data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Configured parameters through LPBAM_SPI_ConfigAdvConf_t are :
          (+) CLKPolarity       : Specifies the serial clock steady state.
                                  This parameter can be a value of @ref SPI_Clock_Polarity.
          (+) CLKPhase          : Specifies the clock active edge for the bit capture.
                                  This parameter can be a value of @ref SPI_Clock_Phase.
          (+) FirstBit          : Specifies whether data transfers start from MSB or LSB bit.
                                  This parameter can be a value of @ref LPBAM_SPI_FirstBit.
          (+) BaudRatePrescaler : Specifies the Baud Rate prescaler value which will be used to configure the transmit
                                  and receive SCK clock.
                                  This parameter can be a value of @ref LPBAM_SPI_BaudRate.
          (+) DataSize          : Specifies the SPI data size.
                                  This parameter can be a value of @ref LPBAM_SPI_Data_Size.
          (+) AutoModeConf      : Specifies the autonomous mode configuration.
              (++) TriggerState     : Specifies the trigger state.
                                      This parameter can be a value of @ref LPBAM_SPI_AutonomousMode_State.
              (++) TriggerSelection : Specifies the autonomous mode trigger signal selection.
                                      This parameter can be a value of @ref LPBAM_SPI_AutonomousMode_TriggerSelection.
              (++) TriggerPolarity  : Specifies the autonomous mode trigger signal polarity sensitivity.
                                      This parameter can be a value of @ref LPBAM_SPI_AutonomousMode_TriggerPolarity
          (+) WakeupIT          : Specifies the wake up source interrupt.
                                  This parameter can be a value of @ref LPBAM_SPI_Wakeup_Interrupt.

      Configured parameters through LPBAM_SPI_DataAdvConf_t are :
          (+) AutoModeConf      : Specifies the autonomous mode configuration.
              (++) TriggerState     : Specifies the trigger state.
                                      This parameter can be a value of @ref LPBAM_SPI_TriggerState.
          (+) DataSize          : Specifies the SPI data size.
                                  This parameter can be a value of @ref LPBAM_SPI_Data_Size.
          (+) Size              : Specifies the element number.
          (+) pTxData           : Specifies the Tx transfer data buffer address (Used only for simplex transmission
                                  and full duplex operations.
          (+) pRxData           : Specifies the Rx transfer data buffer address (Used only for simplex reception
                                  and full duplex operations.

      Configured parameters through LPBAM_SPI_FullAdvConf_t are :
          (+) CLKPolarity       : Specifies the serial clock steady state.
                                  This parameter can be a value of @ref SPI_Clock_Polarity.
          (+) CLKPhase          : Specifies the clock active edge for the bit capture.
                                  This parameter can be a value of @ref SPI_Clock_Phase.
          (+) FirstBit          : Specifies whether data transfers start from MSB or LSB bit.
                                  This parameter can be a value of @ref LPBAM_SPI_FirstBit
          (+) BaudRatePrescaler : Specifies the Baud Rate prescaler value which will be used to configure the transmit
                                  and receive SCK clock.
                                  This parameter can be a value of @ref LPBAM_SPI_BaudRate_Prescaler.
          (+) DataSize          : Specifies the SPI data size.
                                  This parameter can be a value of @ref LPBAM_SPI_Data_Size.
          (+) AutoModeConf      : Specifies the autonomous mode configuration.
              (++) TriggerState     : Specifies the trigger state.
                                      This parameter can be a value of @ref LPBAM_SPI_AutonomousMode_State.
              (++) TriggerSelection : Specifies the autonomous mode trigger signal selection.
                                      This parameter can be a value of @ref LPBAM_SPI_AutonomousMode_TriggerSelection.
              (++) TriggerPolarity  : Specifies the autonomous mode trigger signal polarity sensitivity.
                                      This parameter can be a value of @ref LPBAM_SPI_AutonomousMode_TriggerPolarity
          (+) WakeupIT          : Specifies the wake up source interrupt.
                                  This parameter can be a value of @ref LPBAM_SPI_Wakeup_Interrupt.
          (+) Size              : Specifies the element number.
          (+) pTxData           : Specifies the Tx transfer data buffer address (Used only for simplex transmission
                                  and full duplex operations.
          (+) pRxData           : Specifies the Rx transfer data buffer address (Used only for simplex reception
                                  and full duplex operations.

    [..]
      These APIs must be called when the SPI is well initialized.
          (+) Recommended SPI initialization sequence
              (++) call HAL_SPI_Init to initialize the SPI. (Mandatory)
                   Initialization parameters can be :
                   (+++) Mode                  : SPI_MODE_MASTER.
                   (+++) Direction             : SPI_DIRECTION_2LINES.
                   (+++) DataSize              : SPI_DATASIZE_8BIT.
                   (+++) CLKPolarity           : SPI_POLARITY_LOW.
                   (+++) CLKPhase              : SPI_PHASE_1EDGE.
                   (+++) NSS                   : SPI_NSS_SOFT.
                   (+++) BaudRatePrescaler     : SPI_BAUDRATEPRESCALER_256.
                   (+++) FirstBit              : SPI_FIRSTBIT_MSB.
                   (+++) TIMode                : SPI_TIMODE_DISABLE.
                   (+++) CRCCalculation        : SPI_CRCCALCULATION_DISABLE.
                   (+++) NSSPMode              : SPI_NSS_PULSE_DISABLE.
                   (+++) MasterKeepIOState     : SPI_MASTER_KEEP_IO_STATE_ENABLE.
                   (+++) ReadyMasterManagement : SPI_RDY_MASTER_MANAGEMENT_INTERNALLY.
              (++) call HAL_SPIEx_SetConfigAutonomousMode to configure the autonomous mode. (Mandatory)
                   (+++) TriggerState     : Any.
                   (+++) TriggerSelection : Any.
                   (+++) TriggerPolarity  : Any.
              (++) Call __HAL_SPI_ENABLE_IT() to enable error interrupts.
                   SPI error interrupts can be :
                   (+++) SPI_IT_UDR  : underrun error.
                   (+++) SPI_IT_OVR  : overrun error.
          (+) Call ADV_LPBAM_SPI_EnableDMARequests() to enable SPI DMA requests. (Mandatory in case
                                                                                  ADV_LPBAM_SPI_{Operation}_SetConfigQ()
                                                                                  will not be called)

    [..]
      The following parameters must be configured carefully as they are not configurable by LPBAM APIs :
          (+) Mode                  : This parameter can be a value of @ref SPI_Mode.
          (+) NSS                   : This parameter can be a value of @ref SPI_Slave_Select_Management.
          (+) TIMode                : This parameter should be this value SPI_TIMODE_DISABLE.
          (+) CRCCalculation        : This parameter should be this value SPI_CRCCALCULATION_DISABLE.
          (+) ReadyMasterManagement : This parameter should be this value SPI_RDY_MASTER_MANAGEMENT_INTERNALLY.

    *** Driver user sequence ***
    ============================
    [..]
      Generic user sequence is :
          (+) Initialize the SPI (Using HAL/LL). (Mandatory)
          (+) There are two possibilities to call advanced API:
              (++) Transfer with full API :
                   (+++) For a transmission mode repeat calling ADV_LPBAM_SPI_Transmit_SetFullQ()
                         until complete LPBAM scenario. (Mandatory)
                   (+++) For a reception mode repeat calling ADV_LPBAM_I2C_MasterReceive_SetFullQ()
                         until complete LPBAM scenario. (Mandatory)
                   (+++) For a full duplex mode repeat calling ADV_LPBAM_SPI_TransmitReceive_SetFullQ()
                         until complete LPBAM scenario. (Mandatory)
              (++) Transfer with configuration and data APIs :
                   (+++) For a transmission mode call ADV_LPBAM_SPI_Transmit_SetConfigQ only one time
                         and repeat calling ADV_LPBAM_I2C_MasterTransmit_SetDataQ() until complete
                         LPBAM scenario. (Mandatory)
                   (+++) For a reception mode call ADV_LPBAM_SPI_Receive_SetConfigQ only one time
                         and repeat calling ADV_LPBAM_SPI_Receive_SetDataQ() until complete
                         LPBAM scenario. (Mandatory)
                   (+++) For a full duplex mode call ADV_LPBAM_SPI_TransmitReceive_SetConfigQ only one time
                         and repeat calling ADV_LPBAM_SPI_TransmitReceive_SetDataQ() until complete
                         LPBAM scenario. (Mandatory)
          (+) Call HAL_DMAEx_List_Init() to initialize the DMA in linked-list mode. (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the built queue to the DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() or HAL_DMAEx_List_Start_IT() to start DMA linked list. (Mandatory)

    [..]
      When the SPI configuration matches with application needs, it's strongly recommended to use
      ADV_LPBAM_SPI_{Operation}_SetDataQ() APIs instead of ADV_LPBAM_SPI_{Operation}_SetFullQ() for memory optimization
      purpose.
      Use ADV_LPBAM_SPI_{Operation}_SetFullQ() when SPI reconfiguration is needed instead of calling
      ADV_LPBAM_SPI_{Operation}_SetConfigQ() then ADV_LPBAM_SPI_{Operation}_SetDataQ() for memory optimization purpose.

    [..]
      When using ADV_LPBAM_SPI_TransmitReceive_SetConfigQ() ensure that the Tx queue is passed as parameter.

    *** Constraints ***
    ===================
    [..]
      It's mandatory to set properly DataSize, TriggerSelection and TriggerPolarity while configuring
      LPBAM_SPI_ConfigAdvConf_t or LPBAM_SPI_FullAdvConf_t because their update can be only done with
      ADV_LPBAM_SPI_{Operation}_SetConfigQ() or ADV_LPBAM_SPI_{Operation}_SetFullQ().

    [..]
      It's mandatory, while configuring LPBAM_SPI_DataAdvConf_t after calling ADV_LPBAM_SPI_{Operation}_SetConfigQ()
      or ADV_LPBAM_SPI_{Operation}_SetFullQ(), to attribute to DataSize the same value that it has already been set to
      it in LPBAM_SPI_ConfigAdvConf_t or LPBAM_SPI_FullAdvConf_t otherwise unexpected behavior can appear.

    [..]
      It's strongly not allowed to use the same DMA channel for transmission and reception when the SPI is
      used for different directions.

    [..]
      It's strongly not recommended to execute the same linked-list queue that contains spi configuration and data
      nodes by two different DMA channels as unexpected behavior can appear.

    [..]
      It's mandatory to use only one linked-list queue when spi is used in simplex duplex mode.

    [..]
      It's mandatory to use two linked-list queues (One for the configuration and the transmission
      and another one for the reception) which will be executed simultaneously when spi is used in full duplex mode.

    [..]
      It's mandatory in full duplex mode to start the Rx linked-list queue before starting the Tx one.

    [..]
      It's mandatory to be sure that the slave is enabled and ready to the communication before the master starts
      generating its clock otherwise an unexpected behavior can appear.

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

#ifdef LPBAM_SPI_MODULE_ENABLED

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup SPI_Advanced SPI_Advanced
  * @brief    SPI Advanced LPBAM module driver
  * @{
  */

/** @defgroup LPBAM_SPI_Advanced_Exported_Functions LPBAM SPI Advanced Exported Functions
  * @{
  */

/**
  * @brief  Build the SPI transmit configuration nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_SPI_ConfigAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a SPI_TypeDef structure that selects SPI instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxConfig    : [IN]  Pointer to a LPBAM_SPI_ConfigAdvConf_t structure that contains transmit configuration
  *                              nodes information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_SPI_ConfigTxDesc_t structure that contains transmit configuration
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_SPI_Transmit_SetConfigQ(SPI_TypeDef               *const pInstance,
                                                 LPBAM_DMAListInfo_t       const *const pDMAListInfo,
                                                 LPBAM_SPI_ConfigAdvConf_t const *const pTxConfig,
                                                 LPBAM_SPI_TxConfigDesc_t  *const pDescriptor,
                                                 DMA_QListTypeDef          *const pQueue)
{
  LPBAM_SPI_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_config;

  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /*
   *               ######## SPI disable node ########
   */

  /* Set SPI instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State               = DISABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configuration state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI clear flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert clear flag node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI frame configuration node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID  = (uint32_t)LPBAM_SPI_CONFIG_FRAME_ID;
  config_node.NodeDesc.pSrcVarReg       = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.BaudRatePrescaler  = pTxConfig->BaudRatePrescaler;
  config_node.Config.DataSize           = pTxConfig->DataSize;
  config_node.Config.Direction          = LPBAM_SPI_DIRECTION_2LINES_TXONLY;

  /* Fill node configuration */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build frame configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert frame configuration node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI configure communication node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CONFIG_COM_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.CLKPolarity       = pTxConfig->CLKPolarity;
  config_node.Config.CLKPhase          = pTxConfig->CLKPhase;
  config_node.Config.FirstBit          = pTxConfig->FirstBit;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configure Communication node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configure Communication node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI autonomous mode node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID             = (uint32_t)LPBAM_SPI_TRIG_ID;
  config_node.NodeDesc.pSrcVarReg                  = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.AutoModeConf.TriggerState     = pTxConfig->AutoModeConf.TriggerState;
  config_node.Config.AutoModeConf.TriggerSelection = pTxConfig->AutoModeConf.TriggerSelection;
  config_node.Config.AutoModeConf.TriggerPolarity  = pTxConfig->AutoModeConf.TriggerPolarity;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build autonomous mode node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert autonomous mode node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI wake up interrupt node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_WAKEUP_IT_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.WakeupIT          = pTxConfig->WakeupIT;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build interrupt state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert interrupt state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI enable node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State             = ENABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build enable instance node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert enable instance node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build the SPI transmit data nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_SPI_DataAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a SPI_TypeDef structure that selects SPI instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxData      : [IN]  Pointer to a LPBAM_SPI_DataAdvConf_t structure that contains transmit data nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_SPI_DataTxDesc_t structure that contains transmit data descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_SPI_Transmit_SetDataQ(SPI_TypeDef             *const pInstance,
                                               LPBAM_DMAListInfo_t     const *const pDMAListInfo,
                                               LPBAM_SPI_DataAdvConf_t const *const pTxData,
                                               LPBAM_SPI_TxDataDesc_t  *const pDescriptor,
                                               DMA_QListTypeDef        *const pQueue)
{
  LPBAM_SPI_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_config;

  uint32_t buffer_address;
  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /*
   *               ######## SPI disable node ########
   */

  /* Set SPI instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State               = DISABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configuration state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI set date size node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_DATASIZE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.Size              = pTxData->Size;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build set date size node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert set date size node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI clear flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert clear flag node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI enable node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State             = ENABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build enable instance node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert enable instance node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI start communication node ########
   */

  if (pTxData->AutoModeConf.TriggerState ==  LPBAM_SPI_AUTO_MODE_DISABLE)
  {
    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_START_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build start communication node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert start communication node to SPI queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }
  }

  /* Update node index */
  node_idx++;


  /*
   *               ######## SPI transmit data node ########
   */

  /* Get period buffer address */
  buffer_address                       = (uint32_t)pTxData->pTxData;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_TRANSMIT_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Set SPI configuration */
  config_node.Config.DataSize          = pTxData->DataSize;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build transmit data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert transmit data node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build the SPI transmit full nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_SPI_FullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a SPI_TypeDef structure that selects SPI instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxFull      : [IN]  Pointer to a LPBAM_SPI_FullAdvConf_t structure that contains transmit full nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_SPI_FullTxDesc_t structure that contains transmit full descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_SPI_Transmit_SetFullQ(SPI_TypeDef             *const pInstance,
                                               LPBAM_DMAListInfo_t     const *const pDMAListInfo,
                                               LPBAM_SPI_FullAdvConf_t const *const pTxFull,
                                               LPBAM_SPI_TxFullDesc_t  *const pDescriptor,
                                               DMA_QListTypeDef        *const pQueue)
{
  LPBAM_SPI_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_config;

  uint32_t buffer_address;
  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /*
   *               ######## SPI disable node ########
   */

  /* Set SPI instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State               = DISABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configuration state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI clear flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert clear flag node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI set date size node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_DATASIZE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.Size              = pTxFull->Size;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build set date size node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert set date size node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI frame configuration node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID  = (uint32_t)LPBAM_SPI_CONFIG_FRAME_ID;
  config_node.NodeDesc.pSrcVarReg       = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.BaudRatePrescaler  = pTxFull->BaudRatePrescaler;
  config_node.Config.DataSize           = pTxFull->DataSize;
  config_node.Config.Direction          = LPBAM_SPI_DIRECTION_2LINES_TXONLY;

  /* Fill node configuration */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build frame configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert frame configuration node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI configure communication node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CONFIG_COM_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.CLKPolarity       = pTxFull->CLKPolarity;
  config_node.Config.CLKPhase          = pTxFull->CLKPhase;
  config_node.Config.FirstBit          = pTxFull->FirstBit;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configure Communication node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configure Communication node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI autonomous mode node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID             = (uint32_t)LPBAM_SPI_TRIG_ID;
  config_node.NodeDesc.pSrcVarReg                  = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.AutoModeConf.TriggerState     = pTxFull->AutoModeConf.TriggerState;
  config_node.Config.AutoModeConf.TriggerSelection = pTxFull->AutoModeConf.TriggerSelection;
  config_node.Config.AutoModeConf.TriggerPolarity  = pTxFull->AutoModeConf.TriggerPolarity;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build autonomous mode node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert autonomous mode node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI wake up interrupt node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_WAKEUP_IT_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.WakeupIT          = pTxFull->WakeupIT;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build interrupt state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert interrupt state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI enable node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State             = ENABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build enable instance node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert enable instance node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI start communication node ########
   */

  if (config_node.Config.AutoModeConf.TriggerState ==  LPBAM_SPI_AUTO_MODE_DISABLE)
  {
    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_START_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build start communication node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert start communication node to SPI queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }
  }

  /* Update node index */
  node_idx++;


  /*
   *               ######## SPI transmit data node ########
   */

  /* Get period buffer address */
  buffer_address                       = (uint32_t)pTxFull->pTxData;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_TRANSMIT_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build transmit data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert transmit data node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build the SPI receive configuration nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_SPI_ConfigAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a SPI_TypeDef structure that selects SPI instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pRxConfig    : [IN]  Pointer to a LPBAM_SPI_ConfigAdvConf_t structure that contains receive configuration
  *                              nodes information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_SPI_ConfigRxDesc_t structure that contains receive configuration
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_SPI_Receive_SetConfigQ(SPI_TypeDef               *const pInstance,
                                                LPBAM_DMAListInfo_t       const *const pDMAListInfo,
                                                LPBAM_SPI_ConfigAdvConf_t const *const pRxConfig,
                                                LPBAM_SPI_RxConfigDesc_t  *const pDescriptor,
                                                DMA_QListTypeDef          *const pQueue)
{
  LPBAM_SPI_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_config;

  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /*
   *               ######## SPI disable node ########
   */

  /* Set SPI instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State               = DISABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configuration state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI clear flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert clear flag node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI frame configuration node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID  = (uint32_t)LPBAM_SPI_CONFIG_FRAME_ID;
  config_node.NodeDesc.pSrcVarReg       = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.BaudRatePrescaler  = pRxConfig->BaudRatePrescaler;
  config_node.Config.DataSize           = pRxConfig->DataSize;
  config_node.Config.Direction          = LPBAM_SPI_DIRECTION_2LINES_RXONLY;

  /* Fill node configuration */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build frame configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert frame configuration node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI configure communication node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CONFIG_COM_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.CLKPolarity       = pRxConfig->CLKPolarity;
  config_node.Config.CLKPhase          = pRxConfig->CLKPhase;
  config_node.Config.FirstBit          = pRxConfig->FirstBit;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configure Communication node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configure Communication node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI autonomous mode node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID             = (uint32_t)LPBAM_SPI_TRIG_ID;
  config_node.NodeDesc.pSrcVarReg                  = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.AutoModeConf.TriggerState     = pRxConfig->AutoModeConf.TriggerState;
  config_node.Config.AutoModeConf.TriggerSelection = pRxConfig->AutoModeConf.TriggerSelection;
  config_node.Config.AutoModeConf.TriggerPolarity  = pRxConfig->AutoModeConf.TriggerPolarity;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build autonomous mode node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert autonomous mode node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI wake up interrupt node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_WAKEUP_IT_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.WakeupIT          = pRxConfig->WakeupIT;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build interrupt state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert interrupt state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI enable node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State             = ENABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build enable instance node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert enable instance node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build the SPI receive data nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_SPI_DataAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a SPI_TypeDef structure that selects SPI instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pRxData      : [IN]  Pointer to a LPBAM_SPI_DataAdvConf_t structure that contains receive data nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_SPI_DataRxDesc_t structure that contains receive data descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_SPI_Receive_SetDataQ(SPI_TypeDef             *const pInstance,
                                              LPBAM_DMAListInfo_t     const *const pDMAListInfo,
                                              LPBAM_SPI_DataAdvConf_t const *const pRxData,
                                              LPBAM_SPI_RxDataDesc_t  *const pDescriptor,
                                              DMA_QListTypeDef        *const pQueue)
{
  LPBAM_SPI_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_config;

  uint32_t buffer_address;
  uint32_t node_idx = 0UL;
  uint32_t reg_idx  = 0UL;

  /*
   *               ######## SPI disable node ########
   */

  /* Set SPI instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State               = DISABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configuration state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI set date size node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_DATASIZE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.Size              = pRxData->Size;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build set date size node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert set date size node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI clear flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert clear flag node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI enable node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State             = ENABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build enable instance node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert enable instance node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI start communication node ########
   */

  if (pRxData->AutoModeConf.TriggerState ==  LPBAM_SPI_AUTO_MODE_DISABLE)
  {
    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_START_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build start communication node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert start communication node to SPI queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }
  }

  /* Update node index */
  node_idx++;


  /*
   *               ######## SPI receive data node ########
   */

  /* Get period buffer address */
  buffer_address                       = (uint32_t)pRxData->pRxData;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_RECEIVE_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Set specific parameters for node creation */
  config_node.Config.DataSize          = pRxData->DataSize;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build receive data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert receive data node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build the SPI receive full nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_SPI_FullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a SPI_TypeDef structure that selects SPI instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pRxFull      : [IN]  Pointer to a LPBAM_SPI_FullAdvConf_t structure that contains receive full nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_SPI_FullRxDesc_t structure that contains receive full descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_SPI_Receive_SetFullQ(SPI_TypeDef             *const pInstance,
                                              LPBAM_DMAListInfo_t     const *const pDMAListInfo,
                                              LPBAM_SPI_FullAdvConf_t const *const pRxFull,
                                              LPBAM_SPI_RxFullDesc_t  *const pDescriptor,
                                              DMA_QListTypeDef        *const pQueue)
{
  LPBAM_SPI_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_config;

  uint32_t buffer_address;
  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /*
   *               ######## SPI disable node ########
   */

  /* Set SPI instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State               = DISABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configuration state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI clear flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert clear flag node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI set date size node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_DATASIZE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.Size              = pRxFull->Size;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build set date size node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert set date size node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI frame configuration node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CONFIG_FRAME_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.BaudRatePrescaler = pRxFull->BaudRatePrescaler;
  config_node.Config.DataSize          = pRxFull->DataSize;
  config_node.Config.Direction         = LPBAM_SPI_DIRECTION_2LINES_RXONLY;

  /* Fill node configuration */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build frame configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert frame configuration node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI configure communication node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CONFIG_COM_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.CLKPolarity       = pRxFull->CLKPolarity;
  config_node.Config.CLKPhase          = pRxFull->CLKPhase;
  config_node.Config.FirstBit          = pRxFull->FirstBit;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configure Communication node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configure Communication node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI autonomous mode node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID             = (uint32_t)LPBAM_SPI_TRIG_ID;
  config_node.NodeDesc.pSrcVarReg                  = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.AutoModeConf.TriggerState     = pRxFull->AutoModeConf.TriggerState;
  config_node.Config.AutoModeConf.TriggerSelection = pRxFull->AutoModeConf.TriggerSelection;
  config_node.Config.AutoModeConf.TriggerPolarity  = pRxFull->AutoModeConf.TriggerPolarity;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build autonomous mode node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert autonomous mode node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI wake up interrupt node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_WAKEUP_IT_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.WakeupIT          = pRxFull->WakeupIT;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build interrupt state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert interrupt state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI enable node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State             = ENABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build enable instance node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert enable instance node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI start communication node ########
   */

  if (config_node.Config.AutoModeConf.TriggerState == LPBAM_SPI_AUTO_MODE_DISABLE)
  {
    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_START_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build start communication node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert start communication node to SPI queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }
  }

  /* Update node index */
  node_idx++;


  /*
   *               ######## SPI transmit data node ########
   */

  /* Get period buffer address */
  buffer_address                       = (uint32_t)pRxFull->pRxData;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_RECEIVE_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build transmit data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert transmit data node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build the SPI transmit and receive configuration nodes in DMA linked-list queue according to configured
  *         parameters in LPBAM_SPI_ConfigAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a SPI_TypeDef structure that selects SPI instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxRxConfig  : [IN]  Pointer to a LPBAM_SPI_ConfigAdvConf_t structure that contains transmit and receive
  *                              configuration nodes information.
  * @param  pDescriptor   : [IN]  Pointer to a LPBAM_SPI_ConfigTxRxDesc_t structure that contains transmit and receive
  *                              configuration descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains transmit and receive
  *                              configuration nodes DMA linked-list queue information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_SPI_TransmitReceive_SetConfigQ(SPI_TypeDef                 *const pInstance,
                                                        LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                                        LPBAM_SPI_ConfigAdvConf_t   const *const pTxRxConfig,
                                                        LPBAM_SPI_TxRx_ConfigDesc_t *const pDescriptor,
                                                        DMA_QListTypeDef            *const pQueue)
{
  LPBAM_SPI_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_config;

  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /*
   *               ######## SPI disable node ########
   */

  /* Set SPI instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State               = DISABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configuration state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI clear flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert clear flag node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI frame configuration node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CONFIG_FRAME_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.BaudRatePrescaler = pTxRxConfig->BaudRatePrescaler;
  config_node.Config.DataSize          = pTxRxConfig->DataSize;
  config_node.Config.Direction         = LPBAM_SPI_DIRECTION_2LINES;

  /* Fill node configuration */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build frame configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert frame configuration node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI configure communication node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CONFIG_COM_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.CLKPolarity       = pTxRxConfig->CLKPolarity;
  config_node.Config.CLKPhase          = pTxRxConfig->CLKPhase;
  config_node.Config.FirstBit          = pTxRxConfig->FirstBit;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configure Communication node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configure Communication node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI autonomous mode node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID             = (uint32_t)LPBAM_SPI_TRIG_ID;
  config_node.NodeDesc.pSrcVarReg                  = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.AutoModeConf.TriggerState     = pTxRxConfig->AutoModeConf.TriggerState;
  config_node.Config.AutoModeConf.TriggerSelection = pTxRxConfig->AutoModeConf.TriggerSelection;
  config_node.Config.AutoModeConf.TriggerPolarity  = pTxRxConfig->AutoModeConf.TriggerPolarity;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build autonomous mode node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert autonomous mode node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI wake up interrupt node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_WAKEUP_IT_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.WakeupIT          = pTxRxConfig->WakeupIT;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build interrupt state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert interrupt state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI enable node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State             = ENABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build enable instance node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert enable instance node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build the SPI transmit and receive data nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_SPI_DataAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a SPI_TypeDef structure that selects SPI instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxRxData    : [IN]  Pointer to a LPBAM_SPI_DataAdvConf_t structure that contains transmit and receive data
  *                              nodes information.
  * @param  pRxDescriptor: [IN]  Pointer to a LPBAM_SPI_TxRx_RxDataDesc_t structure that contains transmit and receive
  *                              data descriptor information.
  * @param  pTxDescriptor: [IN]  Pointer to a LPBAM_SPI_TxRx_TxDataDesc_t structure that contains transmit and receive
  *                              data descriptor information.
  * @param  pTxQueue     : [OUT] Pointer to a DMA_QListTypeDef structure that contains transmit data nodes DMA
  *                              linked-list queue information.
  * @param  pRxQueue     : [OUT] Pointer to a DMA_QListTypeDef structure that contains receive data nodes DMA
  *                              linked-list queue information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_SPI_TransmitReceive_SetDataQ(SPI_TypeDef                 *const pInstance,
                                                      LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                                      LPBAM_SPI_DataAdvConf_t     const *const pTxRxData,
                                                      LPBAM_SPI_TxRx_TxDataDesc_t *const pTxDescriptor,
                                                      LPBAM_SPI_TxRx_RxDataDesc_t *const pRxDescriptor,
                                                      DMA_QListTypeDef            *const pTxQueue,
                                                      DMA_QListTypeDef            *const pRxQueue)
{
  LPBAM_SPI_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_config;

  uint32_t buffer_address;
  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /*
   *               ######## SPI disable node ########
   */

  /* Set SPI instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pTxDescriptor->pTxReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State               = DISABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configuration state node to SPI Tx queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI clear flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pTxDescriptor->pTxReg[reg_idx];

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert clear flag node to SPI Tx queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI set date size node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_DATASIZE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pTxDescriptor->pTxReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.Size              = pTxRxData->Size;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build set date size node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert set date size node to SPI Tx queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI enable node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pTxDescriptor->pTxReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State             = ENABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build enable instance node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert enable instance node to SPI Tx queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI start communication node ########
   */

  if (pTxRxData->AutoModeConf.TriggerState == LPBAM_SPI_AUTO_MODE_DISABLE)
  {
    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_START_ID;
    config_node.NodeDesc.pSrcVarReg      = &pTxDescriptor->pTxReg[reg_idx];

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build start communication node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert start communication node to SPI Tx queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }
  }

  /* Update node index */
  node_idx++;


  /*
   *               ######## SPI transmit data node ########
   */

  /* Get period buffer address */
  buffer_address                       = (uint32_t)pTxRxData->pTxData;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_TRANSMIT_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Set SPI configuration */
  config_node.Config.DataSize          = pTxRxData->DataSize;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build transmit data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert transmit data node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }


  /*
   *               ######## SPI receive data node ########
   */

  /* Get period buffer address */
  buffer_address                       = (uint32_t)pTxRxData->pRxData;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_RECEIVE_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build receive data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pRxDescriptor->pRxNodes[0]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert receive data node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pRxQueue, &pRxDescriptor->pRxNodes[0]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build the SPI transmit and receive full nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_SPI_FullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a SPI_TypeDef structure that selects SPI instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxRxFull    : [IN]  Pointer to a LPBAM_SPI_FullAdvConf_t structure that contains transmit and receive full
  *                              nodes information.
  * @param  pRxDescriptor: [IN]  Pointer to a LPBAM_SPI_TxRx_RxFullDesc_t structure that contains transmit and receive
  *                              data descriptor information.
  * @param  pTxDescriptor: [IN]  Pointer to a LPBAM_SPI_TxRx_TxFullDesc_t structure that contains transmit and receive
  *                              data descriptor information.
  * @param  pTxQueue     : [OUT] Pointer to a DMA_QListTypeDef structure that contains transmit full nodes DMA
  *                              linked-list queue information.
  * @param  pRxQueue     : [OUT] Pointer to a DMA_QListTypeDef structure that contains receive full nodes DMA
  *                              linked-list queue information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_SPI_TransmitReceive_SetFullQ(SPI_TypeDef                 *const pInstance,
                                                      LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                                      LPBAM_SPI_FullAdvConf_t     const *const pTxRxFull,
                                                      LPBAM_SPI_TxRx_TxFullDesc_t *const pTxDescriptor,
                                                      LPBAM_SPI_TxRx_RxFullDesc_t *const pRxDescriptor,
                                                      DMA_QListTypeDef            *const pTxQueue,
                                                      DMA_QListTypeDef            *const pRxQueue)
{
  LPBAM_SPI_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_config;

  uint32_t buffer_address;
  uint32_t node_idx = 0U;
  uint32_t reg_idx  = 0U;

  /*
   *               ######## SPI disable node ########
   */

  /* Set SPI instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pTxDescriptor->pTxReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State               = DISABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configuration state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI clear flag node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CLEARFLAG_ID;
  config_node.NodeDesc.pSrcVarReg      = &pTxDescriptor->pTxReg[reg_idx];

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build clear flag node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert clear flag node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI set date size node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_DATASIZE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pTxDescriptor->pTxReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.Size              = pTxRxFull->Size;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build set date size node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert set date size node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI frame configuration node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CONFIG_FRAME_ID;
  config_node.NodeDesc.pSrcVarReg      = &pTxDescriptor->pTxReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.BaudRatePrescaler = pTxRxFull->BaudRatePrescaler;
  config_node.Config.DataSize          = pTxRxFull->DataSize;
  config_node.Config.Direction         = LPBAM_SPI_DIRECTION_2LINES;

  /* Fill node configuration */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build frame configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert frame configuration node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI configure communication node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_CONFIG_COM_ID;
  config_node.NodeDesc.pSrcVarReg      = &pTxDescriptor->pTxReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.CLKPolarity       = pTxRxFull->CLKPolarity;
  config_node.Config.CLKPhase          = pTxRxFull->CLKPhase;
  config_node.Config.FirstBit          = pTxRxFull->FirstBit;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configure Communication node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configure Communication node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI autonomous mode node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID             = (uint32_t)LPBAM_SPI_TRIG_ID;
  config_node.NodeDesc.pSrcVarReg                  = &pTxDescriptor->pTxReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.AutoModeConf.TriggerState     = pTxRxFull->AutoModeConf.TriggerState;
  config_node.Config.AutoModeConf.TriggerSelection = pTxRxFull->AutoModeConf.TriggerSelection;
  config_node.Config.AutoModeConf.TriggerPolarity  = pTxRxFull->AutoModeConf.TriggerPolarity;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build autonomous mode node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert autonomous mode node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI wake up interrupt node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_WAKEUP_IT_ID;
  config_node.NodeDesc.pSrcVarReg      = &pTxDescriptor->pTxReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.WakeupIT          = pTxRxFull->WakeupIT;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build interrupt state node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert interrupt state node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI enable node ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pTxDescriptor->pTxReg[reg_idx];

  /* Set specific parameters for node creation */
  config_node.Config.State             = ENABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build enable instance node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert enable instance node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index and register index */
  node_idx++;
  reg_idx++;


  /*
   *               ######## SPI start communication node ########
   */

  if (config_node.Config.AutoModeConf.TriggerState == LPBAM_SPI_AUTO_MODE_DISABLE)
  {
    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_START_ID;
    config_node.NodeDesc.pSrcVarReg      = &pTxDescriptor->pTxReg[reg_idx];

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build start communication node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert start communication node to SPI Tx queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }
  }

  /* Update node index */
  node_idx++;


  /*
   *               ######## SPI transmit data node ########
   */

  /* Get period buffer address */
  buffer_address                       = (uint32_t)pTxRxFull->pTxData;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_TRANSMIT_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build transmit data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert transmit data node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pTxQueue, &pTxDescriptor->pTxNodes[node_idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }


  /*
   *               ######## SPI receive data node ########
   */

  /* Get period buffer address */
  buffer_address                       = (uint32_t)pTxRxFull->pRxData;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_SPI_RECEIVE_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_SPI_FillNodeConfig(&config_node, &dma_node_config) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build receive data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_config, &pRxDescriptor->pRxNodes[0]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert receive data node to SPI queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pRxQueue, &pRxDescriptor->pRxNodes[0]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Enable the SPI DMA requests.
  * @param  pInstance    : [IN]  Pointer to a SPI_TypeDef structure that selects SPI instance.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_SPI_EnableDMARequests(SPI_TypeDef *const pInstance)
{
  /* Enable SPI DMA requests */
  pInstance->CFG1 |= LPBAM_SPI_TXDMAEN | LPBAM_SPI_RXDMAEN;

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
#endif /* LPBAM_SPI_MODULE_ENABLED */
