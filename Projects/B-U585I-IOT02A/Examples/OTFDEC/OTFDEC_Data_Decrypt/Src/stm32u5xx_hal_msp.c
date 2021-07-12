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

  /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
  */
  HAL_PWREx_DisableUCPDDeadBattery();

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief CRYP MSP Initialization
* This function configures the hardware resources used in this example
* @param hcryp: CRYP handle pointer
* @retval None
*/
void HAL_CRYP_MspInit(CRYP_HandleTypeDef* hcryp)
{
  if(hcryp->Instance==AES)
  {
  /* USER CODE BEGIN AES_MspInit 0 */

  /* USER CODE END AES_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_AES_CLK_ENABLE();
  /* USER CODE BEGIN AES_MspInit 1 */

  /* USER CODE END AES_MspInit 1 */
  }

}

/**
* @brief CRYP MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hcryp: CRYP handle pointer
* @retval None
*/
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef* hcryp)
{
  if(hcryp->Instance==AES)
  {
  /* USER CODE BEGIN AES_MspDeInit 0 */

  /* USER CODE END AES_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_AES_CLK_DISABLE();
  /* USER CODE BEGIN AES_MspDeInit 1 */

  /* USER CODE END AES_MspDeInit 1 */
  }

}

/**
* @brief OSPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hospi: OSPI handle pointer
* @retval None
*/
void HAL_OSPI_MspInit(OSPI_HandleTypeDef* hospi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(hospi->Instance==OCTOSPI2)
  {
  /* USER CODE BEGIN OCTOSPI2_MspInit 0 */

  /* USER CODE END OCTOSPI2_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_OSPI;
    PeriphClkInit.OspiClockSelection = RCC_OSPICLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_OSPI2_CLK_ENABLE();

    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**OCTOSPI2 GPIO Configuration
    PI5     ------> OCTOSPIM_P2_NCS
    PH12     ------> OCTOSPIM_P2_IO7
    PH10     ------> OCTOSPIM_P2_IO5
    PH11     ------> OCTOSPIM_P2_IO6
    PF0     ------> OCTOSPIM_P2_IO0
    PH9     ------> OCTOSPIM_P2_IO4
    PF1     ------> OCTOSPIM_P2_IO1
    PF2     ------> OCTOSPIM_P2_IO2
    PF3     ------> OCTOSPIM_P2_IO3
    PF4     ------> OCTOSPIM_P2_CLK
    PF12     ------> OCTOSPIM_P2_DQS
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_OCTOSPI2;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_OCTOSPI2;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_OCTOSPI2;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* OCTOSPI2 interrupt Init */
    HAL_NVIC_SetPriority(OCTOSPI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OCTOSPI2_IRQn);
  /* USER CODE BEGIN OCTOSPI2_MspInit 1 */

  /* USER CODE END OCTOSPI2_MspInit 1 */
  }

}

/**
* @brief OSPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hospi: OSPI handle pointer
* @retval None
*/
void HAL_OSPI_MspDeInit(OSPI_HandleTypeDef* hospi)
{
  if(hospi->Instance==OCTOSPI2)
  {
  /* USER CODE BEGIN OCTOSPI2_MspDeInit 0 */

  /* USER CODE END OCTOSPI2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_OSPI2_CLK_DISABLE();

    /**OCTOSPI2 GPIO Configuration
    PI5     ------> OCTOSPIM_P2_NCS
    PH12     ------> OCTOSPIM_P2_IO7
    PH10     ------> OCTOSPIM_P2_IO5
    PH11     ------> OCTOSPIM_P2_IO6
    PF0     ------> OCTOSPIM_P2_IO0
    PH9     ------> OCTOSPIM_P2_IO4
    PF1     ------> OCTOSPIM_P2_IO1
    PF2     ------> OCTOSPIM_P2_IO2
    PF3     ------> OCTOSPIM_P2_IO3
    PF4     ------> OCTOSPIM_P2_CLK
    PF12     ------> OCTOSPIM_P2_DQS
    */
    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_5);

    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_12|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_12);

    /* OCTOSPI2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(OCTOSPI2_IRQn);
  /* USER CODE BEGIN OCTOSPI2_MspDeInit 1 */

  /* USER CODE END OCTOSPI2_MspDeInit 1 */
  }

}

/**
* @brief OTFDEC MSP Initialization
* This function configures the hardware resources used in this example
* @param hotfdec: OTFDEC handle pointer
* @retval None
*/
void HAL_OTFDEC_MspInit(OTFDEC_HandleTypeDef* hotfdec)
{
  if(hotfdec->Instance==OTFDEC2)
  {
  /* USER CODE BEGIN OTFDEC2_MspInit 0 */

  /* USER CODE END OTFDEC2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_OTFDEC2_CLK_ENABLE();
    /* OTFDEC2 interrupt Init */
    HAL_NVIC_SetPriority(OTFDEC2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OTFDEC2_IRQn);
  /* USER CODE BEGIN OTFDEC2_MspInit 1 */

  /* USER CODE END OTFDEC2_MspInit 1 */
  }

}

/**
* @brief OTFDEC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hotfdec: OTFDEC handle pointer
* @retval None
*/
void HAL_OTFDEC_MspDeInit(OTFDEC_HandleTypeDef* hotfdec)
{
  if(hotfdec->Instance==OTFDEC2)
  {
  /* USER CODE BEGIN OTFDEC2_MspDeInit 0 */

  /* USER CODE END OTFDEC2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_OTFDEC2_CLK_DISABLE();

    /* OTFDEC2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(OTFDEC2_IRQn);
  /* USER CODE BEGIN OTFDEC2_MspDeInit 1 */

  /* USER CODE END OTFDEC2_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
