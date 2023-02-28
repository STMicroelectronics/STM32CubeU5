/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file   lpbam_txrx_sendbypack_build.c
  * @author MCD Application Team
  * @brief  Provides LPBAM TxRx application SendByPack build services
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
#include "lpbam_txrx.h"

/* Private variables -------------------------------------------------------------------------------------------------*/
/* LPBAM variables declaration */
/* USER CODE BEGIN TxRx_SendByPack_Descs 0 */

/* USER CODE END TxRx_SendByPack_Descs 0 */

/* USER CODE BEGIN Receive_Q_Receive_Data_1_Desc */

/* USER CODE END Receive_Q_Receive_Data_1_Desc */
static LPBAM_UART_RxDataDesc_t Receive_Q_Receive_Data_1_Desc;

/* USER CODE BEGIN Transmit_Q_Transmit_1_Desc */

/* USER CODE END Transmit_Q_Transmit_1_Desc */
static LPBAM_UART_TxFullDesc_t Transmit_Q_Transmit_1_Desc;

/* USER CODE BEGIN Transmit_Q_Transmit_2_Desc */

/* USER CODE END Transmit_Q_Transmit_2_Desc */
static LPBAM_UART_TxFullDesc_t Transmit_Q_Transmit_2_Desc;

/* USER CODE BEGIN TxRx_SendByPack_Descs 1 */

/* USER CODE END TxRx_SendByPack_Descs 1 */

/* Exported variables ------------------------------------------------------------------------------------------------*/
/* LPBAM queues declaration */
DMA_QListTypeDef Receive_Q;
DMA_QListTypeDef Transmit_Q;

/* External variables ------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN EV */
/* LPBAM user buffers declaration */
extern uint8_t Transmit_Buffer_1[65];
extern uint8_t Transmit_Buffer_2[65];
extern uint8_t Receive_Buffer[130];
/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static void MX_Receive_Q_Build(void);
static void MX_Transmit_Q_Build(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
  * @brief TxRx application SendByPack scenario build
  * @param None
  * @retval None
  */
void MX_TxRx_SendByPack_Build(void)
{
  /* USER CODE BEGIN TxRx_SendByPack_Build 0 */

  /* USER CODE END TxRx_SendByPack_Build 0 */

  /* LPBAM build Receive queue */
  MX_Receive_Q_Build();
  /* LPBAM build Transmit queue */
  MX_Transmit_Q_Build();

  /* USER CODE BEGIN TxRx_SendByPack_Build 1 */

  /* USER CODE END TxRx_SendByPack_Build 1 */
}

/* Private functions -------------------------------------------------------------------------------------------------*/

/**
  * @brief  TxRx application SendByPack scenario Receive queue build
  * @param  None
  * @retval None
  */
static void MX_Receive_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_LPUART = {0};
  LPBAM_UART_DataAdvConf_t pRxData_LPUART = {0};

  /**
    * Receive queue Receive_Data_1 build
    */
  pDMAListInfo_LPUART.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_LPUART.pInstance = LPDMA1;
  pRxData_LPUART.pData = (uint8_t*)&Receive_Buffer[0];
  pRxData_LPUART.Size = 130;
  if (ADV_LPBAM_UART_Receive_SetDataQ(LPUART1, &pDMAListInfo_LPUART, &pRxData_LPUART, &Receive_Q_Receive_Data_1_Desc, &Receive_Q) != LPBAM_OK)
  {
    Error_Handler();
  }

}

/**
  * @brief  TxRx application SendByPack scenario Transmit queue build
  * @param  None
  * @retval None
  */
static void MX_Transmit_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_LPUART = {0};
  LPBAM_UART_FullAdvConf_t pTxFull_LPUART = {0};
  LPBAM_COMMON_TrigAdvConf_t pTrigConfig_LPUART = {0};

  /**
    * Transmit queue Transmit_1 build
    */
  pDMAListInfo_LPUART.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_LPUART.pInstance = LPDMA1;
  pTxFull_LPUART.Mode = LPBAM_UART_MODE_TX_RX;
  pTxFull_LPUART.AutoModeConf.AutonomousModeState = LPBAM_UART_AUTONOMOUS_MODE_ENABLE;
  pTxFull_LPUART.AutoModeConf.TriggerSelection = LPBAM_LPUART_LPTIM1_OUT_TRG;
  pTxFull_LPUART.AutoModeConf.TriggerPolarity = LPBAM_UART_TRIG_POLARITY_RISING;
  pTxFull_LPUART.AutoModeConf.DataSize = 8;
  pTxFull_LPUART.WakeupIT = LPBAM_UART_IT_NONE;
  pTxFull_LPUART.pData = (uint8_t*)&Transmit_Buffer_1[0];
  pTxFull_LPUART.Size = 65;
  if (ADV_LPBAM_UART_Transmit_SetFullQ(LPUART1, &pDMAListInfo_LPUART, &pTxFull_LPUART, &Transmit_Q_Transmit_1_Desc, &Transmit_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_LPUART.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_BLOCK_TRANSFER;
  pTrigConfig_LPUART.TriggerConfig.TriggerPolarity = LPBAM_DMA_TRIG_POLARITY_RISING;
  pTrigConfig_LPUART.TriggerConfig.TriggerSelection = LPBAM_LPDMA1_TRIGGER_LPTIM1_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_LPUART, LPBAM_UART_TX_FULLQ_CONFIG_NODE, &Transmit_Q_Transmit_1_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Transmit queue Transmit_2 build
    */
  pTxFull_LPUART.AutoModeConf.DataSize = 4;
  pTxFull_LPUART.pData = (uint8_t*)&Transmit_Buffer_2[0];
  if (ADV_LPBAM_UART_Transmit_SetFullQ(LPUART1, &pDMAListInfo_LPUART, &pTxFull_LPUART, &Transmit_Q_Transmit_2_Desc, &Transmit_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_LPUART, LPBAM_UART_TX_FULLQ_CONFIG_NODE, &Transmit_Q_Transmit_2_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

}

/* USER CODE BEGIN TxRx_SendByPack_Build */

/* USER CODE END TxRx_SendByPack_Build */
