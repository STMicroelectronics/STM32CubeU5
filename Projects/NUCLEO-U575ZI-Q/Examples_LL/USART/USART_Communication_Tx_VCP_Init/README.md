## <b>USART_Communication_Tx_VCP_Init Example Description</b>

This example shows how to configure GPIO and USART peripherals to send characters
asynchronously to an HyperTerminal (PC) in Polling mode. If the transfer could not
be completed within the allocated time, a timeout allows to exit from the sequence
with a Timeout error code. This example is based on STM32U5xx USART LL API. Peripheral
initialization is done using LL unitary services functions for optimization purpose
(performance and size).

USART Peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
No HW flow control is used.
GPIO associated to User push-button is linked with EXTI.
Virtual Com port feature of STLINK could be used for UART communication between board and PC.

Example execution:

On press on push button , first character of buffer to be transmitted is written into USART Transmit Data Register (TDR)
Program then starts polling on USART TXE flag before sending next character.
On last character, program polls on TC flag to ensure transmit of last character is completed.
At completion of Tx Buffer sending, LED1 is turned On.
In case of errors, LED1 is blinking (1sec period).

Program is written so that, any new press on User push-button will lead to new transmission of complete buffer.

### <b>Keywords</b>

Connectivity, UART/USART, Asynchronous, RS-232, baud rate, Polling, HyperTerminal, Transmitter, VCP

### <b>Directory contents</b>

  - USART/USART_Communication_Tx_VCP_Init/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - USART/USART_Communication_Tx_VCP_Init/Inc/main.h                  Header for main.c module
  - USART/USART_Communication_Tx_VCP_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_Communication_Tx_VCP_Init/Src/stm32u5xx_it.c          Interrupt handlers
  - USART/USART_Communication_Tx_VCP_Init/Src/main.c                  Main program
  - USART/USART_Communication_Tx_VCP_Init/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.  
  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.  
  - NUCLEO-U575ZI-Q Set-up  
    Please ensure that USART communication between the target MCU and ST-LINK MCU is properly enabled
    on HW board in order to support Virtual Com Port (Default HW SB configuration allows use of VCP)  
  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration
    (115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control).  
  - Launch the program. Press on User push button on board to initiate data transfer.

### <b>How to use it ?</b>  

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

