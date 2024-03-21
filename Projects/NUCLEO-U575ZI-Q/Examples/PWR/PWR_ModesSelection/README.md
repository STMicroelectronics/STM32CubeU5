
---
pagetitle: Readme
lang: en
---
::: {.row}
::: {.col-sm-12 .col-lg-8}

## <b>PWR_ModesSelection Example Description</b>

-   How to configure the system using HAL drivers to measure the current consumption in different low-power modes.

-   Board: NUCLEO-U575ZI-Q (embeds a STM32U575xx device)

-   In a first step, the user shall enter the system configuration to be applied.
Required system items configuration are :
    -   Low power mode
    -   RTC state
    -   System regulator
    -   System clock
    -   RAM retention

-   When a configuration item is not asked to the user, this means that this item has no impact on system scenario 
and power consumption measure.

-   User can start measuring current when the following message **" You can start measuring power consumption "** 
is printed in the hyperterminal.

-   NUCLEO-U575ZI-Q board LED is used to monitor the system configuration status:

    -   **LED3** (red led) is turned on when any error is occurred.

#### <b>Notes</b>

 1. To measure the current consumption remove JP5 jumper
    and connect an amperemeter to JP5 to measure IDD current.

 2. **This example can not be used in DEBUG mode due to the fact
    that the Cortex-M33 core is no longer clocked during low power mode
    so debugging features are disabled.**

 3. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 4. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Power, PWR, Run, Stop1, Stop2, Stop3, Standby, Shutdown, Retention, Regulator, SMPS, LDO, Clock, RTC, External reset

### <b>Directory contents</b>

-   PWR/PWR_ModesSelection/Src/main.c                  Main program
-   PWR/PWR_ModesSelection/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
-   PWR/PWR_ModesSelection/Src/stm32u5xx_it.c          Interrupt handlers
-   PWR/PWR_ModesSelection/Src/stm32u5xx_hal_msp.c     HAL MSP module
-   PWR/PWR_ModesSelection/Src/console.c               Console output input file
-   PWR/PWR_ModesSelection/Src/lowpower_scenarios.c    Low power scenarios file
-   PWR/PWR_ModesSelection/Src/system_config.c         System configuration file
-   PWR/PWR_ModesSelection/Inc/main.h                  Main program header file
-   PWR/PWR_ModesSelection/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
-   PWR/PWR_ModesSelection/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
-   PWR/PWR_ModesSelection/Inc/stm32u5xx_it.h          Interrupt handlers header file
-   PWR/PWR_ModesSelection/Inc/console.h               Console output input header file
-   PWR/PWR_ModesSelection/Inc/lowpower_scenarios.h    Low power scenarios header file
-   PWR/PWR_ModesSelection/Inc/system_config.h         System configuration header file

### <b>Hardware and Software environment</b>

-   This example runs on STM32U575xx devices without security enabled (TZEN=0).

-   This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q (MB1549) set-up:
    -   **USART1** interface connected to ST-LINK

-   **HyperTerminal setup** of COM port associated to STLink Virtual COM Port:
    -   Baud rate = 9600
    -   Data = 8 bit
    -   Parity = none
    -   Stop = 1 bit
    -   No hardware flow control

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Power reset the board
    -   Open hyperterminal
    -   Select the low power mode to run
    -   Start measuring the consumption
