/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "region_defs.h"
#ifdef TFM_PARTITION_APP_ROT

#include "app_rot_hal.h"
#include "tfm_veneers.h"
#include "tfm_memory_utils.h"
#include "tfm_client.h"
#include "tfm_secure_api.h"
#include "uart_stdout.h"
#ifdef TFM_PSA_API
#include "psa_manifest/tfm_app_rot.h"
#endif
#include "tfm_app_rot_api.h"
extern void Error_Handler(void);
#define INPUT_BUFFER_SIZE  64
#define OUTPUT_BUFFER_SIZE 64
static enum tfm_app_rot_err_t app_rot_ioctl_ipc(const psa_msg_t *msg);

typedef enum tfm_app_rot_err_t (*app_rot_func_t)(const psa_msg_t *msg);

/*
 * \brief .
 *
 * \param[in]
 *
 * \return true if the calling partition is allowed
 */
static enum tfm_app_rot_err_t
app_rot_ioctl_ipc(const psa_msg_t *msg)
{
    void *input = NULL;
    void *output = NULL;
    psa_invec invec = {0};
    psa_outvec outvec = {0};
    uint8_t input_buffer[INPUT_BUFFER_SIZE] = {0};
    uint8_t output_buffer[OUTPUT_BUFFER_SIZE] = {0};
    tfm_app_rot_ioctl_req_t request = 0;
    enum tfm_app_rot_err_t ret = TFM_APP_ROT_ERROR;
    int num = 0;
    uint32_t in_len = PSA_MAX_IOVEC;
    uint32_t out_len = PSA_MAX_IOVEC;

    while ((in_len > 0) && (msg->in_size[in_len - 1] == 0)) {
        in_len--;
    }

    while ((out_len > 0) && (msg->out_size[out_len - 1] == 0)) {
        out_len--;
    }

    if ((in_len < 1) || (in_len > 2) ||
        (out_len > 1)) {
        return TFM_APP_ROT_ERROR;
    }

    num = psa_read(msg->handle, 0, &request, sizeof(request));
    if (num != sizeof(request)) {
        return (enum tfm_app_rot_err_t) PSA_ERROR_PROGRAMMER_ERROR;
    }

    if (in_len > 1) {
        if (msg->in_size[1] > INPUT_BUFFER_SIZE) {
            return (enum tfm_app_rot_err_t) PSA_ERROR_PROGRAMMER_ERROR;
        }
        num = psa_read(msg->handle, 1, &input_buffer, msg->in_size[1]);
        if (num != msg->in_size[1]) {
            return (enum tfm_app_rot_err_t) PSA_ERROR_PROGRAMMER_ERROR;
        }
        invec.base = input_buffer;
        invec.len = msg->in_size[1];
        input = &invec;
    }

    if (out_len > 0) {
        if (msg->out_size[0] > OUTPUT_BUFFER_SIZE) {
            return (enum tfm_app_rot_err_t) PSA_ERROR_PROGRAMMER_ERROR;
        }
        outvec.base = output_buffer;
        outvec.len = msg->out_size[0];
        output = &outvec;
    }

    ret = app_rot_hal_ioctl(request, input, output);

    if (output != NULL) {
        psa_write(msg->handle, 0, outvec.base, outvec.len);
    }

    return ret;
}

static void app_rot_signal_handle(psa_signal_t signal, app_rot_func_t pfn)
{
    psa_msg_t msg;
    psa_status_t status;

    status = psa_get(signal, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
        status = (psa_status_t)pfn(&msg);
        psa_reply(msg.handle, status);
        break;
    case PSA_IPC_DISCONNECT:
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        psa_reply(msg.handle, PSA_ERROR_NOT_SUPPORTED);
    }
}


enum tfm_app_rot_err_t app_rot_init(void)
{
    psa_signal_t signals = 0;
    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals &  TFM_APP_ROT_IOCTL_SIG) {
            app_rot_signal_handle(TFM_APP_ROT_IOCTL_SIG ,
                                   app_rot_ioctl_ipc);
        } else {
            Error_Handler();
        }
    }
}
#endif