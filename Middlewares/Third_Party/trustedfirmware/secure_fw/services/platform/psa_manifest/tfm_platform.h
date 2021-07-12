/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

#ifndef __PSA_MANIFEST_TFM_PLATFORM_H__
#define __PSA_MANIFEST_TFM_PLATFORM_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TFM_PARTITION_PLATFORM
#define PLATFORM_SP_SYSTEM_RESET_SIG                            (1U << ( 0 + 4))
#define PLATFORM_SP_IOCTL_SIG                                   (1U << ( 1 + 4))

#define SPM_TAMPER_IRQ                                          (1U << (27 + 4))
#define SPM_GTZC_IRQ                                            (1U << (26 + 4))

#endif /* TFM_PARTITION_PLATFORM */
#ifdef __cplusplus
}
#endif

#endif /* __PSA_MANIFEST_TFM_PLATFORM_H__ */
