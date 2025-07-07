/*
 * Copyright (c) 2009-2020 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file is derivative of CMSIS V5.6.0 startup_ARMv81MML.c
 * Git SHA: b5f0603d6a584d1724d952fd8b0737458b90d62b
 */

#include "stm32u5xx.h"
/*----------------------------------------------------------------------------
  Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void( *pFunc )( void );

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;

extern void __PROGRAM_START(void) __NO_RETURN;

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
extern void Error_Handler(void);
#endif

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
void Reset_Handler  (void) __NO_RETURN;

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
#define DEFAULT_IRQ_HANDLER(handler_name)  \
void handler_name(void); \
__WEAK void handler_name(void) { \
    while(1); \
}

/* Exceptions */
DEFAULT_IRQ_HANDLER(NMI_Handler)
DEFAULT_IRQ_HANDLER(HardFault_Handler)
DEFAULT_IRQ_HANDLER(MemManage_Handler)
DEFAULT_IRQ_HANDLER(BusFault_Handler)
DEFAULT_IRQ_HANDLER(UsageFault_Handler)
DEFAULT_IRQ_HANDLER(SecureFault_Handler)
DEFAULT_IRQ_HANDLER(SVC_Handler)
DEFAULT_IRQ_HANDLER(DebugMon_Handler)
DEFAULT_IRQ_HANDLER(PendSV_Handler)
DEFAULT_IRQ_HANDLER(SysTick_Handler)

DEFAULT_IRQ_HANDLER(WWDG_IRQHandler)
DEFAULT_IRQ_HANDLER(PVD_PVM_IRQHandler)
DEFAULT_IRQ_HANDLER(RTC_IRQHandler)
DEFAULT_IRQ_HANDLER(RTC_IRQHandler_S)
DEFAULT_IRQ_HANDLER(TAMP_IRQHandler)
DEFAULT_IRQ_HANDLER(RAMCFG_IRQHandler)
DEFAULT_IRQ_HANDLER(FLASH_IRQHandler)
DEFAULT_IRQ_HANDLER(FLASH_IRQHandler_S)
DEFAULT_IRQ_HANDLER(GTZC_IRQHandler)
DEFAULT_IRQ_HANDLER(RCC_IRQHandler)
DEFAULT_IRQ_HANDLER(RCC_IRQHandler_S)
DEFAULT_IRQ_HANDLER(EXTI0_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI1_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI2_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI3_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI4_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI5_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI6_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI7_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI8_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI9_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI10_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI11_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI12_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI13_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI14_IRQHandler)
DEFAULT_IRQ_HANDLER(EXTI15_IRQHandler)
DEFAULT_IRQ_HANDLER(IWDG_IRQHandler)
#if defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U545xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
DEFAULT_IRQ_HANDLER(SAES_IRQHandler)
#endif /* defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U545xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
DEFAULT_IRQ_HANDLER(GPDMA1_Channel0_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel1_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel2_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel3_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel4_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel5_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel6_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel7_IRQHandler)
#if defined(STM32U585xx) || defined(STM32U575xx) || defined(STM32U535xx) || defined(STM32U545xx)
DEFAULT_IRQ_HANDLER(ADC1_IRQHandler)
#else
DEFAULT_IRQ_HANDLER(ADC1_2_IRQHandler)
#endif /* defined(STM32U585xx) || defined(STM32U575xx) || defined(STM32U535xx) || defined(STM32U545xx) */
DEFAULT_IRQ_HANDLER(DAC1_IRQHandler)
DEFAULT_IRQ_HANDLER(FDCAN1_IT0_IRQHandler)
DEFAULT_IRQ_HANDLER(FDCAN1_IT1_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM1_BRK_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM1_UP_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM1_TRG_COM_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM1_CC_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM2_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM3_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM4_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM5_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM6_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM7_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM8_BRK_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM8_UP_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM8_TRG_COM_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM8_CC_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C1_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C1_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C2_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C2_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(SPI1_IRQHandler)
DEFAULT_IRQ_HANDLER(SPI2_IRQHandler)
DEFAULT_IRQ_HANDLER(USART1_IRQHandler)
#if !(defined(STM32U535xx) || defined(STM32U545xx))
DEFAULT_IRQ_HANDLER(USART2_IRQHandler)
#endif /* !(defined(STM32U535xx) || defined(STM32U545xx)) */
DEFAULT_IRQ_HANDLER(USART3_IRQHandler)
DEFAULT_IRQ_HANDLER(UART4_IRQHandler)
DEFAULT_IRQ_HANDLER(UART5_IRQHandler)
DEFAULT_IRQ_HANDLER(LPUART1_IRQHandler)
DEFAULT_IRQ_HANDLER(LPTIM1_IRQHandler)
DEFAULT_IRQ_HANDLER(LPTIM2_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM15_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM16_IRQHandler)
DEFAULT_IRQ_HANDLER(TIM17_IRQHandler)
DEFAULT_IRQ_HANDLER(COMP_IRQHandler)
#if defined(STM32U535xx) || defined(STM32U545xx)
DEFAULT_IRQ_HANDLER(USB_IRQHandler)
#elif defined(STM32U585xx) || defined(STM32U575xx)
DEFAULT_IRQ_HANDLER(OTG_FS_IRQHandler)
#else
DEFAULT_IRQ_HANDLER(OTG_HS_IRQHandler)
#endif /* defined(STM32U535xx) || defined(STM32U545xx) */
DEFAULT_IRQ_HANDLER(CRS_IRQHandler)
#if !(defined(STM32U535xx) || defined(STM32U545xx))
DEFAULT_IRQ_HANDLER(FMC_IRQHandler)
#endif /* !(defined(STM32U535xx) || defined(STM32U545xx)) */
DEFAULT_IRQ_HANDLER(OCTOSPI1_IRQHandler)
DEFAULT_IRQ_HANDLER(PWR_S3WU_IRQHandler)
DEFAULT_IRQ_HANDLER(SDMMC1_IRQHandler)
#if !(defined(STM32U535xx) || defined(STM32U545xx))
DEFAULT_IRQ_HANDLER(SDMMC2_IRQHandler)
#endif /* !(defined(STM32U535xx) || defined(STM32U545xx)) */
DEFAULT_IRQ_HANDLER(DMA2_Channel1_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel8_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel9_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel10_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel11_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel12_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel13_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel14_IRQHandler)
DEFAULT_IRQ_HANDLER(GPDMA1_Channel15_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C3_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C3_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(SAI1_IRQHandler)
#if !(defined(STM32U535xx) || defined(STM32U545xx))
DEFAULT_IRQ_HANDLER(SAI2_IRQHandler)
#endif /* !(defined(STM32U535xx) || defined(STM32U545xx)) */
DEFAULT_IRQ_HANDLER(TSC_IRQHandler)
#if defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U545xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
DEFAULT_IRQ_HANDLER(AES_IRQHandler)
#endif /* defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U545xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
DEFAULT_IRQ_HANDLER(RNG_IRQHandler)
DEFAULT_IRQ_HANDLER(FPU_IRQHandler)
DEFAULT_IRQ_HANDLER(HASH_IRQHandler)
#if defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U545xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
DEFAULT_IRQ_HANDLER(PKA_IRQHandler)
#endif /* defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U545xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
DEFAULT_IRQ_HANDLER(LPTIM3_IRQHandler)
DEFAULT_IRQ_HANDLER(SPI3_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C4_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C4_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(MDF1_FLT0_IRQHandler)
DEFAULT_IRQ_HANDLER(MDF1_FLT1_IRQHandler)
#if !(defined(STM32U535xx) || defined(STM32U545xx))
DEFAULT_IRQ_HANDLER(MDF1_FLT2_IRQHandler)
DEFAULT_IRQ_HANDLER(MDF1_FLT3_IRQHandler)
DEFAULT_IRQ_HANDLER(UCPD1_IRQHandler)
#endif /* !(defined(STM32U535xx) || defined(STM32U545xx)) */
DEFAULT_IRQ_HANDLER(ICACHE_IRQHandler)
#if defined(STM32U545xx)
DEFAULT_IRQ_HANDLER(OTFDEC1_IRQHandler)
#elif defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
DEFAULT_IRQ_HANDLER(OTFDEC1_IRQHandler)
DEFAULT_IRQ_HANDLER(OTFDEC2_IRQHandler)
#endif /* defined(STM32U545xx) */
DEFAULT_IRQ_HANDLER(LPTIM4_IRQHandler)
DEFAULT_IRQ_HANDLER(DCACHE1_IRQHandler)
DEFAULT_IRQ_HANDLER(ADF1_IRQHandler)
DEFAULT_IRQ_HANDLER(ADC4_IRQHandler)
DEFAULT_IRQ_HANDLER(LPDMA1_Channel0_IRQHandler)
DEFAULT_IRQ_HANDLER(LPDMA1_Channel1_IRQHandler)
DEFAULT_IRQ_HANDLER(LPDMA1_Channel2_IRQHandler)
DEFAULT_IRQ_HANDLER(LPDMA1_Channel3_IRQHandler)
#if !(defined(STM32U535xx) || defined(STM32U545xx))
DEFAULT_IRQ_HANDLER(DMA2D_IRQHandler)
#endif /* !(defined(STM32U535xx) || defined(STM32U545xx)) */
DEFAULT_IRQ_HANDLER(DCMI_PSSI_IRQHandler)
#if !(defined(STM32U535xx) || defined(STM32U545xx))
DEFAULT_IRQ_HANDLER(OCTOSPI2_IRQHandler)
DEFAULT_IRQ_HANDLER(MDF1_FLT4_IRQHandler)
DEFAULT_IRQ_HANDLER(MDF1_FLT5_IRQHandler)
#endif /* !(defined(STM32U535xx) || defined(STM32U545xx)) */
DEFAULT_IRQ_HANDLER(CORDIC_IRQHandler)
DEFAULT_IRQ_HANDLER(FMAC_IRQHandler)
DEFAULT_IRQ_HANDLER(LSECSSD_IRQHandler)
#if defined(STM32U595xx) || defined(STM32U599xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
DEFAULT_IRQ_HANDLER(USART6_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C5_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C5_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C6_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C6_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(HSPI1_IRQHandler)
#endif /* defined(STM32U595xx) || defined(STM32U599xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
#if defined(STM32U599xx) || defined(STM32U5A9xx) || defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
DEFAULT_IRQ_HANDLER(GPU2D_IRQHandler)
DEFAULT_IRQ_HANDLER(GPU2D_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(GFXMMU_IRQHandler)
DEFAULT_IRQ_HANDLER(LTDC_IRQHandler)
DEFAULT_IRQ_HANDLER(LTDC_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(DSI_IRQHandler)
DEFAULT_IRQ_HANDLER(DCACHE2_IRQHandler)
#endif /* defined(STM32U599xx) || defined(STM32U5A9xx) || defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
#if defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
DEFAULT_IRQ_HANDLER(GFXTIM_IRQHandler)
DEFAULT_IRQ_HANDLER(JPEG_IRQHandler)
#endif /* defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const pFunc __VECTOR_TABLE[];
       const pFunc __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
  (pFunc)(&__INITIAL_SP),           /*      Initial Stack Pointer */
  Reset_Handler,                    /*      Reset Handler */
  NMI_Handler,                      /* -14: NMI Handler */
  Error_Handler,                    /* -13: Hard Fault Handler */
  Error_Handler,                    /* -12: MPU Fault Handler */
  Error_Handler,                    /* -11: Bus Fault Handler */
  Error_Handler,                    /* -10: Usage Fault Handler */
  Error_Handler,                    /*  -9: Secure Fault Handler */
  0,                                /*      Reserved */
  0,                                /*      Reserved */
  0,                                /*      Reserved */
  SVC_Handler,                      /*  -5: SVCall Handler */
  DebugMon_Handler,                 /*  -4: Debug Monitor Handler */
  0,                                /*      Reserved */
  PendSV_Handler,                   /*  -2: PendSV Handler */
  SysTick_Handler,                  /*  -1: SysTick Handler */
  WWDG_IRQHandler,                  /*   0: Window WatchDog */
  PVD_PVM_IRQHandler,               /*   1: PVD/AVD through EXTI Line detection Interrupt */
  RTC_IRQHandler,                   /*   2: RTC non-secure interrupt */
  RTC_IRQHandler_S,                 /*   3: RTC secure interrupt */
  TAMP_IRQHandler,                  /*   4: Tamper non-secure interrupt  */
  RAMCFG_IRQHandler,                /*   5: RAMCFG global */
  FLASH_IRQHandler,                 /*   6: FLASH non-secure global interrupt */
  FLASH_IRQHandler_S,               /*   7: FLASH secure global interrupt */
  GTZC_IRQHandler,                  /*   8: Global TrustZone Controller interrupt */
  RCC_IRQHandler,                   /*   9: RCC non-secure global interrupts */
  RCC_IRQHandler_S,                 /*  10: RCC secure global interrupts */
  EXTI0_IRQHandler,                 /*  11: EXTI Line0 */
  EXTI1_IRQHandler,                 /*  12: EXTI Line1 */
  EXTI2_IRQHandler,                 /*  13: EXTI Line2 */
  EXTI3_IRQHandler,                 /*  14: EXTI Line3 */
  EXTI4_IRQHandler,                 /*  15: EXTI Line4 */
  EXTI5_IRQHandler,                 /*  16: EXTI Line5 */
  EXTI6_IRQHandler,                 /*  17: EXTI Line6 */
  EXTI7_IRQHandler,                 /*  18: EXTI Line7 */
  EXTI8_IRQHandler,                 /*  19: EXTI Line8 */
  EXTI9_IRQHandler,                 /*  20: EXTI Line9 */
  EXTI10_IRQHandler,                /*  21: EXTI Line10 */
  EXTI11_IRQHandler,                /*  22: EXTI Line11 */
  EXTI12_IRQHandler,                /*  23: EXTI Line12 */
  EXTI13_IRQHandler,                /*  24: EXTI Line13 */
  EXTI14_IRQHandler,                /*  25: EXTI Line14 */
  EXTI15_IRQHandler,                /*  26: EXTI Line15 */
  IWDG_IRQHandler,                  /*  27: IWDG global interrupt */
#if defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U545xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
  SAES_IRQHandler,                  /*  28:Secure AES global interrupt */
#else
  0,                                /*  28: Reserved */
#endif /* defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U545xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
  GPDMA1_Channel0_IRQHandler,       /*  29: GPDMA1 Channel 0 global interrupt */
  GPDMA1_Channel1_IRQHandler,       /*  30: GPDMA1 Channel 1 global interrupt */
  GPDMA1_Channel2_IRQHandler,       /*  31: GPDMA1 Channel 2 global interrupt */
  GPDMA1_Channel3_IRQHandler,       /*  32: GPDMA1 Channel 3 global interrupt */
  GPDMA1_Channel4_IRQHandler,       /*  33: GPDMA1 Channel 4 global interrupt */
  GPDMA1_Channel5_IRQHandler,       /*  34: GPDMA1 Channel 5 global interrupt */
  GPDMA1_Channel6_IRQHandler,       /*  35: GPDMA1 Channel 6 global interrupt */
  GPDMA1_Channel7_IRQHandler,       /*  36: GPDMA1 Channel 7 global interrupt */
#if defined(STM32U585xx) || defined(STM32U575xx) || defined(STM32U535xx) || defined(STM32U545xx)
  ADC1_IRQHandler,                  /*  37: ADC1 global interrupt  */
#else /* defined(STM32U595xx) || defined(STM32U599xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
  ADC1_2_IRQHandler,                /*  37: ADC1_2 global interrupt */
#endif /* defined(STM32U585xx) || defined(STM32U575xx) || defined(STM32U535xx) || defined(STM32U545xx) */
  DAC1_IRQHandler,                  /*  38: DAC1 global interrupt */
  FDCAN1_IT0_IRQHandler,            /*  39: FDCAN1 Interrupt 0 */
  FDCAN1_IT1_IRQHandler,            /*  40: FDCAN1 Interrupt 1 */
  TIM1_BRK_IRQHandler,              /*  41: TIM1 Break */
  TIM1_UP_IRQHandler,               /*  42: TIM1 Update */
  TIM1_TRG_COM_IRQHandler,          /*  43: TIM1 Trigger and Commutation */
  TIM1_CC_IRQHandler,               /*  44: TIM1 Capture Compare */
  TIM2_IRQHandler,                  /*  45: TIM2 */
  TIM3_IRQHandler,                  /*  46: TIM3 */
  TIM4_IRQHandler,                  /*  47: TIM4 */
  TIM5_IRQHandler,                  /*  48: TIM5 */
  TIM6_IRQHandler,                  /*  49: TIM6 */
  TIM7_IRQHandler,                  /*  50: TIM7 */
  TIM8_BRK_IRQHandler,              /*  51: TIM8 Break */
  TIM8_UP_IRQHandler,               /*  52: TIM8 Update */
  TIM8_TRG_COM_IRQHandler,          /*  53: TIM8 Trigger and Commutation */
  TIM8_CC_IRQHandler,               /*  54: TIM8 Capture Compare */
  I2C1_EV_IRQHandler,               /*  55: I2C1 Event */
  I2C1_ER_IRQHandler,               /*  56: I2C1 Error */
  I2C2_EV_IRQHandler,               /*  57: I2C2 Event */
  I2C2_ER_IRQHandler,               /*  58: I2C2 Error */
  SPI1_IRQHandler,                  /*  59: SPI1 */
  SPI2_IRQHandler,                  /*  60: SPI2 */
  USART1_IRQHandler,                /*  61: USART1 */
#if defined(STM32U535xx) || defined(STM32U545xx)
  0,                                /*  62: Reserved */
#else
  USART2_IRQHandler,                /*  62: USART2 */
#endif /* defined(STM32U535xx) || defined(STM32U545xx) */
  USART3_IRQHandler,                /*  63: USART3 */
  UART4_IRQHandler,                 /*  64: UART4 */
  UART5_IRQHandler,                 /*  65: UART5 */
  LPUART1_IRQHandler,               /*  66: LP UART1 */
  LPTIM1_IRQHandler,                /*  67: LP TIM1 */
  LPTIM2_IRQHandler,                /*  68: LP TIM2 */
  TIM15_IRQHandler,                 /*  69: TIM15 */
  TIM16_IRQHandler,                 /*  70: TIM16 */
  TIM17_IRQHandler,                 /*  71: TIM17 */
  COMP_IRQHandler,                  /*  72: COMP1&2 */
#if defined(STM32U535xx) || defined(STM32U545xx)
  USB_IRQHandler,                   /*  73, USB global interrupt */
#elif defined(STM32U585xx) || defined(STM32U575xx)
  OTG_FS_IRQHandler,                /*  73: USB OTG FS global interrupt */
#else /* defined(STM32U595xx) || defined(STM32U599xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
  OTG_HS_IRQHandler,                /*  73: USB OTG HS global interrupt */
#endif /* defined(STM32U535xx) || defined(STM32U545xx) */
  CRS_IRQHandler,                   /*  74: CRS */
#if defined(STM32U535xx) || defined(STM32U545xx)
  0,                                /*  75: Reserved */
#else
  FMC_IRQHandler,                   /*  75: FMC */
#endif /* defined(STM32U535xx) || defined(STM32U545xx) */
  OCTOSPI1_IRQHandler,              /*  76: OctoSPI1 global interrupt */
  PWR_S3WU_IRQHandler,              /*  77: PWR wake up from Stop3 interrupt */
  SDMMC1_IRQHandler,                /*  78: SDMMC1 global interrupt */
#if defined(STM32U535xx) || defined(STM32U545xx)
  0,                                /*  79: Reserved */
#else
  SDMMC2_IRQHandler,                /*  79: SDMMC2 global interrupt*/
#endif /* defined(STM32U535xx) || defined(STM32U545xx) */
  GPDMA1_Channel8_IRQHandler,       /*  80: GPDMA1 Channel 8 global interrupt*/
  GPDMA1_Channel9_IRQHandler,       /*  81: GPDMA1 Channel 9 global interrupt*/
  GPDMA1_Channel10_IRQHandler,      /*  82: GPDMA1 Channel 10 global interrupt*/
  GPDMA1_Channel11_IRQHandler,      /*  83: GPDMA1 Channel 11 global interrupt*/
  GPDMA1_Channel12_IRQHandler,      /*  84: GPDMA1 Channel 12 global interrupt*/
  GPDMA1_Channel13_IRQHandler,      /*  85: GPDMA1 Channel 13 global interrupt*/
  GPDMA1_Channel14_IRQHandler,      /*  86: GPDMA1 Channel 14 global interrupt*/
  GPDMA1_Channel15_IRQHandler,      /*  87: GPDMA1 Channel 15 global interrupt*/
  I2C3_EV_IRQHandler,               /*  88: I2C3 event */
  I2C3_ER_IRQHandler,               /*  89: I2C3 error */
  SAI1_IRQHandler,                  /*  90: Serial Audio Interface 1 global interrupt */
#if defined(STM32U535xx) || defined(STM32U545xx)
  0,                                /*  91: Reserved */
#else
  SAI2_IRQHandler,                  /*  91: Serial Audio Interface 2 global interrupt */
#endif /* defined(STM32U535xx) || defined(STM32U545xx) */
  TSC_IRQHandler,                   /*  92: Touch Sense Controller global interrupt */
#if defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U545xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
  AES_IRQHandler,                   /*  93: AES global interrupt */
#else
  0,                                /*  93: Reserved */
#endif /* defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U545xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
  RNG_IRQHandler,                   /*  94: RNG global interrupt */
  FPU_IRQHandler,                   /*  95: FPU */
  HASH_IRQHandler,                  /*  96: HASH global interrupt */
#if defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U545xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
  PKA_IRQHandler,                   /*  97: PKA global interrupt */
#else
  0,                                /*  97: Reserved */
#endif /* defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U545xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
  LPTIM3_IRQHandler,                /*  98: LP TIM3 */
  SPI3_IRQHandler,                  /*  99: SPI3 */
  I2C4_ER_IRQHandler,               /* 100: I2C4 error */
  I2C4_EV_IRQHandler,               /* 101: I2C4 event */
  MDF1_FLT0_IRQHandler,             /* 102: MDF1 Filter 0 global interrupt */
  MDF1_FLT1_IRQHandler,             /* 103: MDF1 Filter 1 global interrupt */
#if defined(STM32U535xx) || defined(STM32U545xx)
  0,                                /* 104: Reserved */
  0,                                /* 105: Reserved */
  0,                                /* 106: Reserved */
#else
  MDF1_FLT2_IRQHandler,             /* 104: MDF1 Filter 2 global interrupt */
  MDF1_FLT3_IRQHandler,             /* 105: MDF1 Filter 3 global interrupt */
  UCPD1_IRQHandler,                 /* 106: UCPD1 global interrupt */
#endif /* defined(STM32U535xx) || defined(STM32U545xx) */
  ICACHE_IRQHandler,                /* 107: Instruction cache global interrupt*/
#if defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
  OTFDEC1_IRQHandler,               /* 108: OTFDEC1 global interrupt */
  OTFDEC2_IRQHandler,               /* 109: OTFDEC2 global interrupt */
#else
  0,                                /* 108: Reserved */
  0,                                /* 109: Reserved */
#endif /* defined(STM32U585xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
  LPTIM4_IRQHandler,                /* 110: LPTIM4 global interrupt */
  DCACHE1_IRQHandler,               /* 111: Data cache global interrupt */
  ADF1_IRQHandler,                  /* 112: ADF interrupt */
  ADC4_IRQHandler,                  /* 113: ADC4 (12bits) global interrupt */
  LPDMA1_Channel0_IRQHandler,       /* 114: LPDMA1 SmartRun Channel 0 global interrupt */
  LPDMA1_Channel1_IRQHandler,       /* 115: LPDMA1 SmartRun Channel 0 global interrupt */
  LPDMA1_Channel2_IRQHandler,       /* 116: LPDMA1 SmartRun Channel 0 global interrupt */
  LPDMA1_Channel3_IRQHandler,       /* 117: LPDMA1 SmartRun Channel 0 global interrupt */
#if defined(STM32U535xx) || defined(STM32U545xx)
  0,                                /* 118: Reserved */
#else
  DMA2D_IRQHandler,                 /* 118: DMA2D global interrupt */
#endif /* defined(STM32U535xx) || defined(STM32U545xx) */
  DCMI_PSSI_IRQHandler,             /* 119: DCMI/PSSI global interrupt */
#if defined(STM32U535xx) || defined(STM32U545xx)
  0,                                /* 120: Reserved */
  0,                                /* 121: Reserved */
  0,                                /* 122: Reserved */
#else
  OCTOSPI2_IRQHandler,              /* 120: OCTOSPI2 global interrupt */
  MDF1_FLT4_IRQHandler,             /* 121: MDF1 Filter 4 global interrupt */
  MDF1_FLT5_IRQHandler,             /* 122: MDF1 Filter 5 global interrupt */
#endif /* defined(STM32U535xx) || defined(STM32U545xx) */
  CORDIC_IRQHandler,                /* 123: CORDIC global interrupt */
  FMAC_IRQHandler,                  /* 124: FMAC global interrupt  */
  LSECSSD_IRQHandler,               /* 125: LSECSSD and MSI_PLL_UNLOCK global interrupt */
#if defined(STM32U595xx) || defined(STM32U599xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
  USART6_IRQHandler,                /* 126: USART6 global interrupt */
  I2C5_ER_IRQHandler,               /* 127: I2C5 error interrupt */
  I2C5_EV_IRQHandler,               /* 128: I2C5 event interrupt */
  I2C6_ER_IRQHandler,               /* 129: I2C6 error interrupt */
  I2C6_EV_IRQHandler,               /* 130: I2C6 event interrupt */
  HSPI1_IRQHandler,                 /* 131: HSPI1 global */
#endif /* defined(STM32U595xx) || defined(STM32U599xx) || defined(STM32U5A5xx) || defined(STM32U5A9xx) || defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
#if defined(STM32U599xx) || defined(STM32U5A9xx) || defined(STM32U5A9xx) || defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
  GPU2D_IRQHandler,                 /* 132: GPU2D global interrupt */
  GPU2D_ER_IRQHandler,              /* 133: GPU2D error interrupt */
  GFXMMU_IRQHandler,                /* 134: GFXMMU global interrupt */
  LTDC_IRQHandler,                  /* 135: LCD-TFT global interrupt */
  LTDC_ER_IRQHandler,               /* 136: LCD-TFT error interrupt */
  DSI_IRQHandler,                   /* 137: DSIHOST global interrupt */
  DCACHE2_IRQHandler,               /* 138: GPU2D Data cache global interrupt */
#endif /* defined(STM32U599xx) || defined(STM32U5A9xx) || defined(STM32U5A9xx) || defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
#if defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx)
  GFXTIM_IRQHandler,                /* 139: GFXTIM global interrupt */
  JPEG_IRQHandler,                  /* 140: JPEG sync interrupt */
#endif /* defined(STM32U5F9xx) || defined(STM32U5G9xx) || defined(STM32U5G7xx) || defined(STM32U5F7xx) */
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

#ifdef TFM_DEV_MODE
#ifdef __ICCARM__
__no_init volatile uint32_t TamperEventCleared;
#else
  volatile uint32_t TamperEventCleared  __attribute__((section(".bss.NoInit")));
#endif /* __ICCARM__ */
#endif /* TFM_DEV_MODE */
/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
  __set_MSPLIM((uint32_t)(&__STACK_LIMIT));
  SystemInit();                             /* CMSIS System Initialization */
  /* active access to tamper register */
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_PWR_EnableBkUpAccess();

  __HAL_RCC_RTC_ENABLE();

  __HAL_RCC_RTCAPB_CLK_ENABLE();
  /* Release reset of back-up domain in case it is set, to avoid blocking the device (system reset
  does not release it) */
  __HAL_RCC_BACKUPRESET_RELEASE();
   /* Get tamper status */
  if (READ_REG(TAMP->SR))
  {
#ifdef TFM_DEV_MODE
#if 1
/* avoid several re-boot in DEV_MODE with Tamper active, clean tamper configuration*/
    __HAL_RCC_BACKUPRESET_FORCE();
    __HAL_RCC_BACKUPRESET_RELEASE();
    /* wait for event being cleared*/
    while(READ_REG(TAMP->SR));
#else
    /* clear tamper event */
    WRITE_REG(TAMP->SCR, READ_REG(TAMP->SR));
#endif
    /* memorize for log that event has been cleared */
    TamperEventCleared=1;
#else
	/* VBAT and VDD must be set to zero to allow board to restart */
	Error_Handler();
#endif /* TFM_DEV_MODE */
  }
  /*  Enable TAMP IRQ , to catch tamper interrupt in TAMP_IRQHandler */
  /*  else a stack in SRAM2 is cleaned a HardFault can occur, at every pop of
   *  function */
#ifndef TFM_DEV_MODE
  HAL_NVIC_EnableIRQ(TAMP_IRQn);
#endif
  __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}
