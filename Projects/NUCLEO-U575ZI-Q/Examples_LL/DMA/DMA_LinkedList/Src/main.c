/**
  ******************************************************************************
  * @file    DMA/DMA_LinkedList/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body through the LL API
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
#include "main.h"

/** @addtogroup STM32U5xx_LL_Examples
  * @{
  */

/** @addtogroup DMA_LinkedList
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* DMA nodes declaration */
LL_DMA_LinkNodeTypeDef pNode1, pNode2, pNode3;

/* DMA flags declaration */
__IO uint32_t TransferCompleteDetected = 0U, TransferErrorDetected = 0U;

/* Source buffers declaration */
static uint32_t aSRC_Buffer1[BUFFER1_SIZE] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
};

static uint32_t aSRC_Buffer2[BUFFER2_SIZE] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
};

static uint32_t aSRC_Buffer3[BUFFER3_SIZE] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
};

/* Destination buffers declaration */
static uint32_t aDST_Buffer1[BUFFER1_SIZE];
static uint32_t aDST_Buffer2[BUFFER2_SIZE];
static uint32_t aDST_Buffer3[BUFFER3_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Configure_GPIO    (void);
static void Error_Handler     (void);

static void DMA_LinkedListConfig(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 160 MHz */
  SystemClock_Config();

  /* Configure IO in output push-pull mode to drive external LEDs */
  Configure_GPIO();

  /* DMA Linked-list transfer configuration */
  DMA_LinkedListConfig();

  /* Enable DMA channel */
  LL_DMA_EnableChannel(GPDMA1, LL_DMA_CHANNEL_13);

  /* Wait for end of transmission or an error occurred */
  while ((TransferCompleteDetected == 0) && (TransferErrorDetected == 0U));

  /* Check DMA error */
  if (TransferErrorDetected == 1U)
  {
    Error_Handler();
  }

  /* Infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(SUCCESS_LED_GPIO_PORT, SUCCESS_LED_PIN);

    /* Insert delay 250 ms */
    LL_mDelay(250);
  }
}

/**
  * @brief  DMA Linked-list transfer configuration
  * @param  None
  * @retval None
  */
static void DMA_LinkedListConfig(void)
{
  /* DMA node configuration declaration */
  LL_DMA_InitNodeTypeDef       DMA_InitNodeStruct;
  LL_DMA_InitLinkedListTypeDef DMA_InitLinkedListStruct;

  /* Set node 1 configuration ################################################*/
  /* Set node type */
  DMA_InitNodeStruct.NodeType                 = LL_DMA_GPDMA_2D_NODE;
  /* Set node configuration */
  DMA_InitNodeStruct.BlkHWRequest             = LL_DMA_HWREQUEST_SINGLEBURST;
  DMA_InitNodeStruct.Direction                = LL_DMA_DIRECTION_MEMORY_TO_MEMORY;
  DMA_InitNodeStruct.DestAllocatedPort        = LL_DMA_DEST_ALLOCATED_PORT1;
  DMA_InitNodeStruct.DestBurstLength          = 1U;
  DMA_InitNodeStruct.DestIncMode              = LL_DMA_DEST_INCREMENT;
  DMA_InitNodeStruct.DestDataWidth            = LL_DMA_DEST_DATAWIDTH_WORD;
  DMA_InitNodeStruct.SrcAllocatedPort         = LL_DMA_SRC_ALLOCATED_PORT0;
  DMA_InitNodeStruct.SrcBurstLength           = 1U;
  DMA_InitNodeStruct.SrcIncMode               = LL_DMA_SRC_INCREMENT;
  DMA_InitNodeStruct.SrcDataWidth             = LL_DMA_SRC_DATAWIDTH_WORD;
  DMA_InitNodeStruct.TransferEventMode        = LL_DMA_TCEM_LAST_LLITEM_TRANSFER;

  /* Set node data handling parameters */
  DMA_InitNodeStruct.DataAlignment            = LL_DMA_DATA_ALIGN_ZEROPADD;
  DMA_InitNodeStruct.DestHWordExchange        = LL_DMA_DEST_HALFWORD_PRESERVE;
  DMA_InitNodeStruct.DestByteExchange         = LL_DMA_DEST_BYTE_PRESERVE;
  DMA_InitNodeStruct.SrcByteExchange          = LL_DMA_SRC_BYTE_PRESERVE;

  /* Set node trigger parameters */
  DMA_InitNodeStruct.TriggerPolarity          = LL_DMA_TRIG_POLARITY_MASKED;

  /* Set repeated block parameters */
  DMA_InitNodeStruct.BlkRptDestAddrUpdateMode = LL_DMA_BLKRPT_DEST_ADDR_INCREMENT;
  DMA_InitNodeStruct.BlkRptSrcAddrUpdateMode  = LL_DMA_BLKRPT_SRC_ADDR_INCREMENT;
  DMA_InitNodeStruct.DestAddrUpdateMode       = LL_DMA_BURST_DEST_ADDR_INCREMENT;
  DMA_InitNodeStruct.SrcAddrUpdateMode        = LL_DMA_BURST_SRC_ADDR_INCREMENT;
  DMA_InitNodeStruct.BlkRptCount              = 0U;
  DMA_InitNodeStruct.DestAddrOffset           = 0U;
  DMA_InitNodeStruct.SrcAddrOffset            = 0U;
  DMA_InitNodeStruct.BlkRptDestAddrOffset     = 0U;
  DMA_InitNodeStruct.BlkRptSrcAddrOffset      = 0U;

  /* Set registers to be updated */
  DMA_InitNodeStruct.UpdateRegisters          = DEFAULT_REGISTER_UPDATE;

  /* Set transfer parameters */
  DMA_InitNodeStruct.SrcAddress               = (uint32_t)aSRC_Buffer1;
  DMA_InitNodeStruct.DestAddress              = (uint32_t)aDST_Buffer1;
  DMA_InitNodeStruct.BlkDataLength            = (BUFFER1_SIZE * 4U);

  /* Initializes DMA linked list node */
  LL_DMA_CreateLinkNode(&DMA_InitNodeStruct, &pNode1);


  /* Set node 2 configuration ################################################*/
  /* Set node type */
  DMA_InitNodeStruct.NodeType                 = LL_DMA_GPDMA_2D_NODE;
  /* Set node configuration */
  DMA_InitNodeStruct.BlkHWRequest             = LL_DMA_HWREQUEST_SINGLEBURST;
  DMA_InitNodeStruct.Direction                = LL_DMA_DIRECTION_MEMORY_TO_MEMORY;
  DMA_InitNodeStruct.DestAllocatedPort        = LL_DMA_DEST_ALLOCATED_PORT0;
  DMA_InitNodeStruct.DestBurstLength          = 1U;
  DMA_InitNodeStruct.DestIncMode              = LL_DMA_DEST_INCREMENT;
  DMA_InitNodeStruct.DestDataWidth            = LL_DMA_DEST_DATAWIDTH_BYTE;
  DMA_InitNodeStruct.SrcAllocatedPort         = LL_DMA_SRC_ALLOCATED_PORT1;
  DMA_InitNodeStruct.SrcBurstLength           = 1U;
  DMA_InitNodeStruct.SrcIncMode               = LL_DMA_SRC_INCREMENT;
  DMA_InitNodeStruct.SrcDataWidth             = LL_DMA_SRC_DATAWIDTH_BYTE;
  DMA_InitNodeStruct.TransferEventMode        = LL_DMA_TCEM_LAST_LLITEM_TRANSFER;

  /* Set node data handling parameters */
  DMA_InitNodeStruct.DataAlignment            = LL_DMA_DATA_ALIGN_ZEROPADD;
  DMA_InitNodeStruct.DestHWordExchange        = LL_DMA_DEST_HALFWORD_PRESERVE;
  DMA_InitNodeStruct.DestByteExchange         = LL_DMA_DEST_BYTE_PRESERVE;
  DMA_InitNodeStruct.SrcByteExchange          = LL_DMA_SRC_BYTE_PRESERVE;

  /* Set node trigger parameters */
  DMA_InitNodeStruct.TriggerPolarity          = LL_DMA_TRIG_POLARITY_MASKED;

  /* Set repeated block parameters */
  DMA_InitNodeStruct.BlkRptDestAddrUpdateMode = LL_DMA_BLKRPT_DEST_ADDR_INCREMENT;
  DMA_InitNodeStruct.BlkRptSrcAddrUpdateMode  = LL_DMA_BLKRPT_SRC_ADDR_INCREMENT;
  DMA_InitNodeStruct.DestAddrUpdateMode       = LL_DMA_BURST_DEST_ADDR_INCREMENT;
  DMA_InitNodeStruct.SrcAddrUpdateMode        = LL_DMA_BURST_SRC_ADDR_INCREMENT;
  DMA_InitNodeStruct.BlkRptCount              = 0U;
  DMA_InitNodeStruct.DestAddrOffset           = 0U;
  DMA_InitNodeStruct.SrcAddrOffset            = 0U;
  DMA_InitNodeStruct.BlkRptDestAddrOffset     = 0U;
  DMA_InitNodeStruct.BlkRptSrcAddrOffset      = 0U;

  /* Set registers to be updated */
  DMA_InitNodeStruct.UpdateRegisters          = LL_DMA_UPDATE_CTR1 | \
                                                LL_DMA_UPDATE_CBR1 | LL_DMA_UPDATE_CSAR | \
                                                LL_DMA_UPDATE_CDAR | LL_DMA_UPDATE_CLLR;

  /* Set transfer parameters */
  DMA_InitNodeStruct.SrcAddress               = (uint32_t)aSRC_Buffer2;
  DMA_InitNodeStruct.DestAddress              = (uint32_t)aDST_Buffer2;
  DMA_InitNodeStruct.BlkDataLength            = (BUFFER2_SIZE * 4U);

  /* Initializes DMA linked list node */
  LL_DMA_CreateLinkNode(&DMA_InitNodeStruct, &pNode2);


  /* Set node 3 configuration ################################################*/
  /* Set node type */
  DMA_InitNodeStruct.NodeType                 = LL_DMA_GPDMA_2D_NODE;
  /* Set node configuration */
  DMA_InitNodeStruct.BlkHWRequest             = LL_DMA_HWREQUEST_SINGLEBURST;
  DMA_InitNodeStruct.Direction                = LL_DMA_DIRECTION_MEMORY_TO_MEMORY;
  DMA_InitNodeStruct.DestAllocatedPort        = LL_DMA_DEST_ALLOCATED_PORT1;
  DMA_InitNodeStruct.DestBurstLength          = 1U;
  DMA_InitNodeStruct.DestIncMode              = LL_DMA_DEST_INCREMENT;
  DMA_InitNodeStruct.DestDataWidth            = LL_DMA_DEST_DATAWIDTH_HALFWORD;
  DMA_InitNodeStruct.SrcAllocatedPort         = LL_DMA_SRC_ALLOCATED_PORT0;
  DMA_InitNodeStruct.SrcBurstLength           = 1U;
  DMA_InitNodeStruct.SrcIncMode               = LL_DMA_SRC_INCREMENT;
  DMA_InitNodeStruct.SrcDataWidth             = LL_DMA_SRC_DATAWIDTH_HALFWORD;
  DMA_InitNodeStruct.TransferEventMode        = LL_DMA_TCEM_LAST_LLITEM_TRANSFER;

  /* Set node data handling parameters */
  DMA_InitNodeStruct.DataAlignment            = LL_DMA_DATA_PACK_UNPACK;
  DMA_InitNodeStruct.DestHWordExchange        = LL_DMA_DEST_HALFWORD_PRESERVE;
  DMA_InitNodeStruct.DestByteExchange         = LL_DMA_DEST_BYTE_PRESERVE;
  DMA_InitNodeStruct.SrcByteExchange          = LL_DMA_SRC_BYTE_PRESERVE;

  /* Set node trigger parameters */
  DMA_InitNodeStruct.TriggerPolarity          = LL_DMA_TRIG_POLARITY_MASKED;

  /* Set repeated block parameters */
  DMA_InitNodeStruct.BlkRptDestAddrUpdateMode = LL_DMA_BLKRPT_DEST_ADDR_INCREMENT;
  DMA_InitNodeStruct.BlkRptSrcAddrUpdateMode  = LL_DMA_BLKRPT_SRC_ADDR_INCREMENT;
  DMA_InitNodeStruct.DestAddrUpdateMode       = LL_DMA_BURST_DEST_ADDR_INCREMENT;
  DMA_InitNodeStruct.SrcAddrUpdateMode        = LL_DMA_BURST_SRC_ADDR_INCREMENT;
  DMA_InitNodeStruct.BlkRptCount              = 0U;
  DMA_InitNodeStruct.DestAddrOffset           = 0U;
  DMA_InitNodeStruct.SrcAddrOffset            = 0U;
  DMA_InitNodeStruct.BlkRptDestAddrOffset     = 0U;
  DMA_InitNodeStruct.BlkRptSrcAddrOffset      = 0U;

  /* Set registers to be updated */
  DMA_InitNodeStruct.UpdateRegisters          = LL_DMA_UPDATE_CTR1 | \
                                                LL_DMA_UPDATE_CBR1 | LL_DMA_UPDATE_CSAR | \
                                                LL_DMA_UPDATE_CDAR | LL_DMA_UPDATE_CLLR;

  /* Set transfer parameters */
  DMA_InitNodeStruct.SrcAddress               = (uint32_t)aSRC_Buffer3;
  DMA_InitNodeStruct.DestAddress              = (uint32_t)aDST_Buffer3;
  DMA_InitNodeStruct.BlkDataLength            = (BUFFER3_SIZE * 4U);

  /* Initializes DMA linked list node */
  LL_DMA_CreateLinkNode(&DMA_InitNodeStruct, &pNode3);


  /* Build linked-list Queue #################################################*/
  /* Connect Node1 to Node2 */
  LL_DMA_ConnectLinkNode(&pNode1, LL_DMA_CLLR_OFFSET5, &pNode2, LL_DMA_CLLR_OFFSET4);
  /* Connect Node2 to Node3 */
  LL_DMA_ConnectLinkNode(&pNode2, LL_DMA_CLLR_OFFSET4, &pNode3, LL_DMA_CLLR_OFFSET4);
  /* Disconnect Node3 */
  LL_DMA_DisconnectNextLinkNode(&pNode3, LL_DMA_CLLR_OFFSET4);

  /* Initialize DMA channel ##################################################*/
  /* Enable DMA clock */
  DMA_CLK_ENABLE();

  /* Set DMA channel parameter to be configured */
  DMA_InitLinkedListStruct.Priority          = LL_DMA_HIGH_PRIORITY;
  DMA_InitLinkedListStruct.TransferEventMode = LL_DMA_TCEM_LAST_LLITEM_TRANSFER;
  DMA_InitLinkedListStruct.LinkStepMode      = LL_DMA_LSM_FULL_EXECUTION;
  DMA_InitLinkedListStruct.LinkAllocatedPort = LL_DMA_LINK_ALLOCATED_PORT1;

  /* Initialize the DMA linked list */
  LL_DMA_List_Init(GPDMA1, LL_DMA_CHANNEL_13, &DMA_InitLinkedListStruct);


  /* Link created queue to DMA channel #######################################*/
  LL_DMA_SetLinkedListBaseAddr(GPDMA1, LL_DMA_CHANNEL_13, (uint32_t)&pNode1);
  LL_DMA_ConfigLinkUpdate(GPDMA1, LL_DMA_CHANNEL_13, DEFAULT_REGISTER_UPDATE, (uint32_t)&pNode1);

  /* Enable DMA interrupts */
  LL_DMA_EnableIT_USE(GPDMA1, LL_DMA_CHANNEL_13);
  LL_DMA_EnableIT_ULE(GPDMA1, LL_DMA_CHANNEL_13);
  LL_DMA_EnableIT_DTE(GPDMA1, LL_DMA_CHANNEL_13);
  LL_DMA_EnableIT_TC(GPDMA1, LL_DMA_CHANNEL_13);

  /* DMA channel 13 interrupt init */
  NVIC_SetPriority(GPDMA1_Channel13_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(GPDMA1_Channel13_IRQn);
}

/**
  * @brief  This function configures LEDs GPIO
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
static void Configure_GPIO(void)
{
  /* Enable the SUCCESS LED Clock */
  SUCCESS_LED_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external SUCCESS LED */
  LL_GPIO_SetPinMode(SUCCESS_LED_GPIO_PORT, SUCCESS_LED_PIN, LL_GPIO_MODE_OUTPUT);


  /* Enable the ERROR LED Clock */
  ERROR_LED_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external ERROR LED */
  LL_GPIO_SetPinMode(ERROR_LED_GPIO_PORT, ERROR_LED_PIN, LL_GPIO_MODE_OUTPUT);
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
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_MBOOST                     = 1
  *            PLL_M                          = 1
  *            PLL_N                          = 80
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            PLL_P                          = 2
  *            Flash Latency(WS)              = 4
  */
static void SystemClock_Config(void)
{
  /* Enable PWR clock */
  LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_PWR);

  /* Set the regulator supply output voltage */
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }

  /* Switch to SMPS regulator instead of LDO */
  LL_PWR_SetRegulatorSupply(LL_PWR_SMPS_SUPPLY);
  while(LL_PWR_IsActiveFlag_REGULATOR() != 1)
  {
  }

  /* Enable MSI oscillator */
  LL_RCC_MSIS_SetRange(LL_RCC_MSISRANGE_4);
  LL_RCC_MSI_SetCalibTrimming(10, LL_RCC_MSI_OSCILLATOR_0);
  LL_RCC_MSIS_Enable();
  while(LL_RCC_MSIS_IsReady() != 1)
  {
  }

  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);

  /* Configure PLL clock source */
  LL_RCC_PLL1_SetMainSource(LL_RCC_PLL1SOURCE_MSIS);

  /* Enable the EPOD to reach max frequency */
  LL_PWR_EnableEPODBooster();
  while (LL_PWR_IsActiveFlag_BOOST() == 0)
  {
  }

  /* Main PLL configuration and activation */
  LL_RCC_PLL1_EnableDomain_SYS();
  LL_RCC_PLL1FRACN_Disable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_4_8);
  LL_RCC_SetPll1EPodPrescaler(LL_RCC_PLL1MBOOST_DIV_1);
  LL_RCC_PLL1_SetDivider(1);
  LL_RCC_PLL1_SetN(80);
  LL_RCC_PLL1_SetP(2);
  LL_RCC_PLL1_SetQ(2);
  LL_RCC_PLL1_SetR(2);

  LL_RCC_PLL1_Enable();
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

  /* Set AHB, APB1, APB2 and APB3 prescalers */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_1);

  /* Set PLL1 as System Clock Source */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {
  }

 /* Set systick to 1ms with frequency set to 160MHz */
  LL_Init1msTick(160000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(160000000);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn ERROR LED on */
  LL_GPIO_SetOutputPin(ERROR_LED_GPIO_PORT, ERROR_LED_PIN);

  /* Infinite loop */
  while(1)
  {
  }
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

  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */
