/*
 * Copyright (c) 2017-2018 ARM Limited
 *
 * Licensed under the Apace License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apace.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "uart_stdout.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "Driver_USART.h"
#include "target_cfg.h"

#define ASSERT_HIGH(X)  assert(X == ARM_DRIVER_OK)

/* Imports USART driver */
extern ARM_DRIVER_USART TFM_DRIVER_STDIO;

static void uart_putc(unsigned char c)
{
#ifdef NDEBUG
    __attribute__((unused))
#endif
    int32_t ret = ARM_DRIVER_OK;

    ret = TFM_DRIVER_STDIO.Send(&c, 1);
    ASSERT_HIGH(ret);
}

/* Redirects printf to TFM_DRIVER_STDIO in case of ARMCLANG*/
#if defined(__ARMCC_VERSION)
/* Struct FILE is implemented in stdio.h. Used to redirect printf to
 * TFM_DRIVER_STDIO
 */
FILE __stdout;

/* __ARMCC_VERSION is only defined starting from Arm compiler version 6 */
int fputc(int ch, FILE *f)
{
    /* Send byte to USART */
    uart_putc(ch);

    /* Return character written */
    return ch;
}
#elif defined(__GNUC__)
/* Redirects printf to TFM_DRIVER_STDIO in case of GNUARM */
int _write(int fd, char *str, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        /* Send byte to USART */
        uart_putc(str[i]);
    }

    /* Return the number of characters written */
    return len;
}
#elif defined(__ICCARM__)
int putchar(int ch)
{
    /* Send byte to USART */
    uart_putc(ch);

    /* Return character written */
    return ch;
}
#endif

void stdio_init(void)
{
#ifdef NDEBUG
    __attribute__((unused))
#endif
    int32_t ret = ARM_DRIVER_OK;
    ret = TFM_DRIVER_STDIO.Initialize(NULL);
    ASSERT_HIGH(ret);

    ret = TFM_DRIVER_STDIO.Control(ARM_USART_MODE_ASYNCHRONOUS, 115200);
    ASSERT_HIGH(ret);
}

void stdio_uninit(void)
{
#ifdef NDEBUG
    __attribute__((unused))
#endif
    int32_t ret = ARM_DRIVER_OK;
    ret = TFM_DRIVER_STDIO.Uninitialize();
    ASSERT_HIGH(ret);
}

