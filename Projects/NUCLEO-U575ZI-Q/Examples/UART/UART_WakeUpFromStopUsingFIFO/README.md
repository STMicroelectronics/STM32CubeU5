## <b>UART_WakeUpFromStopUsingFIFO Example Description</b>

Configuration of an UART to wake up the MCU from Stop mode with a FIFO level
when a given stimulus is received.

    Board: NUCLEO-U575ZI-Q (embeds a STM32U575ZITxQ device)
    Tx Pin: PA9
    Rx Pin: PA10

LED1 is ON when MCU is not in STOP mode and turn off when MCU is in STOP mode.
If the test fails or if there is an initialization or transfer error, LED1
transmits a sequence of three dots, three dashes, three dots.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 160 MHz for STM32U5xx Devices.

In the first part of the example, the USART1 is configured for
reception with Rx FIFO threshold set to one 1 QUARTER and the CPU enters into
Stop Mode. The UART FIFO threshold interrupt is enabled and, by default,
the UART FIFO full is disabled. The user has to send 2 bytes from the
HyperTerminal. After 2 received bytes, the UART FIFO will reach the quarter
level which will generate an interrupt and wake up the CPU.

In the second part of the example, the USART1 is configured for
reception with FIFO threshold interrupt disabled and FIFO full enabled.
Then the CPU goes to stop mode. The user has to send 8 bytes (size of the UART
FIFO) from the HyperTerminal. After 8 received bytes, the UART FIFO will be
full, which will generate an interrupt and wake up the CPU.

The USART1 is configured as follows:
    - BaudRate = 115200 baud
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - One Stop Bit
    - Odd parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Reception and transmission are enabled in the time

#### <b>Notes</b>

 1. When the parity is enabled, the computed parity is inserted at the MSB
    position of the transmitted data.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Connectivity, UART/USART, baud rate, Wake Up, Low Power, HyperTerminal, full-duplex, Interrupt
Transmitter, Receiver, Asynchronous

### <b>Directory contents</b>

  - UART/UART_WakeUpFromStopUsingFIFO/Inc/stm32u5xx_nucleo_conf.h     BSP configuration file
  - UART/UART_WakeUpFromStopUsingFIFO/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - UART/UART_WakeUpFromStopUsingFIFO/Inc/stm32u5xx_it.h          DMA interrupt handlers header file
  - UART/UART_WakeUpFromStopUsingFIFO/Inc/main.h                   Header for main.c module
  - UART/UART_WakeUpFromStopUsingFIFO/Src/stm32u5xx_it.c          DMA interrupt handlers
  - UART/UART_WakeUpFromStopUsingFIFO/Src/main.c                   Main program
  - UART/UART_WakeUpFromStopUsingFIFO/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - UART/UART_WakeUpFromStopUsingFIFO/Src/system_stm32u5xx.c      STM32U5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575ZITxQ devices.

  - This example has been tested with NUCLEO-U575ZI-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U575ZI-Q Set-up
      Connect a USB cable between the ST-Link USB connector
      and PC to display data on the HyperTerminal.

  - Hyperterminal configuration:
    - Data Length = 7 Bits
    - One Stop Bit
    - Odd parity
    - BaudRate = 115200 baud
    - Flow control: None

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

