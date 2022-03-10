## <b>LPBAM_SPI_TransmitReceive Application Description</b>
-   How to handle SPI two consecutive communications (full duplex then simplex) between two boards with DMA linked-list
feature in low power mode through LPBAM utility.

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

-   The SPI peripheral configuration is ensured by HAL_SPI_Init() function which calls HAL_SPI_MspInit() function.
This HAL_SPI_MspInit() function allows the user to configure the needed SPI resources according to the used hardware
(CLOCK, GPIO).

-   LPBAM_SPI_TransmitReceive application contains :
    -   One LPBAM sub-application named **TxRx**.
-   TxRx application contains :
    -   One LPBAM scenario named **MultiConf**.

-   This LPBAM application is dedicated only for Master Board.

-   The LPBAM TxRx application configures the system to reach the lowest power consumption.

-   The LPBAM MultiConf scenario configures the used peripherals and builds the scenario linked-list.

-   The MultiConf scenario performs 2 different sequences:
    -   Sequence 1 (Full duplex)
        -   Transmit "aTxBuffer1" buffer and receive "aTxBuffer1" in "aRxBuffer1" buffer.
        -   Start transmission is conditioned by the PWM signal rising edge.
    -   Sequence 2 (Simplex)
        -   Transmit "aTxBuffer2" buffer.
        -   Start transmission is conditioned by the PWM signal rising edge.

-   After configuring, building, linking and starting the LPBAM scenario the MCU enters in Stop2 mode.

-   When transfers are completed, the SPI generates the EOT (end of transfer) interrupt to wake up the system.

-   After wake up from Stop2 mode, the main application checks the data transfers then turns on the green led when there
are no issue detected otherwise the red led will turns on.

-   For slave board, the main application performs 2 sequences :
    -   Sequence 1 (Full duplex)
        -   Transmit "aTxBuffer1" buffer and receive "aTxBuffer1" in "aRxBuffer1" buffer.
    -   Sequence 2
        -   Receive "aTxBuffer2" buffer in aRxBuffer2 buffer.

-   When transfers are completed, the main application checks the data transfer then turns on the green led when
there are no issue detected otherwise the red led will turns on.

-   For synchronization purpose between two boards, the user push-button pin (PC.13) is configured as input with external
interrupt (EXTI_Line13), falling edge. After startup, the Master board will configure transmission scenario then turn on
blue led and wait for user push-button pressing to resume program execution.

-   In the "main.h" file:
If the Master board is used, the "#define MASTER_BOARD" must be uncommented, and the "#define MASTER_BOARD" commented in
the Slave board.

-   The typical average power consumption of the system performing SPI transfers is 12uA.

-   NUCLEO-U575ZI-Q board LEDs are used to monitor the transfer status:
    -   LED1 (green led) is turned on at the end of application and no error is detected.
    -   LED2 (blue led) is turned on after configuring transfer and before pressing the user push-button on Master
    board.
    -   LED3 (red led) is turned on when any error is occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

 3. This application (only Master board) can not be used in DEBUG mode due to the fact that the debug pins are
      configured in analog mode for power consumption and Cortex-M33 core is no longer clocked during low power mode so
      debugging features are disabled.

 4. When deleting project folder (EWARM, MDK-ARM or STM32CubeIDE) and regenerating this application with STM32CubeMx tool, 
      ensure to re-adjust linker ROM and RAM sections addresses.

### <b>Keywords</b>

Connectivity, SPI, LPDMA, LPBAM, STOP2, Low power, Full-Duplex, Master, Slave, BaudRate, Transmitter, Receiver

### <b>Directory contents</b>

-   LPBAM/LPBAM_SPI_TransmitReceive/Core/Src/main.c                             Main program file
-   LPBAM/LPBAM_SPI_TransmitReceive/Core/Src/system_stm32u5xx.c                 STM32U5xx system clock configuration file
-   LPBAM/LPBAM_SPI_TransmitReceive/Core/Src/stm32u5xx_it.c                     Interrupt handlers file
-   LPBAM/LPBAM_SPI_TransmitReceive/Core/Src/stm32u5xx_hal_msp.c                HAL MSP module file
-   LPBAM/LPBAM_SPI_TransmitReceive/Core/Inc/stm32_lpbam_conf.h                 LPBAM Configuration file
-   LPBAM/LPBAM_SPI_TransmitReceive/Core/Inc/main.h                             Main program header file
-   LPBAM/LPBAM_SPI_TransmitReceive/Core/Inc/stm32u5xx_nucleo_conf.h            BSP Configuration file
-   LPBAM/LPBAM_SPI_TransmitReceive/Core/Inc/stm32u5xx_hal_conf.h               HAL Configuration file
-   LPBAM/LPBAM_SPI_TransmitReceive/Core/Inc/stm32u5xx_it.h                     Interrupt handlers header file
-   LPBAM/LPBAM_LPUART_TransmitReceive/LPBAM/TxRx/lpbam_txrx.h                  LPBAM TxRx application header file
-   LPBAM/LPBAM_LPUART_TransmitReceive/LPBAM/TxRx/lpbam_txrx_config.c           LPBAM TxRx application configuration file
-   LPBAM/LPBAM_LPUART_TransmitReceive/LPBAM/TxRx/lpbam_txrx_multiconf_config.c LPBAM TxRx application MultiConf scenario configuration file
-   LPBAM/LPBAM_LPUART_TransmitReceive/LPBAM/TxRx/lpbam_txrx_multiconf_build.c  LPBAM TxRx application MultiConf scenario build file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q Set-up
    -   Connect Master board SCK pin PC.10 (pin 1 in CN11 connector) to Slave Board SCK pin PA.5 (pin 11 in CN12 connector)
    -   Connect Master board MISO pin PC.11 (pin 2 in CN11 connector) to Slave Board MISO pin PA.6 (pin 13 in CN12 connector)
    -   Connect Master board MOSI pin PC.12 (pin 3 in CN11 connector) to Slave Board MOSI pin PA.7 (pin 15 in CN12 connector)
    -   Connect Master board GND to Slave Board GND

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
        - Uncomment "#define MASTER_BOARD" and load the project in Master Board
        - Comment "#define MASTER_BOARD" and load the project in Slave Board
    -   UnPlug then Plug STLINK connection to perform a power-on-reset for transmitter board.