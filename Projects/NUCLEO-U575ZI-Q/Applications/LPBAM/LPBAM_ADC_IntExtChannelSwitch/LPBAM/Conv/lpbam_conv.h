/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    lpbam_conv.h
  * @author  MCD Application Team
  * @brief   Header for LPBAM Conv application
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
#ifndef LPBAM_CONV_H
#define LPBAM_CONV_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "stm32_lpbam.h"

/* Exported functions ------------------------------------------------------------------------------------------------*/
/* Conv application initialization */
void MX_Conv_Init(void);

/* Conv application AnalogWDG scenario initialization */
void MX_Conv_AnalogWDG_Init(void);

/* Conv application AnalogWDG scenario de-initialization */
void MX_Conv_AnalogWDG_DeInit(void);

/* Conv application AnalogWDG scenario build */
void MX_Conv_AnalogWDG_Build(void);

/* Conv application AnalogWDG scenario link */
void MX_Conv_AnalogWDG_Link(DMA_HandleTypeDef *hdma);

/* Conv application AnalogWDG scenario unlink */
void MX_Conv_AnalogWDG_UnLink(DMA_HandleTypeDef *hdma);

/* Conv application AnalogWDG scenario start */
void MX_Conv_AnalogWDG_Start(DMA_HandleTypeDef *hdma);

/* Conv application AnalogWDG scenario stop */
void MX_Conv_AnalogWDG_Stop(DMA_HandleTypeDef *hdma);

#endif /* LPBAM_CONV_H */
