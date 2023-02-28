/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_i2c.c
  * @author  MCD Application Team
  * @brief   This file provides the advanced LPBAM layer for I2C peripheral.
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
          (+) stm32_adv_lpbam_i2c.c file
              (++) This file provides the implementation of the advanced LPBAM I2C functions.
          (+) stm32_adv_lpbam_i2c.h file
              (++) This file is the header file of stm32_adv_lpbam_i2c.c. It provides advanced LPBAM I2C functions
                   prototypes and the declaration of their needed exported types and structures.
          (+) STM32xx/stm32_platform_lpbam_i2c.h file
              (++) This header file contains all defines to be used on applicative side.
                   (+++) STM32xx stands for the device supporting LPBAM sub-system.

    *** Driver functions model ***
    ==============================
    [..]
      This section precises this module supported advanced functions model (refer to LPBAM_Utility_GettingStarted.html
      document for function model definition).

    [..]
      This driver provides 3 model of APIs :
          (+) ADV_LPBAM_{Module}_{Mode}_SetConfigQ() : set one peripheral configuration queue.
          (+) ADV_LPBAM_{Module}_{Mode}_SetDataQ() : set one peripheral data transfer queue.
          (+) ADV_LPBAM_{Module}_{Mode}_SetFullQ() : set one peripheral configuration and one data transfer queue.

    *** Driver features ***
    =======================
    [..]
      This section describes this LPBAM module supported features.

    [..]
      This driver provides services covering the LPBAM management of the following I2C features :
          (+) Configure the I2C transfers in master and slave modes.
          (+) Starts the I2C transfers in master and slave modes.
          (+) Configure and starts the I2C transfers in master and slave modes.
          (+) Manage sequential transfers on each driver API.
          (+) Manage reloaded transfers on each driver API.

    *** Functional description ***
    ==============================
    [..]
      This section describes the peripheral features covered by this LPBAM module.

    [..]
      The output of this driver is a queue to be executed by the DMA channel.

      The I2C peripheral transfer is configured in master and slave mode.
      The I2C peripheral transfer is started in master and slave mode.
      The I2C peripheral transfer is configured and started in master and slave mode.
      All APIs supports sequential and reload data transfers.

    *** Driver APIs description ***
    ===============================
    [..]
      This section provides LPBAM module exhaustive APIs description without considering application user call sequence.
      For user call sequence information, please refer to 'Driver user sequence' section below.

    [..]
      Use ADV_LPBAM_I2C_MasterTx_SetConfigQ() API to build a linked-list queue that setup the I2C master configuration
      for transmission operation according to parameters in the LPBAM_I2C_ConfigAdvConf_t structure.
      Configuration parameters through LPBAM_I2C_ConfigAdvConf_t are described below.

    [..]
      Use ADV_LPBAM_I2C_MasterTx_SetDataQ() API to build a linked-list queue that setup the I2C master transmission for
      starting operation according to parameters in the LPBAM_I2C_DataAdvConf_t structure.
      Configuration parameters through LPBAM_I2C_DataAdvConf_t are described below.
      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_I2C_MasterTx_SetDataQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c
      file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) DestInc           : DMA_DINC_FIXED.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.

    [..]
      Use ADV_LPBAM_I2C_MasterTx_SetFullQ() API to build a linked-list queue that setup the I2C master configuration and
      transmission for starting operation according to parameters in the LPBAM_I2C_FullAdvConf_t structure.
      Configuration parameters through LPBAM_I2C_FullAdvConf_t are described below.
      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_I2C_MasterTx_SetFullQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c
      file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) DestInc           : DMA_DINC_FIXED.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.

    [..]
      Use ADV_LPBAM_I2C_SlaveTx_SetConfigQ() API to build a linked-list queue that setup the I2C slave configuration for
      transmission operation according to parameters in the LPBAM_I2C_ConfigAdvConf_t structure.
      Configuration parameters through LPBAM_I2C_ConfigAdvConf_t are described below.

    [..]
      Use ADV_LPBAM_I2C_SlaveTx_SetDataQ() API to build a linked-list queue that setup the I2C slave transmission for
      starting operation according to parameters in the LPBAM_I2C_DataAdvConf_t structure.
      Configuration parameters through LPBAM_I2C_DataAdvConf_t are described below.
      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_I2C_SlaveTx_SetDataQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c
      file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) DestInc           : DMA_DINC_FIXED.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.

    [..]
      Use ADV_LPBAM_I2C_SlaveTx_SetFullQ() API to build a linked-list queue that setup the I2C slave configuration and
      transmission for starting operation according to parameters in the LPBAM_I2C_FullAdvConf_t structure.
      Configuration parameters through LPBAM_I2C_FullAdvConf_t are described below.
      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_I2C_SlaveTx_SetFullQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c
      file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) DestInc           : DMA_DINC_FIXED.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.

    [..]
      Use ADV_LPBAM_I2C_MasterRx_SetConfigQ() API to build a linked-list queue that setup the I2C master configuration
      reception operation according to parameters in the LPBAM_I2C_ConfigAdvConf_t structure.
      Configuration parameters through LPBAM_I2C_ConfigAdvConf_t are described below.

    [..]
      Use ADV_LPBAM_I2C_MasterRx_SetDataQ() API to build a linked-list queue that setup the I2C master reception for
      starting operation according to parameters in the LPBAM_I2C_DataAdvConf_t structure.
      Configuration parameters through LPBAM_I2C_DataAdvConf_t are described below.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_I2C_MasterRx_SetDataQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c
      file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.

    [..]
      Use ADV_LPBAM_I2C_MasterRx_SetFullQ() API to build a linked-list queue that setup the I2C master configuration and
      reception for starting operation according to parameters in the LPBAM_I2C_FullAdvConf_t structure.
      Configuration parameters through LPBAM_I2C_FullAdvConf_t are described below.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_I2C_MasterRx_SetFullQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c
      file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.

    [..]
      Use ADV_LPBAM_I2C_SlaveRx_SetConfigQ() API to build a linked-list queue that setup the I2C slave configuration
      reception for operation according to parameters in the LPBAM_I2C_ConfigAdvConf_t structure.
      Configuration parameters through LPBAM_I2C_ConfigAdvConf_t are described below.

    [..]
      Use ADV_LPBAM_I2C_SlaveRx_SetDataQ() API to build a linked-list queue that setup the I2C slave reception for
      starting operation according to parameters in the LPBAM_I2C_DataAdvConf_t structure.
      Configuration parameters through LPBAM_I2C_DataAdvConf_t are described below.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_I2C_SlaveRx_SetDataQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c
      file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.

    [..]
      Use ADV_LPBAM_I2C_SlaveRx_SetFullQ() API to build a linked-list queue that setup the I2C slave configuration and
      reception for starting operation according to parameters in the LPBAM_I2C_FullAdvConf_t structure.
      Configuration parameters through LPBAM_I2C_FullAdvConf_t are described below.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_I2C_SlaveRx_SetFullQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c
      file.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.

    [..]
      Configuration parameters through LPBAM_I2C_ConfigAdvConf_t are :
          (+) AutoModeConf : Specifies the autonomous mode parameters.
              (++) TriggerState     : Specifies the trigger state.
                                      This parameter can be a value of @ref LPBAM_I2CEx_AutonomousMode_FunctionalState.
              (++) TriggerSelection : Specifies the trigger state.
                                      This parameter can be a value of @ref LPBAM_I2CEx_AutonomousMode_TriggerSelection.
              (++) TriggerPolarity  : Specifies the trigger state.
                                      This parameter can be a value of @ref LPBAM_I2CEx_AutonomousMode_TriggerPolarity.
          (+) Timing       : Specifies the timing value.
                             This parameter calculated by referring to I2C initialization section in Reference manual.
          (+) WakeupIT     : Specifies the wake up source interrupt.
                             This parameter can be a value of @ref LPBAM_I2C_Wakeup_Interrupt.

    [..]
      Configuration parameters through LPBAM_I2C_DataAdvConf_t are :
          (+) AutoModeConf   : Specifies the autonomous mode parameters.
              (++) TriggerState : Specifies the trigger state.
                                  This parameter can be a value of @ref LPBAM_I2CEx_AutonomousMode_FunctionalState.
          (+) AddressingMode : Specifies if 7-bit or 10-bit addressing mode is selected.
          (+) SequenceNumber : Specifies the number of frames to be transferred when sequential transfer is activated.
                               This parameter shall be equal to 1 when transfer is not sequential.
          (+) DevAddress     : Specifies the target device address.
          (+) Size           : Specifies the I2C data number of bytes to transfer.
          (+) pData          : Specifies the transfer buffer address.

    [..]
      Configuration parameters through LPBAM_I2C_FullAdvConf_t are :
          (+) AutoModeConf : Specifies the autonomous mode parameters.
              (++) TriggerState     : Specifies the trigger state.
                                      This parameter can be a value of @ref LPBAM_I2CEx_AutonomousMode_FunctionalState.
              (++) TriggerSelection : Specifies the trigger state.
                                      This parameter can be a value of @ref LPBAM_I2CEx_AutonomousMode_TriggerSelection.
              (++) TriggerPolarity  : Specifies the trigger state.
                                      This parameter can be a value of @ref LPBAM_I2CEx_AutonomousMode_TriggerPolarity.
          (+) Timing       : Specifies the timing value.
                             This parameter calculated by referring to I2C initialization section in Reference manual.
          (+) WakeupIT     : Specifies the wake up source interrupt.
                             This parameter can be a value of @ref LPBAM_I2C_Wakeup_Interrupt.
          (+) AddressingMode : Specifies if 7-bit or 10-bit addressing mode is selected.
          (+) SequenceNumber : Specifies the number of frames to be transferred when sequential transfer is activated.
                               This parameter shall be equal to 1 when transfer is not sequential.
          (+) DevAddress     : Specifies the target device address.
          (+) Size           : Specifies the I2C data number of bytes to transfer.
          (+) pData          : Specifies the transfer buffer address.

    [..]
      These APIs must be called when the I2C is initialized.
          (+) Recommended I2C initialization sequence
              (++) Call HAL_I2C_Init() to initialize the I2C peripheral. (Mandatory)
                   Initialization parameters can be :
                   (+++) Timing                  : Any.
                   (+++) AddressingMode          : Any.
                   (+++) DualAddressMode         : I2C_DUALADDRESS_DISABLE.
                   (+++) OwnAddress2Masks        : I2C_OA2_NOMASK.
                   (+++) GeneralCallMode         : I2C_GENERALCALL_DISABLE.
                   (+++) NoStretchMode           : I2C_NOSTRETCH_DISABLE. (Mandatory)
                   (+++) OwnAddress1             : Any.
                   (+++) OwnAddress2             : Any.
              (++) call HAL_I2CEx_SetConfigAutonomousMode to configure the autonomous mode.
                   (+++) TriggerState     : Any.
                   (+++) TriggerSelection : Any.
                   (+++) TriggerPolarity  : Any.
              (++) Call HAL_I2CEx_EnableWakeUp to enable I2C wakeup from Stop mode(s).
              (++) Call __HAL_I2C_ENABLE_IT() to enable error interrupts.
                   I2C error interrupts can be :
                   (+++) I2C_IT_ERRI : Bus error / Arbitration lost error / Overrun/ Underrun (slave mode) error.

    [..]
      The following parameters must be configured carefully as they are not configurable by LPBAM APIs :
          (+) DualAddressMode        : This parameter can be a value of @ref I2C_DUAL_ADDRESSING_MODE.
          (+) OwnAddress2Masks       : This parameter can be a value of @ref I2C_OWN_ADDRESS2_MASKS.
          (+) GeneralCallMode        : This parameter should be this value I2C_GENERAL_CALL_ADDRESSING_MODE.
          (+) NoStretchMode          : This parameter should be this value I2C_NOSTRETCH_MODE.
          (+) OwnAddress1            : Device first own address.
          (+) OwnAddress2            : Device second own address.

    *** Driver user sequence ***
    ============================
    [..]
      This section provides the steps to follow to build an LPBAM application based on HAL/LL and LPBAM drivers. (refer
      to LPBAM_Utility_GettingStarted.html for linked-list feature description).

    [..]
      Generic driver user sequence (no sequential and no reload transfers) is :
          (+) Initialize the I2C (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_I2C_EnableDMARequests() to enable the I2C DMA requests. (Mandatory)
          (+) Call ADV_LPBAM_I2C_RegisterISR() to register the I2C transfer IRQ handler function. (Mandatory)
          (+) Enable I2C to wakeup CPU from Stop mode(s) (Using HAL/LL). (Optional)
          (+) Enable I2C errors interrupt. (Optional)
          (+) There are two possibilities to call advanced API:
              (++) Single API making configuration and data setup :
                   (+++) For transmission mode, call ADV_LPBAM_I2C_[Mode]Tx_SetFullQ(). (Mandatory)
                   (+++) For reception mode, call ADV_LPBAM_I2C_[Mode]Rx_SetFullQ(). (Mandatory)
              (++) Two APIs making configuration and data setup separately :
                   (+++) For a transmission mode call ADV_LPBAM_I2C_[Mode]Tx_SetConfigQ() then
                         ADV_LPBAM_I2C_[Mode]Tx_SetDataQ(). (Mandatory)
                   (+++) For a reception mode call ADV_LPBAM_I2C_[Mode]Rx_SetConfigQ() and/or
                         ADV_LPBAM_I2C_[Mode]Rx_SetDataQ(). (Mandatory)
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_I2C_[Mode][Tx/Rx]_SetConfigQ(),
              ADV_LPBAM_I2C_[Mode][Tx/Rx]_SetDataQ() or ADV_LPBAM_I2C_[Mode][Tx/Rx]_SetFullQ() output queue.
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
      Sequential transfers driver user sequence is (SequenceNumber > 1) :
          (+) Initialize the I2C (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_I2C_EnableDMARequests() to enable the I2C DMA requests. (Mandatory)
          (+) Call ADV_LPBAM_I2C_RegisterISR() to register the I2C transfer IRQ handler function. (Mandatory)
          (+) Enable I2C to wakeup CPU from Stop mode(s) (Using HAL/LL). (Optional)
          (+) Enable I2C errors interrupt. (Optional)
          (+) There are two possibilities to call advanced API:
              (++) Transfer with full APIs :
                   (+++) For transmission mode, call ADV_LPBAM_I2C_[Mode]Tx_SetFullQ() x times where x = SequenceNumber.
                         (Mandatory)
                   (+++) For reception mode, call ADV_LPBAM_I2C_[Mode]Rx_SetFullQ() x times where x = SequenceNumber.
                         (Mandatory)
              (++) Transfer with configuration and data APIs :
                   (+++) For a transmission mode call ADV_LPBAM_I2C_[Mode]Tx_SetConfigQ() only one time then call
                         ADV_LPBAM_I2C_[Mode]Tx_SetDataQ() x times where x = SequenceNumber. (Mandatory).
                   (+++) For a reception mode call ADV_LPBAM_I2C_[Mode]Rx_SetConfigQ() only one time then call
                         ADV_LPBAM_I2C_[Mode]Rx_SetDataQ() x times where x = SequenceNumber. (Mandatory)
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_I2C_[Mode][Tx/Rx]_SetConfigQ(),
              ADV_LPBAM_I2C_[Mode][Tx/Rx]_SetDataQ() or ADV_LPBAM_I2C_[Mode][Tx/Rx]_SetFullQ() output queue.
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
      Reload transfers (data size > 255) driver user sequence is :
          (+) Initialize the I2C (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_I2C_EnableDMARequests() to enable the I2C DMA requests. (Mandatory)
          (+) Call ADV_LPBAM_I2C_RegisterISR() to register the I2C transfer IRQ handler function. (Mandatory)
          (+) Enable I2C to wakeup CPU from Stop mode(s) (Using HAL/LL). (Optional)
          (+) Enable I2C errors interrupt. (Optional)
          (+) There are two possibilities to call advanced API:
              (++) Transfer with full APIs :
                   (+++) For transmission mode, call ADV_LPBAM_I2C_[Mode]Tx_SetFullQ() y time where y = (Size/255) when
                         Size is a multiple of 255, otherwise y = ((Size/255) + 1). (Mandatory)
                   (+++) For reception mode, call ADV_LPBAM_I2C_[Mode]Rx_SetFullQ() y time where y = (Size/255) when
                         Size is a multiple of 255, otherwise y = ((Size/255) + 1). (Mandatory)
              (++) Transfer with configuration and data APIs :
                   (+++) For a transmission mode call ADV_LPBAM_I2C_[Mode]Tx_SetConfigQ() only one time then call
                         ADV_LPBAM_I2C_[Mode]Tx_SetDataQ() y times where y times where y = (Size/255) when Size is a
                         multiple of 255, otherwise y = ((Size/255) + 1). (Mandatory)
                   (+++) For a reception mode call ADV_LPBAM_I2C_[Mode]Rx_SetConfigQ() only one time then call
                         ADV_LPBAM_I2C_[Mode]Rx_SetDataQ() y times where y = (Size/255) when Size is a multiple of 255,
                         otherwise y = ((Size/255) + 1). (Mandatory)
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_I2C_[Mode][Tx/Rx]_SetConfigQ(),
              ADV_LPBAM_I2C_[Mode][Tx/Rx]_SetDataQ() or ADV_LPBAM_I2C_[Mode][Tx/Rx]_SetFullQ() output queue.
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
      Reload and sequential transfers (SequenceNumber > 1 and data size > 255) driver user sequence is :
          (+) Initialize the I2C (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_I2C_EnableDMARequests() to enable the I2C DMA requests. (Mandatory)
          (+) Call ADV_LPBAM_I2C_RegisterISR() to register the I2C transfer IRQ handler function. (Mandatory)
          (+) Enable I2C to wakeup CPU from Stop mode(s) (Using HAL/LL). (Optional)
          (+) Enable I2C errors interrupt. (Optional)
          (+) There are two possibilities to call advanced API:
              (++) Transfer with full APIs :
                   (+++) For transmission mode, call ADV_LPBAM_I2C_[Mode]Tx_SetFullQ() x times where x = SequenceNumber.
                         Each sequence with data size > 255 should be called y times where y = (Size/255) when Size is
                         a multiple of 255, otherwise y = ((Size/255) + 1). (Mandatory)
                   (+++) For reception mode, call ADV_LPBAM_I2C_[Mode]Rx_SetFullQ() x times where x = SequenceNumber.
                         Each sequence with data size > 255 should be called y times where y = (Size/255) when Size is
                         a multiple of 255, otherwise y = ((Size/255) + 1). (Mandatory)
              (++) Transfer with configuration and data APIs :
                   (+++) For a transmission mode call ADV_LPBAM_I2C_[Mode]Tx_SetConfigQ() only one time then call
                         ADV_LPBAM_I2C_[Mode]Tx_SetDataQ() x times where x = SequenceNumber. Each data sequence with
                         data size > 255 should be called y times where y = (Size/255) when Size is a multiple of 255,
                         otherwise y = ((Size/255) + 1). (Mandatory)
                   (+++) For a reception mode call ADV_LPBAM_I2C_[Mode]Rx_SetConfigQ() only one time then call
                         ADV_LPBAM_I2C_[Mode]Rx_SetDataQ() x times where x = SequenceNumber. Each data sequence with
                         data size > 255 should be called y times where y = (Size/255) when Size is a multiple of 255,
                         otherwise y = ((Size/255) + 1). (Mandatory)
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_I2C_[Mode][Tx/Rx]_SetConfigQ(),
              ADV_LPBAM_I2C_[Mode][Tx/Rx]_SetDataQ() or ADV_LPBAM_I2C_[Mode][Tx/Rx]_SetFullQ() output queue.
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

    *** Recommendation ***
    ======================
    [..]
      This section provides tips and tricks to consider while using LPBAM module drivers to build a user application.

    [..]
      When sequential transfer is activated (SequenceNumber > 1), a transfer with data size greater than 255 bytes is
      considered as one sequence. ADV_LPBAM_I2C_[Master/Slave][Tx/Rx]_SetDataQ() or
      ADV_LPBAM_I2C_[Master/Slave][Tx/Rx]_SetFullQ() should be called x time where x = SequenceNumber.

    [..]
      The maximum data size could be sent at each call is 255 bytes. When data size is greater than 255,
      the ADV_LPBAM_I2C_[Master/Slave][Tx/Rx]_SetDataQ() or ADV_LPBAM_I2C_[Master/Slave][Tx/Rx]_SetFullQ() API should be
      called y time where y time where y = (Size/255) when Size is a multiple of 255. Otherwise, y is the integer part
      of ((Size/255) + 1).

    [..]
      For memory optimization purpose, when no re-configuration is necessary, it's recommended to use
      ADV_LPBAM_I2C_[Master/Slave][Tx/Rx]_SetFullQ().

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

#ifdef LPBAM_I2C_MODULE_ENABLED

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup I2C_Advanced I2C_Advanced
  * @brief    I2C Advanced LPBAM module driver
  * @{
  */

/* Private variables -------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_I2C_Advanced_Private_Variables LPBAM I2C Advanced Private Variables
  * @{
  */
/* Store sequential number */
static uint32_t SequentialIdx = 1U;
/**
  * @}
  */

/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
#if defined (HAL_I2C_MODULE_ENABLED)
/** @defgroup LPBAM_I2C_Advanced_Private_Function_Prototypes LPBAM I2C Advanced Private Function Prototypes
  * @{
  */
static HAL_StatusTypeDef I2C_ISR(struct __I2C_HandleTypeDef *hi2c, uint32_t ITFlags, uint32_t ITSources);
/**
  * @}
  */
#endif /* HAL_I2C_MODULE_ENABLED */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_I2C_Advanced_Exported_Functions LPBAM I2C Advanced Exported Functions
  * @{
  */

/**
  * @brief  Build DMA linked-list queue to setup the configuration of I2C master transmit according to parameters in
  *         LPBAM_I2C_ConfigAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxConfig    : [IN]  Pointer to a LPBAM_I2C_ConfigAdvConf_t structure that contains master transmit
  *                              configuration nodes information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_I2C_MasterTxConfigDesc_t structure that contains transmit
  *                              configuration descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_MasterTx_SetConfigQ(I2C_TypeDef                    *const pInstance,
                                                 LPBAM_DMAListInfo_t            const *const pDMAListInfo,
                                                 LPBAM_I2C_ConfigAdvConf_t      const *const pTxConfig,
                                                 LPBAM_I2C_MasterTxConfigDesc_t *const pDescriptor,
                                                 DMA_QListTypeDef               *const pQueue)
{
  LPBAM_I2C_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  uint8_t idx = 0U;

  /*
   *               ######## Disable I2C Instance ########
   */

  /* Set I2C instance */
  config_node.pInstance                   = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID    = (uint32_t)LPBAM_I2C_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType  = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg         = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Mode                 = LPBAM_I2C_MASTER_TRANSMIT_MODE;
  config_node.Config.State                = DISABLE;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build disable node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert disable node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## Set I2C Timing ########
   */
  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_TIMING_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Timing            = pTxConfig->Timing;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build timing node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert timing node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## Select I2C Trigger Source ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID             = (uint32_t)LPBAM_I2C_TRIG_ID;
  config_node.NodeDesc.pSrcVarReg                  = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.AutoModeConf.TriggerState     = pTxConfig->AutoModeConf.TriggerState;
  config_node.Config.AutoModeConf.TriggerSelection = pTxConfig->AutoModeConf.TriggerSelection;
  config_node.Config.AutoModeConf.TriggerPolarity  = pTxConfig->AutoModeConf.TriggerPolarity;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build trigger source node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert trigger source node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }
  /* Update node index */
  idx++;


  /*
   *               ######## Enable I2C Instance ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.State             = ENABLE;
  config_node.Config.WakeupIT          = pTxConfig->WakeupIT;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build enable node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert enable node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the data of I2C master transmit according to parameters in
  *         LPBAM_I2C_DataAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxData      : [IN]  Pointer to a LPBAM_I2C_DataAdvConf_t structure that contains transmit data nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_I2C_MasterTxDataDesc_t structure that contains transmit data
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_MasterTx_SetDataQ(I2C_TypeDef                  *const pInstance,
                                               LPBAM_DMAListInfo_t          const *const pDMAListInfo,
                                               LPBAM_I2C_DataAdvConf_t      const *const pTxData,
                                               LPBAM_I2C_MasterTxDataDesc_t *const pDescriptor,
                                               DMA_QListTypeDef             *const pQueue)
{
  LPBAM_I2C_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  static uint8_t is_reload_data = 0U;
  static uint32_t buffer_offset = 0U;
  uint8_t idx                   = 0U;
  uint32_t buffer_address;

  /*
   *               ######## Setup reload data variables ########
   */

  /* Check if data size > Max I2C data size */
  if (pTxData->Size > LPBAM_I2C_MAX_DATA_SIZE)
  {
    /* Update reload data information */
    config_node.Config.ReloadDataState  = ENABLE;
    config_node.Config.IsLastReloadData = LPBAM_I2C_IS_NOT_LAST_RELOADED_DATA;

    if (is_reload_data == 1U)
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
    }
    else
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_FIRST_RELOADED_DATA;

      /* Update reload variable */
      is_reload_data = 1U;
    }
  }
  /* Data size < 255 */
  else
  {
    /* Transfer data is reloaded  */
    if (is_reload_data == 1U)
    {
      /* Enable reload data */
      config_node.Config.ReloadDataState   = ENABLE;

      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
      config_node.Config.IsLastReloadData  = LPBAM_I2C_IS_LAST_RELOADED_DATA;

      /* Reset reload data variable */
      is_reload_data = 0U;
    }
    /* Transfer data is not reloaded  */
    else
    {
      /* Disable reload data */
      config_node.Config.ReloadDataState = DISABLE;
    }
  }


  /*
   *               ######## Setup sequential data variables ########
   */

  /* Transfer data is sequential */
  if (pTxData->SequenceNumber > 1U)
  {
    /* Enable sequential data transfer */
    config_node.Config.SequentialDataState = ENABLE;

    /* First sequential data transfer */
    if (SequentialIdx == 1U)
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx++;
    }
    /* Last sequential data transfer */
    else if ((SequentialIdx == pTxData->SequenceNumber) && (is_reload_data == 0U))
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx = 1U;
    }
    else
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;

      /* Check if reload data is disabled */
      if (is_reload_data == 0U)
      {
        /* Update sequential variable */
        SequentialIdx++;
      }
    }
  }
  /* Transfer data is not sequential */
  else
  {
    /* Disable sequential data */
    config_node.Config.SequentialDataState = DISABLE;
  }


  /*
   *               ######## Configure I2C transfer  ########
   */

  /* Set I2C instance */
  config_node.pInstance                         = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID         = (uint32_t)LPBAM_I2C_CONFIG_TRANSACTION_ID;
  config_node.NodeDesc.NodeInfo.NodeType       = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg              = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Mode                      = LPBAM_I2C_MASTER_TRANSMIT_MODE;
  config_node.Config.DevAddress                = pTxData->DevAddress;
  config_node.Config.Size                      = pTxData->Size;
  config_node.Config.AddressingMode            = pTxData->AddressingMode;
  config_node.Config.AutoModeConf.TriggerState = pTxData->AutoModeConf.TriggerState;

  /* Fill configuration node */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configuration node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## I2C Transmit Data  ########
   */

  /* Get data buffer address */
  buffer_address                       = (uint32_t)&pTxData->pData[buffer_offset];

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_TRANSMIT_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build transmit data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert transmit data to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Reload data transfer */
  if (is_reload_data == 1U)
  {
    buffer_offset += LPBAM_I2C_MAX_DATA_SIZE;
  }
  /* Reload data transfer */
  else
  {
    /* Reset buffer offset variable */
    buffer_offset = 0U;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the configuration and data of I2C master transmit according to
  *         parameters in LPBAM_I2C_FullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxFull      : [IN]  Pointer to a LPBAM_I2C_FullAdvConf_t structure that contains transmit full nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_I2C_MasterTxFullDesc_t structure that contains transmit full
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_MasterTx_SetFullQ(I2C_TypeDef                  *const pInstance,
                                               LPBAM_DMAListInfo_t          const *const pDMAListInfo,
                                               LPBAM_I2C_FullAdvConf_t      const *const pTxFull,
                                               LPBAM_I2C_MasterTxFullDesc_t *const pDescriptor,
                                               DMA_QListTypeDef             *const pQueue)
{
  LPBAM_I2C_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  static uint8_t is_reload_data = 0U;
  static uint32_t buffer_offset = 0U;
  uint8_t idx                   = 0U;
  uint32_t buffer_address;

  /*
   *               ######## Setup reload data variables ########
   */

  /* Check if data size > Max I2C data size */
  if (pTxFull->Size > LPBAM_I2C_MAX_DATA_SIZE)
  {
    /* Update reload data information */
    config_node.Config.ReloadDataState  = ENABLE;
    config_node.Config.IsLastReloadData = LPBAM_I2C_IS_NOT_LAST_RELOADED_DATA;

    if (is_reload_data == 1U)
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
    }
    else
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_FIRST_RELOADED_DATA;

      /* Update reload variable */
      is_reload_data = 1U;
    }
  }
  /* Data size < 255 */
  else
  {
    /* Transfer data is reloaded  */
    if (is_reload_data == 1U)
    {
      /* Enable reload data */
      config_node.Config.ReloadDataState = ENABLE;

      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
      config_node.Config.IsLastReloadData  = LPBAM_I2C_IS_LAST_RELOADED_DATA;

      /* Reset reload data variable */
      is_reload_data = 0U;
    }
    /* Transfer data is not reloaded  */
    else
    {
      /* Disable reload data */
      config_node.Config.ReloadDataState = DISABLE;
    }
  }


  /*
   *               ######## Setup sequential data variables ########
   */

  /* Transfer data is sequential */
  if (pTxFull->SequenceNumber > 1U)
  {
    /* Enable sequential data transfer */
    config_node.Config.SequentialDataState = ENABLE;

    /* First sequential data transfer */
    if (SequentialIdx == 1U)
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx++;
    }
    /* Last sequential data transfer */
    else if ((SequentialIdx == pTxFull->SequenceNumber) && (is_reload_data == 0U))
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx = 1U;
    }
    else
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;

      /* Check if reload data is disabled */
      if (is_reload_data == 0U)
      {
        /* Update sequential variable */
        SequentialIdx++;
      }
    }
  }
  /* Transfer data is not sequential */
  else
  {
    /* Disable sequential data */
    config_node.Config.SequentialDataState = DISABLE;
  }

  /* Check reload and sequential information */
  if (((config_node.Config.ReloadDataState == DISABLE) && (config_node.Config.SequentialDataState == DISABLE)) || \
      ((config_node.Config.SequentialDataState == ENABLE)                                                      && \
       (config_node.Config.IsFirstSequentialData == LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA))                       || \
      ((config_node.Config.SequentialDataState == DISABLE)                                                     && \
       (config_node.Config.IsFirstReloadData == LPBAM_I2C_IS_FIRST_RELOADED_DATA)))
  {
    /*
     *               ######## Disable I2C Instance ########
     */

    /* Set I2C instance */
    config_node.pInstance                  = pInstance;

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_STATE_ID;
    config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
    config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.Mode                = LPBAM_I2C_MASTER_TRANSMIT_MODE;
    config_node.Config.State               = DISABLE ;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build disable node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert disable node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;


    /*
     *               ######## Set I2C Timing ########
     */

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_TIMING_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.Timing            = pTxFull->Timing;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build timing node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert timing node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;


    /*
     *               ######## Select I2C Trigger Source ########
     */

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID             = (uint32_t)LPBAM_I2C_TRIG_ID;
    config_node.NodeDesc.pSrcVarReg                  = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.AutoModeConf.TriggerState     = pTxFull->AutoModeConf.TriggerState;
    config_node.Config.AutoModeConf.TriggerSelection = pTxFull->AutoModeConf.TriggerSelection;
    config_node.Config.AutoModeConf.TriggerPolarity  = pTxFull->AutoModeConf.TriggerPolarity;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build trigger source node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert trigger source node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;


    /*
     *               ######## Enable I2C Instance ########
     */

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_STATE_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.State             = ENABLE;
    config_node.Config.WakeupIT          = pTxFull->WakeupIT;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build enable node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert enable node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;
  }
  else
  {
    /* Update node index */
    idx = I2C_MASTER_DATA_NODEIDX;
  }


  /*
   *               ######## Configure I2C transfer  ########
   */

  /* Set I2C instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_CONFIG_TRANSACTION_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Mode                = LPBAM_I2C_MASTER_TRANSMIT_MODE;
  config_node.Config.DevAddress          = pTxFull->DevAddress;
  config_node.Config.Size                = pTxFull->Size;
  config_node.Config.AddressingMode      = pTxFull->AddressingMode;

  /* Fill configuration node */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configuration node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## I2C Transmit Data  ########
   */

  /* Get data buffer address */
  buffer_address                       = (uint32_t)&pTxFull->pData[buffer_offset];

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_TRANSMIT_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build transmit data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert transmit data to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Reload data transfer */
  if (is_reload_data == 1U)
  {
    buffer_offset += LPBAM_I2C_MAX_DATA_SIZE;
  }
  /* Reload data transfer */
  else
  {
    /* Reset buffer offset variable */
    buffer_offset = 0U;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the configuration of I2C slave transmit according to parameters in
  *         LPBAM_I2C_ConfigAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxConfig    : [IN]  Pointer to a LPBAM_I2C_ConfigAdvConf_t structure that contains transmit configuration
  *                              nodes information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_I2C_SlaveTxConfigDesc_t structure that contains transmit
  *                              configuration descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_SlaveTx_SetConfigQ(I2C_TypeDef                   *const pInstance,
                                                LPBAM_DMAListInfo_t           const *const pDMAListInfo,
                                                LPBAM_I2C_ConfigAdvConf_t     const *const pTxConfig,
                                                LPBAM_I2C_SlaveTxConfigDesc_t *const pDescriptor,
                                                DMA_QListTypeDef              *const pQueue)
{
  LPBAM_I2C_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  uint8_t idx = 0U;

  /*
   *               ######## Disable I2C Instance ########
   */

  /* Set I2C instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Mode                = LPBAM_I2C_SLAVE_TRANSMIT_MODE;
  config_node.Config.State               = DISABLE ;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build disable node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert disable node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## Set I2C Timing ########
   */
  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_TIMING_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Timing            = pTxConfig->Timing;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build timing node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert timing node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## Enable I2C Instance ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.State             = ENABLE ;
  config_node.Config.WakeupIT          = pTxConfig->WakeupIT;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build enable node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert enable node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the data of I2C slave transmit according to parameters in
  *         LPBAM_I2C_DataAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxData      : [IN]  Pointer to a LPBAM_I2C_DataAdvConf_t structure that contains transmit data nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_I2C_SlaveTxDataDesc_t structure that contains transmit data
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_SlaveTx_SetDataQ(I2C_TypeDef                 *const pInstance,
                                              LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                              LPBAM_I2C_DataAdvConf_t     const *const pTxData,
                                              LPBAM_I2C_SlaveTxDataDesc_t *const pDescriptor,
                                              DMA_QListTypeDef            *const pQueue)
{
  LPBAM_I2C_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  static uint8_t is_reload_data = 0U;
  static uint32_t buffer_offset = 0U;
  uint8_t idx                   = 0U;
  uint32_t buffer_address;

  /*
   *               ######## Setup reload data variables ########
   */

  /* Check if data size > Max I2C data size */
  if (pTxData->Size > LPBAM_I2C_MAX_DATA_SIZE)
  {
    /* Update reload data information */
    config_node.Config.ReloadDataState  = ENABLE;
    config_node.Config.IsLastReloadData = LPBAM_I2C_IS_NOT_LAST_RELOADED_DATA;

    if (is_reload_data == 1U)
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
    }
    else
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_FIRST_RELOADED_DATA;

      /* Update reload variable */
      is_reload_data = 1U;
    }
  }
  /* Data size < 255 */
  else
  {
    /* Transfer data is reloaded  */
    if (is_reload_data == 1U)
    {
      /* Enable reload data */
      config_node.Config.ReloadDataState = ENABLE;

      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
      config_node.Config.IsLastReloadData  = LPBAM_I2C_IS_LAST_RELOADED_DATA;

      /* Reset reload data variable */
      is_reload_data = 0U;
    }
    /* Transfer data is not reloaded  */
    else
    {
      /* Disable reload data */
      config_node.Config.ReloadDataState = DISABLE;
    }
  }


  /*
   *               ######## Setup sequential data variables ########
   */

  /* Transfer data is sequential */
  if (pTxData->SequenceNumber > 1U)
  {
    /* Enable sequential data transfer */
    config_node.Config.SequentialDataState = ENABLE;

    /* First sequential data transfer */
    if (SequentialIdx == 1U)
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx++;
    }
    /* Last sequential data transfer */
    else if ((SequentialIdx == pTxData->SequenceNumber) && (is_reload_data == 0U))
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx = 1U;
    }
    else
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;

      /* Check if reload data is disabled */
      if (is_reload_data == 0U)
      {
        /* Update sequential variable */
        SequentialIdx++;
      }
    }
  }
  /* Transfer data is not sequential */
  else
  {
    /* Disable sequential data */
    config_node.Config.SequentialDataState = DISABLE;
  }


  /*
   *               ######## Configure I2C transfer  ########
   */

  /* Set I2C instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_CONFIG_TRANSACTION_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Mode                = LPBAM_I2C_SLAVE_TRANSMIT_MODE;
  config_node.Config.Size                = pTxData->Size;
  config_node.Config.AddressingMode      = pTxData->AddressingMode;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## I2C Transmit Data ########
   */

  /* Get data buffer address */
  buffer_address                       = (uint32_t)&pTxData->pData[buffer_offset];

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_TRANSMIT_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build transmit data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect transmit data node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Reload data transfer */
  if (is_reload_data == 1U)
  {
    buffer_offset += LPBAM_I2C_MAX_DATA_SIZE;
  }
  /* Reload data transfer */
  else
  {
    /* Reset buffer offset variable */
    buffer_offset = 0U;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the configuration and data of I2C slave transmit according to
  *         parameters in LPBAM_I2C_FullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pTxFull      : [IN]  Pointer to a LPBAM_I2C_FullAdvConf_t structure that contains transmit full nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_I2C_SlaveTxFullDesc_t structure that contains transmit full
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_SlaveTx_SetFullQ(I2C_TypeDef                 *const pInstance,
                                              LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                              LPBAM_I2C_FullAdvConf_t     const *const pTxFull,
                                              LPBAM_I2C_SlaveTxFullDesc_t *const pDescriptor,
                                              DMA_QListTypeDef            *const pQueue)
{
  LPBAM_I2C_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  static uint8_t is_reload_data = 0U;
  static uint32_t buffer_offset = 0U;
  uint8_t idx                   = 0U;
  uint32_t buffer_address;

  /*
   *               ######## Setup reload data variables ########
   */

  /* Check if data size > Max I2C data size */
  if (pTxFull->Size > LPBAM_I2C_MAX_DATA_SIZE)
  {
    /* Update reload data information */
    config_node.Config.ReloadDataState  = ENABLE;
    config_node.Config.IsLastReloadData = LPBAM_I2C_IS_NOT_LAST_RELOADED_DATA;

    if (is_reload_data == 1U)
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
    }
    else
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_FIRST_RELOADED_DATA;

      /* Update reload variable */
      is_reload_data = 1U;
    }
  }
  /* Data size < 255 */
  else
  {
    /* Transfer data is reloaded  */
    if (is_reload_data == 1U)
    {
      /* Enable reload data */
      config_node.Config.ReloadDataState = ENABLE;

      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
      config_node.Config.IsLastReloadData  = LPBAM_I2C_IS_LAST_RELOADED_DATA;

      /* Reset reload data variable */
      is_reload_data = 0U;
    }
    /* Transfer data is not reloaded  */
    else
    {
      /* Disable reload data */
      config_node.Config.ReloadDataState = DISABLE;
    }
  }


  /*
   *               ######## Setup sequential data variables ########
   */

  /* Transfer data is sequential */
  if (pTxFull->SequenceNumber > 1U)
  {
    /* Enable sequential data transfer */
    config_node.Config.SequentialDataState = ENABLE;

    /* First sequential data transfer */
    if (SequentialIdx == 1U)
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx++;
    }
    /* Last sequential data transfer */
    else if ((SequentialIdx == pTxFull->SequenceNumber) && (is_reload_data == 0U))
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx = 1U;
    }
    else
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;

      /* Check if reload data is disabled */
      if (is_reload_data == 0U)
      {
        /* Update sequential variable */
        SequentialIdx++;
      }
    }
  }
  /* Transfer data is not sequential */
  else
  {
    /* Disable sequential data */
    config_node.Config.SequentialDataState = DISABLE;
  }

  /* Check reload and sequential information */
  if (((config_node.Config.ReloadDataState == DISABLE) && (config_node.Config.SequentialDataState == DISABLE)) || \
      ((config_node.Config.SequentialDataState == ENABLE)                                                      && \
       (config_node.Config.IsFirstSequentialData == LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA))                       || \
      ((config_node.Config.SequentialDataState == DISABLE)                                                     && \
       (config_node.Config.IsFirstReloadData == LPBAM_I2C_IS_FIRST_RELOADED_DATA)))
  {
    /*
     *               ######## Disable I2C Instance ########
     */

    /* Set I2C instance */
    config_node.pInstance                  = pInstance;
    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_STATE_ID;
    config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
    config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.Mode                = LPBAM_I2C_SLAVE_TRANSMIT_MODE;
    config_node.Config.State               = DISABLE ;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build disable node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert disable node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;


    /*
     *               ######## Set I2C Timing ########
     */

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_TIMING_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.Timing            = pTxFull->Timing;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build timing node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert timing node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;


    /*
     *               ######## Enable I2C Instance ########
     */

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_STATE_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.State             = ENABLE ;
    config_node.Config.WakeupIT          = pTxFull->WakeupIT;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build enable node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert enable node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;
  }
  else
  {
    /* Update node index */
    idx = I2C_SLAVE_DATA_NODEIDX;
  }


  /*
   *               ######## Configure I2C transfer  ########
   */

  /* Set I2C instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_CONFIG_TRANSACTION_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Mode                = LPBAM_I2C_SLAVE_TRANSMIT_MODE;
  config_node.Config.Size                = pTxFull->Size;
  config_node.Config.AddressingMode      = pTxFull->AddressingMode;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## I2C Transmit Data ########
   */

  /* Get data buffer address */
  buffer_address                       = (uint32_t)&pTxFull->pData[buffer_offset];

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_TRANSMIT_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build transmit data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect transmit data node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Reload data transfer */
  if (is_reload_data == 1U)
  {
    buffer_offset += LPBAM_I2C_MAX_DATA_SIZE;
  }
  /* Reload data transfer */
  else
  {
    /* Reset buffer offset variable */
    buffer_offset = 0U;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the configuration of I2C master receive according to parameters in
  *         LPBAM_I2C_ConfigAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pRxConfig    : [IN]  Pointer to a LPBAM_I2C_ConfigAdvConf_t structure that contains master receive
  *                              configuration nodes information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_I2C_MasterRxConfigDesc_t structure that contains receive
  *                              configuration descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_MasterRx_SetConfigQ(I2C_TypeDef                    *const pInstance,
                                                 LPBAM_DMAListInfo_t            const *const pDMAListInfo,
                                                 LPBAM_I2C_ConfigAdvConf_t      const *const pRxConfig,
                                                 LPBAM_I2C_MasterRxConfigDesc_t *const pDescriptor,
                                                 DMA_QListTypeDef               *const pQueue)
{
  LPBAM_I2C_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  uint8_t idx = 0U;

  /*
   *               ######## Disable I2C Instance ########
   */

  /* Set I2C instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Mode                = LPBAM_I2C_MASTER_RECEIVE_MODE;
  config_node.Config.State               = DISABLE ;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build disable node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert disable node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## Set I2C Timing ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_TIMING_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Timing            = pRxConfig->Timing;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build timing node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert timing node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## Select I2C Trigger Source ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID             = (uint32_t)LPBAM_I2C_TRIG_ID;
  config_node.NodeDesc.pSrcVarReg                  = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.AutoModeConf.TriggerState     = pRxConfig->AutoModeConf.TriggerState;
  config_node.Config.AutoModeConf.TriggerSelection = pRxConfig->AutoModeConf.TriggerSelection;
  config_node.Config.AutoModeConf.TriggerPolarity  = pRxConfig->AutoModeConf.TriggerPolarity;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build trigger source node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert trigger source node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## Enable I2C Instance ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.State             = ENABLE ;
  config_node.Config.WakeupIT          = pRxConfig->WakeupIT;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build enable node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert enable node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the data of I2C master receive according to parameters in
  *         LPBAM_I2C_DataAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pRxData      : [IN]  Pointer to a LPBAM_I2C_DataAdvConf_t structure that contains receive data nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_I2C_MasterRxDataDesc_t structure that contains receive data
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_MasterRx_SetDataQ(I2C_TypeDef                  *const pInstance,
                                               LPBAM_DMAListInfo_t          const *const pDMAListInfo,
                                               LPBAM_I2C_DataAdvConf_t      const *const pRxData,
                                               LPBAM_I2C_MasterRxDataDesc_t *const pDescriptor,
                                               DMA_QListTypeDef             *const pQueue)
{
  LPBAM_I2C_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  static uint8_t is_reload_data = 0U;
  static uint32_t buffer_offset = 0U;
  uint8_t idx                   = 0U;
  uint32_t buffer_address;

  /*
   *               ######## Setup reload data variables ########
   */

  /* Check if data size > Max I2C data size */
  if (pRxData->Size > LPBAM_I2C_MAX_DATA_SIZE)
  {
    /* Update reload data information */
    config_node.Config.ReloadDataState  = ENABLE;
    config_node.Config.IsLastReloadData = LPBAM_I2C_IS_NOT_LAST_RELOADED_DATA;

    if (is_reload_data == 1U)
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
    }
    else
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_FIRST_RELOADED_DATA;

      /* Update reload variable */
      is_reload_data = 1U;
    }
  }
  /* Data size < 255 */
  else
  {
    /* Transfer data is reloaded  */
    if (is_reload_data == 1U)
    {
      /* Enable reload data */
      config_node.Config.ReloadDataState = ENABLE;

      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
      config_node.Config.IsLastReloadData  = LPBAM_I2C_IS_LAST_RELOADED_DATA;

      /* Reset reload data variable */
      is_reload_data = 0U;
    }
    /* Transfer data is not reloaded  */
    else
    {
      /* Disable reload data */
      config_node.Config.ReloadDataState = DISABLE;
    }
  }


  /*
   *               ######## Setup sequential data variables ########
   */

  /* Transfer data is sequential */
  if (pRxData->SequenceNumber > 1U)
  {
    /* Enable sequential data transfer */
    config_node.Config.SequentialDataState = ENABLE;

    /* First sequential data transfer */
    if (SequentialIdx == 1U)
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx++;
    }
    /* Last sequential data transfer */
    else if ((SequentialIdx == pRxData->SequenceNumber) && (is_reload_data == 0U))
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx = 1U;
    }
    else
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;

      /* Check if reload data is disabled */
      if (is_reload_data == 0U)
      {
        /* Update sequential variable */
        SequentialIdx++;
      }
    }
  }
  /* Transfer data is not sequential */
  else
  {
    /* Disable sequential data */
    config_node.Config.SequentialDataState = DISABLE;
  }


  /*
   *               ######## Configure I2C transfer  ########
   */

  /* Set I2C instance */
  config_node.pInstance                        = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID         = (uint32_t)LPBAM_I2C_CONFIG_TRANSACTION_ID;
  config_node.NodeDesc.NodeInfo.NodeType       = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg              = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Mode                      = LPBAM_I2C_MASTER_RECEIVE_MODE;
  config_node.Config.DevAddress                = pRxData->DevAddress;
  config_node.Config.Size                      = pRxData->Size;
  config_node.Config.AddressingMode            = pRxData->AddressingMode;
  config_node.Config.AutoModeConf.TriggerState = pRxData->AutoModeConf.TriggerState;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configuration node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## I2C Receive Data ########
   */

  /* Get data buffer address */
  buffer_address                       = (uint32_t)&pRxData->pData[buffer_offset];

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_RECEIVE_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build receive data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert receive data to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Reload data transfer */
  if (is_reload_data == 1U)
  {
    buffer_offset += LPBAM_I2C_MAX_DATA_SIZE;
  }
  /* Reload data transfer */
  else
  {
    /* Reset buffer offset variable */
    buffer_offset = 0U;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the configuration and data of I2C master receive according to
  *         parameters in LPBAM_I2C_FullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pRxFull      : [IN]  Pointer to a LPBAM_I2C_FullAdvConf_t structure that contains receive full nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_I2C_MasterRxFullDesc_t structure that contains receive full
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_MasterRx_SetFullQ(I2C_TypeDef                  *const pInstance,
                                               LPBAM_DMAListInfo_t          const *const pDMAListInfo,
                                               LPBAM_I2C_FullAdvConf_t      const *const pRxFull,
                                               LPBAM_I2C_MasterRxFullDesc_t *const pDescriptor,
                                               DMA_QListTypeDef             *const pQueue)
{
  LPBAM_I2C_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  static uint8_t is_reload_data = 0U;
  static uint32_t buffer_offset = 0U;
  uint8_t idx                   = 0U;
  uint32_t buffer_address;

  /*
   *               ######## Setup reload data variables ########
   */

  /* Check if data size > Max I2C data size */
  if (pRxFull->Size > LPBAM_I2C_MAX_DATA_SIZE)
  {
    /* Update reload data information */
    config_node.Config.ReloadDataState  = ENABLE;
    config_node.Config.IsLastReloadData = LPBAM_I2C_IS_NOT_LAST_RELOADED_DATA;

    if (is_reload_data == 1U)
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
    }
    else
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_FIRST_RELOADED_DATA;

      /* Update reload variable */
      is_reload_data = 1U;
    }
  }
  /* Data size < 255 */
  else
  {
    /* Transfer data is reloaded  */
    if (is_reload_data == 1U)
    {
      /* Enable reload data */
      config_node.Config.ReloadDataState = ENABLE;

      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
      config_node.Config.IsLastReloadData  = LPBAM_I2C_IS_LAST_RELOADED_DATA;

      /* Reset reload data variable */
      is_reload_data = 0U;
    }
    /* Transfer data is not reloaded  */
    else
    {
      /* Disable reload data */
      config_node.Config.ReloadDataState = DISABLE;
    }
  }


  /*
   *               ######## Setup sequential data variables ########
   */

  /* Transfer data is sequential */
  if (pRxFull->SequenceNumber > 1U)
  {
    /* Enable sequential data transfer */
    config_node.Config.SequentialDataState = ENABLE;

    /* First sequential data transfer */
    if (SequentialIdx == 1U)
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx++;
    }
    /* Last sequential data transfer */
    else if ((SequentialIdx == pRxFull->SequenceNumber) && (is_reload_data == 0U))
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx = 1U;
    }
    else
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;

      /* Check if reload data is disabled */
      if (is_reload_data == 0U)
      {
        /* Update sequential variable */
        SequentialIdx++;
      }
    }
  }
  /* Transfer data is not sequential */
  else
  {
    /* Disable sequential data */
    config_node.Config.SequentialDataState = DISABLE;
  }

  /* Check reload and sequential information */
  if (((config_node.Config.ReloadDataState == DISABLE) && (config_node.Config.SequentialDataState == DISABLE)) || \
      ((config_node.Config.SequentialDataState == ENABLE)                                                      && \
       (config_node.Config.IsFirstSequentialData == LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA))                       || \
      ((config_node.Config.SequentialDataState == DISABLE)                                                     && \
       (config_node.Config.IsFirstReloadData == LPBAM_I2C_IS_FIRST_RELOADED_DATA)))
  {
    /*
     *               ######## Disable I2C Instance ########
     */

    /* Set I2C instance */
    config_node.pInstance                  = pInstance;

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_STATE_ID;
    config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
    config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.Mode                = LPBAM_I2C_MASTER_RECEIVE_MODE;
    config_node.Config.State               = DISABLE ;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build disable node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert disable node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;


    /*
     *               ######## Set I2C Timing ########
     */

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_TIMING_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.Timing            = pRxFull->Timing;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build timing node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert timing node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;


    /*
     *               ######## Select I2C Trigger Source ########
     */

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID             = (uint32_t)LPBAM_I2C_TRIG_ID;
    config_node.NodeDesc.pSrcVarReg                  = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.AutoModeConf.TriggerState     = pRxFull->AutoModeConf.TriggerState;
    config_node.Config.AutoModeConf.TriggerSelection = pRxFull->AutoModeConf.TriggerSelection;
    config_node.Config.AutoModeConf.TriggerPolarity  = pRxFull->AutoModeConf.TriggerPolarity;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build trigger source node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert trigger source node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;


    /*
     *               ######## Enable I2C Instance ########
     */

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_STATE_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.State             = ENABLE ;
    config_node.Config.WakeupIT          = pRxFull->WakeupIT;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build enable node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert enable node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;
  }
  else
  {
    /* Update node index */
    idx = I2C_MASTER_DATA_NODEIDX;
  }


  /*
   *               ######## Configure I2C transfer  ########
   */

  /* Set I2C instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_CONFIG_TRANSACTION_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Mode                = LPBAM_I2C_MASTER_RECEIVE_MODE;
  config_node.Config.DevAddress          = pRxFull->DevAddress;
  config_node.Config.Size                = pRxFull->Size;
  config_node.Config.AddressingMode      = pRxFull->AddressingMode;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert configuration node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## I2C Receive Data ########
   */

  /* Get data buffer address */
  buffer_address                       = (uint32_t)&pRxFull->pData[buffer_offset];

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_RECEIVE_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build receive data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert receive data to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Reload data transfer */
  if (is_reload_data == 1U)
  {
    buffer_offset += LPBAM_I2C_MAX_DATA_SIZE;
  }
  /* Reload data transfer */
  else
  {
    /* Reset buffer offset variable */
    buffer_offset = 0U;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the configuration of I2C slave receive according to parameters in
  *         LPBAM_I2C_ConfigAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pRxConfig    : [IN]  Pointer to a LPBAM_I2C_ConfigAdvConf_t structure that contains slave receive
  *                              configuration nodes information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_I2C_SlaveRxConfigDesc_t structure that contains receive
  *                              configuration descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_SlaveRx_SetConfigQ(I2C_TypeDef                   *const pInstance,
                                                LPBAM_DMAListInfo_t           const *const pDMAListInfo,
                                                LPBAM_I2C_ConfigAdvConf_t     const *const pRxConfig,
                                                LPBAM_I2C_SlaveRxConfigDesc_t *const pDescriptor,
                                                DMA_QListTypeDef              *const pQueue)
{
  LPBAM_I2C_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  uint8_t idx = 0U;

  /*
   *               ######## Disable I2C Instance ########
   */

  /* Set I2C instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_STATE_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Mode                = LPBAM_I2C_SLAVE_RECEIVE_MODE;
  config_node.Config.State               = DISABLE ;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build disable node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert disable node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## Set I2C Timing ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_TIMING_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Timing            = pRxConfig->Timing;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build timing node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert timing node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## Enable I2C Instance ########
   */

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_STATE_ID;
  config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.State             = ENABLE ;
  config_node.Config.WakeupIT          = pRxConfig->WakeupIT;

  /* Fill descriptor for lpbam configuration transaction */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build enable node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Insert enable node to I2C Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the data of I2C slave receive according to parameters in
  *         LPBAM_I2C_DataAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pRxData      : [IN]  Pointer to a LPBAM_I2C_DataAdvConf_t structure that contains receive data nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_I2C_SlaveRxDataDesc_t structure that contains receive data
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_SlaveRx_SetDataQ(I2C_TypeDef                 *const pInstance,
                                              LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                              LPBAM_I2C_DataAdvConf_t     const *const pRxData,
                                              LPBAM_I2C_SlaveRxDataDesc_t *const pDescriptor,
                                              DMA_QListTypeDef            *const pQueue)
{
  LPBAM_I2C_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  static uint8_t is_reload_data = 0U;
  static uint32_t buffer_offset = 0U;
  uint8_t idx                   = 0U;
  uint32_t buffer_address;

  /*
   *               ######## Setup reload data variables ########
   */

  /* Check if data size > Max I2C data size */
  if (pRxData->Size > LPBAM_I2C_MAX_DATA_SIZE)
  {
    /* Update reload data information */
    config_node.Config.ReloadDataState  = ENABLE;
    config_node.Config.IsLastReloadData = LPBAM_I2C_IS_NOT_LAST_RELOADED_DATA;

    if (is_reload_data == 1U)
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
    }
    else
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_FIRST_RELOADED_DATA;

      /* Update reload variable */
      is_reload_data = 1U;
    }
  }
  /* Data size < 255 */
  else
  {
    /* Transfer data is reloaded  */
    if (is_reload_data == 1U)
    {
      /* Enable reload data */
      config_node.Config.ReloadDataState = ENABLE;

      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
      config_node.Config.IsLastReloadData  = LPBAM_I2C_IS_LAST_RELOADED_DATA;

      /* Reset reload data variable */
      is_reload_data = 0U;
    }
    /* Transfer data is not reloaded  */
    else
    {
      /* Disable reload data */
      config_node.Config.ReloadDataState = DISABLE;
    }
  }


  /*
   *               ######## Setup sequential data variables ########
   */

  /* Transfer data is sequential */
  if (pRxData->SequenceNumber > 1U)
  {
    /* Enable sequential data transfer */
    config_node.Config.SequentialDataState = ENABLE;

    /* First sequential data transfer */
    if (SequentialIdx == 1U)
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx++;
    }
    /* Last sequential data transfer */
    else if ((SequentialIdx == pRxData->SequenceNumber) && (is_reload_data == 0U))
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx = 1U;
    }
    else
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;

      /* Check if reload data is disabled */
      if (is_reload_data == 0U)
      {
        /* Update sequential variable */
        SequentialIdx++;
      }
    }
  }
  /* Transfer data is not sequential */
  else
  {
    /* Disable sequential data */
    config_node.Config.SequentialDataState = DISABLE;
  }


  /*
   *               ######## Configure I2C transfer ########
   */

  /* Set I2C instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_CONFIG_TRANSACTION_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Mode                = LPBAM_I2C_SLAVE_RECEIVE_MODE;
  config_node.Config.Size                = pRxData->Size;
  config_node.Config.AddressingMode      = pRxData->AddressingMode;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## I2C Receive Data ########
   */

  /* Get data buffer address */
  buffer_address                       = (uint32_t)&pRxData->pData[buffer_offset];

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_RECEIVE_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build receive data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect receive data node to Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Reload data transfer */
  if (is_reload_data == 1U)
  {
    buffer_offset += LPBAM_I2C_MAX_DATA_SIZE;
  }
  /* Reload data transfer */
  else
  {
    /* Reset buffer offset variable */
    buffer_offset = 0U;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to setup the configuration and data of I2C slave receive according to
  *         parameters in LPBAM_I2C_FullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pRxFull      : [IN]  Pointer to a LPBAM_I2C_FullAdvConf_t structure that contains receive full nodes
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_I2C_SlaveRxFullDesc_t structure that contains receive full
  *                              descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_SlaveRx_SetFullQ(I2C_TypeDef                 *const pInstance,
                                              LPBAM_DMAListInfo_t         const *const pDMAListInfo,
                                              LPBAM_I2C_FullAdvConf_t     const *const pRxFull,
                                              LPBAM_I2C_SlaveRxFullDesc_t *const pDescriptor,
                                              DMA_QListTypeDef            *const pQueue)
{
  LPBAM_I2C_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  static uint8_t is_reload_data = 0U;
  static uint32_t buffer_offset = 0U;
  uint8_t idx                   = 0U;
  uint32_t buffer_address;

  /*
   *               ######## Setup reload data variables ########
   */

  /* Check if data size > Max I2C data size */
  if (pRxFull->Size > LPBAM_I2C_MAX_DATA_SIZE)
  {
    /* Update reload data information */
    config_node.Config.ReloadDataState  = ENABLE;
    config_node.Config.IsLastReloadData = LPBAM_I2C_IS_NOT_LAST_RELOADED_DATA;

    if (is_reload_data == 1U)
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
    }
    else
    {
      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_FIRST_RELOADED_DATA;

      /* Update reload variable */
      is_reload_data = 1U;
    }
  }
  /* Data size < 255 */
  else
  {
    /* Transfer data is reloaded  */
    if (is_reload_data == 1U)
    {
      /* Enable reload data */
      config_node.Config.ReloadDataState = ENABLE;

      /* Update reload data information */
      config_node.Config.IsFirstReloadData = LPBAM_I2C_IS_NOT_FIRST_RELOADED_DATA;
      config_node.Config.IsLastReloadData  = LPBAM_I2C_IS_LAST_RELOADED_DATA;

      /* Reset reload data variable */
      is_reload_data = 0U;
    }
    /* Transfer data is not reloaded  */
    else
    {
      /* Disable reload data */
      config_node.Config.ReloadDataState = DISABLE;
    }
  }


  /*
   *               ######## Setup sequential data variables ########
   */

  /* Transfer data is sequential */
  if (pRxFull->SequenceNumber > 1U)
  {
    /* Enable sequential data transfer */
    config_node.Config.SequentialDataState = ENABLE;

    /* First sequential data transfer */
    if (SequentialIdx == 1U)
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx++;
    }
    /* Last sequential data transfer */
    else if ((SequentialIdx == pRxFull->SequenceNumber) && (is_reload_data == 0U))
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_LAST_SEQUENTIAL_DATA;
      /* Update sequential variable */
      SequentialIdx = 1U;
    }
    else
    {
      /* Update sequential data information */
      config_node.Config.IsFirstSequentialData = LPBAM_I2C_IS_NOT_FIRST_SEQUENTIAL_DATA;
      config_node.Config.IsLastSequentialData  = LPBAM_I2C_IS_NOT_LAST_SEQUENTIAL_DATA;

      /* Check if reload data is disabled */
      if (is_reload_data == 0U)
      {
        /* Update sequential variable */
        SequentialIdx++;
      }
    }
  }
  /* Transfer data is not sequential */
  else
  {
    /* Disable sequential data */
    config_node.Config.SequentialDataState = DISABLE;
  }

  /* Check reload and sequential information */
  if (((config_node.Config.ReloadDataState == DISABLE) && (config_node.Config.SequentialDataState == DISABLE)) || \
      ((config_node.Config.SequentialDataState == ENABLE)                                                      && \
       (config_node.Config.IsFirstSequentialData == LPBAM_I2C_IS_FIRST_SEQUENTIAL_DATA))                       || \
      ((config_node.Config.SequentialDataState == DISABLE)                                                     && \
       (config_node.Config.IsFirstReloadData == LPBAM_I2C_IS_FIRST_RELOADED_DATA)))
  {
    /*
     *               ######## Disable I2C Instance ########
     */

    /* Set I2C instance */
    config_node.pInstance                  = pInstance;

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_STATE_ID;
    config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
    config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.Mode                = LPBAM_I2C_SLAVE_RECEIVE_MODE;
    config_node.Config.State               = DISABLE ;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build disable node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert disable node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;


    /*
     *               ######## Set I2C Timing ########
     */

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_TIMING_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.Timing            = pRxFull->Timing;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build timing node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert timing node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;


    /*
     *               ######## Enable I2C Instance ########
     */

    /* Set node descriptor */
    config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_STATE_ID;
    config_node.NodeDesc.pSrcVarReg      = &pDescriptor->pReg[idx];

    /* Set specific parameters for node creation */
    config_node.Config.State             = ENABLE ;
    config_node.Config.WakeupIT          = pRxFull->WakeupIT;

    /* Fill descriptor for lpbam configuration transaction */
    if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
    {
      return LPBAM_ERROR;
    }

    /* Build enable node */
    if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Insert enable node to I2C Queue */
    if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
    {
      return LPBAM_ERROR;
    }

    /* Update node index */
    idx++;
  }
  else
  {
    /* Update node index */
    idx = I2C_SLAVE_DATA_NODEIDX;
  }


  /*
   *               ######## Configure I2C transfer ########
   */

  /* Set I2C instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_CONFIG_TRANSACTION_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.Mode                = LPBAM_I2C_SLAVE_RECEIVE_MODE;
  config_node.Config.Size                = pRxFull->Size;
  config_node.Config.AddressingMode      = pRxFull->AddressingMode;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build configuration node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect configuration node to Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update node index */
  idx++;


  /*
   *               ######## I2C Receive Data ########
   */

  /* Get data buffer address */
  buffer_address                       = (uint32_t)&pRxFull->pData[buffer_offset];

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID = (uint32_t)LPBAM_I2C_RECEIVE_DATA_ID;
  config_node.NodeDesc.pBuff           = (uint32_t *)buffer_address;

  /* Fill node configuration */
  if (LPBAM_I2C_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build receive data node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect receive data node to Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[idx]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Reload data transfer */
  if (is_reload_data == 1U)
  {
    buffer_offset += LPBAM_I2C_MAX_DATA_SIZE;
  }
  /* Reload data transfer */
  else
  {
    /* Reset buffer offset variable */
    buffer_offset = 0U;
  }

  return LPBAM_OK;
}

/**
  * @brief  Enable the I2C DMA requests.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_EnableDMARequests(I2C_TypeDef *const pInstance)
{
  /* Enable I2C reload and sequential events DMA request */
  pInstance->AUTOCR |= LPBAM_I2C_TCDMAEN | LPBAM_I2C_TCRDMAEN;

  /* Set I2C DMA requests and, auto clear flags and wake up enable */
  pInstance->CR1 |= LPBAM_I2C_RXDMAEN | LPBAM_I2C_TXDMAEN | LPBAM_I2C_WUPEN;

  /* Check if stop detection interrupt is not enabled */
  if ((pInstance->CR1 & LPBAM_I2C_IT_STOP) != LPBAM_I2C_IT_STOP)
  {
    /* Enable STOP detection flag automatic clear */
    pInstance->CR1 |= LPBAM_I2C_STOPFACLR;
  }
  else
  {
    /* Disable STOP detection flag automatic clear */
    pInstance->CR1 &= ~(LPBAM_I2C_STOPFACLR);
  }

  /* Check if address match interrupt is not enabled */
  if ((pInstance->CR1 & LPBAM_I2C_IT_ADDRI) != LPBAM_I2C_IT_ADDRI)
  {
    /* Enable Address match flag automatic clear */
    pInstance->CR1 |= LPBAM_I2C_ADDRACLR;
  }
  else
  {
    /* Disable Address match flag automatic clear */
    pInstance->CR1 &= ~(LPBAM_I2C_ADDRACLR);
  }

  return LPBAM_OK;
}

#if defined (HAL_I2C_MODULE_ENABLED)
/**
  * @brief  Register I2C transfer IRQ handler function.
  * @param  hi2c         : [IN]  Pointer to a I2C_HandleTypeDef structure that contains the configuration information
  *                              for the specified I2C.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_RegisterISR(I2C_HandleTypeDef *const hi2c)
{
  /* Store ISR */
  hi2c->XferISR = I2C_ISR;

  return LPBAM_OK;
}

/**
  * @brief  Interrupt Sub-Routine which handle the interrupt flags.
  * @param  hi2c         : [IN]  Pointer to a I2C_HandleTypeDef structure that contains the configuration information
  *                              for the specified I2C.
  * @param  ITFlags      : [IN]  Interrupt flags to handle.
  * @param  ITSources    : [IN]  ITSources interrupt sources enabled.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
static HAL_StatusTypeDef I2C_ISR(struct __I2C_HandleTypeDef *hi2c, uint32_t ITFlags, uint32_t ITSources)
{
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
  uint8_t transferdirection = I2C_GET_DIR(hi2c);
  uint16_t slaveaddrcode    = I2C_GET_ADDR_MATCH(hi2c);
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */

  /* Check for active interrupts */
  if ((ITSources & ITFlags) != 0U)
  {
    /* Check whether NACK interrupt is enabled or not */
    if ((ITSources & LPBAM_I2C_IT_NACKI) == LPBAM_I2C_IT_NACKI)
    {
      /* Check if NACK received flag is active */
      if (I2C_CHECK_FLAG(ITFlags, I2C_ISR_NACKF) != RESET)
      {
        /* Clear NACK flag */
        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_ISR_NACKF);
      }
    }

    /* Check whether stop detection interrupt is enabled or not */
    if ((ITSources & LPBAM_I2C_IT_STOP) == LPBAM_I2C_IT_STOP)
    {
      /* Check if STOP flag is active */
      if (I2C_CHECK_FLAG(ITFlags, I2C_ISR_STOPF) != RESET)
      {
        /* Clear stop flag */
        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_ISR_STOPF);

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        /* Call slave Tx complete callback */
        if (hi2c->SlaveTxCpltCallback != NULL)
        {
          hi2c->SlaveTxCpltCallback(hi2c);
        }

        /* Call slave Rx complete callback */
        if (hi2c->SlaveRxCpltCallback != NULL)
        {
          hi2c->SlaveRxCpltCallback(hi2c);
        }
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */
      }
    }

    /* Check whether transfer complete interrupt is enabled or not */
    if ((ITSources & LPBAM_I2C_IT_TC) == LPBAM_I2C_IT_TC)
    {
      /* Check if TC flag is active */
      if (I2C_CHECK_FLAG(ITFlags, I2C_ISR_TC) != RESET)
      {
        /* Generate stop condition to clear TC flag */
        hi2c->Instance->CR2 |= I2C_CR2_STOP;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        /* Call master Tx complete callback */
        if (hi2c->MasterTxCpltCallback != NULL)
        {
          hi2c->MasterTxCpltCallback(hi2c);
        }

        /* Call master Rx complete callback */
        if (hi2c->MasterRxCpltCallback != NULL)
        {
          hi2c->MasterRxCpltCallback(hi2c);
        }
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */
      }
    }

    /* Check whether address match interrupt is enabled or not */
    if ((ITSources & LPBAM_I2C_IT_ADDRI) == LPBAM_I2C_IT_ADDRI)
    {
      /* Check if ADDR flag is active */
      if (I2C_CHECK_FLAG(ITFlags, I2C_ISR_ADDR) != RESET)
      {
        /* Clear address match flag */
        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_ISR_ADDR);

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        /* Call address match callback */
        if (hi2c->AddrCallback != NULL)
        {
          hi2c->AddrCallback(hi2c, transferdirection, slaveaddrcode);
        }
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */
      }
    }
  }

  return HAL_OK;
}
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

#endif /* LPBAM_I2C_MODULE_ENABLED */
