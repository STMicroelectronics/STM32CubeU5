/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    lpbam_txrx.h
  * @author  MCD Application Team
  * @brief   Header for LPBAM TxRx application
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
/* USER CODE END Header */
/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef LPBAM_TXRX_H
#define LPBAM_TXRX_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "stm32_lpbam.h"

/* Exported functions ------------------------------------------------------------------------------------------------*/
/* TxRx application initialization */
void MX_TxRx_Init(void);

/* TxRx application SendByPack scenario initialization */
void MX_TxRx_SendByPack_Init(void);

/* TxRx application SendByPack scenario de-initialization */
void MX_TxRx_SendByPack_DeInit(void);

/* TxRx application SendByPack scenario build */
void MX_TxRx_SendByPack_Build(void);

/* TxRx application SendByPack scenario link */
void MX_TxRx_SendByPack_Link(DMA_HandleTypeDef *hdma);

/* TxRx application SendByPack scenario unlink */
void MX_TxRx_SendByPack_UnLink(DMA_HandleTypeDef *hdma);

/* TxRx application SendByPack scenario start */
void MX_TxRx_SendByPack_Start(DMA_HandleTypeDef *hdma);

/* TxRx application SendByPack scenario stop */
void MX_TxRx_SendByPack_Stop(DMA_HandleTypeDef *hdma);

#endif /* LPBAM_TXRX_H */
