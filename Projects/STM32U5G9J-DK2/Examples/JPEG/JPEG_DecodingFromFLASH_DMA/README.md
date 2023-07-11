---
pagetitle: Readme
lang: en
---
::: {.row}
::: {.col-sm-14 .col-lg-12}

## <b>JPEG_DecodingFromFLASH_DMA Example Description</b>


This example demonstrates how to decode a JPEG image stored in the internal FLASH 
using the JPEG HW decoder in DMA mode. The decoded image is stored in the internal flash.

The SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 MHz.

- The example performs the following actions :

  - Init the JPEG decoder.
  
  - Call the function "JPEG_Decode_DMA" to start decoding the jpg buffer with DMA (Non blocking method).
    
    The "JPEG_Decode_DMA" function is implemented in a separate module decode_dma.c.
    This module is a wrapper on top of the JPEG HAL driver.    
  - The User [Green LED]{style="color: green"} is On if the test ends properly.
  
  - The User [RED LED]{style="color: red"} is On in case of test errors.


**NOTE** 

The JPEG HW decoder outputs are organized in YCbCr format.

Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
  
### <b>Keywords</b>

Graphics, JPEG, Decoding, Flash memory, DMA, Hardware Decoding, YCbCr

### <b>Directory contents</b>

  - JPEG/JPEG_DecodingFromFLASH_DMA/Core/Inc/stm32u5xx_hal_conf.h&emsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HAL configuration file
  - JPEG/JPEG_DecodingFromFLASH_DMA/Core/Inc/stm32u5xx_it.h&emsp;&nbsp;&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Interrupt handlers header file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Inc/stm32u5g9j_discovery_conf.h&emsp;&nbsp;&nbsp;&nbsp;STM32U5G9J-DK2 board configuration file
  - JPEG/JPEG_DecodingFromFLASH_DMA/Core/Inc/main.h&emsp;&nbsp;&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; &nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Header for main.c module  
  - JPEG/JPEG_DecodingFromFLASH_DMA/Core/Inc/decode_dma.h&emsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Header for encode_dma.c module
  - JPEG/JPEG_DecodingFromFLASH_DMA/Core/Inc/image_320_240_jpg.h&emsp;&emsp;&emsp;&emsp;&nbsp; &nbsp;&nbsp;JPEG Image 320x240 to be decoded
  - JPEG/JPEG_DecodingFromFLASH_DMA/Core/Src/stm32u5xx_it.c&emsp;&emsp;&nbsp; &nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;Interrupt handlers
  - JPEG/JPEG_DecodingFromFLASH_DMA/Core/Src/main.c&emsp;&emsp;&emsp;&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Main program
  - JPEG/JPEG_DecodingFromFLASH_DMA/Core/Src/decode_dma.c&emsp;&emsp;&emsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;JPEG Decoding (from memory) with DMA module
  - JPEG/JPEG_DecodingFromFLASH_DMA/Core/Src/stm32u5xx_hal_msp.c&emsp;&ensp;&ensp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HAL MSP module
  - JPEG/JPEG_DecodingFromFLASH_DMA/Core/Src/system_stm32u5xx.c&emsp;&ensp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;stm32u5xx system source file  

### <b>Hardware and Software environment</b>

  - This example runs on STM32U5G9XX devices.

  - This example has been tested with STM32U5G9J-DK2 board and can be
    easily tailored to any other supported device and development board. 

  - The compiler optimizations level could be set to High/Balanced giving a good tradeoff between the code size and the performance (decoding time).

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


:::
:::


:::
:::

