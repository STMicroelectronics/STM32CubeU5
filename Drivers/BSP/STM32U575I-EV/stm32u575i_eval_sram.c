/**
  ******************************************************************************
  * @file    stm32u575i_eval_sram.c
  * @author  MCD Application Team
  * @brief   This file provides a set of functions needed to drive the
  *          IS61WV102416BLL-10MLI SRAM memory mounted on STM32U575I-EVAL board.
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
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
  [..]
   (#) This driver is used to drive the IS61WV102416BLL-10MLI SRAM external memory mounted
       on STM32U575I-EVAL evaluation board.

   (#) This driver does not need a specific component driver for the SRAM device
       to be included with.

   (#) Initialization steps:
       (++) Initialize the SRAM external memory using the BSP_SRAM_Init() function. This
            function includes the MSP layer hardware resources initialization and the
            FMC controller configuration to interface with the external SRAM memory.

   (#) SRAM read/write operations
       (++) SRAM external memory can be accessed with read/write operations once it is
            initialized.
            Read/write operation can be performed with AHB access using the functions
            HAL_SRAM_Read_16b()/HAL_SRAM_Write_16b(), or by DMA transfer using the functions
            HAL_SRAM_Read_DMA()/HAL_SRAM_Write_DMA().
       (++) The AHB access is performed with 16-bit width transaction, the DMA transfer
            configuration is fixed at single (no burst) halfword transfer
            (see the SRAM_MspInit() static function).
       (++) User can implement his own functions for read/write access with his desired
            configurations.
       (++) If interrupt mode is used for DMA transfer, the function BSP_SRAM_DMA_IRQHandler()
            is called in IRQ handler file, to serve the generated interrupt once the DMA
            transfer is complete.

   (#) Error event is handled through a dedicated weak callback that can be override at
       application level: BSP_SRAM_ErrorCallback()

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32u575i_eval_sram.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32U575I_EVAL
  * @{
  */

/** @defgroup STM32U575I_EVAL_SRAM SRAM
  * @{
  */

/* Private variables ---------------------------------------------------------*/

/** @defgroup STM32U575I_EVAL_SRAM_Private_Variables SRAM Private Variables
  * @{
  */
#if (USE_HAL_SRAM_REGISTER_CALLBACKS > 0)
static uint32_t Sram_IsMspCallbacksValid[SRAM_INSTANCES_NBR] = {0};
#endif /*(USE_HAL_SRAM_REGISTER_CALLBACKS > 0)*/
/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/

/** @defgroup STM32U575I_EVAL_SRAM_Exported_Variables SRAM Exported Variables
  * @{
  */
SRAM_HandleTypeDef hsram[SRAM_INSTANCES_NBR];
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/** @defgroup STM32U575I_EVAL_SRAM_Private_Functions SRAM Private Functions
  * @{
  */
static void SRAM_MspInit(SRAM_HandleTypeDef *hSram);
static void SRAM_MspDeInit(SRAM_HandleTypeDef *hSram);

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/

/** @defgroup STM32U575I_EVAL_SRAM_Exported_Functions SRAM Exported Functions
  * @{
  */

/**
  * @brief  Initializes the SRAM device.
  * @param  Instance SRAM instance
  * @retval BSP status
  */
int32_t BSP_SRAM_Init(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= SRAM_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
#if (USE_HAL_SRAM_REGISTER_CALLBACKS == 0)
    /* Msp SRAM initialization */
    SRAM_MspInit(&hsram[Instance]);
#else
    /* Register the SRAM MSP Callbacks */
    if (Sram_IsMspCallbacksValid[Instance] == 0U)
    {
      if (BSP_SRAM_RegisterDefaultMspCallbacks(Instance) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_PERIPH_FAILURE;
      }
    }
#endif /* (USE_HAL_SRAM_REGISTER_CALLBACKS == 0) */

    /* __weak function can be rewritten by the application */
    if (MX_SRAM_BANK1_Init(&hsram[Instance]) != HAL_OK)
    {
      ret = BSP_ERROR_NO_INIT;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }

  }
  return ret ;
}

/**
  * @brief  DeInitializes the SRAM device.
  * @param  Instance SRAM instance
  * @retval BSP status
  */
int32_t BSP_SRAM_DeInit(uint32_t Instance)
{
  int32_t ret;

  if (Instance >= SRAM_INSTANCES_NBR)
  {
    ret =  BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* SRAM device de-initialization */
    if (HAL_SRAM_DeInit(&hsram[Instance]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
#if (USE_HAL_SRAM_REGISTER_CALLBACKS == 0)
      /* SRAM controller de-initialization */
      SRAM_MspDeInit(&hsram[Instance]);
#endif /* (USE_HAL_SRAM_REGISTER_CALLBACKS == 0) */

      ret = BSP_ERROR_NONE;
    }
  }

  return ret;
}

/**
  * @brief  Initializes the SRAM periperal.
  * @retval HAL status
  */
__weak HAL_StatusTypeDef MX_SRAM_BANK1_Init(SRAM_HandleTypeDef *hSram)
{
  static FMC_NORSRAM_TimingTypeDef SramTiming;

  /* SRAM device configuration */
  hSram->Instance = FMC_NORSRAM_DEVICE;
  hSram->Extended = FMC_NORSRAM_EXTENDED_DEVICE;

  /* SRAM device configuration */
  hSram->Init.NSBank             = FMC_NORSRAM_BANK1;
  hSram->Init.DataAddressMux     = FMC_DATA_ADDRESS_MUX_DISABLE;
  hSram->Init.MemoryType         = FMC_MEMORY_TYPE_SRAM;
  hSram->Init.MemoryDataWidth    = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hSram->Init.BurstAccessMode    = FMC_BURST_ACCESS_MODE_DISABLE;
  hSram->Init.WriteOperation     = FMC_WRITE_OPERATION_ENABLE;
  hSram->Init.ExtendedMode       = FMC_EXTENDED_MODE_DISABLE;
  hSram->Init.AsynchronousWait   = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hSram->Init.WriteBurst         = FMC_WRITE_BURST_DISABLE;
  hSram->Init.ContinuousClock    = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hSram->Init.WriteFifo          = FMC_WRITE_FIFO_DISABLE;
  hSram->Init.NBLSetupTime       = 0;
  hSram->Init.PageSize           = FMC_PAGE_SIZE_NONE;

  /* Timings for SRAM IS61WV102416BLL-10MLI */
  SramTiming.AddressSetupTime      = 4; /* 16ns with a clock at 160 MHz (period of 6.25 ns) */
  SramTiming.AddressHoldTime       = 1; /* Min value, Don't care on SRAM Access mode A */
  SramTiming.DataSetupTime         = 1; /* 6ns with a clock at 160 MHz (period of 6.25 ns) */
  SramTiming.DataHoldTime          = 0;
  SramTiming.BusTurnAroundDuration = 1; /* 4ns with a clock at 160 MHz (period of 6.25 ns) */
  SramTiming.CLKDivision           = 2; /* Min value, Don't care on SRAM Access mode A */
  SramTiming.DataLatency           = 2; /* Min value, Don't care on SRAM Access mode A */
  SramTiming.AccessMode            = FMC_ACCESS_MODE_A;

  /* SRAM controller initialization */
  if (HAL_SRAM_Init(hSram, &SramTiming, NULL) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

#if (USE_HAL_SRAM_REGISTER_CALLBACKS > 0)
/**
  * @brief BSP SRAM default Msp Callback registering
  * @param Instance     SRAM Instance
  * @retval BSP status
  */
int32_t BSP_SRAM_RegisterDefaultMspCallbacks(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= SRAM_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_SRAM_RegisterCallback(&hsram[Instance], HAL_SRAM_MSP_INIT_CB_ID, SRAM_MspInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SRAM_RegisterCallback(&hsram[Instance], HAL_SRAM_MSP_DEINIT_CB_ID, SRAM_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      Sram_IsMspCallbacksValid[Instance] = 1;
    }
  }
  /* Return BSP status */
  return ret;
}

/**
  * @brief BSP SRAM Msp Callback registering
  * @param Instance     SRAM Instance
  * @param CallBacks    pointer to MspInit/MspDeInit callbacks functions
  * @retval BSP status
  */
int32_t BSP_SRAM_RegisterMspCallbacks(uint32_t Instance, BSP_SRAM_Cb_t *CallBacks)
{
  int32_t ret = BSP_ERROR_NONE;

  /* Check if the instance is supported */
  if (Instance >= SRAM_INSTANCES_NBR)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Register MspInit/MspDeInit Callbacks */
    if (HAL_SRAM_RegisterCallback(&hsram[Instance], HAL_SRAM_MSP_INIT_CB_ID, CallBacks->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if (HAL_SRAM_RegisterCallback(&hsram[Instance], HAL_SRAM_MSP_DEINIT_CB_ID, CallBacks->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      Sram_IsMspCallbacksValid[Instance] = 1;
    }
  }
  /* Return BSP status */
  return ret;
}
#endif /* (USE_HAL_SRAM_REGISTER_CALLBACKS > 0) */

/**
  * @brief  BSP SRAM DMA interrupt handler.
  * @param  Instance SRAM Instance
  * @retval None.
  */
void BSP_SRAM_IRQHandler(uint32_t Instance)
{
  HAL_DMA_IRQHandler(hsram[Instance].hdma);
}

/**
  * @}
  */

/** @addtogroup STM32U575I_EVAL_SRAM_Private_Functions
  * @{
  */

/**
  * @brief  Initializes SRAM MSP.
  * @retval None
  */
static void SRAM_MspInit(SRAM_HandleTypeDef *hSram)
{
  static DMA_HandleTypeDef dmaHandle;
  GPIO_InitTypeDef gpioinitstruct;

  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /* Enable chosen DMAx clock */
  SRAM_DMAx_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  /* Common GPIO configuration */
  gpioinitstruct.Mode      = GPIO_MODE_AF_PP;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpioinitstruct.Alternate = GPIO_AF12_FMC;

  /* NE configuration */
  /* NE1 : SRAM */
  gpioinitstruct.Pull = GPIO_PULLUP;
  gpioinitstruct.Pin  = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);

  /* NOE and NWE configuration */
  gpioinitstruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);

  /* NBL0, NBL1 configuration */
  gpioinitstruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  HAL_GPIO_Init(GPIOE, &gpioinitstruct);

  /* Address Bus */
  /* GPIOD configuration */
  gpioinitstruct.Pull = GPIO_NOPULL;
  gpioinitstruct.Pin  = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);

  /* GPIOE configuration */
  gpioinitstruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;
  HAL_GPIO_Init(GPIOE, &gpioinitstruct);

  /* GPIOF configuration */
  gpioinitstruct.Pin = GPIO_PIN_0  | GPIO_PIN_1 | GPIO_PIN_2  | GPIO_PIN_3  | \
                       GPIO_PIN_4  | GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | \
                       GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &gpioinitstruct);

  /* GPIOG configuration */
  gpioinitstruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                       GPIO_PIN_4 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOG, &gpioinitstruct);

  /* Data Bus */
  /* GPIOD configuration */
  gpioinitstruct.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_8 | GPIO_PIN_9 | \
                       GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);

  /* GPIOE configuration */
  gpioinitstruct.Pin = GPIO_PIN_7  | GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | \
                       GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | \
                       GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpioinitstruct);

  /* Configure common DMA parameters */
  dmaHandle.Init.Request               = DMA_REQUEST_SW;
  dmaHandle.Init.BlkHWRequest          = DMA_BREQ_SINGLE_BURST;
  dmaHandle.Init.Direction             = DMA_MEMORY_TO_MEMORY;
  dmaHandle.Init.SrcInc                = DMA_SINC_INCREMENTED;
  dmaHandle.Init.DestInc               = DMA_DINC_INCREMENTED;
  dmaHandle.Init.SrcDataWidth          = DMA_SRC_DATAWIDTH_HALFWORD;
  dmaHandle.Init.DestDataWidth         = DMA_DEST_DATAWIDTH_HALFWORD;
  dmaHandle.Init.SrcBurstLength        = 1;
  dmaHandle.Init.DestBurstLength       = 1;
  dmaHandle.Init.Priority              = DMA_HIGH_PRIORITY;
  dmaHandle.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
  dmaHandle.Init.TransferEventMode     = DMA_TCEM_BLOCK_TRANSFER ;
  dmaHandle.Init.Mode                  = DMA_NORMAL;

  dmaHandle.Instance = SRAM_DMAx_CHANNEL;

  /* Deinitialize the DMA channel for new transfer */
  if (HAL_DMA_DeInit(&dmaHandle) != HAL_OK)
  {
    BSP_SRAM_ErrorCallback(0);
  }

  /* Configure the DMA channel */
  if (HAL_DMA_Init(&dmaHandle) != HAL_OK)
  {
    BSP_SRAM_ErrorCallback(0);
  }

  /* Associate the DMA handle */
  __HAL_LINKDMA(hSram, hdma, dmaHandle);

  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(SRAM_DMAx_IRQn, BSP_SRAM_DMA_IT_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(SRAM_DMAx_IRQn);
}

/**
  * @brief  DeInitializes SRAM MSP.
  * @param  hSram  SRAM handle
  * @retval None
  */
static void SRAM_MspDeInit(SRAM_HandleTypeDef *hSram)
{
  GPIO_InitTypeDef gpio_init_structure;
  static DMA_HandleTypeDef dma_handle;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hSram);

  /* GPIOD configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_4  | GPIO_PIN_5  | \
                              GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_11 | \
                              GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_DeInit(GPIOD, gpio_init_structure.Pin);

  /* GPIOE configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_3  | GPIO_PIN_4  | \
                              GPIO_PIN_7  | GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | \
                              GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | \
                              GPIO_PIN_15;
  HAL_GPIO_DeInit(GPIOE, gpio_init_structure.Pin);

  /* GPIOF configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0  | GPIO_PIN_1 | GPIO_PIN_2  | GPIO_PIN_3  | \
                              GPIO_PIN_4  | GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | \
                              GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_DeInit(GPIOF, gpio_init_structure.Pin);

  /* GPIOG configuration */
  gpio_init_structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                              GPIO_PIN_4 | GPIO_PIN_5;
  HAL_GPIO_DeInit(GPIOG, gpio_init_structure.Pin);

  /* Disable NVIC configuration for DMA interrupt */
  HAL_NVIC_DisableIRQ(SRAM_DMAx_IRQn);

  /* Deinitialize the stream for new transfer */
  dma_handle.Instance = SRAM_DMAx_CHANNEL;
  if (HAL_DMA_DeInit(&dma_handle) != HAL_OK)
  {
    BSP_SRAM_ErrorCallback(0);
  }
}

/**
  * @brief  Error callback.
  * @param  Instance SRAM instance.
  * @retval None
  */
__weak void BSP_SRAM_ErrorCallback(uint32_t Instance)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Instance);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the BSP_SRAM_ErrorCallback could be implemented in the user file
    */
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
