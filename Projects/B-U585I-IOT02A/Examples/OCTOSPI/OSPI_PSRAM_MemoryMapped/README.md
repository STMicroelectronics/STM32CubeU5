## <b>OSPI_PSRAM_MemoryMapped Example Description</b>

How to use an OSPI PSRAM memory in memory-mapped mode.

This example describes how to write and read data (10KB) in memory-mapped mode in an OSPI
PSRAM memory and compare the result.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.  
The SystemClock_Config() function is used to configure the system clock for STM32U585xx Devices :  
   - The CPU at 160 MHz.

The green LED (LED7) is turned ON, when comparison is good.  
LED_RED is ON as soon as a comparison error occurs.  
LED_RED toggles as soon as an error is returned by HAL API.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Memory, OCTOSPI, DLYB, Memory Mapped, PSRAM

### <b>Directory contents</b>

  - OCTOSPI/OSPI_PSRAM_MemoryMapped/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - OCTOSPI/OSPI_PSRAM_MemoryMapped/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - OCTOSPI/OSPI_PSRAM_MemoryMapped/Inc/main.h                  Header for main.c module
  - OCTOSPI/OSPI_PSRAM_MemoryMapped/Src/stm32u5xx_it.c          Interrupt handlers
  - OCTOSPI/OSPI_PSRAM_MemoryMapped/Src/main.c                  Main program
  - OCTOSPI/OSPI_PSRAM_MemoryMapped/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - OCTOSPI/OSPI_PSRAM_MemoryMapped/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U585xx devices without security enabled (TZEN=0).

  - This template has been tested with STMicroelectronics B-U585I-IOT02A (MB1551)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

