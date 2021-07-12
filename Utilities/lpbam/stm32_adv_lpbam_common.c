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
      It is strongly recommended to read carefully the GettingStarted.html document before starting developing an LPBAM
      application.

    *** Driver description ***
    ==========================
    [..]
      This advanced LPBAM module counts 3 files :
          (+) stm32_adv_lpbam_common.c
              (++) This file provides the DMA advanced files body.
          (+) stm32_adv_lpbam_common.h
              (++) This file is the header file of stm32_adv_lpbam_common.c. It provides advanced used types.
          (+) stm32_platform_lpbam_common.h
              (++) This header file contains all defines to be used in applicative side.

    *** Driver features ***
    =======================
    [..]
      This driver provides the following list of features :
          (+) Set a data configuration transfer for advanced APIs that supports data transfers.
              Please check @ref LPBAM_Q_Data_Node_Selection to have the exhaustive list of APIs that supports this
              capability.
          (+) Set a hardware trigger signal for a built linked-list queue execution and data transfers.
              Please check @ref LPBAM_Q_Config_Node_Selection and @ref LPBAM_Q_Data_Node_Selection to have the
              exhaustive list of APIs that supports this capability.
          (+) Set a circular mode for a built linked-list queue.
              Please check @ref LPBAM_Q_Config_Node_Selection and @ref LPBAM_Q_Data_Node_Selection to have the
              exhaustive list of APIs that supports this capability.

    *** Functional description ***
    ==============================
    [..]
      Contrarily to all other advanced LPBAM modules, the common module output is not a DMA linked-list queue. This
      module provides a complementary APIs allowing users to customize their LPBAM scenarios according to application
      needs. These APIs must be called after using advanced APIs in order to customize them.

    [..]
      The common module allows to configure data node within an advanced API in order to customize default transfer
      configuration offered.
      The common module allows to set a hardware trigger within an advanced API in order to :
          (+) Condition the whole DMA linked-list execution. When the hardware trigger front (Rising or falling) is
              occurred, the whole DMA queue is executed (when there are no trigger condition set to another queue
              level).
          (+) Condition the DMA linked-list data transfer execution. According to configured parameter, the trigger
              condition can be set at each single data or the whole data to be transmitted.
      The common module allows to circularize the linked-list queue in order to perform a circular transfers (infinite
      loop). The first circular node (the first elementary item to be executed in an infinite loop) is customizable.

    *** Driver APIs description ***
    ===============================
    [..]
      Use ADV_LPBAM_Q_SetDataConfig() API to configure data transfer (when supported) to a built linked-list queue.
      Each data transfer parameter can be managed independently thanks to the following parameters :
          (+) UpdateSrcInc        : Specifies whether the data source increment to be updated or not.
          (+) UpdateDestInc       : Specifies whether the data destination increment to be updated or not.
          (+) UpdateSrcDataWidth  : Specifies whether the data source data width to be updated or not.
          (+) UpdateDestDataWidth : Specifies whether the destination data width to be updated or not.
          (+) UpdateSrcSecure     : Specifies whether the source security to be updated or not.
                                    This field is used for products that supports trust zone architecture.
          (+) UpdateDestSecure    : Specifies whether the destination security to be updated or not.
                                    This field is used for products that supports trust zone architecture.

      Data transfer parameters are :
          (+) SrcInc        : Specifies the source increment mode for the DMA channel.
          (+) DestInc       : Specifies the destination increment mode for the DMA channel.
          (+) SrcDataWidth  : Specifies the source data width for the DMA channel.
          (+) DestDataWidth : Specifies the destination data width for the DMA channel.
          (+) SrcSecure     : Specifies the source security attribute.
                              This field is used for products that supports trust zone architecture.
          (+) DestSecure    : Specifies the destination security attribute.
                              This field is used for products that supports trust zone architecture.

    [..]
      Use ADV_LPBAM_Q_SetTriggerConfig() API to configure a hardware trigger signal to a built linked-list queue.
      Trigger configuration parameters are :
          (+) TriggerMode      : Specifies the DMA channel trigger mode.
          (+) TriggerPolarity  : Specifies the DMA channel trigger event polarity.
          (+) TriggerSelection : Specifies the DMA channel trigger event selection.
      Please refer to stm32u5_platform_lpbam_common.h file for more information.

    [..]
      Use ADV_LPBAM_Q_SetCircularMode() API to circularize a built linked-list queue.
      Please refer to ADV_LPBAM_Q_SetCircularMode() API description for configurable parameters.

    *** Driver user sequence ***
    ============================
    [..]
      Call ADV_LPBAM_Q_SetDataConfig() after any call to advanced API to setup data transfer (when data transfer is
      supported).
      Call ADV_LPBAM_Q_SetTriggerConfig() after any call to advanced API to setup configuration or data transfer (when
      data transfer is supported).
      Call ADV_LPBAM_Q_SetCircularMode() after calling all API scenario to circularize repeated tasks.

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
  transfer_config.UpdateSrcSecure     = pDataConfig->UpdateSrcSecure;
  transfer_config.UpdateDestSecure    = pDataConfig->UpdateDestSecure;
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

  /* Update transfer data configuration */
  transfer_config.TransferConfig.Transfer.SrcInc            = pDataConfig->TransferConfig.Transfer.SrcInc;
  transfer_config.TransferConfig.Transfer.DestInc           = pDataConfig->TransferConfig.Transfer.DestInc;
  transfer_config.TransferConfig.Transfer.SrcDataWidth      = pDataConfig->TransferConfig.Transfer.SrcDataWidth;
  transfer_config.TransferConfig.Transfer.DestDataWidth     = pDataConfig->TransferConfig.Transfer.DestDataWidth;
  transfer_config.TransferConfig.Transfer.TransferEventMode = pDataConfig->TransferConfig.Transfer.TransferEventMode;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  transfer_config.TransferConfig.SrcSecure              = pDataConfig->TransferConfig.SrcSecure;
  transfer_config.TransferConfig.DestSecure             = pDataConfig->TransferConfig.DestSecure;
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

  /* Set data configuration */
  if (LPBAM_SetDMATransferConfig(&transfer_config, (DMA_NodeTypeDef *)node_addr) != LPBAM_OK)
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
  *                              data node of queue
  *                              built by advanced lpbam APIs.
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
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* LPBAM_COMMON_MODULE_ENABLED */
