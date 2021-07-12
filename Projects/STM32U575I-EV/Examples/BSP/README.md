
## <b>BSP Example Description</b>

How to use the different BSP drivers of the board.

This project is targeted to run on STM32U575xx devices on STM32U575I-EV board from STMicroelectronics.

The project configures the maximum system clock frequency at 160Mhz.

This example shows how to use the different functionalities of components available on the board by switching between all tests using User button.

Red LED toggles every 500ms whenever any error is detected.

 ** Press the User push-button to start first Test.  
Press User push-button to start the following test:

#### <b>LED</b>

- This example shows how to switch on, switch off and toggle all leds.

#### <b>JOYSTICK</b>

- Use the joystick to move a pointer inside a rectangle (up/down/right/left) and change the pointer color(select).

#### <b>LCD</b>
- This example shows how to use the different LCD features to display string with different fonts, to display different shapes and to draw a bitmap.

#### <b>TOUCHSCREEN</b>
- This example shows how to use the touchscreen.

#### <b>CAMERA</b>
- This example shows how to use BSP drivers to test the camera.

#### <b>OSPI</b>
- Make sure that the camera module is not mounted before running this demo.
- This example shows how to erase, write and read data available on the Octal Flash memory available on the board 
(STR and DTR in Normal mode and MemoryMapped).
- NOTE: Camera module should be unplugged for this example (Shared pins).

#### <b>SD</b>
- This example shows how to erase, write and read the SD card and also how to detect the presence of the card.

#### <b>SRAM</b>
- This example shows how to erase, write and read data available on the external SRAM available on the board (Direct mode and DMA mode).

#### <b>MOTION SENSOR</b>
- This example shows how to use the accelerometer and gyroscope of the motion sensor.

#### <b>AUDIO RECORD</b>
- You need to plug a headphone to the audio jack (CN27).
- This example will record sound and loopback it. User selects the input he prefers (digital mic U48 or headphones mic via the CN27 connector).

#### <b>AUDIO PLAY</b>
- This example shows how to play/pause/resume or change the volume (min 50% / max 100 %) of an audio playback using Key or Joystick button.

      Plug a headphone to the CN27 connector to hear the sound.
      Default volume is 80%.
      A press of JOY_SEL button will pause the audio file play (LED green and yellow both ON).
      Another press on JOY_SEL button will resumes audio file play (only LED green ON)
      @Note: Copy file 'audio.bin' (available in BSP\AudioFile folder) directly in the flash 
      at @0x08040000 using STM32CubeProgrammer.

#### <b>EEPROM</b>
- This example shows how to read and write data usint the EEPROM I2C M24128(U8).
 
#### Notes
 1. Before running the OSPI demo make sure that the camera module is not mounted on the board.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

BSP, Led, Button, Camera, SRAM, LCD, TS, Touchscreen, Audio, Motion sensor, OSPI 

### <b>Directory contents</b>

  - BSP/Src/audio_record.c         AUDIO RECORD features
  - BSP/Src/camera.c               CAMERA features
  - BSP/Src/eeprom.c               EEPROM feature
  - BSP/Src/joystick.c             joystick feature
  - BSP/Src/lcd.c                  LCD drawing features
  - BSP/Src/led.c                  Led display features
  - BSP/Src/main.c                 Main program
  - BSP/Src/motion_sensor.c        Motion sensor features
  - BSP/Src/ospi.c                 OSPI features
  - BSP/Src/sd.c                   SD features
  - BSP/Src/sram.c                 SRAM features
  - BSP/Src/stm32u5xx_hal_msp.c    HAL MSP module
  - BSP/Src/system_stm32u5xx.c     STM32U5xx system clock configuration file
  - BSP/Src/stm32u5xx_it.c         Interrupt handlers
  - BSP/Src/touchscreen.c          TS features
  - BSP/Inc/main.h                 Main program header file
  - BSP/Inc/stm32u5xx_hal_conf.h   HAL Configuration file
  - BSP/Inc/stm32u575i_eval_conf.h Board Configuration file
  - BSP/Inc/stm32u5xx_it.h         Interrupt handlers header file
  - BSP/Inc/stlogo.h               Image used for BSP LCD example
  - BSP/Inc/sx8651_conf.h          TS configuration file
  - BSP/Inc/mx25lm51245g_conf.h    OSPI NOR memory configuration file
  - BSP/AudioFile/audio.bin        Audio wave extract.
        
### <b>Hardware and Software environment</b>  

  - This example runs on STM32U575xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics STM32U575I-EV (MB1550)
    board and can be easily tailored to any other supported device
    and development board.
  
### <b>How to use it ?</b>

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

