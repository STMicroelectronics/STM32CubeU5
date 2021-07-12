/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "tfm_utils.h"
#include "tfm_core_utils.h"

union mem_ptr_t {
    uintptr_t mem_ptr;
    uint8_t *p_byte;
    uint32_t *p_dword;
};

void *tfm_core_util_memcpy(void *dest, const void *src, size_t n)
{
    union mem_ptr_t p_dest;
    union mem_ptr_t p_src;

    TFM_ASSERT(dest != src);

    p_dest.p_byte = (uint8_t *)dest;
    p_src.p_byte = (uint8_t *)src;

    /* Check src and dest address value to see if dword copy is applicable.
     * If applicable, perform byte copy for the first several unaligned bytes,
     * and then, dword copy for aligned memory.
     */
    if (!((p_dest.mem_ptr ^ p_src.mem_ptr) & (sizeof(uint32_t) - 1))) {
        while (n && (p_dest.mem_ptr & (sizeof(uint32_t) - 1))) {
            *p_dest.p_byte++ = *p_src.p_byte++;
            n--;
        }

        while (n >= sizeof(uint32_t)) {
            *p_dest.p_dword++ = *p_src.p_dword++;
            n -= sizeof(uint32_t);
        }
    }

    /* Dword copy is not applicable, perform byte copy for the remaining
     * unaligned memory.
     */
    while (n--) {
        *p_dest.p_byte++ = *p_src.p_byte++;
    }

    return dest;
}

void *tfm_core_util_memset(void *s, int c, size_t n)
{
    union mem_ptr_t p_mem;
    uint32_t quad_pattern;

    p_mem.p_byte = (uint8_t *)s;
    quad_pattern = (((uint8_t)c) << 24) | (((uint8_t)c) << 16) |
                   (((uint8_t)c) << 8) | ((uint8_t)c);

    while (n && (p_mem.mem_ptr & (sizeof(uint32_t) - 1))) {
        *p_mem.p_byte++ = (uint8_t)c;
        n--;
    }

    while (n >= sizeof(uint32_t)) {
        *p_mem.p_dword++ = quad_pattern;
        n -= sizeof(uint32_t);
    }

    while (n--) {
        *p_mem.p_byte++ = (uint8_t)c;
    }

    return s;
}
