/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_common.c
  * @author  MCD Application Team
  * @brief   This file provides the common LPBAM layer services.
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
  */

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_ll_lpbam.h"

#ifdef LPBAM_COMMON_MODULE_ENABLED

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/** @addtogroup COMMON_Basic
  * @{
  */

/** @defgroup LPBAM_COMMON_Basic_Exported_Functions LPBAM COMMON Basic Exported Functions
  * @{
  */

/**
  * @brief  Set DMA transfer configuration according to configured parameters in the LPBAM_COMMON_TransferConfig_t.
  * @param  pTransferConfig : [IN]  Pointer to a LPBAM_COMMON_TransferConfig_t structure that contains transfer node
  *                                 configuration.
  * @param  pDMANode        : [OUT] Pointer to a DMA_NodeTypeDef structure that contains linked-list node registers
  *                                 configurations.
  * @retval LPBAM Status    : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_SetDMATransferConfig(LPBAM_COMMON_TransferConfig_t const *const pTransferConfig,
                                          DMA_NodeTypeDef               *const pDMANode)
{
  DMA_NodeConfTypeDef dma_node_conf;

  /* Get node configuration */
  if (HAL_DMAEx_List_GetNodeConfig(&dma_node_conf, pDMANode) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Update source increment */
  if (pTransferConfig->UpdateSrcInc == ENABLE)
  {
    dma_node_conf.Init.SrcInc = pTransferConfig->TransferConfig.Transfer.SrcInc;
  }

  /* Update destination increment */
  if (pTransferConfig->UpdateDestInc  == ENABLE)
  {
    dma_node_conf.Init.DestInc = pTransferConfig->TransferConfig.Transfer.DestInc;
  }

  /* Update source data width */
  if (pTransferConfig->UpdateSrcDataWidth == ENABLE)
  {
    dma_node_conf.Init.SrcDataWidth = pTransferConfig->TransferConfig.Transfer.SrcDataWidth;
  }

  /* Update destination data width */
  if (pTransferConfig->UpdateDestDataWidth == ENABLE)
  {
    dma_node_conf.Init.DestDataWidth = pTransferConfig->TransferConfig.Transfer.DestDataWidth;
  }

  /* Update destination data width */
  if (pTransferConfig->UpdateTransferEventMode == ENABLE)
  {
    dma_node_conf.Init.TransferEventMode = pTransferConfig->TransferConfig.Transfer.TransferEventMode;
  }

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /* Update source security */
  if (pTransferConfig->UpdateSrcSecure == ENABLE)
  {
    dma_node_conf.SrcSecure = pTransferConfig->TransferConfig.SrcSecure;
  }

  /* Update destination security */
  if (pTransferConfig->UpdateDestSecure == ENABLE)
  {
    dma_node_conf.DestSecure = pTransferConfig->TransferConfig.DestSecure;
  }
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

  /* Rebuild node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, pDMANode) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  return LPBAM_OK;
}

/**
  * @brief  Set DMA trigger configuration according to configured parameters in the LPBAM_COMMON_TrigConfig_t.
  * @param  pTrigConfig  : [IN]  Pointer to a LPBAM_COMMON_TrigConfig_t structure that contains the trigger
  *                              configuration.
  * @param  pDMANode     : [OUT] Pointer to a DMA_NodeTypeDef structure that contains linked-list DMA node registers
  *                              configurations.
  * @retval LPBAM Status : [OUT] Value from LPBAM_Status_t enumeration.
  */
LPBAM_Status_t LPBAM_SetDMATriggerConfig(LPBAM_COMMON_TrigConfig_t const *const pTrigConfig,
                                         DMA_NodeTypeDef           *const pDMANode)
{
  DMA_NodeConfTypeDef dma_node_conf;

  /* Get DMA node configuration */
  if (HAL_DMAEx_List_GetNodeConfig(&dma_node_conf, pDMANode) != HAL_OK)
  {
    return LPBAM_ERROR;
  }

  /* Set trigger information to DMA node */
  dma_node_conf.TriggerConfig.TriggerMode      = pTrigConfig->TriggerConfig.TriggerMode;
  dma_node_conf.TriggerConfig.TriggerSelection = pTrigConfig->TriggerConfig.TriggerSelection;
  dma_node_conf.TriggerConfig.TriggerPolarity  = pTrigConfig->TriggerConfig.TriggerPolarity;

  /* Rebuild DMA node */
  if (HAL_DMAEx_List_BuildNode(&dma_node_conf, pDMANode) != HAL_OK)
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

#endif /* LPBAM_COMMON_MODULE_ENABLED */
