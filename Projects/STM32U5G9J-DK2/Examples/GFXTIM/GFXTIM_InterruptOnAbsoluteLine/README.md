
---
pagetitle: Readme
lang: en
---
::: {.row}
::: {.col-sm-14 .col-lg-10}

## <b>GFXTIM_InterruptOnAbsoluteLine Example Description</b>

Use of a GFXTIM peripheral to generate interrupts for specific absolute lines.

This example is based on the STM32U5XX GFXTIM HAL APIs. 

In this example the Clock Generator is set in Standalone configuration.

The SystemClock_Config() function is used to configure the system clock  (SYSCLK) to run at 160 MHz.

Based on the system clock (SYSCK) frequency, the Line clock counter is configured to generate a Line clock at 1KHz.

	Line clock counter CLK source = SYSCLOCK
	Line clock counter Auto-reload = (SYSCLOCK / 1000) - 1
	=> Line CK = 1KHz

The absolute timer is set to generate interrupts for <i>Line CK</i> 1500 and <i>Line CK</i> 3000,
which corresponds to 1.5 and 3 seconds respectively.

Line Compare interrupts are enabled to generate a GFXTIM interrupt when Line counter value reaches compare register value.

The configuration of the Absolute timer is set as follow:

	Line compare 1 value = 1500
	Line compare 2 value = 3000
	Line compare 1 interrupt = Enable
	Line compare 2 interrupt = Enable

The example performs the following actions:

  - Initially the User LEDs are Off.
  - Set <i>Line CK</i> configuration using <i>HAL_GFXTIM_ClockGenerator_Config</i> function.
  - Set Absolute Timer configuration using <i>HAL_GFXTIM_AbsoluteTimer_Config</i> function.
  - Call the function <i>HAL_GFXTIM_AbsoluteTimer_Start</i> to Enable the absolute Timer counters.
  - The User [Green LED]{style="color: green"} is On if the test ends properly.
  - The [Red LED]{style="color: red"} is On in case of test error.

**NOTE** 

Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
  
### <b>Keywords</b>

Timers, Display, Graphics, GFXTIM, Frequency

### <b>Directory contents</b>

  - GFXTIM/GFXTIM_InterruptOnAbsoluteLine/Core/Inc/main.h &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; Header for main.c module 
  - GFXTIM/GFXTIM_InterruptOnAbsoluteLine/Core/Inc/stm32u5g9j_discovery_conf.h &emsp; STM32U5G9J-DK2 board configuration file
  - GFXTIM/GFXTIM_InterruptOnAbsoluteLine/Core/Inc/stm32u5xx_hal_conf.h &emsp; &emsp; &emsp; &nbsp; HAL configuration file
  - GFXTIM/GFXTIM_InterruptOnAbsoluteLine/Core/Inc/stm32u5xx_it.h &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; Interrupt handlers header file
  - GFXTIM/GFXTIM_InterruptOnAbsoluteLine/Core/Src/main.c &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &emsp; &nbsp; &nbsp; Header for main.c module
  - GFXTIM/GFXTIM_InterruptOnAbsoluteLine/Core/Src/stm32u5xx_hal_msp.c &emsp; &emsp; &emsp; &nbsp; HAL MSP module
  - GFXTIM/GFXTIM_InterruptOnAbsoluteLine/Core/Src/stm32u5xx_it.c &emsp; &emsp; &emsp; &emsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;Interrupt handlers
  - GFXTIM/GFXTIM_InterruptOnAbsoluteLine/Core/Src/system_stm32u5xx.c &emsp; &emsp; &emsp; &nbsp; &nbsp; STM32U5xx system source file


### <b>Hardware and Software environment</b>
  - This example runs on STM32U5G9XX devices.

  - This example has been tested with STM32U5G9J-DK2 board and can be
    easily tailored to any other supported device and development board. 

  - The example needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.  

  - Care must be taken because of the use HAL_GetTick(), this function provides accurate time (in milliseconds)
      based on variable incremented in SysTick ISR. 

      The HAL_GetTick() is called from GFXTIM ISR process, so the the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


:::
:::


:::
:::

