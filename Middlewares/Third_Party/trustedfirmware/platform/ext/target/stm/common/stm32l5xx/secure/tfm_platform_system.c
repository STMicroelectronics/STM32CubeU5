/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */


#include "target_cfg.h"
#include "tfm_spm_hal.h"
#include "uart_stdout.h"
#include "tfm_platform_system.h"
#include "tfm_hal_platform.h"

void tfm_platform_hal_system_reset(void)
{
    /* Reset the system */
    NVIC_SystemReset();
}

enum tfm_plat_err_t
tfm_platform_hal_pin_service(const psa_invec  *in_vec,  uint32_t num_invec,
                             const psa_outvec *out_vec, uint32_t num_outvec)
{
     return TFM_PLAT_ERR_SYSTEM_ERR;
}


void tfm_spm_hal_system_reset(void)
{
    NVIC_SystemReset();
}

enum tfm_platform_err_t tfm_platform_hal_ioctl(tfm_platform_ioctl_req_t request,
                                               psa_invec  *in_vec,
                                               psa_outvec *out_vec)
{
    return TFM_PLATFORM_ERR_NOT_SUPPORTED;

}

void Error_Handler(void)
{
    /* Reset the system */
    NVIC_SystemReset();
}
