/**
  ******************************************************************************
  * @file    Examples/BSP/Src/main.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the STM32U599J_DK BSP Drivers
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "back.h"

/** @addtogroup STM32U5xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
TS_State_t TS_State = {0};
TS_Init_t TsInit;
/* Private defines -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO FlagStatus UserButtonPressed   = RESET;
uint8_t DrawBackround = 0;
__IO uint16_t x = 0;
__IO uint16_t y = 0;
uint16_t demo_xpos;
uint16_t demo_ypos;
uint32_t brightness;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Display_DemoDescription(void);
static void CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* This project template calls CACHE_Enable() in order to enable the Instruction
  and Data Cache. This function is provided as template implementation that
  User may integrate in his application in order to enhance performance */

  /* Enable the Instruction Cache */
  CACHE_Enable();

  /* Initialize red led */
  BSP_LED_Init(LED_RED);

  /* LCD and TouchScreen initialization */
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE);

  /* Initialize the TouchScreen */
  TsInit.Width       = 800;
  TsInit.Height      = 480;
  TsInit.Orientation = TS_SWAP_NONE;
  TsInit.Accuracy    = 0;
  BSP_TS_Init(0, &TsInit);

  /* Enable TS interrupt */
  BSP_TS_EnableIT(0);

  /* Set UTIL_LCD functions */
  UTIL_LCD_SetFuncDriver(&LCD_Driver);

  Display_DemoDescription();

  /* Get LCD default brightness (50%) */
  (void) BSP_LCD_GetBrightness(0,&brightness);

  /* Set LCD brightness to 100% */
  (void) BSP_LCD_SetBrightness(0, 100);

  /* Background status */
  DrawBackround = 1;

  while (1)
  {
    /* Re-draw the background after return from each module */
    if(DrawBackround == 0)
    {
      DrawBackround = 1;
      Display_DemoDescription();

    }
    if(TS_State.TouchDetected)
    {
      demo_xpos = x;
      demo_ypos = y;

      /* Initialize touch coordinates */
      TS_State.TouchDetected = 0;

      /* Launch Lcd demo */
      if ((demo_xpos > 500) & (demo_xpos < 800) & (demo_ypos > 100) & (demo_ypos < 240))
      {
        if (Lcd_demo() != 0)
        {
          Error_Handler();
        }
        demo_xpos = 0;
        demo_ypos = 0;
      }
      /* Launch Led demo */
      else if ((demo_xpos > 0) & (demo_xpos < 300) & (demo_ypos > 58) & (demo_ypos < 240))
      {
        if (Led_demo() != 0)
        {
          Error_Handler();
        }
        else if(Button_demo()!=0)
        {
          Error_Handler();
        }
        demo_xpos = 0;
        demo_ypos = 0;
      }
      /* Launch HSPI demo */
      else if ((demo_xpos > 70) & (demo_xpos < 359) & (demo_ypos > 240) & (demo_ypos < 480))
      {
        if (HSPI_NOR_demo() != 0)
        {
          Error_Handler();
        }
        demo_xpos = 0;
        demo_ypos = 0;
      }
      /* Launch TouchScreen demo */
      else if ((demo_xpos > 441) & (demo_xpos < 800) & (demo_ypos > 240) & (demo_ypos < 480))
      {
        if (Touchscreen_demo() != 0)
        {
          Error_Handler();
        }
        demo_xpos = 0;
        demo_ypos = 0;
      }
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

  /* Switch to SMPS regulator instead of LDO */
  HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY);

  __HAL_RCC_PWR_CLK_DISABLE();

  /* MSI Oscillator enabled at reset (4Mhz), activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
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
    Error_Handler();
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
  /* Clear LCD */
  UTIL_LCD_Clear(LCD_COLOR_BLACK);

  /* Clear the LCD */
  BSP_LCD_FillRect(0, 0, 0, 400, 240, LCD_COLOR_ARGB8888_ST_BLUE_DARK);
  BSP_LCD_FillRect(0, 0, 240, 400, 240, LCD_COLOR_ARGB8888_ST_BLUE);
  BSP_LCD_FillRect(0, 400, 0, 400, 240, LCD_COLOR_ARGB8888_ST_PINK);
  BSP_LCD_FillRect(0, 400, 240, 400, 240, LCD_COLOR_ARGB8888_ST_YELLOW);

  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(LCD_COLOR_ARGB8888_ST_BLUE_DARK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_DisplayStringAt(80, 120, (uint8_t *)"LED and BUTTON", LEFT_MODE);
  UTIL_LCD_SetBackColor(LCD_COLOR_ARGB8888_ST_PINK);
  UTIL_LCD_DisplayStringAt(180, 120, (uint8_t *)"LCD", RIGHT_MODE);
  UTIL_LCD_SetBackColor(LCD_COLOR_ARGB8888_ST_BLUE);
  UTIL_LCD_DisplayStringAt(160, 360, (uint8_t *)"XSPI", LEFT_MODE);
  UTIL_LCD_SetBackColor(LCD_COLOR_ARGB8888_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(100, 360, (uint8_t *)"TouchScreen", RIGHT_MODE);
}

/**
  * @brief  Enable ICACHE with 2-ways set-associative configuration.
  * @param  None
  * @retval None
  */
static void CACHE_Enable(void)
{
  /* Enable instruction cache (default 2-ways set associative cache) */
  HAL_ICACHE_Enable();
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
    BSP_LED_Toggle(LED_RED);
    HAL_Delay(100);
  }
}

/**
  * @brief EXTI line detection callbacks.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == TS_INT_PIN)
  {
    /* Communication with TS is done via I2C.
    Often the sw requires ISRs (interrupt service routines) to be quick while communication
    with I2C can be considered relatively long (depending on SW requirements).
    Considering that the TS feature don't need immediate reaction,
	it is suggested to use polling mode instead of EXTI mode,
    in order to avoid blocking I2C communication on interrupt service routines */

    /* Here an example of implementation is proposed which is a mix between pooling and exit mode:
    On ISR a flag is set (exti5_received), the main loop polls on the flag rather then polling the TS;
    Mcu communicates with TS only when the flag has been set by ISR. This is just an example:
    the users should choose they strategy depending on their application needs.*/
    TS_State.TouchDetected = 0;

    if (BSP_TS_GetState(0, &TS_State) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }

    if(TS_State.TouchDetected)
    {
      /* Get X and Y position of the touch post calibrated */
      x = TS_State.TouchX;
      y = TS_State.TouchY;
    }

    /* Initialize touch coordinates */
    TS_State.TouchX = 0;
    TS_State.TouchY = 0;
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

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */


/**
  * @}
  */

/**
  * @}
  */
