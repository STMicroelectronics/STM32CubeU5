## <b>LPTIM_PWM_LSE Example Description</b>

This example describes how to configure and use LPTIM to generate a PWM in low power mode
using the LSE as a counter clock, through the HAL LPTIM API.

This project is targeted to run on STM32U575xx devices on NUCLEO-U575ZI-Q board from STMicroelectronics.

At the beginning of the main program the HAL_Init() function is called to reset
all the peripherals, initialize the Flash interface and the systick,the function CACHE_Enable() is called to enhance performance.

The SystemClock_Config() function is used to configure the maximum system clock frequency at 160Mhz.

In this example, the LPTIM instance used is LPTIM1 and the low power mode is Stop mode.

The counter clock is LSE (32.768 KHz), Autoreload equal to 99 so the output
frequency (FrequencyOutput) will be equal to 327.680.

  FrequencyOutput = Counter Clock Frequency / (Autoreload + 1)  

                  = 32768 / 100  
                  = 327.680 Hz

Pulse value equal to 49 and the duty cycle (DutyCycle) is computed as follow:

  DutyCycle = 1 - ((PulseValue + 1)/ (Autoreload + 1))  
  DutyCycle = 50%

To minimize the power consumption, after starting generating the PWM signal,the MCU enters in Stop mode.  
Note that GPIOs are configured in Low Speed to enhance the consumption.

User push-button pin (PC.13)is configured as input with external interrupt (EXTI_Line13),falling edge.  
When User push-button is pressed, wakeup event is generated and PWM signal generation is stopped and LED1(Green LED) is ON.

If LED3(Red LED) is ON , an error is occurred.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)  
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from  
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)  
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.  
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond  
    to have correct HAL operation.

 3. The debugger should be disconnected.

### <b>Keywords</b>

Motor control, LPTIM, stop mode, PWM signal, external interrupt, wakeup event.

### <b>Directory contents</b>

  - LPTIM/LPTIM_PWM_LSE/Src/main.c                  Main program
  - LPTIM/LPTIM_PWM_LSE/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - LPTIM/LPTIM_PWM_LSE/Src/stm32u5xx_it.c          Interrupt handlers
  - LPTIM/LPTIM_PWM_LSE/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - LPTIM/LPTIM_PWM_LSE/Inc/main.h                  Main program header file
  - LPTIM/LPTIM_PWM_LSE/Inc/stm32u5xx_nucleo_conf.h BSP Configuration file
  - LPTIM/LPTIM_PWM_LSE/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - LPTIM/LPTIM_PWM_LSE/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This Example runs on STM32U575xx devices without security enabled (TZEN=0).

  - This Example has been tested with STMicroelectronics NUCLEO-U575ZI-Q (MB1549)  
    board and can be easily tailored to any other supported device  
    and development board.

  - Connect PB.2 (Pin 22 in CN12 connector) to an oscilloscope to monitor the LPTIM1 output waveform.

  - LSE oscillator must be mounted on the board, else this example does not work.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain  
 - Rebuild all files and load your image into target memory  
 - Run the example

