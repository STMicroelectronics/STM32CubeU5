## <b>LPBAM_LPTIM_PWMGeneration Application Description</b>
-   How to handle LPTIM PWM Generation with DMA linked-list feature in low power mode
through LPBAM utility.

-   Board: NUCLEO-U575ZI-Q (embeds a STM32U575xx device)

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

-   In a first step, the application calls ADV_LPBAM_LPTIM_PWM_SetFullQ() to update period, pulse and repetition values.
After that, a linked-list queue is created and placed in the SRAM that will be executed by a DMA channel in Stop 2
power mode. This queue contains configuration nodes to set-up the LPTIM each time a update event was generated to ensure
data transfer in DMA mode.

-   The counter clock is LSE (32.768 KHz).

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

-   To minimize the power consumption, after starting generating the PWM signal, the MCU enters in Stop 2 mode.

-   User push-button pin (PC.13) is configured as input with external interrupt (EXTI_Line13), falling edge. When User
push-button is pressed, wakeup event is generated and green led is turned on.

-   This project contains two configuration :
    -   Debug configuration : uncomment DEBUG_CONFIGURATION flag in the main.h.
        - LPTIM1 output signal is mapped on PB.2 pin.
    -   Power measurement configuration : comment DEBUG_CONFIGURATION flag in the main.h.
        - LPTIM1 output signal is not mapped.

-   The typical average power consumption of the system performing LPTIM PWM ganeration with changing 5 times signal
characteristics is 4.1uA.

-   NUCLEO-U575ZI-Q board LEDs are used to monitor the transfer status:
    -   LED1 (green led) is turned on when no error is detected.
    -   LED3 (red led) is turned on when any error is occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Timer, Watchdog, Motor control, LPTIM, LPBAM, PWM, Signal, Low power, Stop2

### <b>Directory contents</b>

-   LPBAM/LPBAM_LPIM_PWMGeneration/Src/main.c                  Main program
-   LPBAM/LPBAM_LPIM_PWMGeneration/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
-   LPBAM/LPBAM_LPIM_PWMGeneration/Src/stm32u5xx_it.c          Interrupt handlers
-   LPBAM/LPBAM_LPIM_PWMGeneration/Src/stm32u5xx_hal_msp.c     HAL MSP module
-   LPBAM/LPBAM_LPIM_PWMGeneration/Inc/stm32_lpbam_conf.h      LPBAM Configuration file
-   LPBAM/LPBAM_LPIM_PWMGeneration/Inc/main.h                  Main program header file
-   LPBAM/LPBAM_LPIM_PWMGeneration/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
-   LPBAM/LPBAM_LPIM_PWMGeneration/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
-   LPBAM/LPBAM_LPIM_PWMGeneration/Inc/stm32u5xx_it.h          Interrupt handlers header file
-   LPBAM/LPBAM_LPIM_PWMGeneration/Inc/stm32_lpbam_conf.h      LPBAM Configuration file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   Connect PB.2 (Pin 22 in CN12 connector) to an oscilloscope to monitor the LPTIM1 output waveform.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example

