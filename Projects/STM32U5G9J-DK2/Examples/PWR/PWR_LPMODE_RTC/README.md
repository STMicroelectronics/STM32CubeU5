## <b>PWR_LPMODE_RTC Example Description</b>

How to enter the system to different available low power modes and wake up from
these modes by using an interrupt from RTC wakeup timer.

This project is targeted to run on STM32U5G9ZJTxQ devices on STM32U5G9J-DK2 board from STMicroelectronics.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK)
to run at 160 MHz.

The system automatically enters low power mode 5 sec after start-up.
The RTC wake-up is configured to generate an interrupt on rising edge about 10 sec. afterwards.

After wake-up from low power mode, program performs a system reset when it was on Sleep, Stop 0, Stop 1,
Stop 2, Stop 3 modes and the system restarts automatically when it was on Standby and Shutdown modes.

This projects performs multi-configuration :

 - Uncomment CPU_SLEEP_MODE defines in the main.h file to allow the CPU to enter Sleep mode.
 - Uncomment SYSTEM_STOP0_MODE defines in the main.h file to allow the SYSTEM to enter Stop mode.
 - Uncomment SYSTEM_STOP1_MODE defines in the main.h file to allow the SYSTEM to enter Stop 1 mode.
 - Uncomment SYSTEM_STOP2_MODE defines in the main.h file to allow the SYSTEM to enter Stop 2 mode.
 - Uncomment SYSTEM_STOP3_MODE defines in the main.h file to allow the SYSTEM to enter Stop 3 mode.
 - Uncomment SYSTEM_STANDBY_MODE defines in the main.h file to allow the SYSTEM to enter in Standby mode.
 - Uncomment SYSTEM_SHUTDOWN_MODE defines in the main.h file to allow the SYSTEM to enter in Shutdown mode.

This sequence is repeated in loop.

STM32U5G9J-DK2 board's LED can be used to monitor the project operation status:

 - LED3 toggle when no error detected.
 - LED2 is ON when any project error was occurred.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

RAMCFG, ECC, SRAM, Single error, Double error

### <b>Directory contents</b>

  - PWR/PWR_LPMODE_RTC/Src/main.c                  Main program
  - PWR/PWR_LPMODE_RTC/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - PWR/PWR_LPMODE_RTC/Src/stm32u5xx_it.c          Interrupt handlers
  - PWR/PWR_LPMODE_RTC/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - PWR/PWR_LPMODE_RTC/Inc/main.h                  Main program header file
  - PWR/PWR_LPMODE_RTC/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
  - PWR/PWR_LPMODE_RTC/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - PWR/PWR_LPMODE_RTC/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U5G9ZJTxQ devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics STM32U5G9J-DK2
    board and can be easily tailored to any other supported device
    and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

