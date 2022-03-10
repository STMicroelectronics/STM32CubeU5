/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file   lpbam_writeio_writeseq_build.c
  * @author MCD Application Team
  * @brief  Provides LPBAM WriteIO application WriteSeq build services
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
#include "lpbam_writeio.h"

/* Private variables -------------------------------------------------------------------------------------------------*/
/* LPBAM variables declaration */
static LPBAM_GPIO_WritePinSeqFullDesc_t WritePinSequence_1_Desc;

/* Exported variables ------------------------------------------------------------------------------------------------*/
/* LPBAM queues declaration */
DMA_QListTypeDef ToggleIO_Q;

/* External variables ------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN EV */
/* Buffer declaration */
extern uint32_t LPGPIOBufferState[LPGPIO_BUFFER_SIZE];
/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static void MX_ToggleIO_Q_Build(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
  * @brief WriteIO application WriteSeq scenario build
  * @param None
  * @retval None
  */
void MX_WriteIO_WriteSeq_Build(void)
{
  /* USER CODE BEGIN WriteIO_WriteSeq_Build 0 */

  /* USER CODE END WriteIO_WriteSeq_Build 0 */

  /* LPBAM build ToggleIO queue */
  MX_ToggleIO_Q_Build();

  /* USER CODE BEGIN WriteIO_WriteSeq_Build 1 */

  /* USER CODE END WriteIO_WriteSeq_Build 1 */
}

/* Private functions -------------------------------------------------------------------------------------------------*/

/**
  * @brief  WriteIO application WriteSeq scenario ToggleIO queue build
  * @param  None
  * @retval None
  */
static void MX_ToggleIO_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_LPGPIO = {0};
  LPBAM_GPIO_PinSeqFullAdvConf_t pWritePinSeqFull_LPGPIO = {0};
  LPBAM_COMMON_TrigAdvConf_t pTrigConfig_LPGPIO = {0};

  /**
    * ToggleIO queue WritePinSequence_1 build
    */
  pDMAListInfo_LPGPIO.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_LPGPIO.pInstance = LPDMA1;
  pWritePinSeqFull_LPGPIO.Pin = LPBAM_GPIO_PIN_0;
  pWritePinSeqFull_LPGPIO.Size = 2;
  pWritePinSeqFull_LPGPIO.pData = (uint32_t*)&LPGPIOBufferState[0];
  if (ADV_LPBAM_GPIO_WritePinSequence_SetFullQ(LPGPIO1, &pDMAListInfo_LPGPIO, &pWritePinSeqFull_LPGPIO, &WritePinSequence_1_Desc, &ToggleIO_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_LPGPIO.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_SINGLE_BURST_TRANSFER;
  pTrigConfig_LPGPIO.TriggerConfig.TriggerPolarity = LPBAM_DMA_TRIG_POLARITY_RISING;
  pTrigConfig_LPGPIO.TriggerConfig.TriggerSelection = LPBAM_LPDMA1_TRIGGER_LPTIM1_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_LPGPIO, LPBAM_GPIO_WRITEPINSEQ_FULLQ_DATA_NODE, &WritePinSequence_1_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Set circular mode
    */
  if (ADV_LPBAM_Q_SetCircularMode(&WritePinSequence_1_Desc, LPBAM_GPIO_WRITEPINSEQ_FULLQ_DATA_NODE, &ToggleIO_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN WriteIO_WriteSeq_Build */

/* USER CODE END WriteIO_WriteSeq_Build */
