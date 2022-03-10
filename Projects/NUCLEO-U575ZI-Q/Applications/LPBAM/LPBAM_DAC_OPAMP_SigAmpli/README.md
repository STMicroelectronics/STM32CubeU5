## <b>LPBAM_DAC_OPAMP_SigAmpli Application Description</b>
-   How to handle DAC continuous conversion and OPAMP switching configuration using DMA linked-list feature in low power
mode through LPBAM utility.

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

-   The application permits to generate a triangle signal by the DAC peripheral, then amplified by the OPAMP with several
gains.

-   LPBAM_DAC_OPAMP_SigAmpli application contains :
    -   One LPBAM sub-application named **SigAmpli**.
-   SigAmpli application contains :
    -   One LPBAM scenario named **MultiGain**.

-   The LPBAM SigAmpli application configures the system to reach the lowest power consumption.

-   The LPBAM MultiGain scenario configures the used peripherals and builds the scenario linked-list.

-   The DAC signal output is connected internally to the OPAMP input in order to amplify the DAC signal with programmed
gains. The DAC signal generation is triggered by the LPTIM PWM signal.
After amplifying the signal period, The OPAMP gain is modified and the next DAC conversion is started. This sequence is
repeated in an infinite loop.

-   LPTIM1 PWM trigger signal period is equal to 1ms.

-   After configuring, building, linking and starting the LPBAM scenario the MCU enters in Stop2 mode.

-   User push-button pin (PC.13) is configured as input with external interrupt (EXTI_Line13), falling edge. When it
is pressed, the system wakes up from Stop2 mode.

-   After the wake up from Stop2 mode, the LPBAM sub-system is still operating and the blue led is turns on.

-   When pressing another time on the user push-button, the LPBAM scenario is stopped, unlinked and de-initialized and the
green led turns on.

-   The typical average power consumption of the system performing DAC conversion with 1ms period and signal
amplification is 50uA.

-   NUCLEO-U575ZI-Q board LEDs are used to monitor the transfer status:
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

System, Analog, DMA, DAC, OPAMP, LPTIM, LPBAM, Voltage, Signal, Triangular-wave generation, Trigger, Low power, Stop2

### <b>Directory contents</b>

-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/Core/Src/main.c                                  Main program file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/Core/Src/system_stm32u5xx.c                      STM32U5xx system clock configuration file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/Core/Src/stm32u5xx_it.c                          Interrupt handlers file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/Core/Src/stm32u5xx_hal_msp.c                     HAL MSP module file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/Core/Src/triangular_signal.c                     Data triangle signal source file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/Core/Inc/stm32_lpbam_conf.h                      LPBAM Configuration file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/Core/Inc/main.h                                  Main program header file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/Core/Inc/stm32u5xx_nucleo_conf.h                 BSP Configuration file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/Core/Inc/stm32u5xx_hal_conf.h                    HAL Configuration file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/Core/Inc/stm32u5xx_it.h                          Interrupt handlers header file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/Core/Inc/stm32_lpbam_conf.h                      LPBAM Configuration file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/Core/Inc/triangular_signal.h                     Data triangle signal header file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/LPBAM/SigAmpli/lpbam_sigampli.h                  LPBAM SigAmpli application header file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/LPBAM/SigAmpli/lpbam_sigampli_config.c           LPBAM SigAmpli application configuration file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/LPBAM/SigAmpli/lpbam_sigampli_multigain_config.c LPBAM SigAmpli application MultiGain scenario configuration file
-   LPBAM/LPBAM_DAC_OPAMP_SigAmpli/LPBAM/SigAmpli/lpbam_sigampli_multigain_build.c  LPBAM SigAmpli application MultiGain scenario build file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q set-up
    -   Connect the pin PA.3 (pin 1 in CN9 connector) to an oscilloscope (OPAMP output).

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   UnPlug then Plug STLINK connection to perform a power-on-reset.