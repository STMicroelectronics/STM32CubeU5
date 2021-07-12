## <b>I2C_TwoBoards_ComDMA_LowPower Example Description</b>

How to handle I2C data buffer transmission/reception in Low Power Mode between two boards in DMA mode.

     - Board: NUCLEO-U575ZI-Q
     - SCL Pin: PC0 (CN9, A5)
     - SDA Pin: PC1 (CN9, A4)

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick. The SystemClock_Config() function is used to configure the system clock for STM32U575 Devices :
The CPU at 160Mhz.

The I2C peripheral configuration is ensured by HAL_I2C_Init() function which calls HAL_I2C_MspInit() function.
This HAL_I2C_MspInit() function allows the user to configure the needed I2C resources according to the used hardware (CLOCK, GPIO).

The I2C communication is then initiated.

The project is split in two parts: the Master Board and the Slave Board
- Master Board :
  The HAL_I2C_Master_Transmit() function allows the transmission of a predefined data buffer in Master mode using DMA.
- Slave Board :
  The HAL_I2C_Slave_Receive() function allows the reception of a predefined data buffer in Slave mode using DMA during low power mode.
  
The user can choose between Master and Slave through "#define MASTER_BOARD" in the "main.c" file:
 - If the Master board is used, the "#define MASTER_BOARD" must be uncommented.
 - If the Slave board is used the "#define MASTER_BOARD" must be commented.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

The user press the User push-button on the Master Board, I2C Master starts the communication
by sending aTxBuffer through HAL_I2C_Master_Transmit() to I2C Slave which receives
aRxBuffer through HAL_I2C_Slave_Receive() in low Power mode.
The end of this step is monitored through the HAL_I2C_GetState() function.
Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to
check buffers correctness.

NUCLEO-U575ZI-Q's LEDs can be used to monitor the transfer status:
 - LED2 toggles quickly on master board waiting for the User push-button to be pressed.
 - LED2 turns OFF as soon as the push-button is pressed.
 - LED1 is ON when the Master transmission process is complete.
 - LED1 is ON when the Slave reception process is complete.
 - LED3 is ON when there is an error in transmission/reception process.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Connectivity, I2C, half-duplex, DMA, 10-bit addressing, data size, Low power, Stop2, Master, Slave, Transmitter, Receiver

### <b>Directory contents</b>

  - I2C/I2C_TwoBoards_ComDMA_LowPower/Src/main.c                  Main program source file
  - I2C/I2C_TwoBoards_ComDMA_LowPower/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - I2C/I2C_TwoBoards_ComDMA_LowPower/Src/stm32u5xx_it.c          Interrupt handlers source file
  - I2C/I2C_TwoBoards_ComDMA_LowPower/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - I2C/I2C_TwoBoards_ComDMA_LowPower/Src/linked_list.c           Linked list source file
  - I2C/I2C_TwoBoards_ComDMA_LowPower/Inc/main.h                  Main program header file
  - I2C/I2C_TwoBoards_ComDMA_LowPower/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
  - I2C/I2C_TwoBoards_ComDMA_LowPower/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - I2C/I2C_TwoBoards_ComDMA_LowPower/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - I2C/I2C_TwoBoards_ComDMA_LowPower/Inc/linked_list.h           Linked list header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U575ZI-Q Set-up
  
    - Connect I2C_SCL line of Master board (PC0, CN9, A5) to I2C_SCL line of Slave Board (PC0, CN9, A5).
    - Connect I2C_SDA line of Master board (PC1, CN9, A4) to I2C_SDA line of Slave Board (PC1, CN9, A4).
    - Connect GND of Master board to GND of Slave Board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example

