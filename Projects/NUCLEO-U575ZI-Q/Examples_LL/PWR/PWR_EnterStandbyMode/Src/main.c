/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/PWR/PWR_EnterStandbyMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to enter and exit the Standby mode with
  *          a wakeup pin or external reset through the STM32U5xx PWR LL API.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUTTON_MODE_EXTI  1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */

void     Configure_PWR(void);
void     LED_Blinking(uint32_t Period);
void     UserButton_Init(uint32_t Button_Mode);
uint32_t UserButton_GetState(void);
void     EnterStandbyMode(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* Enable PWR clock interface */

  LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_PWR);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the System Power */
  SystemPower_Config();

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /* Initialize User push-button in EXTI mode */
  UserButton_Init(BUTTON_MODE_EXTI);

  /* Configure Power IP */
  Configure_PWR();
  /* Led blinking in Run mode */
  LED_Blinking(LED_BLINK_FAST);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_4)
  {
  }

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_MSIS_Enable();

   /* Wait till MSIS is ready */
  while(LL_RCC_MSIS_IsReady() != 1)
  {
  }

  LL_RCC_MSI_EnableRangeSelection();
  LL_RCC_MSIS_SetRange(LL_RCC_MSISRANGE_4);
  LL_RCC_MSI_SetCalibTrimming(16, LL_RCC_MSI_OSCILLATOR_1);
  LL_RCC_PLL1_ConfigDomain_SYS(LL_RCC_PLL1SOURCE_MSIS, 1, 80, 2);
  LL_RCC_PLL1_EnableDomain_SYS();
  LL_RCC_SetPll1EPodPrescaler(LL_RCC_PLL1MBOOST_DIV_1);
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_4_8);
  LL_RCC_PLL1_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

   /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {
  }

  /* Insure 1us transition state at intermediate medium speed clock*/
  for (__IO uint32_t i = (160 >> 1); i !=0; i--);

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_1);

  LL_Init1msTick(160000000);

  LL_SetSystemCoreClock(160000000);
}

/**
  * @brief Power Configuration
  * @retval None
  */
static void SystemPower_Config(void)
{

  /*
   * Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
   */
  LL_PWR_DisableUCPDDeadBattery();

  /*
   * Switch to SMPS regulator instead of LDO
   */
  LL_PWR_SetRegulatorSupply(LL_PWR_SMPS_SUPPLY);

  while(LL_PWR_IsActiveFlag_REGULATOR()!=1)
  {
  }
/* USER CODE BEGIN PWR */
/* USER CODE END PWR */
}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  LL_ICACHE_SetMode(LL_ICACHE_1WAY);
  LL_ICACHE_Enable();
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

  /**/
  LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);

  /**/
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    LL_mDelay(Period);
  }
}

/**
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  ButtonMode: Specifies Button mode.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability
  * @retval None
  */
void UserButton_Init(uint32_t Button_Mode)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();

  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);

  if(Button_Mode == BUTTON_MODE_EXTI)
  {
    /* Connect External Line to the GPIO */
    USER_BUTTON_SYSCFG_SET_EXTI();

    /* Enable a Rising trigger EXTI line 13 Interrupt */
    USER_BUTTON_EXTI_LINE_ENABLE();
    USER_BUTTON_EXTI_RISING_TRIG_ENABLE();

    /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
    NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn);
    NVIC_SetPriority(USER_BUTTON_EXTI_IRQn,0x03);
  }
}

/**
  * @brief  Returns the selected Button state.
  * @param  None
  * @retval The Button GPIO pin value.
  */
uint32_t UserButton_GetState(void)
{
  return LL_GPIO_IsInputPinSet(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN);
}

/**
  * @brief  Function to configure and initialize PWR IP.
  * @param  None
  * @retval None
  */
void Configure_PWR(void)
{
  /* Enable Power Clock */
  LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_PWR);

  /* Check if the system was resumed from StandBy mode */
  if (LL_PWR_IsActiveFlag_SB() != 0)
  {
    /* Clear Standby flag */
    LL_PWR_ClearFlag_SB();

    /* Change LED speed to SLOW to indicate exit from Standby mode */
    LED_Blinking(LED_BLINK_SLOW);

    /* Wait that user release the User push-button */
    while(UserButton_GetState() == 1){}
  }

  /* Check and Clear the Wakeup flag */
  if (LL_PWR_IsActiveFlag_WU2() != 0)
  {
    LL_PWR_ClearFlag_WU2();
  }
}

/**
  * @brief  Function to configure and enter in Standby Mode.
  * @param  None
  * @retval None
  */
void EnterStandbyMode(void)
{
  /* Wait that user release the User push-button */
  while(UserButton_GetState() == 1){}

  /* Turn-off LED */
  /* Note: LED state at this step depends on blinking state at the instant of user button is pressed. */
  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7);

  /* Disable all used wakeup sources */
  LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN2);

  /* Clear all wake up Flag */
  LL_PWR_ClearFlag_WU();
  
  /* Set the wakeup pin selection 1 */
  LL_PWR_SetWakeUpPinSignal1Selection(LL_PWR_WAKEUP_PIN2);

  /* Set wakeup pin polarity */
  LL_PWR_SetWakeUpPinPolarityHigh(LL_PWR_WAKEUP_PIN2);

  /* Enable wakeup pin */
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN2);

  /* As default User push-button state is high level, need to clear all wake up Flag again */
  LL_PWR_ClearFlag_WU();

  /** Request to enter Standby mode
    * Following procedure describe in STM32U5xx Reference Manual
    * See PWR part, section Low-power modes, Standby mode
    */
  /* Set Standby mode when CPU enters deepsleep */
  LL_PWR_SetPowerMode(LL_PWR_STANDBY_MODE);

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  LL_LPM_EnableDeepSleep();

  /* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM)
  __force_stores();
#endif
  /* Request Wait For Interrupt */
  __WFI();
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage BUTTON IRQ Handler
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Turn LED off */
  LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);

  /* Configure and enter in Standby Mode */
  EnterStandbyMode();

  /* Here Device is in Standby mode */
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }

  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
