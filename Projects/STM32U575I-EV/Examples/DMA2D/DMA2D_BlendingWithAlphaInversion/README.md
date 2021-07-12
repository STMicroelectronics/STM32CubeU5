## <b>DMA2D_BlendingWithAlphaInversion Example Description</b>

- This example provides a description of how to configure DMA2D peripheral in 
Memory_to_Memory with blending transfer and alpha inversion mode based on the STM32Cube HAL API
when security is not enabled (TZEN=0).
- This project is targeted to run on STM32U575xx devices on STM32U575I-EV board from STMicroelectronics.  
The LCD is initialized using the BSP functions for display with color format set to RGB565.  
In this transfer mode two input sources are fetched : foreground and background.
- In this example, the foreground and background are configured as following :

   - Foreground object
   - input memory address at FLASH memory (static image arrays coded in FLASH).
   - Format : RGB565
   - Size   : 240x130
   - Background object
   - input memory address at FLASH memory (static image arrays coded in FLASH).
   - Format : RGB565
   - Size   : 240x130
   - The constant alpha for foreground is decreased to see the background.
   - The alpha mode for foreground and background is configured to see two 
     superposed images in the resulting image out of the ChromArt DMA2D operation.
   - The Foreground alpha inversion is set sequentially to regular then inverted to demonstrate
     this new DMA2D feature capability.

- The DMA2D blends the two sources pixels buffers stored into the FLASH to compute
the resulting pixel in the LCD display frame buffer. 
The output blended image is placed in the center of the LCD.
The transferred data to the output memory address is the result of the blending operation 
between background and foreground (with Foreground alpha inversion set to regular or inverted).

- The example acts as follow :

In an infinite while loop  
  - Use the DMA2D to blend the two sources images to the LCD display frame buffer.  
  - If the foreground alpha inversion is disabled (set to DMA2D_REGULAR_ALPHA) :  
    “Display Blended Image: Foreground Alpha Inversion OFF”  
  - If the foreground alpha inversion is enabled (set to DMA2D_INVERTED_ALPHA) :  
    “Display Blended Image: Foreground Alpha Inversion ON”  
  - Change the alpha inversion setting, wait for 2 sec and go to the next loop.

User can see the result blending image with foreground alpha inversion enabled and disabled.

#### <b>Notes</b>

 1. The example shows how to use the DMA2D blending with alpha inversion set on the foreground layer.
    Nevertheless the same parameter can be applied on the background DMA2D input layer 
    or/and at the output level depending of the user application requirements.

 - GREEN LED is ON when the DMA2D transfer is complete.
 - RED LED toggles when there is a DMA2D transfer error.
 - RED LED is on when there is an error occurs.

 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The application needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

Graphic, DMA2D, RGB565, Blending, Alpha Inversion, Memory to memory, Foreground,
Background, Pixel, SRAM, LCD, Display, TrustZone disabled

### <b>Directory contents</b>

  - DMA2D/DMA2D_BlendingWithAlphaInversion/Src/main.c                  Main program
  - DMA2D/DMA2D_BlendingWithAlphaInversion/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - DMA2D/DMA2D_BlendingWithAlphaInversion/Src/stm32u5xx_it.c          Interrupt handlers
  - DMA2D/DMA2D_BlendingWithAlphaInversion/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - DMA2D/DMA2D_BlendingWithAlphaInversion/Inc/main.h                  Main program header file
  - DMA2D/DMA2D_BlendingWithAlphaInversion/Inc/stm32u575i_eval_conf.h  BSP Configuration file
  - DMA2D/DMA2D_BlendingWithAlphaInversion/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - DMA2D/DMA2D_BlendingWithAlphaInversion/Inc/stm32u5xx_it.h          Interrupt handlers header file
  - DMA2D/DMA2D_BlendingWithAlphaInversion/Inc/RGB565_240x130_1.h      Image used for DMAD2D foreground fetching
  - DMA2D/DMA2D_BlendingWithAlphaInversion/Inc/RGB565_240x130_2.h      Image used for DMAD2D background fetching

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

