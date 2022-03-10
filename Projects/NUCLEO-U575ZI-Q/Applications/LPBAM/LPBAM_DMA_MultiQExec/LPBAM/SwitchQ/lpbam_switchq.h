/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file    lpbam_switchq.h
  * @author  MCD Application Team
  * @brief   Header for LPBAM SwitchQ application
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
#ifndef LPBAM_SWITCHQ_H
#define LPBAM_SWITCHQ_H

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "stm32_lpbam.h"

/* Exported functions ------------------------------------------------------------------------------------------------*/
/* SwitchQ application initialization */
void MX_SwitchQ_Init(void);

/* SwitchQ application TwoQExec scenario initialization */
void MX_SwitchQ_TwoQExec_Init(void);

/* SwitchQ application TwoQExec scenario de-initialization */
void MX_SwitchQ_TwoQExec_DeInit(void);

/* SwitchQ application TwoQExec scenario build */
void MX_SwitchQ_TwoQExec_Build(void);

/* SwitchQ application TwoQExec scenario link */
void MX_SwitchQ_TwoQExec_Link(DMA_HandleTypeDef *hdma);

/* SwitchQ application TwoQExec scenario unlink */
void MX_SwitchQ_TwoQExec_UnLink(DMA_HandleTypeDef *hdma);

/* SwitchQ application TwoQExec scenario start */
void MX_SwitchQ_TwoQExec_Start(DMA_HandleTypeDef *hdma);

/* SwitchQ application TwoQExec scenario stop */
void MX_SwitchQ_TwoQExec_Stop(DMA_HandleTypeDef *hdma);

#endif /* LPBAM_SWITCHQ_H */
