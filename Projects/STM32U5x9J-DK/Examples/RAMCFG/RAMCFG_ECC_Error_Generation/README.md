## <b>RAMCFG_ECC_Error_Generation Example Description</b>

-   How to configure and use the RAMCFG HAL API to manage ECC errors via RAMCFG peripheral.

-   This project is targeted to run on STM32U5x9J_DK board from STMicroelectronics.

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 MHz.

-   When the RAMCFG controller is disabled, the ECC calculation when writing memory is not performed. So, when enabling
the RAMCFG after writing to SRAM, the ECC and data don't match and the RAMCFG controller will generate interrupts
according to ECC error type (Single or double).
By default, interrupts are disabled. After enabling them (single error and double error interrupts), the
RAMCFG generates interrupts on each read access to a corrupted data.
The RAMCFG controller is able to detect and correct ECC single error data and only detect ECC double error data.

-   STM32U5x9J-DK board's LED can be used to monitor the project operation status:
    - **LED3 toggle** when no error detected.
    - **LED4 is ON** when any project error was occurred.

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

-   RAMCFG/RAMCFG_ECC_Error_Generation/Src/main.c                  Main program
-   RAMCFG/RAMCFG_ECC_Error_Generation/Src/system_stm32u5xx.c      stm32u5xx system clock configuration file
-   RAMCFG/RAMCFG_ECC_Error_Generation/Src/stm32u5xx_it.c          Interrupt handlers
-   RAMCFG/RAMCFG_ECC_Error_Generation/Src/stm32u5xx_hal_msp.c     HAL MSP module
-   RAMCFG/RAMCFG_ECC_Error_Generation/Inc/main.h                  Main program header file
-   RAMCFG/RAMCFG_ECC_Error_Generation/Inc/stm32u5x9j_discovery.h  BSP Configuration file
-   RAMCFG/RAMCFG_ECC_Error_Generation/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
-   RAMCFG/RAMCFG_ECC_Error_Generation/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

-   This example runs on STM32U5A9xx devices.
-   This example runs on STM32U5G9xx devices.
-   This example has been tested with STMicroelectronics STM32U5x9J-DK (MB1829B)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example

