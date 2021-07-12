## <b>ADF_AudioSoundDetector Example Description</b>

This example shows how to use the MDF HAL API (ADF instance) to use audio sound activity detection.
For this audio record we will use one SPH0641LM4H digital microphones mounted on the MB1551 module.

This project is targeted to run on STM32U585xx devices on B-U585I-IOT02A boards from STMicroelectronics.

The project configures the maximum system clock frequency at 160Mhz.

SPH0641LM4H runs with a clock frequency from 1MHz to 4,8MHz.
This clock frequency should be delivered by ADF. So ADF_CCK0 (PE9) will be
configured to enable output clock generation, to choose audio clock as an output clock source,
and output clock divider will be set to generate an output clock in SPH0641LM4H frequency clock range.
We will use ADF Clock to be 12MHz and ADF output clock divider will be set to 4 (output clock is 2MHz).

The digital audio outputs from the microphones are coded in PDM (Pulse Density Modulation) and interlaced to use the same data wire connected to PE10.

PE4 is configured as ADF_SDIN0 (MDF serial interface 3 as external serial data input).
We will use MDF serial interface 0 to get channel. MDF bitstream will be configured to take inputs from external serial input 0 on raising edge.
ADF serial interface 0 will also be configured to operate in SPI mode with rising edge and with internal clock.

MDF filter 0 will be used to perform conversions.
We will use a SINC4 filter with oversampling at 64 and integrator oversampling at 1.
Oversampling is set to 64 in order to have an audio sample frequency at 44.1KHz (11.291MHz/(4*64)).
Regular conversions in fast mode will be performed in DMA mode on ADF filter 0.
ADF filter 0 will be used to perform right channels conversions.

Sound Activity Detector (SAD) will detect sound when its level is bigger than the defined threshold.
The ADF in this example is configured as a sound activity detector.
The SAD triggers when the sound level (SDLVL) is bigger than the defined threshold.
Sound activity detection interrupt will be rising and LEDs will be toggeled.
User could configure the system to enter in STOP mode and Sound activity detection interrupt will allow to wakeup from STOP mode.

MB1551 board's LEDs are used to monitor the example status:

  - GREEN LED will toggle as soon as an SAD detection occurs.
  - RED LED is turned on when any error is occurred.


Execution:

The program configures system clocks and GPIO used by ADF.
Then, program performs the following sequence:

1. Fine tune the clocks for audio
2. Initialize ADF.
3. Start ADF conversion in DMA mode (linked list circular mode) with SAD enabled. Note, that data are not transferred to audio codec (can't hear sound).
4. Enter Stop Mode.
5. Wait for SAD interrupt for system wakeup.

### <b>Keywords</b>

Audio, MDF, DMA, SAD, Stop2

### <b>Directory contents</b>

  - MDF/MDF_AudioSoundDetector/Src/main.c                  Main program
  - MDF/MDF_AudioSoundDetector/Src/system_stm32u5xx.c      STM32U5xx system source file
  - MDF/MDF_AudioSoundDetector/Src/stm32u5xx_it.c          Interrupt handlers
  - MDF/MDF_AudioSoundDetector/Inc/main.h                  Main program header file
  - MDF/MDF_AudioSoundDetector/Inc/stm32u575i_eval_conf.h  STM32U575I-EVAL board configuration file
  - MDF/MDF_AudioSoundDetector/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - MDF/MDF_AudioSoundDetector/Inc/stm32u5xx_it.h          Interrupt handlers header file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U585xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics B-U585I-IOT02A (MB1551)
    board and can be easily tailored to any other supported device
    and development board.


### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

