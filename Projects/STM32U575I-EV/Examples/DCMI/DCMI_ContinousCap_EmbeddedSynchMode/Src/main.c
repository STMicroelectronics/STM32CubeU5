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

DCMI_HandleTypeDef hdcmi;

DMA_HandleTypeDef handle_GPDMA1_Channel12;

/* USER CODE BEGIN PV */
__IO uint32_t frame_suspended   = 0;
__IO uint32_t frame_captured    = 0;
uint32_t FRAME_BUFFER_SIZE = (FRAME_WIDTH*FRAME_HEIGHT*2)/4;
uint32_t CAMERA_FRAME_BUFFER[(FRAME_WIDTH*FRAME_HEIGHT*2)/4];
OV5640_SyncCodes_t pSyncroCodes;
DCMI_SyncUnmaskTypeDef SyncUnmask;

extern DMA_QListTypeDef  DCMIQueue;
__IO FlagStatus UserButtonPressed = RESET;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_ICACHE_Init(void);
static void MX_GPIO_Init(void);
static void MX_GPDMA1_Init(void);
static void MX_DCMI_Init(void);
/* USER CODE BEGIN PFP */
static void MPU_Config(void);
static void Example_Description(void);
static uint32_t OV5640_Config(void);
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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* Configure the MPU attributes for SRAM */
  MPU_Config();

    /*##-1- LEDs and User Button initialization  #################################################*/
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  /*##-2- LCD configuration  #################################################*/
  /* LCD initialization and display enable*/
  BSP_LCD_Init(0,LCD_ORIENTATION_LANDSCAPE);
  BSP_LCD_DisplayOn(0);

  Example_Description();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_ICACHE_Init();
  MX_GPIO_Init();
  MX_GPDMA1_Init();
  MX_DCMI_Init();
  /* USER CODE BEGIN 2 */
  MX_DCMIQueue_Config();
  HAL_DMAEx_List_LinkQ(&handle_GPDMA1_Channel12, &DCMIQueue);
  __HAL_LINKDMA(&hdcmi, DMA_Handle, handle_GPDMA1_Channel12);
  /*##-4- Camera Initialization ############################*/
  /* Initialize the Camera in QVGA mode */
  if(OV5640_Config() != 0)
  {
    Error_Handler();
  }

  /* Wait for User button press */
  while (UserButtonPressed != SET);

  /* Reset for next operation */
  UserButtonPressed = RESET;

   /*
     LCD size is 320 x 240 and format is RGB565 i.e. 16 bpp or 2 bytes/pixel.
     The LCD frame size is therefore 320 * 240 half-words of (320*240*2) 8-bit long bytes .
     Since the unit of the DMA associated to DCMI IP is with word , the last parameter of
     HAL_DCMI_Start_DMA is set to:
     FRAME_BUFFER_SIZE = ((FRAME_WIDTH*FRAME_HEIGHT)*2)/4, that is (320 * 240 * 2)/4
   */
  HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)CAMERA_FRAME_BUFFER, FRAME_BUFFER_SIZE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* Display the continuous grap */
    BSP_LCD_FillRGBRect(0,0,0,(uint8_t *)CAMERA_FRAME_BUFFER,FRAME_WIDTH,FRAME_HEIGHT);

    /* for any Press Check whether the Continuous capture should be suspended or resumed */
    while (UserButtonPressed != RESET)
    {
      if(frame_suspended == 1)
      {
        if(HAL_DCMI_Resume(&hdcmi) != HAL_OK)
        {
          Error_Handler();
        }
        frame_suspended=0;
        BSP_LED_Off(LED5);
      }
      else
      {
        if(HAL_DCMI_Suspend(&hdcmi) != HAL_OK)
        {
          Error_Handler();
        }
        frame_suspended++;
        BSP_LED_On(LED5);
      }
      UserButtonPressed = RESET;
    }
    /* Check each time for new frame*/
    if(frame_captured != 0)
    {
      frame_captured = 0;
      BSP_LED_Toggle(LED5);
    }
    else
    {
      BSP_LED_Off(LED5);
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
  * @brief DCMI Initialization Function
  * @param None
  * @retval None
  */
static void MX_DCMI_Init(void)
{

  /* USER CODE BEGIN DCMI_Init 0 */

  /* USER CODE END DCMI_Init 0 */

  /* USER CODE BEGIN DCMI_Init 1 */

  /* USER CODE END DCMI_Init 1 */
  hdcmi.Instance = DCMI;
  hdcmi.Init.SynchroMode = DCMI_SYNCHRO_EMBEDDED;
  hdcmi.Init.PCKPolarity = DCMI_PCKPOLARITY_RISING;
  hdcmi.Init.CaptureRate = DCMI_CR_ALL_FRAME;
  hdcmi.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
  hdcmi.Init.SyncroCode.FrameEndCode = 0xF3;
  hdcmi.Init.SyncroCode.FrameStartCode = 0xF4;
  hdcmi.Init.SyncroCode.LineStartCode = 0xF2;
  hdcmi.Init.SyncroCode.LineEndCode = 0xF1;
  hdcmi.Init.JPEGMode = DCMI_JPEG_DISABLE;
  hdcmi.Init.ByteSelectMode = DCMI_BSM_ALL;
  hdcmi.Init.ByteSelectStart = DCMI_OEBS_ODD;
  hdcmi.Init.LineSelectMode = DCMI_LSM_ALL;
  hdcmi.Init.LineSelectStart = DCMI_OELS_ODD;
  if (HAL_DCMI_Init(&hdcmi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DCMI_Init 2 */
  SyncUnmask.FrameEndUnmask    = 0xFF;
  SyncUnmask.FrameStartUnmask  = 0xFF;
  SyncUnmask.LineEndUnmask     = 0xFF;
  SyncUnmask.LineStartUnmask   = 0xFF;
  if(HAL_DCMI_ConfigSyncUnmask(&hdcmi,&SyncUnmask) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE END DCMI_Init 2 */

}

/**
  * @brief GPDMA1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPDMA1_Init(void)
{

  /* USER CODE BEGIN GPDMA1_Init 0 */

  /* USER CODE END GPDMA1_Init 0 */

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel12_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel12_IRQn);

  /* USER CODE BEGIN GPDMA1_Init 1 */

  /* USER CODE END GPDMA1_Init 1 */
  handle_GPDMA1_Channel12.Instance = GPDMA1_Channel12;
  handle_GPDMA1_Channel12.InitLinkedList.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
  handle_GPDMA1_Channel12.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
  handle_GPDMA1_Channel12.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  handle_GPDMA1_Channel12.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  handle_GPDMA1_Channel12.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;
  if (HAL_DMAEx_List_Init(&handle_GPDMA1_Channel12) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN GPDMA1_Init 2 */

  /* USER CODE END GPDMA1_Init 2 */

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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI13_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI13_IRQn);

}

/* USER CODE BEGIN 4 */
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
  * @brief  Camera Frame Event callback.
  */
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  frame_captured++;
}

/**
  * @brief Configure the Camera Module in continuous
  *        in Embedded Synchronization mode :
  *        Use embedded codes (SynchroCodes)
  *        for Synchronization the same
  *        set to the DCMI IP
  * @param  None
  * @retval 0   OK
  *         !=0 KO
  */
static uint32_t OV5640_Config(void)
{
  OV5640_IO_t              IOCtx;
  uint32_t                 id;
  uint32_t                 status = 0;
  static OV5640_Object_t   OV5640Obj;
  GPIO_InitTypeDef  gpio_init_structure;

  /* Configure the Camera driver */
  IOCtx.Address     = CAMERA_OV5640_ADDRESS;
  IOCtx.Init        = BSP_I2C2_Init;
  IOCtx.DeInit      = BSP_I2C2_DeInit;
  IOCtx.ReadReg     = BSP_I2C2_ReadReg16;
  IOCtx.WriteReg    = BSP_I2C2_WriteReg16;
  IOCtx.GetTick     = BSP_GetTick;

  /* Reset sensor */
  gpio_init_structure.Pin  = RSTI_PIN | XSDN_PIN;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Alternate  = GPIO_AF10_DCMI;
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);

  /* Camera sensor RESET sequence */
  /* Assert the camera STANDBY pin (active high)  */
  HAL_GPIO_WritePin(XSDN_PORT, XSDN_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(RSTI_PORT, RSTI_PIN, GPIO_PIN_RESET);
  HAL_Delay(100);   /* RST and XSDN signals asserted during 100ms */

  /* De-assert the camera STANDBY pin (active high) */
  HAL_GPIO_WritePin(XSDN_PORT, XSDN_PIN, GPIO_PIN_RESET);
  HAL_Delay(3);     /* RST de-asserted and XSDN asserted during 3ms */

  /* De-assert the camera RSTI pin (active low) */
  HAL_GPIO_WritePin(RSTI_PORT, RSTI_PIN, GPIO_PIN_SET);
  HAL_Delay(20);     /* RST de-asserted during 20ms */

  /* Verify that the Camera module is plugged in */
  gpio_init_structure.Pin       = CAM_PLUG_PIN;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Mode      = GPIO_MODE_INPUT;
  HAL_GPIO_Init(CAM_PLUG_PORT, &gpio_init_structure);

  if(((uint32_t)HAL_GPIO_ReadPin(CAM_PLUG_PORT, CAM_PLUG_PIN) & CAM_PLUG_PIN) == CAM_PLUG_PIN)
  {
    status = 1;
  }
  /**/
  if(OV5640_RegisterBusIO (&OV5640Obj, &IOCtx) != OV5640_OK)
  {
    status = OV5640_ERROR;
  }
  /*Read ID*/
  if(OV5640_ReadID(&OV5640Obj, &id) != OV5640_OK)
  {
    status = OV5640_ERROR;
  }
  if(id == OV5640_ID)
  {
    /*Initialize*/
    if(OV5640_Init(&OV5640Obj, CAMERA_R320x240, CAMERA_PF_RGB565) != OV5640_OK)
    {
      status = OV5640_ERROR;
    }

    /* Enable and set SynchroCodes CCIR to the camera Module */
    pSyncroCodes.FrameStartCode = 0xF4;
    pSyncroCodes.FrameEndCode   = 0xF3;
    pSyncroCodes.LineStartCode  = 0xF2;
    pSyncroCodes.LineEndCode    = 0xF1;

    if(OV5640_EmbeddedSynchroConfig(&OV5640Obj,&pSyncroCodes) !=OV5640_OK)
    {
      status = OV5640_ERROR;
    }

    HAL_Delay(100);
    /* Disable Flip and mirror effect */
    if(OV5640_MirrorFlipConfig(&OV5640Obj, OV5640_MIRROR_FLIP_NONE)!= OV5640_OK)
    {
      status = OV5640_ERROR;
    }

    HAL_Delay(100);
  }
  else
  {
    status = OV5640_ERROR;
  }

  return status;
}

/**
  * @brief  Display main example messages
  * @param  None
  * @retval None
  */
static void Example_Description(void)
{
  /* Set GUI functions */
  UTIL_LCD_SetFuncDriver(&LCD_Driver);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set font */
  UTIL_LCD_SetFont(&Font16);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_DARKBLUE);

  /* Display LCD messages */
  UTIL_LCD_DisplayStringAt(0, 5, (uint8_t *)" Continuous Capture in ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 20, (uint8_t *)"Embedded Synchronization Mode", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 35, (uint8_t *)" and Suspend-Resume ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 50, (uint8_t *)" example ", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_FillRect(0, 150, 320, 50, UTIL_LCD_COLOR_DARKBLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_DARKBLUE);
  UTIL_LCD_DisplayStringAt(0, 150, (uint8_t *)"Press USER push-button", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 165, (uint8_t *)"to start:", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 180, (uint8_t *)"then to Suspend/Resume ", CENTER_MODE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ORANGE);
}

/**
  * @brief  EXTI line rising detection callback.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  UserButtonPressed = SET;
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
    HAL_Delay(100);
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
  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
