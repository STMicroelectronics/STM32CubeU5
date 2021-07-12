/**
  ******************************************************************************
  * @file    stm32u5xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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
#include "stm32u5xx_hal.h"
#include "main.h"
#include "audio_recplay.h"
/** @addtogroup STM32U5xx_HAL_Driver
  * @{
  */

/** @defgroup HAL_MSP HAL MSP module driver
  * @brief HAL MSP module.
  * @{
  */
void MDF_MspInit_SAD(MDF_HandleTypeDef *hmdf);
void MDF_MspInit_Recorder(MDF_HandleTypeDef *hmdf);
void MDF_MspInit_PowerMeting(MDF_HandleTypeDef *hmdf);
extern DMA_HandleTypeDef  hSaiDma;
DMA_HandleTypeDef  DMAHandle;
extern uint32_t Digital_filter_module ;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Queue variable declaration */
DMA_QListTypeDef  SAIQueue;
DMA_QListTypeDef  MDFQueue;
/* Node variable declaration */
DMA_NodeTypeDef NodeTx;
DMA_NodeTypeDef NodeRx;


/* Node variable declaration */
#if defined ( __ICCARM__ )
#pragma default_variable_attributes =@".LPBAM_Section"   
DMA_NodeTypeDef MDFNode;
/* Stop placing functions in section SDRAM4 LPBAM_Sectionn */
#pragma default_variable_attributes =   
#endif
#if defined ( __CC_ARM )||( __GNUC__ )
DMA_NodeTypeDef MDFNode __attribute__((section(".LPBAM_Section")));
#endif

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */


/**
  * @brief  Init RTC peripheral.
  * @param  RTC_HandleTypeDef* hrtc: RTC Handle.
  * @retval None.
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef       RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef rtcclk;

  /* Disable bkup domain protection */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  /* Set LSE drive capability configuration */
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_BDCR_LSEDRV_1);

  /* Enable LSE oscillator */
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState       = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Set kernel clock source for RTC */
  rtcclk.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  rtcclk.RTCClockSelection    = RCC_RTCCLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&rtcclk);

  
  /* Enable RTC clock */
  __HAL_RCC_RTC_ENABLE();
  __HAL_RCC_RTCAPB_CLK_ENABLE();

  
  /* RTC global interrupt configuration */
  HAL_NVIC_SetPriority(RTC_IRQn, 7, 7);
  HAL_NVIC_EnableIRQ(RTC_IRQn);
}

/**
  * @brief  RTC MSP De-Initialization
  * @param  hrtc RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct ={0};

  /* Disable Irq */
  HAL_NVIC_DisableIRQ(RTC_IRQn);

  /* Configure LSE as RTC clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_NO_CLK;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    while(1);
  }

  __HAL_RCC_RTC_DISABLE();
  __HAL_RCC_RTCAPB_CLK_DISABLE();
  HAL_PWR_DisableBkUpAccess();
}

/**
  * @brief  Initialize the Global MSP.
  * @retval None
  */
void HAL_MspInit(void)
{
  /* NOTE : This function is generated automatically by STM32CubeMX and eventually
            modified by the user
   */
  
  /* Enable LPDMA1 Autonomous Mode */
  __HAL_RCC_LPDMA1_CLKAM_ENABLE();

  /* Enable SRAM4 Autonomous Mode */
  __HAL_RCC_SRAM4_CLKAM_ENABLE();

  /* Enable LPTIM3 autonomous mode clock */
  __HAL_RCC_LPTIM3_CLKAM_ENABLE();

  /* Enable DAC1 autonomous mode clock */
  __HAL_RCC_DAC1_CLKAM_ENABLE();

  /* Enable OPAMP autonomous mode clock */
  __HAL_RCC_OPAMP_CLKAM_ENABLE();
  
    /* Enable I2C autonomous mode clock */
  __HAL_RCC_I2C3_CLKAM_ENABLE();

  /* Enable RTC autonomous mode clock */
  __HAL_RCC_RTCAPB_CLKAM_ENABLE();
 
     /* Enable DMAx clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  
}

/**
  * @brief  DeInitialize the Global MSP.
  * @retval None
  */
void HAL_MspDeInit(void)
{
  /* NOTE : This function is generated automatically by STM32CubeMX and eventually
            modified by the user
   */
}


/*Audio Module*/

/**
  * @brief  SAI MSP Init.
  * @param  hsai : pointer to a SAI_HandleTypeDef structure that contains
  *                the configuration information for SAI module.
  * @retval None
  */
void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai)
{
  DMA_NodeConfTypeDef dmaLinkNodeConfig;
  GPIO_InitTypeDef    GPIO_Init;

  /* Enable SAI1 clock */
  AUDIO_SAIx_CLK_ENABLE();

  /* Configure GPIOs used for SAI1 */
  AUDIO_SAIx_MCLK_ENABLE();
  AUDIO_SAIx_SCK_ENABLE();
  AUDIO_SAIx_FS_ENABLE();
  AUDIO_SAIx_SD_ENABLE();
  
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_Init.Alternate = AUDIO_SAIx_FS_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_FS_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_FS_GPIO_PORT, &GPIO_Init);

  GPIO_Init.Alternate = AUDIO_SAIx_SCK_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_SCK_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_SCK_GPIO_PORT, &GPIO_Init);

  GPIO_Init.Alternate = AUDIO_SAIx_SD_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_SD_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_SD_GPIO_PORT, &GPIO_Init);

  GPIO_Init.Alternate = AUDIO_SAIx_MCLK_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_MCLK_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_MCLK_GPIO_PORT, &GPIO_Init);

  /* Configure DMA used for SAI1 */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  if(hsai->Instance == AUDIO_SAIx)
  {
    /* Set node type */
    dmaLinkNodeConfig.NodeType                            = DMA_GPDMA_LINEAR_NODE;
    /* Set common node parameters */
    dmaLinkNodeConfig.Init.Request                        = GPDMA1_REQUEST_SAI1_B;
    dmaLinkNodeConfig.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
    dmaLinkNodeConfig.Init.Direction                      = DMA_MEMORY_TO_PERIPH;
    dmaLinkNodeConfig.Init.SrcInc                         = DMA_SINC_INCREMENTED;
    dmaLinkNodeConfig.Init.DestInc                        = DMA_DINC_FIXED;
    dmaLinkNodeConfig.Init.SrcDataWidth                   = DMA_SRC_DATAWIDTH_HALFWORD;
    dmaLinkNodeConfig.Init.DestDataWidth                  = DMA_DEST_DATAWIDTH_HALFWORD;
    dmaLinkNodeConfig.Init.SrcBurstLength                 = 1;
    dmaLinkNodeConfig.Init.DestBurstLength                = 1;
    dmaLinkNodeConfig.Init.Priority                       = DMA_HIGH_PRIORITY;
    dmaLinkNodeConfig.Init.TransferEventMode              = DMA_TCEM_BLOCK_TRANSFER;
    dmaLinkNodeConfig.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;

    /* Set node data handling parameters */
    dmaLinkNodeConfig.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
    dmaLinkNodeConfig.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;

    /* Set node trigger parameters */
    dmaLinkNodeConfig.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

    /* Build NodeTx */
    HAL_DMAEx_List_BuildNode(&dmaLinkNodeConfig, &NodeTx);

    /* Insert NodeTx to SAI queue */
    HAL_DMAEx_List_InsertNode_Tail(&SAIQueue, &NodeTx);

    /* Select the DMA instance to be used for the transfer : GPDMA_Channel0 */
    hSaiDma.Instance                         = GPDMA1_Channel0;

    /* Set queue circular mode for sai queue */
    HAL_DMAEx_List_SetCircularMode(&SAIQueue);

    hSaiDma.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
    hSaiDma.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
    hSaiDma.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
    hSaiDma.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
    hSaiDma.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

    /* DMA linked list init */
    HAL_DMAEx_List_Init(&hSaiDma);

    /* Link SAI queue to DMA channel */
    HAL_DMAEx_List_LinkQ(&hSaiDma, &SAIQueue);

    /* Associate the DMA handle */
    __HAL_LINKDMA(hsai, hdmatx, hSaiDma);

  }

  HAL_NVIC_SetPriority(GPDMA1_Channel0_IRQn, 0x01, 0);
  HAL_NVIC_EnableIRQ(GPDMA1_Channel0_IRQn);
}


/**
  * @brief  MDF MSP Init.
  * @param  hsai : pointer to a MDF_HandleTypeDef structure that contains
  *                the configuration information for SAI module.
  * @retval None
  */

void HAL_MDF_MspInit(MDF_HandleTypeDef *hmdf)
{
  
  if ( Digital_filter_module == 0)
  {
    
     MDF_MspInit_SAD(hmdf);  
  }
  if ( Digital_filter_module == 1)
  {
    
     MDF_MspInit_Recorder(hmdf) ; 
  }
  if ( Digital_filter_module == 2)
  {
    
     MDF_MspInit_PowerMeting(hmdf)  ;
  }  
}

void MDF_MspInit_Recorder(MDF_HandleTypeDef *hmdf)

{
  RCC_OscInitTypeDef  RCC_OscInitStruct = {0};
  DMA_NodeConfTypeDef dmaLinkNodeConfig;
  GPIO_InitTypeDef  GPIO_Init;

    /* Enable Clock Autonomous mode */
  __HAL_RCC_ADF1_CLKAM_ENABLE();
  __HAL_RCC_LPDMA1_CLKAM_ENABLE();
  __HAL_RCC_LPGPIO1_CLKAM_ENABLE();
  __HAL_RCC_SRAM4_CLKAM_ENABLE();
  
  /* MSI Oscillator enabled at reset (4Mhz), activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIK;
  RCC_OscInitStruct.MSIKState      = RCC_MSIK_ON;
  RCC_OscInitStruct.MSIKClockRange = RCC_MSIKRANGE_3;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }  
  __HAL_RCC_MSIKSTOP_ENABLE();
  __HAL_RCC_ADF1_CONFIG(RCC_ADF1CLKSOURCE_MSIK);
  /* Enable MDF1 clock */
  AUDIO_MDFx_CLK_ENABLE();

  /* Configure GPIOs used for ADF1 */
  AUDIO_MDFx_CCK0_ENABLE();
  AUDIO_MDFx_SDIN0_ENABLE();
 
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_PULLDOWN;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_Init.Alternate = AUDIO_MDFx_CCK0_AF;
  GPIO_Init.Pin       = AUDIO_MDFx_CCK0_PIN;
  HAL_GPIO_Init(AUDIO_MDFx_CCK0_GPIO_PORT, &GPIO_Init);

  GPIO_Init.Alternate = AUDIO_MDFx_SDIN0_AF;
  GPIO_Init.Pin       = AUDIO_MDFx_SDIN0_PIN;
  HAL_GPIO_Init(AUDIO_MDFx_SDIN0_GPIO_PORT, &GPIO_Init);

 
  /* Configure DMA used for ADF1 */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  
  /* Set node type */
  dmaLinkNodeConfig.NodeType                            = DMA_GPDMA_LINEAR_NODE;
  /* Set common node parameters */
  dmaLinkNodeConfig.Init.Request                        = GPDMA1_REQUEST_ADF1_FLT0;
  dmaLinkNodeConfig.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
  dmaLinkNodeConfig.Init.Direction                      = DMA_PERIPH_TO_MEMORY;
  dmaLinkNodeConfig.Init.SrcInc                         = DMA_SINC_FIXED;
  dmaLinkNodeConfig.Init.DestInc                        = DMA_DINC_INCREMENTED;
  dmaLinkNodeConfig.Init.SrcDataWidth                   = DMA_SRC_DATAWIDTH_HALFWORD; 
  dmaLinkNodeConfig.Init.DestDataWidth                  = DMA_DEST_DATAWIDTH_HALFWORD;
  dmaLinkNodeConfig.Init.SrcBurstLength                 = 1;
  dmaLinkNodeConfig.Init.DestBurstLength                = 1;
  dmaLinkNodeConfig.Init.Priority                       = DMA_HIGH_PRIORITY;
  dmaLinkNodeConfig.Init.TransferEventMode              = DMA_TCEM_EACH_LL_ITEM_TRANSFER;
  dmaLinkNodeConfig.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
  dmaLinkNodeConfig.Init.Mode                           = DMA_NORMAL;
  
  /* Set node data handling parameters */
  dmaLinkNodeConfig.DataHandlingConfig.DataExchange     = DMA_EXCHANGE_NONE;
  dmaLinkNodeConfig.DataHandlingConfig.DataAlignment    = DMA_DATA_UNPACK;
  
  /* Set node trigger parameters */
  dmaLinkNodeConfig.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;
  dmaLinkNodeConfig.RepeatBlockConfig.RepeatCount         = 1U;
  dmaLinkNodeConfig.RepeatBlockConfig.SrcAddrOffset       = 0;
  dmaLinkNodeConfig.RepeatBlockConfig.DestAddrOffset      = 0;
  dmaLinkNodeConfig.RepeatBlockConfig.BlkSrcAddrOffset    = 0;
  dmaLinkNodeConfig.RepeatBlockConfig.BlkDestAddrOffset   = 0;
  
  
  /* Build NodeTx */
  HAL_DMAEx_List_BuildNode(&dmaLinkNodeConfig, &NodeRx);
  
  /* Insert NodeTx to SAI queue */
  HAL_DMAEx_List_InsertNode_Tail(&MDFQueue, &NodeRx);
  
  /* Select the DMA instance to be used for the transfer : GPDMA_Channel1 */
  DMAHandle.Instance                         = GPDMA1_Channel1;
  
  /* Set queue circular mode for sai queue */
  HAL_DMAEx_List_SetCircularMode(&MDFQueue);
  
  DMAHandle.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
  DMAHandle.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMAHandle.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT0;
  DMAHandle.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  DMAHandle.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;
  
  /* DMA linked list init */
  HAL_DMAEx_List_Init(&DMAHandle);
  
  /* Link SAI queue to DMA channel */
  HAL_DMAEx_List_LinkQ(&DMAHandle, &MDFQueue);
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hmdf, hdma, DMAHandle);
  
  if (IS_MDF_INSTANCE(hmdf->Instance))
  {
    HAL_NVIC_SetPriority(GPDMA1_Channel1_IRQn, 0x01, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);
  }
  else
  {
    HAL_NVIC_SetPriority(ADF1_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(ADF1_IRQn);
  }
    
  
}


/**
  * @brief  MDF MSP Init.
  * @param  hsai : pointer to a MDF_HandleTypeDef structure that contains
  *                the configuration information for SAI module.
  * @retval None
  */
void MDF_MspInit_PowerMeting(MDF_HandleTypeDef *hmdf)
{
  GPIO_InitTypeDef  GPIO_Init;

  /* Enable MDF1 clock */
  AUDIO_MDFx_CLK_ENABLE();

  /* Configure GPIOs used for ADF1 */
  __HAL_RCC_MDF1_CLK_ENABLE();

  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_PULLDOWN;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_Init.Alternate = GPIO_AF6_MDF1;
  GPIO_Init.Pin       = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOF, &GPIO_Init);

  GPIO_Init.Alternate = GPIO_AF6_MDF1;
  GPIO_Init.Pin       = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOE, &GPIO_Init);

  /* Configure DMA used for ADF1 */

  HAL_NVIC_SetPriority(MDF1_FLT3_IRQn, 0x05, 0);
  HAL_NVIC_EnableIRQ(MDF1_FLT3_IRQn);
  
  HAL_NVIC_SetPriority(MDF1_FLT4_IRQn, 0x05, 0);
  HAL_NVIC_EnableIRQ(MDF1_FLT4_IRQn);
}


void MDF_MspInit_SAD(MDF_HandleTypeDef *hmdf)
{
  RCC_OscInitTypeDef  RCC_OscInitStruct = {0};
  DMA_NodeConfTypeDef dmaLinkNodeConfig;
  GPIO_InitTypeDef    GPIO_Init;

  /* Enable Clock Autonomous mode */
  __HAL_RCC_ADF1_CLKAM_ENABLE();
  __HAL_RCC_LPDMA1_CLKAM_ENABLE();
  __HAL_RCC_LPGPIO1_CLKAM_ENABLE();
  __HAL_RCC_SRAM4_CLKAM_ENABLE();

  HAL_PWREx_EnableVddIO2();
  HAL_PWREx_EnableVddA();

  /* MSI Oscillator enabled at reset (4Mhz), activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSIK;
  RCC_OscInitStruct.MSIKState      = RCC_MSIK_ON;
  RCC_OscInitStruct.MSIKClockRange = RCC_MSIKRANGE_3; /* for range 3: 12MHz*/
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  __HAL_RCC_MSIKSTOP_ENABLE();
  __HAL_RCC_ADF1_CONFIG(RCC_ADF1CLKSOURCE_MSIK);

  /* Enable MDF1 clock */
  AUDIO_MDFx_CLK_ENABLE();

  /* Configure GPIOs used for ADF1 */
  AUDIO_MDFx_CCK0_ENABLE();
  AUDIO_MDFx_SDIN0_ENABLE();

  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_PULLDOWN;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_Init.Alternate = AUDIO_MDFx_CCK0_AF;
  GPIO_Init.Pin       = AUDIO_MDFx_CCK0_PIN;
  HAL_GPIO_Init(AUDIO_MDFx_CCK0_GPIO_PORT, &GPIO_Init);

  GPIO_Init.Alternate = AUDIO_MDFx_SDIN0_AF;
  GPIO_Init.Pin       = AUDIO_MDFx_SDIN0_PIN;
  HAL_GPIO_Init(AUDIO_MDFx_SDIN0_GPIO_PORT, &GPIO_Init);


  /* Configure DMA used for ADF1 */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  /* Set node type */
  dmaLinkNodeConfig.NodeType                            = DMA_LPDMA_LINEAR_NODE;
  /* Set common node parameters */
  dmaLinkNodeConfig.Init.Request                        = LPDMA1_REQUEST_ADF1_FLT0;
  dmaLinkNodeConfig.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
  dmaLinkNodeConfig.Init.Direction                      = DMA_PERIPH_TO_MEMORY;
  dmaLinkNodeConfig.Init.SrcInc                         = DMA_SINC_FIXED;
  dmaLinkNodeConfig.Init.DestInc                        = DMA_DINC_INCREMENTED;
  dmaLinkNodeConfig.Init.SrcDataWidth                   = DMA_SRC_DATAWIDTH_WORD;
  dmaLinkNodeConfig.Init.DestDataWidth                  = DMA_DEST_DATAWIDTH_WORD;
  dmaLinkNodeConfig.Init.TransferAllocatedPort          = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
  dmaLinkNodeConfig.Init.TransferEventMode              = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  /* Set node data handling parameters */
  dmaLinkNodeConfig.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  /* Set node trigger parameters */
  dmaLinkNodeConfig.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_MASKED;

  /* Build MDFNode */
  HAL_DMAEx_List_BuildNode(&dmaLinkNodeConfig, &MDFNode);

  /* Insert MDFNode to MDF queue */
  HAL_DMAEx_List_InsertNode_Tail(&MDFQueue, &MDFNode);

  /* Set queue circular mode for sai queue */
  HAL_DMAEx_List_SetCircularMode(&MDFQueue);


  /* Select the DMA instance to be used for the transfer : LPDMA1_Channel0 */
  DMAHandle.Instance                         = LPDMA1_Channel0;

  DMAHandle.InitLinkedList.Priority          = DMA_LOW_PRIORITY_LOW_WEIGHT;
  DMAHandle.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMAHandle.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DMAHandle.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  DMAHandle.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

  /* DMA linked list init */
  if (HAL_DMAEx_List_Init(&DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }
  /* Link SAI queue to DMA channel */
  if (HAL_DMAEx_List_LinkQ(&DMAHandle, &MDFQueue) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hmdf, hdma, DMAHandle);

  HAL_NVIC_SetPriority(ADF1_IRQn, 0x05, 0);
  HAL_NVIC_EnableIRQ(ADF1_IRQn);
}


void HAL_MDF_MspDeInit(MDF_HandleTypeDef *hmdf)
{
  
  if ( Digital_filter_module < 2) /*SAD or Power Metering*/
  {
    /* Reset MDFQueue */
    HAL_DMAEx_List_ResetQ(&MDFQueue);
    /* Reset the DMA Channel configuration*/
    HAL_DMAEx_List_DeInit(&DMAHandle);    
    
  }

 

  
}
/* LPBAM modules */

/**
  * @brief  LPTIM MSP Init
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef *hlptim)
{
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};

  /* Select the LSE clock as LPTIM3 peripheral clock */
  RCC_PeriphCLKInitStruct.PeriphClockSelection  = RCC_PERIPHCLK_LPTIM34;
  RCC_PeriphCLKInitStruct.Lptim34ClockSelection = RCC_LPTIM34CLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable LPTIM3 clock */
  __HAL_RCC_LPTIM3_CLK_ENABLE();

  /* Force & Release the LPTIM3 Periheral Clock Reset */
  /* Force the LPTIM3 Peripheral Clock Reset */
  __HAL_RCC_LPTIM3_FORCE_RESET();

  /* Release the LPTIM3 Peripheral Clock Reset */
  __HAL_RCC_LPTIM3_RELEASE_RESET();

  /* Disable LPTIM3 Interrupt */
  HAL_NVIC_DisableIRQ(LPTIM3_IRQn);
}

/**
  * @brief  LPTIM MSP DeInit
  * @param  hlptim : LPTIM handle
  * @retval None
  */
void HAL_LPTIM_MspDeInit(LPTIM_HandleTypeDef *hlptim)
{
  /* Disable LPTIM3 clock */
  __HAL_RCC_LPTIM3_CLK_DISABLE();

  /* Disable LPTIM3 Interrupt */
  HAL_NVIC_DisableIRQ(LPTIM3_IRQn);
}

/**
  * @brief OPAMP MSP Initialization
  * @param hopamp: OPAMP handle pointer
  * @retval None
  */
void HAL_OPAMP_MspInit(OPAMP_HandleTypeDef *hopamp)
{
  /* Configuration of OPAMP clock */
  __HAL_RCC_OPAMP_CLK_ENABLE();

  /* Force & Release the OPAMP Periheral Clock Reset */
  /* Force the OPAMP Peripheral Clock Reset */
  __HAL_RCC_OPAMP_FORCE_RESET();

  /* Release the OPAMP Peripheral Clock Reset */
  __HAL_RCC_OPAMP_RELEASE_RESET();
}

/**
  * @brief OPAMP MSP DeInit
  * @param hopamp: OPAMP handle pointer
  * @retval None
  */
void HAL_OPAMP_MspDeInit(OPAMP_HandleTypeDef *hopamp)
{
  /* Configuration of OPAMP clock */
  __HAL_RCC_OPAMP_CLK_DISABLE();
}

/**
  * @brief  DAC MSP Init
  * @param  hdac : DAC handle
  * @retval None
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};

  /* Select kernel peripheral clock */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADCDAC;
  RCC_PeriphCLKInitStruct.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set LSE kernel clock for DAC peripheral */
  __HAL_RCC_DAC1_CONFIG(RCC_DAC1CLKSOURCE_LSE);

  /* Enable DAC1 clock */
  __HAL_RCC_DAC1_CLK_ENABLE();
}

/**
  * @brief  DAC MSP DeInit
  * @param  hdac : DAC handle
  * @retval None
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
{
  /* Disable DAC1 clock */
  __HAL_RCC_DAC1_CLK_DISABLE();
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
