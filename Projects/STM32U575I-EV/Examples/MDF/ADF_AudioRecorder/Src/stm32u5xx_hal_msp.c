/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32u5xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief MDF MSP Initialization
* This function configures the hardware resources used in this example
* @param hmdf: MDF handle pointer
* @retval None
*/
void HAL_MDF_MspInit(MDF_HandleTypeDef* hmdf)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(IS_ADF_INSTANCE(hmdf->Instance))
  {
  /* USER CODE BEGIN ADF1_MspInit 0 */

  /* USER CODE END ADF1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADF1;
    PeriphClkInit.Adf1ClockSelection = RCC_ADF1CLKSOURCE_MSIK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_ADF1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADF1 GPIO Configuration
    PB4 (NJTRST)     ------> ADF1_SDI0
    PB3 (JTDO/TRACESWO)     ------> ADF1_CCK0
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_ADF1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN ADF1_MspInit 1 */

  /* USER CODE END ADF1_MspInit 1 */
  }

}

/**
* @brief MDF MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hmdf: MDF handle pointer
* @retval None
*/
void HAL_MDF_MspDeInit(MDF_HandleTypeDef* hmdf)
{
  if(IS_ADF_INSTANCE(hmdf->Instance))
  {
  /* USER CODE BEGIN ADF1_MspDeInit 0 */

  /* USER CODE END ADF1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADF1_CLK_DISABLE();

    /**ADF1 GPIO Configuration
    PB4 (NJTRST)     ------> ADF1_SDI0
    PB3 (JTDO/TRACESWO)     ------> ADF1_CCK0
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4|GPIO_PIN_3);

  /* USER CODE BEGIN ADF1_MspDeInit 1 */

  /* USER CODE END ADF1_MspDeInit 1 */
  }

}

static uint32_t SAI1_client =0;

void HAL_SAI_MspInit(SAI_HandleTypeDef* hsai)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
/* SAI1 */
    if(hsai->Instance==SAI1_Block_B)
    {
      /* Peripheral clock enable */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
    PeriphClkInit.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL3;
    PeriphClkInit.PLL3.PLL3Source = RCC_PLLSOURCE_MSI;
    PeriphClkInit.PLL3.PLL3M = 1;
    PeriphClkInit.PLL3.PLL3N = 80;
    PeriphClkInit.PLL3.PLL3P = 28;
    PeriphClkInit.PLL3.PLL3Q = 28;
    PeriphClkInit.PLL3.PLL3R = 2;
    PeriphClkInit.PLL3.PLL3RGE = RCC_PLLVCIRANGE_0;
    PeriphClkInit.PLL3.PLL3FRACN = 0;
    PeriphClkInit.PLL3.PLL3ClockOut = RCC_PLL3_DIVP;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

      if (SAI1_client == 0)
      {
       __HAL_RCC_SAI1_CLK_ENABLE();
      }
    SAI1_client ++;

    /**SAI1_B_Block_B GPIO Configuration
    PF8     ------> SAI1_SCK_B
    PF7     ------> SAI1_MCLK_B
    PF9     ------> SAI1_FS_B
    PF6     ------> SAI1_SD_B
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_SAI1;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    }
}

void HAL_SAI_MspDeInit(SAI_HandleTypeDef* hsai)
{
/* SAI1 */
    if(hsai->Instance==SAI1_Block_B)
    {
    SAI1_client --;
      if (SAI1_client == 0)
      {
      /* Peripheral clock disable */
      __HAL_RCC_SAI1_CLK_DISABLE();
      }

    /**SAI1_B_Block_B GPIO Configuration
    PF8     ------> SAI1_SCK_B
    PF7     ------> SAI1_MCLK_B
    PF9     ------> SAI1_FS_B
    PF6     ------> SAI1_SD_B
    */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_6);

    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
