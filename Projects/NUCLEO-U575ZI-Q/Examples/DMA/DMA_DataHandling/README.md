## <b>DMA_DataHandling Example Description</b>

-   How to use the DMA Controller to do data handling between transferred data from the source and transfer to the destination through the HAL API.

-   This project is targeted to run on STM32U575ZITxQ devices on NUCLEO-U575ZI-Q board from STMicroelectronics.

-   At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 MHz.

-   DMA channelx is configured to transfer the contents of a 8-word data buffer placed on SRAM memory to the reception buffer placed on SRAM.

-   The start of transfers are triggered by software.

-   Right aligned / Zero padding data handling : DMA loads data from source memory by byte, then will align
   data to the right, pad zeros and write it to the destination memory by half word.
-   Right aligned / Sign extended data handling : DMA loads data from source memory by byte, then will align
   data to the right, extend the sign bit and write it to the destination memory by half word.
-   Right aligned / Left truncated data handling : DMA loads data from source memory by half word, then will
   align data to the right, truncate data left and write it to the destination memory by byte.
-   Left aligned / Right truncated data handling : DMA loads data from source memory by half word, then will
   align data to the left, truncate data right and write it to the destination memory by byte.
-   Source byte exchange data handling : DMA loads data from source memory by word, then will exchange
   two bytes in the middle of the word and write it to the destination memory by word.
-   Destination byte exchange data handling : DMA loads data from source memory by word, then will exchange
   the two bytes of the first half word, exchange the two bytes of the second half word and write it to the
   destination memory by word.
-   Destination byte exchange data handling : DMA loads data from source memory by word, then will exchange
   the two half words and write it to the destination memory by word.

-   NUCLEO-U575ZI-Q board's LED can be used to monitor the project operation status:
    -   **LED1 toggle** when no error detected.
    -   **LED3 is ON** when any error occurred.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

DMA, Data Transfer, Memory to Memory, Channel, SRAM

### <b>Directory contents</b>

-   DMA/DMA_DataHandling/Src/main.c                  Main program
-   DMA/DMA_DataHandling/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
-   DMA/DMA_DataHandling/Src/stm32u5xx_it.c          Interrupt handlers
-   DMA/DMA_DataHandling/Src/stm32u5xx_hal_msp.c     HAL MSP module
-   DMA/DMA_DataHandling/Inc/main.h                  Main program header file
-   DMA/DMA_DataHandling/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
-   DMA/DMA_DataHandling/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
-   DMA/DMA_DataHandling/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

-   This example runs on STM32U575ZITxQ devices.

-   This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :

    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example

