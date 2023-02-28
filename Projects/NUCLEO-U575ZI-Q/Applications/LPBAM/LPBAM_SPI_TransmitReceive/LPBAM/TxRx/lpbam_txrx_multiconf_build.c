/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file   lpbam_txrx_multiconf_build.c
  * @author MCD Application Team
  * @brief  Provides LPBAM TxRx application MultiConf build services
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
/* USER CODE BEGIN TxRx_MultiConf_Descs 0 */

/* USER CODE END TxRx_MultiConf_Descs 0 */

/* USER CODE BEGIN Transmit_Q_TransmitReceive_Data_1_Tx_Desc */

/* USER CODE END Transmit_Q_TransmitReceive_Data_1_Tx_Desc */
static LPBAM_SPI_TxRx_TxDataDesc_t Transmit_Q_TransmitReceive_Data_1_Tx_Desc;

/* USER CODE BEGIN Transmit_Q_Transmit_1_Tx_Desc */

/* USER CODE END Transmit_Q_Transmit_1_Tx_Desc */
static LPBAM_SPI_TxFullDesc_t Transmit_Q_Transmit_1_Tx_Desc;

/* USER CODE BEGIN Coupled_Transmit_Q_TransmitReceive_Data_1_Rx_Desc */

/* USER CODE END Coupled_Transmit_Q_TransmitReceive_Data_1_Rx_Desc */
static LPBAM_SPI_TxRx_RxDataDesc_t Coupled_Transmit_Q_TransmitReceive_Data_1_Rx_Desc;

/* USER CODE BEGIN TxRx_MultiConf_Descs 1 */

/* USER CODE END TxRx_MultiConf_Descs 1 */

/* Exported variables ------------------------------------------------------------------------------------------------*/
/* LPBAM queues declaration */
DMA_QListTypeDef Transmit_Q;
DMA_QListTypeDef Coupled_Transmit_Q;

/* External variables ------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN EV */
/* LPBAM user buffers declaration */
extern uint8_t aTxBuffer1[TX_BUFFER1_SIZE];
extern uint8_t aRxBuffer1[RX_BUFFER1_SIZE];
extern uint8_t aTxBuffer2[TX_BUFFER2_SIZE];
/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static void MX_Transmit_Q_Build(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
  * @brief TxRx application MultiConf scenario build
  * @param None
  * @retval None
  */
void MX_TxRx_MultiConf_Build(void)
{
  /* USER CODE BEGIN TxRx_MultiConf_Build 0 */

  /* USER CODE END TxRx_MultiConf_Build 0 */

  /* LPBAM build Transmit queue */
  MX_Transmit_Q_Build();

  /* USER CODE BEGIN TxRx_MultiConf_Build 1 */

  /* USER CODE END TxRx_MultiConf_Build 1 */
}

/* Private functions -------------------------------------------------------------------------------------------------*/

/**
  * @brief  TxRx application MultiConf scenario Transmit queue build
  * @param  None
  * @retval None
  */
static void MX_Transmit_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_SPI = {0};
  LPBAM_SPI_DataAdvConf_t pTxRxData_SPI = {0};
  LPBAM_SPI_FullAdvConf_t pTxFull_SPI = {0};
  LPBAM_COMMON_TrigAdvConf_t pTrigConfig_SPI = {0};

  /**
    * Transmit queue TransmitReceive_Data_1 build
    */
  pDMAListInfo_SPI.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_SPI.pInstance = LPDMA1;
  pTxRxData_SPI.AutoModeConf.TriggerState = LPBAM_SPI_AUTO_MODE_ENABLE;
  pTxRxData_SPI.DataSize = LPBAM_SPI_DATASIZE_8BIT;
  pTxRxData_SPI.Size = 56;
  pTxRxData_SPI.pTxData = (uint8_t*)&aTxBuffer1[0];
  pTxRxData_SPI.pRxData = (uint8_t*)&aRxBuffer1[0];
  if (ADV_LPBAM_SPI_TxRx_SetDataQ(SPI3, &pDMAListInfo_SPI, &pTxRxData_SPI, &Transmit_Q_TransmitReceive_Data_1_Tx_Desc, &Coupled_Transmit_Q_TransmitReceive_Data_1_Rx_Desc, &Transmit_Q, &Coupled_Transmit_Q) != LPBAM_OK)
  {
    Error_Handler();
  }

  /**
    * Transmit queue Transmit_1 build
    */
  pTxFull_SPI.CLKPolarity = LPBAM_SPI_POLARITY_LOW;
  pTxFull_SPI.CLKPhase = LPBAM_SPI_PHASE_1EDGE;
  pTxFull_SPI.FirstBit = LPBAM_SPI_FIRSTBIT_MSB;
  pTxFull_SPI.BaudRatePrescaler = LPBAM_SPI_BAUDRATEPRESCALER_64;
  pTxFull_SPI.DataSize = LPBAM_SPI_DATASIZE_16BIT;
  pTxFull_SPI.AutoModeConf.TriggerState = LPBAM_SPI_AUTO_MODE_ENABLE;
  pTxFull_SPI.AutoModeConf.TriggerSelection = LPBAM_SPI_GRP2_LPTIM1_CH1_TRG;
  pTxFull_SPI.AutoModeConf.TriggerPolarity = LPBAM_SPI_TRIG_POLARITY_RISING;
  pTxFull_SPI.WakeupIT = LPBAM_SPI_IT_EOT;
  pTxFull_SPI.Size = 36;
  pTxFull_SPI.pTxData = (uint8_t*)&aTxBuffer2[0];
  if (ADV_LPBAM_SPI_Tx_SetFullQ(SPI3, &pDMAListInfo_SPI, &pTxFull_SPI, &Transmit_Q_Transmit_1_Tx_Desc, &Transmit_Q) != LPBAM_OK)
  {
    Error_Handler();
  }
  pTrigConfig_SPI.TriggerConfig.TriggerMode = LPBAM_DMA_TRIGM_BLOCK_TRANSFER;
  pTrigConfig_SPI.TriggerConfig.TriggerPolarity = LPBAM_DMA_TRIG_POLARITY_RISING;
  pTrigConfig_SPI.TriggerConfig.TriggerSelection = LPBAM_LPDMA1_TRIGGER_LPTIM1_CH1;
  if (ADV_LPBAM_Q_SetTriggerConfig(&pTrigConfig_SPI, LPBAM_SPI_TX_FULLQ_CONFIG_NODE, &Transmit_Q_Transmit_1_Tx_Desc) != LPBAM_OK)
  {
    Error_Handler();
  }

}

/* USER CODE BEGIN TxRx_MultiConf_Build */

/* USER CODE END TxRx_MultiConf_Build */
