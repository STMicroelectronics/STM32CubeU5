## <b>DMA_LinkedList Example Description</b>

-   How to use the DMA to perform a list of transfers. The transfer list is organized as linked-list,
each time the current transfer ends the DMA automatically reload the next transfer parameters,
and starts it (without CPU intervention).

-   This project is targeted to run on STM32U575xx device on NUCLEO-U575ZI-Q board from STMicroelectronics.

-   The Linked List is configured using the "DMA_LinkedListConfig" function.
This function configures the DMA for a linked-list transfer which contains 3 nodes.
All nodes are filled using LL function "LL_DMA_CreateLinkNode".
Nodes are connected to the linked-list using the LL function "LL_DMA_ConnectLinkNode".
After setting all nodes parameters using **DMA_LinkedListConfig()** / **LL_DMA_ConnectLinkNode()**,
enable DMA interrupts via **LL_DMA_EnableIT_USE()**, **LL_DMA_EnableIT_ULE()**, **LL_DMA_EnableIT_DTE()**,
and **LL_DMA_EnableIT_TC()** functions and start the transfer using LL_DMA_EnableChannel() function.
Note that using the DMA, the transfer data length is always expressed in bytes whatever
the source and destination data size (byte, half word or word).

-   At the end, the main program waits for the end of DMA transmission or error interrupts: the DMA transmission ends when all transfer nodes are served.

-   NUCLEO-U575ZI-Q board's LED can be used to monitor the transfer status:
    -   **LED1 toggle** when no error detected.
    -   **LED3 is ON** when any error occurred.

-   In this example, HCLK is configured at 160 MHz.

### <b>Keywords</b>

DMA, Data Transfer, Memory to Memory, Channel, SRAM

### <b>Directory contents</b>

-   DMA/DMA_LinkedList/Inc/stm32u5xx_it.h     Interrupt handlers header file
-   DMA/DMA_LinkedList/Inc/main.h             Header for main.c module
-   DMA/DMA_LinkedList/Inc/stm32_assert.h     Example file to include assert_failed function
-   DMA/DMA_LinkedList/Src/stm32u5xx_it.c     Interrupt handlers
-   DMA/DMA_LinkedList/Src/main.c             Main program
-   DMA/DMA_LinkedList/Src/system_stm32u5xx.c STM32U5x system source file

### <b>Hardware and Software environment</b>

-   This example runs on STM32U575xx devices.

-   This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q
    boards and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example

