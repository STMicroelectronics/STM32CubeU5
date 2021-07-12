/**
  ******************************************************************************
  * @file    button.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the button feature in the 
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
  * @brief  Button example
  * @param  None
  * @retval 0 if ok, else value < 0.
  */
int32_t Button_demo(void)
{
  int32_t  result = 0;
  
  printf("\n******BUTTON EXAMPLE******\n");

  StartExample();
  
  /* ------------------- */
  /* Button in GPIO mode */
  /* ------------------- */
  /* Initialize buttons */
  if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO) != BSP_ERROR_NONE) result--;
  
  /* Check buttons state */
  if (BSP_PB_GetState(BUTTON_USER) != RESET) result--;
  
  /* Ask user to press buttons and check results */
  printf("Please press User button (GPIO mode)\n");
  while (BSP_PB_GetState(BUTTON_USER) != 1);
  printf("user button pressed\n");
  
  /* De-initialize buttons */
  if (BSP_PB_DeInit(BUTTON_USER) != BSP_ERROR_NONE) result--;

  HAL_Delay(1000);
  
  /* ------------------- */
  /* Button in EXTI mode */
  /* ------------------- */
  /* Initialize buttons */
  UserButtonPressed = RESET;
  if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI) != BSP_ERROR_NONE) result--;
  
  /* Check buttons state */
  if (BSP_PB_GetState(BUTTON_USER) != RESET) result--;
  
  /* Ask user to press buttons and check results */
  printf("Please press User button (EXTI mode)\n");
  while (UserButtonPressed == RESET);
  printf("User button pressed\n");
  
  HAL_Delay(1000);
  UserButtonPressed = RESET;

  /* Re-ask user to press buttons and check results */
  printf("Please press one more time User button\n");
  while (UserButtonPressed == RESET);
  printf("User button pressed\n");

  HAL_Delay(1000);
  UserButtonPressed = RESET;

  /* De-initialize buttons */
  if (BSP_PB_DeInit(BUTTON_USER) != BSP_ERROR_NONE) result--;

  printf("Button Example Ended\n");
  return result;
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
