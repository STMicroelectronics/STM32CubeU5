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
#include "main.h"
#include "stm32u5xx_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* DMA flags declaration */
extern __IO uint32_t TransferCompleteDetected, TransferErrorDetected;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M33 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
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
}

/******************************************************************************/
/*                 STM32U5xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32U5xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles DMA channel interrupt request.
  * @param  None
  * @retval None
  */
void DMA_IRQHandler(void)
{
  /* Check if User Setting Error flag is active */
  if (LL_DMA_IsActiveFlag_USE(GPDMA1, LL_DMA_CHANNEL_13) != 0U)
  {
    /* Clear User Setting Error flag */
    LL_DMA_ClearFlag_USE(GPDMA1, LL_DMA_CHANNEL_13);
    /* Increment error user flag */
    TransferErrorDetected++;
  }

  /* Check if Update Link Error flag is active */
  if (LL_DMA_IsActiveFlag_ULE(GPDMA1, LL_DMA_CHANNEL_13) != 0U)
  {
    /* Clear Update Link Error flag */
    LL_DMA_ClearFlag_ULE(GPDMA1, LL_DMA_CHANNEL_13);
    /* Increment error user flag */
    TransferErrorDetected++;
  }

  /* Check if Data Transfer Error flag is active */
  if (LL_DMA_IsActiveFlag_DTE(GPDMA1, LL_DMA_CHANNEL_13) != 0U)
  {
    /* Clear Data Transfer Error flag */
    LL_DMA_ClearFlag_DTE(GPDMA1, LL_DMA_CHANNEL_13);
    /* Increment error user flag */
    TransferErrorDetected++;
  }

  /* Check if Transfer Complete flag is active */
  if (LL_DMA_IsActiveFlag_TC(GPDMA1, LL_DMA_CHANNEL_13) != 0U)
  {
    /* Clear Transfer Complete flag */
    LL_DMA_ClearFlag_TC(GPDMA1, LL_DMA_CHANNEL_13);
    /* Increment success user flag */
    TransferCompleteDetected++;
  }
}

