/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdint.h>
#include "stm32u5xx_hal.h"
#include "tfm_api.h"
#include "tfm_ns_interface.h"


static uint8_t users = 0;

#define COMPILER_BARRIER() __ASM volatile("" : : : "memory")

static uint8_t core_util_atomic_incr_u8(volatile uint8_t *valuePtr, uint8_t delta)
{
  COMPILER_BARRIER();
  uint8_t newValue;
  do
  {
    newValue = __LDREXB(valuePtr) + delta;
  } while (__STREXB(newValue, valuePtr));
  COMPILER_BARRIER();
  return newValue;
}
/**
  * \brief NS world, NS lock based dispatcher
  */
int32_t tfm_ns_interface_dispatch(veneer_fn fn,
                                  uint32_t arg0, uint32_t arg1,
                                  uint32_t arg2, uint32_t arg3)
{
  uint32_t ret;
  /*  We're only supporting a single user of RNG */
  if (core_util_atomic_incr_u8(&users, 1) > 1)
  {
    while (1);
  }
  ret = (uint32_t)fn(arg0, arg1, arg2, arg3);
  users = 0;
  return ret;
}

enum tfm_status_e tfm_ns_interface_init(void)
{
  return TFM_SUCCESS;
}

