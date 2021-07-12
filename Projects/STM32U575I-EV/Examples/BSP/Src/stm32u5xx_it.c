/**
  ******************************************************************************
  * @file    stm32u5xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"
#include "stm32u5xx_it.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern DMA_HandleTypeDef hDmaSaiTx, hDmaSaiRx;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M33 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32U5xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32u5xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles GPDMA_Channel10 interrupt request.
  * @param  None
  * @retval None
  */
void GPDMA1_Channel0_IRQHandler(void)
{
  BSP_SRAM_IRQHandler(0);
} 

/**
  * @brief  This function handles GPDMA_Channel6 interrupt request.
  * @param  None
  * @retval None
  */
void LPDMA1_Channel3_IRQHandler(void)
{
  BSP_AUDIO_IN_IRQHandler(1, 0);
} 

/**
  * @brief  This function handles GPDMA_Channel6 interrupt request.
  * @param  None
  * @retval None
  */
void GPDMA1_Channel6_IRQHandler(void)
{
  BSP_AUDIO_IN_IRQHandler(1, 0);
} 

/**
  * @brief  This function handles EXTI13 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI13_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&hpb_exti[0]);
}

/**
  * @brief  This function handles EXTI1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&hpb_exti[1]);
}

/**
  * @brief  This function handles EXTI15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&hts_exti[0]);
}

/**
  * @brief  This function handles DCMI_PSSI interrupt request.
  * @param  None
  * @retval None
  */
void DCMI_PSSI_IRQHandler(void)
{
  HAL_DCMI_IRQHandler(&hcamera_dcmi);
}

/**
  * @brief  This function handles GPDMA_Channel12 interrupt request.
  * @param  None
  * @retval None
  */
void GPDMA1_Channel12_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hcamera_dcmi.DMA_Handle);
}

/**
  * @brief  This function handles EXTI5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI5_IRQHandler(void)
{
  BSP_JOY_IRQHandler(JOY1, JOY_ALL);  
}

/**
  * @brief  This function handles SDMMC1 interrupt request.
  * @param  None
  * @retval None
  */
void SDMMC1_IRQHandler(void)
{
  HAL_SD_IRQHandler(&hsd_sdmmc[0]);
}

/**
  * @brief  This function handles GPDMA_Channel1 interrupt request.
  * @param  None
  * @retval None
  */
void GPDMA1_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hDmaSaiRx);
}

/**
  * @brief  This function handles GPDMA_Channel2 interrupt request.
  * @param  None
  * @retval None
  */
void GPDMA1_Channel2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hDmaSaiTx);
}
