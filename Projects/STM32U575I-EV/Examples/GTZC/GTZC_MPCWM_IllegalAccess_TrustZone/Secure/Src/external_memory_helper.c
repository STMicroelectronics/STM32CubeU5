/* USER CODE BEGIN Header */
/**
  ******************************************************************************
* @file    : Secure/Src/external_memory_helper.c
* @author  : MCD Application Team
* @brief   : Helper C file to configure the external memory
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "external_memory_helper.h"
#include "stm32u5xx_hal.h"
#include "stm32u5xx_ll_fmc.h"
#include "stm32u5xx_hal_sram.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FMC_NOR_DEVICE            FMC_NORSRAM_DEVICE;
#define FMC_SRAM_DEVICE           FMC_NORSRAM_DEVICE;
#define FMC_SRAM_EXTENDED_DEVICE  FMC_NORSRAM_EXTENDED_DEVICE
#define FMC_NOR_EXTENDED_DEVICE   FMC_NORSRAM_EXTENDED_DEVICE
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

SRAM_HandleTypeDef    hsram;
FMC_NORSRAM_TimingTypeDef SRAM_Timing;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/

/**
* @brief  external memory configuration
* @retval None
*/
uint32_t extMemory_Config( void )
{
  /* SRAM device configuration */
  hsram.Instance  = FMC_SRAM_DEVICE;
  hsram.Extended  = FMC_SRAM_EXTENDED_DEVICE;

  hsram.Init.NSBank             = FMC_SRAM_BANKx;
  hsram.Init.DataAddressMux     = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram.Init.MemoryType         = FMC_MEMORY_TYPE_SRAM;
  hsram.Init.MemoryDataWidth    = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram.Init.BurstAccessMode    = FMC_BURST_ACCESS_MODE_DISABLE;
  hsram.Init.WriteOperation     = FMC_WRITE_OPERATION_ENABLE;
  hsram.Init.ExtendedMode       = FMC_EXTENDED_MODE_DISABLE;
  hsram.Init.AsynchronousWait   = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram.Init.WaitSignal         = FMC_WAIT_SIGNAL_DISABLE;
  hsram.Init.WriteBurst         = FMC_WRITE_BURST_DISABLE;
  hsram.Init.ContinuousClock    = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hsram.Init.WriteFifo          = FMC_WRITE_FIFO_DISABLE;
  hsram.Init.NBLSetupTime       = 0;
  hsram.Init.PageSize           = FMC_PAGE_SIZE_NONE;

  /* Timings for SRAM */
  SRAM_Timing.AccessMode            = FMC_ACCESS_MODE_A;
  SRAM_Timing.AddressSetupTime      = 9;
  SRAM_Timing.AddressHoldTime       = 2;  /* Min value, Don't care on SRAM Access mode A */
  SRAM_Timing.DataSetupTime         = 6;
  SRAM_Timing.DataHoldTime          = 0;
  SRAM_Timing.BusTurnAroundDuration = 1;
  SRAM_Timing.CLKDivision           = 2;  /* Min value, Don't care on SRAM Access mode A */
  SRAM_Timing.DataLatency           = 2;  /* Min value, Don't care on SRAM Access mode A */

  /* Init SRAM controller */
  if(HAL_SRAM_Init(&hsram, &SRAM_Timing, &SRAM_Timing) != HAL_OK)
  {
    return 1;
  }

  return 0;
}
/**
* @}
*/

/**
* @}
*/