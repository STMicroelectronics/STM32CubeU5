/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file   lpbam_conv_analogwdg_build.c
  * @author MCD Application Team
  * @brief  Provides LPBAM Conv application AnalogWDG build services
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
#include "lpbam_conv.h"

/* Private variables -------------------------------------------------------------------------------------------------*/
/* LPBAM variables declaration */
/* USER CODE BEGIN Conv_AnalogWDG_Descs 0 */

/* USER CODE END Conv_AnalogWDG_Descs 0 */

/* USER CODE BEGIN Conversion_Q_Conversion_Data_1_Desc */

/* USER CODE END Conversion_Q_Conversion_Data_1_Desc */
static LPBAM_ADC_ConvDataDesc_t Conversion_Q_Conversion_Data_1_Desc;

/* USER CODE BEGIN Conversion_Q_Conversion_1_Desc */

/* USER CODE END Conversion_Q_Conversion_1_Desc */
static LPBAM_ADC_ConvFullDesc_t Conversion_Q_Conversion_1_Desc;

/* USER CODE BEGIN Conv_AnalogWDG_Descs 1 */

/* USER CODE END Conv_AnalogWDG_Descs 1 */

/* Exported variables ------------------------------------------------------------------------------------------------*/
/* LPBAM queues declaration */
DMA_QListTypeDef Conversion_Q;

/* External variables ------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN EV */
/* LPBAM user buffers declaration */
extern uint16_t aData_Sequence1[32];
extern uint16_t aData_Sequence2[32];
/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static void MX_Conversion_Q_Build(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
  * @brief Conv application AnalogWDG scenario build
  * @param None
  * @retval None
  */
void MX_Conv_AnalogWDG_Build(void)
{
  /* USER CODE BEGIN Conv_AnalogWDG_Build 0 */

  /* USER CODE END Conv_AnalogWDG_Build 0 */

  /* LPBAM build Conversion queue */
  MX_Conversion_Q_Build();

  /* USER CODE BEGIN Conv_AnalogWDG_Build 1 */

  /* USER CODE END Conv_AnalogWDG_Build 1 */
}

/* Private functions -------------------------------------------------------------------------------------------------*/

/**
  * @brief  Conv application AnalogWDG scenario Conversion queue build
  * @param  None
  * @retval None
  */
static void MX_Conversion_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_ADC = {0};
  LPBAM_ADC_DataAdvConf_t pConvData_ADC = {0};
  LPBAM_ADC_FullAdvConf_t pConvFull_ADC = {0};
  LPBAM_ADC_ChannelAdvConf_t pChannel_ADC = {0};

  /**
    * Conversion queue Conversion_Data_1 build
    */
  pDMAListInfo_ADC.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_ADC.pInstance = LPDMA1;
  pConvData_ADC.DMAContinuousRequests = DISABLE;
  pConvData_ADC.Size = 32;
  pConvData_ADC.pData = (uint32_t*)&aData_Sequence1[0];
  if (ADV_LPBAM_ADC_Conversion_SetDataQ(ADC4, &pDMAListInfo_ADC, &pConvData_ADC, &Conversion_Q_Conversion_Data_1_Desc, &Conversion_Q) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Conversion queue Conversion_1 build
    */
  pConvFull_ADC.ScanConvMode = LPBAM_ADC_SCAN_DIRECTION_FORWARD;
  pConvFull_ADC.ContinuousConvMode = DISABLE;
  pConvFull_ADC.DiscontinuousConvMode = DISABLE;
  pConvFull_ADC.ExternalTrigConv = LPBAM_ADC_EXTERNALTRIG_LPTIM1_CH1;
  pConvFull_ADC.ExternalTrigConvEdge = LPBAM_ADC_EXTERNALTRIGCONVEDGE_RISING;
  pConvFull_ADC.TriggerFrequencyMode = LPBAM_ADC_TRIGGER_FREQ_LOW;
  pConvFull_ADC.LowPowerAutoWait = ENABLE;
  pConvFull_ADC.DMAContinuousRequests = ENABLE;
  pConvFull_ADC.Size = 32;
  pConvFull_ADC.pData = (uint32_t*)&aData_Sequence2[0];
  pConvFull_ADC.AnalogWDGConfig[0U].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_1;
  pConvFull_ADC.AnalogWDGConfig[0U].WatchdogMode = LPBAM_ADC_ANALOGWATCHDOG_SINGLE_REG;
  pConvFull_ADC.AnalogWDGConfig[0U].Channel = LPBAM_ADC_CHANNEL_4;
  pConvFull_ADC.AnalogWDGConfig[0U].ITMode = ENABLE;
  pConvFull_ADC.AnalogWDGConfig[0U].LowThreshold = 0;
  pConvFull_ADC.AnalogWDGConfig[0U].HighThreshold = 2047;
  if (ADV_LPBAM_ADC_Conversion_SetFullQ(ADC4, &pDMAListInfo_ADC, &pConvFull_ADC, &Conversion_Q_Conversion_1_Desc, &Conversion_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pChannel_ADC.ChannelConfig.Channel = LPBAM_ADC_CHANNEL_4;
  pChannel_ADC.ChannelConfig.Rank = LPBAM_ADC_REGULAR_RANK_1;
  if (ADV_LPBAM_ADC_ChannelConfig(&pChannel_ADC, &Conversion_Q_Conversion_1_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Set circular mode
    */
  if (ADV_LPBAM_Q_SetCircularMode(&Conversion_Q_Conversion_1_Desc, LPBAM_ADC_CONVERSION_FULLQ_DATA_NODE, &Conversion_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN Conv_AnalogWDG_Build */

/* USER CODE END Conv_AnalogWDG_Build */
