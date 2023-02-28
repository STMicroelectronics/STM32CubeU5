/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    : Secure/Src/secure_nsc.c
  * @author  : MCD Application Team
  * @brief   : This file contains the non-secure callable APIs (secure world)
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
/* USER CODE END Header */

/* USER CODE BEGIN Non_Secure_CallLib */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "secure_nsc.h"
#include <stdio.h>

/** @addtogroup STM32U5xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Global variables ----------------------------------------------------------*/
void *pSecureFaultCallback = NULL;   /* Pointer to secure fault callback in Non-secure */
void *pSecureErrorCallback = NULL;   /* Pointer to secure error callback in Non-secure */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Secure registration of non-secure callback.
  * @param  CallbackId  callback identifier
  * @param  func        pointer to non-secure function
  * @retval None
  */
CMSE_NS_ENTRY void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef CallbackId, void *func)
{
  if(func != NULL)
  {
    switch(CallbackId)
    {
      case SECURE_FAULT_CB_ID:           /* SecureFault Interrupt occurred */
        pSecureFaultCallback = func;
        break;
      case GTZC_ERROR_CB_ID:             /* GTZC Interrupt occurred */
        pSecureErrorCallback = func;
        break;
      default:
        /* unknown */
        break;
    }
  }
}

/**
* @brief  Secure call to printf (COM port is assigned to secure).
* @param  format string to display
* @retval None
*/
CMSE_NS_ENTRY void SECURE_DisplayMessage(const uint8_t *format)
{
  printf("%s", (const char *)format);
}

/**
* @brief  Secure call to toggle LED_RED (assigned to secure)
* @retval None
*/
CMSE_NS_ENTRY void SECURE_ToggleRedLed(void)
{
  HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);
}

/**
* @brief  Configure SAU and MPCWM2 in initial conditions
* @note   SAU and MPCWM2 are setup so that only first half SRAM memory
*         is accessible from non-secure
*         Also used to restore initial configuration with
*         External SRAM memory split into 2 halves:
*         0x60000000-0x600FFFFF : non-secure
*         0x60100000-0x601FFFFF : secure
* @retval None
*/
CMSE_NS_ENTRY void SECURE_SAU_MPCWM_SetInitConfig(void)
{
  MPCWM_ConfigTypeDef MPCWM_Desc;
  /* protect this SAU/MPCWM2 setup section from any fault interrupt occurrence */
  __disable_fault_irq();

  /* Set first half of memory as non-secure, second half as secure */

  /* SAU update based on partition_stm32u5xx.h used at setup      */
  /* Reduced non-secure area (SAU region 4) : 0x60000000-0x600FFFFF */
  /* memory from 0x60100000 are then secure */
  SAU->RNR  = (4 & SAU_RNR_REGION_Msk);
  SAU->RBAR = (FMC_BASE & SAU_RBAR_BADDR_Msk);
  SAU->RLAR = (  ((FMC_BASE + 0xFFFFFUL) & SAU_RLAR_LADDR_Msk)
               | ((0 << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk)
               | 1U);

  /* MPCWM2: set one non-secure area on first half */
  /* The second half is set as secure by default */
  MPCWM_Desc.AreaId     = GTZC_TZSC_MPCWM_ID1;
  MPCWM_Desc.Offset     = 0 * GTZC_TZSC_MPCWM_GRANULARITY_1;
  MPCWM_Desc.Length     = 8 * GTZC_TZSC_MPCWM_GRANULARITY_1;
  MPCWM_Desc.AreaStatus = ENABLE;
  MPCWM_Desc.Attribute  = GTZC_TZSC_MPCWM_REGION_NSEC;
  MPCWM_Desc.Lock       = GTZC_TZSC_MPCWM_LOCK_OFF;

  if (HAL_GTZC_TZSC_MPCWM_ConfigMemAttributes(FMC_BASE, &MPCWM_Desc) != HAL_OK)
  {
    /* Initialization error */
    while(1);
  }

  /* re-enable fault interrupt occurrences after this SAU/MPCWM2 setup section */
  __enable_fault_irq();
}

/**
* @brief  Configure SAU and MPCWM2 in User Button Error Case
* @note   SAU and MPCWM2 are setup so that whole SRAM memory is
*         accessible from non-secure
* @retval None
*/
CMSE_NS_ENTRY void SECURE_SAU_MPCWM_SetSecureFaultConfig(void)
{
  MPCWM_ConfigTypeDef MPCWM_Desc;

  /* protect this SAU/MPCWM2 setup section from any fault interrupt occurrence */
  __disable_fault_irq();

  /* Here, with initial configuration, a non-secure access has already been done
  * in the secure area and the Secure Fault interrupt occurred. This function
  * is called from the handler to allow the SW to exit the handler (jump to the
  * address which generated the IRQ) with conditions which will allow the
  * execution of the operation (unless this is an infinite loop).
  * So the process here is to set the whole external SRAM memory as non-secure,
  * on both SAU and MPCWM2 sides.
  */

  /* Whole non-secure area (SAU region 4) : 0x60000000-0x601FFFFF */
  SAU->RNR  = (4 & SAU_RNR_REGION_Msk);
  SAU->RBAR = (FMC_BASE & SAU_RBAR_BADDR_Msk);
  SAU->RLAR = (  ((FMC_BASE + 0x1FFFFFUL) & SAU_RLAR_LADDR_Msk)
               | ((0 << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk)
               | 1U);

  /* MPCWM2: set one non-secure area on whole memory */
  MPCWM_Desc.AreaId = GTZC_TZSC_MPCWM_ID1;
  MPCWM_Desc.Offset = 0;
  MPCWM_Desc.Length = 16 * GTZC_TZSC_MPCWM_GRANULARITY_1;
  MPCWM_Desc.AreaStatus = ENABLE;
  MPCWM_Desc.Attribute  = GTZC_TZSC_MPCWM_REGION_NSEC;
  MPCWM_Desc.Lock       = GTZC_TZSC_MPCWM_LOCK_OFF;

  if (HAL_GTZC_TZSC_MPCWM_ConfigMemAttributes(FMC_BASE, &MPCWM_Desc) != HAL_OK)
  {
    /* Initialization error */
    while(1);
  }

  /* re-enable fault interrupt occurrences after this SAU/MPCWM2 setup section */
  __enable_fault_irq();
}

/**
* @brief  Configure SAU and MPCWM2 in second push button Error Case
* @note   SAU is setup so that whole FMC SRAM memory is declared as non-secure
*         GTZC MPCWM2 is kept as the initial configuration with only the first
*         half NOR memory accessible from non-secure
* @retval None
*/
CMSE_NS_ENTRY void SECURE_SAU_MPCWM_SetIllegalAccessConfig(void)
{
  MPCWM_ConfigTypeDef MPCWM_Desc;
  /* protect this SAU/MPCWM2 setup section from any fault interrupt occurrence */
  __disable_fault_irq();

  /* Here the objective is to provoke a GTZC_IRQn occurrence.
  * We will do a non-secure access in the secure area, but before that, we need
  * to setup SAU, to have a whole non-secure area from its point of view.
  * By this way, SAU and MPCWM2 configuration will be misaligned, and the
  * access to the secure area will be allowed by SAU, but not by MPCWM2.
  * This will generate a GTZC_IRQn interrupt, which is our goal.
  */

  /* Whole non-secure area (SAU region 4) : 0x60000000-0x601FFFFF */
  SAU->RNR  = (4 & SAU_RNR_REGION_Msk);
  SAU->RBAR = (FMC_BASE & SAU_RBAR_BADDR_Msk);
  SAU->RLAR = (  ((FMC_BASE + 0xFFFFFUL) & SAU_RLAR_LADDR_Msk)
                | ((0 << SAU_RLAR_NSC_Pos)  & SAU_RLAR_NSC_Msk)
                | 1U);

  /* MPCWM2 configuration */
  MPCWM_Desc.AreaId = GTZC_TZSC_MPCWM_ID1;
  MPCWM_Desc.Offset = 0;
  MPCWM_Desc.Length = 16 * GTZC_TZSC_MPCWM_GRANULARITY_1;
  MPCWM_Desc.AreaStatus = ENABLE;
  MPCWM_Desc.Attribute  = GTZC_TZSC_MPCWM_REGION_SEC;
  MPCWM_Desc.Lock       = GTZC_TZSC_MPCWM_LOCK_OFF;

  if (HAL_GTZC_TZSC_MPCWM_ConfigMemAttributes(FMC_BASE, &MPCWM_Desc) != HAL_OK)
  {
    /* Initialization error */
    while(1);
  }
  /* re-enable fault interrupt occurrences after this SAU/MPCWM2 setup section */
  __enable_fault_irq();
}
/**
  * @}
  */

/**
  * @}
  */
/* USER CODE END Non_Secure_CallLib */

