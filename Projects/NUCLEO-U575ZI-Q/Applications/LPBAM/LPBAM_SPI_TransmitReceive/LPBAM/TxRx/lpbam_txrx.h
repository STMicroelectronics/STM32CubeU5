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

/* TxRx application MultiConf scenario initialization */
void MX_TxRx_MultiConf_Init(void);

/* TxRx application MultiConf scenario de-initialization */
void MX_TxRx_MultiConf_DeInit(void);

/* TxRx application MultiConf scenario build */
void MX_TxRx_MultiConf_Build(void);

/* TxRx application MultiConf scenario link */
void MX_TxRx_MultiConf_Link(DMA_HandleTypeDef *hdma);

/* TxRx application MultiConf scenario unlink */
void MX_TxRx_MultiConf_UnLink(DMA_HandleTypeDef *hdma);

/* TxRx application MultiConf scenario start */
void MX_TxRx_MultiConf_Start(DMA_HandleTypeDef *hdma);

/* TxRx application MultiConf scenario stop */
void MX_TxRx_MultiConf_Stop(DMA_HandleTypeDef *hdma);

#endif /* LPBAM_TXRX_H */
