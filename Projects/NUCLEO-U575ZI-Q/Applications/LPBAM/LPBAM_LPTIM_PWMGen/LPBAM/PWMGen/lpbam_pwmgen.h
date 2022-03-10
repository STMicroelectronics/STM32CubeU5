/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    lpbam_pwmgen.h
  * @author  MCD Application Team
  * @brief   Header for LPBAM PWMGen application
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
#ifndef LPBAM_PWMGEN_H
#define LPBAM_PWMGEN_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "stm32_lpbam.h"

/* Exported functions ------------------------------------------------------------------------------------------------*/
/* PWMGen application initialization */
void MX_PWMGen_Init(void);

/* PWMGen application MultiValue scenario initialization */
void MX_PWMGen_MultiValue_Init(void);

/* PWMGen application MultiValue scenario de-initialization */
void MX_PWMGen_MultiValue_DeInit(void);

/* PWMGen application MultiValue scenario build */
void MX_PWMGen_MultiValue_Build(void);

/* PWMGen application MultiValue scenario link */
void MX_PWMGen_MultiValue_Link(DMA_HandleTypeDef *hdma);

/* PWMGen application MultiValue scenario unlink */
void MX_PWMGen_MultiValue_UnLink(DMA_HandleTypeDef *hdma);

/* PWMGen application MultiValue scenario start */
void MX_PWMGen_MultiValue_Start(DMA_HandleTypeDef *hdma);

/* PWMGen application MultiValue scenario stop */
void MX_PWMGen_MultiValue_Stop(DMA_HandleTypeDef *hdma);

#endif /* LPBAM_PWMGEN_H */
