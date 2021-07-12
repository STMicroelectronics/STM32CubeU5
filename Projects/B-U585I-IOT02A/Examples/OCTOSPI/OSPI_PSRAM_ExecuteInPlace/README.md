## <b>OSPI_PSRAM_ExecuteInPlace Example Description</b>

How to execute code from OSPI memory after code loading.

This example describes how to execute a part of the code from a OSPI PSRAM memory. To do this, 
a section is created where the function is stored.

At the startup, the OSPI PSRAM is configured in memory-mapped mode and the data are copied from the initialization
section of the flash to the OSPI PSRAM memory. And finally the code is executed in an infinite loop.

LED Green toggle in a infinite loop.  
LED_RED is ON as soon as a comparison error occurs.  
LED_RED toggles as soon as an error is returned by HAL API.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U585xx Devices :  
  - The CPU at 160 MHz.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Memory, OCTOSPI, DLYB, Memory Mapped, PSRAM, Execute in Place

### <b>Directory contents</b>

  - OCTOSPI/OSPI_PSRAM_ExecuteInPlace/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - OCTOSPI/OSPI_PSRAM_ExecuteInPlace/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - OCTOSPI/OSPI_PSRAM_ExecuteInPlace/Inc/main.h                  Header for main.c module
  - OCTOSPI/OSPI_PSRAM_ExecuteInPlace/Src/stm32u5xx_it.c          Interrupt handlers
  - OCTOSPI/OSPI_PSRAM_ExecuteInPlace/Src/main.c                  Main program
  - OCTOSPI/OSPI_PSRAM_ExecuteInPlace/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - OCTOSPI/OSPI_PSRAM_ExecuteInPlace/Src/system_stm32u5xx.c      STM32U5xx system source file

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

