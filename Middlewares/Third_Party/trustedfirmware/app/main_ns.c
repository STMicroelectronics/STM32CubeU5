/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "tfm_api.h"
#include "cmsis_os2.h"
#include "tfm_integ_test.h"
#include "tfm_ns_svc.h"
#include "tfm_ns_interface.h"
#ifdef TEST_FRAMEWORK_NS
#include "test/framework/test_framework_integ_test.h"
#endif
#ifdef PSA_API_TEST_NS
#include "psa_api_test.h"
#endif
#include "target_cfg.h"
#include "Driver_USART.h"

/* For UART the CMSIS driver is used */
extern ARM_DRIVER_USART NS_DRIVER_STDIO;

/**
 * \brief Modified table template for user defined SVC functions
 *
 * \details RTX has a weak definition of osRtxUserSVC, which
 *          is overridden here
 */
#if (defined(__ARMCC_VERSION) && (__ARMCC_VERSION == 6110004))
/* Workaround needed for a bug in Armclang 6.11, more details at:
 * http://www.keil.com/support/docs/4089.htm
 */
__attribute__((section(".gnu.linkonce")))
#endif
extern void * const osRtxUserSVC[1+USER_SVC_COUNT];
       void * const osRtxUserSVC[1+USER_SVC_COUNT] = {
  (void *)USER_SVC_COUNT,

#define X(SVC_ENUM, SVC_HANDLER) (void*)SVC_HANDLER,

    /* SVC API for Services */
#ifdef TFM_NS_CLIENT_IDENTIFICATION
    LIST_SVC_NSPM
#endif

#undef X

/*
 * (void *)user_function1,
 *  ...
 */
};

#if defined(__ARMCC_VERSION)
/* Struct FILE is implemented in stdio.h. Used to redirect printf to
 * NS_DRIVER_STDIO
 */
FILE __stdout;
/* Redirects armclang printf to NS_DRIVER_STDIO */
int fputc(int ch, FILE *f) {
    /* Send byte to NS_DRIVER_STDIO */
    (void)NS_DRIVER_STDIO.Send((const unsigned char *)&ch, 1);
    /* Return character written */
    return ch;
}
#elif defined(__GNUC__)
/* redirects gcc printf to NS_DRIVER_STDIO */
int _write(int fd, char * str, int len)
{
    (void)NS_DRIVER_STDIO.Send(str, len);

    return len;
}
#elif defined(__ICCARM__)
int putchar(int ch)
{
    /* Send byte to NS_DRIVER_STDIO */
    (void)NS_DRIVER_STDIO.Send((const unsigned char *)&ch, 1);
    /* Return character written */
    return ch;
}
#endif

/**
 * \brief List of RTOS thread attributes
 */
#if defined(TEST_FRAMEWORK_NS) || defined(PSA_API_TEST_NS)
static uint64_t test_app_stack[(3u * 1024u) / (sizeof(uint64_t))]; /* 3KB */
static const osThreadAttr_t thread_attr = {
    .name = "test_thread",
    .stack_mem = test_app_stack,
    .stack_size = sizeof(test_app_stack),
};
#endif

/**
 * \brief Static globals to hold RTOS related quantities,
 *        main thread
 */
static osStatus_t     status;
static osThreadId_t   thread_id;
static osThreadFunc_t thread_func;

/**
 * \brief main() function
 */
#ifndef __GNUC__
__attribute__((noreturn))
#endif
int main(void)
{
    (void)NS_DRIVER_STDIO.Initialize(NULL);
    NS_DRIVER_STDIO.Control(ARM_USART_MODE_ASYNCHRONOUS, 115200);

    status = osKernelInitialize();

    /* Initialize the TFM NS interface */
    tfm_ns_interface_init();

#if defined(TEST_FRAMEWORK_NS)
    thread_func = test_app;
#elif defined(PSA_API_TEST_NS)
    thread_func = psa_api_test;
#endif

#if defined(TEST_FRAMEWORK_NS) || defined(PSA_API_TEST_NS)
    thread_id = osThreadNew(thread_func, NULL, &thread_attr);
#else
    UNUSED_VARIABLE(thread_id);
    UNUSED_VARIABLE(thread_func);
#endif

    status = osKernelStart();

    /* Reached only in case of error */
    for (;;) {
    }
}
