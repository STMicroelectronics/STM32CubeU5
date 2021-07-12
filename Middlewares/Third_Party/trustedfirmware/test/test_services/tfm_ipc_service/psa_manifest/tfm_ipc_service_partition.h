/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __PSA_MANIFEST_TFM_IPC_SERVICE_PARTITION_H__
#define __PSA_MANIFEST_TFM_IPC_SERVICE_PARTITION_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TFM_PARTITION_TEST_CORE_IPC
#define IPC_SERVICE_TEST_BASIC_SIGNAL                           (1U << ( 0 + 4))
#define IPC_SERVICE_TEST_PSA_ACCESS_APP_MEM_SIGNAL              (1U << ( 1 + 4))
#define IPC_SERVICE_TEST_PSA_ACCESS_APP_READ_ONLY_MEM_SIGNAL    (1U << ( 2 + 4))
#define IPC_SERVICE_TEST_APP_ACCESS_PSA_MEM_SIGNAL              (1U << ( 3 + 4))

#endif /* TFM_PARTITION_TEST_CORE_IPC */
#ifdef __cplusplus
}
#endif

#endif /* __PSA_MANIFEST_TFM_IPC_SERVICE_PARTITION_H__ */
