## <b>LPBAM_LPTIM_PWMGen Application Description</b>
-   How to handle LPTIM PWM Generation with DMA linked-list feature in low power mode through LPBAM utility.

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

-   LPBAM_LPTIM_PWMGen application contains :
    -   One LPBAM sub-application named **PWMGen**.
-   PWMGen application contains :
    -   One LPBAM scenario named **MultiValue**.

-   The LPBAM PWMGen application configures the system to reach the lowest power consumption.

-   The LPBAM MultiValue scenario configures the used peripherals and builds the scenario linked-list.

-   The MultiValue scenario performs 5 different sequences:
    -   Sequence 1
        -   The period value is equal to 10 ms.
             -> Autoreload = (Counter Clock Frequency * period) - 1
                           = 32768 * (10^(-2)) - 1
                           = 327.

        -   The pulse is equal to 5 ms.
            -> CounterCompare = (Counter Clock Frequency * pulse) - 1
                              = 32768 * 5 * (10^(-3)) - 1
                              = 163.

        -   The pulse and period are repeated one time.
            -> Repetition = 1 - 1
                          = 0.

    -   Sequence 2
        -   The period value is equal to 20 ms.
            -> Autoreload = (Counter Clock Frequency * period) - 1
                          = 32768 * 20 * (10^(-3)) - 1
                          = 654.

        -   The pulse is equal to 10 ms.
            -> CounterCompare = (Counter Clock Frequency * pulse) - 1
                              = 32768 * 10 * (10^(-3)) - 1
                              = 327.

        -   The pulse and period are repeated 2 times.
            -> Repetition = 2 - 1
                          = 1.

    -   Sequence 3
        -   The period value is equal to 30 ms.
            -> Autoreload = (Counter Clock Frequency * period) - 1
                          = 32768 * 30 * (10^(-3)) - 1
                          = 982.

        -   The pulse is equal to 15 ms.
            -> CounterCompare = (Counter Clock Frequency * pulse) - 1
                              = 32768 * 15 * (10^(-3)) - 1
                              = 491.

        -   The pulse and period are repeated 3 times.
            -> Repetition = 3 - 1
                          = 2.

    -   Sequence 4
        -   The period value is equal to 40 ms.
            -> Autoreload = (Counter Clock Frequency * period) - 1
                          = 32768 * 40 * (10^(-3)) - 1
                          = 1310.

        -   The pulse is equal to 20 ms.
            -> CounterCompare = (Counter Clock Frequency * pulse) - 1
                              = 32768 * 20 * (10^(-3)) - 1
                              = 654.

        -   The pulse and period are repeated 4 times.
            -> Repetition = 4 - 1
                          = 3.

    -   Sequence 5
        -   The period value is equal to 50 ms.
            -> Autoreload = (Counter Clock Frequency * period) - 1
                          = 32768 * 50 * (10^(-3)) - 1
                          = 1637.

        -   The pulse is equal to 25 ms.
            -> CounterCompare = (Counter Clock Frequency * pulse) - 1
                              = 32768 * 25 * (10^(-3)) - 1
                              = 818.

        -   The pulse and period are repeated 5 times.
            -> Repetition = 5 - 1
                          = 4.

-   This scenario is repeated in infinite loop.

-   The counter clock is LSE (32.768 KHz).

-   After configuring, building, linking and starting the LPBAM scenario the MCU enters in Stop2 mode.

-   User push-button pin (PC.13) is configured as input with external interrupt (EXTI_Line13), falling edge. When it
is pressed, the system wakes up from Stop2 mode.

-   After the wake up from Stop2 mode, the LPBAM sub-system is still operating and the blue led is turns on.

-   When pressing another time on the user push-button, the LPBAM scenario is stopped, unlinked and de-initialized and the
green led turns on.

-   The typical average power consumption of the system performing LPTIM PWM ganeration with changing 5 times signal
characteristics is 4.4uA.

-   NUCLEO-U575ZI-Q (MB1549) board LEDs are used to monitor the transfer status:
    -   LED1 (green led) is turned on at the end of application and no error is detected.
    -   LED2 (blue led) is turned on after wake up from Stop2 mode and before pressing the user push-button.
    -   LED3 (red led) is turned on when any error is occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

 3. This application can not be used in DEBUG mode due to the fact that the debug pins are configured in analog mode for power
      consumption and Cortex-M33 core is no longer clocked during low power mode so debugging features are disabled.

 4. When deleting project folder (EWARM, MDK-ARM or STM32CubeIDE) and regenerating this application with STM32CubeMx tool, 
      ensure to re-adjust linker ROM and RAM sections addresses.

### <b>Keywords</b>

Timer, Watchdog, Motor control, LPTIM, LPBAM, PWM, Signal, Low power, Stop2

### <b>Directory contents</b>

-   LPBAM/LPBAM_LPTIM_PWMGen/Core/Src/main.c                               Main program file
-   LPBAM/LPBAM_LPTIM_PWMGen/Core/Src/system_stm32u5xx.c                   STM32U5xx system clock configuration file
-   LPBAM/LPBAM_LPTIM_PWMGen/Core/Src/stm32u5xx_it.c                       Interrupt handlers file
-   LPBAM/LPBAM_LPTIM_PWMGen/Core/Src/stm32u5xx_hal_msp.c                  HAL MSP module file
-   LPBAM/LPBAM_LPTIM_PWMGen/Core/Inc/stm32_lpbam_conf.h                   LPBAM Configuration file
-   LPBAM/LPBAM_LPTIM_PWMGen/Core/Inc/main.h                               Main program header file
-   LPBAM/LPBAM_LPTIM_PWMGen/Core/Inc/stm32u5xx_nucleo_conf.h              BSP Configuration file
-   LPBAM/LPBAM_LPTIM_PWMGen/Core/Inc/stm32u5xx_hal_conf.h                 HAL Configuration file
-   LPBAM/LPBAM_LPTIM_PWMGen/Core/Inc/stm32u5xx_it.h                       Interrupt handlers header file
-   LPBAM/LPBAM_LPTIM_PWMGen/Core/Inc/stm32_lpbam_conf.h                   LPBAM Configuration file
-   LPBAM/LPBAM_LPTIM_PWMGen/LPBAM/PWMGen/lpbam_pwmgen.h                   LPBAM PWMGen application header file
-   LPBAM/LPBAM_LPTIM_PWMGen/LPBAM/PWMGen/lpbam_pwmgen_config.c            LPBAM PWMGen application configuration file
-   LPBAM/LPBAM_LPTIM_PWMGen/LPBAM/PWMGen/lpbam_pwmgen_multivalue_config.c LPBAM PWMGen application MultiValue scenario configuration file
-   LPBAM/LPBAM_LPTIM_PWMGen/LPBAM/PWMGen/lpbam_pwmgen_multivalue_build.c  LPBAM PWMGen application MultiValue scenario build file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549) board and can be easily tailored
to any other supported device and development board.

-   Connect the pin PC.1 (Pin 36 in CN11 connector) to an oscilloscope to monitor the LPTIM1 output waveform.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   UnPlug then Plug STLINK connection to perform a power-on-reset.