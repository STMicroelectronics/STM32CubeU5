---
pagetitle: Readme
lang: en
---
::: {.row}
::: {.col-sm-14 .col-lg-12}

## <b>JPEG_EncodingFromFLASH_DMA Example Description</b>

This example demonstrates how to encode an RGB image stored in the internal FLASH 
using the JPEG HW encoder in DMA mode and save it in the internal FLASH.

The SystemClock_Config() function is used to configure the system clock (SYSCLK) to run at 160 MHz.

- The example performs the following actions :

  - Init the JPEG encoder
  - Call the function "JPEG_Encode_DMA" to start encoding the JPG file with DMA (Non blocking method).
    The RGB input image name is automatically selected according to the selected RGB format as follow :
    - Image_RGB888 for RGB888 (#define JPEG_RGB_FORMAT      JPEG_RGB888)
    - Image_RGB565 for RGB565 (#define JPEG_RGB_FORMAT      JPEG_RGB565)
    - Image_ARGB8888 for ARGB8888 (#define JPEG_RGB_FORMAT      JPEG_ARGB8888)

    The Function "JPEG_Encode_DMA" is implemented in a separate module encode_dma.c.    
    This module is a wrapper on top of the JPEG HAL driver.
  - Save encoded data in the internal FLASH memory
  - The User [Green LED]{style="color: green"} is On if the test ends properly.
  - The User [Red LED]{style="color: red"} is On in case of test errors.

**NOTE** 

The JPEG HW encoder inputs are organized in YCbCr blocks.

Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

### <b>Keywords</b>

Graphics, JPEG, Encoding, Flash memory, DMA, ARGB8888, Hardware Encoding, YCbCr, RGB

### <b>Directory contents</b>

The jpeg utilities functions implemented in "jpeg_utils.c" file are used to convert the input RGB pixels to YCbCr blocks.

The JPEG utilities files allowing to convert RGB pixels to YCbCr blocks are available under /Utilities/JPEG

  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Inc/stm32u5xx_hal_conf.h&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HAL configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Inc/stm32u5xx_it.h&nbsp;&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Interrupt handlers header file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Inc/main.h&nbsp;&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; &nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;Header for main.c module  
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Inc/encode_dma.h&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Header for encode_dma.c module
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Inc/jpeg_utils_conf.h&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;JPEG utilities module configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Inc/image_320_240_rgb.h&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Specification of RGB image to load in FLASH
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Inc/stm32u5g9j_discovery_conf.h&nbsp;&nbsp;&nbsp;STM32U5G9J-DK2 board configuration file
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Src/stm32u5xx_it.c&emsp;&nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;Interrupt handlers
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Src/main.c&emsp;&emsp;&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Main program
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Src/encode_dma.c&emsp;&emsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;JPEG Encoding with DMA module   
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Src/stm32u5xx_hal_msp.c&ensp;&ensp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;HAL MSP module 
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Src/system_stm32u5xx.c&ensp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;stm32u5xx system source file  
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Src/Image_argb_8888.c&ensp;&ensp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Example of ARGB888 image
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Src/Image_rgb_565.c&emsp;&emsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Example of RGB565 image
  - JPEG/JPEG_EncodingFromFLASH_DMA/Core/Src/Image_rgb_888.c&emsp;&emsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Example of RGB888 image

### <b>Hardware and Software environment</b>

  - This example runs on STM32U5G9XX devices.

  - This example has been tested with STM32U5G9J-DK2 board and can be
    easily tailored to any other supported device and development board. 

  - The compiler optimization level could be set to High/Balanced giving a good tradeoff between the code size and the performance (encoding time).

### <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


:::
:::


:::
:::


:::
:::

