/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file   lpbam_sigampli.h
  * @author MCD Application Team
  * @brief  Header for LPBAM SigAmpli application
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
#ifndef LPBAM_SIGAMPLI_H
#define LPBAM_SIGAMPLI_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "stm32_lpbam.h"

/* Exported functions ------------------------------------------------------------------------------------------------*/
/* SigAmpli application initialization */
void MX_SigAmpli_Init(void);

/* SigAmpli application MultiGain scenario initialization */
void MX_SigAmpli_MultiGain_Init(void);

/* SigAmpli application MultiGain scenario de-initialization */
void MX_SigAmpli_MultiGain_DeInit(void);

/* SigAmpli application MultiGain scenario build */
void MX_SigAmpli_MultiGain_Build(void);

/* SigAmpli application MultiGain scenario link */
void MX_SigAmpli_MultiGain_Link(DMA_HandleTypeDef *hdma);

/* SigAmpli application MultiGain scenario unlink */
void MX_SigAmpli_MultiGain_UnLink(DMA_HandleTypeDef *hdma);

/* SigAmpli application MultiGain scenario start */
void MX_SigAmpli_MultiGain_Start(DMA_HandleTypeDef *hdma);

/* SigAmpli application MultiGain scenario stop */
void MX_SigAmpli_MultiGain_Stop(DMA_HandleTypeDef *hdma);

#endif /* LPBAM_SIGAMPLI_H */
