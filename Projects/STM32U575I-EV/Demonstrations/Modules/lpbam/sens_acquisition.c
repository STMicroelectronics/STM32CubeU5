/**
  ******************************************************************************
  * @file    sens_acquisition.c
  * @author  MCD Application Team
  * @brief   Sensor acquisition implementation
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "stm32u5xx_hal.h"
#include "main.h"
#include "app_lpbam.h"
#include "sens_acquisition.h"
#include "stm32_lpbam.h"

/* Private typedef ----------------------------------------------------------*/

/* Private constants ----------------------------------------------------------*/
/* Private Variables ---------------------------------------------------------*/
/* Used to exit application */
static __IO uint8_t UserEvent=0;
static __IO uint8_t UserEntry=0;
static TS_State_t TS_State;
static uint32_t count=0;;

DMA_HandleTypeDef        DMAHandle1;
DMA_HandleTypeDef        DMAHandle2;

extern LPTIM_HandleTypeDef LPTIMHandle;
extern I2C_HandleTypeDef hbus_i2c3;

I2C_AutonomousModeConfTypeDef AutonomousModeConf;

/* LPBAM variables declaration */
static LPBAM_DMAListInfo_t     DMAListInfo;
LPBAM_I2C_DataAdvConf_t        I2C_DataAdvConf;

/* Place following data in section  SDRAM4 LPBAM_Section */
#if defined ( __ICCARM__ )
#pragma default_variable_attributes =@".LPBAM_Section"

DMA_QListTypeDef               I2C_TransmitQueue[4];
LPBAM_I2C_MasterTxDataDesc_t   MasterTxDataDesc;
LPBAM_I2C_MasterRxDataDesc_t   MasterRxDataDesc;
uint8_t                        tmp_Reg [4];
uint8_t                        tmp_pData [6];
int16_t                        x [140];
int16_t                        y [140];
int16_t                        z [140];
DMA_NodeConfTypeDef            dmaNodeConf;
DMA_NodeTypeDef                pNode0;
DMA_NodeTypeDef                pNode1;
DMA_NodeTypeDef                pNode2;

/* Stop placing functions in section SDRAM4 LPBAM_Sectionn */
#pragma default_variable_attributes =
#endif

#if defined ( __CC_ARM )||( __GNUC__ )
DMA_QListTypeDef               I2C_TransmitQueue[4] __attribute__((section(".LPBAM_Section"))) ;
LPBAM_I2C_MasterTxDataDesc_t   MasterTxDataDesc __attribute__((section(".LPBAM_Section"))) ;
LPBAM_I2C_MasterRxDataDesc_t   MasterRxDataDesc __attribute__((section(".LPBAM_Section"))) ;
uint8_t                        tmp_Reg [4] __attribute__((section(".LPBAM_Section"))) ;
uint8_t                        tmp_pData [6] __attribute__((section(".LPBAM_Section"))) ;
int16_t                        x [140] __attribute__((section(".LPBAM_Section"))) ;
int16_t                        y [140] __attribute__((section(".LPBAM_Section"))) ;
int16_t                        z [140] __attribute__((section(".LPBAM_Section"))) ;
DMA_NodeConfTypeDef            dmaNodeConf __attribute__((section(".LPBAM_Section"))) ;
DMA_NodeTypeDef                pNode0  __attribute__((section(".LPBAM_Section")));
DMA_NodeTypeDef                pNode1  __attribute__((section(".LPBAM_Section")));
DMA_NodeTypeDef                pNode2 __attribute__((section(".LPBAM_Section")));
#endif


/* Private functions --------------------------------------------------------*/
static void EnterLowPowerMode(void);
static void Sensors_Aquisition(void);
static void Show_Results(void);
static void LPTIM_Config (void);
static void DMA_LinkedListConfig (void);
/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Sens_Acquisition_Demo(void)
{
  /* IMPLEMENT APPLICATION HERE */
  
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_ST_GRAY_DARK);
  
  UTIL_LCD_FillRect(0, 0, 320, 24,UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Sensor Acquisition", CENTER_MODE);
  
  /* Guide Panel */
  UTIL_LCD_FillRect(2, 30, 316, 184,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_SetFont(&Font16);  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_PURPLE);
  UTIL_LCD_DisplayStringAt(4, 60, (uint8_t *)"Autonomous Motion Sensor ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(4, 80, (uint8_t *)"Acquisition and Data Saving ", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(4, 100, (uint8_t *)"to Internal Memory", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(4, 120, (uint8_t *)"in Stop Mode", CENTER_MODE);
  
  UTIL_LCD_DisplayStringAt(4, 140, (uint8_t *)"* Pls Move the Board * ", CENTER_MODE);
  
  UTIL_LCD_SetFont(&Font12);  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_MAGENTA);
  
  UTIL_LCD_DisplayStringAt(4, 170, (uint8_t *)"(Use Joystick for wakeup)", CENTER_MODE);
  
  UTIL_LCD_DrawRect(66, 186, 194, 26,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(67, 187, 192, 24,UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_FillRect(68, 188, 190, 22,UTIL_LCD_COLOR_BLACK);
  
  UTIL_LCD_SetFont(&Font16); 
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(4, 192, (uint8_t *)"Start Acquisition", CENTER_MODE); 
  
  /* Log Panel */
  UTIL_LCD_FillRect(2, 220, 316, 18,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
  
  UTIL_LCD_DisplayStringAt(40, 222, (uint8_t *)"System in Run mode ", LEFT_MODE);
  
  /* Return button */  
  UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(281, 221, 38, 16,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(282, 222, 36, 14,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(286, 224, (uint8_t *)"BCK", LEFT_MODE);  
  
  
  
  do 
  {
    BSP_TS_GetState(0, &TS_State); 
    while(TS_State.TouchDetected == 0)
    {  
      BSP_TS_GetState(0, &TS_State);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_MAGENTA);
      
      if (count%4 == 0)
      {
        UTIL_LCD_DisplayStringAt(20, 222, (uint8_t *) "/", LEFT_MODE);      
        UTIL_LCD_DisplayStringAt(260, 222, (uint8_t *) "/", LEFT_MODE);      
        
      }
      if (count%4 == 1)
      {
        UTIL_LCD_DisplayStringAt(20, 222, (uint8_t *) "-", LEFT_MODE);      
        UTIL_LCD_DisplayStringAt(260, 222, (uint8_t *) "-", LEFT_MODE);      
      }
      if (count%4 == 2)
      {
        UTIL_LCD_DisplayStringAt(20, 222, (uint8_t *) "\\", LEFT_MODE);      
        UTIL_LCD_DisplayStringAt(260, 222, (uint8_t *) "\\", LEFT_MODE);      
        
      }
      if (count%4 == 3)
      {
        UTIL_LCD_DisplayStringAt(20, 222, (uint8_t *) "|", LEFT_MODE);      
        UTIL_LCD_DisplayStringAt(260, 222, (uint8_t *) "|", LEFT_MODE);      
      }
      
      
      count++;
      HAL_Delay(200);
      
    }
    
    while(TS_State.TouchDetected == 1)
    {  
      BSP_TS_GetState(0, &TS_State);
    }
    HAL_Delay(100);    
    
    if ((TS_State.TouchX > 40) && (TS_State.TouchX < 200) && (TS_State.TouchY > 180) && (TS_State.TouchY < 210)) 
    {
      
      UTIL_LCD_SetFont(&Font16);
      UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
      UTIL_LCD_DisplayStringAt(40, 222, (uint8_t *)"System in Stop mode ", LEFT_MODE);
      
      Sensors_Aquisition();     
      HAL_Delay(100);
      EnterLowPowerMode(); 

      Show_Results();
      HAL_Delay(10);
    }
    
    
    
  } while (( TS_State.TouchX < 270) || (TS_State.TouchY < 200)); 
  
  HAL_NVIC_SystemReset();
}


static void Sensors_Aquisition(void)
{
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct = {0};
  
  /* Re-target the HSI to Clock the I2C3 peripheral */
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C3;
  RCC_PeriphCLKInitStruct.I2c3ClockSelection = RCC_I2C3CLKSOURCE_HSI;
  /* Initialize the RCC extended peripherals clocks */
  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    Error_Handler();
  }    
  
  
  
  
  if (BSP_MOTION_SENSOR_Init(0, MOTION_ACCELERO) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }  
  
  /* Enable function */
  if (BSP_MOTION_SENSOR_Enable(0, MOTION_ACCELERO) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
  
  HAL_Delay(100);
  
  
  memset(I2C_TransmitQueue, 0x0, sizeof(I2C_TransmitQueue));
  memset(x, 0x0, sizeof(x)); 
  memset(y, 0x0, sizeof(y)); 
  memset(z, 0x0, sizeof(z)); 
  
  tmp_Reg[0] = LSM6DSO_OUTX_L_A;
  /* ************************************************************************ */
  /* Read Axes x, y, z from Gyroscope *****************************************/
  /* ************************************************************************ */
  
  /* Set Autonomous Mode configuration */
  AutonomousModeConf.TriggerState     = I2C_AUTO_MODE_ENABLE;
  AutonomousModeConf.TriggerPolarity  = I2C_TRIG_POLARITY_RISING;
  AutonomousModeConf.TriggerSelection = I2C_GRP2_LPTIM3_CH1_TRG;
  if (HAL_I2CEx_SetConfigAutonomousMode(&hbus_i2c3, &AutonomousModeConf) != HAL_OK)
  {
    Error_Handler();
  }


  /* Enable errors interrupt */
  __HAL_I2C_ENABLE_IT(&hbus_i2c3, I2C_IT_ERRI);

  /* Enable DMA request */
  if (ADV_LPBAM_I2C_EnableDMARequests(hbus_i2c3.Instance) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Sequence 1 : Transmit in master mode  Sensor register address (LSM6DSO_OUTX_L_A)  */
  /* Set queue type */
 
  DMAListInfo.QueueType                           = LPBAM_LINEAR_ADDRESSING_Q;

  /* Set transfer information */
  I2C_DataAdvConf.DevAddress                      = LSM6DSO_I2C_ADD_L;
  I2C_DataAdvConf.SequenceNumber                  = 4U;
  I2C_DataAdvConf.AddressingMode                  = LPBAM_I2C_ADDRESSINGMODE_7BIT;
  I2C_DataAdvConf.Size                            = I2C_MEMADD_SIZE_8BIT;
  I2C_DataAdvConf.pData                           = (uint8_t *)&tmp_Reg;
  I2C_DataAdvConf.AutoModeConf.TriggerState       = LPBAM_I2C_AUTO_MODE_ENABLE;

  /* Advanced lpbam I2C master transmit set data */
  if (ADV_LPBAM_I2C_MasterTransmit_SetDataQ(I2C3, &DMAListInfo, &I2C_DataAdvConf, &MasterTxDataDesc, &I2C_TransmitQueue[0]) != LPBAM_OK)
  {
    Error_Handler();
  }  
  
  /* Set transfer information */
  I2C_DataAdvConf.Size  = 6;
  I2C_DataAdvConf.pData = (uint8_t *)&tmp_pData;

  /* Advanced lpbam I2C master receive set data*/
  if (ADV_LPBAM_I2C_MasterReceive_SetDataQ(I2C3, &DMAListInfo, &I2C_DataAdvConf, &MasterRxDataDesc, &I2C_TransmitQueue[0]) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set the circular mode */
  HAL_DMAEx_List_SetCircularMode (&I2C_TransmitQueue[0]);
  
  /* DMA linked-list configuration */
  DMA_LinkedListConfig();  
  

  /* ************************************************************************ */
  /* Get Axe x ****************************************************************/
  /* ************************************************************************ */
  /* Set Node0 type */
  dmaNodeConf.NodeType                            = DMA_LPDMA_LINEAR_NODE;
  
  /* Set common node parameters */
  dmaNodeConf.Init.Request                        = DMA_REQUEST_SW;
  dmaNodeConf.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
  dmaNodeConf.Init.Direction                      = DMA_MEMORY_TO_MEMORY;
  dmaNodeConf.Init.SrcInc                         = DMA_SINC_FIXED;
  dmaNodeConf.Init.DestInc                        = DMA_DINC_INCREMENTED;
  dmaNodeConf.Init.SrcDataWidth                   = DMA_SRC_DATAWIDTH_HALFWORD;
  dmaNodeConf.Init.DestDataWidth                  = DMA_DEST_DATAWIDTH_HALFWORD;
  dmaNodeConf.Init.TransferEventMode              = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  
  dmaNodeConf.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  
  /* Set node trigger parameters */
  dmaNodeConf.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_RISING;
  dmaNodeConf.TriggerConfig.TriggerMode           = DMA_TRIGM_SINGLE_BURST_TRANSFER;
  dmaNodeConf.TriggerConfig.TriggerSelection      = LPDMA1_TRIGGER_LPTIM3_CH1;
  
  dmaNodeConf.SrcAddress = (int32_t)&tmp_pData;
  dmaNodeConf.DstAddress = (int32_t)&x;
  dmaNodeConf.DataSize   = 2*140;
  
  /* Build enable node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConf, &pNode0) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Insert enable node to I2C_TransmitQueue */
  if (HAL_DMAEx_List_InsertNode_Tail(&I2C_TransmitQueue[1], &pNode0) != HAL_OK)
  {
    Error_Handler();
  }
  
  /*Configure LPDMA1 Channel1 */
  DMAHandle2.Instance                         = LPDMA1_Channel1;
  
  /* Set parameters to be configured */
  DMAHandle2.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
  DMAHandle2.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMAHandle2.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DMAHandle2.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  DMAHandle2.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_NORMAL;
  
  /* Initialize DMA linked list */
  if (HAL_DMAEx_List_Init(&DMAHandle2) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Link queue to channel DMA */
  if (HAL_DMAEx_List_LinkQ(&DMAHandle2, &I2C_TransmitQueue[1]) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Configure the NVIC for LPDMA1 Channel 1 */
  HAL_NVIC_SetPriority(LPDMA1_Channel1_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(LPDMA1_Channel1_IRQn);
  
  /* Set the circular mode */
  HAL_DMAEx_List_SetCircularMode (&I2C_TransmitQueue[1]);
  
  /* Start DMA linked list in interrupt mode */
  if (HAL_DMAEx_List_Start(&DMAHandle2) != HAL_OK)
  {
    Error_Handler();
  }
  
  
  /* ************************************************************************ */
  /* Get Axe Y ****************************************************************/
  /* ************************************************************************ */
  /* Set Node1 type */
  dmaNodeConf.NodeType                            = DMA_LPDMA_LINEAR_NODE;
  
  /* Set common node parameters */
  dmaNodeConf.Init.Request                        = DMA_REQUEST_SW;
  dmaNodeConf.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
  dmaNodeConf.Init.Direction                      = DMA_MEMORY_TO_MEMORY;
  dmaNodeConf.Init.SrcInc                         = DMA_SINC_FIXED;
  dmaNodeConf.Init.DestInc                        = DMA_DINC_INCREMENTED;
  dmaNodeConf.Init.SrcDataWidth                   = DMA_SRC_DATAWIDTH_HALFWORD;
  dmaNodeConf.Init.DestDataWidth                  = DMA_DEST_DATAWIDTH_HALFWORD;
  dmaNodeConf.Init.TransferEventMode              = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  
  dmaNodeConf.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  
  /* Set node trigger parameters */
  dmaNodeConf.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_RISING;
  dmaNodeConf.TriggerConfig.TriggerMode           = DMA_TRIGM_SINGLE_BURST_TRANSFER;
  dmaNodeConf.TriggerConfig.TriggerSelection      = LPDMA1_TRIGGER_LPTIM3_CH1;
  
  dmaNodeConf.SrcAddress = (int32_t)&tmp_pData[2];
  dmaNodeConf.DstAddress = (int32_t)&y;
  dmaNodeConf.DataSize   = 2*140;
  
  /* Build enable node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConf, &pNode1) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Insert enable node I2C_TransmitQueue */
  if (HAL_DMAEx_List_InsertNode_Tail(&I2C_TransmitQueue[2], &pNode1) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Configure LPDMA1 Channel2 */
  DMAHandle2.Instance                         = LPDMA1_Channel2;
  
  /* Set parameters to be configured */
  DMAHandle2.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
  DMAHandle2.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMAHandle2.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DMAHandle2.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  DMAHandle2.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_NORMAL;
  
  /* Initialize DMA linked list */
  if (HAL_DMAEx_List_Init(&DMAHandle2) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Link queue to channel DMA */
  if (HAL_DMAEx_List_LinkQ(&DMAHandle2, &I2C_TransmitQueue[2]) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Configure the NVIC for LPDMA1 Channel 2 */
  HAL_NVIC_SetPriority(LPDMA1_Channel2_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(LPDMA1_Channel2_IRQn);
  
  /* Set the circular mode */
  HAL_DMAEx_List_SetCircularMode (&I2C_TransmitQueue[2]);
  
  /* Start DMA linked list in interrupt mode */
  if (HAL_DMAEx_List_Start(&DMAHandle2) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* ************************************************************************ */
  /* Get Axe Z ****************************************************************/
  /* ************************************************************************ */
  /* Set Node2 type */
  dmaNodeConf.NodeType                            = DMA_LPDMA_LINEAR_NODE;
  
  /* Set common node parameters */
  dmaNodeConf.Init.Request                        = DMA_REQUEST_SW;
  dmaNodeConf.Init.BlkHWRequest                   = DMA_BREQ_SINGLE_BURST;
  dmaNodeConf.Init.Direction                      = DMA_MEMORY_TO_MEMORY;
  dmaNodeConf.Init.SrcInc                         = DMA_SINC_FIXED;
  dmaNodeConf.Init.DestInc                        = DMA_DINC_INCREMENTED;
  dmaNodeConf.Init.SrcDataWidth                   = DMA_SRC_DATAWIDTH_HALFWORD;
  dmaNodeConf.Init.DestDataWidth                  = DMA_DEST_DATAWIDTH_HALFWORD;
  dmaNodeConf.Init.TransferEventMode              = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  
  dmaNodeConf.DataHandlingConfig.DataAlignment    = DMA_DATA_RIGHTALIGN_ZEROPADDED;
  
  /* Set node trigger parameters */
  dmaNodeConf.TriggerConfig.TriggerPolarity       = DMA_TRIG_POLARITY_RISING;
  dmaNodeConf.TriggerConfig.TriggerMode           = DMA_TRIGM_SINGLE_BURST_TRANSFER;
  dmaNodeConf.TriggerConfig.TriggerSelection      = LPDMA1_TRIGGER_LPTIM3_CH1;
  
  dmaNodeConf.SrcAddress = (int32_t)&tmp_pData[4];
  dmaNodeConf.DstAddress = (int32_t)&z;
  dmaNodeConf.DataSize   = 2*140;
  
  /* Build enable node */
  if (HAL_DMAEx_List_BuildNode(&dmaNodeConf, &pNode2) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Insert enable node to I2C_TransmitQueue */
  if (HAL_DMAEx_List_InsertNode_Tail(&I2C_TransmitQueue[3], &pNode2) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Configure LPDMA1 Channel3 */
  DMAHandle2.Instance                         = LPDMA1_Channel3;
  
  /* Set parameters to be configured */
  DMAHandle2.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
  DMAHandle2.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMAHandle2.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DMAHandle2.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  DMAHandle2.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_NORMAL;
  
  /* Initialize DMA linked list */
  if (HAL_DMAEx_List_Init(&DMAHandle2) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Link queue to channel DMA */
  if (HAL_DMAEx_List_LinkQ(&DMAHandle2, &I2C_TransmitQueue[3]) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Configure the NVIC for LPDMA1 Channel 3 */
  HAL_NVIC_SetPriority(LPDMA1_Channel3_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(LPDMA1_Channel3_IRQn);
  
  HAL_DMAEx_List_SetCircularMode (&I2C_TransmitQueue[3]);
  
  /* Start DMA linked list in interrupt mode */
  if (HAL_DMAEx_List_Start(&DMAHandle2) != HAL_OK)
  {
    Error_Handler();
  }
  
  LPTIM_Config();
  
  
}



void Show_Results(void)
{
  
  if (HAL_LPTIM_PWM_Stop(&LPTIMHandle, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* Results Panel */
  UTIL_LCD_FillRect(2, 30, 316, 210,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_SetFont(&Font12);  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_DisplayStringAt(20, 40, (uint8_t *)"X ", LEFT_MODE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
  UTIL_LCD_DisplayStringAt(20, 52, (uint8_t *)"Y ", LEFT_MODE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DisplayStringAt(20, 64, (uint8_t *)"Z ", LEFT_MODE);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_DisplayStringAt(280, 124, (uint8_t *)"Time", LEFT_MODE);
  
  UTIL_LCD_DrawLine(20, 120, 280,120,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_DrawLine(30, 40, 30,210,UTIL_LCD_COLOR_BLACK);
  
  UTIL_LCD_SetFont(&Font12);  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_MAGENTA);
  
  /* Return button */  
  UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(281, 221, 38, 16,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(282, 222, 36, 14,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(286, 224, (uint8_t *)"BCK", LEFT_MODE);  
  
  
  
  for(int i=0;i<140;i++)
  {
    x[i] = (int16_t) ( x[i]*LSM6DSO_ACC_SENSITIVITY_FS_2G/20 );
    y[i] = (int16_t) ( y[i]*LSM6DSO_ACC_SENSITIVITY_FS_2G/20);
    
    z[i] = (int16_t) ( z[i]*LSM6DSO_ACC_SENSITIVITY_FS_2G/20); 
    
    
  }
  
  for(int i=0;i<140-1;i++)
  {
    
    UTIL_LCD_DrawLine(30+ 2*i, 120-x[i], 30+ 2*(i+1), 120-x[i+1], UTIL_LCD_COLOR_RED);
    UTIL_LCD_DrawLine(30+ 2*i, 120-y[i], 30+ 2*(i+1), 120-y[i+1], UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_DrawLine(30+ 2*i, 120-z[i], 30+ 2*(i+1), 120-z[i+1], UTIL_LCD_COLOR_BLUE);    
  }
  
  
  do 
  {
    BSP_TS_GetState(0, &TS_State); 
    while(TS_State.TouchDetected == 0)
    {  
      BSP_TS_GetState(0, &TS_State);
    }
    while(TS_State.TouchDetected == 1)
    {  
      BSP_TS_GetState(0, &TS_State);
    }
    HAL_Delay(100);    
    
    
  } while (( TS_State.TouchX < 270) || (TS_State.TouchY < 210)); 
  
  HAL_Delay(100);
}

/**
* @brief  Enter low power mode
* @param  None
* @retval None
*/
static void EnterLowPowerMode(void)
{
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enter the system to STOP mode */
  HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);

}

/**
* @brief  LPTIM configuration.
* @param  None
* @retval None
*/
static void LPTIM_Config (void)
{
  LPTIM_OC_ConfigTypeDef sConfig;
  

  uint32_t Period = 0x4ff;

  uint32_t Pulse  = Period / 2U;
  
  /* Set lptim instance */
  LPTIMHandle.Instance = LPTIM3;
  
  /* Set parameters to be configured */
  LPTIMHandle.Init.CounterSource                 = LPTIM_COUNTERSOURCE_INTERNAL;
  LPTIMHandle.Init.UpdateMode                    = LPTIM_UPDATE_IMMEDIATE;
  LPTIMHandle.Init.Clock.Source                  = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  LPTIMHandle.Init.Clock.Prescaler               = LPTIM_PRESCALER_DIV1;
  LPTIMHandle.Init.UltraLowPowerClock.Polarity   = LPTIM_CLOCKPOLARITY_RISING;
  LPTIMHandle.Init.UltraLowPowerClock.SampleTime = LPTIM_CLOCKSAMPLETIME_DIRECTTRANSITION;
  LPTIMHandle.Init.Trigger.Source                = LPTIM_TRIGSOURCE_SOFTWARE;
  LPTIMHandle.Init.Trigger.ActiveEdge            = LPTIM_ACTIVEEDGE_RISING;
  LPTIMHandle.Init.Trigger.SampleTime            = LPTIM_TRIGSAMPLETIME_DIRECTTRANSITION;
  LPTIMHandle.Init.Input1Source                  = LPTIM_INPUT1SOURCE_GPIO;
  LPTIMHandle.Init.Input2Source                  = LPTIM_INPUT2SOURCE_GPIO;
  LPTIMHandle.Init.Period                        = Period;
  LPTIMHandle.Init.RepetitionCounter             = 1;
  
  /* Initialize LPTIM peripheral according to the specified parameters */
  if (HAL_LPTIM_Init (&LPTIMHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Select parameters to be configured */
  sConfig.Pulse      = Pulse;
  sConfig.OCPolarity = LPTIM_OCPOLARITY_HIGH;
  
  /* Configure the current channel */
  if (HAL_LPTIM_OC_ConfigChannel (&LPTIMHandle, &sConfig, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Generate one pulse : LPTIM_OUT is used to trig the DMA in order to start
  transactions                                                             */
  if (HAL_LPTIM_PWM_Start(&LPTIMHandle, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  I2C DMA Linked list  configuration.
  * @param  None
  * @retval None
  */
static void DMA_LinkedListConfig (void)
{
  /* Set DMA instance */
  DMAHandle1.Instance                         = LPDMA1_Channel0;

  /* Set parameters to be configured */
  DMAHandle1.InitLinkedList.Priority          = DMA_HIGH_PRIORITY;
  DMAHandle1.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DMAHandle1.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DMAHandle1.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  DMAHandle1.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;

  /* Initialize DMA linked list */
  if (HAL_DMAEx_List_Init(&DMAHandle1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Link queue to channel DMA */
  if (HAL_DMAEx_List_LinkQ(&DMAHandle1, &I2C_TransmitQueue[0]) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure the NVIC for LPDMA1 Channel 0 */
  HAL_NVIC_SetPriority(LPDMA1_Channel0_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(LPDMA1_Channel0_IRQn);

  /* Start DMA linked list in interrupt mode */
  if (HAL_DMAEx_List_Start_IT(&DMAHandle1) != HAL_OK)
  {
    Error_Handler();
  }
}
/**
* @brief  Get User action
* @param  sel User selection (JOY_SEL,...)
* @note   This example is the only way to get user information.
* @retval None
*/
void Sens_Acquisition_UserAction(uint8_t sel)
{
  UserEvent = sel;
  UserEntry = 1;
}
