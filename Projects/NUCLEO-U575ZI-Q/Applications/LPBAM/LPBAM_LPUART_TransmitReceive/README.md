## <b>LPBAM_LPUART_TransmitReceive Application Description</b>
-   How to handle LPUART transmission/reception between two boards with DMA linked-list feature in low power mode
through LPBAM utility.

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

-   LPBAM_LPUART_TransmitReceive application contains :
    -   One LPBAM sub-application named **TxRx**.
-   TxRx application contains :
    -   One LPBAM scenario named **SendByPack**.

-   This LPBAM application is dedicated only for "TRANSMITTER_BOARD".

-   The LPBAM TxRx application configures the system to reach the lowest power consumption.

-   The LPBAM SendByPack scenario configures the used peripherals and builds the scenario linked-list.

-   The SendByPack scenario performs 2 different sequences:
    -   Sequence 1
        -   Transmit "Transmit_Buffer_1" buffer.
        -   Each packet transmission (8 bytes as packet size) is triggered by the PWM signal rising edge.
    -   Sequence 2
        -   Transmit "Transmit_Buffer_2" buffer and receive "Transmit_Buffer_1" buffer in "Receive_Buffer" buffer.
        -   Each packet transmission (4 bytes as packet size) is triggered by the PWM signal rising edge.

-   After configuring, building, linking and starting the LPBAM scenario the MCU enters in Stop2 mode.

-   When transfers are completed, the DMA channels generates TC (transfer complete) interrupts to wake up the system.

-   After wake up from Stop2 mode, the main application checks the data transfer then turns on the green led when there
are no issue detected otherwise the red led will turns on.

-   For receiver board, the main application performs 2 sequences :
    -   Sequence 1
        -   Receive "Transmit_Buffer_1 + Transmit_Buffer_2" buffer in "Receive_Buffer" buffer.
    -   Sequence 2
        -   Transmit "Transmit_Buffer_1 + Transmit_Buffer_2" buffer from "Receive_Buffer" buffer.

-   When transfers are completed, the main application checks the data transfer then turns on the green led when
there are no issue detected otherwise the red led will turns on.

-   For synchronization purpose between two boards, the user push-button pin (PC.13) is configured as input with external
interrupt (EXTI_Line13), falling edge. After startup, each board will configure transmission scenario then turn on blue
led and wait for user push-button pressing to resume program execution.
-   It is mandatory to press the user push-button of receiver board before pressing the user push-button of transmitter
board.

-   The LPUART is configured as follows:
    -   BaudRate = 9600 baud
    -   Word Length = 8 bits (8 data bits, no parity bit)
    -   One Stop Bit
    -   No parity
    -   Hardware flow control disabled (RTS and CTS signals)
    -   Reception and transmission are enabled in the time

-   The typical average power consumption of the system performing LPUART transfers is 7.2uA (Measured on transmitter
board).

-   NUCLEO-U575ZI-Q board LEDs are used to monitor the transfer status:
    -   LED1 (green led) is turned on at the end of application and no error is detected.
    -   LED2 (blue led) is turned on after configuring transfers and before pressing the user push-button on transmitter
    and receiver boards .
    -   LED3 (red led) is turned on when any error is occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

 3. This application (only transmitter board) can not be used in DEBUG mode due to the fact that the debug pins are
      configured in analog mode for power consumption and Cortex-M33 core is no longer clocked during low power mode so
      debugging features are disabled.

 4. When deleting project folder (EWARM, MDK-ARM or STM32CubeIDE) and regenerating this application with STM32CubeMx tool, 
      ensure to re-adjust linker ROM and RAM sections addresses.

### <b>Keywords</b>

Connectivity, UART/USART, LPBAM, Transmitter, Receiver, Low power, Stop2

### <b>Directory contents</b>

-   LPBAM/LPBAM_LPUART_TransmitReceive/Core/Src/main.c                           Main program file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Core/Src/system_stm32u5xx.c               STM32U5xx system clock configuration file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Core/Src/stm32u5xx_it.c                   Interrupt handlers file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Core/Src/stm32u5xx_hal_msp.c              HAL MSP module file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Core/Inc/stm32_lpbam_conf.h               LPBAM Configuration file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Core/Inc/main.h                           Main program header file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Core/Inc/stm32u5xx_nucleo_conf.h          BSP Configuration file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Core/Inc/stm32u5xx_hal_conf.h             HAL Configuration file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Core/Inc/stm32u5xx_it.h                   Interrupt handlers header file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Core/Inc/stm32_lpbam_conf.h               LPBAM Configuration file
-   LPBAM/LPBAM_LPUART_TransmitReceive/LPBAM/TxRx/lpbam_txrx.h                   LPBAM TxRx application header file
-   LPBAM/LPBAM_LPUART_TransmitReceive/LPBAM/TxRx/lpbam_txrx_config.c            LPBAM TxRx application configuration file
-   LPBAM/LPBAM_LPUART_TransmitReceive/LPBAM/TxRx/lpbam_txrx_sendbypack_config.c LPBAM TxRx application SendByPack scenario configuration file
-   LPBAM/LPBAM_LPUART_TransmitReceive/LPBAM/TxRx/lpbam_txrx_sendbypack_build.c  LPBAM TxRx application SendByPack scenario build file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q set-up
    -   Connect transmitter board pin PB.11 (pin 18 in CN12 connector) (LPUART Tx) and receiver board pin PB.10 (pin 25 in CN12 connector) (LPUART Rx)
    -   Connect transmitter board pin PB.10 (pin 25 in CN12 connector) (LPUART Rx) and receiver board pin PB.11 (pin 18 in CN12 connector) (LPUART Tx)
    -   Connect 1st board GND to 2nd Board GND

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
        - Uncomment "#define TRANSMITTER_BOARD" in the main.h file and load the project in transmitter board.
        - Comment "#define TRANSMITTER_BOARD" in the main.h file and load the project in receiver board.
    -   UnPlug then Plug STLINK connection to perform a power-on-reset for transmitter board.