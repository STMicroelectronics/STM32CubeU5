/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file   lpbam_pwmgen_multivalue_build.c
  * @author MCD Application Team
  * @brief  Provides LPBAM PWMGen application MultiValue build services
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
#include "lpbam_pwmgen.h"

/* Private variables -------------------------------------------------------------------------------------------------*/
/* LPBAM variables declaration */
/* USER CODE BEGIN PWMGen_MultiValue_Descs 0 */

/* USER CODE END PWMGen_MultiValue_Descs 0 */

/* USER CODE BEGIN PWMUpdate_Q_Start_1_Desc */

/* USER CODE END PWMUpdate_Q_Start_1_Desc */
static LPBAM_LPTIM_StartFullDesc_t PWMUpdate_Q_Start_1_Desc;

/* USER CODE BEGIN PWMUpdate_Q_PWM_1_Desc */

/* USER CODE END PWMUpdate_Q_PWM_1_Desc */
static LPBAM_LPTIM_PWMFullDesc_t PWMUpdate_Q_PWM_1_Desc;

/* USER CODE BEGIN PWMUpdate_Q_PWM_2_Desc */

/* USER CODE END PWMUpdate_Q_PWM_2_Desc */
static LPBAM_LPTIM_PWMFullDesc_t PWMUpdate_Q_PWM_2_Desc;

/* USER CODE BEGIN PWMUpdate_Q_PWM_3_Desc */

/* USER CODE END PWMUpdate_Q_PWM_3_Desc */
static LPBAM_LPTIM_PWMFullDesc_t PWMUpdate_Q_PWM_3_Desc;

/* USER CODE BEGIN PWMUpdate_Q_PWM_4_Desc */

/* USER CODE END PWMUpdate_Q_PWM_4_Desc */
static LPBAM_LPTIM_PWMFullDesc_t PWMUpdate_Q_PWM_4_Desc;

/* USER CODE BEGIN PWMUpdate_Q_PWM_5_Desc */

/* USER CODE END PWMUpdate_Q_PWM_5_Desc */
static LPBAM_LPTIM_PWMFullDesc_t PWMUpdate_Q_PWM_5_Desc;

/* USER CODE BEGIN PWMGen_MultiValue_Descs 1 */

/* USER CODE END PWMGen_MultiValue_Descs 1 */

/* Exported variables ------------------------------------------------------------------------------------------------*/
/* LPBAM queues declaration */
DMA_QListTypeDef PWMUpdate_Q;

/* External variables ------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN EV */
/* LPBAM user buffers declaration */

/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static void MX_PWMUpdate_Q_Build(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
  * @brief PWMGen application MultiValue scenario build
  * @param None
  * @retval None
  */
void MX_PWMGen_MultiValue_Build(void)
{
  /* USER CODE BEGIN PWMGen_MultiValue_Build 0 */

  /* USER CODE END PWMGen_MultiValue_Build 0 */

  /* LPBAM build PWMUpdate queue */
  MX_PWMUpdate_Q_Build();

  /* USER CODE BEGIN PWMGen_MultiValue_Build 1 */

  /* USER CODE END PWMGen_MultiValue_Build 1 */
}

/* Private functions -------------------------------------------------------------------------------------------------*/

/**
  * @brief  PWMGen application MultiValue scenario PWMUpdate queue build
  * @param  None
  * @retval None
  */
static void MX_PWMUpdate_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_LPTIM = {0};
  LPBAM_LPTIM_StartFullAdvConf_t pStartFull_LPTIM = {0};
  LPBAM_LPTIM_PWMFullAdvConf_t pPWMFull_LPTIM = {0};

  /**
    * PWMUpdate queue Start_1 build
    */
  pDMAListInfo_LPTIM.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_LPTIM.pInstance = LPDMA1;
  pStartFull_LPTIM.StartMode = LPBAM_LPTIM_START_CONTINUOUS;
  pStartFull_LPTIM.WakeupIT = LPBAM_LPTIM_IT_NONE;
  if (ADV_LPBAM_LPTIM_Start_SetFullQ(LPTIM1, &pDMAListInfo_LPTIM, &pStartFull_LPTIM, &PWMUpdate_Q_Start_1_Desc, &PWMUpdate_Q) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * PWMUpdate queue PWM_1 build
    */
  pPWMFull_LPTIM.UpdatePeriod = ENABLE;
  pPWMFull_LPTIM.PeriodValue = 327;
  pPWMFull_LPTIM.UpdatePulse = ENABLE;
  pPWMFull_LPTIM.PulseValue = 163;
  pPWMFull_LPTIM.UpdateRepetition = ENABLE;
  pPWMFull_LPTIM.RepetitionValue = 0;
  if (ADV_LPBAM_LPTIM_PWM_SetFullQ(LPTIM1, LPBAM_LPTIM_CHANNEL_1, &pDMAListInfo_LPTIM, &pPWMFull_LPTIM, &PWMUpdate_Q_PWM_1_Desc, &PWMUpdate_Q) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * PWMUpdate queue PWM_2 build
    */
  pPWMFull_LPTIM.PeriodValue = 654;
  pPWMFull_LPTIM.PulseValue = 327;
  pPWMFull_LPTIM.RepetitionValue = 1;
  if (ADV_LPBAM_LPTIM_PWM_SetFullQ(LPTIM1, LPBAM_LPTIM_CHANNEL_1, &pDMAListInfo_LPTIM, &pPWMFull_LPTIM, &PWMUpdate_Q_PWM_2_Desc, &PWMUpdate_Q) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * PWMUpdate queue PWM_3 build
    */
  pPWMFull_LPTIM.PeriodValue = 982;
  pPWMFull_LPTIM.PulseValue = 491;
  pPWMFull_LPTIM.RepetitionValue = 2;
  if (ADV_LPBAM_LPTIM_PWM_SetFullQ(LPTIM1, LPBAM_LPTIM_CHANNEL_1, &pDMAListInfo_LPTIM, &pPWMFull_LPTIM, &PWMUpdate_Q_PWM_3_Desc, &PWMUpdate_Q) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * PWMUpdate queue PWM_4 build
    */
  pPWMFull_LPTIM.PeriodValue = 1310;
  pPWMFull_LPTIM.PulseValue = 654;
  pPWMFull_LPTIM.RepetitionValue = 3;
  if (ADV_LPBAM_LPTIM_PWM_SetFullQ(LPTIM1, LPBAM_LPTIM_CHANNEL_1, &pDMAListInfo_LPTIM, &pPWMFull_LPTIM, &PWMUpdate_Q_PWM_4_Desc, &PWMUpdate_Q) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * PWMUpdate queue PWM_5 build
    */
  pPWMFull_LPTIM.PeriodValue = 1637;
  pPWMFull_LPTIM.PulseValue = 818;
  pPWMFull_LPTIM.RepetitionValue = 4;
  if (ADV_LPBAM_LPTIM_PWM_SetFullQ(LPTIM1, LPBAM_LPTIM_CHANNEL_1, &pDMAListInfo_LPTIM, &pPWMFull_LPTIM, &PWMUpdate_Q_PWM_5_Desc, &PWMUpdate_Q) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Set circular mode
    */
  if (ADV_LPBAM_Q_SetCircularMode(&PWMUpdate_Q_PWM_1_Desc, LPBAM_LPTIM_PWM_FULLQ_CONFIG_NODE, &PWMUpdate_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN PWMGen_MultiValue_Build */

/* USER CODE END PWMGen_MultiValue_Build */
