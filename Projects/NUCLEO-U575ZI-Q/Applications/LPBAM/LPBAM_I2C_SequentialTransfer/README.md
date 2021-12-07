## <b>LPBAM_I2C_SequentialTransfer Application Description</b>
-   How to handle I2C sequential transmission/reception with data reload between two boards with
DMA linked-list feature in low power mode through LPBAM utility.

-   Board: NUCLEO-U575ZI-Q
    -   SCL Pin: PC0
    -   SDA Pin: PC1

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick. The SystemClock_Config()
function is used to configure the system clock for STM32U575 Devices :
The CPU at 160Mhz.

-   The I2C peripheral configuration is ensured by HAL_I2C_Init() function which calls HAL_I2C_MspInit() function.
This HAL_I2C_MspInit() function allows the user to configure the needed I2C resources according to the used hardware
(CLOCK, GPIO).

-   The project is split in two parts via compilation flags :
    -   Master Board
    The ADV_LPBAM_I2C_MasterTx_SetDataQ() function allow the transmission of a predefined data buffers in
    Master mode using DMA linked-list.
    The ADV_LPBAM_I2C_MasterRx_SetDataQ() function allow the reception of a predefined data buffers in
    Master mode using DMA linked-list.
    -   Slave Board
    The ADV_LPBAM_I2C_SlaveTx_SetDataQ() function allow the transmission of a predefined data buffers in
    Slave mode using DMA linked-list.
    The ADV_LPBAM_I2C_SlaveRx_SetDataQ() functions allow the reception of a predefined data buffers in
    Slave mode using DMA linked-list.

-   In the "main.h" file:
If the Master board is used, the "#define MASTER_BOARD" must be uncommented, and the "#define MASTER_BOARD" commented in
the Slave board.

-   I2C Master Sequence
    -   In a first step, the MASTER_BOARD application calls ADV_LPBAM_I2C_Masterx_SetDataQ() to send N1 data less
    than 255 bytes, then calls ADV_LPBAM_I2C_MasterRx_SetDataQ() to receive N2 data more than 255 bytes (Reload Data).
    After that, a linked-list queue is created and placed in the SRAM that will be executed by a DMA channel in STOP 2 low
    power mode.
    This queue contains configuration nodes to set-up the master I2C each time a transfer sequence is achieved and contains
    data nodes to ensure data transfer in DMA mode.
    When queue is well created, the application waits for the user to press the user push-button. Then, the RTC is
    configured to generate an alarm signal after 5 seconds. It is used to trigger the I2C communication (Auto Start).
    After that, the whole system enters in STOP 2 mode.

-   I2C Slave Sequence
    -   In a first step, The SLAVE_BOARD application calls ADV_LPBAM_I2C_SlaveRx_SetDataQ() to receive N1 data less
    than 255 bytes, then calls ADV_LPBAM_I2C_SlaveTx_SetDataQ() to send N2 data more than 255 bytes (Reload Data).
    After that, a linked-list queue is created and placed in the SRAM that will be executed by a DMA channel in STOP 2 low
    power mode.
    This queue contains configuration nodes to set-up the slave I2C each time a transfer sequence is achieved
    and contains data nodes to ensure data transfer in DMA mode.
    When queue is well created, the whole system enters in STOP 2 mode.

-   Sequence 1
    -   When the RTC alarm is generated, the master generates its start condition and begins the transmission of N1 data
    (aTxBuffer1 buffer).
    At the same time, the slave receives the start condition and begins the reception of N1 data (aRxBuffer1 buffer).

-   Sequence 2
    -   The DMA configures the master to receive N2 data more than 255 bytes (Reload Data) (aRxBuffer2 buffer).
    At the same time, the DMA configures the slave to send N2 data more than 255 bytes (Reload Data) (atxBuffer2 buffer).

-   When the communication is achieved, the DMA generates a transfer complete interrupt that wakes up the system.

-   Finally, aTxBuffer1, aRxBuffer1 and aTxBuffer2, aRxBuffer2 are compared through Buffercmp() in order to check buffers
correctness.

-   This project contains two configuration :
    -   Debug configuration : uncomment DEBUG_CONFIGURATION flag in the main.h.
    -   Power measurement configuration : comment DEBUG_CONFIGURATION flag in the main.h.

-   The typical average power consumption of the system performing I2C master transmit then receive with reloading data
is 20.4uA.

-   NUCLEO-U575ZI-Q's LEDs can be used to monitor the transfer status:
    -   LED_GREEN toggles when the transfer process is complete.
    -   LED_BLUE toggles when the application waits pressing User push-button.
    -   LED_RED is ON when any error occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Connectivity, I2C, LPDMA, LPBAM, STOP2, Low power, half-duplex, 7-bit addressing, Master, Slave, Transmitter, Receiver

### <b>Directory contents</b>

-   LPBAM/LPBAM_I2C_SequentialTransfer/Src/main.c                  Main program
-   LPBAM/LPBAM_I2C_SequentialTransfer/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
-   LPBAM/LPBAM_I2C_SequentialTransfer/Src/stm32u5xx_it.c          Interrupt handlers
-   LPBAM/LPBAM_I2C_SequentialTransfer/Src/stm32u5xx_hal_msp.c     HAL MSP module
-   LPBAM/LPBAM_I2C_SequentialTransfer/Src/i2c_timing_utility.c    Timing utility Source file
-   LPBAM/LPBAM_I2C_SequentialTransfer/Inc/stm32_lpbam_conf.h      LPBAM Configuration file
-   LPBAM/LPBAM_I2C_SequentialTransfer/Inc/main.h                  Main program header file
-   LPBAM/LPBAM_I2C_SequentialTransfer/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
-   LPBAM/LPBAM_I2C_SequentialTransfer/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
-   LPBAM/LPBAM_I2C_SequentialTransfer/Inc/stm32u5xx_it.h          Interrupt handlers header file
-   LPBAM/LPBAM_I2C_SequentialTransfer/Inc/i2c_timing_utility.h    Timing utility header file


### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q Set-up
    -   Connect I2C_SCL line of Master board (PC0, CN9, A5) to I2C_SCL line of Slave Board (PC0, CN9, A5).
    -   Connect I2C_SDA line of Master board (PC1, CN9, A4) to I2C_SDA line of Slave Board (PC1, CN9, A4).
    -   Connect GND of Master board to GND of Slave Board.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
        - Uncomment "#define MASTER_BOARD" and load the project in Master Board
        - Comment "#define MASTER_BOARD" and load the project in Slave Board
    -   Run the application

