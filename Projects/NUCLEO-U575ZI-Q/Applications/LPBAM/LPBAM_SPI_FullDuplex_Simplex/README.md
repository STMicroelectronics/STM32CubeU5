## <b>LPBAM_SPI_FullDuplex_Simplex Application Description</b>
-   How to handle SPI two consecutive communications (full duplex then simplex) between two boards with DMA linked-list
feature in low power mode through LPBAM utility.

-   Board: NUCLEO-U575ZI-Q

    -   SPI3 : Master Board
        - CLK  Pin: PC10 (CN8.D45)
        - MISO Pin: PC11 (CN8.D46)
        - MOSI Pin: PC12 (CN8.D47)

    -   SPI1 : Slave Board
        - CLK  Pin: PA5 (CN7.D13)
        - MISO Pin: PA6 (CN7.D12)
        - MOSI Pin: PA7 (CN7.D11)


-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick. The SystemClock_Config()
function is used to configure the system clock for STM32U575 Devices :
The CPU at 160Mhz.

-   The SPI peripheral configuration is ensured by HAL_SPI_Init() function which calls HAL_SPI_MspInit() function.
This HAL_SPI_MspInit() function allows the user to configure the needed SPI resources according to the used hardware
(CLOCK, GPIO).

-   The project is split in two parts:
    -   Master Board
        - Communication n°1 : Transmit & Receive Data in FullDuplex mode
        The ADV_LPBAM_SPI_TransmitReceive_SetDataQ() function is used to establish a full duplex communication
        using DMA linked-list in low power mode.
        - Communication n°2 : Transmit data in Simplex mode
        The ADV_LPBAM_SPI_Transmit_SetFullQ() function is used to establish a simplex transmission.
    -   Slave Board
        - Communication n°1 :
        The HAL_SPI_TransmitReceive_IT function is used to establish a full duplex communication.
        - Communication n°2 : Receive data in Simplex mode
        The HAL_SPI_Receive_IT function is used to establish a simplex reception.

-   In the "main.h" file:
If the Master board is used, the "#define MASTER_BOARD" must be uncommented, and the "#define MASTER_BOARD" commented in
the Slave board.

-   SPI Master Sequence :
    -   In a first step, the MASTER_BOARD application calls ADV_LPBAM_SPI_TransmitReceive_SetDataQ() function to to configure the
    SPI instance, send  TXBUFFERSIZE1 data and receive RXBUFFERSIZE1 data.
    Then, after the changeset of data width and the BaudRate, the MASTER_BOARD application calls again calls
    ADV_LPBAM_SPI_Transmit_SetFullQ() to reconfigure the SPI instance and send TXBUFFERSIZE2 data.

    -   After that, two linked-list queues are created and placed in the SRAM. Each queue will be executed by a specific DMA
    channel in STOP 2 low power mode.
    The Tx queue contains the configuration nodes to set-up the master SPI, and contains data nodes to ensure data
    transmission in DMA mode. The Rx queue contains data nodes to ensure data reception in DMA mode.
    When the queues are well created in SRAM, the application waits for the user to press the user push-button.
    Then, LPTIM is configured to be used as a trigger for the first configuration node of each full duplex communication.
    After that, the whole system enters in STOP 2 mode.

    -   A simple press on the push-button wakes up the system from low power mode.

-   SPI Slave Sequence :
    -   The SLAVE_BOARD application calls  the first time HAL_SPI_TransmitReceive_IT() to receive TXBUFFERSIZE1 data coming from
    the Master Board and transmit the same amount of data, then receiving again TXBUFFERSIZE2 data coming from the same
    Master Board.
    The SLAVE_BOARD application calls  the second time HAL_SPI_Receive_IT() to receive TXBUFFERSIZE2
    data coming from the same Master Board.

-   Check after the two communications :
    -   Finally, aTxBuffer1, aRxBuffer1 and aTxBuffer2, aRxBuffer2 are compared through Buffercmp() in order to check buffers
    correctness.

-   NUCLEO-U575ZI-Q's LEDs can be used to monitor the transfer status:
    -   LED_BLUE toggles waiting for any press on User push-button to start the communication.
    -   LED_GREEN turn on when the transfer process is complete.
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

Connectivity, SPI, LPDMA, LPBAM, STOP2, Low power, Full-Duplex, Master, Slave, BaudRate, Transmitter, Receiver

### <b>Directory contents</b>

-   LPBAM/LPBAM_SPI_FullDuplex/Src/main.c                  Main program file
-   LPBAM/LPBAM_SPI_FullDuplex/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
-   LPBAM/LPBAM_SPI_FullDuplex/Src/stm32u5xx_it.c          Interrupt handlers file
-   LPBAM/LPBAM_SPI_FullDuplex/Src/stm32u5xx_hal_msp.c     HAL MSP module file
-   LPBAM/LPBAM_SPI_FullDuplex/Inc/stm32_lpbam_conf.h      LPBAM Configuration file
-   LPBAM/LPBAM_SPI_FullDuplex/Inc/main.h                  Main program header file
-   LPBAM/LPBAM_SPI_FullDuplex/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
-   LPBAM/LPBAM_SPI_FullDuplex/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
-   LPBAM/LPBAM_SPI_FullDuplex/Inc/stm32u5xx_it.h          Interrupt handlers header file


### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q Set-up
    -   Connect Master board SCK pin  (PC10) to Slave Board SCK pin  (PA5)
    -   Connect Master board MISO pin (PC11) to Slave Board MISO pin (PA6)
    -   Connect Master board MOSI pin (PC12) to Slave Board MOSI pin (PA7)
    -   Connect Master board GND to Slave Board GND

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
        - Uncomment "#define MASTER_BOARD" and load the project in Master Board
        - Comment "#define MASTER_BOARD" and load the project in Slave Board
    -   Run the application

