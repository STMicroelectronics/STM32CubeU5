/**
  ******************************************************************************
  * @file    stm32u5xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
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
#include "stm32u5xx_hal.h"
#include "stm32u5xx_it.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern SAI_HandleTypeDef SaiHandle;
extern MDF_HandleTypeDef MDFHandle;
extern MDF_HandleTypeDef MdfFallingHandle;
extern MDF_HandleTypeDef MdfRisingHandle;
extern DMA_HandleTypeDef DAC_DMAHandle;
extern RTC_HandleTypeDef RtcHandle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M33 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32U5xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32u5xx.s).                                               */
/******************************************************************************/


/**
  * @brief  This function handles EXTI13 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI13_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&hpb_exti[0]);
}

/**
  * @brief  This function handles EXTI1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&hpb_exti[1]);
}

/**
  * @brief  This function handles EXTI15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_IRQHandler(void)
{
  uint32_t i=0;
  do {i++;}
  while(i<5000000);
  HAL_EXTI_IRQHandler(&hts_exti[0]);
}

/**
  * @brief  This function handles DCMI_PSSI interrupt request.
  * @param  None
  * @retval None
  */
void DCMI_PSSI_IRQHandler(void)
{
  HAL_DCMI_IRQHandler(&hcamera_dcmi);
}

/**
  * @brief  This function handles GPDMA_Channel12 interrupt request.
  * @param  None
  * @retval None
  */
void GPDMA1_Channel12_IRQHandler(void)
{
  HAL_DMA_IRQHandler(hcamera_dcmi.DMA_Handle);
}

/**
  * @brief  This function handles EXTI5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI5_IRQHandler(void)
{
  BSP_JOY_IRQHandler(JOY1, JOY_ALL);  
}

/**
  * @brief  This function handles SDMMC1 interrupt request.
  * @param  None
  * @retval None
  */
void SDMMC1_IRQHandler(void)
{
  HAL_SD_IRQHandler(&hsd_sdmmc[0]);
}


/* Audio Module */

/**
  * @brief  This function handles SAI DMA interrupt request.
  * @param  None
  * @retval None
  */
void GPDMA1_Channel0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(SaiHandle.hdmatx);
}


/**
  * @brief  This function handles MDF DMA interrupt request.
  * @param  None
  * @retval None
  */
void GPDMA1_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(MDFHandle.hdma);
}

/**
  * @brief  This function handles MDF DMA interrupt request.
  * @param  None
  * @retval None
  */
void LPDMA1_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(MDFHandle.hdma);
}
/**
  * @brief  This function handles ADF interrupt request.
  * @param  None
  * @retval None
  */
void ADF1_IRQHandler(void)
{
  HAL_MDF_IRQHandler(&MDFHandle);
}

/**
  * @brief  This function handles ADF interrupt request.
  * @param  None
  * @retval None
  */
void MDF1_FLT3_IRQHandler(void)
{
  HAL_MDF_IRQHandler(&MdfFallingHandle);
}

void MDF1_FLT4_IRQHandler(void)
{
  HAL_MDF_IRQHandler(&MdfRisingHandle);
}
/* LPBAM Modules */


/**
  * @brief  This function handles DMA channel interrupt request.
  * @param  None
  * @retval None
  */
void LPDMA1_Channel0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&DAC_DMAHandle);
}



/**
  * @brief  This function handles STOP 3 Wake Up interrupt request.
  * @param  None
  * @retval None
  */
void PWR_S3WU_IRQHandler(void)
{
  HAL_PWREx_S3WU_IRQHandler(PWR_WAKEUP_PIN2);
}

/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
  HAL_RTCEx_WakeUpTimerIRQHandler(&RtcHandle);
  HAL_RTC_AlarmIRQHandler(&RtcHandle);
}
