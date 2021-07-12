## <b>RTC_ExitStandbyWithWakeUpTimer_Init Example Description</b>

How to periodically enter and wake up from STANDBY mode thanks to the RTC Wakeup Timer (WUT).
The peripheral initialization uses LL unitary service functions for optimization purposes (performance and size).

In this example, after start-up, SYSCLK is configured to the max frequency using
the PLL with MSI as clock source.

#### <b>Notes</b>

  - LSE oscillator clock is used as RTC clock source by default.
  - The user can use also LSI as RTC clock source.
  - The user uncomment the adequate line on the main.c file.
      @code
        #define RTC_CLOCK_SOURCE_LSE
        /* #define RTC_CLOCK_SOURCE_LSI */
      @endcode
  - LSI oscillator clock is delivered by a 32 kHz RC.
  - LSE (when available on board) is delivered by a 32.768 kHz crystal.

Example execution:
  The RTC Wakeup Timer (WUT) is configured to periodically generate an event every 3 seconds
  and the system enters STANDBY mode. After the WUT event is generated, the system wakes up. The system enters again in STANDBY mode after each wakeup.
  After 5 times the system wakes up (5 * 3 seconds = 15 seconds), the program checks that the RTC seconds value is also 15 seconds.
  It means that the test is OK.

One of the below scenario can occur :
   - LED1 (pin PC.07) toggles 2 times : The system woke up from standby.
   - LED1 (pin PC.07) is ON after 15 seconds : Test is over, result is OK.
   - LED1 (pin PC.07) is toggling in infinite loop at a frequency of 5Hz: An error occurred.

#### <b>Notes</b>

  This example can not be used in DEBUG mode, this is due to the fact
  that the core is no longer clocked during low power mode
  so debugging features are disabled.
  To use the DEBUG mode uncomment __DBGMCU_EnableDBGStandbyMode in main() but
  current consumption in STANDBY mode will increase a lot.

### <b>Keywords</b>

System, RTC, RTC Wakeup timer, Standby mode, LSI, LSE, Interrupt,

### <b>Directory contents</b>

  - RTC/RTC_ExitStandbyWithWakeUpTimer_Init/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - RTC/RTC_ExitStandbyWithWakeUpTimer_Init/Inc/main.h                  Header for main.c module
  - RTC/RTC_ExitStandbyWithWakeUpTimer_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - RTC/RTC_ExitStandbyWithWakeUpTimer_Init/Src/stm32u5xx_it.c          Interrupt handlers
  - RTC/RTC_ExitStandbyWithWakeUpTimer_Init/Src/main.c                  Main program
  - RTC/RTC_ExitStandbyWithWakeUpTimer_Init/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

