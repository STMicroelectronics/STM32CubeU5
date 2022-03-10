/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    lpbam_master.h
  * @author  MCD Application Team
  * @brief   Header for LPBAM Master application
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
#ifndef LPBAM_MASTER_H
#define LPBAM_MASTER_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "stm32_lpbam.h"

/* Exported functions ------------------------------------------------------------------------------------------------*/
/* Master application initialization */
void MX_Master_Init(void);

/* Master application ReloadSeq scenario initialization */
void MX_Master_ReloadSeq_Init(void);

/* Master application ReloadSeq scenario de-initialization */
void MX_Master_ReloadSeq_DeInit(void);

/* Master application ReloadSeq scenario build */
void MX_Master_ReloadSeq_Build(void);

/* Master application ReloadSeq scenario link */
void MX_Master_ReloadSeq_Link(DMA_HandleTypeDef *hdma);

/* Master application ReloadSeq scenario unlink */
void MX_Master_ReloadSeq_UnLink(DMA_HandleTypeDef *hdma);

/* Master application ReloadSeq scenario start */
void MX_Master_ReloadSeq_Start(DMA_HandleTypeDef *hdma);

/* Master application ReloadSeq scenario stop */
void MX_Master_ReloadSeq_Stop(DMA_HandleTypeDef *hdma);

#endif /* LPBAM_MASTER_H */
