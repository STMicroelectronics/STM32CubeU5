/*
 * Copyright (c) 2017-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "cmsis_compiler.h"

#ifndef __TFM_INTEG_TEST_H__
#define __TFM_INTEG_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Avoids the semihosting issue */
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
__asm("  .global __ARM_use_no_argv\n");
#endif

/**
 * \brief Simple macro to mark UNUSED variables
 *
 */
#define UNUSED_VARIABLE(X) ((void)(X))

#if defined(TEST_FRAMEWORK_NS) || defined(TEST_FRAMEWORK_S)
/**
 * \brief Main test application for the RTX-TFM core
 *        integration tests
 *
 */
void test_app(void *argument);
#endif /* TEST_FRAMEWORK_NS || TEST_FRAMEWORK_S */

#ifdef __cplusplus
}
#endif

#endif /* __TFM_INTEG_TEST_H__ */
