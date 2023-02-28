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
#define DATA_SIZE              ((uint32_t)0x01800000U) /* Data Size 24MB */

#define BUFFER_SIZE            ((uint32_t)0x00000400U) /* 1KB */
#define BUFFER_WORD_SIZE       (BUFFER_SIZE>>2)        /* Buffer size in Word */
#define NB_BLOCK_BUFFER        BUFFER_SIZE / BLOCKSIZE /* Number of Block (512B) by Buffer */
#define BLOCK_WORD_SIZE        (BLOCKSIZE>>2) /* Block size in words */


#define DATA_ADDRESS           ((uint32_t)0x00000000U) /* SD Card Address to write/read data */
#define DATA_PATTERN0          ((uint32_t)0x01000000U) /* Data pattern for buffer0*/
#define DATA_PATTERN1          ((uint32_t)0x02000000U) /* Data pattern for buffer1 */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SD_HandleTypeDef hsd1;

/* USER CODE BEGIN PV */
uint32_t Buffer0[BUFFER_WORD_SIZE];
uint32_t Buffer1[BUFFER_WORD_SIZE];


static SDMMC_DMALinkNodeTypeDef pLinkNode[4];
static SDMMC_DMALinkedListTypeDef Read_LinkedList;
static SDMMC_DMALinkedListTypeDef Write_LinkedList;
SD_DMALinkNodeConfTypeDef LinkNodeConf;
__IO uint8_t TxLnkLstBufCplt, RxLnkLstBufCplt,TxNodeBufCplt,RxNodeBufCplt;

uint32_t Node,RBuff0,RBuff1,WBuff0,WBuff1;
uint32_t ReadError = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
static void MX_SDMMC1_SD_Init(void);
/* USER CODE BEGIN PFP */
static void Fill_Buffer(uint32_t *pBuffer, uint16_t BufferLength, uint32_t Offset);
static HAL_StatusTypeDef Buffercmp(uint32_t* pBuffer, uint32_t BufferLength, uint32_t Offset);
void Dynamic_buffer_fill(void);
void Dynamic_buffer_check(void);
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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the System Power */
  SystemPower_Config();

  /* USER CODE BEGIN SysInit */
  /* Add your application code here
     */
 /*##-1- Initialize LEDs mounted on ST32U575I-EV board #####################*/
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_RED);

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ICACHE_Init();
  MX_SDMMC1_SD_Init();
  /* USER CODE BEGIN 2 */
  /* Try to switch to High Speed Mode , if supported by the card */
  HAL_SD_ConfigSpeedBusOperation(&hsd1,SDMMC_SPEED_MODE_HIGH);

  if(HAL_SD_Erase(&hsd1,DATA_ADDRESS,DATA_SIZE/BLOCKSIZE) != HAL_OK)
  {
    Error_Handler();
  }
  /* Wait for the Card Transfer status */
  while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
  {
  }
  /* Fill the buffer to send */
  Fill_Buffer(Buffer0, BUFFER_WORD_SIZE, DATA_PATTERN0);
  Fill_Buffer(Buffer1, BUFFER_WORD_SIZE, DATA_PATTERN1);
  /*  Write steps */
  /*Node 0 preparation */
  LinkNodeConf.BufferAddress = (uint32_t) Buffer0;
  LinkNodeConf.BufferSize = BUFFER_SIZE;
  HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[0], &LinkNodeConf );
  HAL_SDEx_DMALinkedList_InsertNode(&Write_LinkedList, NULL, &pLinkNode[0] );

  /*Node 1 preparation */
  LinkNodeConf.BufferAddress = (uint32_t) Buffer1;
  LinkNodeConf.BufferSize = BUFFER_SIZE;
  HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[1], &LinkNodeConf );
  HAL_SDEx_DMALinkedList_InsertNode(&Write_LinkedList ,&pLinkNode[0], &pLinkNode[1] );


  /*Enable Circular Linked list mode */
  HAL_SDEx_DMALinkedList_EnableCircularMode(&Write_LinkedList );

  TxLnkLstBufCplt=0;
  TxNodeBufCplt=0;
  BSP_LED_On(LED_BLUE);
  /* Write 24MB  on uSD card using DMA transfer */

  HAL_SDEx_DMALinkedList_WriteBlocks(&hsd1,&Write_LinkedList, DATA_ADDRESS, DATA_SIZE/BLOCKSIZE);

  /* Dynamic buffer filling Process*/
  Dynamic_buffer_fill();

  /* Remove write nodes*/
  HAL_SDEx_DMALinkedList_RemoveNode(&Write_LinkedList,&pLinkNode[1] );
  HAL_SDEx_DMALinkedList_RemoveNode(&Write_LinkedList,&pLinkNode[0] );

  /* Wait for the Card Transfer status */
  while(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
  {
  }

  /*  Read steps */
  /*Node 0 preparation */
  LinkNodeConf.BufferAddress = (uint32_t) Buffer0;
  LinkNodeConf.BufferSize = BUFFER_SIZE;
  HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[0], &LinkNodeConf );
  HAL_SDEx_DMALinkedList_InsertNode(&Read_LinkedList, NULL, &pLinkNode[0] );

  /*Node 1 preparation */
  LinkNodeConf.BufferAddress = (uint32_t) Buffer1;
  LinkNodeConf.BufferSize = BUFFER_SIZE;
  HAL_SDEx_DMALinkedList_BuildNode(&pLinkNode[1], &LinkNodeConf );
  HAL_SDEx_DMALinkedList_InsertNode(&Read_LinkedList, &pLinkNode[0], &pLinkNode[1]);

  /*Enable Circular Linked list mode */
  HAL_SDEx_DMALinkedList_EnableCircularMode(&Read_LinkedList );

  /* Read 24MB  from uSD card using DMA transfer */

  HAL_SDEx_DMALinkedList_ReadBlocks(&hsd1,&Read_LinkedList,DATA_ADDRESS,DATA_SIZE/BLOCKSIZE);

  /* Dynamic buffer Check Process*/
  Dynamic_buffer_check();

  BSP_LED_Off(LED_BLUE);

  /* Check the correctness of written dada */

  if (ReadError == 0)
  {

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      BSP_LED_Toggle(LED_GREEN);
      HAL_Delay(200);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  }
  else
  {
    while(1)
    {
      BSP_LED_Toggle(LED_RED);
      HAL_Delay(200);
    }
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

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
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

  /** Initializes the CPU, AHB and APB buses clocks
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
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_FALLING;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 1;
  if (HAL_SD_Init(&hsd1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief Dynamic buffer fill
  * @param None
  * @retval None
  */
void Dynamic_buffer_fill(void)
{

  while ((WBuff0+WBuff1) <= (DATA_SIZE/BUFFER_SIZE))
  {
    /* Wait for Node transfer completed or the whole transfer completed */
    while ((TxNodeBufCplt == 0) && (TxLnkLstBufCplt ==0));

    TxNodeBufCplt = 0;

    if(Node == 0)
    {
      WBuff0++;
      /*Start processing of Node0 buffer, we should lock the node*/
      HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[0]);

      Fill_Buffer(Buffer0, BUFFERSIZE, DATA_PATTERN0 + (WBuff0 * (uint32_t)0x00010000));

      /*Finish processing of Node1 buffer, we can unlock the node*/
      HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[0]);

      /*Set Next Node*/
      Node = 1;
    }
    else if(Node == 1)
    {
      WBuff1++;
      /*Start processing of Node1 buffer, we should lock the node*/
      HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[1]);

      Fill_Buffer(Buffer1, BUFFERSIZE, DATA_PATTERN1 + (WBuff1 * (uint32_t)0x00010000));

      /*Finish processing of Node1 buffer, we can unlock the node*/
      HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[1]);

      /*Set Next Node*/
      Node = 0;
    }
  }
  TxLnkLstBufCplt = 0;
  Node = 0;
}



/**
  * @brief Dynamic buffer check
  * @param None
  * @retval None
  */
void Dynamic_buffer_check(void)
{

  while ((RBuff0+RBuff1) < (DATA_SIZE/BUFFER_SIZE))
  {
    /* Wait for Node transfer completed or the whole transfer completed */
    while ((RxNodeBufCplt == 0) && (RxLnkLstBufCplt ==0));

    RxNodeBufCplt = 0;

    if(Node == 0)
    {
      /*Start processing of Node0 buffer, we should lock the node*/
      HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[0]);

      ReadError += Buffercmp(Buffer0, BUFFERSIZE, DATA_PATTERN0 + (RBuff0 * (uint32_t)0x00010000));

      /*Finish processing of Node0 buffer, we can unlock the node*/
      HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[0]);

      RBuff0++;
      /*Next Node*/
      Node = 1;
    }
    else if(Node == 1)
    {
      /*Start processing of Node0 buffer, we should lock the node*/
      HAL_SDEx_DMALinkedList_LockNode(&pLinkNode[1]);

      ReadError += Buffercmp(Buffer1, BUFFERSIZE, DATA_PATTERN1 + (RBuff1 * (uint32_t)0x00010000));

      /*Finish processing of Node1 buffer, we can unlock the node*/
      HAL_SDEx_DMALinkedList_UnlockNode(&pLinkNode[1]);

      RBuff1++;
      /*Next Node*/
      Node = 0;
    }
  }
  RxLnkLstBufCplt = 0;
  Node = 0;
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SDEx_DMALinkedList_ReadCpltCallback(SD_HandleTypeDef *hsd)
{
  RxLnkLstBufCplt = 1;
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SDEx_DMALinkedList_WriteCpltCallback(SD_HandleTypeDef *hsd)
{
  TxLnkLstBufCplt = 1;
}

/**
  * @brief Read DMA Linked list node Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SDEx_Read_DMALnkLstBufCpltCallback(SD_HandleTypeDef *hsd)
{
  RxNodeBufCplt = 1;
}

/**
  * @brief Read DMA Linked list node Transfer completed callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SDEx_Write_DMALnkLstBufCpltCallback(SD_HandleTypeDef *hsd)
{
  TxNodeBufCplt = 1;
}

/**
  * @brief SD error callbacks
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
  Error_Handler();
}



/**
  * @brief  Fills the goal 32-bit length buffer
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLength: length of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint16_t BufferLength, uint32_t Offset)
{
  uint16_t IndexTmp;

  /* Put in global buffer same values */
  for ( IndexTmp = 0; IndexTmp < BufferLength; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }

}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer, pBuffer1: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer identical to pBuffer1
  *         FAILED: pBuffer differs from pBuffer1
  */
static HAL_StatusTypeDef Buffercmp(uint32_t* pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint32_t counter = 0;
  while(BufferLength--)
  {
    if(pBuffer[counter] != counter + Offset)
    {
      return HAL_ERROR;
    }
    counter++;
  }
  return HAL_OK;
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
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);
  __disable_irq();
  while (1)
  {
    /* Toggle LED_RED: Error */
    BSP_LED_Toggle(LED_RED);
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
