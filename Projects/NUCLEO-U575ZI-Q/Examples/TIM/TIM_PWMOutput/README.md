## <b>TIM_PWMOutput Example Description</b>

- This example shows how to configure the TIM peripheral in PWM (Pulse Width Modulation) mode.

- At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.  
The SystemClock_Config() function is used to configure the system clock for STM32U575ZITxx devices :  
     The CPU at 160 MHz 

- SystemCoreClock is set to 160 MHz for STM32U5xx devices.

- In this example TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2), since APB1 prescaler is equal to 1.  
      TIM1CLK = PCLK2
      PCLK2 = HCLK
      => TIM1CLK = HCLK = SystemCoreClock

- To get TIM1 counter clock at 1 MHz, the prescaler is computed as follows:  
       Prescaler = (TIM1CLK / TIM1 counter clock) - 1
       Prescaler = ((SystemCoreClock) /1 MHz) - 1

- To get TIM1 output clock at 24 KHz, the period (ARR)) is computed as follows:  
       ARR = (TIM1 counter clock / TIM1 output clock) - 1
           = 40

    - TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR + 1)* 100 = 50%
    - TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM1_ARR + 1)* 100 = 37.5%
    - TIM1 Channel3 duty cycle = (TIM1_CCR3/ TIM1_ARR + 1)* 100 = 25%
    - TIM1 Channel4 duty cycle = (TIM1_CCR4/ TIM1_ARR + 1)* 100 = 12.5%

- LED3 is ON when there are an error.

- The PWM waveforms can be displayed using an oscilloscope.

#### <b>Notes</b>

 1. The duty cycles values mentioned above are theoretical (obtained when the system clock frequency is exactly 160 MHz).
    They might be slightly different depending on system clock frequency precision.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.  
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. This example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Motor control, TIM, Output, signal, PWM, Oscilloscope, Frequency, Duty cycle, Waveform.

### <b>Directory contents</b>

  - TIM/TIM_PWMOutput/Inc/stm32u5xx_nucleo_conf.h BSP configuration file
  - TIM/TIM_PWMOutput/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - TIM/TIM_PWMOutput/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - TIM/TIM_PWMOutput/Inc/main.h                  Header for main.c module  
  - TIM/TIM_PWMOutput/Src/stm32u5xx_it.c          Interrupt handlers
  - TIM/TIM_PWMOutput/Src/main.c                  Main program
  - TIM/TIM_PWMOutput/Src/stm32u5xx_hal_msp.c     HAL MSP file
  - TIM/TIM_PWMOutput/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.
  - In this example, the clock is set to 160 MHz.

  - This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q 
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-U575ZI-Q Set-up  
Connect the following pins to an oscilloscope to monitor the different waveforms:  
        - TIM1_CH1 : PE.09 (pin 4 in CN10 connector)  
        - TIM1_CH2 : PE.11 (pin 6 in CN10 connector)  
        - TIM1_CH3 : PE.13 (pin 10 in CN10 connector)  
        - TIM1_CH4 : PE.14 (pin 28 in CN10 connector)  

### <b>How to use it ?</b>

In order to make the program work, you must do the following :  
 - Open your preferred toolchain  
 - Rebuild all files and load your image into target memory  
 - Run the example

