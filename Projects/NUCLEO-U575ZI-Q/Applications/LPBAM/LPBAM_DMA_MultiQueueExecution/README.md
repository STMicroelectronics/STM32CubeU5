
## <b>LPBAM_DMA_MultiQueueExecution Application Description</b>
-   How to handle multi queue execution with DMA linked-list feature in low power mode through LPBAM utility.

-   Board: NUCLEO-U575ZI-Q (embeds a STM32U575xx device)

-   At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :
The CPU at 160 MHz.
The HCLK for AHB peripherals to run at 160 MHz.
The APB clock dividers APB1, APB2, APB3 peripherals to run at 160 MHz.

-   In a first step, the application calls ADV_LPBAM_GPIO_WritePinSequence_SetFullQ() to write LPGPIO pin 0 values.
After that, a linked-list queue "DMA_Slave_ToggleIO_0_Queue" is created and placed in the SRAM that will be executed by
the slave DMA channel in
Stop 2 power mode.
-   The same sequence is called to write LPGPIO pin 1 values and create a linked-list queue "DMA_Slave_ToggleIO_1_Queue".
Created queues are triggered by LPTIM3 PWM rising edge signal.

-   Then, the application calls ADV_LPBAM_DMA_Start_SetFullQ() to link created queues (DMA_Slave_ToggleIO_0_Queue and
DMA_Slave_ToggleIO_1_Queue) by the master DMA channel. After that, a linked-list queue is created and placed in the SRAM
that will be executed by the master DMA channel in Stop 2 power mode.
Created queue execution is triggered by LPTIM1 PWM rising edge signal.

-   Finally, the application calls HAL services to initialize and start the master DMA channel.

-   LPTIM1 PWM trigger signal period is equal to 1s.
-   LPTIM3 PWM trigger signal period is equal to 100ms.

-   This scenario is repeated in infinite loop.

-   To minimize the power consumption, after starting generating the PWM signals, the MCU enters in Stop 2 mode.

-   User push-button pin (PC.13) is configured as input with external interrupt (EXTI_Line13), falling edge. When User
push-button is pressed, wakeup event is generated and green led is turned on.

-   The LPBAM mechanism operating is not impacted when the system enters and exit STOP 2 mode.

-   This project contains two configuration :
    -   Debug configuration : uncomment DEBUG_CONFIGURATION flag in the main.h.
        - LPTIM1 and LPTIM3 output signals are mapped respectively on PB.2 and PB.0 pins.
    -   Power measurement configuration : comment DEBUG_CONFIGURATION flag in the main.h.
        - LPTIM1 and LPTIM3 output signals are not mapped.

-   The typical average power consumption of the system performing DMA master trasnfers with a 1s period and DMA slave
transfers with 100ms period is 4.2uA.

-   NUCLEO-U575ZI-Q board LEDs are used to monitor the transfer status:
    -   LED1 (green led) is turned on after wake up from STOP 2 mode and no error is detected.
    -   LED3 (red led) is turned on when any error is occurred.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

System, DMA transfer, DMA, GPIO, LPBAM, Signal, Low power, Stop2

### <b>Directory contents</b>

-   LPBAM/LPBAM_DMA_MultiQueueExecution/Src/main.c                  Main program
-   LPBAM/LPBAM_DMA_MultiQueueExecution/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
-   LPBAM/LPBAM_DMA_MultiQueueExecution/Src/stm32u5xx_it.c          Interrupt handlers
-   LPBAM/LPBAM_DMA_MultiQueueExecution/Src/stm32u5xx_hal_msp.c     HAL MSP module
-   LPBAM/LPBAM_DMA_MultiQueueExecution/Inc/stm32_lpbam_conf.h      LPBAM Configuration file
-   LPBAM/LPBAM_DMA_MultiQueueExecution/Inc/main.h                  Main program header file
-   LPBAM/LPBAM_DMA_MultiQueueExecution/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
-   LPBAM/LPBAM_DMA_MultiQueueExecution/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
-   LPBAM/LPBAM_DMA_MultiQueueExecution/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

-   This application runs on STM32U575xx devices without security enabled (TZEN=0).

-   This application has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)
    board and can be easily tailored to any other supported device
    and development board.

-   Connect PB.2 (Pin 22 in CN12 connector) to an oscilloscope to visualize the LPTIM1 channel 1 output waveform.
-   Connect PB.0 (Pin 34 in CN11 connector) to an oscilloscope to visualize the LPTIM3 channel 1 output waveform.
-   Connect PA.1 (Pin 30 in CN11 connector) to an oscilloscope to visualize the LPGPIO1 Pin 0 output waveform.
-   Connect PA.3 (Pin 37 in CN12 connector) to an oscilloscope to visualize the LPGPIO1 Pin 1 output waveform.

### <b>How to use it ?</b>

-   In order to make the program work, you must do the following :
    -   Open your preferred toolchain
    -   Rebuild all files and load your image into target memory
    -   Run the example
