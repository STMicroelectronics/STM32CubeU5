## <b>SMBUS_TwoBoards_ComIT_Master Example Description</b>

How to handle SMBUS data buffer transmission/reception between two boards, in interrupt mode.

  - Board: NUCLEO-U575ZI-Q (embeds a STM32U575ZITxQ device)
  - SCL Pin: PB.10 (CN10, D36)
  - SDA Pin: PB.11 (CN10, D35)

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575 Devices :

   - The CPU at 160 MHz.

The SMBUS peripheral configuration is ensured by HAL_SMBUS_Init() function which calls HAL_SMBUS_MspInit() function.
This HAL_SMBUS_MspInit() function allows the user to configure the needed SMBUS resources according to the used hardware (CLOCK,GPIO).

The SMBUS communication is then initiated.
The project is split in two parts: the Master Board and the Slave Board

- Master Board :
  The HAL_SMBUS_Master_Receive_IT() and the HAL_SMBUS_Master_Transmit_IT() functions
  allow respectively the reception and the transmission of a predefined data buffer
  and packet error checking (PEC) byte in Master mode using interrupt.
- Slave Board :
  The HAL_SMBUS_Slave_Receive_IT() and the HAL_SMBUS_Slave_Transmit_IT() functions
  allow respectively the reception and the transmission of a predefined data buffer
  and packet error checking (PEC) byte in Slave mode using interrupt.
If the Master board is used, the project SMBUS_TwoBoards_ComIT_Master must be used.
If the Slave board is used, the project SMBUS_TwoBoards_ComIT_Slave must be used.

For this example the aTxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step after the user press the User push-button on the Master Board,
SMBUS Master starts the communication by sending aTxBuffer through HAL_SMBUS_Master_Transmit_IT()
to SMBUS Slave which receives aRxBuffer through HAL_SMBUS_Slave_Receive_IT().
The second step starts when the user press the User push-button on the Master Board,
the SMBUS Slave sends aTxBuffer through HAL_SMBUS_Slave_Transmit_IT()
to the SMBUS Master which receives aRxBuffer through HAL_SMBUS_Master_Receive_IT().
Finally, aTxBuffer and aRxBuffer are compared through Buffercmp() in order to
check buffers correctness.

NUCLEO-U575ZI-Q's LEDs can be used to monitor the transfer status:

 - LED2 toggles quickly on master board waiting User push-button to be pressed.
 - LED2 turns OFF as soon as the push-button is pressed.
 - LED1 is ON when the transmission process is complete.
 - LED2 toggles quickly on master board waiting User push-button to be pressed.
 - LED2 turns OFF as soon as the push-button is pressed.
 - LED1 is OFF when the reception process is complete.
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

Connectivity, SMBUS, Timing, half-duplex, Interrupt, Master, Slave, Transmitter, Receiver

### <b>Directory contents</b>

  - SMBUS/SMBUS_TwoBoards_ComIT_Master/Inc/stm32u5xx_nucleo_conf.h     BSP configuration file
  - SMBUS/SMBUS_TwoBoards_ComIT_Master/Src/main.c                  Main program
  - SMBUS/SMBUS_TwoBoards_ComIT_Master/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - SMBUS/SMBUS_TwoBoards_ComIT_Master/Src/stm32u5xx_it.c          Interrupt handlers
  - SMBUS/SMBUS_TwoBoards_ComIT_Master/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - SMBUS/SMBUS_TwoBoards_ComIT_Master/Src/i2c_timing_utility.c    Timing utility Source file
  - SMBUS/SMBUS_TwoBoards_ComIT_Master/Inc/main.h                  Main program header file
  - SMBUS/SMBUS_TwoBoards_ComIT_Master/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - SMBUS/SMBUS_TwoBoards_ComIT_Master/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - SMBUS/SMBUS_TwoBoards_ComIT_Master/Inc/i2c_timing_utility.h    Timing utility header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575ZITxQ devices.

  - This example has been tested with STMicroelectronics NUCLEO-U575ZI-Q
    board and can be easily tailored to any other supported device
    and development board.

  - NUCLEO-U575ZI-Q Set-up
    - Connect I2C_SCL line of Master board (PB.10, CN10, D36) to I2C_SCL line of Slave Board (PB.10, CN10, D36).
    - Connect I2C_SDA line of Master board (PB.11, CN10, D35) to I2C_SDA line of Slave Board (PB.11, CN10, D35).
    - Connect GND of Master board to GND of Slave Board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

