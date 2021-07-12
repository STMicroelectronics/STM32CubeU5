## <b>TIM_OCInactive Example Description</b> 

Configuration of the TIM peripheral in Output Compare Inactive mode 
with the corresponding Interrupt requests for each channel.

  The TIM1 frequency is set to SystemCoreClock, and the objective is
  to get TIM1 counter clock at 10 kHz so the Prescaler is computed as following:  
     - Prescaler = (TIM1CLK /TIM1 counter clock) - 1
   
  SystemCoreClock is set to 160 MHz for STM32U5xx Devices.

  The TIM1 CCR1 register value is equal to 10000:  
  TIM1_CH1 delay = CCR1_Val/TIM1 counter clock  = 1s
  so the TIM1 Channel 1 generates a signal with a delay equal to 1s.

  The TIM1 CCR2 register value is equal to 5000:  
  TIM1_CH2 delay = CCR2_Val/TIM1 counter clock = 500 ms
  so the TIM1 Channel 2 generates a signal with a delay equal to 500 ms.

  The TIM1 CCR3 register value is equal to 2500:  
  TIM1_CH3 delay = CCR3_Val/TIM1 counter clock = 250 ms
  so the TIM1 Channel 3 generates a signal with a delay equal to 250 ms.

  The TIM1 CCR4 register value is equal to 1250:  
  TIM1_CH4 delay = CCR4_Val/TIM1 counter clock = 125 ms
  so the TIM1 Channel 4 generates a signal with a delay equal to 125 ms.

  While the counter is lower than the Output compare registers values, which 
  determines the Output delay, the PA5, PA6, PA7 and PD14 pin are turned ON. 

  When the counter value reaches the Output compare registers values, the Output 
  Compare interrupts are generated and, in the handler routine, these pins are turned OFF.
  
#### <b>Notes</b>

 1. Delay values mentioned above are theoretical (obtained when the system clock frequency 
    is exactly 160 MHz). Since the generated system clock frequency may vary from one board to another observed
    delay might be slightly different.
  
 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.  
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. This example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Timer, TIM, Output, Compare, InActive, Signals, Delay, Frequency.

### <b>Directory contents</b>

  - TIM/TIM_OCInactive/Inc/stm32u5xx_nucleo_conf.h BSP configuration file
  - TIM/TIM_OCInactive/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - TIM/TIM_OCInactive/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - TIM/TIM_OCInactive/Inc/main.h                  Header for main.c module  
  - TIM/TIM_OCInactive/Src/stm32u5xx_it.c          Interrupt handlers
  - TIM/TIM_OCInactive/Src/main.c                  Main program
  - TIM/TIM_OCInactive/Src/stm32u5xx_hal_msp.c     HAL MSP file
  - TIM/TIM_OCInactive/Src/system_stm32u5xx.c      STM32U5xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.
    
  - This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q 
    board and can be easily tailored to any other supported device 
    and development board.

  - NUCLEO-U575ZI-Q Set-up  
   Connect the following pins to an oscilloscope to monitor the different waveforms:  
      - PA5: wave form of (TIM1_CH1) ((pin 10 in CN7 connector))
      - PA6: wave form of (TIM1_CH2) ((pin 12 in CN7 connector))
      - PA7: wave form of (TIM1_CH3) ((pin 14 in CN7 connector))
      - PD14: wave form of (TIM1_CH4) ((pin 16 in CN7 connector))

### <b>How to use it ?</b> 

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
