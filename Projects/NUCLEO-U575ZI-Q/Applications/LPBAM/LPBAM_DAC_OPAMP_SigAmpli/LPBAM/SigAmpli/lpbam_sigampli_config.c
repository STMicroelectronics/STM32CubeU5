/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file   lpbam_sigampli_config.c
  * @author MCD Application Team
  * @brief  Provides LPBAM SigAmpli application configuration services
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */
/* USER CODE END Header */
/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "lpbam_sigampli.h"

/* Private variables -------------------------------------------------------------------------------------------------*/
static RAMCFG_HandleTypeDef hramcfg;

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static void MX_SystemClock_Config(void);
static void MX_SystemPower_Config(void);
static void MX_SRAM_WaitState_Config(void);

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
  * @brief  SigAmpli application initialization
  * @param  None
  * @retval None
  */
void MX_SigAmpli_Init(void)
{
  /* USER CODE BEGIN SigAmpli_Init 0 */

  /* USER CODE END SigAmpli_Init 0 */

  /* Configure SRAM wait states */
  MX_SRAM_WaitState_Config();

  /* Configure system clock */
  MX_SystemClock_Config();

  /* Configure system power */
  MX_SystemPower_Config();

  /* USER CODE BEGIN SigAmpli_Init 1 */

  /* USER CODE END SigAmpli_Init 1 */
}

/* Private functions -------------------------------------------------------------------------------------------------*/
/**
  * @brief  System Clock Configuration
  * @param  None
  * @retval None
  */
static void MX_SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  if ((__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_MSI) &&
      (__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK))
  {
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  }

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI
                              |RCC_OSCILLATORTYPE_MSIK;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.LSIDiv = RCC_LSI_DIV1;
  RCC_OscInitStruct.MSIKClockRange = RCC_MSIKRANGE_4;
  RCC_OscInitStruct.MSIKState = RCC_MSIK_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the wakeup clock
  */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);

  /** Disable the force MSIK in stop mode
  */
  __HAL_RCC_MSIKSTOP_DISABLE();

  /** Disable the force HSI in stop mode
  */
  __HAL_RCC_HSISTOP_DISABLE();
}

/**
  * @brief  System Power Configuration
  * @param  None
  * @retval None
  */
static void MX_SystemPower_Config(void)
{
  /* USER CODE BEGIN PWR_Config 0 */

  /* USER CODE END PWR_Config 0 */

  /* Enable PWR interface clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /*
   * Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
   */
  HAL_PWREx_DisableUCPDDeadBattery();

  /*
   * SRAM Power Down In Stop Mode Config
   */
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM1_PAGE1_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM1_PAGE2_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM1_PAGE3_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM2_PAGE1_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM2_PAGE2_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM3_PAGE1_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM3_PAGE2_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM3_PAGE3_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM3_PAGE4_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM3_PAGE5_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM3_PAGE6_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM3_PAGE7_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM3_PAGE8_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_DCACHE1_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_DMA2DRAM_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_PKA32RAM_FULL_STOP_RETENTION);
  HAL_PWREx_DisableRAMsContentStopRetention(PWR_PERIPHRAM_FULL_STOP_RETENTION);
  HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM4_FULL_STOP_RETENTION);
  HAL_PWREx_EnableRAMsContentStopRetention(PWR_ICACHE_FULL_STOP_RETENTION);

  /*
   * Switch to SMPS regulator instead of LDO
   */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }

  /*
   * SRAM4 Fast In Run Mode Config
   */
  HAL_PWREx_DisableSRAM4FastWakeUp();

  /*
   * Smart Run Domain Config
   */
  HAL_PWREx_ConfigSRDDomain(PWR_SRD_DOMAIN_STOP);

  /* USER CODE BEGIN PWR_Config 1 */

  /* USER CODE END PWR_Config 1 */
}

/**
  * @brief  SRAM Wait State configuration
  * @param  None
  * @retval None
  */
static void MX_SRAM_WaitState_Config(void)
{
  /* RAMCFG clock enable */
  __HAL_RCC_RAMCFG_CLK_ENABLE();

  /*
   * RAMCFG initialization
   */
  hramcfg.Instance = RAMCFG_SRAM4;
  if (HAL_RAMCFG_Init(&hramcfg) != HAL_OK)
  {
    Error_Handler();
  }

  /*
   * RAMCFG wait states configuration
   */
  if (HAL_RAMCFG_ConfigWaitState(&hramcfg, RAMCFG_WAITSTATE_0) != HAL_OK)
  {
    Error_Handler();
  }
}
