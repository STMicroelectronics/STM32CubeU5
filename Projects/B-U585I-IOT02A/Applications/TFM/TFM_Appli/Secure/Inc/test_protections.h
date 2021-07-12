/**
  ******************************************************************************
  * @file    test_protections.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for Test Protections functionalities.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TEST_PROTECTIONS_H
#define TEST_PROTECTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#ifdef TFM_PARTITION_APP_ROT
#include "tfm_api.h"
#endif /* TFM_PARTITION_APP_ROT */
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */


/** @addtogroup TEST_PROTECTIONS Protections test example
  * @{
  */
/** @defgroup  SFU_TEST_Exported_Constant Exported Constant
  * @{
  */
#define TEST_PROTECTION_MASK 0xdead0000
/* This MASK is set for previous test that should not create error */
/* If this Mask is present after reset last test is failed */
#define TEST_PROTECTION_FAILED 0xbeef0000
/* Magic number saved in DR0 backup register */
#define MAGIC_TEST_DONE          0xAAA2
#define MAGIC_TEST_INIT          0xAAA3

#define PROTECT_MSG_LEN (32U)           /* message to be displayed length           */
typedef enum
{
    TEST_WRITE_FLASH = 0U,                /* Writing test in FLASH */
    TEST_READ_FLASH,                      /* Reading test in FLASH */
    TEST_ERASE_FLASH,                     /* Erasing test in FLASH */
    TEST_WRITE_RAM,                       /* Writing test in RAM */
    TEST_READ_RAM,                        /* Reading test in RAM */
    TEST_EXECUTE,                         /* Execution function test */
    TEST_WRITE_EXEC,                      /* Write OP code BX_LR and Execute Address */
    TEST_READ_PERIPH,                     /* Read 4 bytes in periph */
    TEST_WRITE_PERIPH,                    /* Write 4 bytes in periph */
    TEST_READ_DMA,                        /* DMA Memory read */
    TEST_WRITE_DMA,                       /* DMA Memory Write */
    TEST_PRIV_WRITE_RAM,
    TEST_PRIV_READ_RAM,
    TEST_PRIV_WRITE_PERIPH,
    TEST_PRIV_READ_PERIPH,
    TEST_PRIV_EXECUTE,
    TEST_PRIV_WRITE_EXEC,
    TEST_END,                             /* Last test : Success ! */
} TestType;
#define TESTTYPE_STRINGS  \
    { \
        "write flash",\
        "read flash",\
        "erase flash",\
        "write 1 bytes",\
        "read 1 byte",\
        "execute",\
        "write_exec",\
        "read 4 bytes",\
        "write 4 bytes",\
        "dma read 8 bytes",\
        "dma write 8 bytes",\
        "pwrite 1 bytes", \
        "pread 1 bytes", \
        "pwrite 4 bytes", \
        "pread 4 bytes", \
        "pexecute", \
        "pwrite_exec",\
        "end"}
#define MAX_PERIPH_READ 10U
typedef enum
{
    ALLOWED, /* access allowed */
    DENIED,  /* illegal access detected generating reset */
    SILENT,  /* illegal access not detected, read 0 , write ignored */

} accessright;

#define ACCESS_STRINGS {\
            "ALLOWED", "DENIED", "SILENT" }

typedef enum
{
TEST_IN_PROGRESS = 0U,                /* Test in progress */
TEST_COMPLETED,                       /* Test ended : success */
TEST_ERROR,                           /* Test aborted : error */
} TestStatus;

typedef enum
{
RNG_TEST,
HASH_TEST,
NOT_APPLICABLE,
} PeriphTest;


/**
  * @}
  */

/** @defgroup  SFU_TEST_Exported_Types Exported Types
  * @{
  */

typedef struct
{
uint32_t address;                     /* address to be tested */
uint8_t msg[PROTECT_MSG_LEN];         /* message to be displayed when testing */
TestType type;                        /* type of test */
PeriphTest periph;
accessright access;
} TestProtection_t;

#define NS_MAX_TEST 0x100

#ifdef TFM_PARTITION_APP_ROT

#define S_MAX_TEST 0x100
#define IOCTL_TEST_PROTECTION_DESC 0
#define IOCTL_TEST_PROTECTION_RUN_TEST 1

#endif /* TFM_PARTITION_APP_ROT */
/**
  * @}
  */

/** @addtogroup  TEST_PROTECTION_Exported_Functions
  * @{
  */
/* Test Protection  Secure exported function */
#if  !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U)
void TEST_PROTECTIONS_Run_SecUserMem(void);
void TEST_PROTECTIONS_Run(void);
#endif /* !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U) */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* TEST_PROTECTIONS_H */

