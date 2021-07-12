## <b>DLYB_OSPI_NOR_FastTuning Example Description</b>

How to use Delay Block (DLYB) with a Fast Tuning.
DLYB is used with OSPI NOR in MemoryMapped DTR mode.

- This example describes how to tune Delay Block when using OSPI NOR in MemoryMapped DTR mode.
- This fast Tuning is based on the delay line length value which is determined at the beginning of Tuning.
- Then delay block will be set with adequate value regarding delay line length.
- Access to OSPI NOR memory will be in memory-mapped mode in order to check data.
- The memory is configured in octal mode.

LED_GREEN is ON if there is no error in the checked data.
LED_RED is ON as soon as a data is wrong.
LED_RED toggles as soon as an error is returned by HAL API.

The project configures the maximum system clock frequency at 160Mhz.

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

Memory, DLYB, OCTOSPI, Memory Mapped, DTR Mode

### <b>Directory contents</b>

  - DLYB/DLYB_OSPI_NOR_FastTuning/Src/main.c                  Main program
  - DLYB/DLYB_OSPI_NOR_FastTuning/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - DLYB/DLYB_OSPI_NOR_FastTuning/Src/stm32u5xx_it.c          Interrupt handlers
  - DLYB/DLYB_OSPI_NOR_FastTuning/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - DLYB/DLYB_OSPI_NOR_FastTuning/Inc/main.h                  Main program header file
  - DLYB/DLYB_OSPI_NOR_FastTuning/Inc/stm32u575i_eval_conf.h  BSP Configuration file
  - DLYB/DLYB_OSPI_NOR_FastTuning/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - DLYB/DLYB_OSPI_NOR_FastTuning/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices without security enabled (TZEN=0).
  - Make sure that the camera module is not plugged (CN16) on the board (MB1550)
  - In this example, the clock is set to 160 MHz.

  - This example has been tested with STMicroelectronics STM32U575I-EV (MB1550)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

