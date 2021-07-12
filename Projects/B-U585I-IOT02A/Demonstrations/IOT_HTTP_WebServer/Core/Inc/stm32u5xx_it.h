/**
  ******************************************************************************
  * @file    stm32u5xx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
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

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef STM32U5xx_IT_H
#define STM32U5xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Private includes --------------------------------------------------------------------------------------------------*/
/* Exported types ----------------------------------------------------------------------------------------------------*/
/* Exported constants ------------------------------------------------------------------------------------------------*/
/* Exported macro ----------------------------------------------------------------------------------------------------*/
/* Exported functions prototypes -------------------------------------------------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SecureFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void LPTIM1_IRQHandler(void);
void SPI2_IRQHandler(void);
void EXTI14_IRQHandler(void);
void EXTI15_IRQHandler(void);

void GPDMA1_Channel4_IRQHandler(void);
void GPDMA1_Channel5_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* STM32U5xx_IT_H */
