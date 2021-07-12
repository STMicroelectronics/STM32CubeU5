/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_CORE_LOG_H__
#define __TFM_CORE_LOG_H__

#include <stddef.h>

/**
 * \brief Output the string.
 *
 * \param[in] str               The string to be printed.
 *
 * \retval void
 */
void tfm_core_log_puts(const char *str);

/**
 * \brief Output the char.
 *
 * \param[in] ch                The char to be printed.
 *
 * \retval void
 */
void tfm_core_log_putc(const char ch);

/**
 * \brief Output characters in a sized buffer.
 *
 * \param[in] pbuf              Pointer of the buffer contains characters to be
 *                              put.
 * \param[in] sz                The size of the buffer.
 *
 * \retval void
 *
 * \note This function is the output layer of string format functions
 *       ('printf' e.g.), do not call it for generic purpose like outputting a
 *       log message. This function filters out nonprintable characters except
 *       '\r' and '\n', because the console may want these characters to do
 *       formatting.
 */
void tfm_core_log_put_buf(const void *pbuf, size_t sz);

/**
 * \brief Output the number in hex format.
 *
 * \param[in] num               Number to be printed.
 *
 * \retval void
 */
void tfm_core_log_hex(uint32_t num);

#endif /* __TFM_CORE_LOG_H__ */
