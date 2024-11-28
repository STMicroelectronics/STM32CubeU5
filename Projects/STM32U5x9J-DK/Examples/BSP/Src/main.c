/**
  ******************************************************************************
  * @file    Examples/BSP/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
#include "hexagone.h"
#include "back.h"

/** @addtogroup STM32U5xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

__IO uint32_t WakeupButtonPressed = 0;
__IO uint32_t TamperButtonPressed = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if (defined (__GNUC__) && !defined(__ARMCC_VERSION))
extern void initialise_monitor_handles(void);
#endif
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t DemoIndex = 0;
__IO FlagStatus UserButtonPressed   = RESET;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Flush_scanf(void);
static void LCD_Init(void);

uint8_t DrawBackround = 0;
uint8_t AutoMode = 0;
TS_State_t TS_State;
TS_Init_t TsInit;
LCD_UTILS_Drv_t pDrv;
__IO uint16_t x = 0;
__IO uint16_t y = 0;
uint16_t demo_xpos;
uint16_t demo_ypos;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
#if (defined (__GNUC__) && !defined(__ARMCC_VERSION))
  initialise_monitor_handles();
  printf("Semihosting Test...\n\r");
#endif

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure user button */
  if (BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  BSP_LED_Init(LED4);

  /* LCD and TouchScreen initialization */
  LCD_Init();

  /* Background status */
  DrawBackround = 1;

  while (1)
  {
    /* Re-draw the background after return from each module */
    if(DrawBackround == 0)
    {
      BSP_LCD_DrawBitmap(0, 0, 0, (uint8_t *)hexagone);

      DrawBackround = 1;
    }
    if(TS_State.TouchDetected)
    {
      /* Get X and Y position of the touch post calibrated */
      demo_xpos = x;
      demo_ypos = y;

      /* Initialize touch coordinates */
      TS_State.TouchDetected = 0;

      /* Launch Led demo */
      if ((demo_xpos > 295) & (demo_xpos < 435) & (demo_ypos > 120) & (demo_ypos < 260))
      {
        demo_xpos = 0;
        demo_ypos = 0;
        if (Led_demo() != 0)
        {
          Error_Handler();
        }
      }
      /* Launch OSPI demo */
      if ((demo_xpos > 32) & (demo_xpos < 175) & (demo_ypos > 0) & (demo_ypos < 120))
      {
        demo_xpos = 0;
        demo_ypos = 0;
        if (OSPI_NOR_demo() != 0)
        {
          Error_Handler();
        }
      }
      /* Launch ToF demo */
      if ((demo_xpos > 32) & (demo_xpos < 175) & (demo_ypos > 120) & (demo_ypos < 260))
      {
        demo_xpos = 0;
        demo_ypos = 0;
        if (Rs_demo() != 0)
        {
          Error_Handler();
        }
      }
      /* Launch eMMC demo */
      if ((demo_xpos > 175) & (demo_xpos < 295) & (demo_ypos > 50) & (demo_ypos < 190))
      {
        demo_xpos = 0;
        demo_ypos = 0;
        if (eMMC_demo() != 0)
        {
          Error_Handler();
        }
      }
      /* Launch HSPI demo */
      if ((demo_xpos > 295) & (demo_xpos < 435) & (demo_ypos > 0) & (demo_ypos < 120))
      {
        demo_xpos = 0;
        demo_ypos = 0;
        if (OSPI_RAM_demo() != 0)
        {
          Error_Handler();
        }
      }
      /* Launch TouchScreen demo */
      if ((demo_xpos > 155) & (demo_xpos < 295) & (demo_ypos > 190) & (demo_ypos < 335))
      {
        demo_xpos = 0;
        demo_ypos = 0;
        if (Touchscreen_demo() != 0)
        {
          Error_Handler();
        }
      }
      /* Launch Lcd demo */
      if ((demo_xpos > 155) & (demo_xpos < 295) & (demo_ypos > 355) & (demo_ypos < 480))
      {
        demo_xpos = 0;
        demo_ypos = 0;
        if (Lcd_demo() != 0)
        {
          Error_Handler();
        }
      }

      /* Launch ES demo */
      if ((demo_xpos > 32) & (demo_xpos < 175) & (demo_ypos > 260) & (demo_ypos < 400))
      {
        demo_xpos = 0;
        demo_ypos = 0;
        if (Es_demo() != 0)
        {
          Error_Handler();
        }
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
  * @brief  LCD and TouchScreen initialization
  * @param  None
  * @retval None
  */
void LCD_Init(void)
{
  BSP_LCD_Init(0, LCD_ORIENTATION_PORTRAIT);

  BSP_LCD_SetBrightness(0, 100);
  BSP_LCD_SetActiveLayer(0, 0);
  BSP_LCD_DisplayOn(0);

  /* Clear the LCD */
  BSP_LCD_FillRect(0, 0, 0, 480, 480, LCD_COLOR_WHITE);

  BSP_LCD_DrawBitmap(0, 0, 0, (uint8_t *)hexagone);

  /* Initialize the TouchScreen */
  TsInit.Width       = 480;
  TsInit.Height      = 480;
  TsInit.Orientation = TS_ORIENTATION_PORTRAIT;
  TsInit.Accuracy    = 0;
  BSP_TS_Init(0, &TsInit);

  pDrv.DrawBitmap  = BSP_LCD_DrawBitmap;
  pDrv.FillRGBRect = BSP_LCD_FillRGBRect;
  pDrv.DrawHLine   = BSP_LCD_DrawHLine;
  pDrv.DrawVLine   = BSP_LCD_DrawVLine;
  pDrv.FillRect    = BSP_LCD_FillRect;
  pDrv.GetPixel    = BSP_LCD_ReadPixel;
  pDrv.SetPixel    = BSP_LCD_WritePixel;
  pDrv.GetXSize    = BSP_LCD_GetXSize;
  pDrv.GetYSize    = BSP_LCD_GetYSize;
  pDrv.SetLayer    = BSP_LCD_SetActiveLayer;
  pDrv.GetFormat   = BSP_LCD_GetFormat;

  /* Set UTIL_LCD functions */
  UTIL_LCD_SetFuncDriver(&pDrv);

  /* Enable TS interrupt */
  BSP_TS_EnableIT(0);
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
  * @brief  Ask user for result.
  * @param  None
  * @retval None
  */
uint32_t CheckResult(void)
{
  uint32_t result = 0;
  uint8_t tmp = 0;

  printf("If result is OK press p (PASS), otherwise press other Key (FAIL) \n");
  scanf("%c", &tmp);
  if (tmp != 'p')
  {
    Flush_scanf();
    printf("Test is FAIL!!!\n");
    result = 1;
  }
  else
  {
    Flush_scanf();
    printf("Test is PASS\n");
  }
  return result;
}

/**
  * @brief  Ask user to start the test.
  * @param  None
  * @retval None
  */
void StartTest(void)
{
  uint8_t tmp = 0;
  do {
    printf("Press s to start the test\n");
    scanf("%c", &tmp);
    Flush_scanf();
  } while (tmp != 's');
}

/**
  * @brief EXTI line detection callbacks.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
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
  }

  TS_State.TouchX = 0;
  TS_State.TouchY = 0;


  if (GPIO_Pin == VL53L5A1_INT_PIN)
  {
    if (BSP_TS_GetState(0, &TS_State) != BSP_ERROR_NONE)
    {
      Error_Handler();
    }
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
  * @brief  Flush scanf buffer until "\n".
  * @param  None
  * @retval None
  */
static void Flush_scanf(void)
{
  while ( getchar() != '\n' );
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


