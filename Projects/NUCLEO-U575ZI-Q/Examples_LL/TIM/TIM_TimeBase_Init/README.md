## <b>TIM_TimeBase_Init Example Description</b>

- Configuration of the TIM peripheral to generate a timebase.  
This example is based on the STM32U5xx TIM LL API.  
The peripheral initialization 
uses LL unitary service functions for optimization purposes (performance and size).

- In this example TIM1 input clock TIM1CLK is set to APB2 clock PCLK2 (TIM1CLK = PCLK2).  
As APB2 pre-scaler is equal to 1 PCLK2 = HCLK, and since AHB pre-scaler is equal to 1,  
      => TIM1CLK = SystemCoreClock (160 MHz)

- To set the TIM1 counter clock frequency to 10 KHz, the pre-scaler (PSC) is calculated as follows:  
PSC = (TIM1CLK / TIM1 counter clock) - 1  
PSC = (SystemCoreClock /10 KHz) - 1  

- SystemCoreClock is set to 160 MHz for STM32U5xx Devices.

- The auto-reload (ARR) is calculated to get a timebase period of 100ms, meaning that initial timebase frequency is 10 Hz.
  - ARR = (TIM1 counter clock / timebase frequency) - 1
  - ARR = (TIM1 counter clock / 10) - 1

- Update interrupts are enabled. Within the update interrupt service routine, pin PC7
  (connected to LED1 on board NUCLEO-U575ZI-Q) is toggled. So the period of blinking of LED1 = 2 * timebase period.

- User push-button can be used to modify the timebase period from 100 ms to 1 s in 100 ms steps.  
  To do so, every time User push-button is pressed, the autoreload register (ARR) is updated.  
  In up-counting update event is generated at each counter overflow (when the counter reaches the auto-reload value). 

- Finally the timebase frequency is calculated as follows:  
  timebase frequency = TIM1 counter clock / ((PSC + 1) * (ARR + 1) * (RCR + 1))

### <b>Keywords</b>

Timer, TIM, timebase, Interrupt, Clock source

### <b>Directory contents</b>

  - TIM/TIM_TimeBase_Init/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - TIM/TIM_TimeBase_Init/Inc/main.h                  Header for main.c module
  - TIM/TIM_TimeBase_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - TIM/TIM_TimeBase_Init/Src/stm32u5xx_it.c          Interrupt handlers
  - TIM/TIM_TimeBase_Init/Src/main.c                  Main program
  - TIM/TIM_TimeBase_Init/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain  
 - Rebuild all files and load your image into target memory  
 - Run the example

