## <b>LPBAM_COMP_InputSwitch Application Description</b>
-   How to handle COMP switch inputs and read compared value using
DMA linked-list feature in low power mode through LPBAM utility.

-   The application consists on the switch of comparator inputs in low power mode, followed by a read operation on the
output value and storing it into a buffer.

-   The system enter stop2 mode and the LPTIM start generating a PWM signal with a period of 1,5s to trig the DMA.
-   Each LPTIM PWM rising edge a new comparator configuration is applied :
    -   Config 1: set input plus 1 to comparator
    -   Read value 1: read output 1 value,
    -   Config 2: set input plus 2 to comparator
    -   Read value 2: read output 2 value.
    -   Config 3: set input plus 1 to comparator
    -   Read value 3: read output 3 value,
    -   Config 4: set input plus 2 to comparator
    -   Read value 4: read output 4 value.
-   At the end the DMA will generate an interrupt to wake up the system.
-   Finally, a comparison is done between the obtained and the expected values.

-   This project contains two configuration :
    -   Debug configuration : uncomment DEBUG_CONFIGURATION flag in the main.h.
    -   Power measurement configuration : comment DEBUG_CONFIGURATION flag in the main.h.

-   The typical average power consumption of the system performing 4 comparisons and 4 output reading with a 1.5s period
between each operation is 4.9uA.

-   NUCLEO-U575ZI-Q board LEDs are used to monitor the transfer status:
    -   LED1 (green led) is turned on when no error is detected.
    -   LED3 (red led) is turned on when any error is occurred.

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Analog, COMP, Comparator, LPBAM, Voltage, VrefInt, Low power, Stop2


### <b>Directory contents</b>

-   LPBAM/LPBAM_COMP_InputSwitch/Src/main.c                  Main program
-   LPBAM/LPBAM_COMP_InputSwitch/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
-   LPBAM/LPBAM_COMP_InputSwitch/Src/stm32u5xx_it.c          Interrupt handlers
-   LPBAM/LPBAM_COMP_InputSwitch/Src/stm32u5xx_hal_msp.c     HAL MSP module
-   LPBAM/LPBAM_COMP_InputSwitch/Inc/stm32_lpbam_conf.h      LPBAM Configuration file
-   LPBAM/LPBAM_COMP_InputSwitch/Inc/main.h                  Main program header file
-   LPBAM/LPBAM_COMP_InputSwitch/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
-   LPBAM/LPBAM_COMP_InputSwitch/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
-   LPBAM/LPBAM_COMP_InputSwitch/Inc/stm32u5xx_it.h          Interrupt handlers header file
-   LPBAM/LPBAM_COMP_InputSwitch/Inc/stm32_lpbam_conf.h      LPBAM Configuration file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q set-up
    -   Connect the pin PB2 (pin 13 CN9) to the GND.
    -   Connect the pin PA2 (pin 13 CN10) to the 3.3V.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example

