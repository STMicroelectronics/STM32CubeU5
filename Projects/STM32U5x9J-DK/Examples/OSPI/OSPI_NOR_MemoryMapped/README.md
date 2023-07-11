## <b>OSPI_NOR_MemoryMapped Example Description</b>

-   How to use a OSPI NOR memory in memory-mapped mode.

-   This example describes how to erase a part of an OSPI NOR memory, write data in memory-mapped mode and access to OSPI NOR memory in memory-mapped mode to check the data in an infinite loop.The memory is configured in octal mode.

-   **LED_GREEN toggles** each time the data have been checked.
-   **LED_RED is ON** as soon as a data is wrong.
-   **LED_RED toggles** as soon as an error is returned by HAL API.

-   The project configures the maximum system clock frequency at 160Mhz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

#### <b>Notes</b>

 1. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

 2. This example calls CACHE_Enable() in order to enable the Instruction
    and Data Cache. This function is provided in order to enhance performance */

### <b>Keywords</b>

Memory, OCTOSPI, Erase, Read, Write, Memory Mapped

### <b>Directory contents</b>

  -   OSPI/OSPI_NOR_MemoryMapped/Src/main.c                       Main program
  -   OSPI/OSPI_NOR_MemoryMapped/Src/system_stm32u5xx.c           STM32U5xx system clock configuration file
  -   OSPI/OSPI_NOR_MemoryMapped/Src/stm32u5xx_it.c               Interrupt handlers
  -   OSPI/OSPI_NOR_MemoryMapped/Src/stm32u5xx_hal_msp.c          HAL MSP module
  -   OSPI/OSPI_NOR_MemoryMapped/Inc/main.h                       Main program header file
  -   OSPI/OSPI_NOR_MemoryMapped/Inc/stm32u5x9j_discovery_conf.h  BSP Configuration file
  -   OSPI/OSPI_NOR_MemoryMapped/Inc/stm32u5xx_hal_conf.h         HAL Configuration file
  -   OSPI/OSPI_NOR_MemoryMapped/Inc/stm32u5xx_it.h               Interrupt handlers header file

### <b>Hardware and Software environment</b>  

  -   This example runs on STM32U5A9xx devices without security enabled (TZEN=0).
  -   In this example, the clock is set to 160 MHz.

  -   This example has been tested with STMicroelectronics STM32U5x9J-DK (MB1829B) board and can be easily tailored to any other supported device and development board.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example

