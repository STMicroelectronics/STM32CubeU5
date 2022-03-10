/**
  ******************************************************************************
  * @file    boot_hal.c
  * @author  MCD Application Team
  * @brief   This file contains  mcuboot stm32l5xx hardware specific implementation
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "boot_hal_cfg.h"
#include "boot_hal.h"
#include "uart_stdout.h"
#include "tfm_low_level_security.h"
#include "target_cfg.h"
#include "cmsis.h"
#include "Driver_Flash.h"
#include "region_defs.h"
#if defined(EXTERNAL_FLASH)
#include "flash_map_backend/flash_map_backend.h"
#endif /*  defined(EXTERNAL_FLASH) */
extern ARM_DRIVER_FLASH FLASH_DEV_NAME;
#if defined(EXTERNAL_FLASH)
extern ARM_DRIVER_FLASH OSPI_FLASH_DEV_NAME;

int flash_device_base(uint8_t fd_id, uintptr_t *ret)
{
    switch (fd_id) {
    case  FLASH_DEVICE_ID :
        *ret = FLASH_DEVICE_BASE;
        break;
    case OSPI_FLASH_DEV_ID:
        *ret = OSPI_FLASH_BASE_ADDRESS;
        break;
    default:
        BOOT_LOG_ERR("invalid flash ID %d; expected %d",
                fd_id, FLASH_DEVICE_ID);
        *ret = -1;
        return -1;
    }
    return 0;
}

#endif /*  defined(EXTERNAL_FLASH) */
void double_jump_to_next_image(uint32_t boot_jump_addr, uint32_t reset_handler_addr) __NO_RETURN;
/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#elif defined(__CC_ARM)
#pragma arm section code = ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */
__attribute__((naked)) void double_jump_to_next_image(uint32_t boot_jump_addr, uint32_t reset_handler_addr)
{
    __ASM volatile(
#if !defined(__ICCARM__)
        ".syntax unified                 \n"
#endif
        "mov     r7, r0                  \n"
        "mov     r8, r1                  \n"
        "bl      boot_clear_bl2_ram_area \n" /* Clear RAM before jump */
        "movs    r0, #0                  \n" /* Clear registers: R0-R12, */
        "mov     r1, r0                  \n" /* except R7 */
        "mov     r2, r0                  \n"
        "mov     r3, r0                  \n"
        "mov     r4, r0                  \n"
        "mov     r5, r0                  \n"
        "mov     r6, r0                  \n"
        "mov     r9, r0                  \n"
        "mov     r10, r0                 \n"
        "mov     r11, r0                 \n"
        "mov     r12, r0                 \n"
        "mov     lr,  r0                 \n"
        "mov     r0, r8                  \n"
        "mov     r8, r1                  \n"
        "bx      r7                      \n" /* Jump to Reset_handler */
    );
}
/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#elif defined(__CC_ARM)
#pragma arm section code
#endif /* __ICCARM__ */

/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#elif defined(__CC_ARM)
#pragma arm section code = ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */
/**
  * @brief This function is called to clear all RAM area before jumping in
  * in Secure application .
  * @note
  * @retval void
  */
void boot_platform_quit(struct boot_arm_vector_table *vt)
{
    /* Clang at O0, stores variables on the stack with SP relative addressing.
     * When manually set the SP then the place of reset vector is lost.
     * Static variables are stored in 'data' or 'bss' section, change of SP has
     * no effect on them.
     */
    static struct boot_arm_vector_table *vt_cpy;

    vt_cpy=vt;
    /* activate protection before jumping in secure image */
    TFM_LL_SECU_UpdateRunTimeProtections();
#if defined(__ARM_ARCH_8M_MAIN__) || defined(__ARM_ARCH_8M_BASE__)
    /* Restore the Main Stack Pointer Limit register's reset value
     * before passing execution to runtime firmware to make the
     * bootloader transparent to it.
     */
    __set_MSPLIM(0);
#endif
    __set_MSP(vt_cpy->msp);
    __DSB();
    __ISB();

    double_jump_to_next_image((uint32_t)&double_jump_to_next_image, vt->reset);
#if !defined(__ICCARM__)
    /* Avoid bx lr instruction (for fault injection) */
    __builtin_unreachable();
#else /* defined(__ICCARM__) */
    while (1);
#endif  /* !defined(__ICCARM__) */
}
/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#elif defined(__CC_ARM)
#pragma arm section code
#endif /* __ICCARM__ */

/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#elif defined(__CC_ARM)
#pragma arm section code = ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */
/**
  * @brief This function is called to clear all RAM area before jumping in
  * in Secure application .
  * @note
  * @retval void
  */
void boot_clear_bl2_ram_area(void)
{
  __IO uint32_t *pt = (uint32_t *)BL2_DATA_START;
  uint32_t index;

  for (index = 0; index < (BL2_DATA_SIZE/4); index++)
  {
    pt[index] = 0;
  }
}

/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#elif defined(__CC_ARM)
#pragma arm section code
#endif /* __ICCARM__ */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/**
  * @brief  Platform init
  * @param  None
  * @retval status
  */
int32_t boot_platform_init(void)
{
  /* STM32L5xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
             can eventually implement his proper time base source (a general purpose
             timer for example or other time source), keeping in mind that Time base
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
             handled in milliseconds basis.
       - Set NVIC Group Priority to 3
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the System clock to have a frequency of 110 MHz */
  SystemClock_Config();

  /* Init for log */
  stdio_init();

  /* Apply Run time Protection */
  TFM_LL_SECU_ApplyRunTimeProtections();
  /* Check static protections */
  TFM_LL_SECU_CheckStaticProtections();

  /* enable SRAM2 retention low power mode */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_EnableSRAM2ContentRetention();

  /* Double protections apply / check to resist to basic hardware attacks */
  /* Apply Run time Protection */
  TFM_LL_SECU_ApplyRunTimeProtections();
  /* Check static protections */
  TFM_LL_SECU_CheckStaticProtections();

  if(FLASH_DEV_NAME.Initialize(NULL) != ARM_DRIVER_OK) {
    BOOT_LOG_ERR("Error while initializing Flash Interface");
    Error_Handler();
  }
#if defined(EXTERNAL_FLASH)
  if (OSPI_FLASH_DEV_NAME.Initialize(NULL) != ARM_DRIVER_OK)
  {
    BOOT_LOG_ERR("Error while initializing ospi Flash Interface");
    Error_Handler();
  }
#endif
  return 0;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 110000000
  *            HCLK(Hz)                       = 110000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 55
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            PLL_P                          = 2
  *            Flash Latency(WS)              = 5
  *            Voltage range                  = 0
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable voltage range 0 for frequency above 80 Mhz */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE0);
  __HAL_RCC_PWR_CLK_DISABLE();

  /* Enable MSI Oscillator and activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 55;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* To avoid undershoot due to maximum frequency, select PLL as system clock source */
  /* with AHB prescaler divider 2 as first step */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* AHB prescaler divider at 1 as second step */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".BL2_NoHdp_Code"
#elif defined(__CC_ARM)
#pragma arm section code = ".BL2_NoHdp_Code"
#else
__attribute__((section(".BL2_NoHdp_Code")))
#endif /* __ICCARM__ */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
#ifdef TFM_ERROR_HANDLER_NON_SECURE
#define WHILE_1_OPCODE 0xe7fe
  typedef void (*nsfptr_t)(void) __attribute__((cmse_nonsecure_call));
  nsfptr_t nsfptr = (nsfptr_t)(SRAM1_BASE_NS + 1);
  __IO uint16_t *pt = (uint16_t *)SRAM1_BASE_NS;
  /*  copy while(1) instruction */
  *pt = WHILE_1_OPCODE;
  /* Flush and refill pipeline  */
  __DSB();
  __ISB();
  /*  call non secure while(1) */
  nsfptr();
#else /*  TFM_ERROR_HANDLER_NON_SECURE */
  /* it is customizeable */
  /* an infinite loop,  and a reset for single fault injection */
  static __IO int once = 1;
  while (once);
  NVIC_SystemReset();
#endif  /*  TFM_ERROR_HANDLER_NON_SECURE */
}

/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#elif defined(__CC_ARM)
#pragma arm section code
#endif /* __ICCARM__ */

#if defined(__ARMCC_VERSION)
/* reimplement the function to reach Error Handler */
void __aeabi_assert(const char *expr, const char *file, int line)
{

  BOOT_LOG_ERR("assertion \" %s \" failed: file %s %d\n", expr, file, line);

  Error_Handler();
}
#endif  /*  __ARMCC_VERSION */
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  Error_Handler();
}
#endif /* USE_FULL_ASSERT */
