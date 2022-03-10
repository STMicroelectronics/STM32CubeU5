/*
 * Copyright (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __NV_COUNTERS_H__
#define __NV_COUNTERS_H__

#include <stdint.h>
#include "cmsis_compiler.h"
/**
 * \brief magic for NV_COUNTERS INIT.
 */
#define NV_COUNTERS_INITIALIZED 0xC0DE0042

/**
 * \brief Struct representing the NV counter data in flash.
 */
struct nv_counters_t {
    uint32_t crc;
    uint32_t init_value; /**< Watermark to indicate if the NV counters have been
                          *   initialised
                          */
    uint32_t counters[DEVICE_NUM_NV_COUNTERS]; /**< Array of NV counters */
};

#endif /* __NV_COUNTERS_H__ */
