/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stlogo.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TSCx_TS1_MAXTHRESHOLD 15 /* This value can be modified, board layout dependency */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

TSC_HandleTypeDef htsc;

/* USER CODE BEGIN PV */
__IO FlagStatus UserButtonPressed   = RESET;
/* Variable used to store the acquisition value of the channem */
__IO uint32_t uhTSCAcquisitionValue;
uint32_t TSCUntouchedValue;
TSC_IOConfigTypeDef IoConfig;

/* Components Initialization Status */
FlagStatus ButtonInitialized = RESET;
FlagStatus LcdInitialized    = RESET;
FlagStatus LedInitialized    = RESET;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_ICACHE_Init(void);
static void MX_GPIO_Init(void);
static void MX_TSC_Init(void);
/* USER CODE BEGIN PFP */
static void MPU_Config(void);
static void TscCalibration(void);
static void SystemHardwareInit(void);
static void Display_DemoDescription(void);
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
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* Configure the MPU attributes for SRAM */
  MPU_Config();

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the System Power */
  SystemPower_Config();

  /* USER CODE BEGIN SysInit */

  /* System Hardware components initialization (Leds, button and LCD) */
  SystemHardwareInit();

  /* Display demo description */
  Display_DemoDescription();

  /* Wait for button press */
  while (UserButtonPressed != SET);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  MX_GPIO_Init();
  MX_TSC_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /* TSC calibration, to store the untouched value */
  TscCalibration();

  while (1)
  {
    /*##-1- Discharge the touch-sensing IOs ####################################*/
    HAL_TSC_IODischarge(&htsc, ENABLE);
    /* Note: a delay can be added here */
    HAL_Delay(1);

    /*##-2- Check if the acquisition is correct (no max count) #################*/
    if (HAL_TSC_GroupGetStatus(&htsc, TSC_GROUP4_IDX) == TSC_GROUP_COMPLETED)
    {
      /*##-3- Read the acquisition value #######################################*/
      uhTSCAcquisitionValue = HAL_TSC_GroupGetValue(&htsc, TSC_GROUP4_IDX);

      /* Note: Show the touch activity on LEDs and LCD.
         The threshold value is indicative and may need to be adjusted */
      if ((TSCUntouchedValue - uhTSCAcquisitionValue) > TSCx_TS1_MAXTHRESHOLD)
      {
        /* TS1 touch detected */
        UTIL_LCD_FillRect(0, 150, 320, 50, UTIL_LCD_COLOR_GREEN);
        HAL_Delay(10);
        BSP_LED_On(LED5);
        BSP_LED_Off(LED6);
      }
      else
      {
        /* TS1 not touched */;
        UTIL_LCD_FillRect(0, 150, 320, 50, UTIL_LCD_COLOR_BLUE);
        BSP_LED_Off(LED5);
        BSP_LED_On(LED6);
      }
    }

    /*##-4- Re-start the acquisition process ###################################*/
    if (HAL_TSC_Start(&htsc) != HAL_OK)
    {
      /* Acquisition Error */
      Error_Handler();
    }
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV1;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLLVCIRANGE_0;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
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
  HAL_PWREx_DisableUCPDDeadBattery();

  /*
   * Switch to SMPS regulator instead of LDO
   */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }
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
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief TSC Initialization Function
  * @param None
  * @retval None
  */
static void MX_TSC_Init(void)
{

  /* USER CODE BEGIN TSC_Init 0 */

  /* USER CODE END TSC_Init 0 */

  /* USER CODE BEGIN TSC_Init 1 */

  /* USER CODE END TSC_Init 1 */

  /** Configure the TSC peripheral
  */
  htsc.Instance = TSC;
  htsc.Init.CTPulseHighLength = TSC_CTPH_2CYCLES;
  htsc.Init.CTPulseLowLength = TSC_CTPL_2CYCLES;
  htsc.Init.SpreadSpectrum = ENABLE;
  htsc.Init.SpreadSpectrumDeviation = 127;
  htsc.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  htsc.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV64;
  htsc.Init.MaxCountValue = TSC_MCV_16383;
  htsc.Init.IODefaultMode = TSC_IODEF_OUT_PP_LOW;
  htsc.Init.SynchroPinPolarity = TSC_SYNC_POLARITY_FALLING;
  htsc.Init.AcquisitionMode = TSC_ACQ_MODE_NORMAL;
  htsc.Init.MaxCountInterrupt = DISABLE;
  htsc.Init.ChannelIOs = TSC_GROUP4_IO1;
  htsc.Init.ShieldIOs = TSC_GROUP1_IO2;
  htsc.Init.SamplingIOs = TSC_GROUP1_IO3|TSC_GROUP4_IO2;
  if (HAL_TSC_Init(&htsc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TSC_Init 2 */

  /*##-2- Configure the touch-sensing IOs ####################################*/
  IoConfig.ChannelIOs  = TSC_GROUP4_IO1;
  IoConfig.SamplingIOs = (TSC_GROUP4_IO2 | TSC_GROUP1_IO3);
  IoConfig.ShieldIOs   = TSC_GROUP1_IO2;

  if (HAL_TSC_IOConfig(&htsc, &IoConfig) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-3- Discharge the touch-sensing IOs ####################################*/
  HAL_TSC_IODischarge(&htsc, ENABLE);
  HAL_Delay(1); /* 1 ms is more than enough to discharge all capacitors */

  /*##-4- Start the acquisition process ######################################*/
  if (HAL_TSC_Start(&htsc) != HAL_OK)
  {
    /* Acquisition Error */
    Error_Handler();
  }
  /* USER CODE END TSC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : BUTTON_USER_Pin */
  GPIO_InitStruct.Pin = BUTTON_USER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_USER_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI13_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI13_IRQn);

}

/* USER CODE BEGIN 4 */

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_13)
  {
    UserButtonPressed = SET;
  }
}

/**
  * @brief  System Hardware Configuration at Boot
  * @param  None
  * @retval None
  */
static void SystemHardwareInit(void)
{
  /* Init LEDs  */
  if (LedInitialized != SET)
  {
    if (BSP_LED_Init(LED5) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    if (BSP_LED_Init(LED6) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    LedInitialized = SET;
  }

  /* Init User push-button in EXTI Mode */
  if (ButtonInitialized != SET)
  {
    if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    ButtonInitialized = SET;
  }

  /* Initialize the LCD */
  if (LcdInitialized != SET)
  {
    /* Initialize the LCD */
    if (BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }

    /* Set LCD functions */
    UTIL_LCD_SetFuncDriver(&LCD_Driver);

    /* Clear the LCD */
    UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

    /* Set the display on */
    if (BSP_LCD_DisplayOn(0) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }

    LcdInitialized = SET;
  }
}

/**
  * @brief  Touch Sensing Controller Calibration
  * @param  None
  * @retval None
  */
static void TscCalibration(void)
{
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillRect(0, 150, 320, 50, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DisplayStringAt(0, 165, (uint8_t *)"CALIBRATION..", CENTER_MODE);
  HAL_Delay(500);

  /*##-1- Discharge the touch-sensing IOs ####################################*/
  HAL_TSC_IODischarge(&htsc, ENABLE);
  /* Note: a delay can be added here */
  HAL_Delay(1);

  /*##-2- Check if the acquisition is correct (no max count) #################*/
  if (HAL_TSC_GroupGetStatus(&htsc, TSC_GROUP4_IDX) == TSC_GROUP_COMPLETED)
  {
    /*##-3- Read the acquisition value #######################################*/
    uhTSCAcquisitionValue = HAL_TSC_GroupGetValue(&htsc, TSC_GROUP4_IDX);

    /* Store the registered value */
    TSCUntouchedValue = uhTSCAcquisitionValue;
  }

  /*##-4- Re-start the acquisition process ###################################*/
  if (HAL_TSC_Start(&htsc) != HAL_OK)
  {
    /* Acquisition Error */
    Error_Handler();
  }
}

/**
  * @brief  Display main demo messages
  * @param  None
  * @retval None
  */
static void Display_DemoDescription(void)
{
  /* Set font */
  UTIL_LCD_SetFont(&Font24);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKBLUE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);

  /* Display LCD messages */
  UTIL_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32U575I-EV", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 35, (uint8_t *)"TSC example", CENTER_MODE);

  /* Draw Bitmap */
  UTIL_LCD_DrawBitmap(120, 65, (uint8_t *)stlogo);

  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 220, (uint8_t *)"Copyright (c) STMicroelectronics 2021", CENTER_MODE);

  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_FillRect(0, 150, 320, 50, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DisplayStringAt(0, 150, (uint8_t *)"Press USER push-button", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 165, (uint8_t *)"to start:", CENTER_MODE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ORANGE);
  UTIL_LCD_DisplayStringAt(0, 180, (uint8_t *)"Touch Sensing Controller TSC", CENTER_MODE);
}

/**
  * @brief  Configure the MPU attributes.
  * @note   The Base Address is External SRAM
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Attributes_InitTypeDef   attr;
  MPU_Region_InitTypeDef       region;

  /* Disable MPU before perloading and config update */
  HAL_MPU_Disable();

  /* Define cacheable memory via MPU */
  attr.Number             = MPU_ATTRIBUTES_NUMBER0;
  attr.Attributes         = 0 ;
  HAL_MPU_ConfigMemoryAttributes(&attr);

  /* BaseAddress-LimitAddress configuration */
  region.Enable           = MPU_REGION_ENABLE;
  region.Number           = MPU_REGION_NUMBER0;
  region.AttributesIndex  = MPU_ATTRIBUTES_NUMBER0;
  region.BaseAddress      = FMC_ADDRESS;
  region.LimitAddress     = FMC_ADDRESS + REGION_SIZE - 1;
  region.AccessPermission = MPU_REGION_ALL_RW;
  region.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  region.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  HAL_MPU_ConfigRegion(&region);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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
  __disable_irq();
  while (1)
  {
    BSP_LED_Toggle(LED6);
    HAL_Delay(200);
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
