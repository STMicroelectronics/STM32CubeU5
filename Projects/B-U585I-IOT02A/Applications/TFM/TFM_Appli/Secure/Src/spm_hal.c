/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */


#include <stdio.h>
#include "platform/include/region.h"
#include "platform/include/tfm_spm_hal.h"
#include "spm_api.h"
#include "spm_db.h"
#include "tfm_platform_core_api.h"
#include "target_cfg.h"
#include "target_flowcontrol.h"
#include "Driver_MPC.h"
#include "mpu_armv8m_drv.h"
#include "region_defs.h"
#include "secure_utilities.h"
#include "platform_irq.h"
#include "low_level_rng.h"

extern void Error_Handler(void);

/* Get address of memory regions to configure MPU */
extern const struct memory_region_limits memory_regions;

struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };
struct mpu_armv8m_dev_t dev_mpu_ns = { MPU_BASE_NS };

#if defined(FLOW_CONTROL)
/* Global variable for Flow Control state */
volatile uint32_t uFlowProtectValue = FLOW_CTRL_INIT_VALUE;
#endif /* FLOW_CONTROL */
volatile uint32_t uFlowStage = FLOW_STAGE_CFG;

enum tfm_plat_err_t tfm_spm_hal_init_isolation_hw(void)
{
  /* STM32U5xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
             can eventually implement his proper time base source (a general purpose
             timer for example or other time source), keeping in mind that Time base
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
             handled in milliseconds basis.
       - Set NVIC Group Priority to 3
       - Low Level Initialization
     */
  HAL_Init();

  /* Configures non-secure memory spaces in the target */
  uFlowStage = FLOW_STAGE_CFG;
  gtzc_init_cfg();
  sau_and_idau_cfg();
  pinmux_init_cfg();

  /* Check configurations with Flow control to resist to basic HW attacks */
  uFlowStage = FLOW_STAGE_CHK;
  gtzc_init_cfg();
  sau_and_idau_cfg();

  /* Start HW randomization */
  RNG_Init();

  return TFM_PLAT_ERR_SUCCESS;
}

void tfm_spm_hal_configure_default_isolation(
  const struct tfm_spm_partition_platform_data_t *platform_data)
{
  if (platform_data)
  {
    /* fix me : not supported*/
    while (1);
  }

}

#define MPU_REGION_TFM_UNPRIV_CODE   0
#define MPU_REGION_TFM_UNPRIV_DATA   1
#define MPU_REGION_TFM_PRIV_CODE     2
#define MPU_REGION_TFM_PRIV_DATA     3

#define PARTITION_REGION_PERIPH      6
#define PARTITION_REGION_NV_DATA     7

#define MPU_NONSECURE_ALIAS_FLASH    4
#define MPU_NONSECURE_ALIAS_RAM      5

REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_UNPRIV_DATA, $$RW$$Base);
REGION_DECLARE(Image$$, TFM_UNPRIV_DATA, $$ZI$$Limit);

REGION_DECLARE(Image$$, TFM_APP_CODE_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_CODE_END, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_END, $$Base);
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Base);
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Limit);

REGION_DECLARE(Load$$LR$$, LR_VENEER, $$Limit);

const struct mpu_armv8m_region_cfg_t region_cfg_init_s[] = {
           /* TFM Core unprivileged code region */
           {
               MPU_REGION_TFM_UNPRIV_CODE,
               (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Base),
               (uint32_t)&REGION_NAME(Load$$LR$$, LR_VENEER, $$Limit) - 1,
               MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
               MPU_ARMV8M_XN_EXEC_OK,
               MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R0,
               FLOW_CTRL_MPU_S_EN_R0,
               FLOW_STEP_MPU_S_CH_R0,
               FLOW_CTRL_MPU_S_CH_R0,
#endif /* FLOW_CONTROL */
           },
           /* TFM Core unprivileged data region */
           {
               MPU_REGION_TFM_UNPRIV_DATA,
               (uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Base),
               (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base) - 1,
               MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R1,
               FLOW_CTRL_MPU_S_EN_R1,
               FLOW_STEP_MPU_S_CH_R1,
               FLOW_CTRL_MPU_S_CH_R1,
#endif /* FLOW_CONTROL */
           },
           /* TFM_Core privileged code region   */
           {
               MPU_REGION_TFM_PRIV_CODE,
               S_CODE_START,
               (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Base) - 1,
               MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
               MPU_ARMV8M_XN_EXEC_OK,
               MPU_ARMV8M_AP_RO_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R2,
               FLOW_CTRL_MPU_S_EN_R2,
               FLOW_STEP_MPU_S_CH_R2,
               FLOW_CTRL_MPU_S_CH_R2,
#endif /* FLOW_CONTROL */
           },
           /* TFM_Core privileged data region   */
           {
               MPU_REGION_TFM_PRIV_DATA,
               (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base),
               S_DATA_LIMIT,
               MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R3,
               FLOW_CTRL_MPU_S_EN_R3,
               FLOW_STEP_MPU_S_CH_R3,
               FLOW_CTRL_MPU_S_CH_R3,
#endif /* FLOW_CONTROL */
           },
           /* peripheral for AppROT partition */
           {
               PARTITION_REGION_PERIPH,
               PERIPH_BASE_S,
               PERIPH_BASE_S+0x0fffffff,
               MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R4,
               FLOW_CTRL_MPU_S_EN_R4,
               FLOW_STEP_MPU_S_CH_R4,
               FLOW_CTRL_MPU_S_CH_R4,
#endif /* FLOW_CONTROL */
           },
           /* TFM Non volatile data region (NVCNT/SST/ITS) */
           {
               PARTITION_REGION_NV_DATA,
               TFM_NV_DATA_START,
               TFM_NV_DATA_LIMIT,
               MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R5,
               FLOW_CTRL_MPU_S_EN_R5,
               FLOW_STEP_MPU_S_CH_R5,
               FLOW_CTRL_MPU_S_CH_R5,
#endif /* FLOW_CONTROL */
           },
           /* RAM Non Secure Alias Not Executable */
           {
               MPU_NONSECURE_ALIAS_RAM,
               SRAM1_BASE_NS,
               SRAM4_BASE_NS + SRAM4_SIZE -1,
               MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R6,
               FLOW_CTRL_MPU_S_EN_R6,
               FLOW_STEP_MPU_S_CH_R6,
               FLOW_CTRL_MPU_S_CH_R6,
#endif /* FLOW_CONTROL */
           },
           /* FLASH Non Secure Alias Not Executable, read only */
           {
               MPU_NONSECURE_ALIAS_FLASH,
               FLASH_BASE_NS,
               FLASH_BASE_NS+FLASH_TOTAL_SIZE-1,
               MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RO_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R7,
               FLOW_CTRL_MPU_S_EN_R7,
               FLOW_STEP_MPU_S_CH_R7,
               FLOW_CTRL_MPU_S_CH_R7,
#endif /* FLOW_CONTROL */
           },
};


static enum spm_err_t tfm_spm_mpu_init(void)
{
  uint32_t i;

  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    mpu_armv8m_clean(&dev_mpu_s);

    /* configure secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_init_s); i++)
    {
      if (mpu_armv8m_region_enable(&dev_mpu_s,
        (struct mpu_armv8m_region_cfg_t *)&region_cfg_init_s[i]) != MPU_ARMV8M_OK)
      {
        return SPM_ERR_INVALID_CONFIG;
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_init_s[i].flow_step_enable,
                                             region_cfg_init_s[i].flow_ctrl_enable);
      }
    }

    /* enable secure MPU */
    mpu_armv8m_enable(&dev_mpu_s, PRIVILEGED_DEFAULT_ENABLE, HARDFAULT_NMI_ENABLE);
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_S_EN, FLOW_CTRL_MPU_S_EN);
  }
  /* verification stage */
  else
  {
    /* check secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_init_s); i++)
    {
      if (mpu_armv8m_region_enable_check(&dev_mpu_s,
        (struct mpu_armv8m_region_cfg_t *)&region_cfg_init_s[i]) != MPU_ARMV8M_OK)
      {
        Error_Handler();
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_init_s[i].flow_step_check,
                                             region_cfg_init_s[i].flow_ctrl_check);
      }
    }

    /* check secure MPU */
    if (mpu_armv8m_check(&dev_mpu_s, PRIVILEGED_DEFAULT_ENABLE,
                      HARDFAULT_NMI_ENABLE) != MPU_ARMV8M_OK)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_S_CH, FLOW_CTRL_MPU_S_CH);
    }

    /* Lock MPU config */
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    SYSCFG->CSLCKR |= SYSCFG_CSLCKR_LOCKSMPU;
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_S_LCK, FLOW_CTRL_MPU_S_LCK);
    if (!(SYSCFG->CSLCKR & SYSCFG_CSLCKR_LOCKSMPU))
        Error_Handler();
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_S_LCK_CH, FLOW_CTRL_MPU_S_LCK_CH);
  }

  return SPM_ERR_OK;
}

enum tfm_plat_err_t tfm_spm_hal_setup_isolation_hw(void)
{
  uFlowStage = FLOW_STAGE_CFG;
  if (tfm_spm_mpu_init() != SPM_ERR_OK)
  {
    ERROR_MSG("Failed to set up initial MPU configuration! Halting.");
    while (1)
    {
      ;
    }
  }

  /* Check configurations with Flow control to resist to basic HW attacks */
  uFlowStage = FLOW_STAGE_CHK;
  if (tfm_spm_mpu_init() != SPM_ERR_OK)
  {
    ERROR_MSG("Failed to check initial MPU configuration! Halting.");
    while (1)
    {
      ;
    }
  }
  return TFM_PLAT_ERR_SUCCESS;
}

uint32_t tfm_spm_hal_get_ns_VTOR(void)
{
  return memory_regions.non_secure_code_start;
}

uint32_t tfm_spm_hal_get_ns_MSP(void)
{
  return *((uint32_t *)memory_regions.non_secure_code_start);
}

uint32_t tfm_spm_hal_get_ns_entry_point(void)
{
  return *((uint32_t *)(memory_regions.non_secure_code_start + 4));
}

void tfm_spm_hal_set_ns_mpu(void)
{

}

enum tfm_plat_err_t tfm_spm_hal_set_secure_irq_priority(IRQn_Type irq_line,
                                                        uint32_t priority)
{

  HAL_NVIC_SetPriority(irq_line, priority , 0); /* Highest priority level */
  return TFM_PLAT_ERR_SUCCESS;
}

void tfm_spm_hal_clear_pending_irq(IRQn_Type irq_line)
{
  NVIC_ClearPendingIRQ(irq_line);
}

void tfm_spm_hal_enable_irq(IRQn_Type irq_line)
{
  NVIC_EnableIRQ(irq_line);
}

void tfm_spm_hal_disable_irq(IRQn_Type irq_line)
{
  NVIC_DisableIRQ(irq_line);
}

enum irq_target_state_t tfm_spm_hal_set_irq_target_state(
  IRQn_Type irq_line,
  enum irq_target_state_t target_state)
{
  uint32_t result;

  if (target_state == TFM_IRQ_TARGET_STATE_SECURE)
  {
    result = NVIC_ClearTargetState(irq_line);
  }
  else
  {
    result = NVIC_SetTargetState(irq_line);
  }

  if (result)
  {
    return TFM_IRQ_TARGET_STATE_NON_SECURE;
  }
  else
  {
    return TFM_IRQ_TARGET_STATE_SECURE;
  }
}

enum tfm_plat_err_t tfm_spm_hal_enable_fault_handlers(void)
{
    return enable_fault_handlers();
}

enum tfm_plat_err_t tfm_spm_hal_system_reset_cfg(void)
{
  return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_spm_hal_nvic_interrupt_target_state_cfg(void)
{
  return nvic_interrupt_target_state_cfg();
}

enum tfm_plat_err_t tfm_spm_hal_nvic_interrupt_enable(void)
{
  return nvic_interrupt_enable();
}
