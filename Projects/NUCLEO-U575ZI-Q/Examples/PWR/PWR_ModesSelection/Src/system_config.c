/**
  ******************************************************************************
  * @file    PWR/PWR_ModesSelection/Src/system_config.c
  * @author  MCD Application Team
  * @brief   This file provides system configuration.
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
#include "system_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DUMMY_VALUE (0xFFFFFFFFU)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RTCHandle;
/* Ramcfg handler declaration */
RAMCFG_HandleTypeDef hramcfg;

/* Private function prototypes -----------------------------------------------*/
static System_StatusTypeDef SystemClock_160MHz(void);
static System_StatusTypeDef SystemClock_24MHz(void);
static System_StatusTypeDef RTC_Enable(void);
static System_StatusTypeDef RTC_Disable(void);
static System_StatusTypeDef SRAM_WaitStateConfig(RAMCFG_TypeDef *pInstance);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Optimize system power consumption configuration.
  * @param  None.
  * @retval System Status.
  */
System_StatusTypeDef system_config(RTC_StateTypeDef state, System_LowPowerModeTypeDef lowpower)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  GPIO_InitTypeDef   GPIO_InitStruct;

  /* Enable the Power-down Mode for Flash Bank 2 */
  if (HAL_FLASHEx_EnablePowerDown(FLASH_BANK_2) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Configuration of the LPM read mode */
  if (HAL_FLASHEx_ConfigLowPowerRead(FLASH_LPM_ENABLE) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

#ifdef Rev_2_1
  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Enable ultra low power mode mode in
  Stop 1, Stop 2, Stop 3 and Standby modes
  and when the regulator is in Range 4 (Run, Sleep or Stop 0 mode) */
  HAL_PWREx_EnableUltraLowPowerMode();
#endif /* Rev_2_1 */

  /* Disable HSI oscillator */
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState       = RCC_HSI_OFF;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Configure all GPIO in analog state to reduce power consumption except for STOP1*/
  if (lowpower != SYSTEM_LOWPOWER_STOP1)
  {
    /* Enable all GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    /* Set parameters to be configured */
    GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Pin   = GPIO_PIN_ALL;

    /* Initialize all GPIO pins */
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    /* Disable all GPIO clocks */
    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_GPIOE_CLK_DISABLE();
    __HAL_RCC_GPIOF_CLK_DISABLE();
    __HAL_RCC_GPIOG_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();
    __HAL_RCC_GPIOI_CLK_DISABLE();
  }

  /* Disable all unused peripheral clocks */
  MODIFY_REG(RCC->AHB1ENR, DUMMY_VALUE, RCC_AHB1ENR_FLASHEN);
  MODIFY_REG(RCC->AHB2ENR1, DUMMY_VALUE, RCC_AHB2ENR1_SRAM2EN);
  MODIFY_REG(RCC->AHB2ENR2, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->AHB3ENR, DUMMY_VALUE, RCC_AHB3ENR_PWREN);
  MODIFY_REG(RCC->APB1ENR1, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->APB1ENR2, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->APB2ENR, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->APB3ENR, DUMMY_VALUE, RCC_APB3ENR_RTCAPBEN);
  MODIFY_REG(RCC->AHB1SMENR, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->AHB2SMENR1, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->AHB2SMENR2, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->AHB3SMENR, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->APB1SMENR1, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->APB1SMENR2, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->APB2SMENR, DUMMY_VALUE, 0U);
  MODIFY_REG(RCC->APB3SMENR, DUMMY_VALUE, 0U);

  /* Disable AHB APB Peripheral Clock */
  __HAL_RCC_AHB1_CLK_DISABLE();
  __HAL_RCC_AHB2_1_CLK_DISABLE();
  __HAL_RCC_AHB2_2_CLK_DISABLE();
  if (lowpower == SYSTEM_LOWPOWER_RUN)
  {
    __HAL_RCC_AHB3_CLK_DISABLE();
  }
  __HAL_RCC_APB1_CLK_DISABLE();
  __HAL_RCC_APB2_CLK_DISABLE();
  if (state == RTC_STATE_DISABLE)
  {
    __HAL_RCC_APB3_CLK_ENABLE();
    __HAL_RCC_RTCAPB_CLK_ENABLE();

    /* Disable LSE oscillator */
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.LSEState       = RCC_LSE_OFF;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      return SYSTEM_ERROR;
    }
  }

  return SYSTEM_OK;
}

/**
  * @brief  Configure system low power mode.
  * @param  lowpower_mode : Specifies low power mode which could be a value of
  *                         System_LowPowerModeTypeDef enumeration.
  * @retval System Status.
  */
System_StatusTypeDef system_lowpower_config(System_LowPowerModeTypeDef lowpower_mode)
{
  System_StatusTypeDef status = SYSTEM_OK;

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  switch (lowpower_mode)
  {
    /* Check if system low power mode is Run */
  case SYSTEM_LOWPOWER_RUN:
    {

      break;
    }

    /* Check if system low power mode is Stop1 */
  case SYSTEM_LOWPOWER_STOP1:
    {
      /* Enter the system to STOP 1 mode */
      HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);

      break;
    }

    /* Check if system low power mode is Stop2 */
  case SYSTEM_LOWPOWER_STOP2:
    {
      /* Enter the system to STOP 2 mode */
      HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

      break;
    }

    /* Check if system low power mode is Stop3 */
  case SYSTEM_LOWPOWER_STOP3:
    {
      /* Enter the system to STOP 3 mode */
      HAL_PWREx_EnterSTOP3Mode(PWR_STOPENTRY_WFI);

      break;
    }

    /* Check if system low power mode is Standby */
  case SYSTEM_LOWPOWER_STANDBY:
    {
      /* Enter the system to STANDBY mode */
      HAL_PWR_EnterSTANDBYMode();

      break;
    }

    /* Check if system low power mode is Shutdown */
  case SYSTEM_LOWPOWER_SHUTDOWN:
    {
      /* Enter the system to SHUTDOWN mode */
      HAL_PWREx_EnterSHUTDOWNMode();

      break;
    }

    /* Return error if low power command is not ok */
  default:
    {
      status = SYSTEM_ERROR;

      break;
    }
  }

  return status;
}

/**
  * @brief  Configure system regulator.
  * @param  regulator : Specifies system regulator which could be a value of
  *                     System_RegulatorTypeDef enumeration.
  *
  * @retval System Status.
  */
System_StatusTypeDef system_regulator_config(System_RegulatorTypeDef regulator)
{
  System_StatusTypeDef status = SYSTEM_OK;

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  switch (regulator)
  {
    /* Check if system regulator is LDO */
  case SYSTEM_REGULATOR_LDO:
    {
      if (HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY) != HAL_OK)
      {
        return SYSTEM_ERROR;
      }

      break;
    }

    /* Check if system regulator is SMPS */
  case SYSTEM_REGULATOR_SMPS:
    {
      if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
      {
        return SYSTEM_ERROR;
      }

      break;
    }

    /* Check system regulator is DEFAULT */
  case SYSTEM_REGULATOR_DEFAULT:
    {
      break;
    }

    /* Return error if system regulator command is not ok */
  default:
    {
      status = SYSTEM_ERROR;

      break;
    }
  }

  return status;
}

/**
  * @brief  Configure rtc state.
  * @param  state : Specifies rtc state which could be a value of RTC_StateTypeDef enumeration.
  *
  * @retval System Status.
  */
System_StatusTypeDef system_rtc_config(RTC_StateTypeDef state)
{
  System_StatusTypeDef status = SYSTEM_OK;

  switch (state)
  {
    /* Check if rtc state is DISABLE */
  case RTC_STATE_DISABLE:
    {
      if (RTC_Disable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }

      break;
    }

    /* Check if rtc state is ENABLE */
  case RTC_STATE_ENABLE:
    {
      if (RTC_Enable() != SYSTEM_OK)
      {
        status = SYSTEM_ERROR;
      }

      break;
    }

    /* Check rtc state is DEFAULT */
  case RTC_STATE_DEFAULT:
    {
      break;
    }

    /* Return error if system regulator command is not ok */
  default:
    {
      status = SYSTEM_ERROR;

      break;
    }
  }

  return status;
}

/**
  * @brief  Configure system clock frequency.
  * @param  clock_frequency : Specifies system clock frequency which could be a value of
  *                           System_ClockFrequencyTypeDef enumeration.
  *
  * @retval System Status.
  */
System_StatusTypeDef system_clock_config(System_ClockFrequencyTypeDef clock_frequency)
{
  System_StatusTypeDef status = SYSTEM_OK;

  switch (clock_frequency)
  {
    /* Check system clock frequency is 160 MH */
  case SYSTEM_CLOCK_160MH:
    {
      if (SystemClock_160MHz() != SYSTEM_OK)
      {
        return SYSTEM_ERROR;
      }

      break;
    }

    /* Check system clock frequency is 24MH */
  case SYSTEM_CLOCK_24MH:
    {
      if (SystemClock_24MHz() != SYSTEM_OK)
      {
        return SYSTEM_ERROR;
      }

      break;
    }

    /* Check system clock frequency is DEFAULT */
  case SYSTEM_CLOCK_DEFAULT:
    {

      break;
    }
    /* Return error if system clock frequency command is not ok */
  default:
    {
      status = SYSTEM_ERROR;

      break;
    }
  }

  return status;
}

/**
  * @brief  Configure SRAM retention.
  * @param  lowpower_mode : Specifies low power mode which could be a value of
  *                         System_LowPowerModeTypeDef enumeration.
  * @param  retention     : Specifies system ram retention configuration.
  * @retval System Status.
  */
System_StatusTypeDef system_ramretention_config(System_LowPowerModeTypeDef lowpower_mode, uint32_t retention)
{
  System_StatusTypeDef status = SYSTEM_OK;

  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Check if system low power is Run */
  switch (lowpower_mode)
  {
  case SYSTEM_LOWPOWER_RUN:
    {
      if ((System_Run_RamRetentionTypeDef)retention == SYSTEM_RET_RUN_SRAM_FULL)
      {
        /* Enable all SRAM retention in Run mode */
        HAL_PWREx_EnableRAMsContentRunRetention(PWR_SRAM1_FULL_RUN_RETENTION);
        HAL_PWREx_EnableRAMsContentRunRetention(PWR_SRAM2_FULL_RUN_RETENTION);
        HAL_PWREx_EnableRAMsContentRunRetention(PWR_SRAM3_FULL_RUN_RETENTION);
        HAL_PWREx_EnableRAMsContentRunRetention(PWR_SRAM4_FULL_RUN_RETENTION);
      }
      else
      {
        /* Enable only SRAM2 retention in Run mode */
        HAL_PWREx_DisableRAMsContentRunRetention(PWR_SRAM1_FULL_RUN_RETENTION);
        HAL_PWREx_EnableRAMsContentRunRetention(PWR_SRAM2_FULL_RUN_RETENTION);
        HAL_PWREx_DisableRAMsContentRunRetention(PWR_SRAM3_FULL_RUN_RETENTION);
        HAL_PWREx_DisableRAMsContentRunRetention(PWR_SRAM4_FULL_RUN_RETENTION);
      }

      break;
    }

    /* Check if system low power is Stop1 */
  case SYSTEM_LOWPOWER_STOP1:
    {
      if ((System_Stop1_RamRetentionTypeDef)retention == SYSTEM_RET_STOP1_SRAM_FULL)
      {
        /* Enable all SRAM retention in Stop1 mode */
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM1_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM2_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM3_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM4_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_ICACHE_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_DCACHE1_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_DMA2DRAM_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_PERIPHRAM_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_PKA32RAM_FULL_STOP_RETENTION);
      }
      else
      {
        /* Enable only 8KB SRAM2 and icache full retention in Stop1 mode */
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM1_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM3_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM4_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_DCACHE1_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_DMA2DRAM_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_PERIPHRAM_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_PKA32RAM_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_ICACHE_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM2_PAGE1_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM2_PAGE2_STOP_RETENTION);
      }

      break;
    }

    /* Check if system low power is Stop2 */
  case SYSTEM_LOWPOWER_STOP2:
    {
      if ((System_Stop2_RamRetentionTypeDef)retention == SYSTEM_RET_STOP2_SRAM_FULL)
      {
        /* Enable all SRAM retention in Stop2 mode */
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM1_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM2_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM3_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM4_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_ICACHE_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_DCACHE1_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_DMA2DRAM_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_PERIPHRAM_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_PKA32RAM_FULL_STOP_RETENTION);
      }
      else
      {
        /* Enable only 8KB SRAM2 and icache full retention in Stop2 mode */
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM1_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM3_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM4_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_DCACHE1_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_DMA2DRAM_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_PERIPHRAM_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_PKA32RAM_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_ICACHE_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM2_PAGE1_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM2_PAGE2_STOP_RETENTION);
      }

      break;
    }
    /* Check if system low power is Stop3 */
  case SYSTEM_LOWPOWER_STOP3:
    {
      if ((System_Stop3_RamRetentionTypeDef)retention == SYSTEM_RET_STOP3_SRAM_FULL)
      {
        /* Enable all SRAM retention in Stop3 mode */
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM1_FULL_STOP_RETENTION);
        HAL_PWREx_EnableSRAM2ContentStandbyRetention(PWR_SRAM2_FULL_STANDBY_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM3_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_SRAM4_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_ICACHE_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_DCACHE1_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_DMA2DRAM_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_PERIPHRAM_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_PKA32RAM_FULL_STOP_RETENTION);
      }
      else
      {
        /* Enable only 8KB SRAM2 and icache full retention in Stop3 mode */
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM1_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM3_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_SRAM4_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_DCACHE1_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_DMA2DRAM_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_PERIPHRAM_FULL_STOP_RETENTION);
        HAL_PWREx_DisableRAMsContentStopRetention(PWR_PKA32RAM_FULL_STOP_RETENTION);
        HAL_PWREx_EnableRAMsContentStopRetention(PWR_ICACHE_FULL_STOP_RETENTION);
        HAL_PWREx_EnableSRAM2ContentStandbyRetention(PWR_SRAM2_PAGE1_STANDBY_RETENTION);
        HAL_PWREx_DisableSRAM2ContentStandbyRetention(PWR_SRAM2_PAGE2_STANDBY_RETENTION);
      }

      break;
    }

    /* Check if system low power is Standby */
  case SYSTEM_LOWPOWER_STANDBY:
    {
      if ((System_Standby_RamRetentionTypeDef)retention == SYSTEM_RET_STANDBY_SRAM2_NO)
      {
        /* Disable all SRAM2 retention in Standby mode */
        HAL_PWREx_DisableSRAM2ContentStandbyRetention(PWR_SRAM2_FULL_STANDBY_RETENTION);
        HAL_PWREx_DisableBkupRAMRetention();
      }
      else if ((System_Standby_RamRetentionTypeDef)retention == SYSTEM_RET_STANDBY_ONLY_SRAM2_8KB_BKPRAM)
      {
        /* Enable only 8KB SRAM2 retention in Standby mode */
        HAL_PWREx_EnableSRAM2ContentStandbyRetention(PWR_SRAM2_PAGE1_STANDBY_RETENTION);
        HAL_PWREx_DisableSRAM2ContentStandbyRetention(PWR_SRAM2_PAGE2_STANDBY_RETENTION);

        /* Check if system regulator is LDO */
        if (HAL_PWREx_GetSupplyConfig() == PWR_LDO_SUPPLY)
        {
          /* Enable the Backup RAM retention in Standby mode */
          HAL_PWREx_EnableBkupRAMRetention();
        }
        else
        {
          if (HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY) != HAL_OK)
          {
            return SYSTEM_ERROR;
          }

          /* Enable the Backup RAM retention in Standby mode */
          HAL_PWREx_EnableBkupRAMRetention();

          if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
          {
            return SYSTEM_ERROR;
          }
        }
      }
      else
      {
        /* Enable full SRAM2 retention in Standby mode */
        HAL_PWREx_EnableSRAM2ContentStandbyRetention(PWR_SRAM2_FULL_STANDBY_RETENTION);

        if (HAL_PWREx_GetSupplyConfig() == PWR_LDO_SUPPLY)
        {
          /* Enable the Backup RAM retention in Standby mode */
          HAL_PWREx_EnableBkupRAMRetention();
        }
        else
        {
          if (HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY) != HAL_OK)
          {
            return SYSTEM_ERROR;
          }

          /* Enable the Backup RAM retention in Standby mode */
          HAL_PWREx_EnableBkupRAMRetention();

          if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
          {
            return SYSTEM_ERROR;
          }
        }
      }

      break;
    }

    /* Check if system low power is Shutdown */
  case SYSTEM_LOWPOWER_SHUTDOWN:
    {

      break;
    }

    /* Return error if SRAM retention command is not ok */
  default:
    {

      status = SYSTEM_ERROR;

      break;
    }
  }

  return status;
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
  * @retval System Status
  */
 static System_StatusTypeDef SystemClock_160MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Switch the system clock to MSI */
  __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_MSI);

  /* Enable voltage range 1 for frequency above 100 Mhz */
  __HAL_RCC_PWR_CLK_ENABLE();
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* MSI Oscillator enabled at reset (4Mhz), activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_4;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
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
    return SYSTEM_ERROR;
  }

  /* Select PLL as system clock source and configure bus clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | \
                                      RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  return SYSTEM_OK;
}

/**
  * @brief  Configure system clock frequency to 24MHz (MSI range 1).
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef SystemClock_24MHz(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Switch the system clock to MSI */
  __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_MSI);

  /* Update MSI to 24Mhz (RCC_MSIRANGE_1) */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState            = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange       = RCC_MSIRANGE_1;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Turn off PLL */
  RCC_OscInitStruct.PLL.PLLState  = RCC_PLL_OFF;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Select MSI as system clock source and configure bus clocks dividers */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_PCLK1 | \
                                      RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Set voltage scaling Range 4 */
  __HAL_RCC_PWR_CLK_ENABLE();
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE4) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Enable or disable the MSI bias mode selection */
  __HAL_RCC_MSIBIAS_SELECTION_ENABLE();

  /* Configure the wait state value for  SRAM 1 */
  if (SRAM_WaitStateConfig(RAMCFG_SRAM1) != SYSTEM_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Configure the wait state value for  SRAM 2 */
  if (SRAM_WaitStateConfig(RAMCFG_SRAM2) != SYSTEM_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Configure the wait state value for  SRAM 3 */
  if (SRAM_WaitStateConfig(RAMCFG_SRAM3) != SYSTEM_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Configure the wait state value for  SRAM 4 */
  if (SRAM_WaitStateConfig(RAMCFG_SRAM4) != SYSTEM_OK)
  {
    return SYSTEM_ERROR;
  }

  return SYSTEM_OK;
}

/**
  * @brief  Configure the RTC peripheral and Initialize it.
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef RTC_Enable(void)
{
  /* Set RTC instance */
  RTCHandle.Instance = RTC;

  /* Set parameter to be configured */
  RTCHandle.Init.HourFormat     = RTC_HOURFORMAT_24;
  RTCHandle.Init.AsynchPrediv   = 0x7FU;
  RTCHandle.Init.SynchPrediv    = 0xF9U;
  RTCHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RTCHandle.Init.OutPutRemap    = RTC_OUTPUT_REMAP_NONE;
  RTCHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RTCHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
  RTCHandle.Init.OutPutPullUp   = RTC_OUTPUT_PULLUP_NONE;

  /* Initialize the RTC peripheral */
  if (HAL_RTC_Init(&RTCHandle) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Set Calendar Ultra-Low power mode */
  if (HAL_RTCEx_SetLowPowerCalib(&RTCHandle, RTC_LPCAL_SET) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  return SYSTEM_OK;
}

/**
  * @brief  De-Initialize the RTC peripheral.
  * @param  None.
  * @retval System Status.
  */
static System_StatusTypeDef RTC_Disable(void)
{
  /* Set RTC instance */
  RTCHandle.Instance = RTC;

  /* Check RTC state */
  if (HAL_RTC_GetState(&RTCHandle) != HAL_RTC_STATE_RESET)
  {
    /* De-Initialize the RTC peripheral */
    if (HAL_RTC_DeInit(&RTCHandle) != HAL_OK)
    {
      return SYSTEM_ERROR;
    }
  }

  return SYSTEM_OK;
}

/**
  * @brief  Configure the RAMCFG peripheral.
  * @param  pInstance:   Pointer to a RAMCFG_TypeDef structure that contains
  *                      SRAMs configuration for the specified RAMCFG
  *                      instance.
  * @retval System Status.
  */
static System_StatusTypeDef SRAM_WaitStateConfig(RAMCFG_TypeDef *pInstance)
{
  /* Initialize the RAMCFG peripheral */
  hramcfg.Instance = pInstance;

  if (HAL_RAMCFG_Init(&hramcfg) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* Configure the wait state value for the given SRAM */
  if (HAL_RAMCFG_ConfigWaitState(&hramcfg, RAMCFG_WAITSTATE_1) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  /* De-Initialize the RAMCFG peripheral */
  if (HAL_RAMCFG_DeInit(&hramcfg) != HAL_OK)
  {
    return SYSTEM_ERROR;
  }

  return SYSTEM_OK;
}
