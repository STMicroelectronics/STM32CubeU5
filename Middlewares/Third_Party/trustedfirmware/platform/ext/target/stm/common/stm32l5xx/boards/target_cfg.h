/*
 * Copyright (c) 2018-2020 Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __STM32L5XX_CFG_H__
#define __STM32L5XX_CFG_H__

#include "uart_stdout.h"
#include "tfm_peripherals_def.h"

#define TFM_DRIVER_STDIO    Driver_USART0
#define NS_DRIVER_STDIO     Driver_USART0

/**
 * \brief Store the addresses of memory regions
 */
struct memory_region_limits {
    uint32_t non_secure_code_start;
    uint32_t non_secure_partition_base;
    uint32_t non_secure_partition_limit;
    uint32_t veneer_base;
    uint32_t veneer_limit;
};

/**
 * \brief Holds the data necessary to do isolation for a specific peripheral.
 */
struct platform_data_t
{
    uint32_t periph_start;
    uint32_t periph_limit;
    int16_t periph_ppc_bank;
    int16_t periph_ppc_loc;
};

/**
 * \brief Configures the Memory Protection Controller.
 */
void gtzc_init_cfg(void);


/**
 * \brief pin mux configuration for non secure access
 * */
void pinmux_init_cfg(void);


/**
 * \brief Configures SAU and IDAU.
 */
void sau_and_idau_cfg(void);

/**
 * \brief Enable Fault handling.
 */
enum tfm_plat_err_t enable_fault_handlers(void);

/**
 * \brief Set NVIC interrupt target state to NS.
 */
enum tfm_plat_err_t nvic_interrupt_target_state_cfg();

#endif /* __STM32L5XX_CFG_H__ */
