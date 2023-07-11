/**
  ******************************************************************************
  * @file    stm32u5g9j_discovery_hspi.c
  * @author  MCD Application Team
  * @brief   This file includes a standard driver for the MX66UW1G45G
  *          HSPI memories mounted on the STM32U5G9J-DK board.
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
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
  [..]
   (#) This driver is used to drive the MX66UW1G45G Octal NOR
       external memories mounted on STM32U5G9J-DK board.

   (#) This driver need specific component driver (MX66UW1G45G) to be included with.

   (#) MX66UW1G45G Initialization steps:
       (++) Initialize the HSPI external memory using the BSP_HSPI_NOR_Init() function. This
            function includes the MSP layer hardware resources initialization and the
            HSPI interface with the external memory.

   (#) MX66UW1G45G Octal NOR memory operations
       (++) HSPI memory can be accessed with read/write operations once it is
            initialized.
            Read/write operation can be performed with AHB access using the functions
            BSP_HSPI_NOR_Read()/BSP_HSPI_NOR_Write().
       (++) The function BSP_HSPI_NOR_GetInfo() returns the configuration of the HSPI memory.
            (see the HSPI memory data sheet)
       (++) Perform erase block operation using the function BSP_HSPI_NOR_Erase_Block() and by
            specifying the block address. You can perform an erase operation of the whole
            chip by calling the function BSP_HSPI_NOR_Erase_Chip().
       (++) The function BSP_HSPI_NOR_GetStatus() returns the current status of the HSPI memory.
            (see the HSPI memory data sheet)
       (++) The memory access can be configured in memory-mapped mode with the call of
            function BSP_HSPI_NOR_EnableMemoryMapped(). To go back in indirect mode, the
            function BSP_HSPI_NOR_DisableMemoryMapped() should be used.
       (++) The erase operation can be suspend and resume with using functions
            BSP_HSPI_NOR_SuspendErase() and BSP_HSPI_NOR_ResumeErase()
       (++) It is possible to put the memory in deep power-down mode to reduce its consumption.
            For this, the function BSP_HSPI_NOR_EnterDeepPowerDown() should be called. To leave
            the deep power-down mode, the function BSP_HSPI_NOR_LeaveDeepPowerDown() should be called.
       (++) The function BSP_HSPI_NOR_ReadID() returns the identifier of the memory
            (see the HSPI memory data sheet)
       (++) The configuration of the interface between peripheral and memory is done by
            the function BSP_HSPI_NOR_ConfigFlash(), three modes are possible :
            - SPI : instruction, address and data on one line
            - STR OPI : instruction, address and data on eight lines with sampling on one edge of clock
            - DTR OPI : instruction, address and data on eight lines with sampling on both edgaes of clock


  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32u5g9j_discovery_hspi.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U5G9J_DK
  * @{
  */

/** @defgroup STM32U5G9J_DK_HSPI HSPI
  * @{
  */

/* Exported variables --------------------------------------------------------*/
/** @addtogroup STM32U5G9J_DK_HSPI_NOR_Exported_Variables Exported Variables
  * @{
  */
static XSPI_HandleTypeDef hhspi_nor[HSPI_NOR_INSTANCES_NUMBER] = {0};
static HSPI_NOR_Ctx_t HSPI_Nor_Ctx[HSPI_NOR_INSTANCES_NUMBER]  = {{
    HSPI_ACCESS_NONE,
    MX66UW1G45G_SPI_MODE,
    MX66UW1G45G_STR_TRANSFER
  }
};
/**
  * @}
  */

/* Private constants --------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @defgroup STM32U5G9J_DK_HSPI_NOR_Private_Variables HSPI_NOR Private Variables
  * @{
  */
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1U)
static uint32_t HSPINor_IsMspCbValid[HSPI_NOR_INSTANCES_NUMBER] = {0};
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 1) */

/**
  * @}
  */



/* Private functions ---------------------------------------------------------*/

/** @defgroup STM32U5G9J_DK_HSPI_NOR_Private_Functions HSPI_NOR Private Functions
  * @{
  */
static void    HSPI_NOR_MspInit(XSPI_HandleTypeDef *hhspi);
static void    HSPI_NOR_MspDeInit(XSPI_HandleTypeDef *hhspi);
static int32_t HSPI_NOR_ResetMemory(uint32_t Instance);
static int32_t HSPI_NOR_EnterDOPIMode(uint32_t Instance);
static int32_t HSPI_NOR_EnterSOPIMode(uint32_t Instance);
static int32_t HSPI_NOR_ExitOPIMode(uint32_t Instance);
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @addtogroup STM32U5G9J_DK_HSPI_NOR_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes the HSPI interface.
  * @param  Instance   HSPI Instance
  * @param  Init       HSPI Init structure
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_Init(uint32_t Instance, BSP_HSPI_NOR_Init_t *Init)
{
  int32_t ret;
  BSP_HSPI_NOR_Info_t pInfo;
  MX_HSPI_InitTypeDef hspi_init;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if the instance is already initialized */
    if (HSPI_Nor_Ctx[Instance].IsInitialized == HSPI_ACCESS_NONE)
    {
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 0U)
      /* Msp HSPI initialization */
      HSPI_NOR_MspInit(&hhspi_nor[Instance]);
#else
      /* Register the HSPI MSP Callbacks */
      if (HSPINor_IsMspCbValid[Instance] == 0UL)
      {
        if (BSP_HSPI_NOR_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
        {
          return BSP_ERROR_PERIPH_FAILURE;
        }
      }
#endif /* USE_HAL_XSPI_REGISTER_CALLBACKS */

      /* Get Flash information of one memory */
      (void)MX66UW1G45G_GetFlashInfo(&pInfo);

      /* Fill config structure */
      hspi_init.ClockPrescaler = 0x01; /* HSPI clock = 128MHz / ClockPrescaler = 64MHz */
      hspi_init.MemorySize     = (uint32_t)POSITION_VAL((uint32_t)pInfo.FlashSize);
      hspi_init.SampleShifting = HAL_XSPI_SAMPLE_SHIFT_NONE;
      hspi_init.TransferRate   = (uint32_t)Init->TransferRate;

      /* STM32 HSPI interface initialization */
      if (MX_HSPI_NOR_Init(&hhspi_nor[Instance], &hspi_init) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      /* HSPI memory reset */
      else if (HSPI_NOR_ResetMemory(Instance) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      /* Check if memory is ready */
      else if (MX66UW1G45G_AutoPollingMemReady(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                               HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      /* Configure the memory */
      else if (BSP_HSPI_NOR_ConfigFlash(Instance, Init->InterfaceMode, Init->TransferRate) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
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
int32_t BSP_HSPI_NOR_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if the instance is already initialized */
    if (HSPI_Nor_Ctx[Instance].IsInitialized != HSPI_ACCESS_NONE)
    {
      /* Disable Memory mapped mode */
      if (HSPI_Nor_Ctx[Instance].IsInitialized == HSPI_ACCESS_MMP)
      {
        if (BSP_HSPI_NOR_DisableMemoryMappedMode(Instance) != BSP_ERROR_NONE)
        {
          return BSP_ERROR_COMPONENT_FAILURE;
        }
      }

      /* Set default HSPI_Nor_Ctx values */
      HSPI_Nor_Ctx[Instance].IsInitialized = HSPI_ACCESS_NONE;
      HSPI_Nor_Ctx[Instance].InterfaceMode = BSP_HSPI_NOR_SPI_MODE;
      HSPI_Nor_Ctx[Instance].TransferRate  = BSP_HSPI_NOR_STR_TRANSFER;

#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 0)
      HSPI_NOR_MspDeInit(&hhspi_nor[Instance]);
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 0) */

      /* Call the DeInit function to reset the driver */
      if (HAL_XSPI_DeInit(&hhspi_nor[Instance]) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
    }
  }

  /* Return BSP status */
  return ret;
}
/**
  * @}
  */

/** @addtogroup STM32U5G9J_DK_HSPI_Exported_Init_Functions
  * @{
  */
/**
  * @brief  Initializes the HSPI interface.
  * @param  hhspi          HSPI handle
  * @param  Init           HSPI config structure
  * @retval BSP status
  */
__weak HAL_StatusTypeDef MX_HSPI_NOR_Init(XSPI_HandleTypeDef *hhspi, MX_HSPI_InitTypeDef *Init)
{
  /* HSPI initialization */
  hhspi->Instance = HSPI1;

  hhspi->Init.FifoThresholdByte       = 1;
  hhspi->Init.MemorySize              = Init->MemorySize; /* 1 GBits */
  hhspi->Init.ChipSelectHighTimeCycle = 2;
  hhspi->Init.FreeRunningClock        = HAL_XSPI_FREERUNCLK_DISABLE;
  hhspi->Init.ClockMode               = HAL_XSPI_CLOCK_MODE_0;
  hhspi->Init.DelayHoldQuarterCycle   = HAL_XSPI_DHQC_DISABLE;
  hhspi->Init.ClockPrescaler          = Init->ClockPrescaler;
  hhspi->Init.SampleShifting          = Init->SampleShifting;
  hhspi->Init.ChipSelectBoundary      = HAL_XSPI_BONDARYOF_NONE;
  hhspi->Init.MemoryMode              = HAL_XSPI_SINGLE_MEM;
  hhspi->Init.WrapSize                = HAL_XSPI_WRAP_NOT_SUPPORTED;

  if (Init->TransferRate == (uint32_t) BSP_HSPI_NOR_DTR_TRANSFER)
  {
    hhspi->Init.MemoryType            = HAL_XSPI_MEMTYPE_MACRONIX;
    hhspi->Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_ENABLE;
  }
  else
  {
    hhspi->Init.MemoryType            = HAL_XSPI_MEMTYPE_MACRONIX;
    hhspi->Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_DISABLE;
  }
  return HAL_XSPI_Init(hhspi);
}
/**
  * @}
  */

/** @addtogroup STM32U5G9J_DK_HSPI_NOR_Exported_Functions
  * @{
  */
#if (USE_HAL_XSPI_REGISTER_CALLBACKS == 1)
/**
  * @brief Default BSP HSPI Msp Callbacks
  * @param Instance      HSPI Instance
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_XSPI_RegisterCallback(&hhspi_nor[Instance], HAL_XSPI_MSP_INIT_CB_ID, HSPI_NOR_MspInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_XSPI_RegisterCallback(&hhspi_nor[Instance], HAL_XSPI_MSP_DEINIT_CB_ID, HSPI_NOR_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      HSPINor_IsMspCbValid[Instance] = 1U;
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
int32_t BSP_HSPI_NOR_RegisterMspCallbacks(uint32_t Instance, BSP_HSPI_Cb_t *CallBacks)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_XSPI_RegisterCallback(&hhspi_nor[Instance], HAL_XSPI_MSP_INIT_CB_ID, CallBacks->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_XSPI_RegisterCallback(&hhspi_nor[Instance],
                                       HAL_XSPI_MSP_DEINIT_CB_ID, CallBacks->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      HSPINor_IsMspCbValid[Instance] = 1U;
    }
  }

  /* Return BSP status */
  return ret;
}
#endif /* (USE_HAL_XSPI_REGISTER_CALLBACKS == 1) */

/**
  * @brief  Reads an amount of data from the HSPI memory.
  * @param  Instance  HSPI instance
  * @param  pData     Pointer to data to be read
  * @param  ReadAddr  Read start address
  * @param  Size      Size of data to read
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_Read(uint32_t Instance, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (HSPI_Nor_Ctx[Instance].TransferRate == BSP_HSPI_NOR_STR_TRANSFER)
    {
      if (MX66UW1G45G_ReadSTR(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                              MX66UW1G45G_4BYTES_SIZE, pData, ReadAddr, Size) != MX66UW1G45G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    else
    {
      if (MX66UW1G45G_ReadDTR(&hhspi_nor[Instance], pData, ReadAddr, Size) != MX66UW1G45G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Writes an amount of data to the HSPI memory.
  * @param  Instance  HSPI instance
  * @param  pData     Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size      Size of data to write
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_Write(uint32_t Instance, uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t end_addr;
  uint32_t current_size;
  uint32_t current_addr;
  uint32_t data_addr;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Calculation of the size between the write address and the end of the page */
    current_size = MX66UW1G45G_PAGE_SIZE - (WriteAddr % MX66UW1G45G_PAGE_SIZE);

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > Size)
    {
      current_size = Size;
    }

    /* Initialize the address variables */
    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;
    data_addr = (uint32_t)pData;

    /* Perform the write page by page */
    do
    {
      /* Check if Flash busy ? */
      if (MX66UW1G45G_AutoPollingMemReady(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                          HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }/* Enable write operations */
      else if (MX66UW1G45G_WriteEnable(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                       HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        if (HSPI_Nor_Ctx[Instance].TransferRate == BSP_HSPI_NOR_STR_TRANSFER)
        {
          /* Issue page program command */
          if (MX66UW1G45G_PageProgram(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                      MX66UW1G45G_4BYTES_SIZE, (uint8_t *)data_addr, current_addr,
                                      current_size) != MX66UW1G45G_OK)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
        }
        else
        {
          /* Issue page program command */
          if (MX66UW1G45G_PageProgramDTR(&hhspi_nor[Instance], (uint8_t *)data_addr, current_addr,
                                         current_size) != MX66UW1G45G_OK)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
        }

        if (ret == BSP_ERROR_NONE)
        {
          /* Configure automatic polling mode to wait for end of program */
          if (MX66UW1G45G_AutoPollingMemReady(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                              HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            /* Update the address and size variables for next page programming */
            current_addr += current_size;
            data_addr += current_size;
            current_size = ((current_addr + MX66UW1G45G_PAGE_SIZE) > end_addr)
                           ? (end_addr - current_addr)
                           : MX66UW1G45G_PAGE_SIZE;
          }
        }
      }
    } while ((current_addr < end_addr) && (ret == BSP_ERROR_NONE));
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Erases the specified block of the HSPI memory.
  * @param  Instance     HSPI instance
  * @param  BlockAddress Block address to erase
  * @param  BlockSize    Erase Block size
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_Erase_Block(uint32_t Instance, uint32_t BlockAddress, BSP_HSPI_NOR_Erase_t BlockSize)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check Flash busy ? */
    if (MX66UW1G45G_AutoPollingMemReady(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                        HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Enable write operations */
    else if (MX66UW1G45G_WriteEnable(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                     HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Issue Block Erase command */
    else if (MX66UW1G45G_BlockErase(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                    HSPI_Nor_Ctx[Instance].TransferRate, MX66UW1G45G_4BYTES_SIZE,
                                    BlockAddress, BlockSize) != MX66UW1G45G_OK)
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
  * @brief  Erases the entire HSPI memory.
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_Erase_Chip(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check Flash busy ? */
    if (MX66UW1G45G_AutoPollingMemReady(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                        HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Enable write operations */
    else if (MX66UW1G45G_WriteEnable(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                     HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Issue Chip erase command */
    else if (MX66UW1G45G_BlockErase(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                    HSPI_Nor_Ctx[Instance].TransferRate, MX66UW1G45G_4BYTES_SIZE, 0,
                                    MX66UW1G45G_ERASE_BULK) != MX66UW1G45G_OK)
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
  * @brief  Reads current status of the HSPI memory.
  * @param  Instance  HSPI instance
  * @retval HSPI memory status: whether busy or not
  */
int32_t BSP_HSPI_NOR_GetStatus(uint32_t Instance)
{
  static uint8_t reg[2];
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (MX66UW1G45G_ReadSecurityRegister(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                         HSPI_Nor_Ctx[Instance].TransferRate, reg) != MX66UW1G45G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Check the value of the register */
    else if ((reg[0] & (MX66UW1G45G_SECR_P_FAIL | MX66UW1G45G_SECR_E_FAIL)) != 0U)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else if ((reg[0] & (MX66UW1G45G_SECR_PSB | MX66UW1G45G_SECR_ESB)) != 0U)
    {
      ret = BSP_ERROR_HSPI_SUSPENDED;
    }
    else if (MX66UW1G45G_ReadStatusRegister(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                            HSPI_Nor_Ctx[Instance].TransferRate, reg) != MX66UW1G45G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }/* Check the value of the register */
    else if ((reg[0] & MX66UW1G45G_SR_WIP) != 0U)
    {
      ret = BSP_ERROR_BUSY;
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
  * @brief  Return the configuration of the HSPI memory.
  * @param  Instance  HSPI instance
  * @param  pInfo     pointer on the configuration structure
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_GetInfo(uint32_t Instance, BSP_HSPI_NOR_Info_t *pInfo)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    (void)MX66UW1G45G_GetFlashInfo(pInfo);
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Configure the HSPI in memory-mapped mode
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_EnableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (HSPI_Nor_Ctx[Instance].TransferRate == BSP_HSPI_NOR_STR_TRANSFER)
    {
      if (MX66UW1G45G_EnableMemoryMappedModeSTR(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                                MX66UW1G45G_4BYTES_SIZE) != MX66UW1G45G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else /* Update HSPI context if all operations are well done */
      {
        HSPI_Nor_Ctx[Instance].IsInitialized = HSPI_ACCESS_MMP;
      }
    }
    else
    {
      if (MX66UW1G45G_EnableMemoryMappedModeDTR(&hhspi_nor[Instance],
                                                HSPI_Nor_Ctx[Instance].InterfaceMode) != MX66UW1G45G_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else /* Update HSPI context if all operations are well done */
      {
        HSPI_Nor_Ctx[Instance].IsInitialized = HSPI_ACCESS_MMP;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Exit form memory-mapped mode
  *         Only 1 Instance can running MMP mode. And it will lock system at this mode.
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_DisableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    if (HSPI_Nor_Ctx[Instance].IsInitialized != HSPI_ACCESS_MMP)
    {
      ret = BSP_ERROR_HSPI_MMP_UNLOCK_FAILURE;
    }/* Abort MMP back to indirect mode */
    else if (HAL_XSPI_Abort(&hhspi_nor[Instance]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else /* Update HSPI NOR context if all operations are well done */
    {
      HSPI_Nor_Ctx[Instance].IsInitialized = HSPI_ACCESS_INDIRECT;
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get flash ID 3 Bytes:
  *         Manufacturer ID, Memory type, Memory density
  * @param  Instance  HSPI instance
  * @param  Id Pointer to flash ID bytes
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_ReadID(uint32_t Instance, uint8_t *Id)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MX66UW1G45G_ReadID(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                              HSPI_Nor_Ctx[Instance].TransferRate, Id) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set Flash to desired Interface mode. And this instance becomes current instance.
  *         If current instance running at MMP mode then this function doesn't work.
  *         Indirect -> Indirect
  * @param  Instance  HSPI instance
  * @param  Mode      HSPI mode
  * @param  Rate      HSPI transfer rate
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_ConfigFlash(uint32_t Instance, BSP_HSPI_NOR_Interface_t Mode, BSP_HSPI_NOR_Transfer_t Rate)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Check if MMP mode locked ************************************************/
    if (HSPI_Nor_Ctx[Instance].IsInitialized == HSPI_ACCESS_MMP)
    {
      ret = BSP_ERROR_HSPI_MMP_LOCK_FAILURE;
    }
    else
    {
      /* Setup Flash interface ***************************************************/
      switch (HSPI_Nor_Ctx[Instance].InterfaceMode)
      {
        case BSP_HSPI_NOR_OPI_MODE :  /* 8-8-8 commands */
          if ((Mode != BSP_HSPI_NOR_OPI_MODE) || (Rate != HSPI_Nor_Ctx[Instance].TransferRate))
          {
            /* Exit OPI mode */
            ret = HSPI_NOR_ExitOPIMode(Instance);

            if ((ret == BSP_ERROR_NONE) && (Mode == BSP_HSPI_NOR_OPI_MODE))
            {

              if (HSPI_Nor_Ctx[Instance].TransferRate == BSP_HSPI_NOR_STR_TRANSFER)
              {
                /* Enter DTR OPI mode */
                ret = HSPI_NOR_EnterDOPIMode(Instance);
              }
              else
              {
                /* Enter STR OPI mode */
                ret = HSPI_NOR_EnterSOPIMode(Instance);
              }
            }
          }
          break;

        case BSP_HSPI_NOR_SPI_MODE :  /* 1-1-1 commands, Power on H/W default setting */
        default :
          if (Mode == BSP_HSPI_NOR_OPI_MODE)
          {
            if (Rate == BSP_HSPI_NOR_STR_TRANSFER)
            {
              /* Enter STR OPI mode */
              ret = HSPI_NOR_EnterSOPIMode(Instance);
            }
            else
            {
              /* Enter DTR OPI mode */
              ret = HSPI_NOR_EnterDOPIMode(Instance);
            }
          }
          break;
      }

      /* Update HSPI context if all operations are well done */
      if (ret == BSP_ERROR_NONE)
      {
        /* Update current status parameter *****************************************/
        HSPI_Nor_Ctx[Instance].IsInitialized = HSPI_ACCESS_INDIRECT;
        HSPI_Nor_Ctx[Instance].InterfaceMode = Mode;
        HSPI_Nor_Ctx[Instance].TransferRate  = Rate;
      }
    }
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function suspends an ongoing erase command.
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_SuspendErase(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check whether the device is busy (erase operation is in progress). */
  else if (BSP_HSPI_NOR_GetStatus(Instance) != BSP_ERROR_BUSY)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX66UW1G45G_Suspend(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                               HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (BSP_HSPI_NOR_GetStatus(Instance) != BSP_ERROR_HSPI_SUSPENDED)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function resumes a paused erase command.
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_ResumeErase(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  /* Check whether the device is busy (erase operation is in progress). */
  else if (BSP_HSPI_NOR_GetStatus(Instance) != BSP_ERROR_HSPI_SUSPENDED)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX66UW1G45G_Resume(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                              HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /*
  When this command is executed, the status register write in progress bit is set to 1, and
  the flag status register program erase controller bit is set to 0. This command is ignored
  if the device is not in a suspended state.
  */
  else if (BSP_HSPI_NOR_GetStatus(Instance) != BSP_ERROR_BUSY)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function enter the HSPI memory in deep power down mode.
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_EnterDeepPowerDown(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MX66UW1G45G_EnterPowerDown(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                      HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* ---          Memory takes 10us max to enter deep power down          --- */

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function leave the HSPI memory from deep power down mode.
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
int32_t BSP_HSPI_NOR_LeaveDeepPowerDown(uint32_t Instance)
{
  int32_t ret;

  /* Check if the instance is supported */
  if (Instance >= HSPI_NOR_INSTANCES_NUMBER)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if (MX66UW1G45G_NoOperation(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                   HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* --- A NOP command is sent to the memory, as the nCS should be low for at least 20 ns --- */
  /* ---                  Memory takes 30us min to leave deep power down                  --- */

  /* Return BSP status */
  return ret;
}
/**
  * @}
  */

/** @addtogroup STM32U5G9J_DK_HSPI_NOR_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the HSPI MSP.
  * @param  hhspi HSPI handle
  * @retval None
  */
static void HSPI_NOR_MspInit(XSPI_HandleTypeDef *hhspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* hhspi unused argument(s) compilation warning */
  UNUSED(hhspi);

  /* Enable the HSPI memory interface clock */
  HSPI_NOR_CLK_ENABLE();

  /* Reset the HSPI memory interface */
  HSPI_NOR_FORCE_RESET();
  HSPI_NOR_RELEASE_RESET();

  /* Enable GPIO clocks */
  HSPI_NOR_CLK_GPIO_CLK_ENABLE();
  HSPI_NOR_DQS_GPIO_CLK_ENABLE();
  HSPI_NOR_CS_GPIO_CLK_ENABLE();
  HSPI_NOR_D0_GPIO_CLK_ENABLE();
  HSPI_NOR_D1_GPIO_CLK_ENABLE();
  HSPI_NOR_D2_GPIO_CLK_ENABLE();
  HSPI_NOR_D3_GPIO_CLK_ENABLE();
  HSPI_NOR_D4_GPIO_CLK_ENABLE();
  HSPI_NOR_D5_GPIO_CLK_ENABLE();
  HSPI_NOR_D6_GPIO_CLK_ENABLE();
  HSPI_NOR_D7_GPIO_CLK_ENABLE();

  /* HSPI CS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_NOR_CS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = HSPI_NOR_CS_PIN_AF;
  HAL_GPIO_Init(HSPI_NOR_CS_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI DQS GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_NOR_DQS_PIN;
  GPIO_InitStruct.Alternate = HSPI_NOR_DQS_PIN_AF;
  HAL_GPIO_Init(HSPI_NOR_DQS_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI CLK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_NOR_CLK_PIN;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Alternate = HSPI_NOR_CLK_PIN_AF;
  HAL_GPIO_Init(HSPI_NOR_CLK_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D0 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_NOR_D0_PIN;
  GPIO_InitStruct.Alternate = HSPI_NOR_D0_PIN_AF;
  HAL_GPIO_Init(HSPI_NOR_D0_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D1 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_NOR_D1_PIN;
  GPIO_InitStruct.Alternate = HSPI_NOR_D1_PIN_AF;
  HAL_GPIO_Init(HSPI_NOR_D1_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D2 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_NOR_D2_PIN;
  GPIO_InitStruct.Alternate = HSPI_NOR_D2_PIN_AF;
  HAL_GPIO_Init(HSPI_NOR_D2_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D3 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_NOR_D3_PIN;
  GPIO_InitStruct.Alternate = HSPI_NOR_D3_PIN_AF;
  HAL_GPIO_Init(HSPI_NOR_D3_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D4 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_NOR_D4_PIN;
  GPIO_InitStruct.Alternate = HSPI_NOR_D4_PIN_AF;
  HAL_GPIO_Init(HSPI_NOR_D4_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D5 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_NOR_D5_PIN;
  GPIO_InitStruct.Alternate = HSPI_NOR_D5_PIN_AF;
  HAL_GPIO_Init(HSPI_NOR_D5_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D6 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_NOR_D6_PIN;
  GPIO_InitStruct.Alternate = HSPI_NOR_D6_PIN_AF;
  HAL_GPIO_Init(HSPI_NOR_D6_GPIO_PORT, &GPIO_InitStruct);

  /* HSPI D7 GPIO pin configuration  */
  GPIO_InitStruct.Pin       = HSPI_NOR_D7_PIN;
  GPIO_InitStruct.Alternate = HSPI_NOR_D7_PIN_AF;
  HAL_GPIO_Init(HSPI_NOR_D7_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  De-Initializes the HSPI MSP.
  * @param  hhspi HSPI handle
  * @retval None
  */
static void HSPI_NOR_MspDeInit(XSPI_HandleTypeDef *hhspi)
{
  /* hhspi unused argument(s) compilation warning */
  UNUSED(hhspi);

  /* HSPI GPIO pins de-configuration  */
  HAL_GPIO_DeInit(HSPI_NOR_CLK_GPIO_PORT, HSPI_NOR_CLK_PIN);
  HAL_GPIO_DeInit(HSPI_NOR_DQS_GPIO_PORT, HSPI_NOR_DQS_PIN);
  HAL_GPIO_DeInit(HSPI_NOR_CS_GPIO_PORT, HSPI_NOR_CS_PIN);
  HAL_GPIO_DeInit(HSPI_NOR_D0_GPIO_PORT, HSPI_NOR_D0_PIN);
  HAL_GPIO_DeInit(HSPI_NOR_D1_GPIO_PORT, HSPI_NOR_D1_PIN);
  HAL_GPIO_DeInit(HSPI_NOR_D2_GPIO_PORT, HSPI_NOR_D2_PIN);
  HAL_GPIO_DeInit(HSPI_NOR_D3_GPIO_PORT, HSPI_NOR_D3_PIN);
  HAL_GPIO_DeInit(HSPI_NOR_D4_GPIO_PORT, HSPI_NOR_D4_PIN);
  HAL_GPIO_DeInit(HSPI_NOR_D5_GPIO_PORT, HSPI_NOR_D5_PIN);
  HAL_GPIO_DeInit(HSPI_NOR_D6_GPIO_PORT, HSPI_NOR_D6_PIN);
  HAL_GPIO_DeInit(HSPI_NOR_D7_GPIO_PORT, HSPI_NOR_D7_PIN);

  /* Reset the HSPI memory interface */
  HSPI_NOR_FORCE_RESET();
  HSPI_NOR_RELEASE_RESET();

  /* Disable the HSPI memory interface clock */
  HSPI_NOR_CLK_DISABLE();
}

/**
  * @brief  This function reset the HSPI memory.
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
static int32_t HSPI_NOR_ResetMemory(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;


  if (MX66UW1G45G_ResetEnable(&hhspi_nor[Instance], BSP_HSPI_NOR_SPI_MODE,
                              BSP_HSPI_NOR_STR_TRANSFER) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX66UW1G45G_ResetMemory(&hhspi_nor[Instance], BSP_HSPI_NOR_SPI_MODE,
                                   BSP_HSPI_NOR_STR_TRANSFER) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX66UW1G45G_ResetEnable(&hhspi_nor[Instance], BSP_HSPI_NOR_OPI_MODE,
                                   BSP_HSPI_NOR_STR_TRANSFER) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX66UW1G45G_ResetMemory(&hhspi_nor[Instance], BSP_HSPI_NOR_OPI_MODE,
                                   BSP_HSPI_NOR_STR_TRANSFER) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX66UW1G45G_ResetEnable(&hhspi_nor[Instance], BSP_HSPI_NOR_OPI_MODE,
                                   BSP_HSPI_NOR_DTR_TRANSFER) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if (MX66UW1G45G_ResetMemory(&hhspi_nor[Instance], BSP_HSPI_NOR_OPI_MODE,
                                   BSP_HSPI_NOR_DTR_TRANSFER) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    HSPI_Nor_Ctx[Instance].IsInitialized = HSPI_ACCESS_INDIRECT;     /* After reset S/W setting to indirect access  */
    HSPI_Nor_Ctx[Instance].InterfaceMode = BSP_HSPI_NOR_SPI_MODE;    /* After reset H/W back to SPI mode by default */
    HSPI_Nor_Ctx[Instance].TransferRate  = BSP_HSPI_NOR_STR_TRANSFER; /* After reset S/W setting to STR mode        */

    /* After SWreset CMD, wait in case SWReset occurred during erase operation */
    HAL_Delay(MX66UW1G45G_RESET_MAX_TIME);
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  This function enables the octal DTR mode of the memory.
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
static int32_t HSPI_NOR_EnterDOPIMode(uint32_t Instance)
{
  int32_t ret;
  uint8_t reg[2];

  /* Enable write operations */
  if (MX66UW1G45G_WriteEnable(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                              HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Write Configuration register 2 (with new dummy cycles) */
  else if (MX66UW1G45G_WriteCfg2Register(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                         HSPI_Nor_Ctx[Instance].TransferRate, MX66UW1G45G_CR2_REG3_ADDR,
                                         MX66UW1G45G_CR2_DC_6_CYCLES) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Enable write operations */
  else if (MX66UW1G45G_WriteEnable(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                   HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Write Configuration register 2 (with Octal I/O SPI protocol) */
  else if (MX66UW1G45G_WriteCfg2Register(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                         HSPI_Nor_Ctx[Instance].TransferRate, MX66UW1G45G_CR2_REG1_ADDR,
                                         MX66UW1G45G_CR2_DOPI) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Wait that the configuration is effective and check that memory is ready */
    HAL_Delay(MX66UW1G45G_WRITE_REG_MAX_TIME);

    /* Reconfigure the memory type of the peripheral */
    hhspi_nor[Instance].Init.MemoryType            = HAL_XSPI_MEMTYPE_MACRONIX;
    hhspi_nor[Instance].Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_ENABLE;
    if (HAL_XSPI_Init(&hhspi_nor[Instance]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    /* Check Flash busy ? */
    else if (MX66UW1G45G_AutoPollingMemReady(&hhspi_nor[Instance], BSP_HSPI_NOR_OPI_MODE,
                                             BSP_HSPI_NOR_DTR_TRANSFER) != MX66UW1G45G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    /* Check the configuration has been correctly done */
    else if (MX66UW1G45G_ReadCfg2Register(&hhspi_nor[Instance], BSP_HSPI_NOR_OPI_MODE, BSP_HSPI_NOR_DTR_TRANSFER,
                                          MX66UW1G45G_CR2_REG1_ADDR, reg) != MX66UW1G45G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else if (reg[0] != MX66UW1G45G_CR2_DOPI)
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
  * @brief  This function enables the octal STR mode of the memory.
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
static int32_t HSPI_NOR_EnterSOPIMode(uint32_t Instance)
{
  int32_t ret;
  uint8_t reg[2];

  /* Enable write operations */
  if (MX66UW1G45G_WriteEnable(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                              HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Write Configuration register 2 (with new dummy cycles) */
  else if (MX66UW1G45G_WriteCfg2Register(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                         HSPI_Nor_Ctx[Instance].TransferRate, MX66UW1G45G_CR2_REG3_ADDR,
                                         MX66UW1G45G_CR2_DC_6_CYCLES) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Enable write operations */
  else if (MX66UW1G45G_WriteEnable(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                   HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  /* Write Configuration register 2 (with Octal I/O SPI protocol) */
  else if (MX66UW1G45G_WriteCfg2Register(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                         HSPI_Nor_Ctx[Instance].TransferRate, MX66UW1G45G_CR2_REG1_ADDR,
                                         MX66UW1G45G_CR2_SOPI) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Wait that the configuration is effective and check that memory is ready */
    HAL_Delay(MX66UW1G45G_WRITE_REG_MAX_TIME);

    /* Check Flash busy ? */
    if (MX66UW1G45G_AutoPollingMemReady(&hhspi_nor[Instance], BSP_HSPI_NOR_OPI_MODE,
                                        BSP_HSPI_NOR_STR_TRANSFER) != MX66UW1G45G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    /* Check the configuration has been correctly done */
    else if (MX66UW1G45G_ReadCfg2Register(&hhspi_nor[Instance], BSP_HSPI_NOR_OPI_MODE, BSP_HSPI_NOR_STR_TRANSFER,
                                          MX66UW1G45G_CR2_REG1_ADDR, reg) != MX66UW1G45G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else if (reg[0] != MX66UW1G45G_CR2_SOPI)
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
  * @brief  This function disables the octal DTR or STR mode of the memory.
  * @param  Instance  HSPI instance
  * @retval BSP status
  */
static int32_t HSPI_NOR_ExitOPIMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  uint8_t reg[2];

  /* Enable write operations */
  if (MX66UW1G45G_WriteEnable(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                              HSPI_Nor_Ctx[Instance].TransferRate) != MX66UW1G45G_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Write Configuration register 2 (with SPI protocol) */
    reg[0] = 0;
    reg[1] = 0;
    if (MX66UW1G45G_WriteCfg2Register(&hhspi_nor[Instance], HSPI_Nor_Ctx[Instance].InterfaceMode,
                                      HSPI_Nor_Ctx[Instance].TransferRate, MX66UW1G45G_CR2_REG1_ADDR,
                                      reg[0]) != MX66UW1G45G_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      /* Wait that the configuration is effective and check that memory is ready */
      HAL_Delay(MX66UW1G45G_WRITE_REG_MAX_TIME);

      if (HSPI_Nor_Ctx[Instance].TransferRate == BSP_HSPI_NOR_DTR_TRANSFER)
      {
        /* Reconfigure the memory type of the peripheral */
        hhspi_nor[Instance].Init.MemoryType            = HAL_XSPI_MEMTYPE_MICRON;
        hhspi_nor[Instance].Init.DelayHoldQuarterCycle = HAL_XSPI_DHQC_DISABLE;
        if (HAL_XSPI_Init(&hhspi_nor[Instance]) != HAL_OK)
        {
          ret = BSP_ERROR_PERIPH_FAILURE;
        }
      }

      if (ret == BSP_ERROR_NONE)
      {
        /* Check Flash busy ? */
        if (MX66UW1G45G_AutoPollingMemReady(&hhspi_nor[Instance], BSP_HSPI_NOR_SPI_MODE,
                                            BSP_HSPI_NOR_STR_TRANSFER) != MX66UW1G45G_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        /* Check the configuration has been correctly done */
        else if (MX66UW1G45G_ReadCfg2Register(&hhspi_nor[Instance], BSP_HSPI_NOR_SPI_MODE, BSP_HSPI_NOR_STR_TRANSFER,
                                              MX66UW1G45G_CR2_REG1_ADDR, reg) != MX66UW1G45G_OK)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else if (reg[0] != 0U)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          /* Nothing to do */
        }
      }
    }
  }

  /* Return BSP status */
  return ret;
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

