/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file   lpbam_tempsens_multithres_build.c
  * @author MCD Application Team
  * @brief  Provides LPBAM TempSens application MultiThres scenario build services
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "lpbam_tempsens.h"

/* Private variables -------------------------------------------------------------------------------------------------*/
/* LPBAM variables declaration */
/* USER CODE BEGIN TempSens_MultiThres_Descs 0 */

/* USER CODE END TempSens_MultiThres_Descs 0 */

/* USER CODE BEGIN Threshold1_Conversion_Q_ConvData_Threshold1_Desc */

/* USER CODE END Threshold1_Conversion_Q_ConvData_Threshold1_Desc */
static LPBAM_ADC_ConvDataDesc_t Threshold1_Conversion_Q_ConvData_Threshold1_Desc;

/* USER CODE BEGIN Threshold2_Conversion_Q_ConvData_Threshold2_Desc */

/* USER CODE END Threshold2_Conversion_Q_ConvData_Threshold2_Desc */
static LPBAM_ADC_ConvDataDesc_t Threshold2_Conversion_Q_ConvData_Threshold2_Desc;

/* USER CODE BEGIN Thresholdx_Config_Q_Threshold1_Q_Start_Desc */

/* USER CODE END Thresholdx_Config_Q_Threshold1_Q_Start_Desc */
static LPBAM_DMA_StartFullDesc_t Thresholdx_Config_Q_Threshold1_Q_Start_Desc;

/* USER CODE BEGIN Thresholdx_Config_Q_Period_10ms_Desc */

/* USER CODE END Thresholdx_Config_Q_Period_10ms_Desc */
static LPBAM_LPTIM_PWMFullDesc_t Thresholdx_Config_Q_Period_10ms_Desc;

/* USER CODE BEGIN Thresholdx_Config_Q_Conversion_Config_Desc */

/* USER CODE END Thresholdx_Config_Q_Conversion_Config_Desc */
static LPBAM_ADC_ConvConfigDesc_t Thresholdx_Config_Q_Conversion_Config_Desc;

/* USER CODE BEGIN Thresholdx_Config_Q_Threshold2_Q_Start_Desc */

/* USER CODE END Thresholdx_Config_Q_Threshold2_Q_Start_Desc */
static LPBAM_DMA_StartFullDesc_t Thresholdx_Config_Q_Threshold2_Q_Start_Desc;

/* USER CODE BEGIN TempSens_MultiThres_Descs 1 */

/* USER CODE END TempSens_MultiThres_Descs 1 */

/* Exported variables ------------------------------------------------------------------------------------------------*/
/* LPBAM queues declaration */
DMA_QListTypeDef Threshold1_Conversion_Q;
DMA_QListTypeDef Threshold2_Conversion_Q;
DMA_QListTypeDef Thresholdx_Config_Q;

/* External variables ------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN EV */
/* LPBAM user buffers declaration */
extern uint16_t Threshold1_Data_Buffer[512];
extern uint16_t Threshold2_Data_Buffer[512];
/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static void MX_Threshold1_Conversion_Q_Build(void);
static void MX_Threshold2_Conversion_Q_Build(void);
static void MX_Thresholdx_Config_Q_Build(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
  * @brief TempSens application MultiThres scenario build
  * @param None
  * @retval None
  */
void MX_TempSens_MultiThres_Build(void)
{
  /* USER CODE BEGIN TempSens_MultiThres_Build 0 */

  /* USER CODE END TempSens_MultiThres_Build 0 */

  /* LPBAM build Threshold1_Conversion queue */
  MX_Threshold1_Conversion_Q_Build();
  /* LPBAM build Threshold2_Conversion queue */
  MX_Threshold2_Conversion_Q_Build();
  /* LPBAM build Thresholdx_Config queue */
  MX_Thresholdx_Config_Q_Build();

  /* USER CODE BEGIN TempSens_MultiThres_Build 1 */

  /* USER CODE END TempSens_MultiThres_Build 1 */
}

/* Private functions -------------------------------------------------------------------------------------------------*/

/**
  * @brief  TempSens application MultiThres scenario Threshold1_Conversion queue build
  * @param  None
  * @retval None
  */
static void MX_Threshold1_Conversion_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_ADC = {0};
  LPBAM_ADC_DataAdvConf_t pConvData_ADC = {0};

  /**
    * Threshold1_Conversion queue ConvData_Threshold1 build
    */
  pDMAListInfo_ADC.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_ADC.pInstance = LPDMA1;
  pConvData_ADC.DMAContinuousRequests = DISABLE;
  pConvData_ADC.Size = 512;
  pConvData_ADC.pData = (uint32_t*)&Threshold1_Data_Buffer[0];
  if (ADV_LPBAM_ADC_Conversion_SetDataQ(ADC4, &pDMAListInfo_ADC, &pConvData_ADC, &Threshold1_Conversion_Q_ConvData_Threshold1_Desc, &Threshold1_Conversion_Q) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Set circular mode
    */
  if (ADV_LPBAM_Q_SetCircularMode(&Threshold1_Conversion_Q_ConvData_Threshold1_Desc, LPBAM_ADC_CONVERSION_DATAQ_DATA_NODE, &Threshold1_Conversion_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  TempSens application MultiThres scenario Threshold2_Conversion queue build
  * @param  None
  * @retval None
  */
static void MX_Threshold2_Conversion_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_ADC = {0};
  LPBAM_ADC_DataAdvConf_t pConvData_ADC = {0};

  /**
    * Threshold2_Conversion queue ConvData_Threshold2 build
    */
  pDMAListInfo_ADC.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_ADC.pInstance = LPDMA1;
  pConvData_ADC.DMAContinuousRequests = DISABLE;
  pConvData_ADC.Size = 512;
  pConvData_ADC.pData = (uint32_t*)&Threshold2_Data_Buffer[0];
  if (ADV_LPBAM_ADC_Conversion_SetDataQ(ADC4, &pDMAListInfo_ADC, &pConvData_ADC, &Threshold2_Conversion_Q_ConvData_Threshold2_Desc, &Threshold2_Conversion_Q) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Set circular mode
    */
  if (ADV_LPBAM_Q_SetCircularMode(&Threshold2_Conversion_Q_ConvData_Threshold2_Desc, LPBAM_ADC_CONVERSION_DATAQ_DATA_NODE, &Threshold2_Conversion_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  TempSens application MultiThres scenario Thresholdx_Config queue build
  * @param  None
  * @retval None
  */
static void MX_Thresholdx_Config_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_LPDMA = {0};
  LPBAM_DMA_StartFullAdvConf_t pStartFull_LPDMA = {0};
  LPBAM_DMAListInfo_t pDMAListInfo_LPTIM = {0};
  LPBAM_LPTIM_PWMFullAdvConf_t pPWMFull_LPTIM = {0};
  LPBAM_COMMON_TrigAdvConf_t pTrigConfig_LPTIM = {0};
  LPBAM_DMAListInfo_t pDMAListInfo_ADC = {0};
  LPBAM_ADC_ConfigAdvConf_t pConvConfig_ADC = {0};
  LPBAM_ADC_ChannelAdvConf_t pChannel_ADC = {0};

  /**
    * Thresholdx_Config queue Threshold1_Q_Start build
    */
  pDMAListInfo_LPDMA.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_LPDMA.pInstance = LPDMA1;
  pStartFull_LPDMA.WakeupIT = LPBAM_DMA_IT_NONE;
  pStartFull_LPDMA.HeadQAddress = (uint32_t)Threshold1_Conversion_Q.Head;
  if (ADV_LPBAM_DMA_Start_SetFullQ(LPDMA1_Channel0, &pDMAListInfo_LPDMA, &pStartFull_LPDMA, &Thresholdx_Config_Q_Threshold1_Q_Start_Desc, &Thresholdx_Config_Q) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Thresholdx_Config queue Period_10ms build
    */
  pDMAListInfo_LPTIM.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_LPTIM.pInstance = LPDMA1;
  pPWMFull_LPTIM.UpdatePeriod = ENABLE;
  pPWMFull_LPTIM.PeriodValue = 319;
  pPWMFull_LPTIM.UpdatePulse = ENABLE;
  pPWMFull_LPTIM.PulseValue = 159;
  pPWMFull_LPTIM.UpdateRepetition = DISABLE;
  pPWMFull_LPTIM.RepetitionValue = 0;
  if (ADV_LPBAM_LPTIM_PWM_SetFullQ(LPTIM1, LPBAM_LPTIM_CHANNEL_1, &pDMAListInfo_LPTIM, &pPWMFull_LPTIM, &Thresholdx_Config_Q_Period_10ms_Desc, &Thresholdx_Config_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_LPTIM.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_BLOCK_TRANSFER;
  pTrigConfig_LPTIM.TriggerConfig.TriggerPolarity = LPBAM_DMA_TRIG_POLARITY_RISING;
  pTrigConfig_LPTIM.TriggerConfig.TriggerSelection = LPBAM_LPDMA1_TRIGGER_ADC4_AWD1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_LPTIM, LPBAM_LPTIM_PWM_FULLQ_CONFIG_NODE, &Thresholdx_Config_Q_Period_10ms_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Thresholdx_Config queue Conversion_Config build
    */
  pDMAListInfo_ADC.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_ADC.pInstance = LPDMA1;
  pConvConfig_ADC.ScanConvMode = LPBAM_ADC_SCAN_DIRECTION_FORWARD;
  pConvConfig_ADC.ContinuousConvMode = DISABLE;
  pConvConfig_ADC.DiscontinuousConvMode = DISABLE;
  pConvConfig_ADC.ExternalTrigConv = LPBAM_ADC_EXTERNALTRIG_LPTIM1_CH1;
  pConvConfig_ADC.ExternalTrigConvEdge = LPBAM_ADC_EXTERNALTRIGCONVEDGE_RISING;
  pConvConfig_ADC.TriggerFrequencyMode = LPBAM_ADC_TRIGGER_FREQ_LOW;
  pConvConfig_ADC.LowPowerAutoWait = ENABLE;
  pConvConfig_ADC.DMAContinuousRequests = ENABLE;
  pConvConfig_ADC.AnalogWDGConfig[0U].WatchdogNumber = LPBAM_ADC_ANALOGWATCHDOG_1;
  pConvConfig_ADC.AnalogWDGConfig[0U].WatchdogMode = LPBAM_ADC_ANALOGWATCHDOG_SINGLE_REG;
  pConvConfig_ADC.AnalogWDGConfig[0U].Channel = LPBAM_ADC_CHANNEL_TEMPSENSOR;
  pConvConfig_ADC.AnalogWDGConfig[0U].ITMode = ENABLE;
  pConvConfig_ADC.AnalogWDGConfig[0U].LowThreshold = 0;
  pConvConfig_ADC.AnalogWDGConfig[0U].HighThreshold = 1100;
  if (ADV_LPBAM_ADC_Conversion_SetConfigQ(ADC4, &pDMAListInfo_ADC, &pConvConfig_ADC, &Thresholdx_Config_Q_Conversion_Config_Desc, &Thresholdx_Config_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pChannel_ADC.ChannelConfig.Channel = LPBAM_ADC_CHANNEL_TEMPSENSOR;
  pChannel_ADC.ChannelConfig.Rank = LPBAM_ADC_REGULAR_RANK_1;
  if (ADV_LPBAM_ADC_ChannelConfig(&pChannel_ADC, &Thresholdx_Config_Q_Conversion_Config_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }
  pChannel_ADC.ChannelConfig.Channel = LPBAM_ADC_CHANNEL_1;
  if (ADV_LPBAM_ADC_ChannelConfig(&pChannel_ADC, &Thresholdx_Config_Q_Conversion_Config_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Thresholdx_Config queue Threshold2_Q_Start build
    */
  pStartFull_LPDMA.HeadQAddress = (uint32_t)Threshold2_Conversion_Q.Head;
  if (ADV_LPBAM_DMA_Start_SetFullQ(LPDMA1_Channel0, &pDMAListInfo_LPDMA, &pStartFull_LPDMA, &Thresholdx_Config_Q_Threshold2_Q_Start_Desc, &Thresholdx_Config_Q) != LPBAM_OK)
  {
    Error_Handler();
  }

}

/* USER CODE BEGIN TempSens_MultiThres_Build */

/* USER CODE END TempSens_MultiThres_Build */
