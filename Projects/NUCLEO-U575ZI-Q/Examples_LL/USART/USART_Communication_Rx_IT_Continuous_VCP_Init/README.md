## <b>USART_Communication_Rx_IT_Continuous_VCP_Init Example Description</b>

This example shows how to configure GPIO and USART peripheral for continuously receiving characters
from HyperTerminal (PC) in Asynchronous mode using Interrupt mode. Peripheral initialization is
done using LL unitary services functions for optimization purpose (performance and size).

USART Peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
No HW flow control is used.
GPIO associated to User push-button is linked with EXTI.

Example execution:

After startup from reset and system configuration, LED1 is blinking quickly.
On press on push button, USART RX Not Empty interrupt is enabled.
When character is received on USART Rx line, a RXNE interrupt occurs.
Received data is read from USART RDR register and stored in user buffer.
A double buffer is available for received data, allowing continuous reception.
User could process data received in Buffer A, while buffer B is used for reception.
When buffer B is full, it could be handled by user, while buffer A becomes active buffer for next reception, and so on.
Each time a reception buffer is full, user data process callback is called.
Data processing consists in echoing data buffer content on PC Com port, and making LED1 toggle.
In case of errors, LED1 is blinking.

Virtual Com port feature of STLINK could be used for UART communication between board and PC.

### <b>Keywords</b>

Connectivity, UART/USART, Asynchronous, RS-232, baud rate, Interrupt, HyperTerminal, Continuous Reception, VCP,
Receiver

### <b>Directory contents</b>

  - USART/USART_Communication_Rx_IT_Continuous_VCP_Init/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - USART/USART_Communication_Rx_IT_Continuous_VCP_Init/Inc/main.h                  Header for main.c module
  - USART/USART_Communication_Rx_IT_Continuous_VCP_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_Communication_Rx_IT_Continuous_VCP_Init/Src/stm32u5xx_it.c          Interrupt handlers
  - USART/USART_Communication_Rx_IT_Continuous_VCP_Init/Src/main.c                  Main program
  - USART/USART_Communication_Rx_IT_Continuous_VCP_Init/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.  
  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.  
  - NUCLEO-U575ZI-Q Set-up  
    Please ensure that USART communication between the target MCU and ST-LINK MCU is properly enabled
    on HW board in order to support Virtual Com Port (Default HW SB configuration allows use of VCP)  
  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration
    (115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control).  
  - Launch the program. Enter characters on PC communication SW side.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

