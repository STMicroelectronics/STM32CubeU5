## <b>SAI_AudioPlay Example Description</b>

How to  play an audio file via SAI using DMA circular mode.

      Plug a headphone to ear the sound  /!\ Take care of yours ears.
      Default volume is 80%.
      The audio file is played in loop
      @Note: Copy file 'audio.bin' (available in AudioFile directly) in the flash 
      at @0x08080000 using STM32CubeProgrammer.

This project is targeted to run on STM32U575xx devices on STM32U575I-EV board from STMicroelectronics.
The project configures the maximum system clock frequency at 160Mhz.

STM32U575I-EV board's LED can be used to monitor the project operation status:

     - LED_GREEN  toggle when no error detected.
     - LED_RED  is ON when any project error was occurred.


#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Audio, SAI, DMA, Buffer update, play, headphone, audio protocol

### <b>Directory contents</b>

  - SAI/SAI_AudioPlay/Src/linked_list.c           Linked list source file
  - SAI/SAI_AudioPlay/Src/main.c                  Main program source file
  - SAI/SAI_AudioPlay/Src/stm32u5xx_it.c          Interrupt handlers source file
  - SAI/SAI_AudioPlay/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - SAI/SAI_AudioPlay/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - SAI/SAI_AudioPlay/Src/linked_list.h           Linked list header file
  - SAI/SAI_AudioPlay/Inc/main.h                  Main program header file
  - SAI/SAI_AudioPlay/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - SAI/SAI_AudioPlay/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - SAI/SAI_AudioPlay/Inc/stm32u575i_eval_conf.h  STM32U575I-EVAL board configuration file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics STM32U575I-EV (MB1550)
    board and can be easily tailored to any other supported device
    and development board.

  - Plug headset on CN26 connector (Headset).

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

