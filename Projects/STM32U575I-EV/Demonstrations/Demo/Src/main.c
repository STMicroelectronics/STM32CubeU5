/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
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
#include <string.h>
#include "main.h"
#include "stm32u575i_eval.h"
#include "stm32u575i_eval_ts.h"
#include "stm32u575i_eval_lcd.h"
#include "stm32u575i_eval_sd.h"
#include "k_config.h"
#include "k_window.h"
#include "k_demo.h"
#include "k_widgets.h"
#include "k_menu.h"
#include "app_about.h"
#include "low_power.h"
#include "app_main.h"
#include "app_security.h"
#include "app_lpbam.h"
#include "app_usbpd.h"

#include "app_calendar.h"
#include "app_audiofilt.h"
#include "app_cameraviewer.h"
#include "resources.h"

/* Private typedef -----------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void CACHE_Enable(void);
static void BootCheckAndExec(void);
/* Private variables ---------------------------------------------------------*/
uint32_t RccBootFlags = 0;
uint32_t PwrSBFlag = 0;
uint8_t  dummy=0;

/* Exported variables --------------------------------------------------------*/
RTC_HandleTypeDef RtcHandle = {0};

/* Exported functions --------------------------------------------------------*/
/**
* @brief  Main program
* @retval int
*/
int main(void)
{

  /* STM32U5xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
             can eventually implement his proper time base source (a general purpose
             timer for example or other time source), keeping in mind that Time base
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
             handled in milliseconds basis.
       - Set NVIC Group Priority to 3
       - Low Level Initialization
  */
  HAL_Init();


  /* This project template calls CACHE_Enable() in order to enable the Instruction
     and Data Cache. This function is provided as template implementation that
     User may integrate in his application in order to enhance performance */

  /* Enable the Instruction Cache */
  CACHE_Enable();
  
  /* Configure the system clock @ 160 MHz (PLL ON) */
  SystemClock_Config();

  /* LED init */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  /* Check Boot Reason */
  BootCheckAndExec();

  /* Enable RTC & backup domain access */
  RTC_Config();


  /* Start the demo thread */
  kDemo_Start();

  /* we should not reach here */
  while(1);
}

/** @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin)
  {
  case JOY_DOWN :
  case JOY_UP :
  case JOY_SEL :
  case JOY_RIGHT :
  case JOY_LEFT :
  case BUTTON_USER :
    kMenu_EventHandler(GPIO_Pin);
    break;
  default :
    Error_Handler();
    break;
  }
}

/** @brief Demonstration kernel initialization
  * @retval None
  */
void kDemo_Initialization(void)
{
  uint32_t pXSize;

  /* Initialize the LCD */
  if (BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE) != BSP_ERROR_NONE)
  {
    /* Initialization Error */
    Error_Handler();
  }

  UTIL_LCD_SetFuncDriver(&LCD_Driver); /* SetFunc before setting device */
  UTIL_LCD_SetDevice(0);            /* SetDevice after funcDriver is set */
  UTIL_LCD_SetFont(&UTIL_LCD_DEFAULT_FONT);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set the display on */
  if (BSP_LCD_DisplayOn(0) != BSP_ERROR_NONE)
  {
    /* Initialization Error */
    Error_Handler();
  }

  
  
  if( (RccBootFlags & RCC_CSR_SFTRSTF) == 0)
  {
    
    /* Display the demonstration window */
    UTIL_LCD_Clear(UTIL_LCD_COLOR_ST_YELLOW);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
    BSP_LCD_GetXSize(0, &pXSize);
    UTIL_LCD_FillRect(0, 0, pXSize, UTIL_LCD_DEFAULT_FONT.Height*2, UTIL_LCD_COLOR_ST_BLUE_DARK);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_DisplayStringAt( 0,              0,      (uint8_t *)EVAL_BOARD, CENTER_MODE);
    UTIL_LCD_DisplayStringAt( 0,  UTIL_LCD_DEFAULT_FONT.Height  , (uint8_t *)"DEMONSTRATION", CENTER_MODE);
    
    BSP_LCD_FillRGBRect(0, (uint32_t)75, (uint32_t)60, (uint8_t *)STLogo, 170, 170);
    
    /* Let time user to see information */
    HAL_Delay(2000);
    
  }
  /* Module Initialization */
  kModule_Init();


}

void kModule_Init(void)
{
   kModule_Add(MODULE_MAIN_APP    , ModuleAppMain);
   kModule_Add(MODULE_SYSINFO     , ModuleSystemInfo);
   kModule_Add(MODULE_LOWPOWER    , ModuleLowPower);
   kModule_Add(MODULE_AUDIOFILT   , ModuleAudiofilt);
   kModule_Add(MODULE_CALENDAR    , ModuleCalendar);
   kModule_Add(MODULE_CAMERA      , ModuleCamera);
   kModule_Add(MODULE_LPBAM       , ModuleLpbam);
   kModule_Add(MODULE_SECURITY    , ModuleTrustZone);
   kModule_Add(MODULE_USBPD       , ModuleUsbPd);
}

void kDemo_UnInitialization(void)
{
  /* Nothing to do */
}

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File pointer to the source file name
* @param  Line assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n",
  file, line) */

  /* Infinite loop */
  while (1)
  {}
}

#endif



/* Private functions ---------------------------------------------------------*/

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 160000000
  *            HCLK(Hz)                       = 160000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            APB3 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_MBOOST                     = 1
  *            PLL_M                          = 1
  *            PLL_N                          = 80
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            PLL_P                          = 2
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable voltage range 1 for frequency above 100 Mhz */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Switch to SMPS regulator instead of LDO */
  HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY);

  __HAL_RCC_PWR_CLK_DISABLE();

  /* MSI Oscillator enabled at reset (4Mhz), activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_4;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  /* Set LSE configuration */
  RCC_OscInitStruct.LSEState            = RCC_LSE_ON;
  /* Set PLL configuration */
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLMBOOST       = RCC_PLLMBOOST_DIV1;
  RCC_OscInitStruct.PLL.PLLM            = 1;
  RCC_OscInitStruct.PLL.PLLN            = 80;
  RCC_OscInitStruct.PLL.PLLR            = 2;
  RCC_OscInitStruct.PLL.PLLP            = 2;
  RCC_OscInitStruct.PLL.PLLQ            = 2;
  RCC_OscInitStruct.PLL.PLLFRACN        = 0;
  RCC_OscInitStruct.PLL.PLLRGE          = RCC_PLLVCIRANGE_0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Select PLL as system clock source and confisgure bus clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
                                      RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Enable ICACHE with 1-way set-associative configuration.
  * @param  None
  * @retval None
  */
static void CACHE_Enable(void)
{
  /* Configure ICACHE associativity mode */
  HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY);

  /* Enable ICACHE */
  HAL_ICACHE_Enable();
}


/**
  * @brief  Configures the RTC & backup domain access
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{
  /* Enable Power Clock*/
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Allow Access to RTC Backup domaine */
  HAL_PWR_EnableBkUpAccess();

  /* Set the RTC time base to 1s */
  RtcHandle.Instance = RTC;
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = 0x7Fu;
  RtcHandle.Init.SynchPrediv = 0x00FFu;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Set Calendar Ultra-Low power mode */
  if (HAL_RTCEx_SetLowPowerCalib(&RtcHandle, RTC_LPCAL_SET) != HAL_OK)
  {
    Error_Handler();
  }
}

void RTC_UnConfig(void)
{
  if(HAL_RTC_DeInit(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  Check boot reason
  * @retval None
  */
static void BootCheckAndExec(void)
{
  /* Get reset flags */
  RccBootFlags = RCC->CSR & (RCC_CSR_PINRSTF | RCC_CSR_BORRSTF | RCC_CSR_SFTRSTF
                             | RCC_CSR_OBLRSTF | RCC_CSR_IWDGRSTF | RCC_CSR_WWDGRSTF | RCC_CSR_LPWRRSTF);

  /* Clear reset flags */
  __HAL_RCC_CLEAR_RESET_FLAGS();

  /* Get Standby flag */
  PwrSBFlag = __HAL_PWR_GET_FLAG(PWR_FLAG_SBF);

  /* Clear Standby flag */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SBF);
}

/**
  * @brief  Error Handler.
  * @retval None
  */
void Error_Handler(void)
{
  while(1)
  {
    HAL_Delay(100);
    BSP_LED_Toggle(LED_RED);
  }
}

/**
  * @brief  BSP Joystick Callback.
  * @param  JoyPin Specifies the pin connected EXTI line
  * @retval None
  */
void BSP_JOY_Callback(JOY_TypeDef JOY, uint32_t JoyPin)
{

  switch(JoyPin)
  {
  case JOY_DOWN :
  case JOY_UP :
  case JOY_SEL :
  case JOY_RIGHT :
  case JOY_LEFT :
    kMenu_EventHandler(JoyPin);
    break;
  default :
    break;
  }
  
}

/**
  * @brief  BSP TS Callback.
  * @param  Instance TS Instance.
  * @retval None.
  */
void BSP_TS_Callback(uint32_t Instance)
{
  if (Instance == 0)
  {
    TS_State_t TS_State;

    BSP_TS_GetState(0, &TS_State);
    if((TS_State.TouchX > 280) && (TS_State.TouchY < 35))
    {
      kMenu_EventHandler(JOY_SEL);
    }
    else
    {
      kMenu_EventHandler(JOY_RIGHT);
    }

  }
}

