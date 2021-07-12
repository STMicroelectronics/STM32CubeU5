/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_its_req_mngr.h"

#include <stdint.h>

#include "psa/storage_common.h"
#include "tfm_internal_trusted_storage.h"
#include "its_utils.h"
#include "flash_layout.h"

#ifdef TFM_PSA_API
#include "psa/service.h"
#include "psa_manifest/tfm_internal_trusted_storage.h"
#else
#include <stdbool.h>
#include "tfm_secure_api.h"
#include "tfm_memory_utils.h"
#include "tfm_api.h"
#endif

static uint8_t asset_data[GET_ALIGNED_FLASH_BYTES(ITS_MAX_ASSET_SIZE)] = {0};

#ifndef TFM_PSA_API
/**
 * \brief Indicates whether ITS has been initialised.
 */
static bool its_is_init = false;

psa_status_t tfm_its_set_req(psa_invec *in_vec, size_t in_len,
                             psa_outvec *out_vec, size_t out_len)
{
    psa_storage_uid_t uid;
    size_t data_length;
    const void *p_data;
    psa_storage_create_flags_t create_flags;
    int32_t client_id;

    (void)out_vec;

    if (!its_is_init) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if ((in_len != 3) || (out_len != 0)) {
        /* The number of arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    if (in_vec[0].len != sizeof(uid) ||
        in_vec[2].len != sizeof(create_flags)) {
        /* The size of one of the arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    uid = *((psa_storage_uid_t *)in_vec[0].base);

    p_data = in_vec[1].base;
    data_length = in_vec[1].len;

    /* Boundary check the incoming request */
    if (data_length > sizeof(asset_data)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }
    tfm_memcpy(asset_data, p_data, data_length);

    create_flags = *(psa_storage_create_flags_t *)in_vec[2].base;

    /* Get the caller's client ID */
    if (tfm_core_get_caller_client_id(&client_id) != (int32_t)TFM_SUCCESS) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    return tfm_its_set(client_id, uid, data_length, asset_data, create_flags);
}

psa_status_t tfm_its_get_req(psa_invec *in_vec, size_t in_len,
                             psa_outvec *out_vec, size_t out_len)
{
    psa_status_t status;
    psa_storage_uid_t uid;
    size_t data_offset;
    size_t data_size;
    void *p_data;
    size_t *p_data_length;
    int32_t client_id;

    if (!its_is_init) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if ((in_len != 2) || (out_len != 1)) {
        /* The number of arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    if (in_vec[0].len != sizeof(uid) ||
        in_vec[1].len != sizeof(data_offset)) {
        /* The size of one of the arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    uid = *((psa_storage_uid_t *)in_vec[0].base);

    data_offset = *(size_t *)in_vec[1].base;

    p_data = out_vec[0].base;
    data_size = out_vec[0].len;

    p_data_length = &out_vec[0].len;

    /* Get the caller's client ID */
    if (tfm_core_get_caller_client_id(&client_id) != (int32_t)TFM_SUCCESS) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    status =  tfm_its_get(client_id, uid, data_offset, data_size, asset_data,
                          p_data_length);
    if (status == PSA_SUCCESS) {
        tfm_memcpy(p_data, asset_data, *p_data_length);
    }

    return status;
}

psa_status_t tfm_its_get_info_req(psa_invec *in_vec, size_t in_len,
                                  psa_outvec *out_vec, size_t out_len)
{
    psa_storage_uid_t uid;
    struct psa_storage_info_t *p_info;
    int32_t client_id;

    if (!its_is_init) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if ((in_len != 1) || (out_len != 1)) {
        /* The number of arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    if (in_vec[0].len != sizeof(uid) ||
        out_vec[0].len != sizeof(*p_info)) {
        /* The size of one of the arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    uid = *((psa_storage_uid_t *)in_vec[0].base);

    p_info = (struct psa_storage_info_t *)out_vec[0].base;

    /* Get the caller's client ID */
    if (tfm_core_get_caller_client_id(&client_id) != (int32_t)TFM_SUCCESS) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    return tfm_its_get_info(client_id, uid, p_info);
}

psa_status_t tfm_its_remove_req(psa_invec *in_vec, size_t in_len,
                                psa_outvec *out_vec, size_t out_len)
{
    psa_storage_uid_t uid;
    int32_t client_id;

    (void)out_vec;

    if (!its_is_init) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if ((in_len != 1) || (out_len != 0)) {
        /* The number of arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    if (in_vec[0].len != sizeof(uid)) {
        /* The input argument size is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    uid = *((psa_storage_uid_t *)in_vec[0].base);

    /* Get the caller's client ID */
    if (tfm_core_get_caller_client_id(&client_id) != (int32_t)TFM_SUCCESS) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    return tfm_its_remove(client_id, uid);
}

#else /* !defined(TFM_PSA_API) */
typedef psa_status_t (*its_func_t)(const psa_msg_t *msg);

static psa_status_t tfm_its_set_ipc(const psa_msg_t *msg)
{
    psa_storage_uid_t uid;
    size_t data_length;
    psa_storage_create_flags_t create_flags;
    size_t num;

    if (msg->in_size[0] != sizeof(uid) ||
        msg->in_size[2] != sizeof(create_flags)) {
        /* The size of one of the arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    data_length = msg->in_size[1];
    if (data_length > sizeof(asset_data)) {
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    num = psa_read(msg->handle, 0, &uid, sizeof(uid));
    if (num != sizeof(uid)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 1, &asset_data, data_length);
    if (num != data_length) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 2, &create_flags, sizeof(create_flags));
    if (num != sizeof(create_flags)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    return tfm_its_set(msg->client_id, uid, data_length, asset_data,
                       create_flags);
}

static psa_status_t tfm_its_get_ipc(const psa_msg_t *msg)
{
    psa_status_t status;
    psa_storage_uid_t uid;
    size_t data_offset;
    size_t data_size;
    size_t data_length;
    size_t num;

    if (msg->in_size[0] != sizeof(uid) ||
        msg->in_size[1] != sizeof(data_offset)) {
        /* The size of one of the arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 0, &uid, sizeof(uid));
    if (num != sizeof(uid)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 1, &data_offset, sizeof(data_offset));
    if (num != sizeof(data_offset)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    data_size = msg->out_size[0];

    status = tfm_its_get(msg->client_id, uid, data_offset, data_size,
                         asset_data, &data_length);
    if (status == PSA_SUCCESS) {
        psa_write(msg->handle, 0, asset_data, data_length);
    }

    return status;
}

static psa_status_t tfm_its_get_info_ipc(const psa_msg_t *msg)
{
    psa_status_t status;
    psa_storage_uid_t uid;
    struct psa_storage_info_t info;
    size_t num;

    if (msg->in_size[0] != sizeof(uid) ||
        msg->out_size[0] != sizeof(info)) {
        /* The size of one of the arguments is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 0, &uid, sizeof(uid));
    if (num != sizeof(uid)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    status = tfm_its_get_info(msg->client_id, uid, &info);
    if (status == PSA_SUCCESS) {
        psa_write(msg->handle, 0, &info, sizeof(info));
    }

    return status;
}

static psa_status_t tfm_its_remove_ipc(const psa_msg_t *msg)
{
    psa_storage_uid_t uid;
    size_t num;

    if (msg->in_size[0] != sizeof(uid)) {
        /* The input argument size is incorrect */
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    num = psa_read(msg->handle, 0, &uid, sizeof(uid));
    if (num != sizeof(uid)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    return tfm_its_remove(msg->client_id, uid);
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

static void its_signal_handle(psa_signal_t signal, its_func_t pfn)
{
    psa_msg_t msg;
    psa_status_t status;

    status = psa_get(signal, &msg);
    if (status != PSA_SUCCESS) {
        return;
    }

    switch (msg.type) {
    case PSA_IPC_CONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        status = pfn(&msg);
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

psa_status_t tfm_its_req_mngr_init(void)
{
#ifdef TFM_PSA_API
    psa_signal_t signals = 0;

    if (tfm_its_init() != PSA_SUCCESS) {
        tfm_abort();
    }

    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & TFM_ITS_SET_SIG) {
            its_signal_handle(TFM_ITS_SET_SIG, tfm_its_set_ipc);
        } else if (signals & TFM_ITS_GET_SIG) {
            its_signal_handle(TFM_ITS_GET_SIG, tfm_its_get_ipc);
        } else if (signals & TFM_ITS_GET_INFO_SIG) {
            its_signal_handle(TFM_ITS_GET_INFO_SIG, tfm_its_get_info_ipc);
        } else if (signals & TFM_ITS_REMOVE_SIG) {
            its_signal_handle(TFM_ITS_REMOVE_SIG, tfm_its_remove_ipc);
        } else {
            tfm_abort();
        }
    }
#else
    if (tfm_its_init() != PSA_SUCCESS) {
        return PSA_ERROR_GENERIC_ERROR;
    }
    its_is_init = true;
    return PSA_SUCCESS;
#endif
}
