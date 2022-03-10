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
     * Unprivileged data is dynamically set during Partition sheduling.
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
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

#define MPU_REGION_VENEERS              0
#define MPU_REGION_TFM_UNPRIV_CODE      1
#define MPU_REGION_NS_STACK             2
#define PARTITION_REGION_RO             3
#define PARTITION_REGION_RW_STACK       4
#ifdef TFM_SP_META_PTR_ENABLE
#define MPU_REGION_SP_META_PTR          7
#endif /* TFM_SP_META_PTR_ENABLE */

REGION_DECLARE(Load$$LR$$, LR_VENEER, $$Base);
REGION_DECLARE(Load$$LR$$, LR_VENEER, $$Limit);
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Base);
REGION_DECLARE(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit);
REGION_DECLARE(Image$$, TFM_APP_CODE_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_CODE_END, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_START, $$Base);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_END, $$Base);
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Base);
REGION_DECLARE(Image$$, ARM_LIB_STACK, $$ZI$$Limit);
#ifdef TFM_SP_META_PTR_ENABLE
REGION_DECLARE(Image$$, TFM_SP_META_PTR, $$RW$$Base);
REGION_DECLARE(Image$$, TFM_SP_META_PTR, $$RW$$Limit);
#endif /* TFM_SP_META_PTR_ENABLE */

const struct mpu_armv8m_region_cfg_t region_cfg[] = {
    /* Veneer region */
    {
        MPU_REGION_VENEERS,
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_VENEER, $$Base),
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_VENEER, $$Limit),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
    /* TFM Core unprivileged code region */
    {
        MPU_REGION_TFM_UNPRIV_CODE,
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_UNPRIV_CODE, $$RO$$Limit),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
    /* NSPM PSP */
    {
        MPU_REGION_NS_STACK,
        (uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Base),
        (uint32_t)&REGION_NAME(Image$$, ARM_LIB_STACK, $$ZI$$Limit),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
    /* RO region */
    {
        PARTITION_REGION_RO,
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_CODE_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_CODE_IDX,
        MPU_ARMV8M_XN_EXEC_OK,
        MPU_ARMV8M_AP_RO_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
    /* RW, ZI and stack as one region */
    {
        PARTITION_REGION_RW_STACK,
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_START, $$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    },
#ifdef TFM_SP_META_PTR_ENABLE
    /* TFM partition metadata pointer region */
    {
        MPU_REGION_SP_META_PTR,
        (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$RW$$Base),
        (uint32_t)&REGION_NAME(Image$$, TFM_SP_META_PTR, $$RW$$Limit),
        MPU_ARMV8M_MAIR_ATTR_DATA_IDX,
        MPU_ARMV8M_XN_EXEC_NEVER,
        MPU_ARMV8M_AP_RW_PRIV_UNPRIV,
        MPU_ARMV8M_SH_NONE
    }
#endif
};
#endif /* TFM_LVL == 3 */
#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */

enum tfm_hal_status_t tfm_hal_set_up_static_boundaries(void)
{
    /* Set up isolation boundaries between SPE and NSPE */
    gtzc_init_cfg();
    sau_and_idau_cfg();
    pinmux_init_cfg();

    /* Set up static isolation boundaries inside SPE */
#ifdef CONFIG_TFM_ENABLE_MEMORY_PROTECT
    int32_t i;
    struct mpu_armv8m_dev_t dev_mpu_s = { MPU_BASE };

    mpu_armv8m_clean(&dev_mpu_s);

#if TFM_LVL == 3
    uint32_t cnt;

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
#else /* TFM_LVL == 3 */
    for (i = 0; i < ARRAY_SIZE(region_cfg); i++) {
        if (mpu_armv8m_region_enable(&dev_mpu_s,
            (struct mpu_armv8m_region_cfg_t *)&region_cfg[i])
            != MPU_ARMV8M_OK) {
            return TFM_HAL_ERROR_GENERIC;
        }
    }
#endif /* TFM_LVL == 3 */

    /* Enable MPU */
    if (mpu_armv8m_enable(&dev_mpu_s,
                          PRIVILEGED_DEFAULT_ENABLE,
                          HARDFAULT_NMI_ENABLE) != MPU_ARMV8M_OK) {
        return TFM_HAL_ERROR_GENERIC;
    }
#endif /* CONFIG_TFM_ENABLE_MEMORY_PROTECT */

    return TFM_HAL_SUCCESS;
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
