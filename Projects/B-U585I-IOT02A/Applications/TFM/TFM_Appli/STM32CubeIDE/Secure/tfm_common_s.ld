;/*
; * Copyright (c) 2009-2019 Arm Limited
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; *     http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS,
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; *
; *
; * This file is derivative of CMSIS V5.00 gcc_arm.ld
; */

/*********** WARNING: This is an auto-generated file. Do not edit! ***********/

/* Linker script to configure memory regions. */
/* This file will be run trough the pre-processor. */

#include "region_defs.h"

MEMORY
{
  FLASH   (rx)  : ORIGIN = S_CODE_START, LENGTH = S_CODE_SIZE
  RAM     (rwx) : ORIGIN = S_DATA_START, LENGTH = S_DATA_SIZE
#if !TFM_MULTI_CORE_TOPOLOGY
  VENEERS (rx)  : ORIGIN = CMSE_VENEER_REGION_START, LENGTH = CMSE_VENEER_REGION_SIZE
#endif
}

__heap_size__  = S_HEAP_SIZE;
__psp_stack_size__ = S_PSP_STACK_SIZE;
__msp_init_stack_size__ = S_MSP_STACK_SIZE_INIT;

/* Library configurations */
GROUP(libgcc.a libc.a libm.a libnosys.a libc_nano.a)

ENTRY(Reset_Handler)

SECTIONS
{
    .TFM_VECTORS : ALIGN(4)
    {
        __vectors_start__ = .;
        KEEP(*(.vectors))
        *startup*(.text*)
        . = ALIGN(4);
        __vectors_end__ = .;
    } > FLASH

    .copy.table : ALIGN(4)
    {
        __copy_table_start__ = .;
        LONG (LOADADDR(.TFM_DATA))
        LONG (ADDR(.TFM_DATA))
        LONG (SIZEOF(.TFM_DATA)/4)
        
        LONG (LOADADDR(.TFM_UNPRIV_DATA))
        LONG (ADDR(.TFM_UNPRIV_DATA))
        LONG (SIZEOF(.TFM_UNPRIV_DATA)/4)
        
        LONG (LOADADDR(.TFM_SP_STORAGE_DATA))
        LONG (ADDR(.TFM_SP_STORAGE_DATA))
        LONG (SIZEOF(.TFM_SP_STORAGE_DATA)/4)
        
        LONG (LOADADDR(.TFM_SP_ITS_DATA))
        LONG (ADDR(.TFM_SP_ITS_DATA))
        LONG (SIZEOF(.TFM_SP_ITS_DATA)/4)

        LONG (LOADADDR(.TFM_SP_CRYPTO_DATA))
        LONG (ADDR(.TFM_SP_CRYPTO_DATA))
        LONG (SIZEOF(.TFM_SP_CRYPTO_DATA)/4)

        LONG (LOADADDR(.TFM_SP_PLATFORM_DATA))
        LONG (ADDR(.TFM_SP_PLATFORM_DATA))
        LONG (SIZEOF(.TFM_SP_PLATFORM_DATA)/4)

        LONG (LOADADDR(.TFM_SP_INITIAL_ATTESTATION_DATA))
        LONG (ADDR(.TFM_SP_INITIAL_ATTESTATION_DATA))
        LONG (SIZEOF(.TFM_SP_INITIAL_ATTESTATION_DATA)/4)
        
#if defined (TFM_PARTITION_APP_ROT)
        LONG (LOADADDR(.TFM_APP_ROT_DATA))
        LONG (ADDR(.TFM_APP_ROT_DATA))
        LONG (SIZEOF(.TFM_APP_ROT_DATA)/4)
#endif /* TFM_PARTITION_APP_ROT */

#ifdef TFM_PARTITION_TEST_CORE
        LONG (LOADADDR(.TFM_SP_CORE_TEST_DATA))
        LONG (ADDR(.TFM_SP_CORE_TEST_DATA))
        LONG (SIZEOF(.TFM_SP_CORE_TEST_DATA)/4)
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
        LONG (LOADADDR(.TFM_SP_CORE_TEST_2_DATA))
        LONG (ADDR(.TFM_SP_CORE_TEST_2_DATA))
        LONG (SIZEOF(.TFM_SP_CORE_TEST_2_DATA)/4)
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
        LONG (LOADADDR(.TFM_SP_SECURE_TEST_PARTITION_DATA))
        LONG (ADDR(.TFM_SP_SECURE_TEST_PARTITION_DATA))
        LONG (SIZEOF(.TFM_SP_SECURE_TEST_PARTITION_DATA)/4)
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
        LONG (LOADADDR(.TFM_SP_IPC_SERVICE_TEST_DATA))
        LONG (ADDR(.TFM_SP_IPC_SERVICE_TEST_DATA))
        LONG (SIZEOF(.TFM_SP_IPC_SERVICE_TEST_DATA)/4)
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
        LONG (LOADADDR(.TFM_SP_IPC_CLIENT_TEST_DATA))
        LONG (ADDR(.TFM_SP_IPC_CLIENT_TEST_DATA))
        LONG (SIZEOF(.TFM_SP_IPC_CLIENT_TEST_DATA)/4)
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
        LONG (LOADADDR(.TFM_IRQ_TEST_1_DATA))
        LONG (ADDR(.TFM_IRQ_TEST_1_DATA))
        LONG (SIZEOF(.TFM_IRQ_TEST_1_DATA)/4)
#endif /* TFM_ENABLE_IRQ_TEST */

        __copy_table_end__ = .;
    } > FLASH

    .zero.table : ALIGN(4)
    {
        __zero_table_start__ = .;
        LONG (ADDR(.TFM_BSS))
        LONG (SIZEOF(.TFM_BSS)/4)
                
        LONG (ADDR(.TFM_SP_STORAGE_BSS))
        LONG (SIZEOF(.TFM_SP_STORAGE_BSS)/4)
        LONG (ADDR(.TFM_SP_STORAGE_STACK))
        LONG (SIZEOF(.TFM_SP_STORAGE_STACK)/4)
        
        LONG (ADDR(.TFM_SP_ITS_BSS))
        LONG (SIZEOF(.TFM_SP_ITS_BSS)/4)
        LONG (ADDR(.TFM_SP_ITS_STACK))
        LONG (SIZEOF(.TFM_SP_ITS_STACK)/4)
        
        LONG (ADDR(.TFM_SP_CRYPTO_BSS))
        LONG (SIZEOF(.TFM_SP_CRYPTO_BSS)/4)
        LONG (ADDR(.TFM_SP_CRYPTO_STACK))
        LONG (SIZEOF(.TFM_SP_CRYPTO_STACK)/4)
    

        LONG (ADDR(.TFM_SP_PLATFORM_BSS))
        LONG (SIZEOF(.TFM_SP_PLATFORM_BSS)/4)
        LONG (ADDR(.TFM_SP_PLATFORM_STACK))
        LONG (SIZEOF(.TFM_SP_PLATFORM_STACK)/4)


        LONG (ADDR(.TFM_SP_INITIAL_ATTESTATION_BSS))
        LONG (SIZEOF(.TFM_SP_INITIAL_ATTESTATION_BSS)/4)
        LONG (ADDR(.TFM_SP_INITIAL_ATTESTATION_STACK))
        LONG (SIZEOF(.TFM_SP_INITIAL_ATTESTATION_STACK)/4)

#if defined (TFM_PARTITION_APP_ROT)
        LONG (ADDR(.TFM_APP_ROT_BSS))
        LONG (SIZEOF(.TFM_APP_ROT_BSS)/4)
        LONG (ADDR(.TFM_APP_ROT_STACK))
        LONG (SIZEOF(.TFM_APP_ROT_STACK)/4)
#endif /* TFM_PARTITION_APP_ROT */

#ifdef TFM_PARTITION_TEST_CORE
        LONG (ADDR(.TFM_SP_CORE_TEST_BSS))
        LONG (SIZEOF(.TFM_SP_CORE_TEST_BSS)/4)
        LONG (ADDR(.TFM_SP_CORE_TEST_STACK))
        LONG (SIZEOF(.TFM_SP_CORE_TEST_STACK)/4)
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE
        LONG (ADDR(.TFM_SP_CORE_TEST_2_BSS))
        LONG (SIZEOF(.TFM_SP_CORE_TEST_2_BSS)/4)
        LONG (ADDR(.TFM_SP_CORE_TEST_2_STACK))
        LONG (SIZEOF(.TFM_SP_CORE_TEST_2_STACK)/4)
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
        LONG (ADDR(.TFM_SP_SECURE_TEST_PARTITION_BSS))
        LONG (SIZEOF(.TFM_SP_SECURE_TEST_PARTITION_BSS)/4)
        LONG (ADDR(.TFM_SP_SECURE_TEST_PARTITION_STACK))
        LONG (SIZEOF(.TFM_SP_SECURE_TEST_PARTITION_STACK)/4)
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
        LONG (ADDR(.TFM_SP_IPC_SERVICE_TEST_BSS))
        LONG (SIZEOF(.TFM_SP_IPC_SERVICE_TEST_BSS)/4)
        LONG (ADDR(.TFM_SP_IPC_SERVICE_TEST_STACK))
        LONG (SIZEOF(.TFM_SP_IPC_SERVICE_TEST_STACK)/4)
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_PARTITION_TEST_CORE_IPC
        LONG (ADDR(.TFM_SP_IPC_CLIENT_TEST_BSS))
        LONG (SIZEOF(.TFM_SP_IPC_CLIENT_TEST_BSS)/4)
        LONG (ADDR(.TFM_SP_IPC_CLIENT_TEST_STACK))
        LONG (SIZEOF(.TFM_SP_IPC_CLIENT_TEST_STACK)/4)
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
        LONG (ADDR(.TFM_IRQ_TEST_1_BSS))
        LONG (SIZEOF(.TFM_IRQ_TEST_1_BSS)/4)
        LONG (ADDR(.TFM_IRQ_TEST_1_STACK))
        LONG (SIZEOF(.TFM_IRQ_TEST_1_STACK)/4)
#endif /* TFM_ENABLE_IRQ_TEST */
        __zero_table_end__ = .;
    } > FLASH

    .TFM_SP_STORAGE : ALIGN(32)
    {
        *sst*:*(.text*)
        *sst*:*(.rodata*)
        *tfm_protected_storage*:*(.text*)
        *tfm_protected_storage*:*(.rodata*)
        *(TFM_SP_STORAGE_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_STORAGE$$RO$$Base = ADDR(.TFM_SP_STORAGE);
    Image$$TFM_SP_STORAGE$$RO$$Limit = ADDR(.TFM_SP_STORAGE) + SIZEOF(.TFM_SP_STORAGE);
    Image$$TFM_SP_STORAGE$$Base = ADDR(.TFM_SP_STORAGE);
    Image$$TFM_SP_STORAGE$$Limit = ADDR(.TFM_SP_STORAGE) + SIZEOF(.TFM_SP_STORAGE);

    .TFM_SP_ITS : ALIGN(32)
    {
        *internal_trusted_storage*:*(.text*)
        *internal_trusted_storage*:*(.rodata*)
        *its*:*(.text*)
        *its*:*(.rodata*)
        *(TFM_SP_ITS_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_ITS$$RO$$Base = ADDR(.TFM_SP_ITS);
    Image$$TFM_SP_ITS$$RO$$Limit = ADDR(.TFM_SP_ITS) + SIZEOF(.TFM_SP_ITS);
    Image$$TFM_SP_ITS$$Base = ADDR(.TFM_SP_ITS);
    Image$$TFM_SP_ITS$$Limit = ADDR(.TFM_SP_ITS) + SIZEOF(.TFM_SP_ITS);
    .TFM_SP_CRYPTO : ALIGN(32)
    {
        crypto*:*(.text*)
        crypto*:*(.rodata*)
        platform*:*(.text*)
        platform*:*(.rodata*)
        platform_util*:*(.text*)
        platform_util*:*(.rodata*)
        *(TFM_SP_CRYPTO_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_CRYPTO$$RO$$Base = ADDR(.TFM_SP_CRYPTO);
    Image$$TFM_SP_CRYPTO$$RO$$Limit = ADDR(.TFM_SP_CRYPTO) + SIZEOF(.TFM_SP_CRYPTO);
    Image$$TFM_SP_CRYPTO$$Base = ADDR(.TFM_SP_CRYPTO);
    Image$$TFM_SP_CRYPTO$$Limit = ADDR(.TFM_SP_CRYPTO) + SIZEOF(.TFM_SP_CRYPTO);

    .TFM_SP_INITIAL_ATTESTATION : ALIGN(32)
    {
        *tfm_attestation*:*(.text*)
        *tfm_attestation*:*(.rodata*)
        *(TFM_SP_INITIAL_ATTESTATION_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_INITIAL_ATTESTATION$$RO$$Base = ADDR(.TFM_SP_INITIAL_ATTESTATION);
    Image$$TFM_SP_INITIAL_ATTESTATION$$RO$$Limit = ADDR(.TFM_SP_INITIAL_ATTESTATION) + SIZEOF(.TFM_SP_INITIAL_ATTESTATION);
    Image$$TFM_SP_INITIAL_ATTESTATION$$Base = ADDR(.TFM_SP_INITIAL_ATTESTATION);
    Image$$TFM_SP_INITIAL_ATTESTATION$$Limit = ADDR(.TFM_SP_INITIAL_ATTESTATION) + SIZEOF(.TFM_SP_INITIAL_ATTESTATION);

    .TFM_SP_CRYPTO : ALIGN(32)
    {
        *crypto*:*(.text*)
        *crypto*:*(.rodata*)
        platform*:*(.text*)
        platform*:*(.rodata*)
        platform_util*:*(.text*)
        platform_util*:*(.rodata*)    
        *(TFM_SP_CRYPTO_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_CRYPTO$$RO$$Base = ADDR(.TFM_SP_CRYPTO);
    Image$$TFM_SP_CRYPTO$$RO$$Limit = ADDR(.TFM_SP_CRYPTO) + SIZEOF(.TFM_SP_CRYPTO);
    Image$$TFM_SP_CRYPTO$$Base = ADDR(.TFM_SP_CRYPTO);
    Image$$TFM_SP_CRYPTO$$Limit = ADDR(.TFM_SP_CRYPTO) + SIZEOF(.TFM_SP_CRYPTO);


    .TFM_SP_PLATFORM : ALIGN(32)
    {
        *tfm_platform_system.o(.text*)
        *tfm_platform_system.o(.rodata*)
        *platform_sp.o(.text*)
        *platform_sp.o(.rodata*)
        *(TFM_SP_PLATFORM_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_PLATFORM$$RO$$Base = ADDR(.TFM_SP_PLATFORM);
    Image$$TFM_SP_PLATFORM$$RO$$Limit = ADDR(.TFM_SP_PLATFORM) + SIZEOF(.TFM_SP_PLATFORM);
    Image$$TFM_SP_PLATFORM$$Base = ADDR(.TFM_SP_PLATFORM);
    Image$$TFM_SP_PLATFORM$$Limit = ADDR(.TFM_SP_PLATFORM) + SIZEOF(.TFM_SP_PLATFORM);


    .TFM_SP_INITIAL_ATTESTATION : ALIGN(32)
    {
        *tfm_attest*:*(.text*)
        *tfm_attest*:*(.rodata*)
        *(TFM_SP_INITIAL_ATTESTATION_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_INITIAL_ATTESTATION$$RO$$Base = ADDR(.TFM_SP_INITIAL_ATTESTATION);
    Image$$TFM_SP_INITIAL_ATTESTATION$$RO$$Limit = ADDR(.TFM_SP_INITIAL_ATTESTATION) + SIZEOF(.TFM_SP_INITIAL_ATTESTATION);
    Image$$TFM_SP_INITIAL_ATTESTATION$$Base = ADDR(.TFM_SP_INITIAL_ATTESTATION);
    Image$$TFM_SP_INITIAL_ATTESTATION$$Limit = ADDR(.TFM_SP_INITIAL_ATTESTATION) + SIZEOF(.TFM_SP_INITIAL_ATTESTATION);

#ifdef TFM_PARTITION_TEST_CORE
    .TFM_SP_CORE_TEST : ALIGN(32)
    {
        *tfm_ss_core_test.*(.text*)
        *tfm_ss_core_test.*(.rodata*)
        *(TFM_SP_CORE_TEST_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_CORE_TEST$$RO$$Base = ADDR(.TFM_SP_CORE_TEST);
    Image$$TFM_SP_CORE_TEST$$RO$$Limit = ADDR(.TFM_SP_CORE_TEST) + SIZEOF(.TFM_SP_CORE_TEST);
    Image$$TFM_SP_CORE_TEST$$Base = ADDR(.TFM_SP_CORE_TEST);
    Image$$TFM_SP_CORE_TEST$$Limit = ADDR(.TFM_SP_CORE_TEST) + SIZEOF(.TFM_SP_CORE_TEST);
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    .TFM_SP_SECURE_TEST_PARTITION : ALIGN(32)
    {
        *tfm_secure_client_service.*(.text*)
        *tfm_secure_client_service.*(.rodata*)
        *test_framework*(.text*)
        *test_framework*(.rodata*)
        *uart_stdout.*(.text*)
        *uart_stdout.*(.rodata*)
        *Driver_USART.*(.text*)
        *Driver_USART.*(.rodata*)
        *arm_uart_drv.*(.text*)
        *arm_uart_drv.*(.rodata*)
        *uart_pl011_drv.*(.text*)
        *uart_pl011_drv.*(.rodata*)
        *uart_cmsdk_drv*(.text*)
        *uart_cmsdk_drv*(.rodata*)
        *secure_suites.*(.text*)
        *secure_suites.*(.rodata*)
        *attestation_s_interface_testsuite.*(.text*)
        *attestation_s_interface_testsuite.*(.rodata*)
        *(TFM_SP_SECURE_TEST_PARTITION_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_SECURE_TEST_PARTITION$$RO$$Base = ADDR(.TFM_SP_SECURE_TEST_PARTITION);
    Image$$TFM_SP_SECURE_TEST_PARTITION$$RO$$Limit = ADDR(.TFM_SP_SECURE_TEST_PARTITION) + SIZEOF(.TFM_SP_SECURE_TEST_PARTITION);
    Image$$TFM_SP_SECURE_TEST_PARTITION$$Base = ADDR(.TFM_SP_SECURE_TEST_PARTITION);
    Image$$TFM_SP_SECURE_TEST_PARTITION$$Limit = ADDR(.TFM_SP_SECURE_TEST_PARTITION) + SIZEOF(.TFM_SP_SECURE_TEST_PARTITION);
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    .TFM_SP_IPC_SERVICE_TEST : ALIGN(32)
    {
        *ipc_service_test.*(.text*)
        *ipc_service_test.*(.rodata*)
        *(TFM_SP_IPC_SERVICE_TEST_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_IPC_SERVICE_TEST$$RO$$Base = ADDR(.TFM_SP_IPC_SERVICE_TEST);
    Image$$TFM_SP_IPC_SERVICE_TEST$$RO$$Limit = ADDR(.TFM_SP_IPC_SERVICE_TEST) + SIZEOF(.TFM_SP_IPC_SERVICE_TEST);
    Image$$TFM_SP_IPC_SERVICE_TEST$$Base = ADDR(.TFM_SP_IPC_SERVICE_TEST);
    Image$$TFM_SP_IPC_SERVICE_TEST$$Limit = ADDR(.TFM_SP_IPC_SERVICE_TEST) + SIZEOF(.TFM_SP_IPC_SERVICE_TEST);
#endif /* TFM_PARTITION_TEST_CORE_IPC */

    .TFM_UNPRIV_CODE : ALIGN(GTZC_FLASH_ALIGN)
    {
        tfm_spm_services*:*(.text*)
        tfm_spm_services*:*(.rodata*)
        tfm_*_secure_api*:*(.text*)
        tfm_*_secure_api*:*(.rodata*)
        *libc_nano*:*(.text*)
        *libc_nano*:*(.rodata*)
        *(SFN)
        *libgcc*:*(.text*)
        *libgcc*:*(.rodata*)
        *stm32u5xx_hal*:*(.text*)
        *stm32u5xx_hal*:*(.rodata*)
        *tick*:*(.text*)
        *tick*:*(.rodata*)
        . = ALIGN(32);
    } > FLASH

    Image$$TFM_UNPRIV_CODE$$RO$$Base = ADDR(.TFM_UNPRIV_CODE);
    Image$$TFM_UNPRIV_CODE$$RO$$Limit = ADDR(.TFM_UNPRIV_CODE) + SIZEOF(.TFM_UNPRIV_CODE);
#if defined (TFM_PARTITION_APP_ROT)
    .TFM_APP_ROT : ALIGN(32)
    {
        *app_rot*.o(.text*)
        *app_rot*.o(.rodata*)
    } > FLASH
    Image$$TFM_APP_ROT$$RO$$Base = ADDR(.TFM_APP_ROT);
    Image$$TFM_APP_ROT$$RO$$Limit = ADDR(.TFM_APP_ROT) + SIZEOF(.TFM_APP_ROT);
    Image$$TFM_APP_ROT$$Base = ADDR(.TFM_APP_ROT);
    Image$$TFM_APP_ROT$$Limit = ADDR(.TFM_APP_ROT) + SIZEOF(.TFM_APP_ROT);
#endif

#ifdef TFM_PARTITION_TEST_CORE
    .TFM_SP_CORE_TEST_2 : ALIGN(32)
    {
        *tfm_ss_core_test_2.*(.text*)
        *tfm_ss_core_test_2.*(.rodata*)
        *(TFM_SP_CORE_TEST_2_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_CORE_TEST_2$$RO$$Base = ADDR(.TFM_SP_CORE_TEST_2);
    Image$$TFM_SP_CORE_TEST_2$$RO$$Limit = ADDR(.TFM_SP_CORE_TEST_2) + SIZEOF(.TFM_SP_CORE_TEST_2);
    Image$$TFM_SP_CORE_TEST_2$$Base = ADDR(.TFM_SP_CORE_TEST_2);
    Image$$TFM_SP_CORE_TEST_2$$Limit = ADDR(.TFM_SP_CORE_TEST_2) + SIZEOF(.TFM_SP_CORE_TEST_2);
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    .TFM_SP_IPC_CLIENT_TEST : ALIGN(32)
    {
        *ipc_client_test.*(.text*)
        *ipc_client_test.*(.rodata*)
        *(TFM_SP_IPC_CLIENT_TEST_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_IPC_CLIENT_TEST$$RO$$Base = ADDR(.TFM_SP_IPC_CLIENT_TEST);
    Image$$TFM_SP_IPC_CLIENT_TEST$$RO$$Limit = ADDR(.TFM_SP_IPC_CLIENT_TEST) + SIZEOF(.TFM_SP_IPC_CLIENT_TEST);
    Image$$TFM_SP_IPC_CLIENT_TEST$$Base = ADDR(.TFM_SP_IPC_CLIENT_TEST);
    Image$$TFM_SP_IPC_CLIENT_TEST$$Limit = ADDR(.TFM_SP_IPC_CLIENT_TEST) + SIZEOF(.TFM_SP_IPC_CLIENT_TEST);
#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
    .TFM_IRQ_TEST_1 : ALIGN(32)
    {
        *tfm_irq_test_service_1.*(.text*)
        *tfm_irq_test_service_1.*(.rodata*)
        *timer_cmsdk*(.text*)
        *timer_cmsdk*(.rodata*)
        *(TFM_IRQ_TEST_1_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_IRQ_TEST_1$$RO$$Base = ADDR(.TFM_IRQ_TEST_1);
    Image$$TFM_IRQ_TEST_1$$RO$$Limit = ADDR(.TFM_IRQ_TEST_1) + SIZEOF(.TFM_IRQ_TEST_1);
    Image$$TFM_IRQ_TEST_1$$Base = ADDR(.TFM_IRQ_TEST_1);
    Image$$TFM_IRQ_TEST_1$$Limit = ADDR(.TFM_IRQ_TEST_1) + SIZEOF(.TFM_IRQ_TEST_1);
#endif /* TFM_ENABLE_IRQ_TEST */

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;


    .ER_TFM_CODE : ALIGN(4)
    {
        *(.text*)

        KEEP(*(.init))
        KEEP(*(.fini))


        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
         *crtbegin.o(.dtors)
         *crtbegin?.o(.dtors)
         *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
         *(SORT(.dtors.*))
         *(.dtors)

        *(.rodata*)

        KEEP(*(.eh_frame*))
    } > FLASH

    .psp_stack : ALIGN(32)
    {
        . += __psp_stack_size__;
    } > RAM
    Image$$ARM_LIB_STACK$$ZI$$Base = ADDR(.psp_stack);
    Image$$ARM_LIB_STACK$$ZI$$Limit = ADDR(.psp_stack) + SIZEOF(.psp_stack);

    .TFM_UNPRIV_DATA : ALIGN(32)
    {
        */tfm_spm_services.o(.data*)
        */device_definition.o(.data*)
        */stm32u5xx_hal*.o(.data*)
        */tick.o(.data*)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_UNPRIV_DATA$$RW$$Base = ADDR(.TFM_UNPRIV_DATA);
    Image$$TFM_UNPRIV_DATA$$RW$$Limit = ADDR(.TFM_UNPRIV_DATA) + SIZEOF(.TFM_UNPRIV_DATA);

    .TFM_UNPRIV_BSS : ALIGN(32)
    {
        */tfm_spm_services.o(.bss*)
        */device_definition.o(.bss*)
        */stm32u5xx_hal*.o(.bss*)
        */tick.o(.bss*)
         */tfm_spm_services.o(COMMON)
        */device_definition.o(COMMON)
        */stm32u5xx_hal*.o(COMMON)
        */tick.o(COMMON)
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_UNPRIV_DATA$$ZI$$Base = ADDR(.TFM_UNPRIV_BSS);
    Image$$TFM_UNPRIV_DATA$$ZI$$Limit = ADDR(.TFM_UNPRIV_BSS) + SIZEOF(.TFM_UNPRIV_BSS);

#if defined (TFM_PARTITION_APP_ROT)   
    .TFM_APP_ROT_DATA : ALIGN(32)
    {
        *app_rot*.o(.data*)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_APP_ROT_DATA$$RW$$Base = ADDR(.TFM_APP_ROT_DATA);
    Image$$TFM_APP_ROT_DATA$$RW$$Limit = ADDR(.TFM_APP_ROT_DATA) + SIZEOF(.TFM_APP_ROT_DATA);

    .TFM_APP_ROT_BSS : ALIGN(32)
    {
        start_of_TFM_APP_ROT = .;
        *app_rot*.o(.bss*)
        *app_rot*.o(COMMON)
        *(TFM_APP_ROT_ATTR_ZI)
        . += (. - start_of_TFM_APP_ROT) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_APP_ROT_DATA$$ZI$$Base = ADDR(.TFM_APP_ROT_BSS);
    Image$$TFM_APP_ROT_DATA$$ZI$$Limit = ADDR(.TFM_APP_ROT_BSS) + SIZEOF(.TFM_APP_ROT_BSS);
    
    .TFM_APP_ROT_STACK : ALIGN(128)
    {
        . += 0x0A00;
    } > RAM
    Image$$TFM_APP_ROT_STACK$$ZI$$Base = ADDR(.TFM_APP_ROT_STACK);
    Image$$TFM_APP_ROT_STACK$$ZI$$Limit = ADDR(.TFM_APP_ROT_STACK) + SIZEOF(.TFM_APP_ROT_STACK);
#endif
    .TFM_APP_RW_STACK_END : ALIGN(GTZC_RAM_ALIGN)
    {
        . += 0x0;
    } > RAM
 
    /**** APPLICATION RoT DATA end here */
    Image$$TFM_APP_RW_STACK_END$$Base =  ADDR(.TFM_APP_RW_STACK_END);
    /**** PSA RoT DATA start here */
    Image$$TFM_PSA_RW_STACK_START$$Base = .;

    .TFM_SP_STORAGE_DATA : ALIGN(32)
    {
        *tfm_storage*:*(.data*)
        *(TFM_SP_STORAGE_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_SP_STORAGE_DATA$$RW$$Base = ADDR(.TFM_SP_STORAGE_DATA);
    Image$$TFM_SP_STORAGE_DATA$$RW$$Limit = ADDR(.TFM_SP_STORAGE_DATA) + SIZEOF(.TFM_SP_STORAGE_DATA);

    .TFM_SP_STORAGE_BSS : ALIGN(32)
    {
        start_of_TFM_SP_STORAGE = .;
        *tfm_storage*:*(.bss*)
        *tfm_storage*:*(COMMON)
        *(TFM_SP_STORAGE_ATTR_ZI)
        . += (. - start_of_TFM_SP_STORAGE) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_SP_STORAGE_DATA$$ZI$$Base = ADDR(.TFM_SP_STORAGE_BSS);
    Image$$TFM_SP_STORAGE_DATA$$ZI$$Limit = ADDR(.TFM_SP_STORAGE_BSS) + SIZEOF(.TFM_SP_STORAGE_BSS);


    .TFM_SP_STORAGE_STACK : ALIGN(128)
    {
        . += 0xA00;
    } > RAM
    Image$$TFM_SP_STORAGE_STACK$$ZI$$Base = ADDR(.TFM_SP_STORAGE_STACK);
    Image$$TFM_SP_STORAGE_STACK$$ZI$$Limit = ADDR(.TFM_SP_STORAGE_STACK) + SIZEOF(.TFM_SP_STORAGE_STACK);



    .TFM_SP_ITS_DATA : ALIGN(32)
    {
        *tfm_internal_trusted_storage*:*(.data*)
        *(TFM_SP_ITS_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_SP_ITS_DATA$$RW$$Base = ADDR(.TFM_SP_ITS_DATA);
    Image$$TFM_SP_ITS_DATA$$RW$$Limit = ADDR(.TFM_SP_ITS_DATA) + SIZEOF(.TFM_SP_ITS_DATA);

    .TFM_SP_ITS_BSS : ALIGN(32)
    {
        start_of_TFM_SP_ITS = .;
        *tfm_internal_trusted_storage*:*(.bss*)
        *tfm_internal_trusted_storage*:*(COMMON)
        *(TFM_SP_ITS_ATTR_ZI)
        . += (. - start_of_TFM_SP_ITS) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_SP_ITS_DATA$$ZI$$Base = ADDR(.TFM_SP_ITS_BSS);
    Image$$TFM_SP_ITS_DATA$$ZI$$Limit = ADDR(.TFM_SP_ITS_BSS) + SIZEOF(.TFM_SP_ITS_BSS);


    .TFM_SP_ITS_STACK : ALIGN(128)
    {
        . += 0x500;
    } > RAM
    Image$$TFM_SP_ITS_STACK$$ZI$$Base = ADDR(.TFM_SP_ITS_STACK);
    Image$$TFM_SP_ITS_STACK$$ZI$$Limit = ADDR(.TFM_SP_ITS_STACK) + SIZEOF(.TFM_SP_ITS_STACK);

    .TFM_SP_CRYPTO_DATA : ALIGN(32)
    {
        *tfm_crypto*:*(.data*)
        *(TFM_SP_CRYPTO_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_SP_CRYPTO_DATA$$RW$$Base = ADDR(.TFM_SP_CRYPTO_DATA);
    Image$$TFM_SP_CRYPTO_DATA$$RW$$Limit = ADDR(.TFM_SP_CRYPTO_DATA) + SIZEOF(.TFM_SP_CRYPTO_DATA);

    .TFM_SP_CRYPTO_BSS : ALIGN(32)
    {
        start_of_TFM_SP_CRYPTO = .;
        *tfm_crypto*:*(.bss*)
        *tfm_crypto*:*(COMMON)
        *(TFM_SP_CRYPTO_ATTR_ZI)
        . += (. - start_of_TFM_SP_CRYPTO) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_SP_CRYPTO_DATA$$ZI$$Base = ADDR(.TFM_SP_CRYPTO_BSS);
    Image$$TFM_SP_CRYPTO_DATA$$ZI$$Limit = ADDR(.TFM_SP_CRYPTO_BSS) + SIZEOF(.TFM_SP_CRYPTO_BSS);

    .TFM_SP_CRYPTO_STACK : ALIGN(128)
    {
        . += 0x2000;
    } > RAM
    Image$$TFM_SP_CRYPTO_STACK$$ZI$$Base = ADDR(.TFM_SP_CRYPTO_STACK);
    Image$$TFM_SP_CRYPTO_STACK$$ZI$$Limit = ADDR(.TFM_SP_CRYPTO_STACK) + SIZEOF(.TFM_SP_CRYPTO_STACK);


    .TFM_SP_PLATFORM_DATA : ALIGN(32)
    {
        *tfm_platform*:*(.data*)
        *(TFM_SP_PLATFORM_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_SP_PLATFORM_DATA$$RW$$Base = ADDR(.TFM_SP_PLATFORM_DATA);
    Image$$TFM_SP_PLATFORM_DATA$$RW$$Limit = ADDR(.TFM_SP_PLATFORM_DATA) + SIZEOF(.TFM_SP_PLATFORM_DATA);

    .TFM_SP_PLATFORM_BSS : ALIGN(32)
    {
        start_of_TFM_SP_PLATFORM = .;
        *tfm_platform*:*(.bss*)
        *tfm_platform*:*(COMMON)
        *(TFM_SP_PLATFORM_ATTR_ZI)
        . += (. - start_of_TFM_SP_PLATFORM) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_SP_PLATFORM_DATA$$ZI$$Base = ADDR(.TFM_SP_PLATFORM_BSS);
    Image$$TFM_SP_PLATFORM_DATA$$ZI$$Limit = ADDR(.TFM_SP_PLATFORM_BSS) + SIZEOF(.TFM_SP_PLATFORM_BSS);

    .TFM_SP_PLATFORM_STACK : ALIGN(128)
    {
        . += 0x0400;
    } > RAM
    Image$$TFM_SP_PLATFORM_STACK$$ZI$$Base = ADDR(.TFM_SP_PLATFORM_STACK);
    Image$$TFM_SP_PLATFORM_STACK$$ZI$$Limit = ADDR(.TFM_SP_PLATFORM_STACK) + SIZEOF(.TFM_SP_PLATFORM_STACK);
    
    .TFM_SP_INITIAL_ATTESTATION_DATA : ALIGN(32)
    {
        *tfm_attestation*:*(.data*)
        *UsefulBuff.*:*(.data*)
        *(TFM_SP_INITIAL_ATTESTATION_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    
    Image$$TFM_SP_INITIAL_ATTESTATION_DATA$$RW$$Base = ADDR(.TFM_SP_INITIAL_ATTESTATION_DATA);
    Image$$TFM_SP_INITIAL_ATTESTATION_DATA$$RW$$Limit = ADDR(.TFM_SP_INITIAL_ATTESTATION_DATA) + SIZEOF(.TFM_SP_INITIAL_ATTESTATION_DATA);

    .TFM_SP_INITIAL_ATTESTATION_BSS : ALIGN(32)
    {
        start_of_TFM_SP_INITIAL_ATTESTATION = .;
        *tfm_attestation*:*(.bss*)
        *UsefulBuff.*:*(.bss*)
        *tfm_attest*:*(COMMON)
        *(TFM_SP_INITIAL_ATTESTATION_ATTR_ZI)
        . += (. - start_of_TFM_SP_INITIAL_ATTESTATION) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_SP_INITIAL_ATTESTATION_DATA$$ZI$$Base = ADDR(.TFM_SP_INITIAL_ATTESTATION_BSS);
    Image$$TFM_SP_INITIAL_ATTESTATION_DATA$$ZI$$Limit = ADDR(.TFM_SP_INITIAL_ATTESTATION_BSS) + SIZEOF(.TFM_SP_INITIAL_ATTESTATION_BSS);

    .TFM_SP_INITIAL_ATTESTATION_STACK : ALIGN(128)
    {
        . += 0x0A80 ;
    } > RAM
    Image$$TFM_SP_INITIAL_ATTESTATION_STACK$$ZI$$Base = ADDR(.TFM_SP_INITIAL_ATTESTATION_STACK);
    Image$$TFM_SP_INITIAL_ATTESTATION_STACK$$ZI$$Limit = ADDR(.TFM_SP_INITIAL_ATTESTATION_STACK) + SIZEOF(.TFM_SP_INITIAL_ATTESTATION_STACK);


#ifdef TFM_PARTITION_TEST_CORE
    .TFM_SP_CORE_TEST_DATA : ALIGN(32)
    {
        *tfm_ss_core_test.*(.data*)
        *(TFM_SP_CORE_TEST_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_SP_CORE_TEST_DATA$$RW$$Base = ADDR(.TFM_SP_CORE_TEST_DATA);
    Image$$TFM_SP_CORE_TEST_DATA$$RW$$Limit = ADDR(.TFM_SP_CORE_TEST_DATA) + SIZEOF(.TFM_SP_CORE_TEST_DATA);

    .TFM_SP_CORE_TEST_BSS : ALIGN(32)
    {
        start_of_TFM_SP_CORE_TEST = .;
        *tfm_ss_core_test.*(.bss*)
        *tfm_ss_core_test.*(COMMON)
        *(TFM_SP_CORE_TEST_ATTR_ZI)
        . += (. - start_of_TFM_SP_CORE_TEST) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_SP_CORE_TEST_DATA$$ZI$$Base = ADDR(.TFM_SP_CORE_TEST_BSS);
    Image$$TFM_SP_CORE_TEST_DATA$$ZI$$Limit = ADDR(.TFM_SP_CORE_TEST_BSS) + SIZEOF(.TFM_SP_CORE_TEST_BSS);

    .TFM_SP_CORE_TEST_STACK : ALIGN(128)
    {
        . += 0x0300;
    } > RAM
    Image$$TFM_SP_CORE_TEST_STACK$$ZI$$Base = ADDR(.TFM_SP_CORE_TEST_STACK);
    Image$$TFM_SP_CORE_TEST_STACK$$ZI$$Limit = ADDR(.TFM_SP_CORE_TEST_STACK) + SIZEOF(.TFM_SP_CORE_TEST_STACK);
#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    .TFM_SP_SECURE_TEST_PARTITION_DATA : ALIGN(32)
    {
            *tfm_secure_client_service.*(.data*)
        *test_framework*(.data*)
        *uart_stdout.*(.data*)
        *Driver_USART.*(.data*)
        *arm_uart_drv.*(.data*)
        *uart_pl011_drv.*(.data*)
        *uart_cmsdk_drv*(.data*)
        *secure_suites.*(.data*)
        *attestation_s_interface_testsuite.*(.data*)
        *(TFM_SP_SECURE_TEST_PARTITION_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_SP_SECURE_TEST_PARTITION_DATA$$RW$$Base = ADDR(.TFM_SP_SECURE_TEST_PARTITION_DATA);
    Image$$TFM_SP_SECURE_TEST_PARTITION_DATA$$RW$$Limit = ADDR(.TFM_SP_SECURE_TEST_PARTITION_DATA) + SIZEOF(.TFM_SP_SECURE_TEST_PARTITION_DATA);

    .TFM_SP_SECURE_TEST_PARTITION_BSS : ALIGN(32)
    {
        start_of_TFM_SP_SECURE_TEST_PARTITION = .;
        *tfm_secure_client_service.*(.bss*)
        *tfm_secure_client_service.*(COMMON)
        *test_framework*(.bss*)
        *test_framework*(COMMON)
        *uart_stdout.*(.bss*)
        *uart_stdout.*(COMMON)
        *Driver_USART.*(.bss*)
        *Driver_USART.*(COMMON)
        *arm_uart_drv.*(.bss*)
        *arm_uart_drv.*(COMMON)
        *uart_pl011_drv.*(.bss*)
        *uart_pl011_drv.*(COMMON)
        *uart_cmsdk_drv*(.bss*)
        *uart_cmsdk_drv*(COMMON)
        *secure_suites.*(.bss*)
        *secure_suites.*(COMMON)
        *attestation_s_interface_testsuite.*(.bss*)
        *attestation_s_interface_testsuite.*(COMMON)
        *(TFM_SP_SECURE_TEST_PARTITION_ATTR_ZI)
        . += (. - start_of_TFM_SP_SECURE_TEST_PARTITION) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_SP_SECURE_TEST_PARTITION_DATA$$ZI$$Base = ADDR(.TFM_SP_SECURE_TEST_PARTITION_BSS);
    Image$$TFM_SP_SECURE_TEST_PARTITION_DATA$$ZI$$Limit = ADDR(.TFM_SP_SECURE_TEST_PARTITION_BSS) + SIZEOF(.TFM_SP_SECURE_TEST_PARTITION_BSS);

    .TFM_SP_SECURE_TEST_PARTITION_STACK : ALIGN(128)
    {
        . += 0x0C00;
    } > RAM
    Image$$TFM_SP_SECURE_TEST_PARTITION_STACK$$ZI$$Base = ADDR(.TFM_SP_SECURE_TEST_PARTITION_STACK);
    Image$$TFM_SP_SECURE_TEST_PARTITION_STACK$$ZI$$Limit = ADDR(.TFM_SP_SECURE_TEST_PARTITION_STACK) + SIZEOF(.TFM_SP_SECURE_TEST_PARTITION_STACK);

#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    .TFM_SP_IPC_SERVICE_TEST_DATA : ALIGN(32)
    {
        *ipc_service_test.*(.data*)
        *(TFM_SP_IPC_SERVICE_TEST_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_SP_IPC_SERVICE_TEST_DATA$$RW$$Base = ADDR(.TFM_SP_IPC_SERVICE_TEST_DATA);
    Image$$TFM_SP_IPC_SERVICE_TEST_DATA$$RW$$Limit = ADDR(.TFM_SP_IPC_SERVICE_TEST_DATA) + SIZEOF(.TFM_SP_IPC_SERVICE_TEST_DATA);

    .TFM_SP_IPC_SERVICE_TEST_BSS : ALIGN(32)
    {
        start_of_TFM_SP_IPC_SERVICE_TEST = .;
        *ipc_service_test.*(.bss*)
        *ipc_service_test.*(COMMON)
        *(TFM_SP_IPC_SERVICE_TEST_ATTR_ZI)
        . += (. - start_of_TFM_SP_IPC_SERVICE_TEST) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_SP_IPC_SERVICE_TEST_DATA$$ZI$$Base = ADDR(.TFM_SP_IPC_SERVICE_TEST_BSS);
    Image$$TFM_SP_IPC_SERVICE_TEST_DATA$$ZI$$Limit = ADDR(.TFM_SP_IPC_SERVICE_TEST_BSS) + SIZEOF(.TFM_SP_IPC_SERVICE_TEST_BSS);


    .TFM_SP_IPC_SERVICE_TEST_STACK : ALIGN(128)
    {
        . += 0x0220;
    } > RAM
    Image$$TFM_SP_IPC_SERVICE_TEST_STACK$$ZI$$Base = ADDR(.TFM_SP_IPC_SERVICE_TEST_STACK);
    Image$$TFM_SP_IPC_SERVICE_TEST_STACK$$ZI$$Limit = ADDR(.TFM_SP_IPC_SERVICE_TEST_STACK) + SIZEOF(.TFM_SP_IPC_SERVICE_TEST_STACK);

#endif /* TFM_PARTITION_TEST_CORE_IPC */

    /**** PSA RoT DATA end here */
    Image$$TFM_PSA_RW_STACK_END$$Base = .;

    /**** APPLICATION RoT DATA start here */
    Image$$TFM_APP_RW_STACK_START$$Base = .;

#ifdef TFM_PARTITION_TEST_CORE
    .TFM_SP_CORE_TEST_2_DATA : ALIGN(32)
    {
        *tfm_ss_core_test_2.*(.data*)
        *(TFM_SP_CORE_TEST_2_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_SP_CORE_TEST_2_DATA$$RW$$Base = ADDR(.TFM_SP_CORE_TEST_2_DATA);
    Image$$TFM_SP_CORE_TEST_2_DATA$$RW$$Limit = ADDR(.TFM_SP_CORE_TEST_2_DATA) + SIZEOF(.TFM_SP_CORE_TEST_2_DATA);

    .TFM_SP_CORE_TEST_2_BSS : ALIGN(32)
    {
        start_of_TFM_SP_CORE_TEST_2 = .;
        *tfm_ss_core_test_2.*(.bss*)
        *tfm_ss_core_test_2.*(COMMON)
        *(TFM_SP_CORE_TEST_2_ATTR_ZI)
        . += (. - start_of_TFM_SP_CORE_TEST_2) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_SP_CORE_TEST_2_DATA$$ZI$$Base = ADDR(.TFM_SP_CORE_TEST_2_BSS);
    Image$$TFM_SP_CORE_TEST_2_DATA$$ZI$$Limit = ADDR(.TFM_SP_CORE_TEST_2_BSS) + SIZEOF(.TFM_SP_CORE_TEST_2_BSS);


    .TFM_SP_CORE_TEST_2_STACK : ALIGN(128)
    {
        . += 0x0200;
    } > RAM
    Image$$TFM_SP_CORE_TEST_2_STACK$$ZI$$Base = ADDR(.TFM_SP_CORE_TEST_2_STACK);
    Image$$TFM_SP_CORE_TEST_2_STACK$$ZI$$Limit = ADDR(.TFM_SP_CORE_TEST_2_STACK) + SIZEOF(.TFM_SP_CORE_TEST_2_STACK);

#endif /* TFM_PARTITION_TEST_CORE */

#ifdef TFM_PARTITION_TEST_CORE_IPC
    .TFM_SP_IPC_CLIENT_TEST_DATA : ALIGN(32)
    {
        *ipc_client_test.*(.data*)
        *(TFM_SP_IPC_CLIENT_TEST_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_SP_IPC_CLIENT_TEST_DATA$$RW$$Base = ADDR(.TFM_SP_IPC_CLIENT_TEST_DATA);
    Image$$TFM_SP_IPC_CLIENT_TEST_DATA$$RW$$Limit = ADDR(.TFM_SP_IPC_CLIENT_TEST_DATA) + SIZEOF(.TFM_SP_IPC_CLIENT_TEST_DATA);

    .TFM_SP_IPC_CLIENT_TEST_BSS : ALIGN(32)
    {
        start_of_TFM_SP_IPC_CLIENT_TEST = .;
        *ipc_client_test.*(.bss*)
        *ipc_client_test.*(COMMON)
        *(TFM_SP_IPC_CLIENT_TEST_ATTR_ZI)
        . += (. - start_of_TFM_SP_IPC_CLIENT_TEST) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_SP_IPC_CLIENT_TEST_DATA$$ZI$$Base = ADDR(.TFM_SP_IPC_CLIENT_TEST_BSS);
    Image$$TFM_SP_IPC_CLIENT_TEST_DATA$$ZI$$Limit = ADDR(.TFM_SP_IPC_CLIENT_TEST_BSS) + SIZEOF(.TFM_SP_IPC_CLIENT_TEST_BSS);

    .TFM_SP_IPC_CLIENT_TEST_STACK : ALIGN(128)
    {
        . += 0x0220;
    } > RAM
    Image$$TFM_SP_IPC_CLIENT_TEST_STACK$$ZI$$Base = ADDR(.TFM_SP_IPC_CLIENT_TEST_STACK);
    Image$$TFM_SP_IPC_CLIENT_TEST_STACK$$ZI$$Limit = ADDR(.TFM_SP_IPC_CLIENT_TEST_STACK) + SIZEOF(.TFM_SP_IPC_CLIENT_TEST_STACK);

#endif /* TFM_PARTITION_TEST_CORE_IPC */

#ifdef TFM_ENABLE_IRQ_TEST
    .TFM_IRQ_TEST_1_DATA : ALIGN(32)
    {
        *tfm_irq_test_service_1.*(.data*)
        *timer_cmsdk*(.data*)
        *(TFM_IRQ_TEST_1_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$TFM_IRQ_TEST_1_DATA$$RW$$Base = ADDR(.TFM_IRQ_TEST_1_DATA);
    Image$$TFM_IRQ_TEST_1_DATA$$RW$$Limit = ADDR(.TFM_IRQ_TEST_1_DATA) + SIZEOF(.TFM_IRQ_TEST_1_DATA);

    .TFM_IRQ_TEST_1_BSS : ALIGN(32)
    {
        start_of_TFM_IRQ_TEST_1 = .;
        *tfm_irq_test_service_1.*(.bss*)
        *tfm_irq_test_service_1.*(COMMON)
        *timer_cmsdk*(.bss*)
        *timer_cmsdk*(COMMON)
        *(TFM_IRQ_TEST_1_ATTR_ZI)
        . += (. - start_of_TFM_IRQ_TEST_1) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$TFM_IRQ_TEST_1_DATA$$ZI$$Base = ADDR(.TFM_IRQ_TEST_1_BSS);
    Image$$TFM_IRQ_TEST_1_DATA$$ZI$$Limit = ADDR(.TFM_IRQ_TEST_1_BSS) + SIZEOF(.TFM_IRQ_TEST_1_BSS);

    .TFM_IRQ_TEST_1_STACK : ALIGN(128)
    {
        . += 0x0400;
    } > RAM
    Image$$TFM_IRQ_TEST_1_STACK$$ZI$$Base = ADDR(.TFM_IRQ_TEST_1_STACK);
    Image$$TFM_IRQ_TEST_1_STACK$$ZI$$Limit = ADDR(.TFM_IRQ_TEST_1_STACK) + SIZEOF(.TFM_IRQ_TEST_1_STACK);

#endif /* TFM_ENABLE_IRQ_TEST */

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    .TFM_SP_SECURE_TEST_PARTITION_DATA : ALIGN(32)
    {
        *libc_nano*:*(.data*)
        . = ALIGN(32);
    } > RAM AT> FLASH

    .TFM_SP_SECURE_TEST_PARTITION_BSS : ALIGN(32)
    {
        /* FixMe:
         * Test framework use printf to print out test result. Implementation of
         * printf in GCC libc use static data and heap as well. To be able to
         * execute test suites with TFM_LVL=3 this workaround is needed to
         * allocate libc static data and heap within the data section of secure
         * test partition. This can be removed if test service will be executed
         * in privileged mode.
         */
        *libc_nano*:*(.bss*)
        *libc_nano*:*(COMMON)

        __end__ = .;
        PROVIDE(end = .);
        __HeapBase = .;
        . += __heap_size__;
        __HeapLimit = .;
        __heap_limit = .; /* Add for _sbrk */

        . = ALIGN(32);
    } > RAM AT> RAM
#else /* TFM_PARTITION_TEST_SECURE_SERVICES */
    .heap : ALIGN(8)
    {
        __end__ = .;
        PROVIDE(end = .);
        __HeapBase = .;
        . += __heap_size__;
        __HeapLimit = .;
        __heap_limit = .; /* Add for _sbrk */
    } > RAM AT> RAM
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

    .TFM_DATA : ALIGN(4)
    {
        *(.data*)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(4);

    } > RAM AT> FLASH
    Image$$ER_TFM_DATA$$RW$$Base = ADDR(.TFM_DATA);
    Image$$ER_TFM_DATA$$RW$$Limit = ADDR(.TFM_DATA) + SIZEOF(.TFM_DATA);

    .TFM_BSS : ALIGN(4)
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM AT> RAM
    Image$$ER_TFM_DATA$$ZI$$Base = ADDR(.TFM_BSS);
    Image$$ER_TFM_DATA$$ZI$$Limit = ADDR(.TFM_BSS) + SIZEOF(.TFM_BSS);

    Image$$ER_TFM_DATA$$Base = ADDR(.TFM_DATA);
    Image$$ER_TFM_DATA$$Limit = ADDR(.TFM_DATA) + SIZEOF(.TFM_DATA) + SIZEOF(.TFM_BSS);

    .msp_stack : ALIGN(32)
    {
        . += __msp_init_stack_size__;
    } > RAM
    Image$$ARM_LIB_STACK_MSP$$ZI$$Base = ADDR(.msp_stack);
    Image$$ARM_LIB_STACK_MSP$$ZI$$Limit = ADDR(.msp_stack) + SIZEOF(.msp_stack);

    /*
     * Place the CMSE Veneers (containing the SG instruction) after the code, in a
     * separate 32 bytes aligned region so that the SAU can programmed to just set
     * this region as Non-Secure Callable.
     */
    .gnu.sgstubs : ALIGN(32)
    {
        *(.gnu.sgstubs*)
        . = ALIGN(32);
    } > VENEERS AT> VENEERS
    Load$$LR$$LR_VENEER$$Base = ADDR(.gnu.sgstubs);
    Load$$LR$$LR_VENEER$$Limit = ADDR(.gnu.sgstubs) + SIZEOF(.gnu.sgstubs);
    Load$$LR$$LR_NS_PARTITION$$Base = NS_PARTITION_START;

    PROVIDE(__stack = Image$$ARM_LIB_STACK$$ZI$$Limit);
    PROVIDE(__StackTop = Image$$ARM_LIB_STACK$$ZI$$Limit);
    PROVIDE(__StackLimit = Image$$ARM_LIB_STACK$$ZI$$Base);
}