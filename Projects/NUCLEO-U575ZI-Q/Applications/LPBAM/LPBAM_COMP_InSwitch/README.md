## <b>LPBAM_COMP_InSwitch Application Description</b>
-   How to handle COMP switch inputs and read compared value using DMA linked-list feature in low power mode through
LPBAM utility.

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

-   The application consists on the switch of comparator inputs in low power mode, followed by a read operation on the
output value and storing it into a buffer.

-   LPBAM_COMP_InSwitch application contains :
    -   One LPBAM sub-application named **InComp**.
-   InComp application contains :
    -   One LPBAM scenario named **InSwitch**.

-   The LPBAM InComp application configures the system to reach the lowest power consumption.

-   The LPBAM InSwitch scenario configures the used peripherals and builds the scenario linked-list.

-   The system enter Stop2 mode and the LPTIM start generating a PWM signal with a period of 1,5s to trig the DMA.

-   Each LPTIM PWM rising edge a new comparator configuration is applied :
    -   Config 1     : set input plus 1 to comparator
    -   Read value 1 : read output 1 value,
    -   Config 2     : set input plus 2 to comparator
    -   Read value 2 : read output 2 value.
    -   Config 3     : set input plus 1 to comparator
    -   Read value 3 : read output 3 value,
    -   Config 4     : set input plus 2 to comparator
    -   Read value 4 : read output 4 value.

-   At the end, the DMA will generate an interrupt to wake up the system.

-   Finally, a comparison is done between the obtained and the expected values.

-   The typical average power consumption of the system performing 4 comparisons and 4 output reading with a 1.5s period
between each operation is 5.2uA.

-   NUCLEO-U575ZI-Q board LEDs are used to monitor the transfer status:
    -   LED1 (green led) is turned on at the end of application and no error is detected.
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

Analog, COMP, Comparator, LPBAM, Voltage, VrefInt, Low power, Stop2

### <b>Directory contents</b>

-   LPBAM/LPBAM_COMP_InSwitch/Core/Src/main.c                             Main program file
-   LPBAM/LPBAM_COMP_InSwitch/Core/Src/system_stm32u5xx.c                 STM32U5xx system clock configuration file
-   LPBAM/LPBAM_COMP_InSwitch/Core/Src/stm32u5xx_it.c                     Interrupt handlers file
-   LPBAM/LPBAM_COMP_InSwitch/Core/Src/stm32u5xx_hal_msp.c                HAL MSP module file
-   LPBAM/LPBAM_COMP_InSwitch/Core/Inc/stm32_lpbam_conf.h                 LPBAM Configuration file
-   LPBAM/LPBAM_COMP_InSwitch/Core/Inc/main.h                             Main program header file
-   LPBAM/LPBAM_COMP_InSwitch/Core/Inc/stm32u5xx_nucleo_conf.h            BSP Configuration file
-   LPBAM/LPBAM_COMP_InSwitch/Core/Inc/stm32u5xx_hal_conf.h               HAL Configuration file
-   LPBAM/LPBAM_COMP_InSwitch/Core/Inc/stm32u5xx_it.h                     Interrupt handlers header file
-   LPBAM/LPBAM_COMP_InSwitch/Core/Inc/stm32_lpbam_conf.h                 LPBAM Configuration file
-   LPBAM/LPBAM_COMP_InSwitch/LPBAM/InComp/lpbam_incomp.h                 LPBAM InComp application header file
-   LPBAM/LPBAM_COMP_InSwitch/LPBAM/InComp/lpbam_incomp_config.c          LPBAM InComp application configuration file
-   LPBAM/LPBAM_COMP_InSwitch/LPBAM/InComp/lpbam_incomp_inswitch_config.c LPBAM InComp application InSwitch scenario configuration file
-   LPBAM/LPBAM_COMP_InSwitch/LPBAM/InComp/lpbam_incomp_inswitch_build.c  LPBAM InComp application InSwitch scenario build file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q set-up
    -   Connect the pin PB.2 (pin 13 in CN9 connector) to the GND.
    -   Connect the pin PA.2 (pin 13 in CN10 connector) to the 3.3V.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   UnPlug then Plug STLINK connection to perform a power-on-reset.