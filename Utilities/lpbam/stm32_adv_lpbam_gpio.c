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
      It is strongly recommended to read carefully the GettingStarted.html document before starting developing an LPBAM
      application.

    *** Driver description ***
    ==========================
    [..]
      This driver is dedicated for GPIO that supports Low power feature.
      This advanced LPBAM module counts 3 files :
          (+) stm32_adv_lpbam_gpio.c
              (++) This file provides the GPIO advanced files body.
          (+) stm32_adv_lpbam_gpio.h
              (++) This file is the header file of stm32_adv_lpbam_gpio.c. It provides used types
          (+) stm32_platform_lpbam_gpio.h
              (++) This header file contains all defines to be used in applicative side.

    *** Driver features ***
    =======================
    [..]
      This driver provides the following list of features :
             (+) Write a single state to GPIO pin(s).
             (+) Write a sequence of states to GPIO pin(s).
             (+) Read a sequence of states from GPIO pin(s).

    *** Functional description ***
    ==============================
    [..]
      When a pin (set of pins) of GPIO is configured in output, writing a single state or a state sequence is possible
      thanks to a built DMA linked-list queue.
      When a pin (set of pins) of GPIO is configured in input, reading a single state or a state sequence is possible
      thanks to a built DMA linked-list queue.

      The output of this driver is a queue to be executed by a DMA channel.

    *** Driver APIs description ***
    ===============================
    [..]
      Use ADV_LPBAM_GPIO_WritePin_SetFullQ() API to build a linked-list queue that writes single state to a GPIO pin
     (set of pins) according to parameters in the LPBAM_GPIO_WritePinAdvConf_t structure.
      Configured parameters are :
          (+) Pin      : Specifies the LPGPIO pin to be configured.
                         This parameter can be a value or a combination of @ref LPBAM_GPIO_pins.
          (+) PinState : Specifies the value to be written to the selected pin(s).
                         This parameter can be one of the LPBAM_GPIO_PinState.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED. (Mandatory)
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_WORD. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_GPIO_WritePinSequence_SetFullQ() API to build a linked-list queue that writes a sequence of states
      to a GPIO pin (set of pins) according to parameters in the LPBAM_GPIO_PinSeqFullAdvConf_t structure.
      Configured parameters are :
          (+) Pin   : Specifies the LPGPIO pin to be configured.
                      This parameter can be a value or a combination of @ref LPBAM_GPIO_pins.
          (+) Size  : Specifies the data buffer address.
          (+) pData : Specifies the amount of data to be written.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_INCREMENTED.
          (+) DestInc           : DMA_DINC_FIXED. (Mandatory)
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_WORD. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_WORD. (Mandatory)
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

    [..]
      Use ADV_LPBAM_GPIO_ReadPinSequence_SetFullQ() API to build a linked-list queue that reads a sequence of states
      to a GPIO pin (set of pins) according to parameters in the LPBAM_GPIO_PinSeqFullAdvConf_t structure.
      Configured parameters are :
          (+) Pin   : Specifies the LPGPIO pin to be configured.
                      This parameter can be a value or a combination of @ref LPBAM_GPIO_pins.
          (+) Size  : Specifies the data buffer address.
          (+) pData : Specifies the amount of data to be written.
      The data node default configuration is as follow:
          (+) SrcInc            : DMA_SINC_FIXED. (Mandatory)
          (+) DestInc           : DMA_DINC_INCREMENTED.
          (+) SrcDataWidth      : DMA_SRC_DATAWIDTH_HALFWORD. (Mandatory)
          (+) DestDataWidth     : DMA_DEST_DATAWIDTH_HALFWORD.
          (+) TransferEventMode : DMA_TCEM_LAST_LL_ITEM_TRANSFER.
          (+) SrcSecure         : DMA_CHANNEL_SRC_SEC. (For trust zone devices)
          (+) DestSecure        : DMA_CHANNEL_DEST_SEC. (For trust zone devices)

      These APIs must be called when the GPIO is well initialized.
          (+) Recommended GPIO initialization sequence
              (++) call HAL_GPIO_Init to initialize the GPIO pin(s). (Mandatory)
                   Initialization parameters can be :
                   (+++) Pin       : Any.
                   (+++) Mode      : Input or Output according to application needs.
                   (+++) Pull      : Any.
                   (+++) Speed     : Any.
                   (+++) Alternate : Must be an LPGPIO alternate function in case of LPGPIO pin use.

    *** Driver user sequence ***
    ============================
    [..]
      This driver user sequence is :
          (+) Configure the LPGPIO pin as input or output mode (Using HAL/LL) layer.
          (+) Repeat calling ADV_LPBAM_GPIO_WritePin_SetFullQ() or/and ADV_LPBAM_GPIO_WritePinSequence_SetFullQ() or
              ADV_LPBAM_GPIO_ReadPinSequence_SetFullQ() until completing the LPBAM scenario. (Mandatory)
          (+) Call ADV_LPBAM_Q_SetTriggerConfig() to add hardware trigger condition for executing
              (++) Please check stm32_adv_lpbam_common.c (how to use section) for more information.
          (+) Call ADV_LPBAM_Q_SetCircularMode() to circularize your linked-list queue
              for infinite scenarios cases. (Optional)
          (+) Initialize DMA channel in linked-list mode (Using HAL/LL).(Mandatory)
              (++) Recommended DMA channel initialization sequence
                  (+++) Call HAL_DMAEx_List_Init() to initialize the DMA in linked-list mode.
                  (+++) Call HAL_DMAEx_List_LinkQ() to link the built queue to the DMA channel.
                  (+++) Call __HAL_DMA_ENABLE_IT() to enable error interrupts.
                       Any DMA error occurred in low power block the LPBAM sub-system mechanisms.
                       DMA error interrupts can be :
                       (++++) DMA_IT_DTE : data transfer error.
                       (++++) DMA_IT_ULE : update link error.
                       (++++) DMA_IT_USE : user setting error.
                  (+++) Call HAL_DMAEx_List_Start() to start the DMA execution.

    *** Constraints ***
    ===================
    [..]
      It's strongly not recommended to call any driver API with the same GPIO pin by more than one linked-list queue.
      When the GPIO nodes will be executed simultaneously unexpected behavior will appear.

    [..]
      It's strongly not recommended to execute the same linked-list queue that contains GPIO configuration by two
      different DMA channels simultaneously as unexpected behavior can appear.

    [..]
      It is mandatory to use only ADV_LPBAM_GPIO_WritePin_SetFullQ() and/or ADV_LPBAM_GPIO_WritePinSequence_SetFullQ()
      for the GPIO pin(s) configured as output.

    [..]
      It is mandatory to use only ADV_LPBAM_GPIO_ReadPinSequence_SetFullQ() for the GPIO pin(s) configured as input.

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
  * @brief  Build the write pin full DMA linked-list queue according to configured parameters in
  *         LPBAM_GPIO_WritePinAdvConf_t.
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
  * @brief  Build the write pin sequence full DMA linked-list queue according to configured parameters in
  *         LPBAM_GPIO_PinSeqAdvConf_t.
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
  * @brief  Build the read pin sequence full DMA linked-list queue according to configured parameters in
  *         LPBAM_GPIO_PinSeqAdvConf_t.
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
