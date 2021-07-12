## <b>USART_Communication_Rx_IT_Init Example Description</b>

This example shows how to configure GPIO and USART peripheral for receiving characters
from HyperTerminal (PC) in Asynchronous mode using Interrupt mode. Peripheral initialization is done
using LL initialization function to demonstrate LL init usage.

USART Peripheral is configured in asynchronous mode (115200 bauds, 8 data bit, 1 start bit, 1 stop bit, no parity).
No HW flow control is used.
GPIO associated to User push-button is linked with EXTI.
USART RX Not Empty interrupt is enabled.  
Example execution:

- When character is received on USART Rx line, a RXNE interrupt occurs.
- USART IRQ Handler routine is then checking received character value.
- On a specific value ('S' or 's'), LED1 is turned On.
- Received character is echoed on Tx line.
- On press on push button, LED1 is turned Off.
- In case of errors, LED1 is blinking.

### <b>Keywords</b>

Connectivity, UART/USART, Asynchronous, RS-232, baud rate, Interrupt, HyperTerminal, Receiver

### <b>Directory contents</b>

  - USART/USART_Communication_Rx_IT_Init/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - USART/USART_Communication_Rx_IT_Init/Inc/main.h                  Header for main.c module
  - USART/USART_Communication_Rx_IT_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - USART/USART_Communication_Rx_IT_Init/Src/stm32u5xx_it.c          Interrupt handlers
  - USART/USART_Communication_Rx_IT_Init/Src/main.c                  Main program
  - USART/USART_Communication_Rx_IT_Init/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.  
  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.  
  - NUCLEO-U575ZI-Q Set-up  
    Connect USART2 TX/RX to respectively RX and TX pins of PC UART (could be done through a USB to UART adapter) :  
    - Connect STM32 MCU board USART2 TX pin (GPIO PA.02 connected to pin 13 of connector CN10)
      to PC COM port RX signal  
    - Connect STM32 MCU board USART2 RX pin (GPIO PA.03 connected to pin 1 of connector CN9) to PC COM port TX signal  
    - Connect STM32 MCU board GND to PC COM port GND signal
  - Launch serial communication SW on PC (as HyperTerminal or TeraTerm) with proper configuration
    (115200 bauds, 8 bits data, 1 stop bit, no parity, no HW flow control).  
  - Launch the program. Enter characters on PC communication SW side.  
### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

