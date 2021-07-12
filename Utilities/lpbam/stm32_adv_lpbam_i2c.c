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
      It is strongly recommended to read carefully the GettingStarted.html document before starting developing an LPBAM
      application.

    *** Driver description ***
    ==========================
    [..]
      This driver is dedicated for I2C that can change registers configuration through a DMA linked-list queue instead
      of using CPU, which allows I2C peripheral to operate in low power mode.
      This advanced LPBAM module counts 3 files :
          (+) stm32_adv_lpbam_i2c.c
              (++) This file provides the I2C advanced files body.
          (+) stm32_adv_lpbam_i2c.h
              (++) This file is the header file of stm32_adv_lpbam_i2c.c It provides used types
          (+) stm32_platform_lpbam_i2c.h
              (++) This header file contains all defines to be used within applicative side.

    *** Driver features ***
    =======================
    [..]
      This driver provides the following list of features :
          (+) Configure the I2C transfers in master and slave modes.
          (+) Starts the I2C transfers in master and slave modes.
          (+) Configure and starts the I2C transfers in master and slave modes.
          (+) Manage sequential transfers on each driver API.
          (+) Manage reloaded transfers on each driver API.

    *** Functional description ***
    ==============================
    [..]
      The I2C peripheral transfer is configured through a DMA channel thanks to a build DMA linked-list queue in master
      and slave mode.
      The I2C peripheral transfer is started through a DMA channel thanks to a build DMA linked-list queue in master
      and slave mode.
      The I2C peripheral transfer is configured and started through a DMA channel thanks to a build DMA linked-list
      queue in master and slave mode.

      The output of this driver is a queue to be executed by the DMA channel.

    [..]
      This driver allows to operate several transfers by setting SequenceNumber parameter and handle a transfers with
      data size greater than 255 bytes thanks to reload mode.

    [..]
      SequenceNumber parameter should reflect the number of expected transfers, otherwise the bus will remain in busy
      state.

    [..]
      When sequential transfer is activated (SequenceNumber > 1), a transfer with data size greater than 255 bytes is
      considered as one sequence and ADV_LPBAM_I2C_[Master/Slave][Transmit/Receive]_SetDataQ() should be called x time
      where x = SequenceNumber.
      The same when using ADV_LPBAM_I2C_[Master/Slave][Transmit/Receive]_SetFullQ() API. Refer to Driver user sequence
      for further information about APIs usage.

    [..]
      The maximum data size could be sent at each call is 255 bytes. Consequently when data size is greater than 255,
      the ADV_LPBAM_I2C_[Master/Slave][Transmit/Receive]_SetDataQ() API should be called y time where y is the integer
      part of ((Size)/255) + 1 (same when using ADV_LPBAM_I2C_[Master/Slave][Transmit/Receive]_SetFullQ() API).
      Refer to Driver user sequence for further information about APIs usage.

    *** Driver APIs description ***
    ===============================
    [..]
      Use ADV_LPBAM_I2C_MasterTransmit_SetConfigQ() API to build a linked-list queue that setup the I2C master
      configuration transmission operation according to parameters in the LPBAM_I2C_ConfigAdvConf_t structure.

    [..]
      Use ADV_LPBAM_I2C_MasterTransmit_SetDataQ() API to build a linked-list queue that setup the I2C master
      transmission starting operation according to parameters in the LPBAM_I2C_DataAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_I2C_MasterTransmit_SetFullQ() API to build a linked-list queue that setup the I2C master
      configuration and transmission starting operation according to parameters in the LPBAM_I2C_FullAdvConf_t
      structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_I2C_SlaveTransmit_SetConfigQ() API to build a linked-list queue that setup the I2C slave
      configuration transmission operation according to parameters in the LPBAM_I2C_ConfigAdvConf_t structure.

    [..]
      Use ADV_LPBAM_I2C_SlaveTransmit_SetDataQ() API to build a linked-list queue that setup the I2C slave
      transmission starting operation according to parameters in the LPBAM_I2C_DataAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_I2C_SlaveTransmit_SetFullQ() API to build a linked-list queue that setup the I2C slave
      configuration and transmission starting operation according to parameters in the LPBAM_I2C_FullAdvConf_t
      structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_I2C_MasterReceive_SetConfigQ() API to build a linked-list queue that setup the I2C master
      configuration reception operation according to parameters in the LPBAM_I2C_ConfigAdvConf_t structure.

    [..]
      Use ADV_LPBAM_I2C_MasterReceive_SetDataQ() API to build a linked-list queue that setup the I2C master
      reception starting operation according to parameters in the LPBAM_I2C_DataAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_I2C_MasterReceive_SetFullQ() API to build a linked-list queue that setup the I2C master
      configuration and reception starting operation according to parameters in the LPBAM_I2C_FullAdvConf_t
      structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_I2C_SlaveReceive_SetConfigQ() API to build a linked-list queue that setup the I2C slave
      configuration reception operation according to parameters in the LPBAM_I2C_ConfigAdvConf_t structure.

    [..]
      Use ADV_LPBAM_I2C_SlaveReceive_SetDataQ() API to build a linked-list queue that setup the I2C slave
      reception starting operation according to parameters in the LPBAM_I2C_DataAdvConf_t structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_I2C_SlaveReceive_SetFullQ() API to build a linked-list queue that setup the I2C slave
      configuration and reception starting operation according to parameters in the LPBAM_I2C_FullAdvConf_t
      structure.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_BYTE.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_BYTE. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_I2C_MasterStopGeneration_SetFullQ() API to build a linked-list queue that setup the I2C master
      stop condition generating.

    [..]
      Configured parameters through LPBAM_I2C_ConfigAdvConf_t are :
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
      Configured parameters through LPBAM_I2C_DataAdvConf_t are :
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
      Configured parameters through LPBAM_I2C_FullAdvConf_t are :
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
      These APIs must be called when the I2C is well initialized.
          (+) Recommended I2C initialization sequence
              (++) call HAL_I2C_Init to initialize the I2C. (Mandatory)
                   Initialization parameters can be :
                   (+++) Timing                  : Any.
                   (+++) AddressingMode          : Any.
                   (+++) DualAddressMode         : I2C_DUALADDRESS_DISABLE.
                   (+++) OwnAddress2Masks        : I2C_OA2_NOMASK.
                   (+++) GeneralCallMode         : I2C_GENERALCALL_DISABLE.
                   (+++) NoStretchMode           : I2C_NOSTRETCH_DISABLE.
                   (+++) OwnAddress1             : Any.
                   (+++) OwnAddress2             : Any.
              (++) call HAL_I2CEx_SetConfigAutonomousMode to configure the autonomous mode.
                   (+++) TriggerState     : Any.
                   (+++) TriggerSelection : Any.
                   (+++) TriggerPolarity  : Any.
              (++) call HAL_I2CEx_EnableWakeUp to enable I2C wakeup from Stop mode(s).
              (++) Call __HAL_I2C_ENABLE_IT() to enable error interrupts.
                   I2C error interrupts can be :
                   (+++) I2C_IT_ERRI : Bus error / Arbitration lost error / Overrun/Underrun (slave mode) error.

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
      Generic driver user sequence (no sequential and no reload transfers) is :
          (+) Initialize the I2C (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_I2C_EnableDMARequests() to enable the I2C DMA requests. (Mandatory)
          (+) There are two possibilities to call advanced API:
              (++) Transfer with full APIs :
                   (+++) For transmission mode, call ADV_LPBAM_I2C_[Mode]Transmit_SetFullQ(). (Mandatory)
                   (+++) For reception mode, call ADV_LPBAM_I2C_[Mode]Receive_SetFullQ(). (Mandatory)
              (++) Transfer with configuration and data APIs :
                   (+++) For a transmission mode call ADV_LPBAM_I2C_[Mode]Transmit_SetConfigQ() then
                         ADV_LPBAM_I2C_[Mode]Transmit_SetDataQ(). (Mandatory)
                   (+++) For a reception mode call ADV_LPBAM_I2C_[Mode]Receive_SetConfigQ() then
                         ADV_LPBAM_I2C_[Mode]Receive_SetDataQ(). (Mandatory)
          (+) In master mode, call ADV_LPBAM_I2C_MasterStopGeneration_SetFullQ() to stop the current transfer and free
              the I2C bus.
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode (Using HAL/LL). (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() or HAL_DMAEx_List_Start_IT() to start DMA linked list. (Mandatory)

    [..]
      Sequential transfers driver user sequence is (SequenceNumber > 1) :
          (+) Initialize the I2C (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_I2C_EnableDMARequests() to enable the I2C DMA requests. (Mandatory)
          (+) There are two possibilities to call advanced API:
              (++) Transfer with full APIs :
                   (+++) For transmission mode, call ADV_LPBAM_I2C_[Mode]Transmit_SetFullQ() x times where
                         x = SequenceNumber. (Mandatory)
                   (+++) For reception mode, call ADV_LPBAM_I2C_[Mode]Receive_SetFullQ() x times where
                         x = SequenceNumber. (Mandatory). (Mandatory)
              (++) Transfer with configuration and data APIs :
                   (+++) For a transmission mode call ADV_LPBAM_I2C_[Mode]Transmit_SetConfigQ() only one time then
                         call ADV_LPBAM_I2C_[Mode]Transmit_SetDataQ() x times where x = SequenceNumber. (Mandatory).
                   (+++) For a reception mode call ADV_LPBAM_I2C_[Mode]Receive_SetConfigQ() only one time then
                         call ADV_LPBAM_I2C_[Mode]Receive_SetDataQ() x times where x = SequenceNumber. (Mandatory)
          (+) In master mode, call ADV_LPBAM_I2C_MasterStopGeneration_SetFullQ() to stop the current transfer and free
              the I2C bus.
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode (Using HAL/LL). (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() or HAL_DMAEx_List_Start_IT() to start DMA linked list. (Mandatory)

    [..]
      Reload transfers driver user sequence is (data size > 255) :
          (+) Initialize the I2C (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_I2C_EnableDMARequests() to enable the I2C DMA requests. (Mandatory)
          (+) There are two possibilities to call advanced API:
              (++) Transfer with full APIs :
                   (+++) For transmission mode, call ADV_LPBAM_I2C_[Mode]Transmit_SetFullQ() y time where
                         y is the integer part of ((Size)/255) + 1. (Mandatory)
                   (+++) For reception mode, call ADV_LPBAM_I2C_[Mode]Receive_SetFullQ() y times where
                         y is the integer part of ((Size)/255) + 1. (Mandatory)
              (++) Transfer with configuration and data APIs :
                   (+++) For a transmission mode call ADV_LPBAM_I2C_[Mode]Transmit_SetConfigQ() only one time then
                         call ADV_LPBAM_I2C_[Mode]Transmit_SetDataQ() y times where y is the integer part of
                         ((Size)/255) + 1. (Mandatory).
                   (+++) For a reception mode call ADV_LPBAM_I2C_[Mode]Receive_SetConfigQ() only one time then
                         call ADV_LPBAM_I2C_[Mode]Receive_SetDataQ() y times where y is the integer part of
                         ((Size)/255) + 1. (Mandatory)
          (+) In master mode, call ADV_LPBAM_I2C_MasterStopGeneration_SetFullQ() to stop the current transfer and free
              the I2C bus.
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode (Using HAL/LL). (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() or HAL_DMAEx_List_Start_IT() to start DMA linked list. (Mandatory)

    [..]
      Reload and sequential transfers driver user sequence is (SequenceNumber > 1 && data size > 255):
          (+) Initialize the I2C (Using HAL/LL). (Mandatory)
          (+) Enable I2C to wakeup CPU from Stop mode(s) (Using HAL/LL). (Optional)
          (+) Enable I2C errors interrupt. (Optional)
          (+) Call ADV_LPBAM_I2C_EnableDMARequests() to enable the I2C DMA requests. (Mandatory)
          (+) There are two possibilities to call advanced API:
              (++) Transfer with full APIs :
                   (+++) For transmission mode, call ADV_LPBAM_I2C_[Mode]Transmit_SetFullQ() x times where
                         x = SequenceNumber. Each sequence with data size > 255 should be called y times where y is the
                         integer part of ((Size)/255) + 1. (Mandatory)
                   (+++) For reception mode, call ADV_LPBAM_I2C_[Mode]Receive_SetFullQ() x times where
                         x = SequenceNumber.  Each sequence with data size > 255 should be called y times where y is the
                         integer part of ((Size)/255) + 1. (Mandatory)
              (++) Transfer with configuration and data APIs :
                   (+++) For a transmission mode call ADV_LPBAM_I2C_[Mode]Transmit_SetConfigQ() only one time then
                         call ADV_LPBAM_I2C_[Mode]Transmit_SetDataQ() x times where x = SequenceNumber. Each data
                         sequence with data size > 255 should be called y times where y is the integer part of
                         ((Size)/255) + 1. (Mandatory)
                   (+++) For a reception mode call ADV_LPBAM_I2C_[Mode]Receive_SetConfigQ() only one time then
                         call ADV_LPBAM_I2C_[Mode]Receive_SetDataQ() x times where x = SequenceNumber. Each data
                         sequence with data size > 255 should be called y times where y is the integer part of
                         ((Size)/255) + 1. (Mandatory)
          (+) In master mode, call ADV_LPBAM_I2C_MasterStopGeneration_SetFullQ() to stop the current transfer and free
              the I2C bus.
          (+) Call HAL_DMAEx_List_Init() to initialize a DMA channel in linked-list mode (Using HAL/LL). (Mandatory)
          (+) Call HAL_DMAEx_List_LinkQ() to link the output queue to the initialized DMA channel. (Mandatory)
          (+) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
              Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
              DMA error interrupts can be :
              (++) DMA_IT_DTE : data transfer error.
              (++) DMA_IT_ULE : update link error.
              (++) DMA_IT_USE : user setting error.
          (+) Call HAL_DMAEx_List_Start() or HAL_DMAEx_List_Start_IT() to start DMA linked list. (Mandatory)

    *** Constraints ***
    ===================
    [..]
      It's mandatory to set properly DataSize, TriggerState, TriggerSelection and TriggerPolarity while configuring
      LPBAM_I2C_ConfigAdvConf_t or LPBAM_SPI_FullAdvConf_t because their update can be only done with
      ADV_LPBAM_I2C_{Operation}_SetConfigQ() or ADV_LPBAM_I2C_{Operation}_SetFullQ().

    [..]
      It's strongly not allowed to repeat calling ADV_LPBAM_I2C_(Master/Slave)(Transmit/Receive)_SetConfigQ() before
      starting the LPBAM scenario as it deactivates then activates the I2C.

    [..]
      It's strongly not recommended to use full APIs in a sequential transaction for better memory optimisation usage.

    [..]
      It's strongly not recommended to execute the same linked-list queue that contains I2C configuration and data
      nodes by two different DMA channels as unexpected behavior can appear.

    [..]
      It's strongly not recommended to call advanced I2C APIs by more than one linked-list queue. When the I2C nodes
      will be executed simultaneously an unexpected behavior will appear.

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
/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_I2C_Advanced_Exported_Functions LPBAM I2C Advanced Exported Functions
  * @{
  */

/**
  * @brief  Build the I2C master transmit configuration nodes in DMA linked-list queue according to configured
  *         parameters in LPBAM_I2C_ConfigAdvConf_t.
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
LPBAM_Status_t ADV_LPBAM_I2C_MasterTransmit_SetConfigQ(I2C_TypeDef                    *const pInstance,
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
  * @brief  Build the I2C master transmit data nodes in DMA linked-list queue according to configured parameters in
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
LPBAM_Status_t ADV_LPBAM_I2C_MasterTransmit_SetDataQ(I2C_TypeDef                  *const pInstance,
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
  config_node.Config.StopConditionState        = DISABLE;

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
  * @brief  Build the I2C master transmit full nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_I2C_FullAdvConf_t.
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
LPBAM_Status_t ADV_LPBAM_I2C_MasterTransmit_SetFullQ(I2C_TypeDef                  *const pInstance,
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
  config_node.Config.StopConditionState  = DISABLE;

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
  * @brief  Build the I2C slave transmit configuration nodes in DMA linked-list queue according to configured parameters
  *         in LPBAM_I2C_ConfigAdvConf_t.
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
LPBAM_Status_t ADV_LPBAM_I2C_SlaveTransmit_SetConfigQ(I2C_TypeDef                   *const pInstance,
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
  * @brief  Build the I2C slave transmit data nodes in DMA linked-list queue according to configured parameters in
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
LPBAM_Status_t ADV_LPBAM_I2C_SlaveTransmit_SetDataQ(I2C_TypeDef                 *const pInstance,
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
  config_node.Config.StopConditionState  = DISABLE;

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
  * @brief  Build the I2C slave transmit full nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_I2C_FullAdvConf_t.
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
LPBAM_Status_t ADV_LPBAM_I2C_SlaveTransmit_SetFullQ(I2C_TypeDef                 *const pInstance,
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
  config_node.Config.StopConditionState  = DISABLE;

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
  * @brief  Build the I2C master receive configuration nodes in DMA linked-list queue according to configured parameters
  *         in LPBAM_I2C_ConfigAdvConf_t.
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
LPBAM_Status_t ADV_LPBAM_I2C_MasterReceive_SetConfigQ(I2C_TypeDef                    *const pInstance,
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
  * @brief  Build the I2C master receive data nodes in DMA linked-list queue according to configured parameters in
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
LPBAM_Status_t ADV_LPBAM_I2C_MasterReceive_SetDataQ(I2C_TypeDef                  *const pInstance,
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
  config_node.Config.StopConditionState        = DISABLE;

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
  * @brief  Build the I2C master receive full nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_I2C_FullAdvConf_t.
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
LPBAM_Status_t ADV_LPBAM_I2C_MasterReceive_SetFullQ(I2C_TypeDef                  *const pInstance,
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
  config_node.Config.StopConditionState  = DISABLE;

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
  * @brief  Build the I2C slave receive configuration nodes in DMA linked-list queue according to configured parameters
  *         in LPBAM_I2C_ConfigAdvConf_t.
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
LPBAM_Status_t ADV_LPBAM_I2C_SlaveReceive_SetConfigQ(I2C_TypeDef                   *const pInstance,
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
  * @brief  Build the I2C slave receive data nodes in DMA linked-list queue according to configured parameters in
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
LPBAM_Status_t ADV_LPBAM_I2C_SlaveReceive_SetDataQ(I2C_TypeDef                 *const pInstance,
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
  config_node.Config.StopConditionState  = DISABLE;

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
  * @brief  Build the I2C slave receive full nodes in DMA linked-list queue according to configured parameters in
  *         LPBAM_I2C_FullAdvConf_t.
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
LPBAM_Status_t ADV_LPBAM_I2C_SlaveReceive_SetFullQ(I2C_TypeDef                 *const pInstance,
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
  config_node.Config.StopConditionState  = DISABLE;

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
  * @brief  Build the I2C master stop condition full nodes in DMA linked-list queue.
  * @param  pInstance    : [IN]  Pointer to a I2C_TypeDef structure that selects I2C instance.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_I2C_MasterStopGenDesc_t structure that contains master stop
  *                              condition descriptor information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_I2C_MasterStopGeneration_SetFullQ(I2C_TypeDef                   *const pInstance,
                                                           LPBAM_DMAListInfo_t           const *const pDMAListInfo,
                                                           LPBAM_I2C_MasterStopGenDesc_t *const pDescriptor,
                                                           DMA_QListTypeDef              *const pQueue)
{
  LPBAM_I2C_ConfNode_t config_node;
  DMA_NodeConfTypeDef  dma_node_conf;
  uint8_t idx = 0U;

  /*
   *               ######## I2C Stop Generation ########
   */

  /* Set I2C instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_I2C_CONFIG_TRANSACTION_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[idx];

  /* Set specific parameters for node creation */
  config_node.Config.StopConditionState  = ENABLE;

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
  pInstance->CR1 |= LPBAM_I2C_RXDMAEN | LPBAM_I2C_TXDMAEN | LPBAM_I2C_STOPFACLR | LPBAM_I2C_ADDRACLR | LPBAM_I2C_WUPEN;

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

#endif /* LPBAM_I2C_MODULE_ENABLED */
