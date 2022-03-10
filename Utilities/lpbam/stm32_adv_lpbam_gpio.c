/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_gpio.c
  * @author  MCD Application Team
  * @brief   This file provides the advanced LPBAM layer for GPIO peripheral.
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
          (+) stm32_adv_lpbam_gpio.c file
              (++) This file provides the implementation of the advanced LPBAM GPIO functions.
          (+) stm32_adv_lpbam_gpio.h file
              (++) This file is the header file of stm32_adv_lpbam_gpio.c. It provides advanced LPBAM GPIO functions
                   prototypes and the declaration of their needed exported types and structures.
          (+) STM32xx/stm32_platform_lpbam_gpio.h file
              (++) This header file contains all defines to be used on applicative side.
                   (+++) STM32xx stands for the device supporting LPBAM sub-system.

    *** Driver functions model ***
    ==============================
    [..]
      This section precises this module supported advanced functions model (refer to LPBAM_Utility_GettingStarted.html
      document for function model definition).

    [..]
      This driver provides 1 model of API :
          (+) ADV_LPBAM_{Module}_{Mode}_SetFullQ() : provides one peripheral configuration queue.

    *** Driver features ***
    =======================
    [..]
      This section describes this LPBAM module supported features.

    [..]
      This driver provides services covering the LPBAM management of the following GPIO features :
             (+) Write a single state to GPIO pin(s).
             (+) Write a sequence of states to GPIO pin(s).
             (+) Read a sequence of states from GPIO pin(s).

    *** Functional description ***
    ==============================
    [..]
      This section describes the peripheral features covered by this LPBAM module.

    [..]
      The output of this driver is a queue to be executed by a DMA channel.

      Write a single or a sequence of states to GPIO pin(s).
      Read a sequence of GPIO pin(s) states.

    *** Driver APIs description ***
    ===============================
    [..]
      This section provides LPBAM module exhaustive APIs description without considering application user call sequence.
      For user call sequence information, please refer to 'Driver user sequence' section below.

    [..]
      Use ADV_LPBAM_GPIO_WritePin_SetFullQ() API to build a linked-list queue that write a single state to a GPIO pin(s)
      according to parameters in the LPBAM_GPIO_WritePinAdvConf_t structure.
      This API allow to write the state of one or multiple pin(s). The pin(s) state is written one shot.
      The configuration parameters are :
          (+) Pin      : Specifies the GPIO pin(s) to write.
                         This parameter can be a value or a combination of @ref LPBAM_GPIO_pins.
          (+) PinState : Specifies the value to be written to the selected pin(s).
                         This parameter can be one of the LPBAM_GPIO_PinState.
      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_WORD.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_GPIO_WritePin_SetFullQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module stm32_adv_lpbam_common.c.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_WORD.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD.

    [..]
      Use ADV_LPBAM_GPIO_WritePinSequence_SetFullQ() API to build a linked-list queue that writes a sequence of states
      to a GPIO pin(s) according to parameters in the LPBAM_GPIO_PinSeqFullAdvConf_t structure.
      This API allow to write the state of one or multiple pin(s). The pin(s) state can be written one shot or several
      times according to the number of times programmed in the Size field.
      The configuration parameters are :
          (+) Pin   : Specifies the GPIO pin(s) to write.
                      This parameter can be a value or a combination of @ref LPBAM_GPIO_pins.
          (+) Size  : Specifies how many times, the pin(s) state will be written.
          (+) pData : Specifies the data buffer address, where the pin(s) state(s) are stored.
      The data transfer default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_WORD.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_GPIO_WritePinSequence_SetFullQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module
      stm32_adv_lpbam_common.c.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) DestInc           : DMA_DINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_WORD.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD.

    [..]
      Use ADV_LPBAM_GPIO_ReadPinSequence_SetFullQ() API to build a linked-list queue that reads a sequence of states
      of GPIO pin(s) according to the specified parameters in the LPBAM_GPIO_PinSeqFullAdvConf_t structure.
      This API allow to read the state of one or multiple pin(s). The pin(s) state can be read one shot or several times
      according to the number of times programmed in the Size field.
      The configuration parameters are :
          (+) Pin   : Specifies the GPIO pin(s) to read.
                      This parameter can be a value or a combination of @ref LPBAM_GPIO_pins.
          (+) Size  : Specifies how many times, the pin(s) state will be read.
          (+) pData : Specifies the data buffer address, where the pin(s) state(s) will be stored.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD.
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_HALFWORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For TrustZone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For TrustZone devices)
      The data transfer configuration can be customized using ADV_LPBAM_Q_SetDataConfig() after
      ADV_LPBAM_GPIO_WritePinSequence_SetFullQ(). The ADV_LPBAM_Q_SetDataConfig() is in the module
      stm32_adv_lpbam_common.c.
      Because of the HW implementation, the following parameters should be kept at default value:
          (+) SrcInc            : DMA_SINC_FIXED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD.
      When reading more then one pin, each data contains the state of all selected pins. Within each data, use
      __LPBAM_GPIO_STATE() macro to get separately the state of each selected GPIO pin.

      These APIs must be called when the GPIO is initialized.
          (+) Recommended GPIO initialization sequence
              (++) call HAL_GPIO_Init to initialize the GPIO pin(s). (Mandatory)
                   Initialization parameters can be :
                   (+++) Pin       : Any.
                   (+++) Mode      : Input or Output according to application needs. (Mandatory)
                   (+++) Pull      : Any.
                   (+++) Speed     : Any.
                   (+++) Alternate : Any.

    *** Driver user sequence ***
    ============================
    [..]
      This section provides the steps to follow to build an LPBAM application based on HAL/LL and LPBAM drivers. (refer
      to LPBAM_Utility_GettingStarted.html for linked-list feature description).

    [..]
      Write Pin(s) user sequence is :
          (+) Configure all needed GPIO pin(s) as output mode (Using HAL/LL). (Mandatory)
          (+) There are two possibilities to call advanced API:
              (++) Write one shot state :
                   (+++) Call ADV_LPBAM_GPIO_WritePin_SetFullQ(). (Mandatory)
              (++) Write sequence of states :
                   (+++) Call ADV_LPBAM_GPIO_WritePinSequence_SetFullQ(). (Mandatory)
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_GPIO_WritePin_SetFullQ or ADV_LPBAM_GPIO_WritePinSequence_SetFullQ()
              output queue.
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
      Read Pin(s) user sequence is :
          (+) Configure all needed GPIO pin(s) as input mode (Using HAL/LL). (Mandatory)
          (+) Call ADV_LPBAM_GPIO_ReadPinSequence_SetFullQ() to read one or sequence of states of GPIO pin(s).
              (Mandatory)
          (+) Call, optionally, ADV_LPBAM_Q_SetTriggerConfig() in stm32_adv_lpbam_common.c to add hardware trigger
              condition for executing of ADV_LPBAM_GPIO_ReadPinSequence_SetFullQ() output queue.
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
      Ensure the consistency between GPIO pin(s) configuration (input or output) and used advanced APIs. (refer to
      'Driver user sequence' to show different user sequence for input and output GPIO)

    [..]
      It is mandatory to use only ADV_LPBAM_GPIO_WritePin_SetFullQ() and/or ADV_LPBAM_GPIO_WritePinSequence_SetFullQ()
      for the GPIO pin(s) configured as output.

    [..]
      By default, the cadency of reading and writing GPIO pin(s) is equal to the speed of the DMA channel access.
      To customize the reading and writing cadency, it is recommended to use a DMA channel hardware trigger signal
      through ADV_LPBAM_Q_SetTriggerConfig().

    [..]
      It's forbidden to execute simultaneously the same linked-list queue with different DMA channels.

    [..]
      It's forbidden to execute simultaneously two linked-list queue using the same GPIO pin.

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

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @defgroup GPIO_Advanced GPIO_Advanced
  * @brief    GPIO Advanced LPBAM module driver
  * @{
  */

#ifdef LPBAM_GPIO_MODULE_ENABLED

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_GPIO_Advanced_Exported_Functions LPBAM GPIO Advanced Exported Functions
  * @{
  */

/**
  * @brief  Build DMA linked-list queue to write pin(s) state according to parameters in the
  *         LPBAM_GPIO_WritePinFullAdvConf_t.
  * @param  pInstance    : [IN]  Pointer to a GPIO_TypeDef structure that selects GPIO port.
  * @param  pDMAListInfo : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and linked-list
  *                              queue type information.
  * @param  pWritePinFull: [IN]  Pointer to a LPBAM_GPIO_WritePinFullAdvConf_t structure that contains write pin
  *                              information.
  * @param  pDescriptor  : [IN]  Pointer to a LPBAM_GPIO_WritePinFullDesc_t structure that contains write pin descriptor
  *                              information.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_GPIO_WritePin_SetFullQ(GPIO_TypeDef                     *const pInstance,
                                                LPBAM_DMAListInfo_t              const *const pDMAListInfo,
                                                LPBAM_GPIO_WritePinFullAdvConf_t const *const pWritePinFull,
                                                LPBAM_GPIO_WritePinFullDesc_t    *const pDescriptor,
                                                DMA_QListTypeDef                 *const pQueue)
{
  LPBAM_GPIO_ConfNode_t config_node;
  DMA_NodeConfTypeDef   dma_node_conf;

  /*
   *               ######## GPIO write pin node ########
   */

  /* Set GPIO instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_GPIO_WRITEPIN_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pSrcVarReg        = &pDescriptor->pReg[0U];

  /* Set GPIO configuration */
  config_node.Config.Pin                 = pWritePinFull->Pin;
  config_node.Config.PinState            = pWritePinFull->PinState;
  config_node.Config.IsSequenceMode      = LPBAM_GPIO_IS_SINGLE_MODE;

  /* Fill node configuration */
  if (LPBAM_GPIO_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build write pin node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect write pin node to GPIO Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to write a sequence of pin(s) state(s) according to parameters in the
  *         LPBAM_GPIO_PinSeqFullAdvConf_t.
  * @param  pInstance        : [IN]  Pointer to a GPIO_TypeDef structure that selects GPIO port.
  * @param  pDMAListInfo     : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance and
  *                                  linked-list queue type information.
  * @param  pWritePinSeqFull : [IN]  Pointer to a LPBAM_GPIO_PinSeqFullAdvConf_t structure that contains write pin
  *                                  sequence information.
  * @param  pDescriptor      : [IN]  Pointer to a LPBAM_GPIO_WritePinSeqFullDesc_t structure that contains write pin
  *                                  sequence descriptor information.
  * @param  pQueue           : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                                  information.
  * @retval LPBAM Status     : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_GPIO_WritePinSequence_SetFullQ(GPIO_TypeDef                     *const pInstance,
                                                        LPBAM_DMAListInfo_t              const *const pDMAListInfo,
                                                        LPBAM_GPIO_PinSeqFullAdvConf_t   const *const pWritePinSeqFull,
                                                        LPBAM_GPIO_WritePinSeqFullDesc_t *const pDescriptor,
                                                        DMA_QListTypeDef                 *const pQueue)
{
  LPBAM_GPIO_ConfNode_t config_node;
  DMA_NodeConfTypeDef   dma_node_conf;

  /*
   *               ######## GPIO write pin node ########
   */

  /* Set GPIO instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_GPIO_WRITEPIN_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pBuff             = (uint32_t *)pWritePinSeqFull->pData;

  /* Set GPIO configuration */
  config_node.Config.Pin                 = pWritePinSeqFull->Pin;
  config_node.Config.Size                = pWritePinSeqFull->Size;
  config_node.Config.IsSequenceMode      = LPBAM_GPIO_IS_SEQUENCE_MODE;

  /* Fill node configuration */
  if (LPBAM_GPIO_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build write pin sequence node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect write pin sequence node to GPIO Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Build DMA linked-list queue to read a sequence of pin(s) state(s) according to parameters in the
  *         LPBAM_GPIO_PinSeqFullAdvConf_t.
  * @param  pInstance       : [IN]  Pointer to a GPIO_TypeDef structure that selects GPIO port.
  * @param  pDMAListInfo    : [IN]  Pointer to a LPBAM_DMAListInfo_t structure that contains DMA instance
  *                                 and linked-list queue type information.
  * @param  pReadPinSeqFull : [IN]  Pointer to a LPBAM_GPIO_PinSeqFullAdvConf_t structure that contains read pin
  *                                 sequence information.
  * @param  pDescriptor     : [IN]  Pointer to a LPBAM_GPIO_ReadPinFullSeqDesc_t structure that contains read pin
  *                                 sequence descriptor information.
  * @param  pQueue          : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                                 information.
  * @retval LPBAM Status    : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_GPIO_ReadPinSequence_SetFullQ(GPIO_TypeDef                    *const pInstance,
                                                       LPBAM_DMAListInfo_t             const *const pDMAListInfo,
                                                       LPBAM_GPIO_PinSeqFullAdvConf_t  const *const pReadPinSeqFull,
                                                       LPBAM_GPIO_ReadPinSeqFullDesc_t *const pDescriptor,
                                                       DMA_QListTypeDef                *const pQueue)
{
  LPBAM_GPIO_ConfNode_t config_node;
  DMA_NodeConfTypeDef   dma_node_conf;

  /*
   *               ######## GPIO read pin node ########
   */

  /* Set GPIO instance */
  config_node.pInstance                  = pInstance;

  /* Set node descriptor */
  config_node.NodeDesc.NodeInfo.NodeID   = (uint32_t)LPBAM_GPIO_READPIN_ID;
  config_node.NodeDesc.NodeInfo.NodeType = pDMAListInfo->QueueType;
  config_node.NodeDesc.pBuff             = (uint32_t *)pReadPinSeqFull->pData;

  /* Set GPIO configuration */
  config_node.Config.Size                = pReadPinSeqFull->Size;

  /* Fill node configuration */
  if (LPBAM_GPIO_FillNodeConfig(&config_node, &dma_node_conf) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  /* Build read pin sequence node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Connect read pin node sequence to GPIO Queue */
  if (HAL_DMAEx_List_InsertNode_Tail(pQueue, &pDescriptor->pNodes[0U]) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

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
#endif /* LPBAM_GPIO_MODULE_ENABLED */
