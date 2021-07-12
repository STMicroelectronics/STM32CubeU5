## <b>PWR_EnterStopMode Example Description</b>

-   How to enter the STOP 0 mode.

-   After start-up LED1 is toggling during 5 seconds, then the system automatically
enter in STOP 0 mode (Final state).

-   LED1 is used to monitor the system state as follows:
    -   **LED1 toggling** : system in Run mode.
    -   **LED1 off** : system in STOP 0 mode.

-   This example does not implement a wake-up source from any peripheral: to wake-up the device,
press on Reset button.

#### <b>Notes</b>
 1. This example is not meant to reflect typical product power consumption values but shows how to use the software to switch from power modes.
For power consumption measurement, please check the PWR_ModesSelection example under  available Projects\NUCLEO-U575ZI-Q\Examples\PWR\PWR_ModesSelection folder.

 2. This example can not be used in DEBUG mode due to the fact
      that the Cortex-M33 core is no longer clocked during low power mode
      so debugging features are disabled.

### <b>Keywords</b>

Power, PWR, Stop mode, Interrupt, Low Power

### <b>Directory contents</b>

-   PWR/PWR_EnterStopMode/Inc/stm32u5xx_it.h          Interrupt handlers header file
-   PWR/PWR_EnterStopMode/Inc/main.h                  Header for main.c module
-   PWR/PWR_EnterStopMode/Inc/stm32_assert.h          Template file to include assert_failed function
-   PWR/PWR_EnterStopMode/Src/stm32u5xx_it.c          Interrupt handlers
-   PWR/PWR_EnterStopMode/Src/main.c                  Main program
-   PWR/PWR_EnterStopMode/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

-   This example runs on STM32U575xx devices.

-   This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q Set-up
    -   LED1 connected to PC.07 pin

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example

