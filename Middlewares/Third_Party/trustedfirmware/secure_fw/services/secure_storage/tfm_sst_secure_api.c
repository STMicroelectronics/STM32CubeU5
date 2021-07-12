/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "psa/protected_storage.h"
#include "tfm_veneers.h"
#ifdef TFM_PSA_API
#include "psa_manifest/sid.h"
#endif

#define IOVEC_LEN(x) (sizeof(x)/sizeof(x[0]))

__attribute__((section("SFN")))
psa_ps_status_t psa_ps_set(psa_ps_uid_t uid,
                           uint32_t data_length,
                           const void *p_data,
                           psa_ps_create_flags_t create_flags)
{
    psa_status_t status;
    psa_ps_status_t err;
#ifdef TFM_PSA_API
    psa_handle_t handle;
#endif

    psa_invec in_vec[] = {
        { .base = &uid,   .len = sizeof(uid) },
        { .base = p_data, .len = data_length },
        { .base = &create_flags, .len = sizeof(create_flags) }
    };

    psa_outvec out_vec[] = {
        { .base = &err , .len = sizeof(err) }
    };

#ifdef TFM_PSA_API
    handle = psa_connect(TFM_SST_SET_SID, TFM_SST_SET_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    status = psa_call(handle, PSA_IPC_CALL, in_vec, IOVEC_LEN(in_vec), out_vec,
                      IOVEC_LEN(out_vec));

    psa_close(handle);

    if (status != PSA_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }
#else
    status = tfm_tfm_sst_set_req_veneer(in_vec, IOVEC_LEN(in_vec),
                                        out_vec, IOVEC_LEN(out_vec));
    if (status != PSA_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }
#endif

    return err;
}

__attribute__((section("SFN")))
psa_ps_status_t psa_ps_get(psa_ps_uid_t uid,
                           uint32_t data_offset,
                           uint32_t data_length,
                           void *p_data)
{
    psa_status_t status;
    psa_ps_status_t err;
#ifdef TFM_PSA_API
    psa_handle_t handle;
#endif

    psa_invec in_vec[] = {
        { .base = &uid, .len = sizeof(uid) },
        { .base = &data_offset, .len = sizeof(data_offset) }
    };

    psa_outvec out_vec[] = {
        { .base = &err,   .len = sizeof(err) },
        { .base = p_data, .len = data_length }
    };

#ifdef TFM_PSA_API
    handle = psa_connect(TFM_SST_GET_SID, TFM_SST_GET_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    status = psa_call(handle, PSA_IPC_CALL, in_vec, IOVEC_LEN(in_vec), out_vec,
                      IOVEC_LEN(out_vec));

    psa_close(handle);

    if (status != PSA_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }
#else
    status = tfm_tfm_sst_get_req_veneer(in_vec, IOVEC_LEN(in_vec),
                                        out_vec, IOVEC_LEN(out_vec));
    if (status != PSA_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }
#endif

    return err;
}

__attribute__((section("SFN")))
psa_ps_status_t psa_ps_get_info(psa_ps_uid_t uid, struct psa_ps_info_t *p_info)
{
    psa_status_t status;
    psa_ps_status_t err;
#ifdef TFM_PSA_API
    psa_handle_t handle;
#endif

    psa_invec in_vec[] = {
        { .base = &uid, .len = sizeof(uid) }
    };

    psa_outvec out_vec[] = {
        { .base = &err,   .len = sizeof(err) },
        { .base = p_info, .len = sizeof(*p_info) }
    };

#ifdef TFM_PSA_API
    handle = psa_connect(TFM_SST_GET_INFO_SID, TFM_SST_GET_INFO_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    status = psa_call(handle, PSA_IPC_CALL, in_vec, IOVEC_LEN(in_vec), out_vec,
                      IOVEC_LEN(out_vec));

    psa_close(handle);

    if (status != PSA_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }
#else
    status = tfm_tfm_sst_get_info_req_veneer(in_vec, IOVEC_LEN(in_vec),
                                             out_vec, IOVEC_LEN(out_vec));

    if (status != PSA_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }
#endif

    return err;
}

__attribute__((section("SFN")))
psa_ps_status_t psa_ps_remove(psa_ps_uid_t uid)
{
    psa_status_t status;
    psa_ps_status_t err;
#ifdef TFM_PSA_API
    psa_handle_t handle;
#endif

    psa_invec in_vec[] = {
        { .base = &uid, .len = sizeof(uid) }
    };

    psa_outvec out_vec[] = {
        { .base = &err, .len = sizeof(err) }
    };

#ifdef TFM_PSA_API
    handle = psa_connect(TFM_SST_REMOVE_SID, TFM_SST_REMOVE_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }

    status = psa_call(handle, PSA_IPC_CALL, in_vec, IOVEC_LEN(in_vec), out_vec,
                      IOVEC_LEN(out_vec));

    psa_close(handle);

    if (status != PSA_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }
#else
    status = tfm_tfm_sst_remove_req_veneer(in_vec, IOVEC_LEN(in_vec),
                                           out_vec, IOVEC_LEN(out_vec));

    if (status != PSA_SUCCESS) {
        return PSA_PS_ERROR_OPERATION_FAILED;
    }
#endif

    return err;
}

__attribute__((section("SFN")))
psa_ps_status_t psa_ps_create(psa_ps_uid_t uid, uint32_t size,
                              psa_ps_create_flags_t create_flags)
{
    (void)uid;
    (void)size;
    (void)create_flags;

    return PSA_PS_ERROR_NOT_SUPPORTED;
}

__attribute__((section("SFN")))
psa_ps_status_t psa_ps_set_extended(psa_ps_uid_t uid, uint32_t data_offset,
                                    uint32_t data_length, const void *p_data)
{
    (void)uid;
    (void)data_offset;
    (void)data_length;
    (void)p_data;

    return PSA_PS_ERROR_NOT_SUPPORTED;
}

__attribute__((section("SFN")))
uint32_t psa_ps_get_support(void)
{
    /* Initialise support_flags to a sensible default, to avoid returning an
     * uninitialised value in case the secure function fails.
     */
    uint32_t support_flags = 0;
#ifdef TFM_PSA_API
    psa_handle_t handle;
#endif

    psa_outvec out_vec[] = {
        { .base = &support_flags, .len = sizeof(support_flags) }
    };

    /* The PSA API does not return an error, so any error from TF-M is
     * ignored.
     */
#ifdef TFM_PSA_API
    handle = psa_connect(TFM_SST_GET_SUPPORT_SID, TFM_SST_GET_SUPPORT_VERSION);
    if (!PSA_HANDLE_IS_VALID(handle)) {
        return support_flags;
    }

    (void)psa_call(handle, PSA_IPC_CALL, NULL, 0, out_vec, IOVEC_LEN(out_vec));

    psa_close(handle);
#else
    (void)tfm_tfm_sst_get_support_req_veneer(NULL, 0,
                                             out_vec, IOVEC_LEN(out_vec));
#endif

    return support_flags;
}
