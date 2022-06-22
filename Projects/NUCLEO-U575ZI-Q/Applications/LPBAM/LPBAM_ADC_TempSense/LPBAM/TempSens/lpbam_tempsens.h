/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    lpbam_tempsens.h
  * @author  MCD Application Team
  * @brief   Header for LPBAM TempSens application
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#ifndef LPBAM_TEMPSENS_H
#define LPBAM_TEMPSENS_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "stm32_lpbam.h"

/* Exported functions ------------------------------------------------------------------------------------------------*/
/* TempSens application initialization */
void MX_TempSens_Init(void);

/* TempSens application MultiThres scenario initialization */
void MX_TempSens_MultiThres_Init(void);

/* TempSens application MultiThres scenario de-initialization */
void MX_TempSens_MultiThres_DeInit(void);

/* TempSens application MultiThres scenario build */
void MX_TempSens_MultiThres_Build(void);

/* TempSens application MultiThres scenario link */
void MX_TempSens_MultiThres_Link(DMA_HandleTypeDef *hdma);

/* TempSens application MultiThres scenario unlink */
void MX_TempSens_MultiThres_UnLink(DMA_HandleTypeDef *hdma);

/* TempSens application MultiThres scenario start */
void MX_TempSens_MultiThres_Start(DMA_HandleTypeDef *hdma);

/* TempSens application MultiThres scenario stop */
void MX_TempSens_MultiThres_Stop(DMA_HandleTypeDef *hdma);

#endif /* LPBAM_TEMPSENS_H */
