/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file   lpbam_incomp_inswitch_build.c
  * @author MCD Application Team
  * @brief  Provides LPBAM InComp application InSwitch build services
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
#include "lpbam_incomp.h"

/* Private variables -------------------------------------------------------------------------------------------------*/
/* LPBAM variables declaration */
static LPBAM_COMP_StartFullDesc_t Start_1_Desc;
static LPBAM_COMP_OutLevelFullDesc_t OutputLevel_1_Desc;
static LPBAM_COMP_StartFullDesc_t Start_2_Desc;
static LPBAM_COMP_OutLevelFullDesc_t OutputLevel_2_Desc;
static LPBAM_COMP_StartFullDesc_t Start_3_Desc;
static LPBAM_COMP_OutLevelFullDesc_t OutputLevel_3_Desc;
static LPBAM_COMP_StartFullDesc_t Start_4_Desc;
static LPBAM_COMP_OutLevelFullDesc_t OutputLevel_4_Desc;

/* Exported variables ------------------------------------------------------------------------------------------------*/
/* LPBAM queues declaration */
DMA_QListTypeDef InputSwitch_Q;

/* External variables ------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN EV */
/* LPBAM user buffers declaration */
extern uint32_t CompOutput_Buffer[OUTPUT_BUFFER_SIZE];
/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static void MX_InputSwitch_Q_Build(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
  * @brief InComp application InSwitch scenario build
  * @param None
  * @retval None
  */
void MX_InComp_InSwitch_Build(void)
{
  /* USER CODE BEGIN InComp_InSwitch_Build 0 */

  /* USER CODE END InComp_InSwitch_Build 0 */

  /* LPBAM build InputSwitch queue */
  MX_InputSwitch_Q_Build();

  /* USER CODE BEGIN InComp_InSwitch_Build 1 */

  /* USER CODE END InComp_InSwitch_Build 1 */
}

/* Private functions -------------------------------------------------------------------------------------------------*/

/**
  * @brief  InComp application InSwitch scenario InputSwitch queue build
  * @param  None
  * @retval None
  */
static void MX_InputSwitch_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_COMP = {0};
  LPBAM_COMP_StartFullAdvConf_t pStartFull_COMP = {0};
  LPBAM_COMMON_TrigAdvConf_t pTrigConfig_COMP = {0};
  LPBAM_COMP_OutLevelFullAdvConf_t pOutLevelFull_COMP = {0};

  /**
    * InputSwitch queue Start_1 build
    */
  pDMAListInfo_COMP.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_COMP.pInstance = LPDMA1;
  pStartFull_COMP.OutputPol = LPBAM_COMP_OUTPUTPOL_NONINVERTED;
  pStartFull_COMP.InputPlus = LPBAM_COMP_INPUT_PLUS_IO2;
  pStartFull_COMP.InputMinus = LPBAM_COMP_INPUT_MINUS_VREFINT;
  if (ADV_LPBAM_COMP_Start_SetFullQ(COMP1, &pDMAListInfo_COMP, &pStartFull_COMP, &Start_1_Desc, &InputSwitch_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_COMP.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_BLOCK_TRANSFER;
  pTrigConfig_COMP.TriggerConfig.TriggerPolarity = LPBAM_DMA_TRIG_POLARITY_RISING;
  pTrigConfig_COMP.TriggerConfig.TriggerSelection = LPBAM_LPDMA1_TRIGGER_LPTIM1_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_COMP, LPBAM_COMP_START_FULLQ_CONFIG_NODE, &Start_1_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * InputSwitch queue OutputLevel_1 build
    */
  pOutLevelFull_COMP.Size = 1;
  pOutLevelFull_COMP.pData = (uint32_t*)&CompOutput_Buffer[0];
  if (ADV_LPBAM_COMP_OutputLevel_SetFullQ(COMP1, &pDMAListInfo_COMP, &pOutLevelFull_COMP, &OutputLevel_1_Desc, &InputSwitch_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_COMP.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_SINGLE_BURST_TRANSFER;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_COMP, LPBAM_COMP_OUTPUTLEVEL_FULLQ_DATA_NODE, &OutputLevel_1_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * InputSwitch queue Start_2 build
    */
  pStartFull_COMP.InputPlus = LPBAM_COMP_INPUT_PLUS_IO3;
  if (ADV_LPBAM_COMP_Start_SetFullQ(COMP1, &pDMAListInfo_COMP, &pStartFull_COMP, &Start_2_Desc, &InputSwitch_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_COMP.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_BLOCK_TRANSFER;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_COMP, LPBAM_COMP_START_FULLQ_CONFIG_NODE, &Start_2_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * InputSwitch queue OutputLevel_2 build
    */
  pOutLevelFull_COMP.pData = (uint32_t*)&CompOutput_Buffer[1];
  if (ADV_LPBAM_COMP_OutputLevel_SetFullQ(COMP1, &pDMAListInfo_COMP, &pOutLevelFull_COMP, &OutputLevel_2_Desc, &InputSwitch_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_COMP.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_SINGLE_BURST_TRANSFER;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_COMP, LPBAM_COMP_OUTPUTLEVEL_FULLQ_DATA_NODE, &OutputLevel_2_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * InputSwitch queue Start_3 build
    */
  pStartFull_COMP.InputPlus = LPBAM_COMP_INPUT_PLUS_IO2;
  if (ADV_LPBAM_COMP_Start_SetFullQ(COMP1, &pDMAListInfo_COMP, &pStartFull_COMP, &Start_3_Desc, &InputSwitch_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_COMP.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_BLOCK_TRANSFER;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_COMP, LPBAM_COMP_START_FULLQ_CONFIG_NODE, &Start_3_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * InputSwitch queue OutputLevel_3 build
    */
  pOutLevelFull_COMP.pData = (uint32_t*)&CompOutput_Buffer[2];
  if (ADV_LPBAM_COMP_OutputLevel_SetFullQ(COMP1, &pDMAListInfo_COMP, &pOutLevelFull_COMP, &OutputLevel_3_Desc, &InputSwitch_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_COMP.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_SINGLE_BURST_TRANSFER;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_COMP, LPBAM_COMP_OUTPUTLEVEL_FULLQ_DATA_NODE, &OutputLevel_3_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * InputSwitch queue Start_4 build
    */
  pStartFull_COMP.InputPlus = LPBAM_COMP_INPUT_PLUS_IO3;
  if (ADV_LPBAM_COMP_Start_SetFullQ(COMP1, &pDMAListInfo_COMP, &pStartFull_COMP, &Start_4_Desc, &InputSwitch_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_COMP.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_BLOCK_TRANSFER;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_COMP, LPBAM_COMP_START_FULLQ_CONFIG_NODE, &Start_4_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * InputSwitch queue OutputLevel_4 build
    */
  pOutLevelFull_COMP.pData = (uint32_t*)&CompOutput_Buffer[3];
  if (ADV_LPBAM_COMP_OutputLevel_SetFullQ(COMP1, &pDMAListInfo_COMP, &pOutLevelFull_COMP, &OutputLevel_4_Desc, &InputSwitch_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_COMP.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_SINGLE_BURST_TRANSFER;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_COMP, LPBAM_COMP_OUTPUTLEVEL_FULLQ_DATA_NODE, &OutputLevel_4_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

}

/* USER CODE BEGIN InComp_InSwitch_Build */

/* USER CODE END InComp_InSwitch_Build */
