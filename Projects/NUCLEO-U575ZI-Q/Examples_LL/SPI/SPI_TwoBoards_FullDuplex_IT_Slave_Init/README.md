## <b>SPI_TwoBoards_FullDuplex_IT_Slave_Init Example Description</b>

Data buffer transmission and receptionvia SPI using Interrupt mode.

This example is based on the STM32U5xx SPI LL API. The peripheral
initialization uses LL unitary service functions for optimization purposes (performance and size).

The communication is done with 2 boards through SPI.

This example shows how to configure GPIO and SPI peripherals
to use a Full-Duplex communication using IT mode through the STM32U5xx COM_INSTANCE1_TYPE LL API.

This example is split in two projects, Master board and Slave board:

- Master Board  
  SPI1 Peripheral is configured in Master mode.  
  SPI1_IRQn activated.  
  RXP and TXP Interrupts SPI peripheral activated.  
  GPIO associated to User push-button is linked with EXTI.  

- Slave Board  
  SPI1 Peripheral is configured in Slave mode.  
  SPI1_IRQn activated.  
  RXP and TXP Interrupts SPI peripheral activated.

Example execution:  
On BOARD MASTER, LED1 is blinking Fast (200ms) and wait User push-button action.  
Press User push-button on BOARD MASTER start a Full-Duplex communication through IT.  
On MASTER side, Clock will be generated on SCK line, Transmission(MOSI Line) and reception (MISO Line)
will be done at the same time.  
SLAVE SPI will received  the Clock (SCK Line), so Transmission(MISO Line) and reception (MOSI Line) will be done also.

LED1 is On on both boards if data is well received.

In case of errors, LED1 is blinking Slowly (1s).

#### <b>Notes</b>
 1. You need to perform a reset on Master board, then perform it on Slave board
    to have the correct behaviour of this example.

### <b>Keywords</b>

Connectivity, SPI, Full-duplex, Transmission, Reception, Slave, MISO, MOSI, Interrupt

### <b>Directory contents</b>

  - SPI/SPI_TwoBoards_FullDuplex_IT_Slave_Init/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - SPI/SPI_TwoBoards_FullDuplex_IT_Slave_Init/Inc/main.h                  Header for main.c module
  - SPI/SPI_TwoBoards_FullDuplex_IT_Slave_Init/Inc/stm32_assert.h          Template file to include assert_failed function
  - SPI/SPI_TwoBoards_FullDuplex_IT_Slave_Init/Src/stm32u5xx_it.c          Interrupt handlers
  - SPI/SPI_TwoBoards_FullDuplex_IT_Slave_Init/Src/main.c                  Main program
  - SPI/SPI_TwoBoards_FullDuplex_IT_Slave_Init/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U575ZI-Q Set-up  
    - Connect Master board PA5 to Slave Board PA5 (connected to pin 10 of CN7 connector)  
    - Connect Master board PA6 to Slave Board PA6 (connected to pin 12 of CN7 connector)  
    - Connect Master board PA7 to Slave Board PA7 (connected to pin 14 of CN7 connector)  
    - Connect Master board GND to Slave Board GND

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory  
    o Load the Master project in Master Board  
    o Load the Slave project in Slave Board  
 - Run the example

