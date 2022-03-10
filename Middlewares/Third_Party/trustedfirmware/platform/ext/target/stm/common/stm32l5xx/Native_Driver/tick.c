/**
  ******************************************************************************
  * @file    tick.c
  * @author  MCD Application Team
  * @brief   Tick implementation without tick interrupt
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "stm32l5xx_hal.h"
/**
  * @brief This function configures the source of the time base:
  *        The time source is configured to have 1ms time base with a dedicated
  *        Tick interrupt priority.
  * @note This function overwrites the one declared as __weak in HAL.
  *       In this implementation, nothing is done.
  * @retval HAL status
  */

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  /* load 1Hz for timeout 1 second */
  uint32_t ticks = SystemCoreClock ;
  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_ENABLE_Msk;
  return HAL_OK;
}

/**
  * @brief Provide a tick value in millisecond.
  * @note This function overwrites the one declared as __weak in HAL.
  *       In this implementation, time is counted without using SysTick timer interrupts.
  * @retval tick value
  */
uint32_t HAL_GetTick(void)
{
  static uint32_t m_uTick = 0U;
  static uint32_t t1 = 0U, tdelta = 0U;
  uint32_t t2;

  t2 =  SysTick->VAL;

  if (t2 <= t1)
  {
    tdelta += t1 - t2;
  }
  else
  {
    tdelta += t1 + SysTick->LOAD - t2;
  }

  if (tdelta > (SystemCoreClock / (1000U)))
  {
    tdelta = 0U;
    m_uTick ++;
  }

  t1 = t2;
  return m_uTick;
}