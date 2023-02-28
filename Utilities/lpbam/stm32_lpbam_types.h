/**
  **********************************************************************************************************************
  * @file    stm32_lpbam_types.h
  * @author  MCD Application Team
  * @brief   This file implements lpbam types.
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

/* Define to prevent recursive inclusion ---------------------------------------------------------------------------- */
#ifndef STM32_LPBAM_TYPES_H
#define STM32_LPBAM_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------------------------------- */
#include "stm32_lpbam_conf.h"

/** @addtogroup LPBAM_Utilities_Driver
  * @{
  */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/** @defgroup LPBAM_Exported_Types LPBAM Exported Types
  * @brief    LPBAM Exported Types
  * @{
  */

/**
  * @brief LPBAM DMA linked-list structure definition.
  */
typedef struct
{
  uint32_t QueueType;     /*!< Specifies the linked-list queue type.
                               This parameter can be a value of @ref LPBAM_Linked_List_Queue_Type */

  DMA_TypeDef *pInstance; /*!< Specifies the DMA instance.                                        */

} LPBAM_DMAListInfo_t;

/**
  * @brief LPBAM DMA linked-list structure definition.
  */
typedef struct
{
  uint32_t    NodeID;     /*!< Specifies the DMA node ID.
                               This parameter can be a value of LPBAM_XXX_ID_t enumeration       */

  uint32_t    NodeType;   /*!< Specifies the linked-list node type.
                               This parameter can be a value of @ref LPBAM_Linked_List_Node_Type */

  DMA_TypeDef *pInstance; /*!< Specifies the DMA instance.                                       */

} LPBAM_DMANodeInfo_t;

/**
  * @brief LPBAM descriptor items structure definition.
  */
typedef struct
{
  uint16_t RegOffset;   /*!< Specifies the peripheral register offset    */

  uint16_t RegDataSize; /*!< Specifies the peripheral register data size */

} LPBAM_ItemDesc_t;

/**
  * @brief LPBAM descriptor information structure definition.
  */
typedef struct
{
  uint32_t Request;   /*!< Specifies the DMA request             */

  uint32_t Direction; /*!< Specifies the DMA direction           */

  uint32_t SrcAddr;   /*!< Specifies the DMA source address      */

  uint32_t DstAddr;   /*!< Specifies the DMA destination address */

  uint32_t DataSize;  /*!< Specifies the DMA data size           */

} LPBAM_InfoDesc_t;

/**
  * @brief LPBAM generic descriptor structure definition.
  */
typedef struct
{
  LPBAM_DMANodeInfo_t NodeInfo;   /*!< Specifies the DMA linked-list node information                                */

  uint32_t           *pSrcVarReg; /*!< Pointer to source variable address that will be copied to peripheral register */

  uint32_t           *pBuff;      /*!< Pointer to data buffer                                                        */

} LPBAM_Desc_t;

/**
  * @brief LPBAM Status enumeration definition.
  */
typedef enum
{
  LPBAM_OK         = 0x00,
  LPBAM_ERROR      = 0x01,
  LPBAM_INVALID_ID = 0x02

} LPBAM_Status_t;

/**
  * @brief LPBAM Functional State enumeration definition.
  */
typedef FunctionalState LPBAM_FunctionalState;
/**
  * @}
  */

/* Exported constants ----------------------------------------------------------------------------------------------- */
/** @defgroup Exported_Constants Exported Constants
  * @{
  */

/** @defgroup LPBAM_Linked_List_Queue_Type LPBAM Linked List Queue Type
  * @{
  */
#define LPBAM_LINEAR_ADDRESSING_Q (0x11U)
/**
  * @}
  */

/** @defgroup LPBAM_Linked_List_Node_Type LPBAM Linked List Node Type
  * @{
  */
#define LPBAM_LINEAR_ADDRESSING_NODE (0x11U)
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32_LPBAM_TYPES_H */
