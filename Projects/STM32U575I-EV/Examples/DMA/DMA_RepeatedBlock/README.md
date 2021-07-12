## <b>DMA_RepeatedBlock Example Description</b>

-   How to configure and use the DMA HAL API to perform repeated block transactions.

-   This project is targeted to run on STM32U575AIIxQ devices on STM32U575I-EV board from STMicroelectronics.

-   The project configures the maximum system clock frequency at 160 MHz.

-   The start of DMA transfer is triggered by software.
    -   Incremented gather DMA transaction : the DMA loads from the source a scattered data buffer, then
    gather data before sending them to the destination in incrementing mode.
    -   Incremented scatter DMA transaction : the DMA loads from the source a gathered data buffer, then
    scatter data before sending them to the destination in incrementing mode.
    -   Decremented gather DMA transaction : the DMA loads from the source a dispersed data buffer, then
    collect data before sending them to the destination in decrementing mode.
    -   Decremented scatter DMA transaction : the DMA loads from the source a centralized data buffer, then
    scatter data before sending them to the destination in decrementing mode.

-   STM32U575I-EV board's LED can be used to monitor the transfers status status:
    -   **LED5 toggles** when no error detected.
    -   **LED6 is ON** when any error occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Memory, DMA, Repeated Block, 2D addressing,

### <b>Directory contents</b>

-   DMA/DMA_RepeatedBlock/Src/main.c                    Main program
-   DMA/DMA_RepeatedBlock/Src/system_stm32u5xx.c        STM32U5xx system clock configuration file
-   DMA/DMA_RepeatedBlock/Src/stm32u5xx_it.c            Interrupt handlers
-   DMA/DMA_RepeatedBlock/Src/stm32u5xx_hal_msp.c       HAL MSP module
-   DMA/DMA_RepeatedBlock/Inc/stm32u575i_eval_conf.h    BSP configuration file
-   DMA/DMA_RepeatedBlock/Inc/main.h                    Main program header file
-   DMA/DMA_RepeatedBlock/Inc/stm32u5xx_hal_conf.h      HAL Configuration file
-   DMA/DMA_RepeatedBlock/Inc/stm32u5xx_it.h            Interrupt handlers header file

### <b>Hardware and Software environment</b>

-   This example runs on STM32U575AIIxQ devices.

-   This example has been tested with STMicroelectronics STM32U575I-EV
    board and can be easily tailored to any other supported device
    and development board.


### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example

