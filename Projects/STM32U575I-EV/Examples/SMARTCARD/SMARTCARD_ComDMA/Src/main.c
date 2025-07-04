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

SMARTCARD_HandleTypeDef hsmartcard3;
DMA_HandleTypeDef handle_GPDMA1_Channel1;
DMA_HandleTypeDef handle_GPDMA1_Channel0;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPIO_Init(void);
static void MX_GPDMA1_Init(void);
static void MX_ICACHE_Init(void);
static void MX_USART3_SMARTCARD_Init(void);
/* USER CODE BEGIN PFP */
__IO uint32_t CardInserted = 0;

/* APDU Transport Structures */
SmartCard_ADPU_Commands SmartCard_ADPU;
SmartCard_ADPU_Response SmartCard_Response;

uint8_t SmartCard_Command[5] = {0};
uint8_t SmartCard_Answer[50] = {0};

/* Smartcard Interface IO pins */
BSP_IO_Init_t   SmartCard_3_5V   = {SMARTCARD_3_5V_PIN   , IO_MODE_OUTPUT_PP       , IO_PULLUP};
BSP_IO_Init_t   SmartCard_RESET  = {SMARTCARD_RESET_PIN  , IO_MODE_OUTPUT_PP       , IO_PULLUP};
BSP_IO_Init_t   SmartCard_CMDVCC = {SMARTCARD_CMDVCC_PIN , IO_MODE_OUTPUT_PP       , IO_PULLUP};
BSP_IO_Init_t   SmartCard_OFF    = {SMARTCARD_OFF_PIN    , IO_MODE_IT_FALLING_EDGE , IO_PULLUP};

/* Directories & Files ID */
uint8_t MasterRoot[2] = {0x3F, 0x00};

SmartCard_ATR SmartCard_A2R;
uint8_t SmartCard_ATR_Table[40] = {0};

/* Private function prototypes -----------------------------------------------*/
static void SmartCard_DeInit(void);
static void SmartCard_IOConfig(void);
static void SmartCard_Handler(SmartCard_State *SCState, SmartCard_ADPU_Commands *SC_ADPU, SmartCard_ADPU_Response *SC_Response);
static uint8_t SmartCard_decode_Answer2reset(uint8_t *card);
static void SmartCard_AnswerReq(SmartCard_State *SCState, uint8_t *card, uint8_t length);
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

  /* Configure the System Power */
  SystemPower_Config();

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* Configure LED5, LED6 */
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  /* Initialize the IO expander */
  BSP_IOEXPANDER_Init(0, IOEXPANDER_IO_MODE);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_GPDMA1_Init();
  MX_ICACHE_Init();
  MX_USART3_SMARTCARD_Init();
  /* USER CODE BEGIN 2 */
    /* Configure Smartcard Interface GPIO pins */
  SmartCard_IOConfig();
  /* Loop while no Smartcard is detected */
  while(CardInserted == 0)
  {
    /* Toggle LED5*/
    BSP_LED_Toggle(LED5);
    HAL_Delay(100);
  }
  BSP_LED_Off(LED5);

  /* Disable the power to the smartcard */
  BSP_IO_WritePin(0, SMARTCARD_CMDVCC_PIN, IO_PIN_SET);

  /* -------------------------Wait answer to reset (A2R)-------------------------------*/
  SmartCard_State SCState = SC_POWER_ON;

  SmartCard_ADPU.Header.CLA = 0x00;
  SmartCard_ADPU.Header.INS = SC_GET_A2R;
  SmartCard_ADPU.Header.P1 = 0x00;
  SmartCard_ADPU.Header.P2 = 0x00;
  SmartCard_ADPU.Body.LC = 0x00;

  /* Power on card */
  while(SCState != SC_ACTIVE_ON_T0)
  {
    SmartCard_Handler(&SCState, &SmartCard_ADPU, &SmartCard_Response);
  }

  /* Wait a little after ATR message reception */
  HAL_Delay(100);

  /* Select MF -------------------------------------------------------------*/
  /* Send header -------------------------------------------------------------*/

  /* Start Tx DMA for Header transmission */
  SmartCard_Command[0] = SC_CLA_GSM11;
  SmartCard_Command[1] = SC_SELECT_FILE;
  SmartCard_Command[2] = 0x00;
  SmartCard_Command[3] = 0x00;
  SmartCard_Command[4] = 0x02;

  if(HAL_SMARTCARD_Transmit_DMA(&hsmartcard3, (uint8_t *)SmartCard_Command, 5) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_SMARTCARD_GetState(&hsmartcard3) != HAL_SMARTCARD_STATE_READY);

  /* Start Rx DMA for Ack Byte reception */
  if(HAL_SMARTCARD_Receive_DMA(&hsmartcard3, (uint8_t *)SmartCard_Answer, 1) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_SMARTCARD_GetState(&hsmartcard3) != HAL_SMARTCARD_STATE_READY);

  if (  ((SmartCard_Answer[0] & (uint8_t)0xFE) != (((uint8_t)~(SC_SELECT_FILE)) & (uint8_t)0xFE))
      &&((SmartCard_Answer[0] & (uint8_t)0xFE) != (SC_SELECT_FILE & (uint8_t)0xFE)))
  {
    Error_Handler();
  }

  /* ACK received */
  /* Start Tx DMA for APDU Command parameters */
  if(HAL_SMARTCARD_Transmit_DMA(&hsmartcard3, (uint8_t *)MasterRoot, 2) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_SMARTCARD_GetState(&hsmartcard3) != HAL_SMARTCARD_STATE_READY);

  /* Start Rx DMA for SW1/SW2 reception */
  if(HAL_SMARTCARD_Receive_DMA(&hsmartcard3, (uint8_t *)SmartCard_Answer, 2) != HAL_OK)
  {
    Error_Handler();
  }
  while (HAL_SMARTCARD_GetState(&hsmartcard3) != HAL_SMARTCARD_STATE_READY);

  if ((SmartCard_Answer[0] != 0x9F) &&(SmartCard_Answer[1] != 0x16))
  {
    Error_Handler();
  }

  /* Smartcard Deinit */
  SmartCard_DeInit();

  /* turn LED5 on */
  BSP_LED_On(LED5);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
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
    HAL_NVIC_SetPriority(GPDMA1_Channel0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel0_IRQn);
    HAL_NVIC_SetPriority(GPDMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);

  /* USER CODE BEGIN GPDMA1_Init 1 */

  /* USER CODE END GPDMA1_Init 1 */
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
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_SMARTCARD_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  hsmartcard3.Instance = USART3;
  hsmartcard3.Init.BaudRate = 10752;
  hsmartcard3.Init.WordLength = SMARTCARD_WORDLENGTH_9B;
  hsmartcard3.Init.StopBits = SMARTCARD_STOPBITS_1_5;
  hsmartcard3.Init.Parity = SMARTCARD_PARITY_EVEN;
  hsmartcard3.Init.Mode = SMARTCARD_MODE_TX_RX;
  hsmartcard3.Init.CLKPolarity = SMARTCARD_POLARITY_LOW;
  hsmartcard3.Init.CLKPhase = SMARTCARD_PHASE_1EDGE;
  hsmartcard3.Init.CLKLastBit = SMARTCARD_LASTBIT_DISABLE;
  hsmartcard3.Init.OneBitSampling = SMARTCARD_ONE_BIT_SAMPLE_DISABLE;
  hsmartcard3.Init.Prescaler = 2;
  hsmartcard3.Init.GuardTime = 0;
  hsmartcard3.Init.NACKEnable = SMARTCARD_NACK_DISABLE;
  hsmartcard3.Init.TimeOutEnable = SMARTCARD_TIMEOUT_DISABLE;
  hsmartcard3.Init.BlockLength = 0;
  hsmartcard3.Init.AutoRetryCount = 0;
  hsmartcard3.Init.ClockPrescaler = SMARTCARD_PRESCALER_DIV1;
  hsmartcard3.AdvancedInit.AdvFeatureInit = SMARTCARD_ADVFEATURE_NO_INIT;
  if (HAL_SMARTCARD_Init(&hsmartcard3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_SMARTCARDEx_SetTxFifoThreshold(&hsmartcard3, SMARTCARD_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_SMARTCARDEx_SetRxFifoThreshold(&hsmartcard3, SMARTCARD_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_SMARTCARDEx_DisableFifoMode(&hsmartcard3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */
  /* Set the smartcard reset pin to HIGH */
  BSP_IO_WritePin(0, SMARTCARD_RESET_PIN, IO_PIN_SET);

  /* Select 5V */
  BSP_IO_WritePin(0, SMARTCARD_3_5V_PIN, IO_PIN_SET);
  HAL_Delay(1);

  /* Enable the power to the smartcard */
  BSP_IO_WritePin(0, SMARTCARD_CMDVCC_PIN, IO_PIN_RESET);

  /* Set the smartcard reset pin to LOW */
  HAL_Delay(1);
  BSP_IO_WritePin(0, SMARTCARD_RESET_PIN, IO_PIN_RESET);
  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Deinitializes all resources used by the Smartcard interface.
  * @param  None
  * @retval None
  */
static void SmartCard_DeInit(void)
{
  /* Disable the power of the smartcard */
   BSP_IO_WritePin(0, SMARTCARD_CMDVCC_PIN, IO_PIN_SET);

  /* Deinitializes the hsmartcard3 */
  HAL_SMARTCARD_DeInit(&hsmartcard3);
}

/**
  * @brief  Configures GPIO hardware resources used for Smartcard.
  * @param  None
  * @retval None
  */
static void SmartCard_IOConfig(void)
{
   /* Configure Smartcard 3/5V pin */
  BSP_IO_Init(0, &SmartCard_3_5V);

  /* Configure Smartcard CMDVCC pin */
  BSP_IO_Init(0, &SmartCard_CMDVCC);

  /* Configure Smartcard RESET pin */
  BSP_IO_Init(0, &SmartCard_RESET);

  /* Configure Smartcard OFF presence pin */
  BSP_IO_Init(0, &SmartCard_OFF);

  BSP_IO_WritePin(0, SMARTCARD_CMDVCC_PIN, IO_PIN_SET);
  BSP_IO_WritePin(0, SMARTCARD_RESET_PIN,  IO_PIN_SET);

  /* Initialize CardInserted */
  /* Update the configuration of SMARTCARD_OFF_PIN */
  /* Configure IO interrupt acquisition mode */
  if(BSP_IO_ReadPin(0, SMARTCARD_OFF_PIN) == IO_PIN_SET)
  {
    CardInserted = 1;
    SmartCard_OFF.Mode = IO_MODE_IT_FALLING_EDGE;
  }
  else
  {
    CardInserted = 0;
    SmartCard_OFF.Mode = IO_MODE_IT_RISING_EDGE;
  }
  BSP_IO_Init(0, &SmartCard_OFF);
}

/**
  * @brief  Handles all Smartcard states and serves to send and receive all
  *         communication data between Smartcard and reader.
  * @param  SCState: pointer to an SC_State enumeration that will contain the
  *         Smartcard state.
  * @param  SC_ADPU: pointer to an SmartCard_ADPU_Commands structure that will be initialized.
  * @param  SC_Response: pointer to a SmartCard_ADPU_Response structure which will be initialized.
  * @retval None
  */
static void SmartCard_Handler(SmartCard_State *SCState, SmartCard_ADPU_Commands *SC_ADPU, SmartCard_ADPU_Response *SC_Response)
{
  uint32_t i = 0;
  switch(*SCState)
  {
  case SC_POWER_ON:
    if (SC_ADPU->Header.INS == SC_GET_A2R)
    {
      MX_USART3_SMARTCARD_Init();
      /* Reset Data from SC buffer -------------------------------------------*/
      for (i = 0; i < 40; i++)
      {
        SmartCard_ATR_Table[i] = 0;
      }

      /* Reset SC_A2R Structure ----------------------------------------------*/
      SmartCard_A2R.TS = 0;
      SmartCard_A2R.T0 = 0;
      for (i = 0; i < 20; i++)
      {
        SmartCard_A2R.T[i] = 0;
      }
      for (i = 0; i < 20; i++)
      {
        SmartCard_A2R.H[i] = 0;
      }
      SmartCard_A2R.Tlength = 0;
      SmartCard_A2R.Hlength = 0;

      /* Next State ----------------------------------------------------------*/
      *SCState = SC_RESET_LOW;
    }
    break;

  case SC_RESET_LOW:
    if(SC_ADPU->Header.INS == SC_GET_A2R)
    {
      /* If card is detected then Power ON, Card Reset and wait for an answer) */
      if (CardInserted == 1)
      {
        while(((*SCState) != SC_POWER_OFF) && ((*SCState) != SC_ACTIVE))
        {
          SmartCard_AnswerReq(SCState, (uint8_t *)&SmartCard_ATR_Table, 40); /* Check for answer to reset */
        }
      }
      else
      {
        (*SCState) = SC_POWER_OFF;
      }
    }
    break;

  case SC_ACTIVE:
    if (SC_ADPU->Header.INS == SC_GET_A2R)
    {
      if(SmartCard_decode_Answer2reset(&SmartCard_ATR_Table[0]) == T0_PROTOCOL)
      {
        (*SCState) = SC_ACTIVE_ON_T0;
      }
      else
      {
        (*SCState) = SC_POWER_OFF;
      }
    }
    break;

  case SC_POWER_OFF:
     /* Disable Smartcard interface */
    SmartCard_DeInit();
    break;

  default: (*SCState) = SC_POWER_OFF;
  }
}

/**
  * @brief  Requests the reset answer from card.
  * @param  SCState: pointer to an SC_State enumeration that will contain the Smartcard state.
  * @param  card: pointer to a buffer which will contain the card ATR.
  * @param  length: maximum ATR length
  * @retval None
  */
static void SmartCard_AnswerReq(SmartCard_State *SCState, uint8_t *card, uint8_t length)
{
  uint32_t tickstart = 0;
  switch(*SCState)
  {
  case SC_RESET_LOW:
    /* Check response with reset low -----------------------------------------*/

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();

    HAL_SMARTCARD_Receive_DMA(&hsmartcard3, card, length);
    while (HAL_SMARTCARD_GetState(&hsmartcard3) != HAL_SMARTCARD_STATE_READY)
    {
      /* In case of timeout (less bytes received than expected, return Error result */
      if((HAL_GetTick()-tickstart) > SC_RECEIVE_TIMEOUT)
      {
        /* Clean DMA handle used for Rx operation */
        HAL_SMARTCARD_AbortReceive(&hsmartcard3);
      }
    }

    if(card[0] != 0x00)
    {
      (*SCState) = SC_ACTIVE;
    }
    else
    {
      (*SCState) = SC_RESET_HIGH;
    }
    break;

  case SC_RESET_HIGH:
    /* Check response with reset high ----------------------------------------*/
    /*Set the reset pin High */
    BSP_IO_WritePin(0, SMARTCARD_RESET_PIN, IO_PIN_SET);

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();

    HAL_SMARTCARD_Receive_DMA(&hsmartcard3, card, length);
    while (HAL_SMARTCARD_GetState(&hsmartcard3) != HAL_SMARTCARD_STATE_READY)
    {
      /* In case of timeout (less bytes received than expected, return Error result */
      if((HAL_GetTick()-tickstart) > SC_RECEIVE_TIMEOUT)
      {
        /* Clean DMA handle used for Rx operation */
        HAL_SMARTCARD_AbortReceive(&hsmartcard3);
      }
    }

    if(card[0])
    {
      (*SCState) = SC_ACTIVE;
    }
    else
    {
      (*SCState) = SC_POWER_OFF;
    }
    break;

  case SC_ACTIVE:
    break;

  case SC_POWER_OFF:
    /* Close Connection if no answer received --------------------------------*/
    /* Reset pin high - a bit is used as level shifter from 3.3 to 5 V */
    BSP_IO_WritePin(0, SMARTCARD_RESET_PIN, IO_PIN_SET);

    /* Disable the power to the smartcard */
    BSP_IO_WritePin(0, SMARTCARD_CMDVCC_PIN, IO_PIN_SET);
    break;

  default:
    (*SCState) = SC_RESET_LOW;
    break;
  }
}

/**
  * @brief  Decodes the Answer to reset received from card.
  * @param  card: pointer to the buffer containing the card ATR.
  * @retval None
  */
static uint8_t SmartCard_decode_Answer2reset(uint8_t *card)
{
  uint32_t i = 0, flag = 0, buf = 0, protocol = 0;

  SmartCard_A2R.TS = card[0];  /* Initial character */
  SmartCard_A2R.T0 = card[1];  /* Format character */

  SmartCard_A2R.Hlength = SmartCard_A2R.T0 & (uint8_t)0x0F;

  if ((SmartCard_A2R.T0 & (uint8_t)0x80) == 0x80)
  {
    flag = 1;
  }

  for (i = 0; i < 4; i++)
  {
    SmartCard_A2R.Tlength = SmartCard_A2R.Tlength + (((SmartCard_A2R.T0 & (uint8_t)0xF0) >> (4 + i)) & (uint8_t)0x1);
  }

  for (i = 0; i < SmartCard_A2R.Tlength; i++)
  {
    SmartCard_A2R.T[i] = card[i + 2];
  }

  protocol = SmartCard_A2R.T[SmartCard_A2R.Tlength - 1] & (uint8_t)0x0F;

  while (flag)
  {
    if ((SmartCard_A2R.T[SmartCard_A2R.Tlength - 1] & (uint8_t)0x80) == 0x80)
    {
      flag = 1;
    }
    else
    {
      flag = 0;
    }

    buf = SmartCard_A2R.Tlength;
    SmartCard_A2R.Tlength = 0;

    for (i = 0; i < 4; i++)
    {
      SmartCard_A2R.Tlength = SmartCard_A2R.Tlength + (((SmartCard_A2R.T[buf - 1] & (uint8_t)0xF0) >> (4 + i)) & (uint8_t)0x1);
    }

    for (i = 0;i < SmartCard_A2R.Tlength; i++)
    {
      SmartCard_A2R.T[buf + i] = card[i + 2 + buf];
    }
    SmartCard_A2R.Tlength += (uint8_t)buf;
  }

  for (i = 0; i < SmartCard_A2R.Hlength; i++)
  {
    SmartCard_A2R.H[i] = card[i + 2 + SmartCard_A2R.Tlength];
  }

  return (uint8_t)protocol;
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
