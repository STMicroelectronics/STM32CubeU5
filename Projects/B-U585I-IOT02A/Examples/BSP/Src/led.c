/**
  ******************************************************************************
  * @file    led.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the led display feature in the 
  *          b_u585i_iot02 driver
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

/** @addtogroup STM32U5xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Led demo
  * @param  None
  * @retval 0 if ok, else value < 0.
  */
int32_t Led_demo(void)
{
  int32_t  result = 0;

  printf("\n******LEDs EXAMPLE******\n");
  
  StartExample();

  /* Initialize the LEDs */
  if (BSP_LED_Init(LED6) != BSP_ERROR_NONE) result--;
  if (BSP_LED_Init(LED7) != BSP_ERROR_NONE) result--;

  /* Switch on the LEDs */
  printf("Switch on LED6\n");
  if (BSP_LED_On(LED6) != BSP_ERROR_NONE) result--;
  printf("Switch on LED7\n");
  if (BSP_LED_On(LED7) != BSP_ERROR_NONE) result--;

  /* Check LEDs states */
  if (BSP_LED_GetState(LED6) != 1) result--;
  if (BSP_LED_GetState(LED7) != 1) result--;
  
  /* Check result */
  if (CheckResult() != 0U) result--;
  
  /* Toggle the LEDs */
  printf("Switch off LED6\n");
  if (BSP_LED_Toggle(LED6) != BSP_ERROR_NONE) result--;
  printf("Switch off LED7\n");
  if (BSP_LED_Toggle(LED7) != BSP_ERROR_NONE) result--;

  /* Check LEDs states */
  if (BSP_LED_GetState(LED6) != 0) result--;
  if (BSP_LED_GetState(LED7) != 0) result--;
  
  /* Check result */
  if (CheckResult() != 0U) result--;

  /* Toggle the LEDs */
  printf("Switch on LED6\n");
  if (BSP_LED_Toggle(LED6) != BSP_ERROR_NONE) result--;
  printf("Switch on LED7\n");
  if (BSP_LED_Toggle(LED7) != BSP_ERROR_NONE) result--;

  /* Check result */
  if (CheckResult() != 0U) result--;

  /* Switch off the LEDs */
  printf("Switch off LED6\n");
  if (BSP_LED_Off(LED6) != BSP_ERROR_NONE) result--;
  printf("Switch off LED7\n");
  if (BSP_LED_Off(LED7) != BSP_ERROR_NONE) result--;

  /* Check result */
  if (CheckResult() != 0U) result--;

  /* De-Initialize the LEDs */
  if (BSP_LED_DeInit(LED6) != BSP_ERROR_NONE) result--;
  if (BSP_LED_DeInit(LED7) != BSP_ERROR_NONE) result--;
  
  printf("LEDs Example Ended\n");
  return result;
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
