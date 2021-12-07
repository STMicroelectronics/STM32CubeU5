/**
  ******************************************************************************
  * @file    stm32u5xx_it.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32U5xx_IT_H
#define STM32U5xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx.h"
#include "stm32u575i_eval.h"
#include "stm32u575i_eval_audio.h"
#include "stm32u575i_eval_camera.h"
#include "stm32u575i_eval_sd.h"
#include "stm32u575i_eval_ts.h"
#include "stm32u575i_eval_sram.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void EXTI1_IRQHandler(void);
void EXTI5_IRQHandler(void);
void EXTI13_IRQHandler(void);
void EXTI15_IRQHandler(void);
void GPDMA1_Channel0_IRQHandler(void);
void GPDMA1_Channel1_IRQHandler(void);
void GPDMA1_Channel2_IRQHandler(void);
void GPDMA1_Channel6_IRQHandler(void);
void GPDMA1_Channel12_IRQHandler(void);
void LPDMA1_Channel0_IRQHandler(void);
void LPDMA1_Channel1_IRQHandler(void);
void SDMMC1_IRQHandler(void);
void DCMI_PSSI_IRQHandler(void);
void ADF1_IRQHandler(void);
void MDF1_FLT3_IRQHandler(void);
void MDF1_FLT4_IRQHandler(void);
void PWR_S3WU_IRQHandler(void);
void RTC_IRQHandler(void);
#ifdef __cplusplus
}
#endif

#endif /* STM32U5xx_IT_H */