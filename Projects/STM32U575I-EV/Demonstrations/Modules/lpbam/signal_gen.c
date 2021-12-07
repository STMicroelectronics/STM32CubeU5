/**
  ******************************************************************************
  * @file    sens_acquisition.c
  * @author  MCD Application Team
  * @brief   Signal generator implementation
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
#include "signal_gen.h"
#include "stm32_lpbam.h"

/* Private typedef ----------------------------------------------------------*/
/* Private constants ----------------------------------------------------------*/
#define TRIANGLE_SIGNAL 0
#define SINUS_SIGNAL 1
#define SQAURE_SIGNAL 2
#define DC_SIGNAL 3
/* Private Variables ---------------------------------------------------------*/
/* DAC handler declaration         */
DAC_HandleTypeDef   DACHandle;
/* LPTIM DAC handle declaration    */
LPTIM_HandleTypeDef LPTIMHandle;
/* DAC DMA handler declaration     */
DMA_HandleTypeDef   DAC_DMAHandle;

/* LPBAM variables declaration */
LPBAM_DMAListInfo_t         DMAListInfo;
LPBAM_DAC_DataAdvConf_t     DAC_DataAdvConf;
LPBAM_COMMON_DataAdvConf_t  DataConfig;

/* Place following data in section  SDRAM4 LPBAM_Section */
#if defined ( __ICCARM__ )
#pragma default_variable_attributes =@".LPBAM_Section"

LPBAM_DAC_ConvDataDesc_t    DAC_DataDesc;
uint32_t   Signal_Buffer[DAC_SIGNAL_DATASIZE] ;
/* DMA node configuration declaration */
DMA_NodeConfTypeDef dmaNodeConfig;
/* DMA queues declaration */
DMA_QListTypeDef    DAC_StartQueue;

/* Stop placing functions in section SDRAM4 LPBAM_Section */
#pragma default_variable_attributes =
#endif

#if defined ( __CC_ARM )||( __GNUC__ )
LPBAM_DAC_ConvDataDesc_t    DAC_DataDesc __attribute__((section(".LPBAM_Section"))) ;
uint32_t   Signal_Buffer[DAC_SIGNAL_DATASIZE] __attribute__((section(".LPBAM_Section")));
/* DMA node configuration declaration */
DMA_NodeConfTypeDef dmaNodeConfig __attribute__((section(".LPBAM_Section"))) ;
/* DMA queues declaration */
DMA_QListTypeDef    DAC_StartQueue __attribute__((section(".LPBAM_Section"))) ;
#endif


/* Private function prototypes -----------------------------------------------*/
/* System static APIs */

/* LPTIM static API */
void LPTIM_Config              (void);
/* DAC static APIs */
static void DAC_Config                (void);
static void DMA_DAC_LinkedListConfig  (void);

static void EnterLowPowerMode(void);
static void Signal_Generation(uint32_t SignalIndex) ;   

/* Used to exit application */
static __IO uint8_t UserEvent=0;
static __IO uint8_t UserEntry=0;
static uint32_t count=0;;



static uint8_t SignalTypeStr[10][10]={"Triangle"," Square "," Sinus  ","   DC   "};

static uint8_t SignalIndex = 0;
/* Private typedef -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void SigGenerator_Demo(void)
{
  uint32_t joyState = JOY_NONE;
  uint32_t Button   = 0;
  /* IMPLEMENT APPLICATION HERE */
  
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_ST_GRAY_DARK);
  
  UTIL_LCD_FillRect(0, 0, 320, 24,UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Signal Generator", CENTER_MODE);
  
  /* Control Panel */
  
  UTIL_LCD_FillRect(2, 30, 316, 70,UTIL_LCD_COLOR_WHITE);  
  
  UTIL_LCD_SetFont(&Font20);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_ST_GRAY);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_PURPLE);
  UTIL_LCD_DisplayStringAt(4, 36, (uint8_t *)"     Signal Type     ", CENTER_MODE);
  
  
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
  UTIL_LCD_DisplayStringAt(4, 72, (uint8_t *)"Triangle", CENTER_MODE);
  
  /* Button*/
  UTIL_LCD_DrawRect(110, 66, 100, 26,UTIL_LCD_COLOR_BLACK); 
  UTIL_LCD_DrawRect(111, 67,  98, 24,UTIL_LCD_COLOR_ST_YELLOW); 
  
  UTIL_LCD_DrawRect(60, 70, 42, 20,UTIL_LCD_COLOR_RED);
  UTIL_LCD_DrawRect(59, 69, 44, 22,UTIL_LCD_COLOR_RED);
  UTIL_LCD_FillRect(61, 71, 40, 18,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(62, 72, 38, 16,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(70, 72, (uint8_t *)"<<", LEFT_MODE);    
  
  UTIL_LCD_DrawRect(220, 70, 42, 20,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(221, 71, 40, 18,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(222, 72, 38, 16,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(230, 72, (uint8_t *)">>", LEFT_MODE); 
  
  
  
  /* Guide Panel */
  UTIL_LCD_FillRect(2, 106, 316, 110,UTIL_LCD_COLOR_WHITE);    
  
  UTIL_LCD_SetFont(&Font16);  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_PURPLE);
  UTIL_LCD_DisplayStringAt(2, 110, (uint8_t *)"Autonomous Signal Generation", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(4, 130, (uint8_t *)"in Stop Mode", CENTER_MODE);

  UTIL_LCD_SetFont(&Font12);  
  UTIL_LCD_DisplayStringAt(4, 150, (uint8_t *)"(Connect Scope to CN8)", CENTER_MODE);
  
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_MAGENTA);

  UTIL_LCD_DisplayStringAt(4, 170, (uint8_t *)"(Use Joystick for wakeup)", CENTER_MODE);
  
  UTIL_LCD_DrawRect(60, 186, 190, 26,UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_FillRect(61, 187, 188, 24,UTIL_LCD_COLOR_ST_YELLOW); 
  UTIL_LCD_FillRect(62, 188, 186, 22,UTIL_LCD_COLOR_BLACK);  
  
  UTIL_LCD_SetFont(&Font16); 
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_YELLOW);
  UTIL_LCD_DisplayStringAt(63, 192, (uint8_t *)"Start Generation", LEFT_MODE); 
  
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
    joyState = JOY_NONE;
    while (joyState == JOY_NONE)
    {  
      joyState = BSP_JOY_GetState(JOY1);
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
    
    /* anti bounding assert */ 
    while (BSP_JOY_GetState(JOY1) != JOY_NONE);
    HAL_Delay(100);


    
    if (joyState != JOY_SEL)
    {    
      Button++;
      if (Button == 4)
      {
        Button = 0;
      }
      UTIL_LCD_DrawRect(60, 70, 42, 20,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(59, 69, 44, 22,UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DrawRect(220, 70, 42, 20,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(219, 69, 44, 22,UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DrawRect(60, 186, 190, 26,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(59, 185, 192, 28,UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_BLACK);
      UTIL_LCD_DrawRect(279, 219, 41, 20,UTIL_LCD_COLOR_WHITE);
      switch(Button)
      {
      case 0 :
        UTIL_LCD_DrawRect(60, 70, 42, 20,UTIL_LCD_COLOR_RED);
        UTIL_LCD_DrawRect(59, 69, 44, 22,UTIL_LCD_COLOR_RED);
        break;
      case 1 :
        UTIL_LCD_DrawRect(220, 70, 42, 20,UTIL_LCD_COLOR_RED);
        UTIL_LCD_DrawRect(219, 69, 44, 22,UTIL_LCD_COLOR_RED);
        break;
      case 2 :
        UTIL_LCD_DrawRect(60, 186, 190, 26,UTIL_LCD_COLOR_RED);
        UTIL_LCD_DrawRect(59, 185, 192, 28,UTIL_LCD_COLOR_RED);
        break;
      case 3 :
        UTIL_LCD_DrawRect(280, 220, 40, 18,UTIL_LCD_COLOR_RED);
        UTIL_LCD_DrawRect(279, 219, 41, 20,UTIL_LCD_COLOR_RED);
        break;        
      default:
        break;
      }

        
      
    }


    
    if (joyState == JOY_SEL) 
    {
      
      
      if (Button == 0)
      {
        if (SignalIndex > 0)
          SignalIndex--;
        else
          SignalIndex=4;
        
        UTIL_LCD_SetFont(&Font16);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
        UTIL_LCD_DisplayStringAt(4, 72, SignalTypeStr[SignalIndex], CENTER_MODE); 
        
        
      }
      if (Button == 1)
      {
        
        if (SignalIndex < 3)
          SignalIndex++;
        else
          SignalIndex=0;
        
        UTIL_LCD_SetFont(&Font16);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_BLUE_DARK);
        UTIL_LCD_DisplayStringAt(4, 72, SignalTypeStr[SignalIndex], CENTER_MODE); 
        
      }
      
      if (Button == 2)
      {
                       
        UTIL_LCD_SetFont(&Font16);
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
        UTIL_LCD_DisplayStringAt(40, 222, (uint8_t *)"System in Stop mode ", LEFT_MODE);
        
        Signal_Generation(SignalIndex);  
        EnterLowPowerMode();  
        SystemClock_Config();
        
        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GREEN);
        
        UTIL_LCD_DisplayStringAt(40, 222, (uint8_t *)"System in Run mode ", LEFT_MODE); 
        HAL_Delay(10);
      }
      
      if (Button == 3)
      {
        break;  
      }
    }

    
    
  } while (1);
  
  HAL_NVIC_SystemReset();
}

static void Signal_Generation(uint32_t SignalIndex)     
{

  if (SignalIndex == TRIANGLE_SIGNAL)
    memcpy(Signal_Buffer,TriangularSignal_Buffer,sizeof(Signal_Buffer)) ;
  if (SignalIndex == 2)
    memcpy(Signal_Buffer,SinusSignal_Buffer,sizeof(Signal_Buffer)) ;
  if (SignalIndex == SINUS_SIGNAL)
    memcpy(Signal_Buffer,SqaureSignal_Buffer,sizeof(Signal_Buffer)) ;
  if (SignalIndex == DC_SIGNAL) 
  {
    for (int i=0; i<DAC_SIGNAL_DATASIZE;i++)
      Signal_Buffer[i] = 4064;
  }
  
  memset(  &DAC_StartQueue,0,sizeof(DAC_StartQueue)); 
  
  /*
  *   ######## LPBAM DAC configuration ########
  */
  
  /* DAC Initialization and configuration */
  DAC_Config();
  
  /* Set DMA information */
  DMAListInfo.pInstance       = LPDMA1;
  DMAListInfo.QueueType       = LPBAM_LINEAR_ADDRESSING_Q;

  /* Set data information */
  DAC_DataAdvConf.Alignment   = LPBAM_DAC_ALIGN_12B_R;
  DAC_DataAdvConf.Size        = DAC_SIGNAL_DATASIZE;
  DAC_DataAdvConf.pData       = (uint32_t *)Signal_Buffer;

  /* Advanced lpbam DAC start conversion */
  if (ADV_LPBAM_DAC_Conversion_SetDataQ(DAC1, LPBAM_DAC_CHANNEL_1, &DMAListInfo, &DAC_DataAdvConf, &DAC_DataDesc, &DAC_StartQueue) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Update DAC DMA queue transfer event generation */
  DataConfig.TransferConfig.Transfer.TransferEventMode = LPBAM_DMA_TCEM_BLOCK_TRANSFER;
  DataConfig.UpdateSrcInc                              = DISABLE;
  DataConfig.UpdateDestInc                             = DISABLE;
  DataConfig.UpdateSrcDataWidth                        = DISABLE;
  DataConfig.UpdateDestDataWidth                       = DISABLE;
  DataConfig.UpdateTransferEventMode                   = ENABLE;
  if (ADV_LPBAM_Q_SetDataConfig(&DataConfig, LPBAM_DAC_CONVERSION_DATAQ_DATA_NODE, &DAC_DataDesc) != LPBAM_OK)
  {
    Error_Handler();
  }

  /* Set circular mode for linked-list queue */
  if (ADV_LPBAM_Q_SetCircularMode(&DAC_DataDesc, LPBAM_DAC_CONVERSION_DATAQ_DATA_NODE, &DAC_StartQueue) != LPBAM_OK)
  {
    Error_Handler ();
  }

  /* Link DAC queue and start DMA channel */
  DMA_DAC_LinkedListConfig();

 

  /* LPTIM Initialization sequence */
  LPTIM_Config();

  /* Enable PWR clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  
}



/**
* @brief  LPTIM configuration.
* @param  None
* @retval None
*/
void LPTIM_Config(void)
{
  LPTIM_OC_ConfigTypeDef  sConfig;
  
  /* Set LPTIM instance */
  LPTIMHandle.Instance                           = LPTIM3;
  /* Set parameter to be configured */
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
  LPTIMHandle.Init.Period                        = 2U;
  
  /* Init LPTIM peripheral */
  if (HAL_LPTIM_Init(&LPTIMHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Set parameter to be configured */
  sConfig.Pulse      = LPTIMHandle.Init.Period / 2U;
  sConfig.OCPolarity = LPTIM_OCPOLARITY_HIGH;
  
  /* Configure LPTIM peripheral */
  if (HAL_LPTIM_OC_ConfigChannel(&LPTIMHandle, &sConfig, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* LPTIM start PWM */
  if (HAL_LPTIM_PWM_Start(&LPTIMHandle, LPTIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
* @brief  DAC configuration.
* @param  None
* @retval None
*/
static void DAC_Config(void)
{
  DAC_ChannelConfTypeDef        dacChannelConfig;
  DAC_AutonomousModeConfTypeDef AutonomousModeConfig;

  /* Set DAC instance */
  DACHandle.Instance = DAC1;

  /* Init DAC peripheral */
  if(HAL_DAC_Init(&DACHandle) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set parameter to be configured */
  dacChannelConfig.DAC_AutonomousMode                      = DAC_AUTONOMOUS_MODE_ENABLE;
  dacChannelConfig.DAC_HighFrequency                       = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
  dacChannelConfig.DAC_DMADoubleDataMode                   = DISABLE;
  dacChannelConfig.DAC_SignedFormat                        = DISABLE;
  dacChannelConfig.DAC_Trigger                             = DAC_TRIGGER_LPTIM3_OUT;
  dacChannelConfig.DAC_OutputBuffer                        = DAC_OUTPUTBUFFER_DISABLE;
  dacChannelConfig.DAC_ConnectOnChipPeripheral             = DAC_CHIPCONNECT_BOTH;
  dacChannelConfig.DAC_UserTrimming                        = DAC_TRIMMING_USER;
  dacChannelConfig.DAC_TrimmingValue                       = 16U;
  /* Setup DAC sample and hold */
  dacChannelConfig.DAC_SampleAndHold                       = DAC_SAMPLEANDHOLD_ENABLE;
  dacChannelConfig.DAC_SampleAndHoldConfig.DAC_SampleTime  = 10U;
  dacChannelConfig.DAC_SampleAndHoldConfig.DAC_HoldTime    = 20U;
  dacChannelConfig.DAC_SampleAndHoldConfig.DAC_RefreshTime = 5U;

  /* Configures the DAC channel1 */
  if (HAL_DAC_ConfigChannel(&DACHandle, &dacChannelConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Set autonomous mode configuration */
  AutonomousModeConfig.AutonomousModeState = DAC_AUTONOMOUS_MODE_ENABLE;
  if (HAL_DACEx_SetConfigAutonomousMode(&DACHandle, &AutonomousModeConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable DMA request */
  if (ADV_LPBAM_DAC_EnableDMARequests(DACHandle.Instance, LPBAM_DAC_CHANNEL_1) != LPBAM_OK)
  {
    Error_Handler();
  }
}


/**
* @brief  DMA DAC linked list configuration.
* @param  None
* @retval None
*/
static void DMA_DAC_LinkedListConfig(void)
{
  /* Enable LPDMA1 clock */
  __HAL_RCC_LPDMA1_CLK_ENABLE();
  
  /* Set DMA instance */
  DAC_DMAHandle.Instance                         = LPDMA1_Channel0;
  /* Set linked list configuration */
  DAC_DMAHandle.InitLinkedList.Priority          = DMA_LOW_PRIORITY_LOW_WEIGHT;
  DAC_DMAHandle.InitLinkedList.LinkStepMode      = DMA_LSM_FULL_EXECUTION;
  DAC_DMAHandle.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  DAC_DMAHandle.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  DAC_DMAHandle.InitLinkedList.LinkedListMode    = DMA_LINKEDLIST_CIRCULAR;
  
  /* DMA linked list init */
  if(HAL_DMAEx_List_Init(&DAC_DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Link DAC queue to DMA channel */
  if(HAL_DMAEx_List_LinkQ(&DAC_DMAHandle, &DAC_StartQueue) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Enable DMA error interrupts */
  __HAL_DMA_ENABLE_IT(&DAC_DMAHandle, DMA_IT_DTE | DMA_IT_ULE | DMA_IT_USE | \
    DMA_IT_TO /*| DMA_IT_TC*/);
  
  /* Enable NVIC for DMA channel */
  HAL_NVIC_SetPriority(LPDMA1_Channel0_IRQn, 0U, 0U);
  HAL_NVIC_EnableIRQ(LPDMA1_Channel0_IRQn);
  
  /* Start DMA linked list */
  if (HAL_DMAEx_List_Start(&DAC_DMAHandle) != HAL_OK)
  {
    Error_Handler();
  }
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
  
  /* Enter the system to STOP1 mode */
  HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
}
/**
* @brief  Get User action
* @param  sel User selection (JOY_SEL,...)
* @note   This example is the only way to get user information.
* @retval None
*/
void SigGenerator_UserAction(uint8_t sel)
{
  UserEvent = sel;
  UserEntry = 1;
}
