/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file   lpbam_switchq_twoqexec_build.c
  * @author MCD Application Team
  * @brief  Provides LPBAM SwitchQ application TwoQExec scenario build services
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
#include "lpbam_switchq.h"

/* Private variables -------------------------------------------------------------------------------------------------*/
/* LPBAM variables declaration */
static LPBAM_GPIO_WritePinSeqFullDesc_t WritePinSequence_0_Desc;
static LPBAM_GPIO_WritePinSeqFullDesc_t WritePinSequence_1_Desc;
static LPBAM_DMA_StartFullDesc_t Start_1_Desc;
static LPBAM_DMA_StartFullDesc_t Start_2_Desc;

/* Exported variables ------------------------------------------------------------------------------------------------*/
/* LPBAM queues declaration */
DMA_QListTypeDef Slave_Toggle_IO0_Q;
DMA_QListTypeDef Slave_Toggle_IO1_Q;
DMA_QListTypeDef Master_SwitchQueues_Q;

/* External variables ------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN EV */
extern uint32_t ToggleIO0_Buffer[TOGGLE_BUFFER_SIZE];
extern uint32_t ToggleIO1_Buffer[TOGGLE_BUFFER_SIZE];
/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static void MX_Slave_Toggle_IO0_Q_Build(void);
static void MX_Slave_Toggle_IO1_Q_Build(void);
static void MX_Master_SwitchQueues_Q_Build(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
  * @brief SwitchQ application TwoQExec scenario build
  * @param None
  * @retval None
  */
void MX_SwitchQ_TwoQExec_Build(void)
{
  /* USER CODE BEGIN SwitchQ_TwoQExec_Build 0 */

  /* USER CODE END SwitchQ_TwoQExec_Build 0 */

  /* LPBAM build Slave_Toggle_IO0 queue */
  MX_Slave_Toggle_IO0_Q_Build();
  /* LPBAM build Slave_Toggle_IO1 queue */
  MX_Slave_Toggle_IO1_Q_Build();
  /* LPBAM build Master_SwitchQueues queue */
  MX_Master_SwitchQueues_Q_Build();

  /* USER CODE BEGIN SwitchQ_TwoQExec_Build 1 */

  /* USER CODE END SwitchQ_TwoQExec_Build 1 */
}

/* Private functions -------------------------------------------------------------------------------------------------*/

/**
  * @brief  SwitchQ application TwoQExec scenario Slave_Toggle_IO0 queue build
  * @param  None
  * @retval None
  */
static void MX_Slave_Toggle_IO0_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_LPGPIO = {0};
  LPBAM_GPIO_PinSeqFullAdvConf_t pWritePinSeqFull_LPGPIO = {0};
  LPBAM_COMMON_TrigAdvConf_t pTrigConfig_LPGPIO = {0};

  /**
    * Slave_Toggle_IO0 queue WritePinSequence_0 build
    */
  pDMAListInfo_LPGPIO.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_LPGPIO.pInstance = LPDMA1;
  pWritePinSeqFull_LPGPIO.Pin = LPBAM_GPIO_PIN_0;
  pWritePinSeqFull_LPGPIO.Size = 2;
  pWritePinSeqFull_LPGPIO.pData = (uint32_t*)&ToggleIO0_Buffer[0];
  if (ADV_LPBAM_GPIO_WritePinSequence_SetFullQ(LPGPIO1, &pDMAListInfo_LPGPIO, &pWritePinSeqFull_LPGPIO, &WritePinSequence_0_Desc, &Slave_Toggle_IO0_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_LPGPIO.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_SINGLE_BURST_TRANSFER;
  pTrigConfig_LPGPIO.TriggerConfig.TriggerPolarity = LPBAM_DMA_TRIG_POLARITY_RISING;
  pTrigConfig_LPGPIO.TriggerConfig.TriggerSelection = LPBAM_LPDMA1_TRIGGER_LPTIM3_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_LPGPIO, LPBAM_GPIO_WRITEPINSEQ_FULLQ_DATA_NODE, &WritePinSequence_0_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Set circular mode
    */
  if (ADV_LPBAM_Q_SetCircularMode(&WritePinSequence_0_Desc, LPBAM_GPIO_WRITEPINSEQ_FULLQ_DATA_NODE, &Slave_Toggle_IO0_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  SwitchQ application TwoQExec scenario Slave_Toggle_IO1 queue build
  * @param  None
  * @retval None
  */
static void MX_Slave_Toggle_IO1_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_LPGPIO = {0};
  LPBAM_GPIO_PinSeqFullAdvConf_t pWritePinSeqFull_LPGPIO = {0};
  LPBAM_COMMON_TrigAdvConf_t pTrigConfig_LPGPIO = {0};

  /**
    * Slave_Toggle_IO1 queue WritePinSequence_1 build
    */
  pDMAListInfo_LPGPIO.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_LPGPIO.pInstance = LPDMA1;
  pWritePinSeqFull_LPGPIO.Pin = LPBAM_GPIO_PIN_1;
  pWritePinSeqFull_LPGPIO.Size = 2;
  pWritePinSeqFull_LPGPIO.pData = (uint32_t*)&ToggleIO1_Buffer[0];
  if (ADV_LPBAM_GPIO_WritePinSequence_SetFullQ(LPGPIO1, &pDMAListInfo_LPGPIO, &pWritePinSeqFull_LPGPIO, &WritePinSequence_1_Desc, &Slave_Toggle_IO1_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_LPGPIO.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_SINGLE_BURST_TRANSFER;
  pTrigConfig_LPGPIO.TriggerConfig.TriggerPolarity = LPBAM_DMA_TRIG_POLARITY_RISING;
  pTrigConfig_LPGPIO.TriggerConfig.TriggerSelection = LPBAM_LPDMA1_TRIGGER_LPTIM3_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_LPGPIO, LPBAM_GPIO_WRITEPINSEQ_FULLQ_DATA_NODE, &WritePinSequence_1_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Set circular mode
    */
  if (ADV_LPBAM_Q_SetCircularMode(&WritePinSequence_1_Desc, LPBAM_GPIO_WRITEPINSEQ_FULLQ_DATA_NODE, &Slave_Toggle_IO1_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  SwitchQ application TwoQExec scenario Master_SwitchQueues queue build
  * @param  None
  * @retval None
  */
static void MX_Master_SwitchQueues_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_LPDMA = {0};
  LPBAM_DMA_StartFullAdvConf_t pStartFull_LPDMA = {0};
  LPBAM_COMMON_TrigAdvConf_t pTrigConfig_LPDMA = {0};

  /**
    * Master_SwitchQueues queue Start_1 build
    */
  pDMAListInfo_LPDMA.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_LPDMA.pInstance = LPDMA1;
  pStartFull_LPDMA.WakeupIT = LPBAM_DMA_IT_NONE;
  pStartFull_LPDMA.HeadQAddress = (uint32_t)Slave_Toggle_IO0_Q.Head;
  if (ADV_LPBAM_DMA_Start_SetFullQ(LPDMA1_Channel3, &pDMAListInfo_LPDMA, &pStartFull_LPDMA, &Start_1_Desc, &Master_SwitchQueues_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_LPDMA.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_BLOCK_TRANSFER;
  pTrigConfig_LPDMA.TriggerConfig.TriggerPolarity = LPBAM_DMA_TRIG_POLARITY_RISING;
  pTrigConfig_LPDMA.TriggerConfig.TriggerSelection = LPBAM_LPDMA1_TRIGGER_LPTIM1_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_LPDMA, LPBAM_DMA_START_FULLQ_CONFIG_NODE, &Start_1_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Master_SwitchQueues queue Start_2 build
    */
  pStartFull_LPDMA.HeadQAddress = (uint32_t)Slave_Toggle_IO1_Q.Head;
  if (ADV_LPBAM_DMA_Start_SetFullQ(LPDMA1_Channel3, &pDMAListInfo_LPDMA, &pStartFull_LPDMA, &Start_2_Desc, &Master_SwitchQueues_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_LPDMA, LPBAM_DMA_START_FULLQ_CONFIG_NODE, &Start_2_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Set circular mode
    */
  if (ADV_LPBAM_Q_SetCircularMode(&Start_1_Desc, LPBAM_DMA_START_FULLQ_CONFIG_NODE, &Master_SwitchQueues_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN SwitchQ_TwoQExec_Build */

/* USER CODE END SwitchQ_TwoQExec_Build */
