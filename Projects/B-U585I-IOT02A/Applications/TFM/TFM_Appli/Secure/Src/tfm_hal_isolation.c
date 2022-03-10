/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "cmsis.h"
#include "Driver_Common.h"
#include "mpu_armv8m_drv.h"
#include "region.h"
#include "target_cfg.h"
#include "tfm_hal_isolation.h"
#include "tfm_plat_defs.h"
#include "target_flowcontrol.h"
#include "region_defs.h"
#include "low_level_rng.h"
#include "fih.h"

extern void Error_Handler(void);

#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
#if TFM_LVL == 3
REGION_DECLARE(Load$$LR$$, LR_VENEER, $$Base);
REGION_DECLARE(Load$$LR$$, LR_VENEER, $$Limit);
REGION_DECLARE(Image$$, PT_RO_START, $$Base);
REGION_DECLARE(Image$$, PT_RO_END, $$Base);
REGION_DECLARE(Image$$, PT_PRIV_RWZI_START, $$Base);
REGION_DECLARE(Image$$, PT_PRIV_RWZI_END, $$Base);

static uint32_t g_static_region_cnt;

static struct mpu_armv8m_region_cfg_t isolation_regions[] = {
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_VENEER, $$Base),
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_VENEER, $$Limit),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE,
    },
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, PT_RO_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, PT_RO_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE,
    },
    /* For isolation Level 3, set up static isolation for privileged data.
     * Unprivileged data is dynamically set during Partition scheduling.
     */
    {
        0, /* will be updated before using */
        (uint32_t)&REGION_NAME(Image$$, PT_PRIV_RWZI_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, PT_PRIV_RWZI_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_ONLY,
        MPU_ARMV8M_SH_NONE,
    },
};
#else /* TFM_LVL == 3 */


REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit);
/* End of privileged Region +1*/
REGION_DECLARE(Image$$, TFM_PSA_RW_STACK_END, $$Base);
REGION_DECLARE(Load$$LR$$, LR_VENEER, $$Limit);

const struct mpu_armv8m_region_cfg_t region_cfg_init_s[] = {
            /* region 0: RO/PRIV_ONLY/EXEC_NEVER from NS flash base to NS appli (except appli validation bytes) */
           {
               0,
               FLASH_BASE_NS,
               FLASH_BASE_NS + NS_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_NS_PARTITION_SIZE - 1 - (~MPU_RLAR_LIMIT_Msk +1),
               MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RO_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R0,
               FLOW_CTRL_MPU_S_EN_R0,
               FLOW_STEP_MPU_S_CH_R0,
               FLOW_CTRL_MPU_S_CH_R0,
#endif /* FLOW_CONTROL */
           },
           /* region 1: RW/PRIV_ONLY/EXEC_NEVER from NS validation appli to S Appli offset */
           {
               1,
               FLASH_BASE_NS + NS_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_NS_PARTITION_SIZE - (~MPU_RLAR_LIMIT_Msk +1),
               FLASH_BASE_S + S_IMAGE_PRIMARY_PARTITION_OFFSET - 1,
               MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R1,
               FLOW_CTRL_MPU_S_EN_R1,
               FLOW_STEP_MPU_S_CH_R1,
               FLOW_CTRL_MPU_S_CH_R1,
#endif /* FLOW_CONTROL */
           },
           /* region 2: RO/PRIV_ONLY/EXEC_OK from S Appli offset to S Appli unpriv code start */
           {
               2,
               FLASH_BASE_S + S_IMAGE_PRIMARY_PARTITION_OFFSET,
               (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Base) - 1,
               MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
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
           /* region 3: RO/PRIV_UNPRIV/EXEC_OK from S Appli unpriv code start to VENEER code end */
           {
               3,
               (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Base),
               (uint32_t)&REGION_NAME(Load$$LR$$, LR_VENEER, $$Limit) - 1,
               MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
               MPU_ARMV8M_XN_EXEC_OK,
               MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R3,
               FLOW_CTRL_MPU_S_EN_R3,
               FLOW_STEP_MPU_S_CH_R3,
               FLOW_CTRL_MPU_S_CH_R3,
#endif /* FLOW_CONTROL */
           },
           /* region 3: RW/PRIV_ONLY/EXEC_NEVER from S Appli Validation bytes to S data start (SRAM2) */
           {
               4,
               FLASH_BASE_S + S_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_PARTITION_SIZE - (~MPU_RLAR_LIMIT_Msk +1),
               S_DATA_START - 1,
               MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_ONLY,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R4,
               FLOW_CTRL_MPU_S_EN_R4,
               FLOW_STEP_MPU_S_CH_R4,
               FLOW_CTRL_MPU_S_CH_R4,
#endif /* FLOW_CONTROL */
           },
           /* region 4: RW/PRIV_ONLY/EXEC_NEVER for SRAM2 */
           {
               5,
               SRAM2_BASE_S,
               (uint32_t)&REGION_NAME(Image$$, TFM_PSA_RW_STACK_END, $$Base) -1,
               MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
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
           /* region 6: RW/PRIV_UNPRIV/EXEC_NEVER for SRAM2 */
           {
                6,
               (uint32_t)&REGION_NAME(Image$$, TFM_PSA_RW_STACK_END, $$Base),
               SRAM3_BASE_S + SRAM3_S_SIZE -1,
               MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
	             MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R6,
               FLOW_CTRL_MPU_S_EN_R6,
               FLOW_STEP_MPU_S_CH_R6,
               FLOW_CTRL_MPU_S_CH_R6,
#endif /* FLOW_CONTROL */
           },
           /* region 7: RW/PRIV_UNPRIV/EXEC_NEVER for S and NS Periph */
           {
               7,
               PERIPH_BASE_NS,
               PERIPH_BASE_S+0x0fffffff,
               MPU_ARMV8M_MAIR_ATTR_DATANOCACHE_IDX,
               MPU_ARMV8M_XN_EXEC_NEVER,
               MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
               MPU_ARMV8M_SH_NONE,
#ifdef FLOW_CONTROL
               FLOW_STEP_MPU_S_EN_R7,
               FLOW_CTRL_MPU_S_EN_R7,
               FLOW_STEP_MPU_S_CH_R7,
               FLOW_CTRL_MPU_S_CH_R7,
#endif /* FLOW_CONTROL */
           },
};
#ifdef TFM_FIH_PROFILE_ON
static fih_int mpu_init(void)
#else
static enum tfm_hal_status_t mpu_init(void)
#endif
{
  struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };
  uint32_t i;

  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* FIXME disable secure MPU to be able to re config it for the appli */
    mpu_armv8m_disable(&dev_mpu_s);
    mpu_armv8m_clean(&dev_mpu_s);

    /* configure secure MPU regions */
    for (i = 0; i < ARRAY_SIZE(region_cfg_init_s); i++)
    {
      if (mpu_armv8m_region_enable(&dev_mpu_s,
        (struct mpu_armv8m_region_cfg_t *)&region_cfg_init_s[i]) != MPU_ARMV8M_OK)
      {
#ifdef TFM_FIH_PROFILE_ON
	FIH_RET(fih_int_encode(TFM_HAL_ERROR_GENERIC));
#else
	return TFM_HAL_ERROR_GENERIC;
#endif
      }
      else
      {
        /* Execution stopped if flow control failed */
        FLOW_CONTROL_STEP(uFlowProtectValue, region_cfg_init_s[i].flow_step_enable,
                                             region_cfg_init_s[i].flow_ctrl_enable);
      }
    }

    /* enable secure MPU */
    mpu_armv8m_enable(&dev_mpu_s, PRIVILEGED_DEFAULT_DISABLE, HARDFAULT_NMI_ENABLE);
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
    if (mpu_armv8m_check(&dev_mpu_s, PRIVILEGED_DEFAULT_DISABLE,
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
    __IO uint32_t read_reg = (uint32_t) &SYSCFG->CSLCKR;
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    /* In TFM_DEV_MODE, MPU is not locked, this allows to see MPU configuration */
#ifndef TFM_DEV_MODE
    SYSCFG->CSLCKR |= SYSCFG_CSLCKR_LOCKSMPU;
#endif
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_S_LCK, FLOW_CTRL_MPU_S_LCK);
#ifndef TFM_DEV_MODE
    if (!((*(uint32_t *)read_reg) & SYSCFG_CSLCKR_LOCKSMPU))
        Error_Handler();
#endif
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_S_LCK_CH, FLOW_CTRL_MPU_S_LCK_CH);
  }
#ifdef TFM_FIH_PROFILE_ON
  FIH_RET(fih_int_encode(TFM_HAL_SUCCESS));
#else
   return TFM_HAL_SUCCESS;
#endif
}
#endif
#endif
#if defined(FLOW_CONTROL)
/* Global variable for Flow Control state */
volatile uint32_t uFlowProtectValue = FLOW_CTRL_INIT_VALUE;
#endif /* FLOW_CONTROL */
volatile uint32_t uFlowStage = FLOW_STAGE_CFG;


#ifdef TFM_FIH_PROFILE_ON
fih_int tfm_hal_set_up_static_boundaries(void)
#else
enum tfm_hal_status_t tfm_hal_set_up_static_boundaries(void)
#endif
{
#ifdef TFM_FIH_PROFILE_ON
  fih_int fih_rc = fih_int_encode(TFM_HAL_ERROR_GENERIC);
#endif
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
#ifdef TFM_FIH_PROFILE_ON
  FIH_CALL(gtzc_init_cfg, fih_rc);
  if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
    Error_Handler();
  }
#else
  gtzc_init_cfg();
#endif

#ifdef TFM_FIH_PROFILE_ON
  FIH_CALL(sau_and_idau_cfg, fih_rc);
  if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
    Error_Handler();
  }
#else
  sau_and_idau_cfg();
#endif

#ifdef TFM_FIH_PROFILE_ON
  FIH_CALL(pinmux_init_cfg, fih_rc);
  if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
    Error_Handler();
  }
#else
  pinmux_init_cfg();
#endif


  /* Check configurations with Flow control to resist to basic HW attacks */
  uFlowStage = FLOW_STAGE_CHK;
  #ifdef TFM_FIH_PROFILE_ON
  FIH_CALL(gtzc_init_cfg, fih_rc);
  if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
    Error_Handler();
  }
#else
  gtzc_init_cfg();
#endif

#ifdef TFM_FIH_PROFILE_ON
  FIH_CALL(sau_and_idau_cfg, fih_rc);
  if (fih_not_eq(fih_rc, fih_int_encode(TFM_HAL_SUCCESS))) {
    Error_Handler();
  }
#else
  sau_and_idau_cfg();
#endif

#ifdef TFM_FIH_PROFILE_ON
  /* RNG_Init() was already done in tfm_hal_random_init() */
#else
  /* Start HW randomization */
  RNG_Init();
#endif

    /* Set up static isolation boundaries inside SPE */
#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
 

#if TFM_LVL == 3
    uint32_t cnt;
    int32_t i;
    struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };

    mpu_armv8m_clean(&dev_mpu_s);

    /* Update MPU region numbers. The numbers start from 0 and are continuous */
    cnt = sizeof(isolation_regions) / sizeof(isolation_regions[0]);
    g_static_region_cnt = cnt;
    for (i = 0; i < cnt; i++) {
        /* Update region number */
        isolation_regions[i].region_nr = i;
        /* Enable regions */
        if (mpu_armv8m_region_enable(&dev_mpu_s, &isolation_regions[i])
                                                             != MPU_ARMV8M_OK) {
            return TFM_HAL_ERROR_GENERIC;
        }
    }
        /* Enable MPU */
    if (mpu_armv8m_enable(&dev_mpu_s,
                          PRIVILEGED_DEFAULT_DISABLE,
                          HARDFAULT_NMI_ENABLE) != MPU_ARMV8M_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }

#else /* TFM_LVL == 3 */
    uFlowStage = FLOW_STAGE_CFG;
#ifdef TFM_FIH_PROFILE_ON
    FIH_CALL(mpu_init, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_PLAT_ERR_SUCCESS))) {
      Error_Handler();
    }
#else
    mpu_init();
#endif

    uFlowStage = FLOW_STAGE_CHK;
#ifdef TFM_FIH_PROFILE_ON
    FIH_CALL(mpu_init, fih_rc);
    if (fih_not_eq(fih_rc, fih_int_encode(TFM_PLAT_ERR_SUCCESS))) {
      Error_Handler();
    }
#else
    mpu_init();
#endif


#endif /* TFM_LVL == 3 */

#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */

#ifdef TFM_FIH_PROFILE_ON
    fih_rc = fih_int_encode(TFM_HAL_SUCCESS);
    FIH_RET(fih_rc);
#else
    return TFM_HAL_SUCCESS;
#endif
}

#if TFM_LVL == 3
enum tfm_hal_status_t tfm_hal_mpu_update_partition_boundary(uintptr_t start,
                                                            uintptr_t end)
{
    struct mpu_armv8m_region_cfg_t cfg;
    enum mpu_armv8m_error_t mpu_err;
    struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };

    /* Partition boundary regions is right after static regions */
    cfg.region_nr = g_static_region_cnt;
    cfg.region_base = start;
    cfg.region_limit = end;
    cfg.region_attridx = MPU_ARMV8M_MAIR_ATTR_DATA_IDX;
    cfg.attr_access = MPU_ARMV8M_AP_RW_PRIV_UNPRIV;
    cfg.attr_exec = MPU_ARMV8M_XN_EXEC_NEVER;
    cfg.attr_sh = MPU_ARMV8M_SH_NONE;
    mpu_err = mpu_armv8m_region_enable(&dev_mpu_s, &cfg);
    if (mpu_err != MPU_ARMV8M_OK) {
      return TFM_HAL_ERROR_GENERIC;
    }
    return TFM_HAL_SUCCESS;
}
#endif /* TFM_LVL == 3 */