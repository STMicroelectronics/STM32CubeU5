/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    lpbam_slave.h
  * @author  MCD Application Team
  * @brief   Header for LPBAM Slave application
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
#ifndef LPBAM_SLAVE_H
#define LPBAM_SLAVE_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "stm32_lpbam.h"

/* Exported functions ------------------------------------------------------------------------------------------------*/
/* Slave application initialization */
void MX_Slave_Init(void);

/* Slave application ReloadSeq scenario initialization */
void MX_Slave_ReloadSeq_Init(void);

/* Slave application ReloadSeq scenario de-initialization */
void MX_Slave_ReloadSeq_DeInit(void);

/* Slave application ReloadSeq scenario build */
void MX_Slave_ReloadSeq_Build(void);

/* Slave application ReloadSeq scenario link */
void MX_Slave_ReloadSeq_Link(DMA_HandleTypeDef *hdma);

/* Slave application ReloadSeq scenario unlink */
void MX_Slave_ReloadSeq_UnLink(DMA_HandleTypeDef *hdma);

/* Slave application ReloadSeq scenario start */
void MX_Slave_ReloadSeq_Start(DMA_HandleTypeDef *hdma);

/* Slave application ReloadSeq scenario stop */
void MX_Slave_ReloadSeq_Stop(DMA_HandleTypeDef *hdma);

#endif /* LPBAM_SLAVE_H */
