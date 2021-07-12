## <b>I2C_TwoBoards_ComDMA_Autonomous_Slave Example Description</b>

How to handle autonomously I2C data buffer transmission/reception between two boards in DMA mode through GPDMA1_Channel_3 trigger.

      - Board: NUCLEO-U575ZI-Q
      - SCL Pin: PB10 (CN10, D36)
      - SDA Pin: PB11 (CN10, D35)

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick. 
The SystemClock_Config() function is used to configure the system clock for STM32U575 devices and to make the CPU running at 160 Mhz.

The I2C peripheral configuration is ensured by HAL_I2C_Init() function which calls HAL_I2C_MspInit() function.
This HAL_I2C_MspInit() function allows the user to configure the needed I2C resources according to the used hardware (CLOCK, GPIO).

The call of HAL_I2C_Slave_Receive() and the HAL_I2C_Slave_Transmit() function allows respectively 
the reception and the transmission of a predefined data buffer in Slave mode using DMA.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button on the Master Board,
I2C Master starts the communication by sending aTxBuffer through HAL_I2C_Master_Transmit()
after the trigger event (Transfer Complete GPDMA1 Channel3) to I2C Slave which receives
aRxBuffer through HAL_I2C_Slave_Receive(). The second step starts when the user press
the User push-button on the Master Board, the I2C Slave sends aTxBuffer through
HAL_I2C_Slave_Transmit() to the I2C Master which receives aRxBuffer through
HAL_I2C_Master_Receive() after the trigger event (Transfer Complete GPDMA1 Channel3).
The end of this two steps are monitored through the HAL_I2C_GetState() function
result. Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to
check buffers correctness.

NUCLEO-U575ZI-Q's LEDs can be used to monitor the transfer status:

 - LED2 toggles quickly on master board waiting for the User push-button to be pressed.
 - LED2 turns OFF as soon as the push-button is pressed.
 - LED1 is ON when the Master transmission / Slave reception process is complete.
 - LED2 toggles quickly on master board waiting for the User push-button to be pressed another time.
 - LED2 turns OFF as soon as the push-button is pressed a second time.
 - LED1 is OFF when the Master reception / Slave transmission process is complete
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

Connectivity, I2C, half-duplex, DMA, 10-bit addressing, data size, Trigger, Master, Slave, Transmitter, Receiver

### <b>Directory contents</b>

  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Slave/Src/main.c                  Main program source file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Slave/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Slave/Src/stm32u5xx_it.c          Interrupt handlers source file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Slave/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Slave/Src/linked_list.c           Linked list source file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Slave/Inc/main.h                  Main program header file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Slave/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Slave/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Slave/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - I2C/I2C_TwoBoards_ComDMA_Autonomous_Slave/Inc/linked_list.h           Linked list header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U575ZI-Q Set-up
    - Connect I2C_SCL line of Master board (PB10, CN10, D36) to I2C_SCL line of Slave Board (PB10, CN10, D36).
    - Connect I2C_SDA line of Master board (PB11, CN10, D35) to I2C_SDA line of Slave Board (PB11, CN10, D35).
    - Connect GND of Master board to GND of Slave Board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

