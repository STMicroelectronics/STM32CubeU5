/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    lpbam_incomp.h
  * @author  MCD Application Team
  * @brief   Header for LPBAM InComp application
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
#ifndef LPBAM_INCOMP_H
#define LPBAM_INCOMP_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "stm32_lpbam.h"

/* Exported functions ------------------------------------------------------------------------------------------------*/
/* InComp application initialization */
void MX_InComp_Init(void);

/* InComp application InSwitch scenario initialization */
void MX_InComp_InSwitch_Init(void);

/* InComp application InSwitch scenario de-initialization */
void MX_InComp_InSwitch_DeInit(void);

/* InComp application InSwitch scenario build */
void MX_InComp_InSwitch_Build(void);

/* InComp application InSwitch scenario link */
void MX_InComp_InSwitch_Link(DMA_HandleTypeDef *hdma);

/* InComp application InSwitch scenario unlink */
void MX_InComp_InSwitch_UnLink(DMA_HandleTypeDef *hdma);

/* InComp application InSwitch scenario start */
void MX_InComp_InSwitch_Start(DMA_HandleTypeDef *hdma);

/* InComp application InSwitch scenario stop */
void MX_InComp_InSwitch_Stop(DMA_HandleTypeDef *hdma);

#endif /* LPBAM_INCOMP_H */
