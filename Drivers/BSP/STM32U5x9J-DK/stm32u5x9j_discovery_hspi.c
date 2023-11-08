/**
  ******************************************************************************
  * @file    stm32u5x9j_discovery_hspi.c
  * @author  MCD Application Team
  * @brief   This file includes a standard driver for the APS512XX 16bits-HSPI memory
  *          mounted on HSPI/OSPI daughter board MB1648 used with PEB3 Cobra MB1309 board.
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
  [..]
   (#) This driver is used to drive the APS512XX 16bits-HSPI PSRAM external memory mounted on
       on HSPI/OSPI daughter board MB1648 used with PEB3 Cobra MB1309 board.

   (#) This driver need specific component driver (APS512XX) to be included with.

   (#) APS512XX Initialization steps:
       (++) Initialize the 16bits-HSPI PSRAM external memory using the BSP_HSPI_RAM_Init() function. This
            function includes the MSP layer hardware resources initialization and the
            HSPI interface with the external memory.

   (#) APS512XX 16bits-HSPI PSRAM memory operations
       (++) 16bits-HSPI PSRAM memory can be accessed with read/write operations once it is
            initialized.
            Read/write operation can be performed with AHB access using the functions
            BSP_HSPI_RAM_Read()/BSP_HSPI_RAM_Write().
            Read/write operation can be performed with DMA using the functions
            BSP_HSPI_RAM_Read_DMA()/BSP_HSPI_RAM_Write_DMA().
       (++) The memory access can be configured in memory-mapped mode with the call of
            function BSP_HSPI_RAM_EnableMemoryMapped(). To go back in indirect mode, the
            function BSP_HSPI_RAM_DisableMemoryMapped() should be used.
       (++) It is possible to put the memory in deep power-down mode to reduce its consumption.
            For this, the function BSP_HSPI_RAM_EnterDeepPowerDown() should be called. To leave
            the deep power-down mode, the function BSP_HSPI_RAM_LeaveDeepPowerDown() should be called.
       (++) The configuration of the interface between peripheral and memory is done by
            the function BSP_HSPI_RAM_Config16bitsRAM() :
            - two latency types are possible :
              - Fixed latency
              - Variable latency
            - five latency codes are possible (for read and write) :
              - 3 latency clocks
              - 4 latency clocks
              - 5 latency clocks
              - 6 latency clocks
              - 7 latency clocks
            - four burst configuration are possible :
              - Linear burst access
              - 16-bytes hybrid burst access
              - 32-bytes hybrid burst access
              - 64-bytes hybrid burst access
            - two IO modes are possible :
              - IO x8 mode
              - IO x16 mode

  @endverbatim
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
#include "stm32u5x9j_discovery_hspi.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U5x9J_DISCOVERY
  * @{
  */

/** @defgroup STM32U5x9J_DISCOVERY_HSPI HSPI
  * @{
  */

/* Exported variables --------------------------------------------------------*/
/** @addtogroup STM32U5x9J_DISCOVERY_HSPI_RAM_Exported_Variables HSPI RAM Exported Variables
  * @{
  */
XSPI_HandleTypeDef hhspi_ram[HSPI_RAM_INSTANCES_NUMBER] = {0};
HSPI_RAM_Ctx_t Hspi_Ram_Ctx[HSPI_RAM_INSTANCES_NUMBER] = {{
    HSPI_ACCESS_NONE,
    BSP_HSPI_RAM_VARIABLE_LATENCY,
    DEFAULT_READ_LATENCY_CODE,
    DEFAULT_WRITE_LATENCY_CODE,
    BSP_HSPI_RAM_HYBRID_BURST,
    BSP_HSPI_RAM_BURST_32_BYTES,
    BSP_HSPI_RAM_IO_X8_MODE
  }
};
/**
  * @}
  */

/* Private constants --------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @defgroup STM32U5x9J_DISCOVERY_HSPI_RAM_Private_Variables HSPI RAM Private Variables
  * @{
  */
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
static uint32_t HspiRam_IsMspCbValid[HSPI_RAM_INSTANCES_NUMBER] = {0};
#endif /* USE_HAL_XSPI_REGISTER_CALLBACKS */
/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/

/** @defgroup STM32U5x9J_DISCOVERY_HSPI_RAM_Private_Functions HSPI RAM Private Functions
  * @{
  */
static void HSPI_RAM_MspInit(XSPI_HandleTypeDef *hhspi);
static void HSPI_RAM_MspDeInit(const XSPI_HandleTypeDef *hhspi);

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @addtogroup STM32U5x9J_DISCOVERY_HSPI_RAM_Exported_Functions HSPI RAM Exported Functions
  * @{
  */

/**
  * @brief  Initializes the HSPI interface.
  * @param  Instance   HSPI Instance
  * @param  Init       HSPI Init structure
  * @retval BSP status
  */
int32_t BSP_HSPI_RAM_Init(uint32_t Instance, BSP_HSPI_RAM_Cfg_t *Init)
{
  int32_t ret;
  MX_HSPI_InitTypeDef hspi_init;

  /* Check if the instance is supported */
  if (Instance >= HSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if the instance is already initialized */
    if (Hspi_Ram_Ctx[Instance].IsInitialized == HSPI_ACCESS_NONE)
    {
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 0)
      /* Msp HSPI initialization */
      HSPI_RAM_MspInit(&hhspi_ram[Instance]);
#else
      /* Register the HSPI MSP Callbacks */
      if (HspiRam_IsMspCbValid[Instance] == 0UL)
      {
        if (BSP_HSPI_RAM_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
        {
          return BSP_ERROR_PERIPH_FAILURE;
        }
      }
#endif /* USE_HAL_XSPI_REGISTER_CALLBACKS */

      /* Fill config structure */
      hspi_init.ClockPrescaler = 0; /* HSPI clock = 160MHz / ClockPrescaler = 160MHz */
      hspi_init.MemorySize     = (uint32_t)POSITION_VAL(APS512XX_RAM_SIZE);
      hspi_init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;

      /* STM32 HSPI interface initialization */
      if (MX_HSPI_RAM_Init(&hhspi_ram[Instance], &hspi_init) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      /* Reset the memory */
      else if (APS512XX_Reset(&hhspi_ram[Instance]) != 0)/*APS512XX_OK*/
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      /* Configure the memory */
      else if (BSP_HSPI_RAM_Config16BitsOctalRAM(Instance, Init) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        Hspi_Ram_Ctx[Instance].IsInitialized = HSPI_ACCESS_INDIRECT;
        ret = BSP_ERROR_NONE;
      }
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  De-Initializes the HSPI interface.
  * @param  Instance   HSPI Instance
  * @retval BSP status
  */
int32_t BSP_HSPI_RAM_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= HSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if the instance is already initialized */
    if (Hspi_Ram_Ctx[Instance].IsInitialized != HSPI_ACCESS_NONE)
    {
      /* Disable Memory mapped mode */
      if (Hspi_Ram_Ctx[Instance].IsInitialized == HSPI_ACCESS_MMP)
      {
        if (BSP_HSPI_RAM_DisableMemoryMappedMode(Instance) != BSP_ERROR_NONE)
        {
          return BSP_ERROR_COMPONENT_FAILURE;
        }
      }

      /* Set default Hspi_Ram_Ctx values */
      Hspi_Ram_Ctx[Instance].IsInitialized    = HSPI_ACCESS_NONE;
      Hspi_Ram_Ctx[Instance].LatencyType      = BSP_HSPI_RAM_VARIABLE_LATENCY;
      Hspi_Ram_Ctx[Instance].ReadLatencyCode  = DEFAULT_READ_LATENCY_CODE;
      Hspi_Ram_Ctx[Instance].WriteLatencyCode = DEFAULT_WRITE_LATENCY_CODE;
      Hspi_Ram_Ctx[Instance].BurstType        = BSP_HSPI_RAM_HYBRID_BURST;
      Hspi_Ram_Ctx[Instance].BurstLength      = BSP_HSPI_RAM_BURST_32_BYTES;
      Hspi_Ram_Ctx[Instance].IOMode           = BSP_HSPI_RAM_IO_X8_MODE;

#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 0)
      HSPI_RAM_MspDeInit(&hhspi_ram[Instance]);
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 0) */

      /* Call the DeInit function to reset the driver */
      if (HAL_XSPI_DeInit(&hhspi_ram[Instance]) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }
  }

  /* Return BSP status */
  return ret;
}


/**
  * @brief  Initializes the HSPI interface.
  * @param  hhspi          HSPI handle
  * @param  Init           HSPI config structure
  * @retval BSP status
  */
__weak HAL_StatusTypeDef MX_HSPI_RAM_Init(XSPI_HandleTypeDef *hhspi, MX_HSPI_InitTypeDef *Init)
{
  HAL_StatusTypeDef status;
  uint32_t hspi_clk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_HSPI);

  /* HSPI initialization */
  hhspi->Instance = HSPI1;

  hhspi->Init.FifoThresholdByte       = 2U;
  hhspi->Init.MemoryMode              = HAL_XSPI_SINGLE_MEM;
  hhspi->Init.MemoryType              = HAL_XSPI_MEMTYPE_APMEM_16BITS;
  hhspi->Init.MemorySize              = Init->MemorySize;
  hhspi->Init.ChipSelectHighTimeCycle = 1U;
  hhspi->Init.FreeRunningClock        = HAL_XSPI_FREERUNCLK_DISABLE;
  hhspi->Init.ClockMode               = HAL_XSPI_CLOCK_MODE_0;
  hhspi->Init.WrapSize                = HAL_XSPI_WRAP_32_BYTES;
  hhspi->Init.ClockPrescaler          = Init->ClockPrescaler;
  hhspi->Init.SampleShifting          = Init->SampleShifting;
  hhspi->Init.DelayHoldQuarterCycle   = HAL_XSPI_DHQC_DISABLE;
  hhspi->Init.ChipSelectBoundary      = 11U;
  hhspi->Init.DelayBlockBypass        = HAL_XSPI_DELAY_BLOCK_ON;
  /* tCEM = 2 us max => REFRESH+4 clock cycles for read */
  hhspi->Init.Refresh                 = ((2U * (hspi_clk / (Init->ClockPrescaler + 1U))) / 1000000U) - 4U;
  hhspi->Init.MaxTran                 = 0U;

  status = HAL_XSPI_Init(hhspi);

  return status;
}

#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
/**
  * @brief Default BSP HSPI Msp Callbacks
  * @param Instance      HSPI Instance
  * @retval BSP status
  */
int32_t BSP_HSPI_RAM_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= HSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_XSPI_RegisterCallback(&hhspi_ram[Instance], HAL_XSPI_MSP_INIT_CB_ID, HSPI_RAM_MspInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_XSPI_RegisterCallback(&hhspi_ram[Instance], HAL_XSPI_MSP_DEINIT_CB_ID, HSPI_RAM_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      HspiRam_IsMspCbValid[Instance] = 1U;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief BSP HSPI Msp Callback registering
  * @param Instance     HSPI Instance
  * @param CallBacks    pointer to MspInit/MspDeInit callbacks functions
  * @retval BSP status
  */
int32_t BSP_HSPI_RAM_RegisterMspCallbacks(uint32_t Instance, BSP_HSPI_Cb_t *CallBacks)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= HSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_XSPI_RegisterCallback(&hhspi_ram[Instance], HAL_XSPI_MSP_INIT_CB_ID, CallBacks->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_XSPI_RegisterCallback(&hhspi_ram[Instance], HAL_XSPI_MSP_DEINIT_CB_ID,
                                       CallBacks->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      HspiRam_IsMspCbValid[Instance] = 1U;
    }
  }

  /* Return BSP status */
  return ret;
}
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Reads an amount of data from the 16-bits Octal RAM memory.
  * @param  Instance  HSPI instance
  * @param  pData     Pointer to data to be read
  * @param  ReadAddr  Read start address
  * @param  Size      Size of data to read
  * @retval BSP status
  */
int32_t BSP_HSPI_RAM_Read(uint32_t Instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }

  else
  {
    if (APS512XX_Read(&hhspi_ram[Instance], pData, ReadAddr, Size,
                      (uint32_t)APS512XX_READ_LATENCY((uint32_t)(Hspi_Ram_Ctx[Instance].ReadLatencyCode),
                                                      (uint32_t)(Hspi_Ram_Ctx[Instance].LatencyType)),
                      (uint32_t)(Hspi_Ram_Ctx[Instance].IOMode),
                      (uint32_t)(Hspi_Ram_Ctx[Instance].BurstType)) != (int32_t)APS512XX_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Reads an amount of data from the 16-bits Octal RAM memory in DMA mode.
  * @param  Instance  HSPI instance
  * @param  pData     Pointer to data to be read
  * @param  ReadAddr  Read start address
  * @param  Size      Size of data to read
  * @retval BSP status
  */
int32_t BSP_HSPI_RAM_Read_DMA(uint32_t Instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (APS512XX_Read_DMA(&hhspi_ram[Instance], pData, ReadAddr, Size,
                          (uint32_t)APS512XX_READ_LATENCY((uint32_t)(Hspi_Ram_Ctx[Instance].ReadLatencyCode),
                                                          (uint32_t)(Hspi_Ram_Ctx[Instance].LatencyType)),
                          (uint32_t)(Hspi_Ram_Ctx[Instance].IOMode),
                          (uint32_t)(Hspi_Ram_Ctx[Instance].BurstType)) != (int32_t)APS512XX_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Writes an amount of data to the 16-bits Octal RAM memory.
  * @param  Instance  HSPI instance
  * @param  pData     Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size      Size of data to write
  * @retval BSP status
  */
int32_t BSP_HSPI_RAM_Write(uint32_t Instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }

  else
  {
    if (APS512XX_Write(&hhspi_ram[Instance], pData, WriteAddr, Size,
                       (uint32_t)APS512XX_WRITE_LATENCY((uint32_t)(Hspi_Ram_Ctx[Instance].WriteLatencyCode)),
                       (uint32_t)(Hspi_Ram_Ctx[Instance].IOMode),
                       (uint32_t)(Hspi_Ram_Ctx[Instance].BurstType)) != (int32_t)APS512XX_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Writes an amount of data to the 16-bits Octal RAM memory in DMA mode.
  * @param  Instance  HSPI instance
  * @param  pData     Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size      Size of data to write
  * @retval BSP status
  */
int32_t BSP_HSPI_RAM_Write_DMA(uint32_t Instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (APS512XX_Write_DMA(&hhspi_ram[Instance], pData, WriteAddr, Size,
                           (uint32_t)APS512XX_WRITE_LATENCY((uint32_t)(Hspi_Ram_Ctx[Instance].WriteLatencyCode)),
                           (uint32_t)(Hspi_Ram_Ctx[Instance].IOMode),
                           (uint32_t)(Hspi_Ram_Ctx[Instance].BurstType)) != (int32_t)APS512XX_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Configure the HSPI in memory-mapped mode
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
int32_t BSP_HSPI_RAM_EnableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= HSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (APS512XX_EnableMemoryMappedMode(&hhspi_ram[Instance], (uint32_t)APS512XX_READ_LATENCY((uint32_t)\
                                        ((uint32_t)(Hspi_Ram_Ctx[Instance].ReadLatencyCode)),
                                        (uint32_t)(Hspi_Ram_Ctx[Instance].LatencyType)),
                                        APS512XX_WRITE_LATENCY((uint32_t)\
                                                               (Hspi_Ram_Ctx[Instance].WriteLatencyCode)),
                                        (uint32_t)(Hspi_Ram_Ctx[Instance].IOMode),
                                        (uint32_t)(Hspi_Ram_Ctx[Instance].BurstType)) != (int32_t)APS512XX_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else /* Update HSPI context if all operations are well done */
    {
      Hspi_Ram_Ctx[Instance].IsInitialized = HSPI_ACCESS_MMP;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Exit form memory-mapped mode
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
int32_t BSP_HSPI_RAM_DisableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t tickstart = HAL_GetTick();

  /* Check if the instance is supported */
  if (Instance >= HSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (Hspi_Ram_Ctx[Instance].IsInitialized != HSPI_ACCESS_MMP)
    {
      ret = BSP_ERROR_HSPI_MMP_UNLOCK_FAILURE;
    }
    /* Abort MMP back to indirect mode */
    else if (HAL_XSPI_Abort(&hhspi_ram[Instance]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    /* Update HSPI 16-bits Octal RAM context if all operations are well done */
    else
    {
      /* Wait until flag is in expected state */
      while ((HAL_XSPI_GET_FLAG(&hhspi_ram[Instance], HAL_XSPI_FLAG_BUSY)) != RESET)
      {
        /* Check for the Timeout */
        if (((HAL_GetTick() - tickstart) > hhspi_ram[Instance].Timeout) || (hhspi_ram[Instance].Timeout == 0U))
        {
          ret = BSP_ERROR_PERIPH_FAILURE;
          break;
        }
      }

      if (ret == BSP_ERROR_NONE)
      {
        /* Configure CR register with functional mode as indirect mode*/
        MODIFY_REG(hhspi_ram[Instance].Instance->CR, (HSPI_CR_FMODE), 0U);
        Hspi_Ram_Ctx[Instance].IsInitialized = HSPI_ACCESS_INDIRECT;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set 16-bits Octal RAM to desired configuration. And this instance becomes current instance.
  *         If current instance running at MMP mode then this function doesn't work.
  *         Indirect -> Indirect
  * @param  Instance    HSPI instance
  * @param  Cfg         pointer to the RAM config
  * @retval BSP status
  */
int32_t BSP_HSPI_RAM_Config16BitsOctalRAM(uint32_t Instance, BSP_HSPI_RAM_Cfg_t *Cfg)
{
  int32_t ret = BSP_ERROR_NONE;
  uint8_t reg[2];

  /* Check if the instance is supported */
  if ((Instance >= HSPI_RAM_INSTANCES_NUMBER)
      || (Cfg->BurstLength == APS512XX_BURST_2_KBYTES))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if MMP mode locked ***********************************************/
    if (Hspi_Ram_Ctx[Instance].IsInitialized == HSPI_ACCESS_MMP)
    {
      ret = BSP_ERROR_HSPI_MMP_LOCK_FAILURE;
    }
    else
    {
      if ((Hspi_Ram_Ctx[Instance].IsInitialized  == HSPI_ACCESS_NONE) ||
          (Hspi_Ram_Ctx[Instance].LatencyType     != Cfg->LatencyType)  ||
          (Hspi_Ram_Ctx[Instance].ReadLatencyCode != Cfg->ReadLatencyCode))
      {
        /* Reading the configuration of Mode Register 0 ***********************/
        if (APS512XX_ReadReg(&hhspi_ram[Instance], APS512XX_MR0_ADDRESS, reg,
                             (uint32_t)APS512XX_READ_REG_LATENCY((uint32_t)(Hspi_Ram_Ctx[Instance].ReadLatencyCode)))\
            != (int32_t)APS512XX_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          /* Configure the 16-bits Octal RAM memory ***************************/
          MODIFY_REG(reg[0], ((uint8_t)APS512XX_MR0_LATENCY_TYPE | (uint8_t)APS512XX_MR0_READ_LATENCY_CODE |
                              (uint8_t)APS512XX_MR0_DRIVE_STRENGTH),
                     ((uint8_t)(Cfg->LatencyType) | (uint8_t)(Cfg->ReadLatencyCode) | (uint8_t)CONF_HSPI_DS));

          if (APS512XX_WriteReg(&hhspi_ram[Instance], APS512XX_MR0_ADDRESS, reg[0]) != APS512XX_OK)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          /* Configure the peripheral *****************************************/
        }
      }

      if (ret == BSP_ERROR_NONE)
      {
        if ((Hspi_Ram_Ctx[Instance].IsInitialized   == HSPI_ACCESS_NONE) ||
            (Hspi_Ram_Ctx[Instance].WriteLatencyCode != Cfg->WriteLatencyCode))
        {
          /* Reading the configuration of Mode Register 4 ***********************/
          if (APS512XX_ReadReg(&hhspi_ram[Instance], APS512XX_MR4_ADDRESS, reg,
                               (uint32_t)APS512XX_READ_REG_LATENCY((uint32_t)(Hspi_Ram_Ctx[Instance].ReadLatencyCode)))\
              != (int32_t)APS512XX_OK)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            /* Configure the 16-bits Octal RAM memory ***************************/
            MODIFY_REG(reg[0], (uint8_t)(APS512XX_MR4_WRITE_LATENCY_CODE | APS512XX_MR4_RF_RATE | APS512XX_MR4_PASR),
                       ((uint8_t)Cfg->WriteLatencyCode | CONF_HSPI_RF | CONF_HSPI_PASR));

            if (APS512XX_WriteReg(&hhspi_ram[Instance], APS512XX_MR4_ADDRESS, reg[0]) != APS512XX_OK)
            {
              ret = BSP_ERROR_COMPONENT_FAILURE;
            }
          }
        }
      }

      if (ret == BSP_ERROR_NONE)
      {
        if ((Hspi_Ram_Ctx[Instance].IsInitialized == HSPI_ACCESS_NONE)  ||
            (Hspi_Ram_Ctx[Instance].BurstType      != Cfg->BurstType)   ||
            (Hspi_Ram_Ctx[Instance].BurstLength    != Cfg->BurstLength) ||
            (Hspi_Ram_Ctx[Instance].IOMode         != Cfg->IOMode))
        {
          /* Reading the configuration of Mode Register 8 ***********************/
          if (APS512XX_ReadReg(&hhspi_ram[Instance], APS512XX_MR8_ADDRESS, reg,
                               (uint32_t)APS512XX_READ_REG_LATENCY((uint32_t)\
                                                                   (Hspi_Ram_Ctx[Instance].ReadLatencyCode)))\
              != (int32_t)APS512XX_OK)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            /* Configure the 16-bits Octal RAM memory ***************************/
            MODIFY_REG(reg[0], ((uint8_t)APS512XX_MR8_X8_X16 | (uint8_t)APS512XX_MR8_BL),
                       ((uint8_t)(Cfg->IOMode) | (uint8_t)(Cfg->BurstLength)));

            if (APS512XX_WriteReg(&hhspi_ram[Instance], APS512XX_MR8_ADDRESS, reg[0]) != APS512XX_OK)
            {
              ret = BSP_ERROR_COMPONENT_FAILURE;
            }
            else
            {
              /* Configure the peripheral *****************************************/
              if (Cfg->BurstType == BSP_HSPI_RAM_HYBRID_BURST)
              {
                switch (Cfg->BurstLength)
                {
                  case APS512XX_MR8_BL_16_BYTES :
                    hhspi_ram[Instance].Init.WrapSize = HAL_XSPI_WRAP_16_BYTES;
                    break;
                  case APS512XX_MR8_BL_32_BYTES :
                    hhspi_ram[Instance].Init.WrapSize = HAL_XSPI_WRAP_32_BYTES;
                    break;
                  case APS512XX_MR8_BL_64_BYTES :
                    hhspi_ram[Instance].Init.WrapSize = HAL_XSPI_WRAP_64_BYTES;
                    break;
                  default :
                    break;
                }
              }
              else
              {
                hhspi_ram[Instance].Init.WrapSize = HAL_XSPI_WRAP_NOT_SUPPORTED;
              }

              if (Cfg->IOMode == BSP_HSPI_RAM_IO_X16_MODE)
              {
                hhspi_ram[Instance].Init.MemoryType = HAL_XSPI_MEMTYPE_APMEM_16BITS;
              }
              else
              {
                hhspi_ram[Instance].Init.MemoryType = HAL_XSPI_MEMTYPE_APMEM;
              }

              if (HAL_XSPI_DeInit(&hhspi_ram[Instance]) != HAL_OK)
              {
                ret = BSP_ERROR_PERIPH_FAILURE;
              }

              if (HAL_XSPI_Init(&hhspi_ram[Instance]) != HAL_OK)
              {
                ret = BSP_ERROR_PERIPH_FAILURE;
              }
            }
          }
        }
      }

      /* Update HSPI context if all operations are well done */
      if (ret == BSP_ERROR_NONE)
      {
        /* Update current status parameter *****************************************/
        Hspi_Ram_Ctx[Instance].IsInitialized    = HSPI_ACCESS_INDIRECT;
        Hspi_Ram_Ctx[Instance].LatencyType      = Cfg->LatencyType;
        Hspi_Ram_Ctx[Instance].ReadLatencyCode  = Cfg->ReadLatencyCode;
        Hspi_Ram_Ctx[Instance].WriteLatencyCode = Cfg->WriteLatencyCode;
        Hspi_Ram_Ctx[Instance].BurstType        = Cfg->BurstType;
        Hspi_Ram_Ctx[Instance].BurstLength      = Cfg->BurstLength;
        Hspi_Ram_Ctx[Instance].IOMode           = Cfg->IOMode;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function enter the 16-bits Octal RAM memory in deep power down mode.
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
int32_t BSP_HSPI_RAM_EnterDeepPowerDown(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (APS512XX_EnterPowerDown(&hhspi_ram[Instance]) != APS512XX_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* ---         Memory enter deep power down as soon nCS goes high        --- */
    /* --- At least 500us should be respected before leaving deep power down --- */

    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function leave the 16-bits Octal RAM memory from deep power down mode.
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
int32_t BSP_HSPI_RAM_LeaveDeepPowerDown(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_RAM_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (APS512XX_LeavePowerDown(&hhspi_ram[Instance]) != APS512XX_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* --- A dummy command is sent to the memory, as the nCS should be low for at least 60 ns  --- */
    /* ---                  Memory takes 150us max to leave deep power down                    --- */

    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Handles HSPI 16-bits Octal RAM DMA transfer interrupt request.
  * @param  Instance HSPI instance
  * @retval None
  */
void BSP_HSPI_RAM_DMA_IRQHandler(uint32_t Instance)
{
  HAL_DMA_IRQHandler(hhspi_ram[Instance].hdmatx);
  HAL_DMA_IRQHandler(hhspi_ram[Instance].hdmarx);
}

/**
  * @brief  Handles HSPI 16-bits Octal RAM interrupt request.
  * @param  Instance HSPI instance
  * @retval None
  */
void BSP_HSPI_RAM_IRQHandler(uint32_t Instance)
{
  HAL_XSPI_IRQHandler(&hhspi_ram[Instance]);
}
/**
  * @}
  */

/** @addtogroup STM32U5x9J_DISCOVERY_HSPI_RAM_Private_Functions HSPI RAM Private Functions
  * @{
  */

/**
  * @brief  Initializes the HSPI MSP.
  * @param  hhspi HSPI handle
  * @retval None
  */
static void HSPI_RAM_MspInit(XSPI_HandleTypeDef *hhspi)
{
  static DMA_HandleTypeDef hdmatx, hdmarx;
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();
  SET_BIT(PWR->SVMCR, PWR_SVMCR_IO2SV);

  /* Enable the HSPI memory interface, DMA and GPIO clocks */
  HSPI_CLK_ENABLE();
  HSPI_RAM_DMAx_CLK_ENABLE();
  HSPI_CLK_GPIO_CLK_ENABLE();
  HSPI_DQS0_GPIO_CLK_ENABLE();
  HSPI_DQS1_GPIO_CLK_ENABLE();
  HSPI_CS_GPIO_CLK_ENABLE();
  HSPI_D0_GPIO_CLK_ENABLE();
  HSPI_D1_GPIO_CLK_ENABLE();
  HSPI_D2_GPIO_CLK_ENABLE();
  HSPI_D3_GPIO_CLK_ENABLE();
  HSPI_D4_GPIO_CLK_ENABLE();
  HSPI_D5_GPIO_CLK_ENABLE();
  HSPI_D6_GPIO_CLK_ENABLE();
  HSPI_D7_GPIO_CLK_ENABLE();
  HSPI_D8_GPIO_CLK_ENABLE();
  HSPI_D9_GPIO_CLK_ENABLE();
  HSPI_D10_GPIO_CLK_ENABLE();
  HSPI_D11_GPIO_CLK_ENABLE();
  HSPI_D12_GPIO_CLK_ENABLE();
  HSPI_D13_GPIO_CLK_ENABLE();
  HSPI_D14_GPIO_CLK_ENABLE();
  HSPI_D15_GPIO_CLK_ENABLE();

  /*  Activate HSLV  */
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_CS_GPIO_PORT, HSPI_CS_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_DQS0_GPIO_PORT, HSPI_DQS0_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_DQS1_GPIO_PORT, HSPI_DQS1_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_CLK_GPIO_PORT, HSPI_CLK_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_NCLK_GPIO_PORT, HSPI_NCLK_PIN);

  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D0_GPIO_PORT, HSPI_D0_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D1_GPIO_PORT, HSPI_D1_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D2_GPIO_PORT, HSPI_D2_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D3_GPIO_PORT, HSPI_D3_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D4_GPIO_PORT, HSPI_D4_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D5_GPIO_PORT, HSPI_D5_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D6_GPIO_PORT, HSPI_D6_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D7_GPIO_PORT, HSPI_D7_PIN);

  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D8_GPIO_PORT, HSPI_D8_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D9_GPIO_PORT, HSPI_D9_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D10_GPIO_PORT, HSPI_D10_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D11_GPIO_PORT, HSPI_D11_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D12_GPIO_PORT, HSPI_D12_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D13_GPIO_PORT, HSPI_D13_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D14_GPIO_PORT, HSPI_D14_PIN);
  HAL_GPIO_EnableHighSPeedLowVoltage(HSPI_D15_GPIO_PORT, HSPI_D15_PIN);

  /* Reset the HSPI memory interface */
  HSPI_FORCE_RESET();
  HSPI_RELEASE_RESET();

  /* HSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = HSPI_CS_PIN_AF;
  HAL_GPIO_Init(HSPI_CS_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI DQS0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_DQS0_PIN;
  GPIO_InitStruct.Alternate = HSPI_DQS0_PIN_AF;
  HAL_GPIO_Init(HSPI_DQS0_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI DQS1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_DQS1_PIN;
  GPIO_InitStruct.Alternate = HSPI_DQS1_PIN_AF;
  HAL_GPIO_Init(HSPI_DQS1_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI CLK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_CLK_PIN;
  GPIO_InitStruct.Alternate = HSPI_CLK_PIN_AF;
  HAL_GPIO_Init(HSPI_CLK_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D0_PIN;
  GPIO_InitStruct.Alternate = HSPI_D0_PIN_AF;
  HAL_GPIO_Init(HSPI_D0_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D1_PIN;
  GPIO_InitStruct.Alternate = HSPI_D1_PIN_AF;
  HAL_GPIO_Init(HSPI_D1_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D2_PIN;
  GPIO_InitStruct.Alternate = HSPI_D2_PIN_AF;
  HAL_GPIO_Init(HSPI_D2_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D3_PIN;
  GPIO_InitStruct.Alternate = HSPI_D3_PIN_AF;
  HAL_GPIO_Init(HSPI_D3_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D4 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D4_PIN;
  GPIO_InitStruct.Alternate = HSPI_D4_PIN_AF;
  HAL_GPIO_Init(HSPI_D4_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D5 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D5_PIN;
  GPIO_InitStruct.Alternate = HSPI_D5_PIN_AF;
  HAL_GPIO_Init(HSPI_D5_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D6 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D6_PIN;
  GPIO_InitStruct.Alternate = HSPI_D6_PIN_AF;
  HAL_GPIO_Init(HSPI_D6_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D7 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D7_PIN;
  GPIO_InitStruct.Alternate = HSPI_D7_PIN_AF;
  HAL_GPIO_Init(HSPI_D7_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D8 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D8_PIN;
  GPIO_InitStruct.Alternate = HSPI_D8_PIN_AF;
  HAL_GPIO_Init(HSPI_D8_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D9 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D9_PIN;
  GPIO_InitStruct.Alternate = HSPI_D9_PIN_AF;
  HAL_GPIO_Init(HSPI_D9_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D10 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D10_PIN;
  GPIO_InitStruct.Alternate = HSPI_D10_PIN_AF;
  HAL_GPIO_Init(HSPI_D10_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D11 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D11_PIN;
  GPIO_InitStruct.Alternate = HSPI_D11_PIN_AF;
  HAL_GPIO_Init(HSPI_D11_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D12 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D12_PIN;
  GPIO_InitStruct.Alternate = HSPI_D12_PIN_AF;
  HAL_GPIO_Init(HSPI_D12_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D13 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D13_PIN;
  GPIO_InitStruct.Alternate = HSPI_D13_PIN_AF;
  HAL_GPIO_Init(HSPI_D13_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D14 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D14_PIN;
  GPIO_InitStruct.Alternate = HSPI_D14_PIN_AF;
  HAL_GPIO_Init(HSPI_D14_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D15 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_D15_PIN;
  GPIO_InitStruct.Alternate = HSPI_D15_PIN_AF;
  HAL_GPIO_Init(HSPI_D15_GPIO_PORT, &GPIO_InitStruct);

  hdmatx.Init.Request = GPDMA1_REQUEST_HSPI1;
  hdmatx.Instance = GPDMA1_Channel12;
  HAL_NVIC_SetPriority(GPDMA1_Channel12_IRQn, 0x00, 0);
  HAL_NVIC_EnableIRQ(GPDMA1_Channel12_IRQn);

  /* Configure the HSPI DMA transmit */
  hdmatx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdmatx.Init.SrcInc              = DMA_SINC_INCREMENTED;
  hdmatx.Init.DestInc             = DMA_DINC_FIXED;
  hdmatx.Init.Priority            = DMA_HIGH_PRIORITY;
  hdmatx.Init.SrcBurstLength      = 1;
  hdmatx.Init.DestBurstLength     = 1;
  hdmatx.Init.TransferEventMode   = DMA_TCEM_BLOCK_TRANSFER;
  hdmatx.Init.SrcDataWidth        = DMA_SRC_DATAWIDTH_WORD ;
  hdmatx.Init.DestDataWidth       = DMA_DEST_DATAWIDTH_WORD  ;

  /* Enable the GPDMA clock */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  __HAL_LINKDMA(hhspi, hdmatx, hdmatx);

  /* Initialize the DMA channel */
  (void)HAL_DMA_Init(&hdmatx);

  hdmarx.Init.Request = GPDMA1_REQUEST_HSPI1;
  hdmarx.Instance = GPDMA1_Channel7;
  HAL_NVIC_SetPriority(GPDMA1_Channel7_IRQn, 0x00, 0);
  HAL_NVIC_EnableIRQ(GPDMA1_Channel7_IRQn);

  /* Configure the HSPI DMA receive */
  hdmarx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdmarx.Init.SrcInc              = DMA_SINC_FIXED;
  hdmarx.Init.DestInc             = DMA_DINC_INCREMENTED;
  hdmarx.Init.Priority            = DMA_HIGH_PRIORITY;
  hdmarx.Init.SrcBurstLength      = 1;
  hdmarx.Init.DestBurstLength     = 1;
  hdmarx.Init.TransferEventMode   = DMA_TCEM_BLOCK_TRANSFER;
  hdmarx.Init.SrcDataWidth        = DMA_SRC_DATAWIDTH_WORD ;
  hdmarx.Init.DestDataWidth       = DMA_DEST_DATAWIDTH_WORD  ;

  /* Enable the GPDMA clock */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  __HAL_LINKDMA(hhspi, hdmarx, hdmarx);

  /* Initialize the DMA channel */
  (void)HAL_DMA_Init(&hdmarx);

  /* Enable and set priority of the HSPI and DMA interrupts */
  HAL_NVIC_SetPriority(HSPI1_IRQn, BSP_HSPI_RAM_IT_PRIORITY, 0);
  HAL_NVIC_SetPriority(HSPI_RAM_DMAx_IRQn, BSP_HSPI_RAM_DMA_IT_PRIORITY, 0);

  HAL_NVIC_EnableIRQ(HSPI1_IRQn);
  HAL_NVIC_EnableIRQ(HSPI_RAM_DMAx_IRQn);
}

/**
  * @brief  De-Initializes the HSPI MSP.
  * @param  hhspi HSPI handle
  * @retval None
  */
static void HSPI_RAM_MspDeInit(const XSPI_HandleTypeDef *hhspi)
{
  static DMA_HandleTypeDef dma_handle;

  /* hhspi unused argument(s) compilation warning */
  UNUSED(hhspi);

  /* Disable DMA and HSPI interrupts */
  HAL_NVIC_DisableIRQ(HSPI_RAM_DMAx_IRQn);
  HAL_NVIC_DisableIRQ(HSPI1_IRQn);

  /* De-configure the HSPI DMA */
  dma_handle.Instance = HSPI_RAM_DMAx_CHANNEL;
  (void) HAL_DMA_DeInit(&dma_handle);

  /* HSPI GPIO pins de-configuration  */
  HAL_GPIO_DeInit(HSPI_CLK_GPIO_PORT, HSPI_CLK_PIN);
  HAL_GPIO_DeInit(HSPI_NCLK_GPIO_PORT, HSPI_NCLK_PIN);
  HAL_GPIO_DeInit(HSPI_DQS0_GPIO_PORT, HSPI_DQS0_PIN);
  HAL_GPIO_DeInit(HSPI_DQS1_GPIO_PORT, HSPI_DQS1_PIN);
  HAL_GPIO_DeInit(HSPI_CS_GPIO_PORT, HSPI_CS_PIN);
  HAL_GPIO_DeInit(HSPI_D0_GPIO_PORT, HSPI_D0_PIN);
  HAL_GPIO_DeInit(HSPI_D1_GPIO_PORT, HSPI_D1_PIN);
  HAL_GPIO_DeInit(HSPI_D2_GPIO_PORT, HSPI_D2_PIN);
  HAL_GPIO_DeInit(HSPI_D3_GPIO_PORT, HSPI_D3_PIN);
  HAL_GPIO_DeInit(HSPI_D4_GPIO_PORT, HSPI_D4_PIN);
  HAL_GPIO_DeInit(HSPI_D5_GPIO_PORT, HSPI_D5_PIN);
  HAL_GPIO_DeInit(HSPI_D6_GPIO_PORT, HSPI_D6_PIN);
  HAL_GPIO_DeInit(HSPI_D7_GPIO_PORT, HSPI_D7_PIN);
  HAL_GPIO_DeInit(HSPI_D8_GPIO_PORT, HSPI_D8_PIN);
  HAL_GPIO_DeInit(HSPI_D9_GPIO_PORT, HSPI_D9_PIN);
  HAL_GPIO_DeInit(HSPI_D10_GPIO_PORT, HSPI_D10_PIN);
  HAL_GPIO_DeInit(HSPI_D11_GPIO_PORT, HSPI_D11_PIN);
  HAL_GPIO_DeInit(HSPI_D12_GPIO_PORT, HSPI_D12_PIN);
  HAL_GPIO_DeInit(HSPI_D13_GPIO_PORT, HSPI_D13_PIN);
  HAL_GPIO_DeInit(HSPI_D14_GPIO_PORT, HSPI_D14_PIN);
  HAL_GPIO_DeInit(HSPI_D15_GPIO_PORT, HSPI_D15_PIN);

  /* Reset the HSPI memory interface */
  HSPI_FORCE_RESET();
  HSPI_RELEASE_RESET();

  /* Disable the HSPI memory interface clock */
  HSPI_CLK_DISABLE();
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
