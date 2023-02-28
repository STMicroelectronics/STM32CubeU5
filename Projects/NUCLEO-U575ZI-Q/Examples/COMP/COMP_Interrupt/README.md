## <b>COMP_Interrupt Example Description</b>

How to use a comparator peripheral to compare a voltage level applied on a GPIO pin to the the internal voltage reference (VREFINT), in interrupt mode.

This project is targeted to run on STM32U575xx devices on NUCLEO-U575ZI-Q board from STMicroelectronics.

The reference template project configures the maximum system clock frequency at 160Mhz.

When the comparator input crosses (either rising or falling edges) the internal reference voltage VREFINT (1.22V), the comparator generates an interrupt
and system wakes-up from Stop mode.

The System enters Stop mode 5 seconds after the comparator is started and after any system wake-up triggered by the comparator interrupt.

Before entering in Stop mode, LED1 toggles 4 times quickly.

In this example, the comparator input plus is connected on pin PB.02.

when system wakes-up from Stop mode, there is a delay of 5 seconds then LED1 toggles 4 times quickly.

If LED1 is toggling continuously without any voltage update, it indicates that the system
generated an error.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

 3. This example can not be used in debug mode (debugger attached) due to the fact
    that Cortex-M33 SysTick would remain active and induce exit from low power mode.
    Example must be used in application mode: once the image is loaded, power off the board by unplugging
    the power cable then power on the board again.

### <b>Keywords</b>

Analog, COMP, Comparator, Voltage, VrefInt, Wake Up, Low power, Stop2

### <b>Directory contents</b>

  - COMP/COMP_Interrupt/Src/main.c                  Main program
  - COMP/COMP_Interrupt/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - COMP/COMP_Interrupt/Src/stm32u5xx_it.c          Interrupt handlers
  - COMP/COMP_Interrupt/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - COMP/COMP_Interrupt/Inc/main.h                  Main program header file
  - COMP/COMP_Interrupt/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
  - COMP/COMP_Interrupt/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - COMP/COMP_Interrupt/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This template runs on STM32U575xx devices without security enabled (TZEN=0).
  - In this example, the clock is set to 160 MHz.

  - This template has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U575ZI-Q Set-up

    - Use GREEN LED1 connected to PC.07 pin
    - Apply an external variable voltage on PB.02 (connected on pin 13 on CN9) with average voltage 1.22V.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Once the image is loaded, power off the board by unplugging
   the power cable then power on the board again
 - Run the example

