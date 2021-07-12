## <b>OSPI_NOR_MemoryMapped Example Description</b>

How to use a OSPI NOR memory in memory-mapped mode.

This example describes how to erase a part of an OSPI NOR memory, write data in memory-mapped mode and
access to OSPI NOR memory in memory-mapped mode to check the data in an infinite loop.

LED_GREEN toggles each time the data have been checked.  
LED_RED is on as soon as a data is wrong.  
LED_RED toggles as soon as an error is returned by HAL API.

In this example, HCLK is configured at 160 MHz.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Memory, OCTOSPI, NOR, Memory Mapped

### <b>Directory contents</b> 

  - OCTOSPI/OSPI_NOR_MemoryMapped/Src/main.c                 Main program
  - OCTOSPI/OSPI_NOR_MemoryMapped/Src/system_stm32u5xx.c     STM32U5xx system clock configuration file
  - OCTOSPI/OSPI_NOR_MemoryMapped/Src/stm32u5xx_it.c         Interrupt handlers 
  - OCTOSPI/OSPI_NOR_MemoryMapped/Src/stm32u5xx_hal_msp.c    HAL MSP module
  - OCTOSPI/OSPI_NOR_MemoryMapped/Inc/main.h                 Main program header file  
  - OCTOSPI/OSPI_NOR_MemoryMapped/Inc/stm32u5xx_hal_conf.h   HAL Configuration file
  - OCTOSPI/OSPI_NOR_MemoryMapped/Inc/stm32u5xx_it.h         Interrupt handlers header file
  - OCTOSPI/OSPI_NOR_MemoryMapped/Inc/stm32u575i_eval_conf.h      HAL configuration file  

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575AIIx devices.
    
  - This example has been tested with STM32U575I-EV board and can be 
    easily tailored to any other supported device and development board.

  - Make sure that the camera module is not plugged (CN16) on the board (MB1550)

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

