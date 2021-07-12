/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PERIPHERALS_DEF_H__
#define __TFM_PERIPHERALS_DEF_H__

struct tfm_spm_partition_platform_data_t;

extern struct tfm_spm_partition_platform_data_t tfm_peripheral_std_uart;

#define TFM_PERIPHERAL_STD_UART     (&tfm_peripheral_std_uart)

#define TAMP_IRQ TAMP_IRQn
#define GTZC_IRQ GTZC_IRQn

#endif /* __TFM_PERIPHERALS_DEF_H__ */
