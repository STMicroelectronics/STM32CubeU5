/**
  ******************************************************************************
  * @file    test_protections.c
  * @author  MCD Application Team
  * @brief   Test Protections module.
  *          This file provides set of firmware functions to manage Test Protections
  *          functionalities.
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

/* Includes ------------------------------------------------------------------*/
#if !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U)
#include "main.h"
#include "com.h"
#include "common.h"
#endif /* !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U) */
#include "mpu_armv8m_drv.h"
#include "string.h"
#include "stm32u5xx_hal.h"
#include "region_defs.h"
#include "test_protections.h"
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && !defined(TFM_PARTITION_APP_ROT)
/* empty file in TFM_PARTITION_APP_ROT not defined, when compiled in secure */

#else
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#include "region.h"
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */
/** @addtogroup TEST_PROTECTIONS Test protections
  * @{
  */

/** @defgroup  TEST_PROTECTIONS_Private_Defines Private Defines
  * @{
  */
#define NS_MAX_TEST 0x100
#define S_MAX_TEST 0x100

/* code OP to be used to test BX LR */
#define BX_LR_OP_CODE 0x4770

/** @defgroup  TEST_PROTECTIONS_Private_Variables Private Variables
  * @{
  */
const  __attribute__((__aligned__(2))) uint16_t BX_LR =  BX_LR_OP_CODE;
/*  function used to validate TEST_EXECUTE */
void Test_Execute(void);
void Test_Execute(void)
{
    static __IO int TestExec = 0;
    TestExec++;
}

/* Automatic tests : list of tests (Address, Message, Operation */
/* Flash programming by 64 bits */
#define SHIFT_FLASH_WRITE (8U-1U)
#if !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U)
const TestProtection_t aProtectTests[] =
{
    {NS_RAM_ALIAS(S_DATA_START - S_RAM_ALIAS(0)), "Data Secure Start", TEST_READ_RAM, NOT_APPLICABLE, DENIED},
    {NS_RAM_ALIAS(S_DATA_START - S_RAM_ALIAS(0) + S_DATA_SIZE), "Data Secure End", TEST_READ_RAM, NOT_APPLICABLE, DENIED},
    {S_ROM_ALIAS(S_IMAGE_PRIMARY_AREA_OFFSET), "Code Secure Start", TEST_READ_FLASH, NOT_APPLICABLE, DENIED},
    {S_ROM_ALIAS(S_IMAGE_PRIMARY_AREA_OFFSET) + FLASH_S_PARTITION_SIZE - 1, "Code Secure END(veneer)", TEST_READ_FLASH, NOT_APPLICABLE, DENIED},
    { (uint32_t) &Test_Execute, "fun call", TEST_EXECUTE,  NOT_APPLICABLE, ALLOWED},
    { (uint32_t)(&BX_LR) + 1, "BX_LR const", TEST_EXECUTE, NOT_APPLICABLE, ALLOWED },
    {NS_RAM_ALIAS(S_DATA_START - S_RAM_ALIAS(0) - 4096), "Read NS RAM", TEST_READ_RAM, NOT_APPLICABLE, ALLOWED},
    {NS_RAM_ALIAS(S_DATA_START - S_RAM_ALIAS(0) - 4096), "Exec NS RAM", TEST_WRITE_EXEC, NOT_APPLICABLE, DENIED},
    {0x00000000, "Execution successful", TEST_END}
};
#else
static __attribute__((__aligned__(4))) uint32_t DMA_NON_SECURE[2] = {  0, 0 };
const  __attribute__((__aligned__(4))) uint32_t DMA_CONST[2] = {  0xfeedbeef, 0xcacadada };
REGION_DECLARE(Load$$LR$$, LR_VENEER, $$Limit);
REGION_DECLARE(Image$$, TFM_APP_RW_STACK_END, $$Base);
const TestProtection_t aProtectTests[] =
{
    { (uint32_t)DMA_CONST, "NPRIV SEC", TEST_READ_DMA, NOT_APPLICABLE, ALLOWED},
    { (uint32_t)DMA_NON_SECURE, "NPRIV SEC", TEST_WRITE_DMA, NOT_APPLICABLE,  ALLOWED},
    { (uint32_t) &GTZC_TZIC1->IER3, "GTZC1_TZCIC IER3", TEST_WRITE_PERIPH, NOT_APPLICABLE, SILENT},
    { (uint32_t) &GTZC_TZIC2->IER3, "GTZC2_TZCIC IER3", TEST_READ_PERIPH, NOT_APPLICABLE, SILENT},
    {  (uint32_t) &REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base) - 1, "NPRIV END", TEST_READ_RAM, NOT_APPLICABLE, ALLOWED},
    {  (uint32_t) &REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base), "PRIV Begin", TEST_WRITE_RAM, NOT_APPLICABLE, DENIED},
    {  (uint32_t) &REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base) - 8, "NPRIV END", TEST_READ_DMA, NOT_APPLICABLE, ALLOWED},
    {  (uint32_t) &REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base), "NPRIV END", TEST_READ_DMA, NOT_APPLICABLE, SILENT},
    {  S_RAM_ALIAS(BOOT_TFM_SHARED_DATA_BASE - SRAM1_BASE_S), "BL2 SHARED", TEST_WRITE_DMA, NOT_APPLICABLE, SILENT},
    {  S_ROM_ALIAS(FLASH_ITS_AREA_OFFSET), "ITS", TEST_READ_DMA, NOT_APPLICABLE, SILENT},
    {  S_ROM_ALIAS(FLASH_ITS_AREA_OFFSET), "ITS", TEST_READ_DMA, NOT_APPLICABLE, SILENT},
    {  S_RAM_ALIAS(S_DATA_START - S_RAM_ALIAS(0) + S_DATA_SIZE), "PRIV", TEST_READ_RAM, NOT_APPLICABLE, DENIED},
    {  S_RAM_ALIAS(BOOT_TFM_SHARED_DATA_BASE - SRAM1_BASE_S), "BL2 SHARED", TEST_READ_RAM, NOT_APPLICABLE, DENIED},
    {  S_ROM_ALIAS(FLASH_ITS_AREA_OFFSET), "ITS", TEST_READ_FLASH, NOT_APPLICABLE, DENIED},
    {  S_RAM_ALIAS(S_DATA_START - S_RAM_ALIAS(0)), "Data Secure Start", TEST_READ_RAM, NOT_APPLICABLE, ALLOWED},
    { (uint32_t)DMA_NON_SECURE, "NPRIV SEC", TEST_WRITE_EXEC, NOT_APPLICABLE,  DENIED},
    {  S_ROM_ALIAS(S_IMAGE_PRIMARY_AREA_OFFSET), "Code Secure Start", TEST_READ_FLASH, NOT_APPLICABLE, DENIED},
    {
        (uint32_t)(&REGION_NAME(Load$$LR$$, LR_VENEER, $$Limit)) - 1, "Code Secure END(veneer)",
        TEST_READ_FLASH, NOT_APPLICABLE, ALLOWED
    },
    {
        (uint32_t)(&REGION_NAME(Load$$LR$$, LR_VENEER, $$Limit)) + 1, "Code Secure END(veneer)",
        TEST_READ_FLASH, NOT_APPLICABLE, DENIED
    },
    { (uint32_t) &((RNG_TypeDef *)RNG_BASE)->SR, "RNG IP SR", TEST_READ_PERIPH, RNG_TEST, SILENT},
    { (uint32_t) &((RNG_TypeDef *)RNG_BASE)->DR, "RNG IP DR", TEST_READ_PERIPH, RNG_TEST, SILENT},
    { (uint32_t) &Test_Execute, "fun call", TEST_EXECUTE,  NOT_APPLICABLE, ALLOWED},
    { (uint32_t)(&BX_LR) + 1, "BX_LR const", TEST_EXECUTE, NOT_APPLICABLE, ALLOWED },
    { (uint32_t) &REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base) - 2, "Exec NPRIV RAM", TEST_WRITE_EXEC, NOT_APPLICABLE, DENIED},

#ifdef TFM_TEST_PRIV_PROTECTION
    { (uint32_t) &priv_test_table, "priv table", TEST_PRIV_WRITE_RAM, NOT_APPLICABLE, ALLOWED},
    { (uint32_t) S_ROM_ALIAS(FLASH_BL2_NVCNT_AREA_OFFSET), "BL2 NVM Begin", TEST_PRIV_READ_RAM, NOT_APPLICABLE, SILENT},
    { (uint32_t) S_ROM_ALIAS(FLASH_AREA_PERSO_OFFSET), "PERSO Begin", TEST_PRIV_READ_RAM, NOT_APPLICABLE, SILENT},
    { (uint32_t) S_ROM_ALIAS(FLASH_BL2_HDP_END-FLASH_AREA_IMAGE_SECTOR_SIZE), "HDP End-PageSize+1", TEST_PRIV_READ_RAM, NOT_APPLICABLE, SILENT},
    { (uint32_t) S_ROM_ALIAS(FLASH_BL2_HDP_END), "HDP End", TEST_PRIV_READ_RAM, NOT_APPLICABLE, SILENT},
    { (uint32_t) S_ROM_ALIAS(FLASH_BL2_HDP_END + 1), "HDP End+1", TEST_PRIV_READ_RAM, NOT_APPLICABLE, ALLOWED},
    { (uint32_t) BOOT_TFM_SHARED_DATA_BASE - 1, "BL2 SHARED-1", TEST_PRIV_WRITE_RAM, NOT_APPLICABLE, ALLOWED},
    { (uint32_t) BOOT_TFM_SHARED_DATA_BASE, "BL2 SHARED", TEST_PRIV_WRITE_RAM, NOT_APPLICABLE, DENIED},
    { (uint32_t) &Test_Execute, "fun call", TEST_PRIV_EXECUTE,  NOT_APPLICABLE, ALLOWED},
    { (uint32_t) &BX_LR + 1, "BX_LR const", TEST_PRIV_EXECUTE, NOT_APPLICABLE, ALLOWED },
    { (uint32_t) &REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base), "Read RAM", TEST_PRIV_READ_RAM, NOT_APPLICABLE, ALLOWED },
    { (uint32_t) &REGION_NAME(Image$$, TFM_APP_RW_STACK_END, $$Base), "Exec RAM", TEST_PRIV_WRITE_EXEC, NOT_APPLICABLE, DENIED},
#endif
    {0x00000000, "Execution successful", TEST_END}
};
#endif /*  !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U) */
/* buffer used for DMA test */
const  __attribute__((__aligned__(4))) uint32_t DMA_CONST_2[2] = {  0xfadabeef, 0xbababeef };
const  __attribute__((__aligned__(4))) uint32_t DMA_NULL[2] = {  0, 0 };
/* Automatic test : list of operation */
#if !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U)
const uint8_t aTestOperation[][20] = TESTTYPE_STRINGS;
const uint8_t aTestAccess[][20] =  ACCESS_STRINGS;
#endif /*  !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U) */
/**
  * @}
  */

/** @defgroup  TEST_PROTECTIONS_Private_Functions Private Functions
  * @{
  */
#if !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U)
static void TEST_PROTECTIONS_PrintTestingMenu(void);
#ifdef TFM_PARTITION_APP_ROT
static TestStatus TEST_PROTECTION_RUN_PrivSecMem(uint32_t test_idx);
#endif /* TFM_PARTITION_APP_ROT */
#endif /* !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U) */
static TestStatus set_periph_clk(PeriphTest periph);
static TestStatus do_test(uint32_t test_idx);
#ifdef TFM_TEST_PRIV_PROTECTION
static enum tfm_platform_err_t test_priv_read(uint32_t address, uint32_t length, void *buffer);
static enum tfm_platform_err_t test_priv_write(uint32_t address, uint32_t length, void *buffer);
static enum tfm_platform_err_t test_priv_write_execute(uint32_t address, uint32_t length, void *buffer);
#endif
static TestStatus dma_m2m(void *to, void *from, size_t n);
/**
  * @}
  */

/** @defgroup  TEST_PROTECTIONS_Exported_Functions Exported Functions
  * @{
  */

/** @defgroup  TEST_PROTECTIONS_Control_Functions Control Functions
  * @{
  */
#if defined(TFM_PARTITION_APP_ROT) && (!defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U))
/**
  * @brief Test Access on Secure priv Area from Secure Npriv
  * @param None.
  * @retval None.
  */
static TestStatus TEST_PROTECTION_RUN_PrivSecMem(uint32_t test_idx)
{
    TestStatus status = TEST_IN_PROGRESS;
    enum tfm_app_rot_err_t ret;
    TestProtection_t test_desc;
    uint32_t test_secure_idx;
    psa_invec input;
    psa_outvec output;
    test_secure_idx = test_idx - NS_MAX_TEST;
    output.base = &test_desc;
    output.len = sizeof(test_desc);
    input.base = &test_secure_idx;
    input.len = sizeof(test_secure_idx);
    /* request test descriptor to display test on console*/
    ret = tfm_app_rot_ioctl((tfm_app_rot_ioctl_req_t)IOCTL_TEST_PROTECTION_DESC,
                            &input, &output);
    if (ret == TFM_APP_ROT_ERR_SUCCESS)
    {
        printf("\r\n= [TEST_S %d] %s @ %s %08x %s\r\n",(int)test_secure_idx, \
               aTestOperation[test_desc.type], test_desc.msg,(unsigned int)test_desc.address, aTestAccess[test_desc.access]);
        /* handle information about access to modify Test */
        if (test_desc.access == ALLOWED)
        {
            TestNumber = TEST_PROTECTION_FAILED | ((test_idx + 1) & 0xffff);
        }
        else
            TestNumber = TEST_PROTECTION_MASK | ((test_idx + 1) & 0xffff);
    }
    else  printf("\r\n= [TEST_S %d] : wrong descriptor\r\n",(int)test_secure_idx);
    output.base = NULL;
    output.len = 0;
    input.base = &test_secure_idx;
    input.len = sizeof(test_secure_idx);
    /* now execute the test */
    ret = tfm_app_rot_ioctl((tfm_app_rot_ioctl_req_t)IOCTL_TEST_PROTECTION_RUN_TEST,
                            &input, &output);
    switch (ret)
    {
        case TFM_APP_ROT_ERR_NOT_SUPPORTED:
            status = TEST_COMPLETED;
            break;
        case TFM_APP_ROT_ERR_INVALID_PARAM:
            printf("\r\n= [TEST_S] : Wrong parameter\r\n");
            status = TEST_ERROR;
            break;
        case TFM_APP_ROT_ERROR:
            printf("\r\n= [TEST_S] : Failed\r\n");
            status = TEST_ERROR;
            break;
        case TFM_APP_ROT_ERR_SUCCESS:
            status = TEST_IN_PROGRESS;
            break;
        default:
            printf("\r\n= [TEST_S] : Return value unknown\r\n");
            status = TEST_ERROR;
            break;
    }
    return status;
}
#endif /* defined(TFM_PARTITION_APP_ROT) && (!defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U)) */
/**
  * @brief  Display the TEST Main Menu choices on HyperTerminal
  * @param  Testnumber giving the test that generate a reset
  * @retval None.
  */
#if  !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U)
void TEST_PROTECTIONS_Run(void)
{
    uint8_t key = 0U;
    uint8_t exit = 0U;

    /*-1- Print Main Menu message*/
    TEST_PROTECTIONS_PrintTestingMenu();

    while (exit == 0U)
    {
        key = 0U;

        /* Clean the input path */
        COM_Flush();

        /* Receive key */
        if (COM_Receive(&key, 1U, RX_TIMEOUT) == HAL_OK)
        {
            switch (key)
            {
                case '1' :
                    TEST_PROTECTIONS_Run_SecUserMem();
                    break;
                case 'x' :
                    exit = 1U;
                    break;

                default:
                    printf("Invalid Number !\r");
                    break;
            }

            /* Print Main Menu message */
            TEST_PROTECTIONS_PrintTestingMenu();
        }
    }
}

/**
  * @brief Test Access on Secure Area
  * @param None.
  * @retval None.
  */
void TEST_PROTECTIONS_Run_SecUserMem(void)
{
    uint32_t test_idx;
    TestStatus status = TEST_IN_PROGRESS;
    TestProtection_t *test_desc;

    if ((TestNumber & TEST_PROTECTION_FAILED) == TEST_PROTECTION_FAILED)
    {
        status = TEST_ERROR;
        /* change status to possibly continue at next launch */
        test_idx = TestNumber & 0xffff;
        TestNumber = TEST_PROTECTION_MASK | test_idx;
    }
    if (((TestNumber & TEST_PROTECTION_MASK) == 0)
       /* Take into account TestNumber value after Flash or Power Off */
       || ((TestNumber & 0xffff0000) != TEST_PROTECTION_MASK))
    {
        /* test begin */
        TestNumber = TEST_PROTECTION_MASK;
    }

    while (status == TEST_IN_PROGRESS)
    {
        /* slow down execution */

        HAL_Delay(100);
        test_idx = TestNumber & 0xffff;
        if (((test_idx < NS_MAX_TEST) && (test_idx > (sizeof(aProtectTests) / sizeof(TestProtection_t) -1))) ||
            (test_idx > (NS_MAX_TEST + S_MAX_TEST - 1)))
        {
            /* Test Number is out of range */

            TestNumber = 0;
            return;
        }

        if (test_idx < NS_MAX_TEST)
        {
            /*  update to next test  */
            TestNumber = TEST_PROTECTION_MASK | ((test_idx + 1) & 0xffff);
            test_desc = (TestProtection_t *)&aProtectTests[test_idx];
            printf("\r\n= [TEST_NS %d] %s @ %s %08x %s\r\n",(int)test_idx, \
                   aTestOperation[test_desc->type], test_desc->msg,(unsigned int)test_desc->address, aTestAccess[test_desc->access]);
            status = do_test(test_idx);
#if !defined(TFM_PARTITION_APP_ROT)
        }
#else
            if (status == TEST_COMPLETED)
            {
                /* set next test_number */
                TestNumber = TEST_PROTECTION_MASK | (NS_MAX_TEST + 1);
                /* call Secure test */
                test_idx = NS_MAX_TEST;
                status = TEST_PROTECTION_RUN_PrivSecMem(test_idx);
            }

        }
        else
        {
            status = TEST_PROTECTION_RUN_PrivSecMem(test_idx);
        }
#endif /* TFM_PARTITION_APP_ROT */
    }
    while (status == TEST_IN_PROGRESS);

    if (status == TEST_ERROR)
    {
        printf("\r\n TEST Protection : Failed");
    }
    else if (status == TEST_COMPLETED)
    {
        /* reset for next run (need a RESET of the platform to restart a session) */
        printf("\r\n TEST Protection : Passed");
        printf("\b\b\b\b\b\b\b\b");
        TestNumber = 0;            /* no more test in progress */
    }
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup  TEST_PROTECTION_Private_Functions Private Functions
  * @{
  */
/**
  * @brief  Display the TEST Main Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
static void TEST_PROTECTIONS_PrintTestingMenu(void)
{
    printf("\r\n=================== Test Menu ============================\r\n\n");
    printf("  Test Protection --------------------------------------- 1\r\n\n");
    printf("  Previous Menu ----------------------------------------- x\r\n\n");
}
#else /* secure interface called by APP Rot generic service */
enum tfm_app_rot_err_t app_rot_hal_ioctl(tfm_app_rot_ioctl_req_t request,
                                         psa_invec *in_vec,
                                         psa_outvec *out_vec)
{
    /* one vector in in_vect and one vector in out_vect an int32 */
    uint32_t test_idx;
    TestStatus status;
    TestProtection_t *test_desc;
    if ((!in_vec) || (in_vec->len != 4))
    {
        return TFM_APP_ROT_ERR_INVALID_PARAM;
    }
    test_idx = *((uint32_t *)in_vec->base);
    if (test_idx > (sizeof(aProtectTests) / sizeof(TestProtection_t) -1))
    {
        /* test index is too large */
        return TFM_APP_ROT_ERR_NOT_SUPPORTED;
    }
    if (request == IOCTL_TEST_PROTECTION_DESC)
    {
        /* check outvec */
        if ((out_vec) && (out_vec->base) && (out_vec->len == sizeof(TestProtection_t)))
        {
            test_desc = out_vec->base;
            memcpy(test_desc, &aProtectTests[test_idx], sizeof(TestProtection_t));
            return TFM_APP_ROT_ERR_SUCCESS;
        }
        else return  TFM_APP_ROT_ERR_INVALID_PARAM;
    }
    if (request == IOCTL_TEST_PROTECTION_RUN_TEST)
    {
        status = do_test(test_idx);
        if (status == TEST_COMPLETED)
        {
            return TFM_APP_ROT_ERR_NOT_SUPPORTED;
        }
        if (status == TEST_ERROR)
        {
            return TFM_APP_ROT_ERROR;
        }
        if (status == TEST_IN_PROGRESS)
        {
            return TFM_APP_ROT_ERR_SUCCESS;
        }

    }
    return TFM_APP_ROT_ERR_INVALID_PARAM;
}
#endif /* #if  !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U) */
/**
  * @brief  Enable periph clock require to perform test
  * @param  periph to test.
  * @retval enable 1 , disable 0.
  */
static TestStatus set_periph_clk(PeriphTest periph)
{
    TestStatus status = TEST_IN_PROGRESS;
    switch (periph)
    {
        case NOT_APPLICABLE:
            break;
        case RNG_TEST:
            if (!__HAL_RCC_RNG_IS_CLK_ENABLED())
            {
                __HAL_RCC_RNG_CONFIG(RCC_RNGCLKSOURCE_HSI48);
                __HAL_RCC_RNG_CLK_ENABLE();
            }
            if (!__HAL_RCC_RNG_IS_CLK_ENABLED())
            {
                status = TEST_ERROR;
            }
            break;
        case HASH_TEST:
            if (!__HAL_RCC_HASH_IS_CLK_ENABLED())
            {
                __HAL_RCC_RNG_CONFIG(RCC_RNGCLKSOURCE_HSI48);
                __HAL_RCC_RNG_CLK_ENABLE();
            }
            if (!__HAL_RCC_HASH_IS_CLK_ENABLED())
            {
                status = TEST_ERROR;
            }
            break;
        default:
            break;
    }
    return status;
}
#ifdef TFM_TEST_PRIV_PROTECTION
static enum tfm_platform_err_t test_priv_read(uint32_t address, uint32_t length, void *buffer)
{
    uint32_t data[2];
    psa_invec input;
    psa_outvec output;
    data[0] = address;
    data[1] = length;
    input.base = data;
    input.len = sizeof(data);
    output.base = buffer;
    output.len = length;
    return tfm_platform_ioctl(PLATFORM_IOCTL_TEST_READ, &input, &output);
}

static enum tfm_platform_err_t test_priv_write(uint32_t address, uint32_t length, void *buffer)
{
    /* maximum write is 8 bytes */
    uint32_t data[4];
    psa_invec input;
    data[0] = address;
    data[1] = length;
    if (length > (sizeof(data) - 2 * sizeof(uint32_t)))
    {
        length = sizeof(data) - 2 * sizeof(uint32_t);
    }
    input.base = data;
    input.len = length + 8;
    memcpy(&data[2], buffer, length);
    return tfm_platform_ioctl(PLATFORM_IOCTL_TEST_WRITE, &input, NULL);
}

static  enum tfm_platform_err_t test_priv_write_execute(uint32_t address,  uint32_t length, void *buffer)
{
    /* maximum write is 8 bytes */
    uint32_t data[2];
    psa_invec input;
    data[0] = address;
    if (length > (sizeof(data) - 1 * sizeof(uint32_t)))
    {
        length = sizeof(data) - 1 * sizeof(uint32_t);
    }
    input.base = &data;
    input.len = length + sizeof(uint32_t);
    if (length)
    {
        memcpy(&data[1], buffer, length);
    }
    return tfm_platform_ioctl(PLATFORM_IOCTL_TEST_WRITE_EXEC, &input, NULL);
}
#endif /* TFM_TEST_PRIV_PROTECTION */
/**
  * @brief  perform test
  * @param  test index
  * @retval TestStatus
  */
static TestStatus do_test(uint32_t test_idx)
{
    /* flash write is not supported in secure */
    uint32_t flashErrCode = 0;
    uint64_t pattern[2] = {0U};
#ifdef TFM_TEST_PRIV_PROTECTION
    uint8_t byte_value = 0xfa;
    uint8_t byte = 0xde;
#endif /* TFM_TEST_PRIV_PROTECTION */
    uint32_t page_error = 0U;
    FLASH_EraseInitTypeDef p_erase_init;
    TestStatus status = TEST_IN_PROGRESS;
    __IO uint8_t tmp;
    __IO uint32_t tmp_reg;
    void (*func)(void);
    uint32_t loop;
    /* used to try RAM execution */
    uint16_t DataVal;
    __IO uint16_t *pData;
    uint32_t dmabuff[] = {0xdeadbeef, 0xfadadead};
    void *dest;
    void *from;

    switch (aProtectTests[test_idx].type)
    {
        /* Trying to read in FLASH : RDP set in case of PCROP protected area
           or reset generated if under FWALL or MPU protection */
        case TEST_READ_FLASH :
            tmp = *(uint8_t *)(aProtectTests[test_idx].address);
#if  !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U)
            HAL_Delay(1);        /* ensure Flag is set */
#endif
            if ((aProtectTests[test_idx].access == DENIED) ||
                ((aProtectTests[test_idx].access == SILENT) && (tmp)))
            {
                status = TEST_ERROR;
            }
            break;
        /* Trying to write in RAM  */
        case TEST_WRITE_RAM :
            *(uint32_t *)aProtectTests[test_idx].address = 0x00000000;
            if (aProtectTests[test_idx].access == DENIED)
            {
                status = TEST_ERROR;
            }
            break;

        /* Trying to read in RAM  */
        case TEST_READ_RAM :
            tmp = *(uint8_t *)aProtectTests[test_idx].address;
            if ((aProtectTests[test_idx].access == DENIED) ||
                ((aProtectTests[test_idx].access == SILENT) && (tmp)))
            {
                status = TEST_ERROR;
            }

            break;
        /* Tryning to execute code  */
        case TEST_EXECUTE :
            func = (void(*)(void))(aProtectTests[test_idx].address);
            func();
            switch (aProtectTests[test_idx].access)
            {
                case ALLOWED:
                    break;
                default:
                    status = TEST_ERROR;
            }
            break;
        case TEST_WRITE_EXEC :
            /*  address code alignment check*/
            if (aProtectTests[test_idx].address & 1)
            {
                status = TEST_ERROR;
                break;
            }
            pData = (uint16_t *)aProtectTests[test_idx].address;
            DataVal = *pData;
            *pData = BX_LR;
            /* Flush and refill pipeline  */
            __DSB();
            __ISB();
            func = (void(*)(void))(aProtectTests[test_idx].address + 1);
            func();
            *pData = DataVal;
            switch (aProtectTests[test_idx].access)
            {
                case ALLOWED:
                    break;
                default:
                    status = TEST_ERROR;
            }
            break;

        /* Tryning to read peripheral code  */
        case TEST_READ_PERIPH :
            status = set_periph_clk(aProtectTests[test_idx].periph);
            if (status == TEST_IN_PROGRESS)
            {
                loop = MAX_PERIPH_READ;
                do
                {
                    loop--;
                    tmp_reg = *((__IO uint32_t *)aProtectTests[test_idx].address);
                } while (loop && tmp_reg);
                if ((aProtectTests[test_idx].access == DENIED) ||
                    ((aProtectTests[test_idx].access == SILENT) && (tmp_reg)))
                {
                    status = TEST_ERROR;
                }
            }
            break;
        case TEST_WRITE_PERIPH :
            status = set_periph_clk(aProtectTests[test_idx].periph);
            if (status == TEST_IN_PROGRESS)
            {
                *((__IO uint32_t *)aProtectTests[test_idx].address) = 0x0;
                /* error on writing 0 is silent*/
                if (aProtectTests[test_idx].access == DENIED)
                {
                    status = TEST_ERROR;
                }
            }
            break;
        case TEST_WRITE_DMA :
            dest = (void *)aProtectTests[test_idx].address;
            status = dma_m2m((void *)dest, (void *)DMA_CONST_2, sizeof(DMA_CONST_2));
            if (status == TEST_IN_PROGRESS)
            {
                switch (aProtectTests[test_idx].access)
                {
                    case ALLOWED:
                        if (memcmp(dest, DMA_CONST_2, sizeof(DMA_CONST_2)))
                        {
                            status = TEST_ERROR;
                        }
                        break;
                    case SILENT:
#ifdef   TFM_TEST_PRIV_PROTECTION
                        /*  use platform service to check access was not  performed  */
                        if (test_priv_read(aProtectTests[test_idx].address, sizeof(DMA_CONST_2), dmabuff) !=  TFM_PLATFORM_ERR_SUCCESS)
                        {
                            status = TEST_ERROR;
                            break;

                        }
                        /*  if content is the same access, dma access has been done */
                        if (!memcmp(dmabuff, DMA_CONST_2, sizeof(DMA_CONST_2)))
                        {
                            status = TEST_ERROR;
                        }
#else
                        /* as unprivileged access are silent , no check is possible , unless a platform memcmp service is provided */
                        /* test is considered as passed */
#endif
                        break;
                    case DENIED:
                    default:
                        status = TEST_ERROR;
                }
            }
            break;
        case TEST_READ_DMA :
            from = (void *)aProtectTests[test_idx].address;
            status = dma_m2m((void *)dmabuff, from, sizeof(dmabuff));
            if (status == TEST_IN_PROGRESS)
            {
                switch (aProtectTests[test_idx].access)
                {
                    case ALLOWED:
                        if (memcmp(from, dmabuff, sizeof(dmabuff)))
                        {
                            status = TEST_ERROR;
                        }
                        break;
                    case SILENT:
                        if (memcmp(DMA_NULL, dmabuff, sizeof(DMA_NULL)))
                        {
                            status = TEST_ERROR;
                        }
                        break;
                    default:
                        status = TEST_ERROR;
                }
            }
            break;
        case TEST_WRITE_FLASH :
            HAL_FLASH_Unlock();
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, aProtectTests[test_idx].address, (uint32_t)&pattern);
            HAL_FLASH_Lock();
            flashErrCode = HAL_FLASH_GetError();
#if  !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U)
            printf("\r\nflash error code: %x", (int)flashErrCode);
#endif
            if ((flashErrCode & HAL_FLASH_ERROR_WRP) == 0U)
            {
                status = TEST_ERROR;
            }

            /*
             * OBs protection test: MPU generates a MEMORY FAULT and a RESET
             * So the test is FAILED if we reach this line
             */
            if (0 == strncmp("OBs @", (const char *)aProtectTests[test_idx].msg, 5))
            {
                status = TEST_ERROR;
            }
            /* else not an OB protection test so probably not an error */
            break;

        /* Trying to erase 512 bytes in FLASH : WRP flag set for WRP or PCROP protected area
           or reset generated if under FWALL or MPU protection */
        case TEST_ERASE_FLASH :
            HAL_FLASH_Unlock();
            p_erase_init.Banks       = FLASH_BANK_1;
            p_erase_init.TypeErase   = FLASH_TYPEERASE_PAGES;
            p_erase_init.Page        = (aProtectTests[test_idx].address) / 0x800;
            p_erase_init.NbPages     = 1;
            HAL_FLASHEx_Erase(&p_erase_init, &page_error);
            HAL_FLASH_Lock();
            if ((HAL_FLASH_GetError() & HAL_FLASH_ERROR_WRP) == 0U)
            {
                status = TEST_ERROR;
            }
            break;
            /* End of execution */
#ifdef TFM_TEST_PRIV_PROTECTION
        case  TEST_PRIV_WRITE_RAM :
            if (aProtectTests[test_idx].access == ALLOWED)
                /* if access is allowed , we can read the initial value, and check it has been modified */
            {
                test_priv_read(aProtectTests[test_idx].address, 1, &byte_value);
            }
            if (test_priv_write(aProtectTests[test_idx].address, 1, &byte) != TFM_PLATFORM_ERR_SUCCESS)
            {
#if  !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U)
                printf("\r\npriv_write_byte %x error ", (int)aProtectTests[test_idx].address);
#endif
                status = TEST_ERROR;
                break;
            }
            switch (aProtectTests[test_idx].access)
            {
                case ALLOWED:
                    /* read to check area modification */
                    byte = 0x0;
                    test_priv_read(aProtectTests[test_idx].address, 1, &byte);
                    if (byte != 0xde)
                    {
                        status = TEST_ERROR;
                    }
                    /* put back previous value */
                    test_priv_write(aProtectTests[test_idx].address, 1, &byte_value);
                    break;
                case SILENT:
                    test_priv_read(aProtectTests[test_idx].address, 1, &byte);
                    if (byte == 0xde)
                    {
                        status = TEST_ERROR;
                    }
                    break;
                default:
                    status = TEST_ERROR;
            }
            break;
        case  TEST_PRIV_READ_RAM:
            if (test_priv_read(aProtectTests[test_idx].address, 1, &byte) != TFM_PLATFORM_ERR_SUCCESS)
            {
#if  !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U)
                printf("\r\npriv_write_byte %x error ", (int)aProtectTests[test_idx].address);
#endif
                status = TEST_ERROR;
                break;
            }
            switch (aProtectTests[test_idx].access)
            {
                case ALLOWED:
                    break;
                case SILENT:
                    if (byte != 0x0)
                    {
                        status = TEST_ERROR;
                    }
                    break;
                default:
                    status = TEST_ERROR;
            }
            break;
        case TEST_PRIV_EXECUTE:
            if (test_priv_write_execute(aProtectTests[test_idx].address, 0, NULL) != TFM_PLATFORM_ERR_SUCCESS)
            {
#if  !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U)
                printf("\r\npriv_write_exec %x error ", (int)aProtectTests[test_idx].address);
#endif
                status = TEST_ERROR;
                break;
            }
            switch (aProtectTests[test_idx].access)
            {
                case ALLOWED:
                    break;
                default:
                    status = TEST_ERROR;
            }
            break;
        case TEST_PRIV_WRITE_EXEC :
            /*  a thumb instruction is written, address must be aligned  */
            if (aProtectTests[test_idx].address & 1)
            {
                status = TEST_ERROR;
                break;
            }

            if (test_priv_write_execute(aProtectTests[test_idx].address, sizeof(BX_LR), (void *)&BX_LR) != TFM_PLATFORM_ERR_SUCCESS)
            {
#if  !defined (__ARM_FEATURE_CMSE) || (__ARM_FEATURE_CMSE != 3U)
                printf("\r\npriv_write_exec %x error ", (int)aProtectTests[test_idx].address);
#endif
                status = TEST_ERROR;
                break;
            }
            switch (aProtectTests[test_idx].access)
            {
                case ALLOWED:
                    break;
                default:
                    status = TEST_ERROR;
            }
            break;


#endif
        case TEST_END :
            status = TEST_COMPLETED;
            break;

        default :
            break;
    }
    return status;
}
/**
  * @brief  GPDMA transaction mem to mem
  * @param  to : memory destination.
  * @param  from : memory source.
  * @param  n : transaction size in bytes
  * @retval TestStatus
  */
static TestStatus dma_m2m(void *to, void *from, size_t n)
{

    DMA_HandleTypeDef DMAHandle;
    DMA_QListTypeDef  Queue;
    __HAL_RCC_GPDMA1_CLK_ENABLE();
#if  defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    DMAHandle.Instance                    = GPDMA1_Channel0;
#else
    DMAHandle.Instance                    = GPDMA1_Channel1;
#endif /* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */
    /* Set parameter to be configured */
    DMAHandle.Init.Request                = DMA_REQUEST_SW;
    DMAHandle.Init.BlkHWRequest           = DMA_BREQ_SINGLE_BURST;
    DMAHandle.Init.Direction              = DMA_MEMORY_TO_MEMORY;
    DMAHandle.Init.SrcInc                 = DMA_SINC_INCREMENTED;
    DMAHandle.Init.DestInc                = DMA_DINC_INCREMENTED;
    DMAHandle.Init.SrcDataWidth           = DMA_SRC_DATAWIDTH_WORD;
    DMAHandle.Init.DestDataWidth          = DMA_DEST_DATAWIDTH_WORD;
    DMAHandle.Init.SrcBurstLength         = 1;
    DMAHandle.Init.DestBurstLength        = 1;
    DMAHandle.Init.Priority               = DMA_LOW_PRIORITY_HIGH_WEIGHT;
    DMAHandle.Init.TransferEventMode      = DMA_TCEM_BLOCK_TRANSFER;
    DMAHandle.Init.TransferAllocatedPort  = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
    DMAHandle.Mode                        = DMA_NORMAL;
    DMAHandle.LinkedListQueue             = &Queue;

    /* Initialize the DMA channel */
    if (HAL_DMA_Init(&DMAHandle) != HAL_OK)
    {
        return TEST_ERROR;
    }
#if  defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    if (HAL_DMA_ConfigChannelAttributes(&DMAHandle,
                                        DMA_CHANNEL_SEC | DMA_CHANNEL_NPRIV | DMA_CHANNEL_SRC_SEC | DMA_CHANNEL_DEST_SEC) != HAL_OK)
    {
        return TEST_ERROR;
    }
#endif/* defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */
    if (HAL_DMA_Start(&DMAHandle, (uint32_t)from, (uint32_t)to, n) != HAL_OK)
    {
        return TEST_ERROR;
    }
    if (HAL_DMA_PollForTransfer(&DMAHandle, HAL_DMA_FULL_TRANSFER, 3000UL) != HAL_OK)
    {
        return TEST_ERROR;
    }
    return TEST_IN_PROGRESS;
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /*defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) && !defined(TFM_PARTITION_APP_ROT) */
