## <b>RTC_Calendar Example Description</b>

- Configuration of the calendar using the RTC HAL API.

- At the beginning of the main program the HAL_Init() function is called to reset all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 MHz.

- The RTC peripheral configuration is ensured by the HAL_RTC_Init() function.
This later is calling the HAL_RTC_MspInit()function which core is implementing the configuration of the needed RTC resources according to the used hardware (CLOCK, 
PWR, RTC clock source and BackUp). You may update this function to change RTC configuration.

- LSI oscillator clock is used as RTC clock source. 
    The user can use also LSE as RTC clock source.
    - The user uncomment the adequate line on the main.h file.
	
      @code
	  
        #define RTC_CLOCK_SOURCE_LSI  
		
        /* #define RTC_CLOCK_SOURCE_LSE */
		
      @endcode
	  
    - Open the ioc file with STM32CubeMX and select :
      LSE as "Crystal/Ceramic Resonator" in RCC configuration.
      LSE as RTC clock source in Clock configuration.
    - Generate code
    LSI oscillator clock is delivered by a 32 kHz.
    LSE (when available on board) is delivered by a 32.768 kHz crystal.

- HAL_RTC_SetTime()and HAL_RTC_SetDate() functions are then called to initialize the time and the date.

- A key value is written in backup data register 1 to indicate if the RTC is already configured.  
The RTC is in the backup (BKP) domain, still powered by VBAT when VDD is switched off,
so the RTC configuration is not lost if a battery is connected to the VBAT pin. 
The program behaves as follows:

1. After startup the program checks the backup data register 0 value:

    - BKP_DR0 value not correct: (RTC_BKP_DR0 value is not correct or has not yet
      been programmed when the program is executed for the first time) the RTC is
      configured and internal time stamp is enabled.
    
    - BKP_DR0 value correct: this means that the RTC is configured and the time
      and date are displayed on Debugger.

2. When a reset (except power on reset) occurs the BKP domain is not reset and the RTC configuration is not lost. **LED6 is ON**.
   
3. When power on reset occurs:

    - If a battery is connected to the VBAT pin: the BKP domain is not reset and
      the RTC configuration is not lost. **LED6 is ON**.

    - If no battery is connected to the VBAT pin: the BKP domain is reset and the
      RTC configuration is lost.

- **LED5** is turned ON when the RTC configuration is done correctly.


- The current time and date are updated and displayed on the debugger in aShowTime and aShowDate variables.

#### <b>Notes</b>
 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
 2. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

System, RTC, Calendar, Backup Domain, Reset

### <b>Directory contents</b>

  - RTC/RTC_Calendar/Inc/stm32u575i_eval_conf.h  BSP configuration file
  - RTC/RTC_Calendar/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - RTC/RTC_Calendar/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - RTC/RTC_Calendar/Inc/main.h                  Header for main.c module  
  - RTC/RTC_Calendar/Src/stm32u5xx_it.c          Interrupt handlers
  - RTC/RTC_Calendar/Src/main.c                  Main program
  - RTC/RTC_Calendar/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - RTC/RTC_Calendar/Src/system_stm32u5xx.c      STM32U5xx system source file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices.
  - This example has been tested with STMicroelectronics STM32U575I-EV
    board and can be easily tailored to any other supported device and 
    development board. 
  - STM32U575I-EV Set-up
  - Make sure that J19 is in position 1-2 to connect 3V battery to VBAT pin.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

