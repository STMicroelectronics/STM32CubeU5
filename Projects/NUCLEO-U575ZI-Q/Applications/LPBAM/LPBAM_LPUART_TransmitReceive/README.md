## <b>LPBAM_LPUART_TransmitReceive Application Description</b>
-   How to handle LPUART transmission/reception between two boards with
DMA linked-list feature in low power mode through LPBAM utility.

-   Board: NUCLEO-U575ZI-Q (embeds a STM32U575xx device)
    -   Tx Pin: PB11
    -   Rx Pin: PB10




-   The user presses the User push-button on board 1. Then the system enter stop2 mode and the LPTIM start generating a
PWM signal to trig the LPUART.
AT each rising edge of the PWM signal, the LPUART send a packet of 8 bytes to the board 2.
After the end of transmission of the first buffer, the LPUART change the packet size to 4 and repeat
the process of transmission.
when the LPUART of board 2 receive all data, it will send it to the board 1.
The DMA will generate an interrupt to wake up the system when the board 1 receive data from board 2.
Finally, board 1 and 2 compare the received messages to that sent.

-   WARNING: as both boards do not behave the same way, "TRANSMITTER_BOARD" compilation switch is defined in /Src/main.h and
must be enabled at compilation time before loading the executable in the board that first transmits then receives.
The receiving then transmitting board needs to be loaded with an executable software obtained with TRANSMITTER_BOARD
disabled.

-   NUCLEO-U575ZI-Q board LEDs are used to monitor the transfer status:
    -   While board is waiting for the user to press the User push-button, its LED2 is blinking rapidly (100 ms period).
    -   When the test passes, LED1 on both boards is turned on, otherwise the test has failed.
    -   Once the transmission/reception processes are completed LED1 is turned on.
    -   If there is an initialization or transfer error, LED3 is turned on.

-   The LPUART is configured as follows:
    -   BaudRate = 9600 baud
    -   Word Length = 8 bits (8 data bits, no parity bit)
    -   One Stop Bit
    -   No parity
    -   Hardware flow control disabled (RTS and CTS signals)
    -   Reception and transmission are enabled in the time


-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.


#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Connectivity, UART/USART, LPBAM, Transmitter, Receiver, Low power, Stop2


### <b>Directory contents</b>

-   LPBAM/LPBAM_LPUART_TransmitReceive/Src/main.c                  Main program
-   LPBAM/LPBAM_LPUART_TransmitReceive/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Src/stm32u5xx_it.c          Interrupt handlers
-   LPBAM/LPBAM_LPUART_TransmitReceive/Src/stm32u5xx_hal_msp.c     HAL MSP module
-   LPBAM/LPBAM_LPUART_TransmitReceive/Inc/stm32_lpbam_conf.h      LPBAM Configuration file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Inc/main.h                  Main program header file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Inc/stm32u5xx_it.h          Interrupt handlers header file
-   LPBAM/LPBAM_LPUART_TransmitReceive/Inc/stm32_lpbam_conf.h      LPBAM Configuration file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   NUCLEO-U575ZI-Q set-up
    -   Connect a wire between 1st board PB11 pin (LPUART Tx) and 2nd board PB10 pin (LPUART Rx)
    -   Connect a wire between 1st board PB10 pin (LPUART Rx) and 2nd board PB11 pin (LPUART Tx)
    -   Connect 1st board GND to 2nd Board GND

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
        - Uncomment "#define TRANSMITTER_BOARD" and load the project in Board 1
        - Comment "#define TRANSMITTER_BOARD" and load the project in Board 2
    -   Run the example

