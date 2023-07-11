/* USER CODE BEGIN Header */
/**
  **********************************************************************************************************************
  * @file   lpbam_slave_reloadseq_build.c
  * @author MCD Application Team
  * @brief  Provides LPBAM Slave application ReloadSeq build services
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
#include "lpbam_slave.h"

/* Private variables -------------------------------------------------------------------------------------------------*/
/* LPBAM variables declaration */
/* USER CODE BEGIN Slave_ReloadSeq_Descs 0 */

/* USER CODE END Slave_ReloadSeq_Descs 0 */

/* USER CODE BEGIN Slave_Q_Slave_Receive_Data_1_Desc[1] */

/* USER CODE END Slave_Q_Slave_Receive_Data_1_Desc[1] */
static LPBAM_I2C_SlaveRxDataDesc_t Slave_Q_Slave_Receive_Data_1_Desc[1];

/* USER CODE BEGIN Slave_Q_Slave_Transmit_Data_1_Desc[2] */

/* USER CODE END Slave_Q_Slave_Transmit_Data_1_Desc[2] */
static LPBAM_I2C_SlaveTxDataDesc_t Slave_Q_Slave_Transmit_Data_1_Desc[2];

/* USER CODE BEGIN Slave_ReloadSeq_Descs 1 */

/* USER CODE END Slave_ReloadSeq_Descs 1 */

/* Exported variables ------------------------------------------------------------------------------------------------*/
/* LPBAM queues declaration */
DMA_QListTypeDef Slave_Q;

/* External variables ------------------------------------------------------------------------------------------------*/
/* USER CODE BEGIN EV */
/* LPBAM user buffers declaration */
extern uint8_t aTxBuffer2[TX_BUFFER2_SIZE];
extern uint8_t aRxBuffer1[RX_BUFFER1_SIZE];
/* USER CODE END EV */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static void MX_Slave_Q_Build(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
  * @brief Slave application ReloadSeq scenario build
  * @param None
  * @retval None
  */
void MX_Slave_ReloadSeq_Build(void)
{
  /* USER CODE BEGIN Slave_ReloadSeq_Build 0 */

  /* USER CODE END Slave_ReloadSeq_Build 0 */

  /* LPBAM build Slave queue */
  MX_Slave_Q_Build();

  /* USER CODE BEGIN Slave_ReloadSeq_Build 1 */

  /* USER CODE END Slave_ReloadSeq_Build 1 */
}

/* Private functions -------------------------------------------------------------------------------------------------*/

/**
  * @brief  Slave application ReloadSeq scenario Slave queue build
  * @param  None
  * @retval None
  */
static void MX_Slave_Q_Build(void)
{
  /* LPBAM build variable */
  LPBAM_DMAListInfo_t pDMAListInfo_I2C = {0};
  LPBAM_I2C_DataAdvConf_t pRxData_I2C = {0};
  LPBAM_I2C_DataAdvConf_t pTxData_I2C = {0};
  uint32_t data_size = 0;
  uint32_t tmp_data_size = 0;
  uint32_t transfer_idx = 0;

  /**
    * Slave queue Slave_Receive_Data_1 build
    */
  pDMAListInfo_I2C.QueueType = LPBAM_LINEAR_ADDRESSING_Q;
  pDMAListInfo_I2C.pInstance = LPDMA1;
  pRxData_I2C.AutoModeConf.TriggerState = LPBAM_I2C_AUTO_MODE_DISABLE;
  pRxData_I2C.AddressingMode = LPBAM_I2C_ADDRESSINGMODE_7BIT;
  pRxData_I2C.SequenceNumber = 2;
  pRxData_I2C.pData = (uint8_t*)&aRxBuffer1[0];
  pRxData_I2C.Size = 55;
  /* Set transfer parameters */
  data_size = pRxData_I2C.Size;
  tmp_data_size = pRxData_I2C.Size;
  transfer_idx = 0;

  /* Repeat inserting I2C master Tx data queue until completing all data */
  while (data_size != 0U)
  {
    if (ADV_LPBAM_I2C_SlaveRx_SetDataQ(I2C3, &pDMAListInfo_I2C, &pRxData_I2C, &Slave_Q_Slave_Receive_Data_1_Desc[transfer_idx], &Slave_Q) != LPBAM_OK)
    {
      Error_Handler();
    }

    transfer_idx++;

    if (data_size > LPBAM_I2C_MAX_DATA_SIZE)
    {
      data_size -= LPBAM_I2C_MAX_DATA_SIZE;
    }
    else
    {
      data_size = 0U;
    }

    pRxData_I2C.Size = data_size;
  }
  pRxData_I2C.Size = tmp_data_size;

  /**
    * Slave queue Slave_Transmit_Data_1 build
    */
  pTxData_I2C.AutoModeConf.TriggerState = LPBAM_I2C_AUTO_MODE_DISABLE;
  pTxData_I2C.AddressingMode = LPBAM_I2C_ADDRESSINGMODE_7BIT;
  pTxData_I2C.SequenceNumber = 2;
  pTxData_I2C.pData = (uint8_t*)&aTxBuffer2[0];
  pTxData_I2C.Size = 310;
  /* Set transfer parameters */
  data_size = pTxData_I2C.Size;
  tmp_data_size = pTxData_I2C.Size;
  transfer_idx = 0;

  /* Repeat inserting I2C master Tx data queue until completing all data */
  while (data_size != 0U)
  {
    if (ADV_LPBAM_I2C_SlaveTx_SetDataQ(I2C3, &pDMAListInfo_I2C, &pTxData_I2C, &Slave_Q_Slave_Transmit_Data_1_Desc[transfer_idx], &Slave_Q) != LPBAM_OK)
    {
      Error_Handler();
    }

    transfer_idx++;

    if (data_size > LPBAM_I2C_MAX_DATA_SIZE)
    {
      data_size -= LPBAM_I2C_MAX_DATA_SIZE;
    }
    else
    {
      data_size = 0U;
    }

    pTxData_I2C.Size = data_size;
  }
  pTxData_I2C.Size = tmp_data_size;

}

/* USER CODE BEGIN Slave_ReloadSeq_Build */

/* USER CODE END Slave_ReloadSeq_Build */
