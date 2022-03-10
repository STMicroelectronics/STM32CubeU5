/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_UTILS_H__
#define __TFM_UTILS_H__

#include <stdbool.h>
#include <stdio.h>

/*
 * CPU spin here.
 * Note: this function is used to handle PROGRAMMER ERROR.
 */
void tfm_core_panic(void);

/* Core assert and spin */
#ifndef NDEBUG
#define TFM_CORE_ASSERT(cond)                                       \
            do {                                                    \
                if (!(cond)) {                                      \
                    while (1)                                       \
                        ;                                           \
                }                                                   \
            } while (0)
#else
#define TFM_CORE_ASSERT(cond)
#endif

/* Get container structure start address from member */
#define TFM_GET_CONTAINER_PTR(ptr, type, member) \
    (type *)((unsigned long)(ptr) - offsetof(type, member))

#define ERROR_MSG(msg)

#endif /* __TFM_UTILS_H__ */
