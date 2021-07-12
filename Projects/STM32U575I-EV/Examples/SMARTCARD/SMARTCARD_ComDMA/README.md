## <b>SMARTCARD_ComDMA Example Description</b>

This example aims to show how to communicate with a smartcard using DMA mode.


    - SMARTCARD_IO    : PC4      : IO of data
    - SMARTCARD_CLK   : PB12     : IO of clock
    - SMARTCARD_OFF   : MFX_IO6  : IO to detect the presence of the smartcard on the eval board
    - SMARTCARD_RST   : MFX_IO7  : IO to manage the reset high and low of the smartcard
    - SMARTCARD_CMDVCC: MFX_IO9  : IO to enable and disable the power of the smartcard
    - SMARTCARD_3/5V  : MFX_IO10 : IO to manage the source of power ( 5V or 3v)



- This project is targeted to run on STM32U575xx devices on STM32U575I-EV board from   STMicroelectronics.

- The project configures the maximum system clock frequency at 160Mhz.

- After initialisation of the IO_Expander and configuration of the IOs showed above the LED5     still toggling waiting the insertion of the smartcard on the eval board.

- When the smartcard is detected, LED5 is turned Off and the process of initialization is    started:

  - initialization of the smartcard interface:

      * instance   = USART3
      * BaudRate   = 10752
      * WordLength = SMARTCARD_WORDLENGTH_9B
      * Parity     = SMARTCARD_PARITY_EVEN
      * Mode       = SMARTCARD_MODE_TX_RX

  - Set the smartcard reset pin to HIGH
  - Select 5V to power the smartcard
  - Enable the power to the smartcard
  - Set the smartcard reset pin to LOW

- Wait an answer from the smartcard after reset low process
- Set the smartcard reset pin to HIGH
- Wait an answer from the smartcard after reset High process
- Load data exchanged in the table SmartCard_ATR_Table

- Configure the header of the command in the buffer "SmartCard_Command"
  - the command header is a sequence of 5 bytes:
      - byte0: CLA is an instruction class
      - byte1: INS is an instruction code
      - byte2: P1 is a reference completing the instruction code: ex a start address
      - byte3: P2 is a reference completing the instruction code: ex an end address
      - byte4: codes the number n of data bytes

- Send a command from the stm32 to the smartcard to read 2 bytes from the selected file stored   in the smartcard

- Receive one byte containing the instruction code (INS) from the smartcard which is an       acknowledge byte

- Receive two bytes stored in the "SmartCard_Answer" which are the result of the execution of    the command

- DeInitialize the smartcard

- If LED5 (led green) is ON, it indicates that the example finish with success

- If LED6 (led RED) is ON, it indicates that the system generated an error.


#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Connectivity, UART/USART, smartcard, DMA, Transmitter, Receiver, synchronous mode

### <b>Directory contents</b>

  - SMARTCARD/SMARTCARD_ComDMA/Src/main.c                  Main program
  - SMARTCARD/SMARTCARD_ComDMA/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - SMARTCARD/SMARTCARD_ComDMA/Src/stm32u5xx_it.c          Interrupt handlers
  - SMARTCARD/SMARTCARD_ComDMA/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - SMARTCARD/SMARTCARD_ComDMA/Inc/main.h                  Main program header file
  - SMARTCARD/SMARTCARD_ComDMA/Inc/stm32u575i_eval_conf.h  BSP Configuration file
  - SMARTCARD/SMARTCARD_ComDMA/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - SMARTCARD/SMARTCARD_ComDMA/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics STM32U575I-EV (MB1550A)
    board and can be easily tailored to any other supported device
    and development board.

  - STM32U575I-EV Set-up

    - Insert an STM32 GSM File system smartcard

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
 - Insert an STM32 GSM File system smartcard

