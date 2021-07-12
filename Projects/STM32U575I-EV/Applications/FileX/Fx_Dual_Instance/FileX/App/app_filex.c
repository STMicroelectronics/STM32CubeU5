/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_filex.c
  * @author  MCD Application Team
  * @brief   FileX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_filex.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32u575i_eval.h"
#include "stm32u575i_eval_ospi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEFAULT_SECTOR_SIZE              512
#define DEFAULT_STACK_SIZE               (2 * 1024)
#define DEFAULT_THREAD_PRIO              10
#define DEFAULT_TIME_SLICE               4
#define DEFAULT_PREEMPTION_THRESHOLD     DEFAULT_THREAD_PRIO

/* fx media buffer of size equals a one sector */
#define DEFAULT_MEDIA_BUF_LENGTH         DEFAULT_SECTOR_SIZE

#define THREAD_ID_M                      0
#define THREAD_ID_1                      1
#define THREAD_ID_2                      2

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

    /* Buffer for FileX FX_MEDIA sector cache. this should be 32-Bytes
aligned to avoid cache maintenance issues */
ALIGN_32BYTES (uint32_t media_memory[DEFAULT_SECTOR_SIZE / sizeof(uint32_t)]);

/* Define FileX global data structures.  */
FX_MEDIA        sdio_disk;
FX_MEDIA        nor_flash_disk;
FX_FILE         fx_file;
FX_FILE         fx_file_one;
/* Define ThreadX global data structures.  */
TX_THREAD       fx_thread_main;
TX_THREAD       fx_thread_one;
TX_THREAD       fx_thread_two;
/* Define child threads completion event flags */
TX_EVENT_FLAGS_GROUP    finish_flag;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
#if defined ( __GNUC__)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

VOID fx_thread_main_entry(ULONG thread_input);
VOID fx_thread_one_entry(ULONG thread_input);
VOID fx_thread_two_entry(ULONG thread_input);
VOID App_Error_Handler(INT id);

void Error_Handler(void);
static VOID os_delay(ULONG delay);
/* USER CODE END PFP */

/**
  * @brief  Application FileX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_FileX_Init(VOID *memory_ptr)
{
  UINT ret = FX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_FileX_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END App_FileX_MEM_POOL */

  /* USER CODE BEGIN MX_FileX_Init */
  VOID *pointer;

  /* Allocate memory for the main thread's stack */
  ret = tx_byte_allocate(byte_pool, &pointer, DEFAULT_STACK_SIZE, TX_NO_WAIT);

  if (ret != FX_SUCCESS)
  {
    /* Failed at allocating memory */
    Error_Handler();
  }

  /* Create the main thread.  */
  tx_thread_create(&fx_thread_main, "fx_thread_main", fx_thread_main_entry, 0, pointer, DEFAULT_STACK_SIZE, DEFAULT_THREAD_PRIO,
                   DEFAULT_PREEMPTION_THRESHOLD, DEFAULT_TIME_SLICE, TX_AUTO_START);

  /* Allocate memory for the 1st concurrent thread's stack */
  ret = tx_byte_allocate(byte_pool, &pointer, DEFAULT_STACK_SIZE, TX_NO_WAIT);

  if (ret != FX_SUCCESS)
  {
    /* Failed at allocating memory */
    Error_Handler();
  }

  /* Create the 1st concurrent thread.  */
  tx_thread_create(&fx_thread_one, "fx_thread_one", fx_thread_one_entry, 0, pointer, DEFAULT_STACK_SIZE, DEFAULT_THREAD_PRIO,
                   DEFAULT_PREEMPTION_THRESHOLD, DEFAULT_TIME_SLICE, TX_DONT_START);

  /* Allocate memory for the 2nd concurrent thread's stack */
  ret = tx_byte_allocate(byte_pool, &pointer, DEFAULT_STACK_SIZE, TX_NO_WAIT);

  if (ret != FX_SUCCESS)
  {
    /* Failed at allocating memory */
    Error_Handler();
  }

  /* Create the 2nd concurrent thread */
  tx_thread_create(&fx_thread_two, "fx_thread_two", fx_thread_two_entry, 0, pointer, DEFAULT_STACK_SIZE, DEFAULT_THREAD_PRIO,
                   DEFAULT_PREEMPTION_THRESHOLD, DEFAULT_TIME_SLICE, TX_DONT_START);

  /* An event flag to indicate the status of execution */
  tx_event_flags_create(&finish_flag, "event_flag");

  /* Initialize FileX.  */
  fx_system_initialize();

  /* USER CODE END MX_FileX_Init */

  return ret;
}

/* USER CODE BEGIN 1 */
static VOID os_delay(ULONG delay)
{
  ULONG start = tx_time_get();
  while ((tx_time_get() - start) < delay) {}
}

VOID fx_thread_main_entry(ULONG thread_input)
{

  UINT status;
  ULONG event_flags;


  /* Open the SD disk driver.  */
  status =  fx_media_open(&sdio_disk, "STM32_SDIO_DISK", fx_stm32_sd_driver, 0, (VOID *) media_memory, sizeof(media_memory));

  /* Check the media open status.  */
  if (status != FX_SUCCESS)
  {
    App_Error_Handler(THREAD_ID_M);
  }

  /* Media opened successfully, we start the concurrent threads. */
  status = tx_thread_resume(&fx_thread_one) & tx_thread_resume(&fx_thread_two);

  /* Check the concurrent thread was started correctly.  */
  if (status != TX_SUCCESS)
  {
    App_Error_Handler(THREAD_ID_M);
  }

  /* block here waiting for concurrent threads to finish processing */
  status = tx_event_flags_get(&finish_flag, 0x11, TX_AND_CLEAR,
  	&event_flags, TX_WAIT_FOREVER);

  /* Check the status.  */
  if (status != TX_SUCCESS)
  {
    /* Error getting the event flags, call error handler.  */
    App_Error_Handler(THREAD_ID_M);
  }

  /* Close the media.  */
  status =  fx_media_close(&sdio_disk);

  /* Check the media close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the media, call error handler.  */
    App_Error_Handler(THREAD_ID_M);
  }

  /* Toggle green LED to indicate processing finish OK */
  while(1)
  {
  	BSP_LED_Toggle(LED_GREEN);
  	os_delay(40);
  }
}

VOID fx_thread_one_entry(ULONG thread_input)
{

  UINT status;
  ULONG bytes_read;
  CHAR read_buffer[32];
  CHAR data[] = "This file is stored in the microSD card";

  /* Create a file called STM32_FILE1.TXT in the root directory.  */
  status =  fx_file_create(&sdio_disk, "STM32_FILE1.TXT");

  /* Check the create status.  */
  if (status != FX_SUCCESS)
  {
    /* Check for an already created status. This is expected on the
    second pass of this loop!  */
    if (status != FX_ALREADY_CREATED)
    {
      /* Create error, call error handler.  */
      App_Error_Handler(THREAD_ID_1);
    }
  }

  /* Open the test file.  */
  status =  fx_file_open(&sdio_disk, &fx_file_one, "STM32_FILE1.TXT", FX_OPEN_FOR_WRITE);

  /* Check the file open status.  */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, call error handler.  */
    App_Error_Handler(THREAD_ID_1);
  }

  /* Seek to the beginning of the test file.  */
  status =  fx_file_seek(&fx_file_one, 0);

  /* Check the file seek status.  */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, call error handler.  */
    App_Error_Handler(THREAD_ID_1);
  }

  /* Write a string to the test file.  */
  status =  fx_file_write(&fx_file_one, data, sizeof(data));

  /* Check the file write status.  */
  if (status != FX_SUCCESS)
  {
    /* Error writing to a file, call error handler.  */
    App_Error_Handler(THREAD_ID_1);
  }

  /* Close the test file.  */
  status =  fx_file_close(&fx_file_one);

  /* Check the file close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler.  */
    App_Error_Handler(THREAD_ID_1);
  }

  status = fx_media_flush(&sdio_disk);

  /* Check the media flush  status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler.  */
    App_Error_Handler(THREAD_ID_1);
  }

  /* Open the test file.  */
  status =  fx_file_open(&sdio_disk, &fx_file_one, "STM32_FILE1.TXT", FX_OPEN_FOR_READ);

  /* Check the file open status.  */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, call error handler.  */
    App_Error_Handler(THREAD_ID_1);
  }

  /* Seek to the beginning of the test file.  */
  status =  fx_file_seek(&fx_file_one, 0);

  /* Check the file seek status.  */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, call error handler.  */
    App_Error_Handler(THREAD_ID_1);
  }

  /* Read the content of the test file.  */
  status =  fx_file_read(&fx_file_one, read_buffer, sizeof(data), &bytes_read);

  /* Check the file read status.  */
  if ((status != FX_SUCCESS) || (bytes_read != sizeof(data)))
  {
    /* Error reading file, call error handler.  */
    App_Error_Handler(THREAD_ID_1);
  }

  /* Close the test file.  */
  status =  fx_file_close(&fx_file_one);

  /* Check the file close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler.  */
    App_Error_Handler(THREAD_ID_1);
  }

  status = tx_event_flags_set(&finish_flag,  0x01, TX_OR);

  /* Check the event setting status.  */
  if (status != TX_SUCCESS)
  {
    /* Error calling the event setter, call error handler.  */
    App_Error_Handler(THREAD_ID_1);
  }

  while(1);
}

VOID fx_thread_two_entry(ULONG thread_input)
{

  UINT status;
  ULONG bytes_read;
  ULONG available_space_pre;
  ULONG available_space_post;
  CHAR read_buffer[32];
  CHAR data[] = "This file i stored in the Nor flash memory";
  BSP_OSPI_NOR_Info_t ospi_info;

  printf("FileX/LevelX NOR OCTO-SPI Application Start.\n");

  /* Get NOR chip info */
  if(BSP_OSPI_NOR_GetInfo(LX_STM32_OSPI_INSTANCE, &ospi_info) != BSP_ERROR_NONE)
  {
    while(1);
  }

  /* Print the absolute size of the NOR chip*/
  printf("Total NOR Flash Chip size is: %lu bytes.\n", ospi_info.FlashSize);

  /* Format the NOR flash as FAT */
  status =  fx_media_format(&nor_flash_disk,
                            fx_stm32_levelx_nor_driver,   // Driver entry
                            (VOID*)LX_NOR_OSPI_DRIVER_ID,    // Device info pointer
                            (UCHAR *) media_memory,       // Media buffer pointer
                            DEFAULT_MEDIA_BUF_LENGTH,     // Media buffer size
                            "NOR_FLASH_DISK",             // Volume Name
                            1,                            // Number of FATs
                            32,                           // Directory Entries
                            0,                            // Hidden sectors
                            ospi_info.FlashSize / DEFAULT_SECTOR_SIZE,      // Total sectors
                            DEFAULT_SECTOR_SIZE,          // Sector size
                            8,                            // Sectors per cluster
                            1,                            // Heads
                            1);                           // Sectors per track

  /* Check if the format status */
  if (status != FX_SUCCESS)
  {
    App_Error_Handler(THREAD_ID_2);
  }

  /* Open the OCTO-SPI NOR Flash disk driver.  */
  status =  fx_media_open(&nor_flash_disk, "FX_LX_NOR_DISK", fx_stm32_levelx_nor_driver,(VOID*)LX_NOR_OSPI_DRIVER_ID , (VOID *) media_memory, DEFAULT_MEDIA_BUF_LENGTH);

  /* Check the media open status.  */
  if (status != FX_SUCCESS)
  {
    App_Error_Handler(THREAD_ID_2);
  }

  /* Get the available usable space */
  status =  fx_media_space_available(&nor_flash_disk, &available_space_pre);

  printf("User available NOR Flash disk space size before file is written: %lu bytes.\n", available_space_pre);

  /* Check the get available state request status.  */
  if (status != FX_SUCCESS)
  {
    App_Error_Handler(THREAD_ID_2);
  }

  /* Create a file called STM32.TXT in the root directory.  */
  status =  fx_file_create(&nor_flash_disk, "STM32.TXT");

  /* Check the create status.  */
  if (status != FX_SUCCESS)
  {
    /* Check for an already created status. This is expected on the
    second pass of this loop!  */
    if (status != FX_ALREADY_CREATED)
    {
      /* Create error, call error handler.  */
      App_Error_Handler(THREAD_ID_2);
    }
  }

  /* Open the test file.  */
  status =  fx_file_open(&nor_flash_disk, &fx_file, "STM32.TXT", FX_OPEN_FOR_WRITE);

  /* Check the file open status.  */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Seek to the beginning of the test file.  */
  status =  fx_file_seek(&fx_file, 0);

  /* Check the file seek status.  */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Write a string to the test file.  */
  status =  fx_file_write(&fx_file, data, sizeof(data));

  /* Check the file write status.  */
  if (status != FX_SUCCESS)
  {
    /* Error writing to a file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Close the test file.  */
  status =  fx_file_close(&fx_file);

  /* Check the file close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  status = fx_media_flush(&nor_flash_disk);

  /* Check the media flush  status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

   /* Open the test file.  */
  status =  fx_file_open(&nor_flash_disk, &fx_file, "STM32.TXT", FX_OPEN_FOR_READ);

  /* Check the file open status.  */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Seek to the beginning of the test file.  */
  status =  fx_file_seek(&fx_file, 0);

  /* Check the file seek status.  */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Read the first 28 bytes of the test file.  */
  status =  fx_file_read(&fx_file, read_buffer, sizeof(data), &bytes_read);

  /* Check the file read status.  */
  if ((status != FX_SUCCESS) || (bytes_read != sizeof(data)))
  {
    /* Error reading file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
   }

  /* Close the test file.  */
  status =  fx_file_close(&fx_file);

  /* Check the file close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Get the available usable space, after the file has been created */
  status =  fx_media_space_available(&nor_flash_disk, &available_space_post);

  printf("User available NOR Flash disk space size after file is written: %lu bytes.\n", available_space_post);
  printf("The test file occupied a total of %lu cluster(s) (%u per cluster).",
         (available_space_pre - available_space_post) / (nor_flash_disk.fx_media_bytes_per_sector * nor_flash_disk.fx_media_sectors_per_cluster),
         nor_flash_disk.fx_media_bytes_per_sector * nor_flash_disk.fx_media_sectors_per_cluster);

  /* Check the get available state request status.  */
  if (status != FX_SUCCESS)
  {
    App_Error_Handler(THREAD_ID_2);
  }

  /* Close the media.  */
  status =  fx_media_close(&nor_flash_disk);

  /* Check the media close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the media, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  status = tx_event_flags_set(&finish_flag,  0x10, TX_OR);

  /* Check the event setting status.  */
  if (status != TX_SUCCESS)
  {
    /* Error calling the event setter, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  while(1);
}

VOID App_Error_Handler(INT id)
{

switch (id)
  {

  case THREAD_ID_M :
    /* terminate the other threads to preserve the call stack pointing here */
    tx_thread_terminate(&fx_thread_one);
    tx_thread_terminate(&fx_thread_two);
    break;

  case THREAD_ID_1 :
  	/* terminate the other threads to preserve the call stack pointing here */
    tx_thread_terminate(&fx_thread_main);
    tx_thread_terminate(&fx_thread_two);
    break;

  case THREAD_ID_2 :
  	/* terminate the other threads to preserve the call stack pointing here */
    tx_thread_terminate(&fx_thread_main);
    tx_thread_terminate(&fx_thread_one);
    break;
  }

  /* Call the main error handler */
  Error_Handler();

}
/* USER CODE END 1 */
