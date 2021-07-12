/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#include "secure_fw/services/secure_storage/psa_manifest/tfm_secure_storage.h"
#include "secure_fw/services/internal_trusted_storage/psa_manifest/tfm_internal_trusted_storage.h"
#include "secure_fw/services/audit_logging/psa_manifest/tfm_audit_logging.h"
#include "secure_fw/services/crypto/psa_manifest/tfm_crypto.h"
#include "secure_fw/services/platform/psa_manifest/tfm_platform.h"
#include "secure_fw/services/initial_attestation/psa_manifest/tfm_initial_attestation.h"
#include "test/test_services/tfm_core_test/psa_manifest/tfm_test_core.h"
#include "test/test_services/tfm_core_test_2/psa_manifest/tfm_test_core_2.h"
#include "test/test_services/tfm_secure_client_service/psa_manifest/tfm_test_client_service.h"
#include "test/test_services/tfm_ipc_service/psa_manifest/tfm_ipc_service_partition.h"
#include "test/test_services/tfm_ipc_client/psa_manifest/tfm_ipc_client_partition.h"
#include "test/test_services/tfm_irq_test_service_1/psa_manifest/tfm_irq_test_service_1.h"
#include "secure_fw/services/app_rot/psa_manifest/tfm_app_rot.h"
#include "cmsis_compiler.h"

/* Definitions of the signals of the IRQs (if any) */
const struct tfm_core_irq_signal_data_t tfm_core_irq_signals[] = {
#ifdef TFM_PARTITION_PLATFORM
    { TFM_SP_PLATFORM_ID, SPM_TAMPER_IRQ, TAMP_IRQ, 0 },
    { TFM_SP_PLATFORM_ID, SPM_GTZC_IRQ, GTZC_IRQ, 0},
#endif /* TFM_PARTITION_PLATFORM */
#ifdef TFM_ENABLE_IRQ_TEST
    { TFM_IRQ_TEST_1_ID, SPM_CORE_IRQ_TEST_1_SIGNAL_TIMER_0_IRQ, TFM_TIMER0_IRQ, 64 },
#endif /* TFM_ENABLE_IRQ_TEST */
    {0, 0,(IRQn_Type)0, 0}
};

const size_t tfm_core_irq_signals_count = (sizeof(tfm_core_irq_signals) /
                                           sizeof(*tfm_core_irq_signals)) - 1;

/* Definitions of privileged IRQ handlers (if any) */
#ifdef TFM_PARTITION_PLATFORM
void TAMP_IRQHandler(void)
{
    __disable_irq();
    /* It is OK to call tfm_irq_handler directly from here, as we are already
     * in handler mode, and we will not be pre-empted as we disabled interrupts
     */
    tfm_irq_handler(TFM_SP_PLATFORM_ID, SPM_TAMPER_IRQ, TAMP_IRQ);
    __enable_irq();
}

void GTZC_IRQHandler(void)
{
    __disable_irq();
    /* It is OK to call tfm_irq_handler directly from here, as we are already
     * in handler mode, and we will not be pre-empted as we disabled interrupts
     */
    tfm_irq_handler(TFM_SP_PLATFORM_ID, SPM_GTZC_IRQ, GTZC_IRQ);
    __enable_irq();
}

#endif /* TFM_PARTITION_PLATFORM */

#ifdef TFM_ENABLE_IRQ_TEST
void TFM_TIMER0_IRQHandler(void)
{
    __disable_irq();
    /* It is OK to call tfm_irq_handler directly from here, as we are already
     * in handler mode, and we will not be pre-empted as we disabled interrupts
     */
    tfm_irq_handler(TFM_IRQ_TEST_1_ID, SPM_CORE_IRQ_TEST_1_SIGNAL_TIMER_0_IRQ, TFM_TIMER0_IRQ);
    __enable_irq();
}

#endif /* TFM_ENABLE_IRQ_TEST */

