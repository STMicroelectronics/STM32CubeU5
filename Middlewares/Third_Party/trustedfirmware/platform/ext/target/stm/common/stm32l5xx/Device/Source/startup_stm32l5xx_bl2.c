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

#include "cmsis.h"
#include "region.h"


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
DEFAULT_IRQ_HANDLER(TAMP_IRQHandler_S)
DEFAULT_IRQ_HANDLER(FLASH_IRQHandler)
DEFAULT_IRQ_HANDLER(FLASH_IRQHandler_S)
DEFAULT_IRQ_HANDLER(SERR_IRQHandler)
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
DEFAULT_IRQ_HANDLER(DMAMUX1_IRQHandler)
DEFAULT_IRQ_HANDLER(DMAMUX1_IRQHandler_S)
DEFAULT_IRQ_HANDLER(DMA1_Channel1_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA1_Channel2_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA1_Channel3_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA1_Channel4_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA1_Channel5_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA1_Channel6_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA1_Channel7_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA1_Channel8_IRQHandler)
DEFAULT_IRQ_HANDLER(ADC1_2_IRQHandler)
DEFAULT_IRQ_HANDLER(DAC_IRQHandler)
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
DEFAULT_IRQ_HANDLER(USART2_IRQHandler)
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
DEFAULT_IRQ_HANDLER(USB_FS_IRQHandler)
DEFAULT_IRQ_HANDLER(CRS_IRQHandler)
DEFAULT_IRQ_HANDLER(FMC_IRQHandler)
DEFAULT_IRQ_HANDLER(OCTOSPI1_IRQHandler)
DEFAULT_IRQ_HANDLER(SDMMC1_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA2_Channel1_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA2_Channel2_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA2_Channel3_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA2_Channel4_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA2_Channel5_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA2_Channel6_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA2_Channel7_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA2_Channel8_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C3_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C3_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(SAI1_IRQHandler)
DEFAULT_IRQ_HANDLER(SAI2_IRQHandler)
DEFAULT_IRQ_HANDLER(TSC_IRQHandler)
#ifdef STM32L562xx
DEFAULT_IRQ_HANDLER(AES_IRQHandler)
#endif
DEFAULT_IRQ_HANDLER(RNG_IRQHandler)
DEFAULT_IRQ_HANDLER(FPU_IRQHandler)
DEFAULT_IRQ_HANDLER(HASH_IRQHandler)
#ifdef STM32L562xx
DEFAULT_IRQ_HANDLER(PKA_IRQHandler)
#endif
DEFAULT_IRQ_HANDLER(LPTIM3_IRQHandler)
DEFAULT_IRQ_HANDLER(SPI3_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C4_ER_IRQHandler)
DEFAULT_IRQ_HANDLER(I2C4_EV_IRQHandler)
DEFAULT_IRQ_HANDLER(DFSDM1_FLT0_IRQHandler)
DEFAULT_IRQ_HANDLER(DFSDM1_FLT1_IRQHandler)
DEFAULT_IRQ_HANDLER(DFSDM1_FLT2_IRQHandler)
DEFAULT_IRQ_HANDLER(DFSDM1_FLT3_IRQHandler)
DEFAULT_IRQ_HANDLER(UCPD1_IRQHandler)
DEFAULT_IRQ_HANDLER(ICACHE_IRQHandler)
DEFAULT_IRQ_HANDLER(OTFDEC1_IRQHandler)

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
  HardFault_Handler,                /* -13: Hard Fault Handler */
  MemManage_Handler,                /* -12: MPU Fault Handler */
  BusFault_Handler,                 /* -11: Bus Fault Handler */
  UsageFault_Handler,               /* -10: Usage Fault Handler */
  SecureFault_Handler,              /*  -9: Secure Fault Handler */
  0,                                /*      Reserved */
  0,                                /*      Reserved */
  0,                                /*      Reserved */
  SVC_Handler,                      /*  -5: SVCall Handler */
  DebugMon_Handler,                 /*  -4: Debug Monitor Handler */
  0,                                /*      Reserved */
  PendSV_Handler,                   /*  -2: PendSV Handler */
  SysTick_Handler,                  /*  -1: SysTick Handler */
  WWDG_IRQHandler,                  /*   0: Window WatchDog */
  PVD_PVM_IRQHandler,               /*   1: PVD/PVM1/PVM2/PVM3/PVM4 through EXTI Line detection */
  RTC_IRQHandler,                   /*   2: RTC non-secure interrupts through the EXTI line */
  RTC_IRQHandler_S,                 /*   3: RRTC secure interrupts through the EXTI line */
  TAMP_IRQHandler,                  /*   4: RTamper non-secure interrupts through the EXTI line */
  TAMP_IRQHandler_S,                /*   5: RTamper secure interrupts through the EXTI line */
  FLASH_IRQHandler,                 /*   6: RFLASH non-secure interrupts */
  FLASH_IRQHandler_S,               /*   7: RFLASH secure global interrupts */
  SERR_IRQHandler,                  /*   8: RSecure Error interrupts */
  RCC_IRQHandler,                   /*   9: RRCC non-secure global interrupts */
  RCC_IRQHandler_S,                 /*  10: RRCC secure global interrupts */
  EXTI0_IRQHandler,                 /*  11: REXTI Line0 */
  EXTI1_IRQHandler,                 /*  12: REXTI Line1 */
  EXTI2_IRQHandler,                 /*  13: REXTI Line2 */
  EXTI3_IRQHandler,                 /*  14: REXTI Line3 */
  EXTI4_IRQHandler,                 /*  15: REXTI Line4 */
  EXTI5_IRQHandler,                 /*  16: REXTI Line5 */
  EXTI6_IRQHandler,                 /*  17: REXTI Line6 */
  EXTI7_IRQHandler,                 /*  18: REXTI Line7 */
  EXTI8_IRQHandler,                 /*  19: REXTI Line8 */
  EXTI9_IRQHandler,                 /*  20: REXTI Line9 */
  EXTI10_IRQHandler,                /*  21: EXTI Line10 */
  EXTI11_IRQHandler,                /*  22: EXTI Line11 */
  EXTI12_IRQHandler,                /*  23: EXTI Line12 */
  EXTI13_IRQHandler,                /*  24: EXTI Line13 */
  EXTI14_IRQHandler,                /*  25: EXTI Line14 */
  EXTI15_IRQHandler,                /*  26: EXTI Line15 */
  DMAMUX1_IRQHandler,               /*  27: DMAMUX1 non-secure */
  DMAMUX1_IRQHandler_S,             /*  28: DMAMUX1 secure */
  DMA1_Channel1_IRQHandler,         /*  29: DMA1 Channel 1 */
  DMA1_Channel2_IRQHandler,         /*  30: DMA1 Channel 2 */
  DMA1_Channel3_IRQHandler,         /*  31: DMA1 Channel 3 */
  DMA1_Channel4_IRQHandler,         /*  32: DMA1 Channel 4 */
  DMA1_Channel5_IRQHandler,         /*  33: DMA1 Channel 5 */
  DMA1_Channel6_IRQHandler,         /*  34: DMA1 Channel 6 */
  DMA1_Channel7_IRQHandler,         /*  35: DMA1 Channel 7 */
  DMA1_Channel8_IRQHandler,         /*  36: DMA1 Channel 8 */
  ADC1_2_IRQHandler,                /*  37: ADC1 & ADC2 */
  DAC_IRQHandler,                   /*  38: DAC1&2 underrun errors */
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
  USART2_IRQHandler,                /*  62: USART2 */
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
  USB_FS_IRQHandler,                /*  73: USB FS */
  CRS_IRQHandler,                   /*  74: CRS */
  FMC_IRQHandler,                   /*  75: FMC */
  OCTOSPI1_IRQHandler,              /*  76: OctoSPI1 global interrupt */
  0,                                /*  77: Reserved */
  SDMMC1_IRQHandler,                /*  78: SDMMC1 */
  0,                                /*  79: Reserved */
  DMA2_Channel1_IRQHandler,         /*  80: DMA2 Channel 1 */
  DMA2_Channel2_IRQHandler,         /*  81: DMA2 Channel 2 */
  DMA2_Channel3_IRQHandler,         /*  82: DMA2 Channel 3 */
  DMA2_Channel4_IRQHandler,         /*  83: DMA2 Channel 4 */
  DMA2_Channel5_IRQHandler,         /*  84: DMA2 Channel 5 */
  DMA2_Channel6_IRQHandler,         /*  85: DMA2 Channel 6 */
  DMA2_Channel7_IRQHandler,         /*  86: DMA2 Channel 7 */
  DMA2_Channel8_IRQHandler,         /*  87: DMA2 Channel 8 */
  I2C3_EV_IRQHandler,               /*  88: I2C3 event */
  I2C3_ER_IRQHandler,               /*  89: I2C3 error */
  SAI1_IRQHandler,                  /*  90: Serial Audio Interface 1 global interrupt */
  SAI2_IRQHandler,                  /*  91: Serial Audio Interface 2 global interrupt */
  TSC_IRQHandler,                   /*  92: Touch Sense Controller global interrupt */
#ifdef STM32L562xx
  AES_IRQHandler,                   /*  93: AES global interrupt */
#else
  0,                                /*  93: Reserved */
#endif
  RNG_IRQHandler,                   /*  94: RNG global interrupt */
  FPU_IRQHandler,                   /*  95: FPU */
  HASH_IRQHandler,                  /*  96: HASH global interrupt */
#ifdef STM32L562xx
  PKA_IRQHandler,                   /*  97: PKA global interrupt */
#else
  0,                                /*  97: Reserved */
#endif
  LPTIM3_IRQHandler,                /*  98: LP TIM3 */
  SPI3_IRQHandler,                  /*  99: SPI3 */
  I2C4_ER_IRQHandler,               /* 100: I2C4 error */
  I2C4_EV_IRQHandler,               /* 101: I2C4 event */
  DFSDM1_FLT0_IRQHandler,           /* 102: DFSDM1 Filter 0 global Interrupt */
  DFSDM1_FLT1_IRQHandler,           /* 103: DFSDM1 Filter 1 global Interrupt */
  DFSDM1_FLT2_IRQHandler,           /* 104: DFSDM1 Filter 2 global Interrupt */
  DFSDM1_FLT3_IRQHandler,           /* 105: DFSDM1 Filter 3 global Interrupt */
  UCPD1_IRQHandler,                 /* 106: UCPD1 */
  ICACHE_IRQHandler,                /* 107: ICACHE */
  OTFDEC1_IRQHandler                /* 108: OTFDEC1 */
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
  __set_MSPLIM((uint32_t)(&__STACK_LIMIT));
  SystemInit();                             /* CMSIS System Initialization */
  __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}
