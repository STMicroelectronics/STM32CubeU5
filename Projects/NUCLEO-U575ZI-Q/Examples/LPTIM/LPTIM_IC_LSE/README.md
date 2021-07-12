## <b>LPTIM_IC_LSE Example Description</b>

This example shows how to use the LPTIM peripheral to measure the frequency of
an external signal in low power mode using the LSE as a counter clock, through the HAL LPTIM API.

This project is targeted to run on STM32U575xx devices on NUCLEO-U575ZI-Q board from STMicroelectronics.

At the beginning of the main program the HAL_Init() function is called to reset  
all the peripherals, initialize the Flash interface and the systick,the function CACHE_Enable() is called to enhance performance.

The SystemClock_Config() function is used to configure the maximum system clock frequency at 160Mhz.

In this example, the LPTIM instance used is LPTIM1 and the low power mode is Stop mode.  
LPTIM1 is configured in Input Capture Mode: the external signal is connected to LPTIM1 Channel1 used as input pin.

To minimize power consumption, after starting input capture mode, MCU enters STOP2 mode and DMA  
transfers data from CCR1 register to SRD_DmaCapturedValue buffer .  
When the transfer is completed the DMA generates a interruption to wakeup the MCU .

The counter clock is LSE (32.768 KHz).  
The input signal frequency(InputSignalFreq) is 100HZ.  

The Expected_InputSignalFreq = LSE_VALUE / InputSignalFreq .

The Measured_InputSignalFreq (the difference between the 2 captured values) = SRD_DmaCapturedValue[1] - SRD_DmaCapturedValue[0].

If LED1(Green LED) is ON , it indicates that the frequency measurement are correct.  
If LED3(Red LED) is ON , an error is occurred.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)  
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from  
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)  
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.  
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Metering, LPTIM, stop mode, IC signal, wakeup event.

### <b>Directory contents</b>

  - LPTIM/LPTIM_IC_LSE/Src/main.c                  Main program
  - LPTIM/LPTIM_IC_LSE/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - LPTIM/LPTIM_IC_LSE/Src/stm32u5xx_it.c          Interrupt handlers
  - LPTIM/LPTIM_IC_LSE/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - LPTIM/LPTIM_IC_LSE/Inc/main.h                  Main program header file
  - LPTIM/LPTIM_IC_LSE/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
  - LPTIM/LPTIM_IC_LSE/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - LPTIM/LPTIM_IC_LSE/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This Example runs on STM32U575xx devices without security enabled (TZEN=0).

  - This Example has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)  
    board and can be easily tailored to any other supported device  
    and development board.

  - Connect an external signal to LPTIM1 Channel1(PB.2 in CN12 connector).

  - LSE oscillator must be mounted on the board, else this example does not work.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain  
 - Rebuild all files and load your image into target memory  
 - Run the example

