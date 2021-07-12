/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifdef TFM_PARTITION_PLATFORM

#include "platform_sp.h"

#include "tfm_platform_system.h"
#include "tfm_veneers.h"
#include "tfm_memory_utils.h"
#include "tfm_client.h"
#include "tfm_secure_api.h"
#ifdef TFM_PSA_API
#include "psa_manifest/tfm_platform.h"


#define INPUT_BUFFER_SIZE  64
#define OUTPUT_BUFFER_SIZE 64

typedef enum tfm_platform_err_t (*plat_func_t)(const psa_msg_t *msg);
#endif
extern void Error_Handler(void);
/*
 * \brief Verifies ownership of a nv_counter resource to a partition id.
 *
 * \param[in] nv_counter_no  Number of nv_counter as assigned in platform.
 *
 * \return true if the calling partition is allowed to access this counter id
 */


static enum tfm_platform_err_t
platform_sp_system_reset_ipc(const psa_msg_t *msg)
{
    (void)msg; /* unused parameter */
    /* function is not returning */
    tfm_platform_hal_system_reset();
    /* this creates link error */
    return TFM_PLATFORM_ERR_SYSTEM_ERROR;
}



static enum tfm_platform_err_t
platform_sp_ioctl_ipc(const psa_msg_t *msg)
{
    void *input = NULL;
    void *output = NULL;
    psa_invec invec = {0};
    psa_outvec outvec = {0};
    uint8_t input_buffer[INPUT_BUFFER_SIZE] = {0};
    uint8_t output_buffer[OUTPUT_BUFFER_SIZE] = {0};
    tfm_platform_ioctl_req_t request = 0;
    enum tfm_platform_err_t ret = TFM_PLATFORM_ERR_SYSTEM_ERROR;
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
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    }

    num = psa_read(msg->handle, 0, &request, sizeof(request));
    if (num != sizeof(request)) {
        return (enum tfm_platform_err_t) PSA_ERROR_PROGRAMMER_ERROR;
    }

    if (in_len > 1) {
        if (msg->in_size[1] > INPUT_BUFFER_SIZE) {
            return (enum tfm_platform_err_t) PSA_ERROR_PROGRAMMER_ERROR;
        }
        num = psa_read(msg->handle, 1, &input_buffer, msg->in_size[1]);
        if (num != msg->in_size[1]) {
            return (enum tfm_platform_err_t) PSA_ERROR_PROGRAMMER_ERROR;
        }
        invec.base = input_buffer;
        invec.len = msg->in_size[1];
        input = &invec;
    }

    if (out_len > 0) {
        if (msg->out_size[0] > OUTPUT_BUFFER_SIZE) {
            return (enum tfm_platform_err_t) PSA_ERROR_PROGRAMMER_ERROR;
        }
        outvec.base = output_buffer;
        outvec.len = msg->out_size[0];
        output = &outvec;
    }

    ret = tfm_platform_hal_ioctl(request, input, output);

    if (output != NULL) {
        psa_write(msg->handle, 0, outvec.base, outvec.len);
    }

    return ret;
}

static void platform_signal_handle(psa_signal_t signal, plat_func_t pfn)
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


enum tfm_platform_err_t platform_sp_init(void)
{
    psa_signal_t signals = 0;
    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & SPM_TAMPER_IRQ)
        {
	    LOG_MSG("TAMPER EVENT");
             tfm_platform_hal_system_reset();
        }
        if (signals & SPM_GTZC_IRQ)
        {    
	    LOG_MSG("GTZC EVENT");
            tfm_platform_hal_system_reset();
        }
        if (signals & PLATFORM_SP_SYSTEM_RESET_SIG) {
            platform_signal_handle(PLATFORM_SP_SYSTEM_RESET_SIG,
                                   platform_sp_system_reset_ipc);
        } else if (signals & PLATFORM_SP_IOCTL_SIG) {
            platform_signal_handle(PLATFORM_SP_IOCTL_SIG,
                                   platform_sp_ioctl_ipc);
        } else {
            Error_Handler();
        }
    }
}
#endif