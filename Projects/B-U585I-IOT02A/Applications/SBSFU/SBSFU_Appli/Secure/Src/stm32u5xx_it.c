/**
  ******************************************************************************
  * @file    stm32u5xx_it.c
  * @author  MCD Application Team
  * @brief   Secure Interrupt Service Routines.
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
#include "main.h"
#include "stm32u5xx_it.h"
/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private user code ---------------------------------------------------------*/


/* External variables --------------------------------------------------------*/
extern funcptr_NS pSecureFaultCallback;
extern funcptr_NS pSecureErrorCallback;


/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* Reset for Test, Non Secure Fault Memory Fault are escaladed in Secure HW fault  */
  /* Since Non Secure Memory Fault Interrupt is not activated */
  NVIC_SystemReset();  

}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{

  while (1)
  {

  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{

  while (1)
  {

  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{

  while (1)
  {

  }
}

/**
  * @brief This function handles Secure fault.
  */
void SecureFault_Handler(void)
{

  funcptr_NS callback_NS; /* non-secure callback function pointer */

  if (pSecureFaultCallback != (funcptr_NS)NULL)
  {
    /* return function pointer with cleared LSB */
    callback_NS = (funcptr_NS)cmse_nsfptr_create(pSecureFaultCallback);

    callback_NS();
  }
  else
  {

    while (1)
    {

    }

  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{

}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  while (1)
  {
  }

}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  while (1)
  {
  }

}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{

  HAL_IncTick();

  HAL_SYSTICK_Callback();

}

/******************************************************************************/
/* STM32U5xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32u5xx.c).                    */
/******************************************************************************/

/**
  * @brief This function handles Global TrustZone controller global interrupt.
  */
void GTZC_IRQHandler(void)
{

  funcptr_NS callback_NS; /* non-secure callback function pointer */

  HAL_GTZC_IRQHandler();

  if (pSecureErrorCallback != (funcptr_NS)NULL)
  {
    /* return function pointer with cleared LSB */
    callback_NS = (funcptr_NS)cmse_nsfptr_create(pSecureErrorCallback);

    callback_NS();
  }
  else
  {
    /* Something went wrong in test case */
    while (1);
  }

}