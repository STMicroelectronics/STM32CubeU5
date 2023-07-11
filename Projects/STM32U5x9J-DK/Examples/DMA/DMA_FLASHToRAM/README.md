## <b>DMA_FLASHToRAM Example Description</b>

-   How to use a DMA to transfer a word data buffer from Flash memory to embedded SRAM through the HAL API.

-   At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 MHz.

-   DMA channelx is configured to transfer the contents of a 32-word data buffer stored in Flash memory to the reception buffer declared in RAM.

-   The start of transfer is triggered by software. DMA channelx memory-to-memory transfer is enabled. Source and destination addresses incrementing is also enabled.
The transfer is started by setting the channel enable bit for DMA channelx.

-   At the end of the transfer a Transfer Complete interrupt is generated since it is enabled and the callback function (customized by user) is called.

-   STM32U5x9J-DK board's LEDs can be used to monitor the transfer status:

    -   **LED3 toggles** when the transfer is complete (into the Transfer Complete interrupt routine).
    -   **LED4 is ON** when there is a transfer error or when a Error_Handler is called

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

System, DMA, Data Transfer, Memory to memory, Stream, Flash, RAM

### <b>Directory contents</b>

-   DMA/DMA_FLASHToRAM/Inc/stm32u5x9j_discovery.h       BSP configuration file
-   DMA/DMA_FLASHToRAM/Src/system_stm32u5xx.c           stm32u5xx system source file
-   DMA/DMA_FLASHToRAM/Src/stm32u5xx_it.c               Interrupt handlers
-   DMA/DMA_FLASHToRAM/Src/main.c                       Main program
-   DMA/DMA_FLASHToRAM/Inc/stm32u5xx_hal_conf.h         HAL Configuration file
-   DMA/DMA_FLASHToRAM/Inc/stm32u5xx_it.h               Interrupt handlers header file
-   DMA/DMA_FLASHToRAM/Inc/main.h                       Main program header file
-   DMA/DMA_FLASHToRAM/Src/stm32u5xx_hal_msp.c          HAL MSP module

### <b>Hardware and Software environment</b>

-   This example runs on STM32U5A9xx Devices.
-   This example runs on STM32U5G9xx Devices.
-   This example has been tested with STMicroelectronics STM32U5x9J-DK
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example


