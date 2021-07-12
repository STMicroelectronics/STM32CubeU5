## <b>SPI_FullDuplex_ComDMA_Autonomous_Master Example Description</b>

How to handle autonomously data buffer transmission/reception between two boards via SPI using DMA mode.

     - Board    : NUCLEO-U575ZI-Q
     - CLK  Pin : PA5 (CN7.D13)
     - MISO Pin : PA6 (CN7.D12)
     - MOSI Pin : PA7 (CN7.D11)

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

In a first step after the user press the User push-button, SPI Master starts the
communication, after the trigger event (Transfer Complete GPDMA1 Channel3),by sending aTxBuffer
and receiving aRxBuffer through HAL_SPI_TransmitReceive_DMA(), at the same time SPI Slave transmits
aTxBuffer and receives aRxBuffer through HAL_SPI_TransmitReceive_DMA().
The callback functions (HAL_SPI_TxRxCpltCallback and HAL_SPI_ErrorCallbackand) update
the variable wTransferState used in the main function to check the transfer status.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to check buffers correctness.

STM32 board's LEDs can be used to monitor the transfer status:

 - LED1 toggles quickly on master board waiting for the User push-button to be pressed.
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

Connectivity, SPI, full-duplex, DMA, baud rate, data size, Trigger Mode, Master, Slave, Transmitter, Receiver

### <b>Directory contents</b>

  - SPI/SPI_FullDuplex_ComDMA_Autonomous_Master/Src/linked_list.c           Linked list source file
  - SPI/SPI_FullDuplex_ComDMA_Autonomous_Master/Src/main.c                  Main program source file
  - SPI/SPI_FullDuplex_ComDMA_Autonomous_Master/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - SPI/SPI_FullDuplex_ComDMA_Autonomous_Master/Src/stm32u5xx_it.c          Interrupt handlers source file
  - SPI/SPI_FullDuplex_ComDMA_Autonomous_Master/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - SPI/SPI_FullDuplex_ComDMA_Autonomous_Master/Src/linked_list.h           Linked list header file
  - SPI/SPI_FullDuplex_ComDMA_Autonomous_Master/Inc/main.h                  Main program header file
  - SPI/SPI_FullDuplex_ComDMA_Autonomous_Master/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
  - SPI/SPI_FullDuplex_ComDMA_Autonomous_Master/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - SPI/SPI_FullDuplex_ComDMA_Autonomous_Master/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U575ZI-Q Set-up

    - Connect Master board SCK pin  (PA5) to Slave Board SCK pin  (PA5) (connector CN7.D13)
    - Connect Master board MISO pin (PA6) to Slave Board MISO pin (PA6) (connector CN7.D12)
    - Connect Master board MOSI pin (PA7) to Slave Board MOSI pin (PA7) (connector CN7.D11)
    - Connect Master board GND to Slave Board GND

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load the project in Master Board
 - Run the example

