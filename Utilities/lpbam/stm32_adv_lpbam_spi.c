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
      It is recommended to read the LPBAM_Utility_GettingStarted.html document, available at the root of LPBAM utility
      folder, prior to any LPBAM application development start.

    *** Driver description ***
    ==========================
    [..]
      This section provide description of the driver files content (refer to LPBAM_Utility_GettingStarted.html document
      for more information)

    [..]
      It is composed of 3 files :
          (+) stm32_adv_lpbam_spi.c file
              (++) This file provides the implementation of the advanced LPBAM SPI functions.
          (+) stm32_adv_lpbam_spi.h file
              (++) This file is the header file of stm32_adv_lpbam_spi.c. It provides advanced LPBAM SPI functions
                   prototypes and the declaration of their needed exported types and structures.
          (+) STM32xx/stm32_platform_lpbam_spi.h file
              (++) This header file contains all defines to be used on applicative side.
                   (+++) STM32xx stands for the device supporting LPBAM sub-system.

    *** Driver functions model ***
    ==============================
    [..]
      This section precises this module supported advanced functions model (refer to LPBAM_Utility_GettingStarted.html
      document for function model definition).

    [..]
      An advanced functionality in this module can be inserted using 3 main types of functions :
          (+) ADV_LPBAM_{Peripheral}_{Operation}_SetConfigQ() : Inserts a {Peripheral} functional configuration for the
                                                                selected {Operation} in a queue.
          (+) ADV_LPBAM_{Peripheral}_{Operation}_SetDataQ()   : Inserts a {Peripheral} data transfer configuration for
                                                                the selected {Operation} in a queue.
          (+) ADV_LPBAM_{Peripheral}_{Operation}_SetFullQ()   : Inserts a {Peripheral} functional and data transfer
                                                                configuration for the selected {Operation} in a queue.

    *** Driver features ***
    =======================
    [..]
      This section describes this LPBAM module supported features.

    [..]
      This driver provides services covering the LPBAM management of the following SPI features :
          (+) SPI peripheral functional configuration:
              (++) simplex transmit: ADV_LPBAM_SPI_Tx_SetConfigQ;
              (++) simplex receive: ADV_LPBAM_SPI_Rx_SetConfigQ;
              (++) full duplex: ADV_LPBAM_SPI_TxRx_SetConfigQ.
          (+) SPI data transfers configuration:
              (++) simplex transmit: ADV_LPBAM_SPI_Tx_SetDataQ;
              (++) simplex receive: ADV_LPBAM_SPI_Rx_SetDataQ;
              (++) full duplex: ADV_LPBAM_SPI_TxRx_SetDataQ.
          (+) SPI peripheral and data transfers configuration:
              (++) simplex transmit: ADV_LPBAM_SPI_Tx_SetFullQ;
              (++) simplex receive: ADV_LPBAM_SPI_Rx_SetFullQ;
              (++) full duplex: ADV_LPBAM_SPI_TxRx_SetFullQ
          (+) Warning(s):
              (++) The half-duplex is not supported by this driver.
              (++) The SPI Master/Slave operating mode (configured at the initialization phase) cannot be modified by
                   this driver.

    *** Functional description ***
    ==============================
    [..]
      This section describes the peripheral features covered by this LPBAM module.

    [..]
      The output of this driver can be :
          (+) For simplex operations, only one queue to be executed by a DMA channel.
          (+) For full duplex operations, two queues to be executed by two DMA channels.

      The SPI peripheral transfer is configured in simplex or full duplex mode.
      The SPI peripheral transfer is started in simplex or full duplex mode.
      The SPI peripheral transfer is configured and started in simplex or full duplex mode.

    *** Driver APIs description ***
    ===============================
    [..]
      This section provides LPBAM module exhaustive APIs description without considering application user call sequence.
      For user call sequence information, please refer to 'Driver user sequence' section below.

    [..]
      Use ADV_LPBAM_SPI_Tx_SetConfigQ() API to build a linked-list queue that setup the SPI simplex configuration
      transmission operation according to parameters in the LPBAM_SPI_ConfigAdvConf_t structure.

    [..]
      Use ADV_LPBAM_SPI_Tx_SetDataQ() API to build a linked-list queue that setup the SPI simplex transmission
      starting operation according to parameters in the LPBAM_SPI_DataAdvConf_t structure.
      The data transfers default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)

    [..]
      Use ADV_LPBAM_SPI_Tx_SetFullQ() API to build a linked-list queue that setup the SPI simplex configuration and
      transmission starting operation according to parameters in the LPBAM_SPI_FullAdvConf_t structure.
      The data transfers default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)

    [..]
      Use ADV_LPBAM_SPI_Rx_SetConfigQ() API to build a linked-list queue that setup the SPI simplex configuration
      reception operation according to parameters in the LPBAM_SPI_ConfigAdvConf_t structure.

    [..]
      Use ADV_LPBAM_SPI_Rx_SetDataQ() API to build a linked-list queue that setup the SPI simplex reception starting
      operation according to parameters in the LPBAM_SPI_DataAdvConf_t structure.
      The data transfers default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)

    [..]
      Use ADV_LPBAM_SPI_Rx_SetFullQ() API to build a linked-list queue that setup the SPI simplex configuration and
      reception starting operation according to parameters in the LPBAM_SPI_FullAdvConf_t structure.
      The data transfers default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)

    [..]
      Use ADV_LPBAM_SPI_TxRx_SetConfigQ() API to build a linked-list queue that setup the SPI full duplex configuration
      transmission and reception operation according to parameters in the LPBAM_SPI_ConfigAdvConf_t structure.
      The output of this API is one queue that must be executed by the DMA transmit channel.

    [..]
      Use ADV_LPBAM_SPI_TxRx_SetDataQ() API to build a linked-list queue that setup the SPI full duplex
      transmission and reception starting operation according to parameters in the LPBAM_SPI_DataAdvConf_t structure.
      The output of this API is two queues that must be executed by two different DMA channel.
      The transmit data transfers default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The receive data transfers default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)

    [..]
      Use ADV_LPBAM_SPI_TxRx_SetFullQ() API to build a linked-list queue that setup the SPI full duplex configuration,
      transmission and reception starting operation according to parameters in the LPBAM_SPI_FullAdvConf_t structure.
      The output of this API is two queues that must be executed by two different DMA channel.
      The transmit data transfers default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The receive data transfers default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE or DMA_SRC_DATAWIDTH_HALFWORD according to programmed DataSize
                                  value. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE or DMA_DEST_DATAWIDTH_HALFWORD according to programmed
                                  DataSize value.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)

    [..]
      Configuration parameters through LPBAM_SPI_ConfigAdvConf_t are :
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

      Configuration parameters through LPBAM_SPI_DataAdvConf_t are :
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

      Configuration parameters through LPBAM_SPI_FullAdvConf_t are :
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
      These APIs must be called when the SPI is initialized.
          (+) Recommended SPI initialization sequence
              (++) call HAL_SPI_Init to initialize the SPI. (Mandatory)
                   Initialization parameters can be :
                   (+++) Mode                    : SPI_MODE_MASTER.
                   (+++) Direction               : SPI_DIRECTION_2LINES.
                   (+++) DataSize                : SPI_DATASIZE_8BIT.
                   (+++) CLKPolarity             : SPI_POLARITY_LOW.
                   (+++) CLKPhase                : SPI_PHASE_1EDGE.
                   (+++) NSS                     : SPI_NSS_SOFT.
                   (+++) BaudRatePrescaler       : SPI_BAUDRATEPRESCALER_256.
                   (+++) FirstBit                : SPI_FIRSTBIT_MSB.
                   (+++) TIMode                  : SPI_TIMODE_DISABLE.
                   (+++) CRCCalculation          : SPI_CRCCALCULATION_DISABLE.
                   (+++) NSSPMode                : SPI_NSS_PULSE_DISABLE.
                   (+++) MasterKeepIOState       : SPI_MASTER_KEEP_IO_STATE_ENABLE.
                   (+++) ReadyMasterManagement   : SPI_RDY_MASTER_MANAGEMENT_INTERNALLY.
                   (+++) FifoThreshold           : SPI_FIFO_THRESHOLD_04DATA. (Recommended when DataSize =
                                                                               SPI_DATASIZE_16BIT)
                   (+++) MasterSSIdleness        : SPI_MASTER_SS_IDLENESS_00CYCLE.
                   (+++) MasterInterDataIdleness : SPI_MASTER_INTERDATA_IDLENESS_00CYCLE.
                   (+++) MasterReceiverAutoSusp  : SPI_MASTER_RX_AUTOSUSP_DISABLE.
                   (+++) IOSwap                  : SPI_IO_SWAP_DISABLE.
                   (+++) ReadyPolarity           : SPI_RDY_POLARITY_HIGH.
              (++) call HAL_SPIEx_SetConfigAutonomousMode to configure the autonomous mode. (Mandatory)
                   (+++) TriggerState     : SPI_AUTO_MODE_ENABLE. (Recommended)
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
      The following parameters can be configured by LPBAM APIs :
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
          All other parameters are no reconfigured by the LPBAM APIs.

    *** Driver user sequence ***
    ============================
    [..]
      This section provides the steps to follow to build an LPBAM application based on HAL/LL and LPBAM drivers. (refer
      to LPBAM_Utility_GettingStarted.html for linked-list feature description).

    [..]
      Generic user sequence is :
          (+) Initialize the SPI (Using HAL/LL). (Mandatory)
          (+) There are two possibilities to call advanced API:
              (++) Invoking a single function performing peripheral configuration and data transfer setup :
                   (+++) Call, when needed, ADV_LPBAM_SPI_Tx_SetFullQ(). (Mandatory)
                   (+++) Call, when needed, ADV_LPBAM_SPI_Rx_SetFullQ(). (Mandatory)
                   (+++) Call, when needed, ADV_LPBAM_SPI_TxRx_SetFullQ(). (Mandatory)
              (++) Or Invoking two functions performing peripheral configuration and data transfer setup separately:
                   (+++) Call, when needed, ADV_LPBAM_SPI_Tx_SetConfigQ() and/or ADV_LPBAM_SPI_Tx_SetDataQ().
                         (Mandatory)
                   (+++) Call, when needed, ADV_LPBAM_SPI_Rx_SetConfigQ() and/or ADV_LPBAM_SPI_Rx_SetDataQ().
                         (Mandatory)
                   (+++) Call, when needed, ADV_LPBAM_SPI_TxRx_SetConfigQ() and/or ADV_LPBAM_SPI_TxRx_SetDataQ().
                         (Mandatory)
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode. (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power mode, it blocks the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() to start the DMA channel linked-list execution. (Mandatory)

    *** Recommendation ***
    ======================
    [..]
      This section provides tips and tricks to consider while using LPBAM module drivers to build a user application.

    [..]
      Memory optimization:
          (+) When the SPI peripheral doesn't need to be reconfigured, use ADV_LPBAM_SPI_{Operation}_SetDataQ() APIs
              instead of ADV_LPBAM_SPI_{Operation}_SetFullQ().
          (+) When the SPI peripheral needs to be reconfigured, use ADV_LPBAM_SPI_{Operation}_SetFullQ() instead of
              ADV_LPBAM_SPI_{Operation}_SetConfigQ() then ADV_LPBAM_SPI_{Operation}_SetDataQ().

    [..]
      SPI full duplex transfer requires 2 dedicated queues (one for transmit Tx and one for receive Rx).
      When SPI peripheral reconfiguration is needed, it is mandatory to invoke ADV_LPBAM_SPI_TxRx_SetConfigQ() only one
      time before calling data transfer APIs in the Tx queue.

    [..]
      It's mandatory to set properly DataSize, TriggerSelection and TriggerPolarity while configuring
      LPBAM_SPI_ConfigAdvConf_t or LPBAM_SPI_FullAdvConf_t because their update can be only done with
      ADV_LPBAM_SPI_{Operation}_SetConfigQ() or ADV_LPBAM_SPI_{Operation}_SetFullQ().

    [..]
      It's mandatory to ensure the consistency of DataSize parameter between ADV_LPBAM_SPI_{Operation}_SetConfigQ() and
      ADV_LPBAM_SPI_{Operation}_SetDataQ() in LPBAM_SPI_ConfigAdvConf_t and LPBAM_SPI_DataAdvConf_t structures for the
      same transfer setup.

    [..]
      It's mandatory to use only one linked-list queue when spi is used in simplex mode.

    [..]
      It's mandatory to use two linked-list queues (One for the configuration and the transmission
      and another one for the only reception) which will be executed simultaneously when spi is used in full duplex
      mode.

    [..]
      To avoid missing Rx DMA request transfers in full duplex, it's mandatory to start the DMA channel execution of Rx
      linked-list queue before starting the DMA channel execution of Tx linked-list queue.

    [..]
      It's mandatory to be sure that the slave is enabled and ready to the communication before the master starts
      generating its clock otherwise an unexpected behavior can appear.

    [..]
      When using different communication direction (Rx Receive/Tx Transmit/TxRx TransmitReceive) in the same queue,
      it's mandatory to ensure that IOs are configured to support TxRx TransmitReceive mode (full duplex) at the
      initialization stage (HAL_SPI_Init).

    [..]
      It's forbidden to use the same DMA channel for Tx transmission and Rx reception.

    [..]
      It's forbidden to execute simultaneously the same linked-list queue with different DMA channels.

    [..]
      It's forbidden to execute simultaneously two linked-list queue using the same peripheral.

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
  * @brief  Build DMA linked-list queue to setup the configuration of SPI transmit according to parameters in
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
LPBAM_Status_t ADV_LPBAM_SPI_Tx_SetConfigQ(SPI_TypeDef               *const pInstance,
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
  * @brief  Build DMA linked-list queue to setup the data of SPI transmit according to parameters in
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
LPBAM_Status_t ADV_LPBAM_SPI_Tx_SetDataQ(SPI_TypeDef             *const pInstance,
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
  * @brief  Build DMA linked-list queue to setup the configuration and data of SPI transmit according to parameters in
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
LPBAM_Status_t ADV_LPBAM_SPI_Tx_SetFullQ(SPI_TypeDef             *const pInstance,
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
  * @brief  Build DMA linked-list queue to setup the configuration of SPI receive according to parameters in
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
LPBAM_Status_t ADV_LPBAM_SPI_Rx_SetConfigQ(SPI_TypeDef               *const pInstance,
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
  * @brief  Build DMA linked-list queue to setup the data of SPI receive according to parameters in
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
LPBAM_Status_t ADV_LPBAM_SPI_Rx_SetDataQ(SPI_TypeDef             *const pInstance,
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
  * @brief  Build DMA linked-list queue to setup the configuration and data of SPI receive according to parameters in
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
LPBAM_Status_t ADV_LPBAM_SPI_Rx_SetFullQ(SPI_TypeDef             *const pInstance,
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
  * @brief  Build DMA linked-list queue to setup the configuration of SPI transmit and receive according to parameters
  *         in LPBAM_SPI_ConfigAdvConf_t.
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
LPBAM_Status_t ADV_LPBAM_SPI_TxRx_SetConfigQ(SPI_TypeDef                 *const pInstance,
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
  * @brief  Build DMA linked-list queue to setup the data of SPI transmit and receive according to parameters in
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
LPBAM_Status_t ADV_LPBAM_SPI_TxRx_SetDataQ(SPI_TypeDef                 *const pInstance,
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
  * @brief  Build DMA linked-list queue to setup the configuration and data of SPI transmit and receive according to
  *         parameters in LPBAM_SPI_FullAdvConf_t.
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
LPBAM_Status_t ADV_LPBAM_SPI_TxRx_SetFullQ(SPI_TypeDef                 *const pInstance,
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
