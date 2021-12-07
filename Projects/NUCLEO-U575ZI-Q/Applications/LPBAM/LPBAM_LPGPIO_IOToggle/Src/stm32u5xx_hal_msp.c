/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    LPBAM/LPBAM_LPGPIO_IOToggle/Src/stm32u5xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   This file provides code for the MSP Initialization
  *          and de-Initialization codes.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @brief  Initializes the MSP.
  * @retval None
  */
void HAL_MspInit(void)
{
  /* Enable LPGPIO1 autonomous mode clock */
  __HAL_RCC_LPGPIO1_CLKAM_ENABLE();
  __HAL_RCC_LPGPIO1_CLK_SLEEP_ENABLE();

  /* Enable LPDMA1 autonomous mode clock */
  __HAL_RCC_LPDMA1_CLK_SLEEP_ENABLE();
  __HAL_RCC_LPDMA1_CLKAM_ENABLE();

  /* Enable LPTIM1 autonomous mode clock */
  __HAL_RCC_LPTIM1_CLK_SLEEP_ENABLE();
  __HAL_RCC_LPTIM1_CLKAM_ENABLE();

  /* Enable SRAM4 autonomous mode clock */
  __HAL_RCC_SRAM4_CLK_SLEEP_ENABLE();
  __HAL_RCC_SRAM4_CLKAM_ENABLE();
}

/**
* @brief  LPTIM MSP Init
* @param  hlptim : LPTIM handle
* @retval None
*/
void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef *hlptim)
{
  /* ## - 1 - Enable LPTIM clock ############################################ */
  __HAL_RCC_LPTIM1_CLK_ENABLE();

  /* ## - 2 - Force & Release the LPTIM Periheral Clock Reset ############### */
  /* Force the LPTIM Periheral Clock Reset */
  __HAL_RCC_LPTIM1_FORCE_RESET();

  /* Release the LPTIM Periheral Clock Reset */
  __HAL_RCC_LPTIM1_RELEASE_RESET();

#if defined (DEBUG_CONFIGURATION)
  GPIO_InitTypeDef GPIO_InitStruct;

  /* ## - 3 - Enable & Configure LPTIM Ultra Low Power ################# */
  /* Configure PB.2 (LPTIM1_OUT) in alternate function (AF1), Medium speed
  push-pull mode and pull-up enabled.
  Note: In order to reduce power consumption: GPIO Speed is configured in
  MediumSpeed */
  /* Enable GPIO PORT */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /* Configure PB.2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.Alternate = GPIO_AF1_LPTIM1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif /* defined (DEBUG_CONFIGURATION) */
}
/**
  * @}
  */
