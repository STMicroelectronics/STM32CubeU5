## <b>PWR_EnterStandbyMode Example Description</b>

-   How to enter the Standby mode and wake up from this mode by using an external
reset or a wakeup pin.

-   In the associated software, the system clock is set to 160 MHz.
An EXTI line is connected to the User push-button through PC.13 and configured to generate an
interrupt on Rising edge upon key press.

-   Request to enter in standby mode:
When a Rising edge is detected on the EXTI line, an interrupt is generated.
In the EXTI handler routine, the wake-up pin LL_PWR_WAKEUP_PIN2 is enabled and the
corresponding wake-up flag cleared. 

Then, the system enters Standby mode causing **LED1** to stop toggling.

-   Exit from Standby mode :
    -   The user can wake-up the system in pressing the User push-button which is
connected to the wake-up pin LL_PWR_WAKEUP_PIN2.
    -   A Rising edge on WKUP pin will wake-up the system from Standby.

-   Alternatively, an external reset (reset button on board) will wake-up system from Standby
as well as wake-up pin.

-   After wake-up from Standby mode, program execution restarts in the same way as
after a reset and **LED1** restarts toggling.
These steps are repeated in an infinite loop.

-   LED1 is used to monitor the system state as follows:
    -   **LED1 fast toggling** (each 200ms): system in Run mode from reset (power on reset or reset pin while system was in Run mode)
    -   **LED1 slow toggling** (each 500ms): system in Run mode after exiting from Standby mode
    -   **LED1 off**: system in Standby mode

#### <b>Notes</b>
 1. This example is not meant to reflect typical product power consumption values but shows how to use the software to switch from power modes.
For power consumption measurement, please check the PWR_ModesSelection example under  available Projects\NUCLEO-U575ZI-Q\Examples\PWR\PWR_ModesSelection folder.

 2. This example can not be used in DEBUG mode due to the fact
      that the Cortex-M33 core is no longer clocked during low power mode
      so debugging features are disabled.


### <b>Keywords</b>

Power, PWR, Standby mode, Interrupt, EXTI, Wakeup, Low Power, External reset,

### <b>Directory contents</b>

-   PWR/PWR_EnterStandbyMode/Inc/stm32u5xx_it.h          Interrupt handlers header file
-   PWR/PWR_EnterStandbyMode/Inc/main.h                  Header for main.c module
-   PWR/PWR_EnterStandbyMode/Inc/stm32_assert.h          Template file to include assert_failed function
-   PWR/PWR_EnterStandbyMode/Src/stm32u5xx_it.c          Interrupt handlers
-   PWR/PWR_EnterStandbyMode/Src/main.c                  Main program
-   PWR/PWR_EnterStandbyMode/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

-   This example runs on STM32U575xx devices.

-   This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q Set-up
    -   LED1 connected to PC.07 pin
    -   User push-button connected to pin PC.13 (External line 13)
    -   WakeUp Pin LL_PWR_WAKEUP_PIN2 connected to PC.13

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example

