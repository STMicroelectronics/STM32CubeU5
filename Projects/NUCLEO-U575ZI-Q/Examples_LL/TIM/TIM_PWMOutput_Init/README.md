## <b>TIM_PWMOutput_Init Example Description</b>

Use of a timer peripheral to generate a PWM output signal and update the PWM duty cycle.  
This example is based on the STM32U5xx TIM LL API.  
The peripheral initialization uses LL initialization function to demonstrate LL Init.

In this example TIM2 input clock TIM2CLK is set to APB1 clock (PCLK1),
    since APB1 pre-scaler is equal to 1.

    TIM2CLK = PCLK1  
    PCLK1 = HCLK  
    => TIM2CLK = HCLK = SystemCoreClock (160 MHz)

- To set the TIM2 counter clock frequency to 10 KHz, the pre-scaler (PSC) is calculated as follows:  
  PSC = (TIM2CLK / TIM2 counter clock) - 1  
  PSC = (SystemCoreClock /10 KHz) - 1

- SystemCoreClock is set to 160 MHz for STM32U5xx Devices.

- Auto-reload (ARR) is calculated to get a time base period of 10 ms,
  meaning a time base frequency of 100 Hz.  
  ARR = (TIM2 counter clock / time base frequency) - 1  
  ARR = (TIM2 counter clock / 100) - 1  
- Initially, the capture/compare register (CCR1) of the output channel is set to
  half the auto-reload value meaning a initial duty cycle of 50%.  
  Generally speaking this duty cycle is calculated as follows:
   Duty cycle = (CCR1 / ARR) * 100

- The timer output channel must be connected to TIM2_CH1 on board NUCLEO-U575ZI-Q.  
  Thus TIM2_CH1 status (on/off) mirrors the timer output level (active v.s. inactive).

User push-button can be used to change the duty cycle from 0% up to 100% by
steps of 10%. Duty cycle is periodically measured.  
It can be observed through the debugger by watching the variable uwMeasuredDutyCycle.

### <b>Keywords</b>

Timers, TIM, Output, signal, PWM, Oscilloscope, Frequency, Duty cycle, Waveform

### <b>Directory contents</b>

  - TIM/TIM_PWMOutput_Init/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - TIM/TIM_PWMOutput_Init/Inc/main.h                  Header for main.c module
  - TIM/TIM_PWMOutput_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_PWMOutput_Init/Src/stm32u5xx_it.c          Interrupt handlers
  - TIM/TIM_PWMOutput_Init/Src/main.c                  Main program
  - TIM/TIM_PWMOutput_Init/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.
  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U575ZI-Q Set-up:
  
      - TIM2_CH1  PA5: connected to pin 10 of CN7 connector 

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain  
 - Rebuild all files and load your image into target memory  
 - Run the example

