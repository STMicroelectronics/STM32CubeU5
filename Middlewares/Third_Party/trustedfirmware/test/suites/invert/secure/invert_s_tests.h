/*
 * Copyright (c) 2017, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __INVERT_S_TESTS_H__
#define __INVERT_S_TESTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "test/framework/test_framework.h"

/**
 * \brief Register testsuite for the invert test service.
 *
 * \param[in] p_test_suite The test suite to be executed.
 */
void register_testsuite_s_invert_interface(struct test_suite_t *p_test_suite);

#ifdef __cplusplus
}
#endif

#endif /* __INVERT_S_TESTS_H__ */
