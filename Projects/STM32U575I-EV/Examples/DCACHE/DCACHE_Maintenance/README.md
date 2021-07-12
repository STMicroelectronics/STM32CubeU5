## <b>DCACHE_Maintenance Example Description</b>

How to do Data-Cache maintenance on a shared memory buffer accessed by 2 masters (CPU and DMA).

This project is targeted to run on STM32U575xx devices on STM32U575I-EV board from STMicroelectronics.

This example includes three MPU configurations of External SRAM, and only one configuration
should be chosen in main.h:

- MPU_WRITE_THROUGH.
- MPU_WRITE_BACK_WRITE_ALLOCATE.
- MPU_WRITE_BACK_NO_WRITE_ALLOCATE.

In this examples, two DMA transfers are configured using **GPDMA1_Channel15**:

*First Transfer:*

   - From Flash to external SRAM
   - Destination buffer is put in DCACHE, before starting the transfer
   
*Second Transfer:*

   - From external SRAM cacheable buffer to internal SRAM non-cacheable buffer
   - Source buffer modified by CPU in DCACHE, before starting the transfer

Then, these 2 DMA transfers are started, then source and destination buffers are compared.
Leds colors will indicate the buffer comparison results.
In the first step, only one or two transfer comparison results could be wrong, depending
on the chosen MPU configuration.
Then, User has to press on Tamper button to do a Cache maintenance, in order to
ensure the cache coherency between CPU and DMA.
Thereafter, a new buffer comparison is done to indicate transfer status after Cache maintenance,
and leds colors should indicate the correct comparison status for the 2 transfers.

STM32U575I-EV board's LEDs can be used to monitor the transfer status:

 **For the first transfer:**

 - **LED_GREEN is ON** when the transfer is completed and buffers comparison is correct.
 - **LED_GREEN is OFF** when the transfer is completed and buffers comparison is wrong.
 
 **For the second transfer:** 

 - **LED_RED is ON** when the transfer is completed and buffers comparison is correct.
 - **LED_RED is OFF** when the transfer is completed and buffers comparison is wrong.

**Expected behaviour:**

LED_GREEN and LED_RED will be ON to indicating that the 2 transfers are correct, after pressing on TAMPER button 

Cache Maintenance:

* It is recommended to enable the cache and maintain its coherence, but depending
on the use case, it is also possible to configure the MPU as "Write through",
to guarantee the write access coherence. In that case, the MPU must be configured
as Cacheable/Not bufferable/Not Shareable. Even though the user must manage
the cache coherence for read accesses.

* When User can’t ensure the Buffer Length Alignment, there are two possible solutions:
   * Use the **HAL_DCACHE_CleanInvalidByAddr()** instead of **HAL_InvalidateDCache_by_Addr()**.
   * Use a Write-Through MPU configuration, so there is no need to make a Cache clean.

* Be careful before using the Invalidate all data cache in Write-Back policy:
   * Risk to lose all the modification which are not yet evicted.
   * Use always Cache Clean before the Cache Invalidate.

* Clean all Data-Cache shall not cause a coherency problem but will impact performance,
since it will fetch all dirty lines in the DCACHE and evicted even the non-shareable data.

#### <b>Notes</b>

 1. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 2. The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

### <b>Keywords</b>

Memory, DCACHE, FMC, SRAM, Read, Write, Initialization, Access, TrustZone disabled

### <b>Directory contents</b>

  - DCACHE/DCACHE_Maintenace/Src/main.c                  Main program
  - DCACHE/DCACHE_Maintenace/Src/system_stm32u5xx.c      STM32U5xx system clock configuration file
  - DCACHE/DCACHE_Maintenace/Src/stm32u5xx_it.c          Interrupt handlers
  - DCACHE/DCACHE_Maintenace/Src/stm32u5xx_hal_msp.c     HAL MSP module
  - DCACHE/DCACHE_Maintenace/Inc/main.h                  Main program header file
  - DCACHE/DCACHE_Maintenace/Inc/stm32u575i_eval_conf.h  BSP Configuration file
  - DCACHE/DCACHE_Maintenace/Inc/stm32u5xx_hal_conf.h    HAL Configuration file
  - DCACHE/DCACHE_Maintenace/Inc/stm32u5xx_it.h          Interrupt handlers header file

### <b>Hardware and Software environment</b>

  - This example runs on STM32U575xx devices without security enabled (TZEN=0).

  - This example has been tested with STMicroelectronics STM32U575I-EV (MB1550A)
    board and can be easily tailored to any other supported device
    and development board.

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

