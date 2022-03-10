
/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "psa/service.h"
#include "psa_manifest/tfm_ffm11_partition.h"
#include "tfm/tfm_spm_services.h"
#include "tfm_sp_log.h"

/**
 * \brief An example service implementation that prints out a message.
 */
static void tfm_ffm11_service1(void)
{
    psa_status_t status;
    uint32_t arg;
    psa_msg_t msg;
    size_t num;

    /* Retrieve the message corresponding to the example service signal */
    status = psa_get(TFM_FFM11_SERVICE1_SIGNAL, &msg);
    if (status != PSA_SUCCESS) {
        return;
    }

    /* Decode the message */
    switch (msg.type) {
    case PSA_IPC_CALL:
        if (msg.in_size[0] != sizeof(arg)) {
            status = PSA_ERROR_PROGRAMMER_ERROR;
            break;
        }
        /* Print arg from client */
        num = psa_read(msg.handle, 0, &arg, sizeof(arg));
        if (num != msg.in_size[0]) {
            status = PSA_ERROR_PROGRAMMER_ERROR;
            break;
        }
        LOG_INFFMT("[Example FFM11 partition] Service called! arg=%x\r\n", arg);
        status = PSA_SUCCESS;
        break;
    default:
        /* Invalid message type */
        status = PSA_ERROR_PROGRAMMER_ERROR;
        break;
    }
    /* Reply with the message result status to unblock the client */
    psa_reply(msg.handle, status);
}

/**
 * \brief The example FFM-1.1 partition's entry function.
 */
void tfm_ffm11_partition_main(void)
{
    psa_signal_t signals;

    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & TFM_FFM11_SERVICE1_SIGNAL) {
            tfm_ffm11_service1();
        }
        /*
         * The other services are created in yaml for testing manifest tool,
         * but not handled here. They are reserved for future use.
         */
    }
}
