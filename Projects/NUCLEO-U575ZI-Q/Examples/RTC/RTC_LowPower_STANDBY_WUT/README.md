## <b>RTC_LowPower_STANDBY_WUT Example Description</b>

How to periodically enter and wake up from STANDBY mode thanks to the RTC Wake-Up Timer (WUT).

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 MHz.
The LSE clock is used as RTC clock source by default.
The wake-up Pin 7 is connected internally to the RTC Wake-Up Timer (WUT) event.

The RTC Wake-Up Timer (WUT) is configured to generate an event every 3 seconds and the system enters in STANDBY mode.
The program enters again in STANDBY mode after each wake-up.
After 5 wake-up (5 * 3 seconds = 15 seconds), the program checks that the RTC seconds value is also 15 seconds.
It means that the test is OK.

One of the below scenario can occur :

 - **LED3 toggles for ever** : an error occurs.
 - **LED1 toggles 2 times** : wake-up from standby.
 - **LED1 is ON after 15 seconds** : Test is over, result is OK.

#### <b>Notes</b>

 1. This example can not be used in DEBUG mode, this is due to the fact
    that the Cortex-M33 core is no longer clocked during low power mode
    so debugging features are disabled.
    To use the DEBUG mode uncomment HAL_DBGMCU_EnableDBG in main() but
    current consumption in STANDBY mode will be increased a lot.

 2. Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
    the RTC clock source; in this case the Backup domain will be reset in
    order to modify the RTC Clock source, as consequence RTC registers (including
    the backup registers) and RCC_CSR register are set to their reset values.

 3. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 4. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

RTC, Wake-Up Timer, Standby Mode,LSE,Backup Register

### <b>Directory contents</b>

  - RTC/RTC_LowPower_STANDBY_WUT/Inc/stm32u5xx_hal_conf.h     HAL configuration file
  - RTC/RTC_LowPower_STANDBY_WUT/Inc/stm32u5xx_it.h           Header for stm32u5xx_it.c
  - RTC/RTC_LowPower_STANDBY_WUT/Inc/main.h                   Header file for main.c
  - RTC/RTC_LowPower_STANDBY_WUT/Src/system_stm32u5xx.c       stm32U5xx system clock configuration file
  - RTC/RTC_LowPower_STANDBY_WUT/Src/stm32u5xx_it.c           Interrupt handlers
  - RTC/RTC_LowPower_STANDBY_WUT/Src/main.c                   Main program
  - RTC/RTC_LowPower_STANDBY_WUT/Src/stm32u5xx_hal_msp.c      HAL MSP module

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575ZITxQ devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q
    board and can be easily tailored to any other supported device
    and development board.

 ### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

