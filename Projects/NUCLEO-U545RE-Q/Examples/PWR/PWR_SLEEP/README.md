## <b>PWR_SLEEP Example Description</b>

How to enter the Sleep mode and wake up from this mode by using an interrupt.

In the associated software, the system clock is set to 160 MHz.
An EXTI line is connected to the user button through PC.13 and configured
to generate an interrupt on falling edge upon key press.
The SysTick is programmed to generate an interrupt each 1 ms and in the SysTick
interrupt handler, LED2 is toggled in order to indicate whether the MCU is in SLEEP mode
or RUN mode.

5 seconds after start-up, the system automatically enters SLEEP mode and
LED2 stops toggling.
The User push-button can be pressed at any time to wake-up the system.
The software then comes back in RUN mode for 5 sec. before automatically entering SLEEP mode again.

LED2 is used to monitor the system state as follows:

 - LED2 toggling: system in RUN mode
 - LED2 off : system in SLEEP mode

These steps are repeated in an infinite loop.

#### <b>Notes</b>

 1. To measure the current consumption in SLEEP mode, remove JP4 jumper
    and connect an amperemeter to JP4 to measure IDD current.

 2. This example can not be used in DEBUG mode due to the fact
    that the Cortex-M33 core is no longer clocked during low power mode
    so debugging features are disabled.

 3. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 4. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Power, PWR, EXTI, Sleep mode, Interrupt, Wakeup, External reset

### <b>Directory contents</b>

  - PWR/PWR_SLEEP/Inc/stm32u5xx_nucleo_conf.h     BSP configuration file
  - PWR/PWR_SLEEP/Inc/stm32u5xx_conf.h         HAL Configuration file
  - PWR/PWR_SLEEP/Inc/stm32u5xx_it.h           Header for stm32u5xx_it.c
  - PWR/PWR_SLEEP/Inc/main.h                         Header file for main.c
  - PWR/PWR_SLEEP/Src/system_stm32u5xx.c       STM32U5xx system clock configuration file
  - PWR/PWR_SLEEP/Src/stm32u5xx_it.c           Interrupt handlers
  - PWR/PWR_SLEEP/Src/stm32u5xx_hal_msp.c      HAL MSP module
  - PWR/PWR_SLEEP/Src/main.c                         Main program

### <b>Hardware and Software environment</b>

  - This example runs on STM32U5xx devices

  - This example has been tested with STMicroelectronics NUCLEO-U545RE-Q
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U545RE-Q set-up:
    - LED2 connected to PA.05 pin
    - Use the User push-button connected to pin PC.13 (External line 13)

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

