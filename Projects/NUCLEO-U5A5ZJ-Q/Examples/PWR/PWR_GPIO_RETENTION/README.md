## <b>PWR_GPIO_RETENTION Example Description</b>

-   This example demonstrates how to configure GPIO pull-up and pull-down settings during standby mode and after wakeup on an STM32 board using the HAL API.

-   In this example, the code configures two GPIOs in output push-pull mode and applies pull-up and pull-down configurations.
These settings are retained during and after wake-up until the software clears the GPIO pull-up and pull-down configuration.

-   **PF.08**  is configured in output push-pull mode with no pull and initialized to a low logical state.
-   **PB.02**  is configured in output push-pull mode with no pull and initialized to a high logical state.

-   These configurations ensure that the GPIO states are maintained during Standby mode and after wake-up.
The pull-up and pull-down configurations are applied using the 'HAL_PWREx_EnablePullUpPullDownConfig()' function.

-   The system starts by blinking LED2 for 500 milliseconds, then enters standby mode, turning off LED2.
After waking up, the system clears the standby flag, disables pull-up and pull-down settings, and starts blinking LED2 to indicate run mode.

-   The PC.13 is configured to wake up the system from standby mode when a rising edge is detected.

In this example, HCLK is configured at 160 MHz.

-   Two leds LED2 and LED3 are used to monitor the system state as follows:

    -   **LED3 ON**: configuration failed (system will go to an infinite loop).
    -   **LED2 toggling**: system in Run mode.
    -   **LED2 off** : system in Standby mode.

#### <b>Notes</b>
 1. To measure the current consumption in Standby mode, remove JP5 jumper
    and connect an amperemeter to JP5 to measure IDD current.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

 4. This application can not be used in DEBUG mode and Cortex-M33 core is no longer clocked during low power mode so debugging features are disabled.

### <b>Keywords</b>

System, GPIO, Power, Low Power, Alternate function, Push-pull, Retain, APC, Standby mode

### <b>Directory contents</b>

-   PWR/PWR_GPIO_RETENTION/Inc/stm32u5xx_nucleo_conf.h BSP configuration file
-   PWR/PWR_GPIO_RETENTION/Inc/stm32u5xx_hal_conf.h    HAL configuration file
-   PWR/PWR_GPIO_RETENTION/Inc/stm32u5xx_it.h          Interrupt handlers header file
-   PWR/PWR_GPIO_RETENTION/Inc/main.h                  Header for main.c module
-   PWR/PWR_GPIO_RETENTION/Src/stm32u5xx_it.c          Interrupt handlers
-   PWR/PWR_GPIO_RETENTION/Src/stm32u5xx_hal_msp.c     HAL MSP file
-   PWR/PWR_GPIO_RETENTION/Src/main.c                  Main program
-   PWR/PWR_GPIO_RETENTION/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

-   This example runs on **STM32U5A5xx** devices.

-   This example has been tested with **NUCLEO-U5A5ZJ-Q**  board and can be
    easily tailored to any other supported device and development board.

-   NUCLEO-U5A5ZJ-Q Set-up
    -   Use LED2 connected PB.07 pin
    -   Use LED3 connected PG.02 pin
    -   Connect PB.02 and PF.08 pins to an oscilloscope to check that the IOs keeps the same logical state in run and standby modes.
    -   User push-button connected to pin PC.13 (External line 13)

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :

    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example

