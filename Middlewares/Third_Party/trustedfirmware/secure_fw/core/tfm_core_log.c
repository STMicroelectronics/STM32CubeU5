/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stddef.h>
#include "Driver_USART.h"
#include "target_cfg.h"
#include "tfm_utils.h"
#include "uart_stdout.h"

/* Imports USART driver */
extern ARM_DRIVER_USART TFM_DRIVER_STDIO;

#define UART_PUTC(c)                                \
        do {                                        \
            (void)TFM_DRIVER_STDIO.Send(&c, 1);     \
        } while (0)

void tfm_core_log_puts(const char *str)
{
    TFM_ASSERT(str != NULL);

    while (*str != '\0') {
        UART_PUTC(*str);
        str++;
    }
}

void tfm_core_log_putc(const char ch)
{
    UART_PUTC(ch);
}

void tfm_core_log_put_buf(const void *pbuf, size_t sz)
{
    const char *p = (const char *)pbuf;

    TFM_ASSERT(p != NULL);

    while (sz--) {
        /* Printable ASCII starts from 0x20 */
        if (*p > 0x1F || *p == '\n' || *p == '\r') {
            UART_PUTC(*p);
        }
        p++;
    }
}

const static char HEX_DIGITS[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void tfm_core_log_hex(uint32_t num)
{
    uint32_t pos;

    tfm_core_log_putc('0');
    tfm_core_log_putc('x');
    for (pos = 0; pos < 8; pos++, num <<= 4) {
        UART_PUTC(HEX_DIGITS[(num & 0xF0000000) >> 28]);
    }
}
