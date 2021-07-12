/*
 * Copyright (c) 2018 ARM Limited
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

#ifndef __STM32U5xx_CFG_H__
#define __STM32U5xx_CFG_H__

#include "platform/ext/common/uart_stdout.h"
#include "tfm_peripherals_def.h"

#define TFM_DRIVER_STDIO    TFM_Driver_USART0
#define NS_DRIVER_STDIO     TFM_Driver_USART0

/*
// <o>Floating Point Unit usage
//     <0=> Secure state only
//     <3=> Secure and Non-Secure state
//   <i> Value for SCB->NSACR register bits CP10, CP11
*/
#define SCB_NSACR_CP10_11_VAL       3

/*
// <o>Treat floating-point registers as Secure
//     <0=> Disabled
//     <1=> Enabled
//   <i> Value for FPU->FPCCR register bit TS
*/
#define FPU_FPCCR_TS_VAL            0

/*
// <o>Clear on return (CLRONRET) accessibility
//     <0=> Secure and Non-Secure state
//     <1=> Secure state only
//   <i> Value for FPU->FPCCR register bit CLRONRETS
*/
#define FPU_FPCCR_CLRONRETS_VAL     0

/*
// <o>Clear floating-point caller saved registers on exception return
//     <0=> Disabled
//     <1=> Enabled
//   <i> Value for FPU->FPCCR register bit CLRONRET
*/
#define FPU_FPCCR_CLRONRET_VAL      1


#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

typedef enum
{
  TFM_FALSE = 0U,
  TFM_TRUE = !TFM_FALSE
} TFM_BoolTypeDef;

typedef enum
{
  TFM_DISABLE = 0U,
  TFM_ENABLE = !TFM_DISABLE
} TFM_FunctionalState;

struct sau_cfg_t {
    uint32_t RNR;
    uint32_t RBAR;
    uint32_t RLAR;
    TFM_BoolTypeDef nsc;
#ifdef FLOW_CONTROL
    uint32_t flow_step_enable;
    uint32_t flow_ctrl_enable;
    uint32_t flow_step_check;
    uint32_t flow_ctrl_check;
#endif
};

/**
  * \brief Store the addresses of memory regions
  */
struct memory_region_limits
{
  uint32_t non_secure_code_start;
  uint32_t non_secure_partition_base;
  uint32_t non_secure_partition_limit;
  uint32_t veneer_base;
  uint32_t veneer_limit;
};
/**
  * \brief Holds the data necessary to do isolation for a specific peripheral.
  */
struct tfm_spm_partition_platform_data_t
{
  uint32_t periph_start;
  uint32_t periph_limit;
  int16_t periph_ppc_bank;
  int16_t periph_ppc_loc;
};

/**
  * \brief Configures the Global TrustZone Controller.
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
  * \brief Enable Interruption Handled by Secure.
  */
enum tfm_plat_err_t nvic_interrupt_enable();
/**
  * \brief Configure all interruption on Non Secure side.
  */
enum tfm_plat_err_t nvic_interrupt_target_state_cfg();
#endif /* __STM32U5xx_CFG_H__ */