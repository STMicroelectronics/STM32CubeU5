/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_BlendingWithAlphaInversion/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure
  *          DMA2D peripheral in Memory to Memory with Blending transfer mode and
  *          alpha inversion
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
#include "RGB565_240x130_1.h"
#include "RGB565_240x130_2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

DMA2D_HandleTypeDef hdma2d;

/* USER CODE BEGIN PV */
/* Blended image Ready flag */
static __IO uint32_t   blended_image_ready = 0;
static uint32_t LCD_X_Size = 0, LCD_Y_Size = 0;
static uint32_t xoffset = 0,yoffset = 0;
/*DMA2D Alpha inversion  config*/
static uint32_t AlphaInvert_Config = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
static void MX_DMA2D_Init(void);
/* USER CODE BEGIN PFP */
static void TransferError(DMA2D_HandleTypeDef* hdma2d);
static void TransferComplete(DMA2D_HandleTypeDef* hdma2d);
static void DMA2D_AlphaInvertedConfig(void);
static void MPU_Config(void);
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
  /* STM32U5xx HAL library initialization:
       - Configure the Flash prefetch
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 3
       - Low Level Initialization
     */
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

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */
 /*##-1- Initialize Leds and the LCD #################################################*/
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  if(BSP_LCD_Init(0,LCD_ORIENTATION_LANDSCAPE) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  UTIL_LCD_SetFuncDriver(&LCD_Driver);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetFont(&Font8);

  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKBLUE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);

  /* Set the display on */
  if (BSP_LCD_DisplayOn(0) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* Get the LCD width and height */
  BSP_LCD_GetXSize(0, &LCD_X_Size);
  BSP_LCD_GetYSize(0, &LCD_Y_Size);

  /* Prepare using DMA2D the LCD frame buffer for display : LCD buffer clear and user message priniting*/
  UTIL_LCD_Clear(UTIL_LCD_COLOR_DARKGRAY);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font16);

  /* Print example description */
  UTIL_LCD_DisplayStringAt(0, 200, (uint8_t *)"ChromART Blending With Alpha Inversion example", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 220, (uint8_t *)"Inversion example", CENTER_MODE);
  HAL_Delay(100);

  /*Initialize Alpha Inversion setting*/
  AlphaInvert_Config = DMA2D_REGULAR_ALPHA;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
 while (1)
  {
    /* Turn LED5 Off */
    BSP_LED_Off(LED5);
    /* Reset blended_image_ready flag */
    blended_image_ready = 0;

    xoffset = (LCD_X_Size - IMAGE_SIZE_X) / 2 ;
    yoffset = (LCD_Y_Size - IMAGE_SIZE_Y) / 2 ;

    UTIL_LCD_DisplayStringAt(0, 5, (uint8_t *)"Blended Image: Foreground ", CENTER_MODE);
    /*##-2- Configure DMA2D : Configure foreground and background layers ##############*/
    if(AlphaInvert_Config == DMA2D_REGULAR_ALPHA)
    {
      MX_DMA2D_Init();
      UTIL_LCD_DisplayStringAt(0,22, (uint8_t *)"Alpha inversion OFF", CENTER_MODE);
    }
    else
    {
      DMA2D_AlphaInvertedConfig();
      UTIL_LCD_DisplayStringAt(0,22, (uint8_t *)"Alpha inversion ON ", CENTER_MODE);
    }


    /*##-3- Start DMA2D transfer in interrupt mode ################################################*/
    /*## RGB565_240x130_1[] is the foreground layer and RGB565_240x130_2[] is the background layer */
    if(HAL_DMA2D_BlendingStart_IT(&hdma2d,
                                  (uint32_t)&RGB565_240x130_1,
                                  (uint32_t)&RGB565_240x130_2,
                                  (uint32_t)(LCD_FRAME_BUFFER),
                                  IMAGE_SIZE_X,
                                  IMAGE_SIZE_Y) != HAL_OK)
    {
      Error_Handler();
    }

    /* Wait until blended image is ready to be displayed */
    while(blended_image_ready == 0) {;}

    /*Display the blended image*/
    BSP_LCD_FillRGBRect(0,xoffset,yoffset,(uint8_t *)(LCD_FRAME_BUFFER),IMAGE_SIZE_X,IMAGE_SIZE_Y);

    /* Update Alpha Inversion setting */
    AlphaInvert_Config = (AlphaInvert_Config == DMA2D_REGULAR_ALPHA) ? DMA2D_INVERTED_ALPHA : DMA2D_REGULAR_ALPHA;

    /* wait for 2 sec */
    HAL_Delay(2000);
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /* Switch to SMPS regulator instead of LDO */
  if(HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
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
  __HAL_RCC_PWR_CLK_DISABLE();
}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */
  /* DMA2D Callbacks Configuration */
  hdma2d.XferCpltCallback  = TransferComplete;
  hdma2d.XferErrorCallback = TransferError;
  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M_BLEND;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.Init.BytesSwap = DMA2D_BYTES_REGULAR;
  hdma2d.Init.LineOffsetMode = DMA2D_LOM_PIXELS;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0x32;
  hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;
  hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR;
  hdma2d.LayerCfg[0].InputOffset = 0;
  hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d.LayerCfg[0].AlphaMode = DMA2D_REPLACE_ALPHA;
  hdma2d.LayerCfg[0].InputAlpha = 0xFF;
  hdma2d.LayerCfg[0].AlphaInverted = DMA2D_REGULAR_ALPHA;
  hdma2d.LayerCfg[0].RedBlueSwap = DMA2D_RB_REGULAR;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 0) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */
  /* Apply DMA2D Foreground configuration */
  HAL_DMA2D_ConfigLayer(&hdma2d, 1);

  /* Apply DMA2D Background configuration */
  HAL_DMA2D_ConfigLayer(&hdma2d, 0);
  /* USER CODE END DMA2D_Init 2 */

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

/* USER CODE BEGIN 4 */
/**
  * @brief DMA2D configuration.
  * @param  None
  *         can be DMA2D_REGULAR_ALPHA or DMA2D_INVERTED_ALPHA
  * @note  This function Configure the DMA2D peripheral :
  *        1) Configure the Transfer mode as memory to memory with blending.
  *        2) Configure the output color mode as RGB565 pixel format.
  *        3) Configure the Foreground
  *          - Foreground image is loaded from FLASH memory (RGB565_240x130_2[])
  *          - constant alpha value (decreased to see the background)
  *          - Alpha inversion set to parameter DMA2D_INVERTED_ALPHA.
  *          - color mode as RGB565 pixel format
  *        4) Configure the Background
  *          - Background image loaded from FLASH memory (RGB565_240x130_1[])
  *          - Alpha inversion set to DMA2D_REGULAR_ALPHA (No Alpha inversion)
  *          - color mode as RGB565 pixel format
  * @retval
  *  None
  */
static void DMA2D_AlphaInvertedConfig(void)
{
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d.Init.Mode          = DMA2D_M2M_BLEND;             /* DMA2D mode Memory to Memory with Blending */
  hdma2d.Init.ColorMode     = DMA2D_OUTPUT_RGB565;         /* output format of DMA2D */
  hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;         /* No Output Alpha Inversion*/
  hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;            /* No Output Red & Blue swap */

  /* Output offset in pixels == nb of pixels to be added at end of line to come to the  */
  /* first pixel of the next line : on the output side of the DMA2D                     */
  hdma2d.Init.OutputOffset = 0;

  /* DMA2D Callbacks Configuration */
  hdma2d.XferCpltCallback  = TransferComplete;
  hdma2d.XferErrorCallback = TransferError;

  /* Foreground layer Configuration */
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0x32;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d.LayerCfg[1].InputOffset = 0x0;                 /* No offset in input */
  hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR;    /* No ForeGround Red a Blue swap */
  hdma2d.LayerCfg[1].AlphaInverted = DMA2D_INVERTED_ALPHA; /* ForeGround Alpha inversion setting */

  /* Background layer Configuration */
  hdma2d.LayerCfg[0].AlphaMode = DMA2D_REPLACE_ALPHA;
  hdma2d.LayerCfg[0].InputAlpha = 0xFF;                   /* fully opaque */
  hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d.LayerCfg[0].InputOffset = 0x0;                   /* No offset in input */
  hdma2d.LayerCfg[0].RedBlueSwap = DMA2D_RB_REGULAR;      /* No BackGround Red a Blue swap */
  hdma2d.LayerCfg[0].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No BackGround Alpha inversion */

  hdma2d.Instance = DMA2D;

  /* DMA2D Initialization */
  if(HAL_DMA2D_DeInit(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }

  if(HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }

  /* Apply DMA2D Foreground configuration */
  HAL_DMA2D_ConfigLayer(&hdma2d, 1);

  /* Apply DMA2D Background configuration */
  HAL_DMA2D_ConfigLayer(&hdma2d, 0);
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
  * @brief  DMA2D Transfer completed callback
  * @param  hdma2d: DMA2D handle.
  * @note   This example shows a simple way to report end of DMA2D transfer, and
  *         you can add your own implementation.
  * @retval None
  */
static void TransferComplete(DMA2D_HandleTypeDef *hdma2d)
{
  /* Turn LED5 On */
  BSP_LED_On(LED5);

  /* The Blended image is now ready for display */
  blended_image_ready = 1;
}

/**
  * @brief  DMA2D error callbacks
  * @param  hdma2d: DMA2D handle
  * @note   This example shows a simple way to report DMA2D transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
static void TransferError(DMA2D_HandleTypeDef *hdma2d)
{
  /* Turn LED6 On */
  while(1)
  {
    BSP_LED_Toggle(LED6);
    HAL_Delay(100);
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  while(1)
  {
    BSP_LED_On(LED6);
  } /* Blocking on error */
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
