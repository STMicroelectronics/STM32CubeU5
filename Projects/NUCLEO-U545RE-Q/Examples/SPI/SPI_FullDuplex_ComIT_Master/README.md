## <b>SPI_FullDuplex_ComIT_Master Example Description</b>

Data buffer transmission/reception between two boards via SPI using Interrupt mode.

	Board: NUCLEO-U545RE-Q (embeds a STM32U545RETxQ device)
	CLK Pin: PA1 (pin 02 on CN8 connector)
	MISO Pin: PA6 (pin 05 on CN5 connector)
	MOSI Pin: PA7 (pin 04 on CN5 connector)

HAL architecture allows user to easily change code to move to DMA or Polling 
mode. To see others communication modes please check following examples:

 - SPI\SPI_FullDuplex_ComPolling_Master and SPI\SPI_FullDuplex_ComPolling_Slave
 - SPI\SPI_FullDuplex_ComDMA_Master and SPI\SPI_FullDuplex_ComDMA_Slave

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 160 MHz.

The SPI peripheral configuration is ensured by the HAL_SPI_Init() function.
This later is calling the HAL_SPI_MspInit()function which core is implementing
the configuration of the needed SPI resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change SPI configuration.

The SPI communication is then initiated.
The HAL_SPI_TransmitReceive_IT() function allows the reception and the 
transmission of a predefined data buffer at the same time (Full Duplex Mode).
If the Master board is used, the project SPI_FullDuplex_ComIT_Master must be used.
If the Slave board is used, the project SPI_FullDuplex_ComIT_Slave must be used.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button, SPI Master starts the
communication by sending aTxBuffer and receiving aRxBuffer through 
HAL_SPI_TransmitReceive_IT(), at the same time SPI Slave transmits aTxBuffer 
and receives aRxBuffer through HAL_SPI_TransmitReceive_IT(). 
The callback functions (HAL_SPI_TxRxCpltCallback and HAL_SPI_ErrorCallbackand) update 
the variable wTransferState used in the main function to check the transfer status.
Finally, aRxBuffer and aTxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

STM32 board's LEDs can be used to monitor the transfer status:

 - LED2 toggles quickly on master board waiting User push-button to be pressed.
 - LED2 turns ON if transmission/reception is complete and OK.
 - LED2 toggles slowly when there is a timeout or an error in transmission/reception process.   

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

Connectivity, SPI, Full-duplex, Interrupt, Transmission, Reception, Master, Slave, MISO, MOSI

### <b>Directory contents</b>

  - SPI/SPI_FullDuplex_ComIT_Master/Inc/stm32u5xx_nucleo_conf.h BSP configuration file
  - SPI/SPI_FullDuplex_ComIT_Master/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - SPI/SPI_FullDuplex_ComIT_Master/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - SPI/SPI_FullDuplex_ComIT_Master/Inc/main.h                  Header for main.c module  
  - SPI/SPI_FullDuplex_ComIT_Master/Src/stm32u5xx_it.c          Interrupt handlers
  - SPI/SPI_FullDuplex_ComIT_Master/Src/main.c                  Main program
  - SPI/SPI_FullDuplex_ComIT_Master/Src/system_stm32u5xx.c      stm32u5xx system source file
  - SPI/SPI_FullDuplex_ComIT_Master/Src/stm32u5xx_hal_msp.c     HAL MSP file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U545RETxQ devices.

  - This example has been tested with NUCLEO-U545RE-Q board and can be
    easily tailored to any other supported device and development board.

  - NUCLEO-U545RE-Q Set-up

    - Connect Master board PA1 (pin 02 on CN8 connector) to Slave Board PA1 (pin 02 on CN8 connector)
    - Connect Master board PA6 (pin 05 on CN5 connector) to Slave Board PA6 (pin 05 on CN5 connector)
    - Connect Master board PA7 (pin 04 on CN5 connector) to Slave Board PA7 (pin 04 on CN5 connector)
    - Connect Master board GND  to Slave Board GND

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files (master project) and load your image into target memory
    - Load the project in Master Board
 - Rebuild all files (slave project) and load your image into target memory
    - Load the project in Slave Board
 - Run the example

