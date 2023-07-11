## <b>USART_SlaveMode Example Description</b>

- This example describes an USART-SPI communication (transmit/receive) between two boards where the USART is configured as a slave.

- Boards: NUCLEO-U575ZI-Q (embeds a STM32U575xx device).
- Two identical boards are connected as shown below.  
  - Board 1: SPI Master board
  - Board 2: USART Slave board

- In a first step, after the user press on the User push-button, Master (SPI) starts
the communication by sending aTxBuffer and receiving aRxBuffer through
HAL_SPI_TransmitReceive(), at the same time Slave (USART) transmits aTxBuffer
and receives aRxBuffer through HAL_USART_TransmitReceive().
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to
check buffers correctness.

**WARNING**: as both boards do not behave the same way, "SLAVE_BOARD" compilation
switch is defined in Src/main.c and must be enabled at compilation time before
loading the executable in the USART slave board.
The SPI master board needs to be loaded with an executable software obtained
with SLAVE_BOARD disabled.
if you want to repeat the execution of the example, you should reset the master board
first else the execution will fail and a red led will be on.

NUCLEO-U575ZI-Q board LEDs are used to monitor the transfer status:

- While master board is waiting for the user to press the User push-button, its LED1 is blinking rapidly (100 ms period).
- When the transmission/reception processes starts, LED1 is turned off.
- When the test passes, LED1 on both boards is turned on, otherwise the
  test has failed.
- If there is an initialization or transfer error, LED3 is turned on.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :  
  - The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate
    delay (in milliseconds) based on variable incremented in SysTick ISR. This
    implies that if HAL_Delay() is called from a peripheral ISR process, then
    the SysTick interrupt must have higher priority (numerically lower) than
    the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use
    HAL_NVIC_SetPriority() function.

 2. The example needs to ensure that the SysTick time base is always set to 1 millisecond to have correct HAL operation.

 3. It is highly recommended to set the SPI parameter "MasterKeepIOState" to "SPI_MASTER_KEEP_IO_STATE_ENABLE"
    at init time. This configuration allows to avoid glitches on SPI lines especially the clock.
    Glitches that could lead to Master/Slave desynchronization. This config allows SPI Master to keep
    control on output state even if it is disabled.

### <b>Keywords</b>

Connectivity, UART/USART, SPI interface, full-duplex, Polling, Receiver, Transmitter, Synchronous, Slave

### <b>Directory contents</b>

  - UART/USARTSlaveMode/Inc/main.h                  Main program header file
  - UART/USARTSlaveMode/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - UART/USARTSlaveMode/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - UART/USARTSlaveMode/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
  - UART/USARTSlaveMode/Src/main.c                  Main program source file
  - UART/USARTSlaveMode/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - UART/USARTSlaveMode/Src/stm32u5xx_it.c          Interrupt handlers source file
  - UART/USARTSlaveMode/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.

  - This example has been tested with two NUCLEO-U575ZI-Q boards embedding
    a STM32U575xx device and can be easily tailored to any other supported device
    and development board.

    - NUCLEO-U575ZI-Q set-up
    - Connect Master board PB03(CN7 pin 15) to Slave Board (CN09 pin 2)PD07
    - Connect Master board PB04(CN7 pin 19) to Slave Board (CN09 pin 6)PD05
    - Connect Master board PB05(CN7 pin 13) to Slave Board (CN09 pin 4)PD06
    - Connect Master board GND to Slave Board GND

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory :

    - Uncomment "#define SLAVE_BOARD" and load the project in Slave Board
    - Comment "#define SLAVE_BOARD" and load the project in Master Board
  - Run the example

