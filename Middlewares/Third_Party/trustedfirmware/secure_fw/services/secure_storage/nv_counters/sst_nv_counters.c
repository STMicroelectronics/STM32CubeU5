/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "sst_nv_counters.h"

psa_ps_status_t sst_init_nv_counter(void)
{
    enum tfm_plat_err_t err;

    err = tfm_plat_init_nv_counter();
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    return PSA_PS_SUCCESS;
}

psa_ps_status_t sst_read_nv_counter(enum tfm_nv_counter_t counter_id,
                                       uint32_t *val)
{
    enum tfm_plat_err_t err;

    err = tfm_plat_read_nv_counter(counter_id, SST_NV_COUNTER_SIZE,
                                   (uint8_t *)val);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    return PSA_PS_SUCCESS;
}

psa_ps_status_t sst_increment_nv_counter(enum tfm_nv_counter_t counter_id)
{
    enum tfm_plat_err_t err;

    /* NOTE: tfm_plat_increment_nv_counter returns TFM_PLAT_ERR_MAX_VALUE when
     *       the counter reaches its maximum value. The current SST
     *       implementation treats this condition as an error as, from that
     *       moment onwards, the rollback protection can not be achieved based
     *       on the NV counters.
     */
    err = tfm_plat_increment_nv_counter(counter_id);
    if (err != TFM_PLAT_ERR_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    return PSA_PS_SUCCESS;
}
