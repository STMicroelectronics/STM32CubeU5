## <b>LPBAM_LPGPIO_IOToggle Application Description</b>
-   How to toggle a LPGPIO Pin with DMA linked-list feature in low power mode through LPBAM utility every 1s.

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 160 MHz.

-   LPGPIO_PIN_0 is configured as output.
LPBAM GPIO layer is used to configure write pin node to set and reset the selected LPGPIO pin every 1s.
LPDMA1_Channel0 is configured to transfer the contents of user buffer (LPGPIOBufferState) stored in SRAM4 memory to the
LPGPIO1 register (LPGPIO1_ODR register).
When queue is well created, the whole system enters in STOP 2 mode.

-   Each transmitted state is triggered by LPTIM1 rising edge PWM signal.
When DMA node is completed (2 LPGPIO pin state), a transfer complete interrupt is generated to wakeup the
system and toggles the green led.

-   The example exits STOP2 mode when the user press the user push-button.

-   The LPBAM operating scenario is not impacted when the system exits from STOP 2 mode.

-   NUCLEO-U575ZI-Q (MB1549) board's LEDs can be used to monitor the LPGPIO Pin status:
    -   LED1 is ON when no error detected.
    -   LED3 is ON when any error occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

System, LPGPIO, LPDMA, LPBAM, Output, Alternate function, Push-pull, Toggle

### <b>Directory contents</b>

-   LPBAM/LPBAM_LPGPIO_IOToggle/Inc/stm32_lpbam_conf.h      LPBAM Configuration file
-   LPBAM/LPBAM_LPGPIO_IOToggle/Inc/stm32u5xx_nucleo_conf.h BSP configuration file
-   LPBAM/LPBAM_LPGPIO_IOToggle/Inc/stm32u5xx_hal_conf.h    HAL configuration file
-   LPBAM/LPBAM_LPGPIO_IOToggle/Inc/stm32u5xx_it.h          Interrupt handlers header file
-   LPBAM/LPBAM_LPGPIO_IOToggle/Inc/main.h                  Header for main.c module
-   LPBAM/LPBAM_LPGPIO_IOToggle/Src/stm32u5xx_it.c          Interrupt handlers
-   LPBAM/LPBAM_LPGPIO_IOToggle/Src/stm32u5xx_hal_msp.c     HAL MSP file
-   LPBAM/LPBAM_LPGPIO_IOToggle/Src/main.c                  Main program
-   LPBAM/LPBAM_LPGPIO_IOToggle/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with NUCLEO-U575ZI-Q (MB1549) board and can be
    easily tailored to any other supported device and development board.

-   NUCLEO-U575ZI-Q set-up
    -   Connect the pin PB2 to an oscilloscope (LPTIM Channel 1 output).
    -   Connect the pin PA1 to an oscilloscope (LPGPIO1 pin 0 output).

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the application


