## <b>GPIO_Demo Example Description</b>

How to configure external interrupt lines.

This example shows how to use the NUCLEO-U575ZI-Q board to make an LED blink at different 
speeds when you press a button.

In this example:

- External line 13 is connected to PC.13 pin.
- when falling edge is detected on External line 13 by pressing User push-button, LEDs blink at different 
speeds.

On NUCLEO-U575ZI-Q:

- External line 13 is connected to User push-button

In this example, HCLK is configured at 160 MHz.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

System, GPIO, EXTI, Output, Alternate function, Push-pull, Toggle

### <b>Directory contents</b>

  - GPIO/GPIO_Demo/Inc/stm32u5xx_nucleo_conf.h BSP configuration file
  - GPIO/GPIO_Demo/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - GPIO/GPIO_Demo/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - GPIO/GPIO_Demo/Inc/main.h                  Header for main.c module  
  - GPIO/GPIO_Demo/Src/stm32u5xx_it.c          Interrupt handlers
  - GPIO/GPIO_Demo/Src/stm32u5xx_hal_msp.c     HAL MSP file
  - GPIO/GPIO_Demo/Src/main.c                  Main program
  - GPIO/GPIO_Demo/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


