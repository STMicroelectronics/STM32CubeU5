/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/RCC/RCC_UseHSI_PLLasSystemClock/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to change dynamically SYSCLK through
  *          the STM32U5xx RCC LL API.
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

/* Structure based on parameters used for PLL config */
typedef struct
{
  uint32_t Frequency;   /*!< SYSCLK frequency requested */
  uint32_t PLLM;        /*!< PLLM factor used for PLL */
  uint32_t PLLN;        /*!< PLLN factor used for PLL */
  uint32_t PLLR;        /*!< PLLN factor used for PLL */
  uint32_t Latency;     /*!< Latency to be used with SYSCLK frequency */
} RCC_PLL_ConfigTypeDef;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Number of PLL Config */
#define RCC_PLL_CONFIG_NB   2
#define RCC_FREQUENCY_LOW          ((uint32_t)40000000) /* Low Frequency set to 40MHz*/
#define RCC_FREQUENCY_HIGH         ((uint32_t)160000000) /* High Frequency set to 160MHz*/

/* Oscillator time-out values */
#define HSE_TIMEOUT_VALUE          ((uint32_t)5000) /* Time out for HSE start up, in ms */
#define HSI_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define MSI_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define PLL_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define CLOCKSWITCH_TIMEOUT_VALUE  ((uint32_t)5000) /* 5 s    */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* Variable to set different PLL config with HSI as PLL source clock */
static RCC_PLL_ConfigTypeDef aPLL_ConfigHSI[RCC_PLL_CONFIG_NB] =
{
  {RCC_FREQUENCY_LOW, 4, 20, 2, LL_FLASH_LATENCY_2},
  {RCC_FREQUENCY_HIGH, 2, 40, 2, LL_FLASH_LATENCY_8},
};

/* PLL Config index */
__IO uint8_t bPLLIndex = 0;

/* Variable to save the current configuration to apply */
static uint32_t uwFrequency = RCC_FREQUENCY_HIGH, uwPLLN = 0, uwPLLM = 0, uwPLLR  = 0, uwLatency = 0;

/* Variable to indicate a change of PLL config after a button press */
__IO uint8_t bButtonPress = 0;

uint32_t Timeout = 0; /* Variable used for Timeout management */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */

uint32_t RCC_StartHSIAndWaitForHSIReady(void);
uint32_t ChangePLL_HSI_Config(void);
void     LED_Blinking(uint32_t Period);
uint32_t ChangePLLConfiguration(uint32_t PLLSource, uint32_t PLLM, uint32_t PLLN, uint32_t PLLR);

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

  /* Enable HSI to be able to select it as sysclk source */
  /* Start HSI and wait for ready */
  if (RCC_StartHSIAndWaitForHSIReady() != RCC_ERROR_NONE)
  {
    /* Problem to enable HSI, blink LED1 */
    LED_Blinking(LED_BLINK_ERROR);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Toggle LED accordingly to the frequency */
    if (uwFrequency == RCC_FREQUENCY_LOW)
    {
      /* Slow toggle */
      LED_Blinking(LED_BLINK_SLOW);
    }
    else
    {
      /* Fast toggle */
      LED_Blinking(LED_BLINK_FAST);
    }

    /* PLL config change has been requested */
    if (ChangePLL_HSI_Config() != RCC_ERROR_NONE)
    {
      /* Problem to switch to HSI, blink LED1 */
      LED_Blinking(LED_BLINK_ERROR);
    }

    /* Reset button press */
    bButtonPress = 0;
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
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

  /**/
  LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);

  /**/
  LL_EXTI_SetEXTISource(LL_EXTI_EXTI_PORTC, LL_EXTI_EXTI_LINE13);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_13;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin, LL_GPIO_PULL_NO);

  /**/
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin, LL_GPIO_MODE_INPUT);

  /**/
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI13_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3, 0));
  NVIC_EnableIRQ(EXTI13_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief  Enable HSI and Wait for HSI ready
  * @param  None
  * @retval RCC_ERROR_NONE if no error
  */
uint32_t RCC_StartHSIAndWaitForHSIReady()
{
  /* Enable HSI and wait for HSI ready*/
  LL_RCC_HSI_Enable();

#if (USE_TIMEOUT == 1)
  Timeout = HSI_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
  while (LL_RCC_HSI_IsReady() != 1)
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the Time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
      if (Timeout-- == 0)
      {
        /* Time-out occurred. Return an error */
        return RCC_ERROR_TIMEOUT;
      }
    }
#endif /* USE_TIMEOUT */
  }

  return RCC_ERROR_NONE;
}

/**
  * @brief  Switch the PLL source to HSI, and select the PLL as SYSCLK
  *         source to reach new requested frequency.
  * @param  None
  * @retval RCC_ERROR_NONE if no error
  */
uint32_t ChangePLL_HSI_Config(void)
{
  /* Select HSI as system clock */
  /* Wait for HSI switched */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
#if (USE_TIMEOUT == 1)
  Timeout = CLOCKSWITCH_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
      if (Timeout-- == 0)
      {
        /* Time-out occurred. Return an error */
        return RCC_ERROR_TIMEOUT;
      }
    }
#endif /* USE_TIMEOUT */
  }

  /* Configure PLL with new configuration */
  if (ChangePLLConfiguration(LL_RCC_PLL1SOURCE_HSI, uwPLLM, uwPLLN, uwPLLR) != RCC_ERROR_NONE)
  {
    return RCC_ERROR_TIMEOUT;
  }

  /* Latency must be managed differently if increase or decrease the frequency */
  if (uwFrequency == RCC_FREQUENCY_LOW)
  {
    /* Decrease Frequency - latency should be set after setting PLL as clock source */
    /* Select PLL as system clock */
    /* Wait until the PLL is switched on */
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);
#if (USE_TIMEOUT == 1)
    Timeout = CLOCKSWITCH_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
    {
#if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        if (Timeout-- == 0)
        {
          /* Time-out occurred. Return an error */
          return RCC_ERROR_TIMEOUT;
        }
      }
#endif /* USE_TIMEOUT */
    }

    /* Set new latency */
    LL_FLASH_SetLatency(uwLatency);
  }
  else
  {
    /* Increase Frequency - latency should be set before setting PLL as clock source */
    /* Set new latency */
    LL_FLASH_SetLatency(uwLatency);

    /* Select PLL as system clock */
    /* Wait until the PLL is switched on */
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);
#if (USE_TIMEOUT == 1)
    Timeout = CLOCKSWITCH_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
    {
#if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        if (Timeout-- == 0)
        {
          /* Time-out occurred. Return an error */
          return RCC_ERROR_TIMEOUT;
        }
      }
#endif /* USE_TIMEOUT */
    }
  }

  /* Set systick to 1ms */
  LL_Init1msTick(uwFrequency);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  SystemCoreClock = uwFrequency;

  return RCC_ERROR_NONE;
}

/**
  * @brief  Function to change Main PLL configuration
 #error
  * @retval RCC_ERROR_NONE if no error
  */
uint32_t ChangePLLConfiguration(uint32_t PLLSource, uint32_t PLLM, uint32_t PLLN, uint32_t PLLR)
{
  /* Disable the PLL */
  /* Wait until PLLRDY is cleared */
  LL_RCC_PLL1_Disable();
#if (USE_TIMEOUT == 1)
  Timeout = PLL_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
  while (LL_RCC_PLL1_IsReady() != 0)
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
      if (Timeout-- == 0)
      {
        /* Time-out occurred. Return an error */
        return RCC_ERROR_TIMEOUT;
      }
    }
#endif /* USE_TIMEOUT */
  }

  /* Configure PLL */
  LL_RCC_PLL1_ConfigDomain_SYS(PLLSource, PLLM, PLLN, PLLR);
  /* Enable the PLL */
  LL_RCC_PLL1_Enable();
  /* Wait until PLLRDY is set */
#if (USE_TIMEOUT == 1)
  Timeout = PLL_TIMEOUT_VALUE;
#endif /* USE_TIMEOUT */
  while (LL_RCC_PLL1_IsReady() != 1)
  {
#if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
      if (Timeout-- == 0)
      {
        /* Time-out occurred. Return an error */
        return RCC_ERROR_TIMEOUT;
      }
    }
#endif /* USE_TIMEOUT */
  }

  return RCC_ERROR_NONE;
}

/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  *         Exit of this function when a press button is detected
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  if (Period != LED_BLINK_ERROR)
  {
    /* Toggle IO in an infinite loop up to a detection of press button */
    while (bButtonPress != 1)
    {
      /* LED1 is blinking at Period ms */
      LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
      LL_mDelay(Period);
    }
  }
  else
  {
    /* Toggle IO in an infinite loop due to an error */
    while (1)
    {
      /* Error if LED1 is slowly blinking (1 sec. period) */
      LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
      LL_mDelay(Period);
    }
  }
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Function to manage User button press
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Get the PLL config to apply */
  uwFrequency = aPLL_ConfigHSI[bPLLIndex].Frequency;
  uwPLLM      = aPLL_ConfigHSI[bPLLIndex].PLLM;
  uwPLLN      = aPLL_ConfigHSI[bPLLIndex].PLLN;
  uwPLLR      = aPLL_ConfigHSI[bPLLIndex].PLLR;
  uwLatency   = aPLL_ConfigHSI[bPLLIndex].Latency;

  /* Set new PLL config Index */
  bPLLIndex = (bPLLIndex + 1) % RCC_PLL_CONFIG_NB;

  /* Set variable to request of PLL config change */
  bButtonPress = 1;
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
