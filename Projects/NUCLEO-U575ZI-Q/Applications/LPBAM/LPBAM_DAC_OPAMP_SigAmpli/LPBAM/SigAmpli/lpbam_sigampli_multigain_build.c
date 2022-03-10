/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file   lpbam_sigampli_multigain_build.c
  * @author MCD Application Team
  * @brief  Provides LPBAM SigAmpli application MultiGain scenario build services
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
#include "lpbam_sigampli.h"

/* Private variables -------------------------------------------------------------------------------------------------*/
/* LPBAM variables declaration */
static LPBAM_DAC_ConvDataDesc_t Conversion_Data_Desc;
static LPBAM_OPAMP_StartFullDesc_t Start_1_Desc;
static LPBAM_OPAMP_StartFullDesc_t Start_2_Desc;
static LPBAM_OPAMP_StartFullDesc_t Start_3_Desc;
static LPBAM_OPAMP_StartFullDesc_t Start_4_Desc;

/* Exported variables ------------------------------------------------------------------------------------------------*/
/* LPBAM queues declaration */
DMA_QListTypeDef DAC_Conversion_Q;
DMA_QListTypeDef OPAMP_Config_Q;

/* External variables ------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN EV */
extern uint32_t TriangularSignal_Buffer[DAC_TRIANGULAR_SIGNAL_DATASIZE];
/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static void MX_DAC_Conversion_Q_Build(void);
static void MX_OPAMP_Config_Q_Build(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
  * @brief SigAmpli application MultiGain scenario build
  * @param None
  * @retval None
  */
void MX_SigAmpli_MultiGain_Build(void)
{
  /* USER CODE BEGIN SigAmpli_MultiGain_Build 0 */

  /* USER CODE END SigAmpli_MultiGain_Build 0 */

  /* LPBAM build DAC_Conversion queue */
  MX_DAC_Conversion_Q_Build();
  /* LPBAM build OPAMP_Config queue */
  MX_OPAMP_Config_Q_Build();

  /* USER CODE BEGIN SigAmpli_MultiGain_Build 1 */

  /* USER CODE END SigAmpli_MultiGain_Build 1 */
}

/* Private functions -------------------------------------------------------------------------------------------------*/

/**
  * @brief  SigAmpli application MultiGain scenario DAC_Conversion queue build
  * @param  None
  * @retval None
  */
static void MX_DAC_Conversion_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_DAC = {0};
  LPBAM_DAC_DataAdvConf_t pSetData_DAC = {0};
  LPBAM_COMMON_DataAdvConf_t pDataConfig_DAC = {0};

  /**
    * DAC_Conversion queue Conversion_Data build
    */
  pDMAListInfo_DAC.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_DAC.pInstance = LPDMA1;
  pSetData_DAC.Alignment = LPBAM_DAC_ALIGN_12B_R;
  pSetData_DAC.Size = 940;
  pSetData_DAC.pData = (uint32_t*)&TriangularSignal_Buffer[0];
  if (ADV_LPBAM_DAC_Conversion_SetDataQ(DAC1, LPBAM_DAC_CHANNEL_1, &pDMAListInfo_DAC, &pSetData_DAC, &Conversion_Data_Desc, &DAC_Conversion_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pDataConfig_DAC.TransferConfig.Transfer.SrcInc = LPBAM_DMA_SINC_INCREMENTED;
  pDataConfig_DAC.TransferConfig.Transfer.DestInc = LPBAM_DMA_DINC_FIXED;
  pDataConfig_DAC.TransferConfig.Transfer.SrcDataWidth = LPBAM_DMA_SRC_DATAWIDTH_WORD;
  pDataConfig_DAC.TransferConfig.Transfer.DestDataWidth = LPBAM_DMA_DEST_DATAWIDTH_WORD;
  pDataConfig_DAC.TransferConfig.Transfer.TransferEventMode = LPBAM_DMA_TCEM_BLOCK_TRANSFER;
  pDataConfig_DAC.UpdateSrcInc = DISABLE;
  pDataConfig_DAC.UpdateDestInc = DISABLE;
  pDataConfig_DAC.UpdateSrcDataWidth = DISABLE;
  pDataConfig_DAC.UpdateDestDataWidth = DISABLE;
  pDataConfig_DAC.UpdateTransferEventMode = ENABLE;
  if (ADV_LPBAM_Q_SetDataConfig(&pDataConfig_DAC, LPBAM_DAC_CONVERSION_DATAQ_DATA_NODE, &Conversion_Data_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Set circular mode
    */
  if (ADV_LPBAM_Q_SetCircularMode(&Conversion_Data_Desc, LPBAM_DAC_CONVERSION_DATAQ_DATA_NODE, &DAC_Conversion_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  SigAmpli application MultiGain scenario OPAMP_Config queue build
  * @param  None
  * @retval None
  */
static void MX_OPAMP_Config_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_OPAMP = {0};
  LPBAM_OPAMP_StartFullAdvConf_t pStartFull_OPAMP = {0};
  LPBAM_COMMON_TrigAdvConf_t pTrigConfig_OPAMP = {0};

  /**
    * OPAMP_Config queue Start_1 build
    */
  pDMAListInfo_OPAMP.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_OPAMP.pInstance = LPDMA1;
  pStartFull_OPAMP.Mode = LPBAM_OPAMP_PGA_MODE;
  pStartFull_OPAMP.NonInvertingInput = LPBAM_OPAMP_NONINVERTINGINPUT_DAC_CH;
  pStartFull_OPAMP.InvertingInput = LPBAM_OPAMP_INVERTINGINPUT_CONNECT_NO;
  pStartFull_OPAMP.PgaGain = LPBAM_OPAMP_PGA_GAIN_2;
  if (ADV_LPBAM_OPAMP_Start_SetFullQ(OPAMP1, &pDMAListInfo_OPAMP, &pStartFull_OPAMP, &Start_1_Desc, &OPAMP_Config_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_OPAMP.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_BLOCK_TRANSFER;
  pTrigConfig_OPAMP.TriggerConfig.TriggerPolarity = LPBAM_DMA_TRIG_POLARITY_RISING;
  pTrigConfig_OPAMP.TriggerConfig.TriggerSelection = LPBAM_LPDMA1_TRIGGER_LPDMA1_CH0_TCF;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_OPAMP, LPBAM_OPAMP_START_FULLQ_CONFIG_NODE, &Start_1_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * OPAMP_Config queue Start_2 build
    */
  pStartFull_OPAMP.PgaGain = LPBAM_OPAMP_PGA_GAIN_4;
  if (ADV_LPBAM_OPAMP_Start_SetFullQ(OPAMP1, &pDMAListInfo_OPAMP, &pStartFull_OPAMP, &Start_2_Desc, &OPAMP_Config_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_OPAMP, LPBAM_OPAMP_START_FULLQ_CONFIG_NODE, &Start_2_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * OPAMP_Config queue Start_3 build
    */
  pStartFull_OPAMP.PgaGain = LPBAM_OPAMP_PGA_GAIN_8;
  if (ADV_LPBAM_OPAMP_Start_SetFullQ(OPAMP1, &pDMAListInfo_OPAMP, &pStartFull_OPAMP, &Start_3_Desc, &OPAMP_Config_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_OPAMP, LPBAM_OPAMP_START_FULLQ_CONFIG_NODE, &Start_3_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * OPAMP_Config queue Start_4 build
    */
  pStartFull_OPAMP.PgaGain = LPBAM_OPAMP_PGA_GAIN_16;
  if (ADV_LPBAM_OPAMP_Start_SetFullQ(OPAMP1, &pDMAListInfo_OPAMP, &pStartFull_OPAMP, &Start_4_Desc, &OPAMP_Config_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_OPAMP, LPBAM_OPAMP_START_FULLQ_CONFIG_NODE, &Start_4_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Set circular mode
    */
  if (ADV_LPBAM_Q_SetCircularMode(&Start_1_Desc, LPBAM_OPAMP_START_FULLQ_CONFIG_NODE, &OPAMP_Config_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN SigAmpli_MultiGain_Build */

/* USER CODE END SigAmpli_MultiGain_Build */
