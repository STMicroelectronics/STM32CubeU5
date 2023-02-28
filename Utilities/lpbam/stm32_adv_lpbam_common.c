/**
  **********************************************************************************************************************
  * @file    stm32_adv_lpbam_common.c
  * @author  MCD Application Team
  * @brief   This file provides the advanced LPBAM layer for COMMON peripheral.
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
          (+) stm32_adv_lpbam_common.c file
              (++) This file provides the implementation of the advanced LPBAM COMMON functions.
          (+) stm32_adv_lpbam_common.h file
              (++) This file is the header file of stm32_adv_lpbam_common.c. It provides advanced LPBAM COMMON functions
                   prototypes and the declaration of their needed exported types and structures.
          (+) STM32xx/stm32_platform_lpbam_common.h file
              (++) This header file contains all defines to be used on applicative side.
                   (+++) STM32xx stands for the device supporting LPBAM sub-system.

    *** Driver functions model ***
    ==============================
    [..]
      This section precises this module supported advanced functions model (refer to LPBAM_Utility_GettingStarted.html
      document for function model definition).

    [..]
      This driver provides 3 model of APIs :
          (+) ADV_LPBAM_Q_Set{Item}Config() : apply configuration to input queue.
          (+) ADV_LPBAM_Q_Set{Item}Mode()   : apply mode to input queue.
          (+) ADV_LPBAM_Q_ConvertQ()        : apply format to input queue.

    *** Driver features ***
    =======================
    [..]
      This section describes this LPBAM module supported features.

    [..]
      This driver provides services covering the LPBAM management of the following COMMON features :
          (+) Set a data configuration transfer to advanced APIs that supports data transfers.
              Check LPBAM_Q_Data_Node_Selection to have the exhaustive list of APIs that supports this capability.
          (+) Set a transfer event generation to advanced APIs that doesn't supports data transfers.
              Check LPBAM_Q_LastNode_Selection to have the exhaustive list of APIs that supports this capability.
          (+) Set a hardware trigger signal to a built linked-list queue execution and data transfers.
              Check LPBAM_Q_Config_Node_Selection and LPBAM_Q_Data_Node_Selection to have the exhaustive list of APIs
              that supports this capability.
          (+) Set a circular mode to a built linked-list queue.
              Check LPBAM_Q_Config_Node_Selection and LPBAM_Q_Data_Node_Selection to have the exhaustive list of APIs
              that supports this capability.
          (+) Convert a built linked-list queue to static or dynamic format.

    *** Functional description ***
    ==============================
    [..]
      This section describes the peripheral features covered by this LPBAM module.

    [..]
      Contrarily to all other advanced LPBAM modules, the common module output is not a DMA linked-list queue. This
      module provides a complementary APIs allowing users to customize their LPBAM scenarios according to application
      needs. These APIs must be called after using advanced APIs in order to customize them.

    [..]
      The common module allows to configure data transfer within an advanced API in order to customize default
      functional transfer configuration.

    [..]
      The common module allows to configure transfer event generation within an advanced API in order to customize
      default transfer event generation configuration.

    [..]
      The common module allows to set a hardware trigger within an advanced API in order to :
          (+) Condition the whole linked-list queue execution with a hardware trigger signal.
          (+) Condition the data transfer queue execution with a hardware trigger signal.
              (++) Data transfer trigger granularity can be at single data.
              (++) Data transfer trigger granularity can be at whole data.

    [..]
      The common module allows to circularize a linked-list queue in order to perform transfers in an infinite loop.
          (+) First circular item in the input queue can be one of parameters in LPBAM_Q_Config_Node_Selection and
              LPBAM_Q_Data_Node_Selection sections.

    [..]
      The common module allows to convert a linked-list queue in static or dynamic format.
          (+) When converted to static format, all queue operation are allowed. Executing static queues is not optimal
              in term of execution time and consumption point of view.
          (+) When converted to dynamic format, all queue operation are forbidden. Executing dynamic queues is optimal
              in term of execution time and consumption point of view.

    *** Driver APIs description ***
    ===============================
    [..]
      This section provides LPBAM module exhaustive APIs description without considering application user call sequence.
      For user call sequence information, please refer to 'Driver user sequence' section below.

    [..]
      Use ADV_LPBAM_Q_SetDataConfig() API to configure data transfer to an advanced API linked-list queue that contains
      data transfer. Advanced API data node constant is defined in LPBAM_Q_Data_Node_Selection section.
      The data transfer settings parameters can be updated separately thanks to the following fields in
      LPBAM_COMMON_DataAdvConf_t structure :
          (+) UpdateSrcInc        : Specifies whether the data source increment to be updated or not.
          (+) UpdateDestInc       : Specifies whether the data destination increment to be updated or not.
          (+) UpdateSrcDataWidth  : Specifies whether the data source data width to be updated or not.
          (+) UpdateDestDataWidth : Specifies whether the destination data width to be updated or not.
          (+) UpdateSrcSecure     : Specifies whether the source security to be updated or not.
                                    This field is used for products that supports TrustZone architecture.
          (+) UpdateDestSecure    : Specifies whether the destination security to be updated or not.
                                    This field is used for products that supports TrustZone architecture.
      The data transfer parameters are :
          (+) SrcInc        : Specifies the source increment mode for the DMA channel.
          (+) DestInc       : Specifies the destination increment mode for the DMA channel.
          (+) SrcDataWidth  : Specifies the source data width for the DMA channel.
          (+) DestDataWidth : Specifies the destination data width for the DMA channel.
          (+) SrcSecure     : Specifies the source security attribute.
                              This field is used for products that supports trust zone architecture.
          (+) DestSecure    : Specifies the destination security attribute.
                              This field is used for products that supports trust zone architecture.
      Default data transfer settings for any advanced API is described under 'Driver APIs description' section by each
      module.

    [..]
      Use ADV_LPBAM_Q_SetTransferEventGeneration() API to configure transfer event generation to an advanced API
      linked-list queue. Advanced API transfer event generation node constant is defined in LPBAM_Q_LastNode_Selection
      section.

    [..]
      Use ADV_LPBAM_Q_SetTriggerConfig() API to configure a hardware trigger signal to an advanced API linked-list
      queue. The exhaustive list of advanced APIs that supports triggers are listed on LPBAM_Q_Config_Node_Selection
      and LPBAM_Q_Data_Node_Selection sections.
      The trigger configuration parameters are :
          (+) TriggerMode      : Specifies the DMA channel trigger mode.
          (+) TriggerPolarity  : Specifies the DMA channel trigger event polarity.
          (+) TriggerSelection : Specifies the DMA channel trigger event selection.
      Please refer to stm32u5_platform_lpbam_common.h file for more information.

    [..]
      Use ADV_LPBAM_Q_SetCircularMode() API to circularize a built linked-list queue. The exhaustive list of advanced
      API items that can be the first circular item are listed on LPBAM_Q_Config_Node_Selection and
      LPBAM_Q_Data_Node_Selection sections.

    [..]
      Use ADV_LPBAM_Q_ConvertQ() API to convert a built linked-list queue to dynamic format when the whole queue is
      built in order to reduce time execution and consuption.
      When the linked-list queue is dynamic, use ADV_LPBAM_Q_ConvertQ() API to convert a built linked-list queue to
      static format in order to modify it (Add new LPBAM function / Set/Remove circular mode).

    *** Driver user sequence ***
    ============================
    [..]
      This section provides the steps to follow to build an LPBAM application based on HAL/LL and LPBAM drivers. (refer
      to LPBAM_Utility_GettingStarted.html for linked-list feature description).

    [..]
      Call ADV_LPBAM_Q_SetDataConfig() after any call to advanced API to setup data transfer (when data transfer is
      supported).

    [..]
      Call ADV_LPBAM_Q_SetTransferEventGeneration() after any call to advanced API to setup transfer event generation
      (when transfer event generation is supported).

    [..]
      Call ADV_LPBAM_Q_SetTriggerConfig() after any call to advanced API to setup configuration or data transfer (when
      data transfer is supported).

    [..]
      Call ADV_LPBAM_Q_SetCircularMode() after calling all scenario APIs to circularize the input linked-list queue.

    [..]
      Call ADV_LPBAM_Q_ConvertQ() after calling all scenario APIs to convert the input linked-list queue to static or
      dynamic format.

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

#ifdef LPBAM_COMMON_MODULE_ENABLED

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup COMMON_Advanced COMMON_Advanced
  * @{
  */

/** @defgroup LPBAM_COMMON_Advanced_Exported_Functions LPBAM COMMON Advanced Exported Functions
  * @{
  */

/**
  * @brief  Set LPBAM transfer data node configuration according to configured parameters in the
  *         LPBAM_COMMON_DataAdvConf_t and according to node level.
  * @param  pDataConfig  : [IN]  Pointer to a LPBAM_COMMON_TrigAdvConf_t structure that contains the trigger
  *                              configuration.
  * @param  DataNode     : [IN]  Specifies the data node to be updated.
  *                              This parameter can be a value of @ref LPBAM_Q_Data_Node_Selection.
  * @param  pDescriptor  : [OUT] Pointer to all available LPBAM descriptors used in advanced layer.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_Q_SetDataConfig(LPBAM_COMMON_DataAdvConf_t const *const pDataConfig,
                                         uint32_t                   DataNode,
                                         void                       const *const pDescriptor)
{
  LPBAM_COMMON_TransferConfig_t transfer_config;
  uint32_t node_addr = ((uint32_t)pDescriptor);

  /* Calculate data node address */
  node_addr += (DataNode * sizeof(DMA_NodeTypeDef));

  /* Set transfer fields to be updated */
  transfer_config.UpdateSrcInc            = pDataConfig->UpdateSrcInc;
  transfer_config.UpdateDestInc           = pDataConfig->UpdateDestInc;
  transfer_config.UpdateSrcDataWidth      = pDataConfig->UpdateSrcDataWidth;
  transfer_config.UpdateDestDataWidth     = pDataConfig->UpdateDestDataWidth;
  transfer_config.UpdateTransferEventMode = pDataConfig->UpdateTransferEventMode;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  transfer_config.UpdateSrcSecure         = pDataConfig->UpdateSrcSecure;
  transfer_config.UpdateDestSecure        = pDataConfig->UpdateDestSecure;
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

  /* Update transfer data configuration */
  transfer_config.TransferConfig.Transfer.SrcInc            = pDataConfig->TransferConfig.Transfer.SrcInc;
  transfer_config.TransferConfig.Transfer.DestInc           = pDataConfig->TransferConfig.Transfer.DestInc;
  transfer_config.TransferConfig.Transfer.SrcDataWidth      = pDataConfig->TransferConfig.Transfer.SrcDataWidth;
  transfer_config.TransferConfig.Transfer.DestDataWidth     = pDataConfig->TransferConfig.Transfer.DestDataWidth;
  transfer_config.TransferConfig.Transfer.TransferEventMode = pDataConfig->TransferConfig.Transfer.TransferEventMode;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  transfer_config.TransferConfig.SrcSecure                  = pDataConfig->TransferConfig.SrcSecure;
  transfer_config.TransferConfig.DestSecure                 = pDataConfig->TransferConfig.DestSecure;
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

  /* Set DMA transfer configuration */
  if (LPBAM_SetDMATransferConfig(&transfer_config, (DMA_NodeTypeDef *)node_addr) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Set LPBAM transfer event generation according to TransferEvent and TransferEventNode parameters.
  * @param  TransferEvent     : [IN]  Specifies the transfer event to be generated.
  *                                   This parameter can be a value of @ref LPBAM_DMA_Transfer_Event_Mode.
  * @param  TransferEventNode : [IN]  Specifies the transfer event mode node to be updated.
  *                                   This parameter can be a value of @ref LPBAM_Q_LastNode_Selection.
  * @param  pDescriptor       : [OUT] Pointer to all available LPBAM descriptors used in advanced layer.
  * @retval LPBAM Status      : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_Q_SetTransferEventGeneration(uint32_t TransferEvent,
                                                      uint32_t TransferEventNode,
                                                      void     const *const pDescriptor)
{
  LPBAM_COMMON_TransferConfig_t transfer_event_config;
  uint32_t node_addr = ((uint32_t)pDescriptor);

  /* Calculate data node address */
  node_addr += (TransferEventNode * sizeof(DMA_NodeTypeDef));

  /* Set transfer fields to be updated */
  transfer_event_config.UpdateSrcInc            = DISABLE;
  transfer_event_config.UpdateDestInc           = DISABLE;
  transfer_event_config.UpdateSrcDataWidth      = DISABLE;
  transfer_event_config.UpdateDestDataWidth     = DISABLE;
  transfer_event_config.UpdateTransferEventMode = ENABLE;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  transfer_event_config.UpdateSrcSecure         = DISABLE;
  transfer_event_config.UpdateDestSecure        = DISABLE;
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

  /* Update transfer data configuration */
  transfer_event_config.TransferConfig.Transfer.TransferEventMode = TransferEvent;

  /* Set DMA transfer configuration */
  if (LPBAM_SetDMATransferConfig(&transfer_event_config, (DMA_NodeTypeDef *)node_addr) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Set LPBAM queue trigger according to configured parameters in the LPBAM_COMMON_TrigAdvConf_t and according
  *         to node level.
  * @param  pTrigConfig  : [IN]  Pointer to a LPBAM_COMMON_TrigAdvConf_t structure that contains the trigger
  *                              configuration.
  * @param  NodeLevel    : [IN]  Specifies whether the trigger shall be applied on the configuration or the data node
  *                              of queue built by advanced lpbam APIs.
  *                              This parameter can be a value of @ref LPBAM_Q_Config_Node_Selection or
  *                              @ref LPBAM_Q_Data_Node_Selection.
  * @param  pDescriptor  : [OUT] Pointer to all available LPBAM descriptors used in advanced layer.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_Q_SetTriggerConfig(LPBAM_COMMON_TrigAdvConf_t const *const pTrigConfig,
                                            uint32_t                   NodeLevel,
                                            void                       const *const pDescriptor)
{
  LPBAM_COMMON_TrigConfig_t trig_conf;
  uint32_t node_addr = ((uint32_t)pDescriptor);

  /* Calculate data node address */
  node_addr += (NodeLevel * sizeof(DMA_NodeTypeDef));

  /* Update trigger configuration */
  trig_conf.TriggerConfig.TriggerMode      = pTrigConfig->TriggerConfig.TriggerMode;
  trig_conf.TriggerConfig.TriggerPolarity  = pTrigConfig->TriggerConfig.TriggerPolarity;
  trig_conf.TriggerConfig.TriggerSelection = pTrigConfig->TriggerConfig.TriggerSelection;

  /* Set DMA trigger configuration */
  if (LPBAM_SetDMATriggerConfig(&trig_conf, (DMA_NodeTypeDef *)node_addr) != LPBAM_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Set LPBAM circular mode to linked-list queue according to selected descriptor and node level.
  * @param  pDescriptor  : [IN]  Pointer to all available LPBAM descriptors used in advanced layer.
  * @param  NodeLevel    : [IN]  Specifies whether the first circular node shall be applied on the configuration or the
  *                              data node of queue built by advanced lpbam APIs.
  *                              This parameter can be a value of @ref LPBAM_Q_Config_Node_Selection or
  *                              @ref LPBAM_Q_Data_Node_Selection.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_Q_SetCircularMode(void             const *const pDescriptor,
                                           uint32_t         NodeLevel,
                                           DMA_QListTypeDef *const pQueue)
{
  uint32_t node_addr = ((uint32_t)pDescriptor);

  /* Check pDescriptor parameter */
  if (pDescriptor == NULL)
  {
    /* Set circular mode for the whole queue */
    if (HAL_DMAEx_List_SetCircularMode(pQueue) != HAL_OK)
    {
      return LPBAM_ERROR;
    }
  }
  else
  {
    /* Set circular mode with selecting the first circular node from the descriptor */
    node_addr += (NodeLevel * sizeof(DMA_NodeTypeDef));

    /* Set circular mode according to first circular node selection */
    if (HAL_DMAEx_List_SetCircularModeConfig(pQueue, (DMA_NodeTypeDef *)node_addr) != HAL_OK)
    {
      return LPBAM_ERROR;
    }
  }

  return LPBAM_OK;
}

/**
  * @brief  Convert linked-list queue according to selected type.
  * @param  Type         : [IN]  Specifies whether the queue type is static or dynamic.
  *                              This parameter can be a value of @ref LPBAM_DMA_Queue_Type.
  * @param  pQueue       : [OUT] Pointer to a DMA_QListTypeDef structure that contains DMA linked-list queue
  *                              information.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t ADV_LPBAM_Q_ConvertQ(uint32_t         Type,
                                    DMA_QListTypeDef *const pQueue)
{
  /* Check queue type */
  if (Type == LPBAM_DMA_STATIC_Q)
  {
    /* Convert the DMA queue to static */
    if (HAL_DMAEx_List_ConvertQToStatic(pQueue) != HAL_OK)
    {
      return LPBAM_ERROR;
    }
  }
  else
  {
    /* Convert the DMA queue to dynamic */
    if (HAL_DMAEx_List_ConvertQToDynamic(pQueue) != HAL_OK)
    {
      return LPBAM_ERROR;
    }
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
#endif /* LPBAM_COMMON_MODULE_ENABLED */
