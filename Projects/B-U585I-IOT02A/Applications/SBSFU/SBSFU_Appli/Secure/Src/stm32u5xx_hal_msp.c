/**
  ******************************************************************************
  * @file    stm32u5xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   Secure HAL MSP module.
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

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
  /* Temporary workaround because of any GDB server illegal access at startup */
  __HAL_RCC_GTZC1_CLK_ENABLE();
  (void)HAL_GTZC_TZIC_ClearFlag(GTZC_PERIPH_ALL);
#endif /* defined(__GNUC__) && !defined(__ARMCC_VERSION)  */

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_GTZC1_CLK_ENABLE();

  /* System interrupt init*/

  /* Peripheral interrupt init */
  /* GTZC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(GTZC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(GTZC_IRQn);

  /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
  */
  HAL_PWREx_DisableUCPDDeadBattery();

}
