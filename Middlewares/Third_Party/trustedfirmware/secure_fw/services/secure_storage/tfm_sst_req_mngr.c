/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_sst_req_mngr.h"

#include <stdbool.h>
#include <stdint.h>

#include "psa/protected_storage.h"
#include "tfm_secure_api.h"
#include "tfm_api.h"
#include "tfm_protected_storage.h"
#ifdef TFM_PSA_API
#include "psa/service.h"
#include "psa_manifest/tfm_secure_storage.h"
#include "flash_layout.h"
#endif

#ifndef TFM_PSA_API
#include "tfm_memory_utils.h"

static void *p_data;

/*
 * \brief Indicates whether SST has been initialised.
 */
static bool sst_is_init = false;

/*
 * \brief Initialises SST, if not already initialised.
 *
 * \note In library mode, initialisation is delayed until the first secure
 *       function call, as calls to the Crypto service are required for
 *       initialisation.
 *
 * \return PSA_SUCCESS if SST is initialised, PSA_ERROR_CONNECTION_REFUSED
 *         otherwise.
 */
static psa_status_t sst_check_init(void)
{
    if (!sst_is_init) {
        if (tfm_sst_init() != PSA_PS_SUCCESS) {
            return PSA_ERROR_CONNECTION_REFUSED;
        }
        sst_is_init = true;
    }

    return PSA_SUCCESS;
}

psa_status_t tfm_sst_set_req(psa_invec *in_vec, size_t in_len,
                             psa_outvec *out_vec, size_t out_len)
{
    psa_ps_uid_t uid;
    uint32_t data_length;
    int32_t client_id;
    int32_t status;
    psa_ps_create_flags_t create_flags;
    psa_ps_status_t *err;

    if (sst_check_init() != PSA_SUCCESS) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if ((in_len != 3) || (out_len != 1)) {
        /* The number of arguments are incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if (in_vec[0].len != sizeof(psa_ps_uid_t)) {
        /* The input argument size is incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    uid = *((psa_ps_uid_t *)in_vec[0].base);

    p_data = (void *)in_vec[1].base;
    data_length = in_vec[1].len;

    if (in_vec[2].len != sizeof(psa_ps_create_flags_t)) {
        /* The input argument size is incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    create_flags = *(psa_ps_create_flags_t *)in_vec[2].base;

    if (out_vec[0].len != sizeof(psa_ps_status_t)) {
        /* The output argument size is incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    err = (psa_ps_status_t *)out_vec[0].base;

    /* Get the caller's client ID */
    status = tfm_core_get_caller_client_id(&client_id);
    if (status != (int32_t)TFM_SUCCESS) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    *err = tfm_sst_set(client_id, uid, data_length, create_flags);

    return PSA_SUCCESS;
}

psa_status_t tfm_sst_get_req(psa_invec *in_vec, size_t in_len,
                             psa_outvec *out_vec, size_t out_len)
{
    uint32_t data_offset;
    uint32_t data_length;
    int32_t client_id;
    psa_ps_uid_t uid;
    int32_t status;
    psa_ps_status_t *err;

    if (sst_check_init() != PSA_SUCCESS) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if ((in_len != 2) || (out_len != 2)) {
        /* The number of arguments are incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if (in_vec[0].len != sizeof(psa_ps_uid_t)) {
        /* The input argument size is incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    uid = *((psa_ps_uid_t *)in_vec[0].base);

    if (in_vec[1].len != sizeof(data_offset)) {
        /* The input argument size is incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    data_offset = *(uint32_t *)in_vec[1].base;

    if (out_vec[0].len != sizeof(psa_ps_status_t)) {
        /* The output argument size is incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    err = (psa_ps_status_t *)out_vec[0].base;

    p_data = (void *)out_vec[1].base;
    data_length = out_vec[1].len;

    /* Get the caller's client ID */
    status = tfm_core_get_caller_client_id(&client_id);
    if (status != (int32_t)TFM_SUCCESS) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    *err = tfm_sst_get(client_id, uid, data_offset, data_length);

    return PSA_SUCCESS;

}

psa_status_t tfm_sst_get_info_req(psa_invec *in_vec, size_t in_len,
                                  psa_outvec *out_vec, size_t out_len)
{
    int32_t client_id;
    psa_ps_uid_t uid;
    struct psa_ps_info_t *p_info;
    int32_t status;
    psa_ps_status_t *err;

    if (sst_check_init() != PSA_SUCCESS) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if ((in_len != 1) || (out_len != 2)) {
        /* The number of arguments are incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if (in_vec[0].len != sizeof(psa_ps_uid_t)) {
        /* The input argument size is incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    uid = *((psa_ps_uid_t *)in_vec[0].base);

    if (out_vec[0].len != sizeof(psa_ps_status_t)) {
        /* The output argument size is incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    err = (psa_ps_status_t *)out_vec[0].base;

    if (out_vec[1].len != sizeof(struct psa_ps_info_t)) {
        /* The output argument size is incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    p_info = (struct psa_ps_info_t *)out_vec[1].base;

    /* Get the caller's client ID */
    status = tfm_core_get_caller_client_id(&client_id);
    if (status != (int32_t)TFM_SUCCESS) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    *err = tfm_sst_get_info(client_id, uid, p_info);

    return PSA_SUCCESS;
}

psa_status_t tfm_sst_remove_req(psa_invec *in_vec, size_t in_len,
                                psa_outvec *out_vec, size_t out_len)
{
    int32_t client_id;
    psa_ps_uid_t uid;
    int32_t status;
    psa_ps_status_t *err;

    if (sst_check_init() != PSA_SUCCESS) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if ((in_len != 1) || (out_len != 1)) {
        /* The number of arguments are incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if (in_vec[0].len != sizeof(psa_ps_uid_t)) {
        /* The input argument size is incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    uid = *((psa_ps_uid_t *)in_vec[0].base);

    if (out_vec[0].len != sizeof(psa_ps_status_t)) {
        /* The output argument size is incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    err = (psa_ps_status_t *)out_vec[0].base;

    /* Get the caller's client ID */
    status = tfm_core_get_caller_client_id(&client_id);
    if (status != (int32_t)TFM_SUCCESS) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    *err = tfm_sst_remove(client_id, uid);

    return PSA_SUCCESS;
}

psa_status_t tfm_sst_get_support_req(psa_invec *in_vec, size_t in_len,
                                     psa_outvec *out_vec, size_t out_len)
{
    uint32_t *support_flags;

    (void)in_vec;

    if (sst_check_init() != PSA_SUCCESS) {
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if ((in_len != 0) || (out_len != 1)) {
        /* The number of arguments are incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    if (out_vec[0].len != sizeof(*support_flags)) {
        /* The output argument size is incorrect */
        return PSA_ERROR_CONNECTION_REFUSED;
    }

    support_flags = (uint32_t *)out_vec[0].base;

    *support_flags = tfm_sst_get_support();

    return PSA_SUCCESS;
}

#else /* !defined(TFM_PSA_API) */
typedef psa_status_t (*sst_func_t)(void);
static psa_msg_t msg;

static psa_status_t tfm_sst_set_ipc(void)
{
    psa_ps_uid_t uid;
    int32_t client_id;
    psa_ps_create_flags_t create_flags;
    size_t in_size[3], out_size, num = 0;
    psa_ps_status_t err;

    client_id = msg.client_id;
    in_size[0] = msg.in_size[0];
    in_size[1] = msg.in_size[1];
    in_size[2] = msg.in_size[2];
    out_size = msg.out_size[0];
    if (in_size[0] != sizeof(psa_ps_uid_t) ||
        in_size[2] != sizeof(psa_ps_create_flags_t) ||
        out_size != sizeof(psa_ps_status_t)) {
        /* The size of one of the arguments is incorrect */
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    num = psa_read(msg.handle, 0, &uid, in_size[0]);
    if (num != in_size[0]) {
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    num = psa_read(msg.handle, 2, &create_flags, in_size[2]);
    if (num != in_size[2]) {
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    err = tfm_sst_set(client_id, uid, in_size[1], create_flags);
    psa_write(msg.handle, 0, &err, out_size);
    return PSA_SUCCESS;
}

static psa_status_t tfm_sst_get_ipc(void)
{
    psa_ps_uid_t uid;
    int32_t client_id;
    uint32_t data_offset;
    size_t in_size[2], out_size[2], num = 0;
    psa_ps_status_t err;

    client_id = msg.client_id;
    in_size[0] = msg.in_size[0];
    in_size[1] = msg.in_size[1];
    out_size[0] = msg.out_size[0];
    out_size[1] = msg.out_size[1];
    if (in_size[0] != sizeof(psa_ps_uid_t) ||
        in_size[1] != sizeof(uint32_t) ||
        out_size[0] != sizeof(psa_ps_status_t)) {
        /* The size of one of the arguments is incorrect */
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    num = psa_read(msg.handle, 0, &uid, in_size[0]);
    if (num != in_size[0]) {
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    num = psa_read(msg.handle, 1, &data_offset, in_size[1]);
    if (num != in_size[1]) {
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    err = tfm_sst_get(client_id, uid, data_offset, out_size[1]);
    psa_write(msg.handle, 0, &err, out_size[0]);
    return PSA_SUCCESS;
}

static psa_status_t tfm_sst_get_info_ipc(void)
{
    psa_ps_uid_t uid;
    int32_t client_id;
    struct psa_ps_info_t info;
    size_t in_size, out_size[2], num = 0;
    psa_ps_status_t err;

    client_id = msg.client_id;
    in_size = msg.in_size[0];
    out_size[0] = msg.out_size[0];
    out_size[1] = msg.out_size[1];
    if (in_size != sizeof(psa_ps_uid_t) ||
        out_size[0] != sizeof(psa_ps_status_t) ||
        out_size[1] != sizeof(struct psa_ps_info_t)) {
        /* The size of one of the arguments is incorrect */
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    num = psa_read(msg.handle, 0, &uid, in_size);
    if (num != in_size) {
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    err = tfm_sst_get_info(client_id, uid, &info);
    psa_write(msg.handle, 0, &err, out_size[0]);
    if (err == PSA_PS_SUCCESS) {
        psa_write(msg.handle, 1, &info, out_size[1]);
    }
    return PSA_SUCCESS;
}

static psa_status_t tfm_sst_remove_ipc(void)
{
    psa_ps_uid_t uid;
    int32_t client_id;
    size_t in_size, out_size, num = 0;
    psa_ps_status_t err;

    client_id = msg.client_id;
    in_size = msg.in_size[0];
    out_size = msg.out_size[0];
    if (in_size != sizeof(psa_ps_uid_t) ||
        out_size != sizeof(psa_ps_status_t)) {
        /* The size of one of the arguments is incorrect */
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    num = psa_read(msg.handle, 0, &uid, in_size);
    if (num != in_size) {
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    err = tfm_sst_remove(client_id, uid);
    psa_write(msg.handle, 0, &err, out_size);
    return PSA_SUCCESS;
}

static psa_status_t tfm_sst_get_support_ipc(void)
{
    size_t out_size;
    uint32_t support_flags;

    out_size = msg.out_size[0];
    if (out_size != sizeof(support_flags)) {
        /* The output argument size is incorrect */
        return PSA_PS_ERROR_INVALID_ARGUMENT;
    }

    support_flags = tfm_sst_get_support();
    psa_write(msg.handle, 0, &support_flags, out_size);
    return PSA_SUCCESS;
}

/*
 * Fixme: Temporarily implement abort as infinite loop,
 * will replace it later.
 */
static void tfm_abort(void)
{
    while (1)
        ;
}

static void ps_signal_handle(psa_signal_t signal, sst_func_t pfn)
{
    psa_status_t status;

    status = psa_get(signal, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        status = pfn();
        psa_reply(msg.handle, status);
        break;
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        tfm_abort();
    }
}
#endif /* !defined(TFM_PSA_API) */

psa_ps_status_t tfm_sst_req_mngr_init(void)
{
#ifdef TFM_PSA_API
    psa_signal_t signals = 0;

    if (tfm_sst_init() != PSA_PS_SUCCESS) {
        tfm_abort();
    }

    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & TFM_SST_SET_SIG) {
            ps_signal_handle(TFM_SST_SET_SIG, tfm_sst_set_ipc);
        } else if (signals & TFM_SST_GET_SIG) {
            ps_signal_handle(TFM_SST_GET_SIG, tfm_sst_get_ipc);
        } else if (signals & TFM_SST_GET_INFO_SIG) {
            ps_signal_handle(TFM_SST_GET_INFO_SIG, tfm_sst_get_info_ipc);
        } else if (signals & TFM_SST_REMOVE_SIG) {
            ps_signal_handle(TFM_SST_REMOVE_SIG, tfm_sst_remove_ipc);
        } else if (signals & TFM_SST_GET_SUPPORT_SIG) {
            ps_signal_handle(TFM_SST_GET_SUPPORT_SIG, tfm_sst_get_support_ipc);
        } else {
            tfm_abort();
        }
    }
    /* NOTREACHED */
#endif
    /* In library mode, initialisation is delayed until the first secure
     * function call, as calls to the Crypto service are required for
     * initialisation.
     */
    return PSA_PS_SUCCESS;
}

psa_ps_status_t sst_req_mngr_read_asset_data(uint8_t *out_data, uint32_t size)
{
#ifdef TFM_PSA_API
  size_t num = 0;
  num = psa_read(msg.handle, 1, out_data, size);
  if (num != size) {
      return PSA_PS_ERROR_INVALID_ARGUMENT;
  }
#else /* TFM_PSA_API */
  (void)tfm_memcpy(out_data, p_data, size);
#endif
  return PSA_PS_SUCCESS;
}

void sst_req_mngr_write_asset_data(const uint8_t *in_data,
                                   uint32_t size)
{
#ifdef TFM_PSA_API
  psa_write(msg.handle, 1, in_data, size);
#else /* TFM_PSA_API */
  (void)tfm_memcpy(p_data, in_data, size);
#endif
}
