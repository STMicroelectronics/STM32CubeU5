## <b>TIM_PWMInput Example Description</b>

Use of the TIM peripheral to measure an external signal frequency and duty cycle.  
The TIM2CLK frequency is set to SystemCoreClock/1 (Hz), the Prescaler is 0 so the counter clock is SystemCoreClock/1 (Hz).  
TIM2 is configured in PWM Input Mode: the external signal is connected to TIM2 Channel2 used as input pin.

To measure the frequency and the duty cycle, we use the TIM2 CC2 interrupt request,
so in the TIM2_IRQHandler routine, the frequency and the duty cycle of the external
signal are computed.

- "uwFrequency" variable contains the external signal frequency:  
  - TIM2 counter clock = SystemCoreClock / 1,
  - uwFrequency = TIM2 counter clock / TIM2_CCR2 in Hz,

"uwDutyCycle" variable contains the external signal duty cycle:  
 uwDutyCycle = (TIM2_CCR1*100) / (TIM2_CCR2) in %.

The minimum frequency value to measure is (TIM2 counter clock / CCR MAX)  
                                         = (160 MHz/1)/ 65535  
                                         = 2441,4 Hz

TIM1 can be used to generate the external signal in case a function generator
is not available. TIM1 is configured in PWM Output Mode to produce a square wave on PA8.  
Frequency and duty cycles can be changed by pressing the User Button (PC.13).  
Six combinations are available (see tables aFrequency[] and aDutyCycle[]).

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.  
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.  
 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Timers, Input, signals, PWM, External signal, Frequency, Duty cycle, Measure

### <b>Directory contents</b>

  - TIM/TIM_PWMInput/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - TIM/TIM_PWMInput/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - TIM/TIM_PWMInput/Inc/main.h                  Header for main.c module
  - TIM/TIM_PWMInput/Src/stm32u5xx_it.c          Interrupt handlers
  - TIM/TIM_PWMInput/Src/main.c                  Main program
  - TIM/TIM_PWMInput/Src/stm32u5xx_hal_msp.c     HAL MSP file
  - TIM/TIM_PWMInput/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.  
  - This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q
    board and can be easily tailored to any other supported device
    and development board.  
  - NUCLEO-U575ZI-Q Set-up  
    - You can either Connect the external signal to measure to the TIM2 CH2 pin (PA1) (pin 11 in CN10 connector).  
    - Or connect TIM1 CH1 pin PA8 (pin 31 in CN10 connector) to the TIM2 CH2 pin (PA1) instead of an external signal.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain  
 - Rebuild all files and load your image into target memory  
 - Run the example

