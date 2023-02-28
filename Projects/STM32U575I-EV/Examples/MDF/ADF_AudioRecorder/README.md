## <b>ADF_AudioRecorder Example Description</b>

- This example shows how to use the MDF HAL API (ADF instance) to perform mono audio recording. 
For this audio record we will use one MP23DB01 digital microphones mounted on the MB1550 module.

- At the beginning of the main program the HAL_Init() function is called to reset all the peripherals initialize the Flash interface and the systick.
The SystemClock_Config() function is used to configure the system clock for STM32U575xx Devices :

     - The CPU at 160MHz
     - The HCLK, AHB and APB1/APB2/APB3 peripherals at 160MHz.

- MP23DB01 runs with a clock frequency from 1MHz to 3,25MHz.
This clock frequency should be delivered by ADF. So ADF1_CCK0 (PB3) will be configured to enable output clock generation, to choose audio clock as an output clock source,
and output clock divider will be set to generate an output clock in MP23DB01 frequency clock range.
Audio clock will be configured to 11.291MHz, so ADF output clock divider will be set to 4.

- The digital audio outputs from the microphones are coded in PDM (Pulse Density Modulation) and interlaced to use the same data wire connected to PB4.

- PB4 is configured as ADF1_SDI0 (ADF serial interface 0 as external serial data input).
We will use ADF serial interface 0 to get channel. ADF bitstream will be configured to take inputs from external serial input 0 on raising edge.
ADF serial interface 0 will also be configured to operate in SPI mode with rising edge and with internal clock.

- ADF filter 0 will be used to perform conversions.
We will use a SINC4 filter with oversampling at 64 and integrator oversampling at 1.
Oversampling is set to 64 in order to have an audio sample frequency at 44.1KHz (11.291MHz/(4*64)).
Regular conversions in fast mode will be performed in DMA mode on ADF filter 0 to fill a buffer containing left audio samples at 44.1KHz.
ADF filter 0 will be used to perform right channels conversions.

- Playback of the recorded data will be performed on headset using HAL_SAI and CS42L51 audio codec.
A circular playback buffer will be filled as soon as recorded data are available.
When half left and right buffers will be filled, we put first parts of left and right channels data on the first half of playback buffer.
When left and right buffers will be full filled, we put second parts of left and right channels data on the second half of playback buffer.

MB1550 board's LEDs are used to monitor the example status:

  - RED LED toggle with a period of one second when there is an error occurred.

<b>Execution:</b>

The program configures system clocks and GPIO used by ADF.
Then, program performs the following sequence:

1. Fine tune the clocks for audio
2. Initialize the audio codec (CS42L51) 
3. Initialize SAI for audio playback.
4. Initialize ADF.
5. Start ADF conversion in DMA mode (linked list circular mode).
6. Store values on the record buffer.
7. Transmit data from play buffer to the SAP via DMA (linked list circular mode).

### <b>Keywords</b>

Audio, SAI, MDF, ADF, DMA, Buffer update, play, headphone, audio protocol

### <b>Directory contents</b>

  - MDF/ADF_AudioRecorder/Src/main.c                  Main program
  - MDF/ADF_AudioRecorder/Src/linked_list.c           LinkedList configuration file
  - MDF/ADF_AudioRecorder/Src/system_stm32u5xx.c      STM32U5xx system source file
  - MDF/ADF_AudioRecorder/Src/stm32u5xx_it.c          Interrupt handlers
  - MDF/MDF_AudioSoundDetector/Src/stm32u5xx_hal_msp.c HAL MSP module
  - MDF/ADF_AudioRecorder/Inc/main.h                  Main program header file
  - MDF/ADF_AudioRecorder/Inc/linked_list.h           LinkedList configuration header file
  - MDF/ADF_AudioRecorder/Inc/stm32u575i_eval_conf.h  STM32U575I-EVAL board configuration file
  - MDF/ADF_AudioRecorder/Inc/stm32u5xx_hal_conf.h    HAL configuration file
  - MDF/ADF_AudioRecorder/Inc/stm32u5xx_it.h          Interrupt handlers header file


### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics STM32U575I-EV (MB1550)
    board and can be easily tailored to any other supported device
    and development board.

  - Plug headset on CN27 connector (Headset).

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

