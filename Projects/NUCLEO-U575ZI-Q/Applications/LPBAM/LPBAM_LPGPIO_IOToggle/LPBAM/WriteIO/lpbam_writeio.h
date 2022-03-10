/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    lpbam_writeio.h
  * @author  MCD Application Team
  * @brief   Header for LPBAM WriteIO application
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
#ifndef LPBAM_WRITEIO_H
#define LPBAM_WRITEIO_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "stm32_lpbam.h"

/* Exported functions ------------------------------------------------------------------------------------------------*/
/* WriteIO application initialization */
void MX_WriteIO_Init(void);

/* WriteIO application WriteSeq scenario initialization */
void MX_WriteIO_WriteSeq_Init(void);

/* WriteIO application WriteSeq scenario de-initialization */
void MX_WriteIO_WriteSeq_DeInit(void);

/* WriteIO application WriteSeq scenario build */
void MX_WriteIO_WriteSeq_Build(void);

/* WriteIO application WriteSeq scenario link */
void MX_WriteIO_WriteSeq_Link(DMA_HandleTypeDef *hdma);

/* WriteIO application WriteSeq scenario unlink */
void MX_WriteIO_WriteSeq_UnLink(DMA_HandleTypeDef *hdma);

/* WriteIO application WriteSeq scenario start */
void MX_WriteIO_WriteSeq_Start(DMA_HandleTypeDef *hdma);

/* WriteIO application WriteSeq scenario stop */
void MX_WriteIO_WriteSeq_Stop(DMA_HandleTypeDef *hdma);

#endif /* LPBAM_WRITEIO_H */
