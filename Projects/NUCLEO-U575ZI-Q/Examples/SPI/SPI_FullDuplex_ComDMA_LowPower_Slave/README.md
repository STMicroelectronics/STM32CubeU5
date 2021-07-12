## <b>SPI_FullDuplex_ComDMA_LowPower_Slave Example Description</b>

How to handle data buffer transmission/reception in Low Power mode between two boards via SPI using DMA mode.

        - Board    : NUCLEO-U575ZI-Q
        - CLK  Pin : PC10 (CN8.D45)
        - MISO Pin : PC11 (CN8.D46)
        - MOSI Pin : PC12 (CN8.D47)

This project is targeted to run on STM32U575xx devices on NUCLEO-U575ZI-Q board from STMicroelectronics.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick. The SystemClock_Config() function
is used to configure the system clock for STM32U575 devices and to make the CPU running at 160 Mhz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK &
GPIO). You may update this function to change SPI configuration.

The SPI communication is then initiated.
The HAL_SPI_TransmitReceive_DMA() function allows the reception and the
transmission of a predefined data buffer at the same time (Full Duplex Mode).

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step the user must run the Slave program: get the SPI instance ready to communicate,
disable the error interrupts, enable the End Of Transfer (EOT) interrupt then entre the whole
system in Stop2 mode. The next step, after the user press the User push-button, SPI Master starts the
communication by sending aTxBuffer and receiving aRxBuffer through
HAL_SPI_TransmitReceive_DMA(), at the same time SPI Slave transmits aTxBuffer
and receives aRxBuffer through HAL_SPI_TransmitReceive_DMA() in Low Power Mode.
The wake up from Low Power mode will be done when the End Of Transfer interrupt occurs.
The callback functions (HAL_SPI_TxRxCpltCallback and HAL_SPI_ErrorCallbackand) update
the variable wTransferState used in the main function to check the transfer status.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to check buffers correctness.

STM32 board's LEDs can be used to monitor the transfer status:

 - LED1 toggles quickly on master board waiting User push-button to be pressed.
 - LED1 turns ON when the transmission process is complete.
 - LED2 turns ON when the reception process is complete.
 - LED3 turns ON when there is an error in transmission/reception process.

#### <b>Notes</b>

 1. You need to perform a reset on Slave board, then perform it on Master board
    to have the correct behaviour of this example.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application need to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Connectivity, SPI, full-duplex, DMA, baud rate, data size, Low power, Stop2 , Master, Slave, Transmitter, Receiver

### <b>Directory contents</b>

  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Src/linked_list.c           Linked list source file
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Src/main.c                  Main program source file
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Src/stm32u5xx_it.c          Interrupt handlers
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Src/linked_list.h           Linked list header file
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Inc/main.h                  Main program header file
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - SPI/SPI_FullDuplex_ComDMA_LowPower_Slave/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U575ZI-Q Set-up

    - Connect Master board SCK pin  (PC10) to Slave Board SCK pin  (PC10) (connector CN8.D45)
    - Connect Master board MISO pin (PC11) to Slave Board MISO pin (PC11) (connector CN8.D46)
    - Connect Master board MOSI pin (PC12) to Slave Board MOSI pin (PC12) (connector CN8.D47)
    - Connect Master board GND to Slave Board GND

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load the project in Slave Board
 - Run the example

