/*
 * Copyright (c) 2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __ARCH_H__
#define __ARCH_H__

/*
 * This function pointer is meant to only hold non secure function pointers.
 * It will be turned into a non-secure one (LSB cleared) before being called
 * whatever happens anyway (unless cast to another function pointer type).
 * Registers will be cleared before branching so that no information leaks
 * from secure to non-secure world.
 */
typedef void (*nsfptr_t) (void) __attribute__((cmse_nonsecure_call));

extern nsfptr_t ns_entry;

/**
 * \brief  Jump to non-secure code.
 */
void jump_to_ns_code(void);

#endif /* __ARCH_H__ */
