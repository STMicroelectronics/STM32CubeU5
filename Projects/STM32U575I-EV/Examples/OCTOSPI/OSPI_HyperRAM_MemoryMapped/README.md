## <b>OSPI_HyperRAM_MemoryMapped Example Description</b>


- How to use a OSPI HyperRAM memory in memory-mapped mode.

- This example describes how to write and read data in memory-mapped mode in a OSPI HyperRAM memory and compare the result in an infinite loop.

- LED_GREEN toggles each time the data have been checked.  
- LED_RED is ON as soon as a data is wrong.  
- LED_RED toggles as soon as an error is returned by HAL API.

- The project configures the maximum system clock frequency at 160Mhz.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

 3. This example calls CACHE_Enable() in order to enable the Instruction
    and Data Cache. This function is provided  in order to enhance performance */

### <b>Keywords</b>

Memory, OCTOSPI, Memory Mapped, HyperRAM

### <b>Directory contents</b>

  - OCTOSPI/OSPI_HyperRAM_MemoryMapped/Src/main.c                  Main program
  - OCTOSPI/OSPI_HyperRAM_MemoryMapped/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - OCTOSPI/OSPI_HyperRAM_MemoryMapped/Src/stm32u5xx_it.c          Interrupt handlers
  - OCTOSPI/OSPI_HyperRAM_MemoryMapped/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - OCTOSPI/OSPI_HyperRAM_MemoryMapped/Inc/main.h                  Main program header file
  - OCTOSPI/OSPI_HyperRAM_MemoryMapped/Inc/stm32u575i_eval_conf.h  BSP Configuration file
  - OCTOSPI/OSPI_HyperRAM_MemoryMapped/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - OCTOSPI/OSPI_HyperRAM_MemoryMapped/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices without security enabled (TZEN=0).
  - Make sure that the MB1242 HyperRAM module (ISR3VI64) is plugged (CN11) on the board (MB1550)
  - The MB1242 hardware configuration is: SW1 (CS1: OFF, CS2: OFF) and SW2 (CS1: OFF, CS2: ON)
  - In this example, the clock is set to 160 MHz.

  - This example has been tested with STMicroelectronics STM32U575I-EV (MB1550)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

