## <b>BSP Example Description</b>

How to use the different BSP drivers of the board.

This project is targeted to run on STM32U585xx devices on B_U585I_IOT02A board from STMicroelectronics.

The project configures the maximum system clock frequency at 160Mhz.

This example shows how to use the different functionalities of components available on the board.

Red LED toggles every 500ms whenever any error is detected.

It uses the Terminal I/O to display information : 

 - When resorting to EWARM IAR IDE:
   Command Code is displayed on debugger as follows: View --> Terminal I/O

 - When resorting to MDK-ARM KEIL IDE:
   Command Code is displayed on debugger as follows: View --> Serial Windows --> Debug (printf) Viewer
 
 - When resorting to STM32CubeIDE:
   In Debug configuration window\ Startup, in addition to "monitor reset halt" add the command "monitor arm semihosting enable"
   Command Code is displayed on debugger as follows: Window--> Show View--> Console.
   
At the beginning, the example waits until the User Button is pressed. Once 
this is done, it goes to run the following tests:

 ** Button ** 
Press User push-button to start the following test:

 ** LED **
This example shows how to switch on, switch off and toggle all leds.

 ** CAMERA **
This example shows how to use BSP drivers to test the camera.

 ** OSPI NOR **
This example shows how to erase, write and read data available on the Octal Flash memory
available on the board. (STR and DTR in Normal mode and MemoryMapped)

 ** OSPI RAM **
This example shows how to write and read data available on the Octal Sram memory
available on the board. (Normal mode and MemoryMapped)

 ** MOTION SENSOR **
This example shows how to use the accelerometer and gyroscope of the motion sensor.

 ** ENVIRONMENTAL SENSOR **
This example shows how to use the temperature, humidity and pressure of the environmental sensor.

 ** EEPROM **
This example shows how to read and write data usint the EEPROM I2C M24256(U22)

#### <b>Notes</b>

 1. Before running the OSPI demo make sure that the camera module is not mounted on the board.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Directory contents</b>

  - BSP/Src/camera.c               CAMERA features
  - BSP/Src/eeprom.c               EEPROM feature
  - BSP/Src/led.c                  Led display features
  - BSP/Src/main.c                 Main program
  - BSP/Src/button.c               Button feature
  - BSP/Src/motion_sensor.c        Motion sensor features
  - BSP/Src/env_sensor.c           Env sensor features
  - BSP/Src/ospi_nor.c             OSPI NOR features
  - BSP/Src/ospi_ram.c             OSPI RAM features
  - BSP/Src/stm32u5xx_hal_msp.c    HAL MSP module
  - BSP/Src/system_stm32u5xx.c     STM32U5xx system clock configuration file
  - BSP/Src/stm32u5xx_it.c         Interrupt handlers
  - BSP/Inc/main.h                 Main program header file
  - BSP/Inc/stm32u5xx_hal_conf.h   HAL Configuration file
  - BSP/Inc/b_u585i_iot02_conf.h   Board Configuration file
  - BSP/Inc/stm32u5xx_it.h         Interrupt handlers header file
  - BSP/Inc/mx25lm51245g_conf.h    OSPI NOR memory configuration file
  - BSP/Inc/aps6408_conf.h         OSPI SRAM memory configuration file

### <b>Keywords</b>

BSP, Led, Button, Camera, Env sensor, Audio, Motion sensor, OSPI 

### <b>Hardware and Software environment</b>  

  - This example runs on STM32U585xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics B_U585I_IOT02A (MB1551)
    board and can be easily tailored to any other supported device
    and development board.
  
### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

