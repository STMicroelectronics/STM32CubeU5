/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Examples_LL/RTC/RTC_Tamper_Init/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32U5xx RTC LL API
  *          to write/read data to/from RTC Backup data registers and demonstrates
  *          the Tamper detection feature.
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

#define BACKUP_COUNT 20
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

__IO FlagStatus TamperStatus = RESET;

/* Backup registers table */
uint32_t aBKPDataReg[BACKUP_COUNT] =
{
  LL_RTC_BKP_DR0,  LL_RTC_BKP_DR1,  LL_RTC_BKP_DR2,
  LL_RTC_BKP_DR3,  LL_RTC_BKP_DR4,  LL_RTC_BKP_DR5,
  LL_RTC_BKP_DR6,  LL_RTC_BKP_DR7,  LL_RTC_BKP_DR8,
  LL_RTC_BKP_DR9,  LL_RTC_BKP_DR10, LL_RTC_BKP_DR11,
  LL_RTC_BKP_DR12, LL_RTC_BKP_DR13, LL_RTC_BKP_DR14,
  LL_RTC_BKP_DR15, LL_RTC_BKP_DR16, LL_RTC_BKP_DR17,
  LL_RTC_BKP_DR18, LL_RTC_BKP_DR19
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
static void MX_RTC_Init(void);
static void MX_TAMP_RTC_Init(void);
/* USER CODE BEGIN PFP */
void     LED_On(void);
void     LED_Blinking(uint32_t Period);
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

  register uint32_t index = 0;


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* Enable PWR clock interface */

  LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_PWR);

  /* USER CODE BEGIN Init */
  /* Enables access to the backup domain */
  LL_PWR_EnableBkUpAccess();

  /* USER CODE END Init */

  /* Configure the System Power */
  SystemPower_Config();

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ICACHE_Init();
  MX_RTC_Init();
  MX_TAMP_RTC_Init();
  /* USER CODE BEGIN 2 */
  LL_RTC_ClearFlag_TAMP1(RTC);
  /* Write Data on the Back Up registers  */
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    LL_RTC_BKP_SetRegister(RTC, aBKPDataReg[index], 0xDF59 + (index * 0x5A));
  }

  /* Check Data is stored on the Back Up registers */
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    if (LL_RTC_BKP_GetRegister(RTC, aBKPDataReg[index]) != (0xDF59 + (index * 0x5A)))
    {
      LED_Blinking(LED_BLINK_ERROR);
    }
  }

  /* Wait for Tamper detection */
  while (TamperStatus != SET)
  {
    LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    LL_mDelay(LED_BLINK_FAST);
  }

  /* LED1 On: Tamper button pressed */
  LED_On();

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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_4)
  {
  }

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_PWR_EnableBkUpAccess();
  while (LL_PWR_IsEnabledBkUpAccess () == 0U)
  {
  }

  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1)
  {
  }

  LL_RCC_MSIS_Enable();

   /* Wait till MSIS is ready */
  while(LL_RCC_MSIS_IsReady() != 1)
  {
  }

  LL_RCC_MSI_EnableRangeSelection();
  LL_RCC_MSIS_SetRange(LL_RCC_MSISRANGE_4);
  LL_RCC_MSI_SetCalibTrimming(16, LL_RCC_MSI_OSCILLATOR_1);
  LL_RCC_PLL1_ConfigDomain_SYS(LL_RCC_PLL1SOURCE_MSIS, 1, 80, 2);
  LL_RCC_PLL1_EnableDomain_SYS();
  LL_RCC_SetPll1EPodPrescaler(LL_RCC_PLL1MBOOST_DIV_1);
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_4_8);
  LL_RCC_PLL1_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

   /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {
  }

  /* Insure 1us transition state at intermediate medium speed clock*/
  for (__IO uint32_t i = (160 >> 1); i !=0; i--);

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_1);

  LL_Init1msTick(160000000);

  LL_SetSystemCoreClock(160000000);
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
  LL_PWR_DisableUCPDDeadBattery();

  /*
   * Switch to SMPS regulator instead of LDO
   */
  LL_PWR_SetRegulatorSupply(LL_PWR_SMPS_SUPPLY);

  while(LL_PWR_IsActiveFlag_REGULATOR()!=1)
  {
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
  LL_ICACHE_SetMode(LL_ICACHE_1WAY);
  LL_ICACHE_Enable();
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  LL_RTC_InitTypeDef RTC_InitStruct = {0};

  if(LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSI)
  {
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
  }

  /* Peripheral clock enable */
  LL_RCC_EnableRTC();
  LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_RTCAPB);
  LL_SRDAMR_GRP1_EnableAutonomousClock(LL_SRDAMR_GRP1_PERIPH_RTCAPBAMEN);

  /* USER CODE BEGIN RTC_Init 1 */

  /* Enables the PWR Clock and Enables access to the backup domain */
  LL_PWR_EnableBkUpAccess();
  /* USER CODE END RTC_Init 1 */
  RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
  RTC_InitStruct.AsynchPrescaler = 127;
  RTC_InitStruct.SynchPrescaler = 255;
  LL_RTC_Init(RTC, &RTC_InitStruct);
  LL_RTC_SetBackupRegisterPrivilege(RTC, LL_RTC_PRIVILEGE_BKUP_ZONE_NONE);
  LL_RTC_SetBackupRegProtection(RTC, LL_RTC_BKP_DR0, LL_RTC_BKP_DR0);
  LL_RTC_SetRtcPrivilege(RTC, LL_RTC_PRIVILEGE_FULL_NO);
  /* USER CODE BEGIN RTC_Init 2 */
  /* Enable IT TAMPER */
  LL_RTC_EnableIT_TAMP1(RTC);

  /* Configure the NVIC for RTC Tamper */
  NVIC_SetPriority(TAMP_IRQn, 0x0F);
  NVIC_EnableIRQ(TAMP_IRQn);

  /* RTC Tamper Interrupt Configuration: EXTI configuration */
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_19);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_19);

  /* Clear the Tamper interrupt pending bit */
  LL_RTC_ClearFlag_TAMP1(RTC);
  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TAMP Initialization Function
  * @param None
  * @retval None
  */
static void MX_TAMP_RTC_Init(void)
{

  /* USER CODE BEGIN TAMP_Init 0 */

  /* USER CODE END TAMP_Init 0 */

  /* Peripheral clock enable */
  LL_RCC_EnableRTC();

  /* USER CODE BEGIN TAMP_Init 1 */

  /* USER CODE END TAMP_Init 1 */
  LL_RTC_SetTampPrivilege(RTC, LL_TAMP_PRIVILEGE_FULL_YES);
  LL_RTC_IncrementMonotonicCounter(RTC);
  LL_RTC_TAMPER_SetFilterCount(RTC, LL_RTC_TAMPER_FILTER_DISABLE);
  LL_RTC_TAMPER_SetPrecharge(RTC, LL_RTC_TAMPER_DURATION_1RTCCLK);
  LL_RTC_TAMPER_SetSamplingFreq(RTC, LL_RTC_TAMPER_SAMPLFREQDIV_32768);
  LL_RTC_TAMPER_EnablePullUp(RTC);

  /** Enable the RTC Tamper 1
  */
  LL_RTC_TAMPER_Enable(RTC, LL_RTC_TAMPER_1);
  LL_RTC_TAMPER_EnableEraseBKP(RTC, LL_RTC_TAMPER_NOERASE_TAMPER1);
  LL_RTC_TAMPER_DisableMask(RTC, LL_RTC_TAMPER_MASK_TAMPER1);
  LL_RTC_TS_EnableOnTamper(RTC);
  LL_RTC_TAMPER_ATAMP_DisableActiveMode(LL_RTC_TAMPER_ATAMP_TAMP1AM);
  /* USER CODE BEGIN TAMP_Init 2 */

  /* USER CODE END TAMP_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

  /**/
  LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);

  /**/
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


/**
  * @brief  Turn-on LED1.
  * @param  None
  * @retval None
  */
void LED_On(void)
{
  /* Turn LED1 on */
  LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
}

/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle IO in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    LL_mDelay(Period);
  }
}

/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT                                               */
/******************************************************************************/
/**
  * @brief  Tamper event callback function
  * @param  None
  * @retval None
  */
void Tamper_Callback(void)
{
  register uint32_t index = 0;

  /* Deactivate the tamper */
  LL_RTC_TAMPER_Disable(RTC, LL_RTC_TAMPER_1);

  /* Check Data is cleared on the Back Up registers */
  for (index = 0; index < BACKUP_COUNT; index++)
  {
    if (LL_RTC_BKP_GetRegister(RTC, aBKPDataReg[index]) != 0x00)
    {
      LED_Blinking(LED_BLINK_ERROR);
    }
  }
  TamperStatus = SET;
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

  /* Infinite loop */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
