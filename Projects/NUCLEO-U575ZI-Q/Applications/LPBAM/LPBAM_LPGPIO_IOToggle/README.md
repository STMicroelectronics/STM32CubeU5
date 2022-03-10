## <b>LPBAM_LPGPIO_IOToggle Application Description</b>
-   How to toggle a LPGPIO Pin with DMA linked-list feature in low power mode through LPBAM utility every 1s.

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

-   LPBAM_LPGPIO_IOToggle application contains :
    -   One LPBAM sub-application named **WriteIO**.
-   WriteIO application contains :
    -   One LPBAM scenario named **WriteSeq**.

-   The LPBAM WriteIO application configures the system to reach the lowest power consumption.

-   The LPBAM WriteSeq scenario configures the used peripherals and builds the scenario linked-list.

-   The WriteSeq scenario performs :
    -   Create write LPGPIO pin 0 values queue.
        -   Each write operation is triggered by LPTIM1 PWM rising edge signal.

-   This scenario is repeated in infinite loop.

-   LPTIM1 PWM trigger signal period is equal to 1s.

-   After configuring, building, linking and starting the LPBAM scenario the MCU enters in Stop2 mode.

-   User push-button pin (PC.13) is configured as input with external interrupt (EXTI_Line13), falling edge. When it
is pressed, the system wakes up from Stop2 mode.

-   After the wake up from Stop2 mode, the LPBAM sub-system is still operating and the blue led is turns on.

-   When pressing another time on the user push-button, the LPBAM scenario is stopped, unlinked and de-initialized and the
green led turns on.

-   The typical average power consumption of the system performing LPGPIO pin toggling with a 1s period is 4.2uA.

-   NUCLEO-U575ZI-Q (MB1549) board's LEDs can be used to monitor the LPGPIO Pin status:
    -   LED1 (green led) is turned on at the end of application and no error is detected.
    -   LED2 (blue led) is turned on after wake up from Stop2 mode and before pressing the user push-button.
    -   LED3 (red led) is turned on when any error is occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

 3. This application can not be used in DEBUG mode due to the fact that the debug pins are configured in analog mode for power
      consumption and Cortex-M33 core is no longer clocked during low power mode so debugging features are disabled.

 4. When deleting project folder (EWARM, MDK-ARM or STM32CubeIDE) and regenerating this application with STM32CubeMx tool, 
      ensure to re-adjust linker ROM and RAM sections addresses.

### <b>Keywords</b>

System, LPGPIO, LPDMA, LPBAM, Output, Alternate function, Push-pull, Toggle

### <b>Directory contents</b>

-   LPBAM/LPBAM_LPGPIO_IOToggle/Core/Inc/stm32_lpbam_conf.h                   LPBAM Configuration file
-   LPBAM/LPBAM_LPGPIO_IOToggle/Core/Inc/stm32u5xx_nucleo_conf.h              BSP configuration file
-   LPBAM/LPBAM_LPGPIO_IOToggle/Core/Inc/stm32u5xx_hal_conf.h                 HAL configuration file
-   LPBAM/LPBAM_LPGPIO_IOToggle/Core/Inc/stm32u5xx_it.h                       Interrupt handlers header file
-   LPBAM/LPBAM_LPGPIO_IOToggle/Core/Inc/main.h                               Header for main.c module file
-   LPBAM/LPBAM_LPGPIO_IOToggle/Core/Src/stm32u5xx_it.c                       Interrupt handlers file
-   LPBAM/LPBAM_LPGPIO_IOToggle/Core/Src/stm32u5xx_hal_msp.c                  HAL MSP file
-   LPBAM/LPBAM_LPGPIO_IOToggle/Core/Src/main.c                               Main program file
-   LPBAM/LPBAM_LPGPIO_IOToggle/Core/Src/system_stm32u5xx.c                   STM32U5xx system source file
-   LPBAM/LPBAM_LPGPIO_IOToggle/LPBAM/WriteIO/lpbam_writeio.h                 LPBAM WriteIO application header file
-   LPBAM/LPBAM_LPGPIO_IOToggle/LPBAM/WriteIO/lpbam_writeio_config.c          LPBAM WriteIO application configuration file
-   LPBAM/LPBAM_LPGPIO_IOToggle/LPBAM/WriteIO/lpbam_writeio_writeseq_config.c LPBAM WriteIO application WriteSeq scenario configuration file
-   LPBAM/LPBAM_LPGPIO_IOToggle/LPBAM/WriteIO/lpbam_writeio_writeseq_build.c  LPBAM WriteIO application WriteSeq scenario build file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with NUCLEO-U575ZI-Q (MB1549) board and can be
    easily tailored to any other supported device and development board.

-   NUCLEO-U575ZI-Q set-up
    -   Connect the pin PA.1 (pin 11 in CN10 connector)to an oscilloscope (LPGPIO1 pin 0 output).

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   UnPlug then Plug STLINK connection to perform a power-on-reset.