/**
  ******************************************************************************
  * @file    Examples/BSP/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
#include "stdio.h"
#include "string.h"
#include "stlogo.h"

/** @addtogroup STM32U5xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private variables --------------------------------------------------------*/
__IO FlagStatus UserButtonPressed   = RESET;
__IO JOYPin_TypeDef JoyPinPressed   = JOY_NONE;
__IO FlagStatus TouchDetected       = RESET;

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[50]; 
  uint32_t DemoIndex;
} BSP_DemoTypedef;

BSP_DemoTypedef  BSP_examples[]=
{
  {Led_demo, "LED", 0},
  {Joystick_demo, "JOYSTICK", 1},
  {LCD_demo, "LCD", 2},
  {Touchscreen_demo, "TouchScreen", 3},
  {Camera_demo, "Camera", 4},
  {OSPI_demo, "OSPI", 5},
  {SD_demo, "mSD", 6},
  {SRAM_demo, "SRAM", 7},
  {MotionSensor_demo, "Motion Sensor", 8},
  {AudioRecord_demo, "Audio Record", 9},
  {AudioPlay_demo, "Audio Play", 10},
  {EEPROM_demo, "EEPROM", 11}
};

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))
#define Screen_Oientation LCD_ORIENTATION_LANDSCAPE

/* Private variables ---------------------------------------------------------*/
uint8_t DemoIndex = 0;
uint32_t CameraTest = 0;

/* Components Initialization Status */
FlagStatus ButtonInitialized = RESET;
FlagStatus JoyInitialized    = RESET;
FlagStatus LcdInitialized    = RESET;
FlagStatus LedInitialized    = RESET;
FlagStatus TsInitialized     = RESET;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void SystemHardwareInit(void);
static void Display_DemoDescription(void);
static void MPU_Config(void);
static void CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32U5xx HAL library initialization:
       - Configure the Flash prefetch
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 3
       - Low Level Initialization
     */
  HAL_Init();
  
  /* Configure the MPU attributes for SRAM */
  MPU_Config();

  /* Enable the Instruction and Data Cache */
  CACHE_Enable();

  /* Configure the System clock to have a frequency of 160 MHz */
  SystemClock_Config();

  /* System common Hardware components initialization (Leds, button, joystick and LCD) */
  SystemHardwareInit();

  /* Display demo description */
  Display_DemoDescription();

  /* Infinite loop */
  while (1)
  {
    if(UserButtonPressed == SET)
    {
      /* Add delay to avoid rebound and reset it status */
      HAL_Delay(500);
      UserButtonPressed = RESET;
      
      BSP_examples[DemoIndex++].DemoFunc();
      
      if(DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
      {
        DemoIndex = 0;
      }
      /* Display demo description */
      Display_DemoDescription();
    }
  }
}

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
  __HAL_RCC_PWR_CLK_DISABLE();

  /* MSI Oscillator enabled at reset (4Mhz), activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV1;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLFRACN= 0;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLLVCIRANGE_0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
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
    if (BSP_LED_Init(LED7) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    HAL_Delay(50);
    if (BSP_LED_Init(LED8) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    LedInitialized = SET;
  }
  
  /* Init User and Tamper push-buttons in EXTI Mode */
  if (ButtonInitialized != SET)
  {
    if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    if (BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    ButtonInitialized = SET;
  }
  
  /* Init Joystick in interrupt mode */
  if (JoyInitialized != SET)
  {
    if (BSP_JOY_Init(JOY1, JOY_MODE_EXTI, JOY_ALL) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
    JoyInitialized = SET;
  }

  /* Initialize the LCD */
  if (LcdInitialized != SET)
  {    
    /* Initialize the LCD */
    if (BSP_LCD_Init(0, Screen_Oientation) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }

    /* Set GUI functions */
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

  /* Initialize the TouchScreen */
  if (TsInitialized != SET)
  {
    TS_Init_t TsInit;

    /* Initialize the TouchScreen */
    TsInit.Width       = 320;
    TsInit.Height      = 240;
    TsInit.Orientation = Screen_Oientation;
    TsInit.Accuracy    = 10;
    if (BSP_TS_Init(0, &TsInit) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }

    /* Configure TS interrupt */
    if (BSP_TS_EnableIT(0) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }

    TsInitialized = SET;
  }
}

/**
  * @brief  Display main demo messages
  * @param  None
  * @retval None
  */
static void Display_DemoDescription(void)
{
  char desc[60];

  /* Set font */
  UTIL_LCD_SetFont(&Font24);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  
  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKBLUE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  
  /* Display LCD messages */
  UTIL_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32U575I-EV BSP", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 35, (uint8_t *)"drivers example", CENTER_MODE);
 
  /* Draw Bitmap */
  UTIL_LCD_DrawBitmap(120, 65, (uint8_t *)stlogo);
  
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 220, (uint8_t *)"Copyright (c) STMicroelectronics 2020", CENTER_MODE);
  
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_FillRect(0, 150, 320, 50, UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DisplayStringAt(0, 150, (uint8_t *)"Press USER push-button", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 165, (uint8_t *)"to start:", CENTER_MODE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  sprintf(desc,"%s example", BSP_examples[DemoIndex].DemoName);
  UTIL_LCD_DisplayStringAt(0, 180, (uint8_t *)desc, CENTER_MODE);
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

/**
  * @brief  Enable ICACHE and DCACHE.
  * @param  None
  * @retval None
  */
static void CACHE_Enable(void)
{
  DCACHE_HandleTypeDef hdcache;

  /* Configure ICACHE associativity mode */
  HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY);

  /* Enable ICACHE */
  HAL_ICACHE_Enable();

  /* Enable DCACHE clock */
  __HAL_RCC_DCACHE1_CLK_ENABLE();

  /* Enable DCACHE */
  hdcache.Instance = DCACHE1;
  hdcache.Init.ReadBurstType = DCACHE_READ_BURST_INCR;
  HAL_DCACHE_Enable(&hdcache);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while(1)
  {
    /* Toggle RED LED */
    (void) BSP_LED_Toggle(LED_RED);
    HAL_Delay(500);
  }
}

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  if (Button == BUTTON_USER)
  {
    UserButtonPressed = SET;
  }
}

/**
  * @brief  BSP Joystick callback
  * @param  JOY Joystick instance
  * @param  JoyPin Specifies the pin connected EXTI line
  * @retval None.
  */
void BSP_JOY_Callback(JOY_TypeDef JOY, uint32_t JoyPin)
{
  if (JOY == JOY1)
  {
    JoyPinPressed = (JOYPin_TypeDef )JoyPin;
  }
}

/**
  * @brief  TS Callback.
  * @param  Instance TS Instance.
  * @retval None.
  */
void BSP_TS_Callback(uint32_t Instance)
{  
  if (Instance == 0)
  {
    TouchDetected = SET;
  }
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */
