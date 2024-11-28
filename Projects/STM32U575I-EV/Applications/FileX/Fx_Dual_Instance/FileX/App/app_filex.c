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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* Main thread stack size */
#define FX_APP_THREAD_STACK_SIZE         (2 * 1024)
/* Main thread priority */
#define FX_APP_THREAD_PRIO               10
/* USER CODE BEGIN PD */
#define DEFAULT_SECTOR_SIZE              512
#define DEFAULT_TIME_SLICE               4
/* fx media buffer of size equals a one sector */
#define DEFAULT_MEDIA_BUF_LENGTH         DEFAULT_SECTOR_SIZE

#define THREAD_ID_M                      0
#define THREAD_ID_1                      1
#define THREAD_ID_2                      2

#define read_buffer_size                 40

#define DEFAULT_QUEUE_LENGTH             16

/* Message content*/
typedef enum {
CARD_STATUS_CHANGED             = 99,
CARD_STATUS_DISCONNECTED        = 88,
CARD_STATUS_CONNECTED           = 77
} SD_ConnectionStateTypeDef;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define MEDIA_CLOSED                     1UL
#define MEDIA_OPENED                     0UL
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* Main thread global data structures.  */
TX_THREAD       fx_app_thread;

/* Buffer for FileX FX_MEDIA sector cache. */
ALIGN_32BYTES (uint32_t fx_sd_media_memory[FX_STM32_SD_DEFAULT_SECTOR_SIZE / sizeof(uint32_t)]);
/* Define FileX global data structures.  */
FX_MEDIA        sdio_disk;

/* Buffer for FileX FX_MEDIA sector cache. */
ALIGN_32BYTES (uint32_t fx_nor_ospi_media_memory[FX_NOR_OSPI_SECTOR_SIZE / sizeof(uint32_t)]);
/* Define FileX global data structures.  */
FX_MEDIA        nor_ospi_flash_disk;

/* USER CODE BEGIN PV */
static UINT media_status;
/* Define FileX global data structures.  */
FX_FILE         fx_file_sd;
FX_FILE         fx_file_ospi;
/* Define ThreadX global data structures.  */
TX_THREAD       fx_thread_one;
TX_THREAD       fx_thread_two;
/* Define child threads completion event flags */
TX_EVENT_FLAGS_GROUP    finish_flag;
TX_QUEUE        tx_msg_queue;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* Main thread entry function.  */
void fx_app_thread_entry(ULONG thread_input);

/* USER CODE BEGIN PFP */
VOID fx_thread_one_entry(ULONG thread_input);
VOID fx_thread_two_entry(ULONG thread_input);
VOID App_Error_Handler(INT id);
static uint8_t SD_IsDetected(uint32_t Instance);
static VOID media_close_callback (FX_MEDIA *media_ptr);
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
  VOID *pointer;

/* USER CODE BEGIN MX_FileX_MEM_POOL */

/* USER CODE END MX_FileX_MEM_POOL */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*Allocate memory for the main thread's stack*/
  ret = tx_byte_allocate(byte_pool, &pointer, FX_APP_THREAD_STACK_SIZE, TX_NO_WAIT);

/* Check FX_APP_THREAD_STACK_SIZE allocation*/
  if (ret != FX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

/* Create the main thread.  */
  ret = tx_thread_create(&fx_app_thread, FX_APP_THREAD_NAME, fx_app_thread_entry, 0, pointer, FX_APP_THREAD_STACK_SIZE,
                         FX_APP_THREAD_PRIO, FX_APP_PREEMPTION_THRESHOLD, FX_APP_THREAD_TIME_SLICE, FX_APP_THREAD_AUTO_START);

/* Check main thread creation */
  if (ret != FX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

/* USER CODE BEGIN MX_FileX_Init */
  /* Allocate memory for the 1st concurrent thread's stack */
  ret = tx_byte_allocate(byte_pool, &pointer, FX_APP_THREAD_STACK_SIZE, TX_NO_WAIT);

  if (ret != FX_SUCCESS)
  {
    /* Failed at allocating memory */
    Error_Handler();
  }

  /* Create the 1st concurrent thread.  */
  tx_thread_create(&fx_thread_one, "fx_thread_one", fx_thread_one_entry, 0, pointer, FX_APP_THREAD_STACK_SIZE, FX_APP_THREAD_PRIO,
                   FX_APP_PREEMPTION_THRESHOLD, DEFAULT_TIME_SLICE, TX_DONT_START);

  /* Allocate memory for the 2nd concurrent thread's stack */
  ret = tx_byte_allocate(byte_pool, &pointer, FX_APP_THREAD_STACK_SIZE, TX_NO_WAIT);

  if (ret != FX_SUCCESS)
  {
    /* Failed at allocating memory */
    Error_Handler();
  }

  /* Create the 2nd concurrent thread */
  tx_thread_create(&fx_thread_two, "fx_thread_two", fx_thread_two_entry, 0, pointer, FX_APP_THREAD_STACK_SIZE, FX_APP_THREAD_PRIO,
                   FX_APP_PREEMPTION_THRESHOLD, DEFAULT_TIME_SLICE, TX_DONT_START);

  /* An event flag to indicate the status of execution */
  tx_event_flags_create(&finish_flag, "event_flag");

  /* Create the message queue */
  ret = tx_queue_create(&tx_msg_queue, "sd_event_queue", 1, pointer, DEFAULT_QUEUE_LENGTH * sizeof(ULONG));

/* Check main thread creation */
  if (ret != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }
/* USER CODE END MX_FileX_Init */

/* Initialize FileX.  */
  fx_system_initialize();

/* USER CODE BEGIN MX_FileX_Init 1*/

/* USER CODE END MX_FileX_Init 1*/

  return ret;
}

/**
 * @brief  Main thread entry.
 * @param thread_input: ULONG user argument used by the thread entry
 * @retval none
*/
 void fx_app_thread_entry(ULONG thread_input)
 {

  UINT sd_status = FX_SUCCESS;

  UINT nor_ospi_status = FX_SUCCESS;

/* USER CODE BEGIN fx_app_thread_entry 0*/
  UINT tx_status = FX_SUCCESS;

  ULONG event_flags;
/* USER CODE END fx_app_thread_entry 0*/

/* Open the SD disk driver */
  sd_status =  fx_media_open(&sdio_disk, FX_SD_VOLUME_NAME, fx_stm32_sd_driver, (VOID *)FX_NULL, (VOID *) fx_sd_media_memory, sizeof(fx_sd_media_memory));

/* Check the media open sd_status */
  if (sd_status != FX_SUCCESS)
  {
     /* USER CODE BEGIN SD DRIVER get info error */
    while(1);
    /* USER CODE END SD DRIVER get info error */
  }

/* Format the OCTO-SPI NOR flash as FAT */
  nor_ospi_status =  fx_media_format(&nor_ospi_flash_disk,                                                           // nor_ospi_flash_disk pointer
                                     fx_stm32_levelx_nor_driver,                                                     // Driver entry
                                     (VOID *)LX_NOR_OSPI_DRIVER_ID,                                                  // Device info pointer
                                     (UCHAR *) fx_nor_ospi_media_memory,                                             // Media buffer pointer
                                     sizeof(fx_nor_ospi_media_memory),                                               // Media buffer size
                                     FX_NOR_OSPI_VOLUME_NAME,                                                        // Volume Name
                                     FX_NOR_OSPI_NUMBER_OF_FATS,                                                     // Number of FATs
                                     32,                                                                             // Directory Entries
                                     FX_NOR_OSPI_HIDDEN_SECTORS,                                                     // Hidden sectors
                                     (LX_STM32_OSPI_FLASH_SIZE - LX_STM32_OSPI_SECTOR_SIZE)/ FX_NOR_OSPI_SECTOR_SIZE,// Total sectors minus one
                                     FX_NOR_OSPI_SECTOR_SIZE,                                                        // Sector size
                                     8,                                                                              // Sectors per cluster
                                     1,                                                                              // Heads
                                     1);                                                                             // Sectors per track

/* Check the format nor_ospi_status */
  if (nor_ospi_status != FX_SUCCESS)
  {
    /* USER CODE BEGIN OCTO-SPI NOR format error */
    while(1);
    /* USER CODE END OCTO-SPI NOR format error */
  }

  /* Open the OCTO-SPI NOR driver */
 nor_ospi_status =  fx_media_open(&nor_ospi_flash_disk, FX_NOR_OSPI_VOLUME_NAME, fx_stm32_levelx_nor_driver, (VOID *)LX_NOR_OSPI_DRIVER_ID, (VOID *) fx_nor_ospi_media_memory, sizeof(fx_nor_ospi_media_memory));

/* Check the media open nor_ospi_status */
  if (nor_ospi_status != FX_SUCCESS)
  {
    /* USER CODE BEGIN OCTO-SPI NOR open error */
    while(1);
    /* USER CODE END OCTO-SPI NOR open error */
  }

/* USER CODE BEGIN fx_app_thread_entry 1*/
  /* Media opened successfully, we start the concurrent threads. */
  tx_status = tx_thread_resume(&fx_thread_one);

  /* Check the concurrent thread was started correctly.  */
  if (tx_status != TX_SUCCESS)
  {
    App_Error_Handler(THREAD_ID_M);
  }

  tx_status = tx_thread_resume(&fx_thread_two);


  /* Check the concurrent thread was started correctly.  */
  if (tx_status != TX_SUCCESS)
  {
    App_Error_Handler(THREAD_ID_M);
  }

  /* block here waiting for concurrent threads to finish processing */
  tx_status = tx_event_flags_get(&finish_flag, 0x11, TX_AND_CLEAR,
                              &event_flags, TX_WAIT_FOREVER);

  /* Check the tx_status.  */
  if (tx_status != TX_SUCCESS)
  {
    /* Error getting the event flags, call error handler.  */
    App_Error_Handler(THREAD_ID_M);
  }

  /* Close the media.  */
  tx_status =  fx_media_close(&sdio_disk);

  /* Check the media close tx_status.  */
  if (tx_status != FX_SUCCESS)
  {
    /* Error closing the media, call error handler.  */
    App_Error_Handler(THREAD_ID_M);
  }

  /* Toggle green LED to indicate processing finish OK */
  while (1)
  {
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 2);

  }
/* USER CODE END fx_app_thread_entry 1*/
  }

/* USER CODE BEGIN 1 */

VOID fx_thread_one_entry(ULONG thread_input)
{
  UINT sd_status = FX_SUCCESS;
  ULONG r_msg;
  ULONG s_msg = CARD_STATUS_CHANGED;
  ULONG last_status = CARD_STATUS_DISCONNECTED;
  ULONG bytes_read;
  CHAR read_buffer[read_buffer_size];
  CHAR data[] = "This file is stored in the microSD card";

  fx_media_close_notify_set(&sdio_disk, media_close_callback);

  if(SD_IsDetected(FX_STM32_SD_INSTANCE) == HAL_OK)
  {
    /* SD card is already inserted, place the info into the queue */
    tx_queue_send(&tx_msg_queue, &s_msg, TX_NO_WAIT);
  }
  else
  {
    /* Indicate that SD card is not inserted from start */
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
  }

  /* Infinite Loop */
  for( ;; )
  {

    /* We wait here for a valid SD card insertion event, if it is not inserted already */
    while(1)
    {

      while(tx_queue_receive(&tx_msg_queue, &r_msg, TX_TIMER_TICKS_PER_SECOND / 2) != TX_SUCCESS)
      {
        /* Toggle GREEN LED to indicate idle state after a successful operation */
        if(last_status == CARD_STATUS_CONNECTED)
        {
          HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        }
      }

      /* check if we received the correct event message */
      if(r_msg == CARD_STATUS_CHANGED)
      {
        /* reset the status */
        r_msg = 0;

        /* for debouncing purpose we wait a bit till it settles down */
        tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND / 2);

        if(SD_IsDetected(FX_STM32_SD_INSTANCE) == HAL_OK)
        {
          /* We have a valid SD insertion event, start processing.. */
          /* Update last known status */
          last_status = CARD_STATUS_CONNECTED;
          HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET); /*LED_RED Off*/
          break;
        }
        else
        {
          /* Update last known status */
          last_status = CARD_STATUS_DISCONNECTED;
          HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);  /*LED_GREEN Off*/
          HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET); /*LED_RED On*/
        }
      }
    }

    /* Create a file called STM32.TXT in the root directory.  */
    if (media_status == MEDIA_CLOSED)
    {
      sd_status = fx_media_open(&sdio_disk, FX_SD_VOLUME_NAME, fx_stm32_sd_driver, (VOID *)FX_NULL, (VOID *) fx_sd_media_memory, sizeof(fx_sd_media_memory));

      /* Check the media open sd_status */
      if (sd_status != FX_SUCCESS)
      {
        App_Error_Handler(THREAD_ID_1);
      }
      media_status = MEDIA_OPENED;
    }

    sd_status =  fx_file_create(&sdio_disk, "STM32_FILE1.TXT");

    /* Check the create status.  */
    if (sd_status != FX_SUCCESS)
    {
      /* Check for an already created status. This is expected on the
      second pass of this loop!  */
      if (sd_status != FX_ALREADY_CREATED)
      {
        App_Error_Handler(THREAD_ID_1);
      }
    }

    /* Open the test file.  */
    sd_status =  fx_file_open(&sdio_disk, &fx_file_sd, "STM32_FILE1.TXT", FX_OPEN_FOR_WRITE);

    /* Check the file open status.  */
    if (sd_status != FX_SUCCESS)
    {
      App_Error_Handler(THREAD_ID_1);
    }

    /* Seek to the beginning of the test file.  */
    sd_status =  fx_file_seek(&fx_file_sd, 0);

    /* Check the file seek status.  */
    if (sd_status != FX_SUCCESS)
    {
      App_Error_Handler(THREAD_ID_1);
    }

    /* Write a string to the test file.  */
    sd_status =  fx_file_write(&fx_file_sd, data, sizeof(data));

    /* Check the file write status.  */
    if (sd_status != FX_SUCCESS)
    {
      App_Error_Handler(THREAD_ID_1);
    }

    /* Close the test file.  */
    sd_status =  fx_file_close(&fx_file_sd);

    /* Check the file close status.  */
    if (sd_status != FX_SUCCESS)
    {
      App_Error_Handler(THREAD_ID_1);
    }

    sd_status = fx_media_flush(&sdio_disk);

    /* Check the media flush  status.  */
    if (sd_status != FX_SUCCESS)
    {
      App_Error_Handler(THREAD_ID_1);
    }

    /* Open the test file.  */
    sd_status =  fx_file_open(&sdio_disk, &fx_file_sd, "STM32_FILE1.TXT", FX_OPEN_FOR_READ);

    /* Check the file open status.  */
    if (sd_status != FX_SUCCESS)
    {
      App_Error_Handler(THREAD_ID_1);
    }

    /* Seek to the beginning of the test file.  */
    sd_status =  fx_file_seek(&fx_file_sd, 0);

    /* Check the file seek status.  */
    if (sd_status != FX_SUCCESS)
    {
      App_Error_Handler(THREAD_ID_1);
    }

    /* Read the first 28 bytes of the test file.  */
    sd_status =  fx_file_read(&fx_file_sd, read_buffer, sizeof(data), &bytes_read);

    /* Check the file read status.  */
    if ((sd_status != FX_SUCCESS) || (bytes_read != sizeof(data)))
    {
      App_Error_Handler(THREAD_ID_1);
    }

    /* Close the test file.  */
    sd_status =  fx_file_close(&fx_file_sd);

    /* Check the file close status.  */
    if (sd_status != FX_SUCCESS)
    {
      App_Error_Handler(THREAD_ID_1);
    }

    /* Close the media.  */
    sd_status =  fx_media_close(&sdio_disk);

    /* Check the media close status.  */
    if (sd_status != FX_SUCCESS)
    {
      App_Error_Handler(THREAD_ID_1);
    }

    sd_status = tx_event_flags_set(&finish_flag, FX_SUCCESS, TX_OR);

    /* Check the event setting status.  */
    if (sd_status != TX_SUCCESS)
    {
      /* Error calling the event setter, call error handler.  */
      App_Error_Handler(THREAD_ID_1);
    }

    /* Do nothing wait for the other thread */
    tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND);
  }
}
VOID fx_thread_two_entry(ULONG thread_input)
{

  UINT status;
  ULONG bytes_read;
  ULONG available_space_pre;
  ULONG available_space_post;
  CHAR read_buffer[43];
  CHAR data[] = "This file is stored in the Nor flash memory";

  printf("FileX/LevelX NOR OCTO-SPI Application Start.\n");


  /* Print the absolute size of the NOR chip*/
  printf("Total NOR Flash Chip size is: %lu bytes.\n", (ULONG)LX_STM32_OSPI_FLASH_SIZE);

  /* Get the available usable space */
  status =  fx_media_space_available(&nor_ospi_flash_disk, &available_space_pre);

  printf("User available NOR Flash disk space size before file is written: %lu bytes.\n", available_space_pre);

  /* Check the get available state request status.  */
  if (status != FX_SUCCESS)
  {
    App_Error_Handler(THREAD_ID_2);
  }

  /* Create a file called STM32.TXT in the root directory.  */
  status =  fx_file_create(&nor_ospi_flash_disk, "STM32.TXT");

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
  status =  fx_file_open(&nor_ospi_flash_disk, &fx_file_ospi, "STM32.TXT", FX_OPEN_FOR_WRITE);

  /* Check the file open status.  */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Seek to the beginning of the test file.  */
  status =  fx_file_seek(&fx_file_ospi, 0);

  /* Check the file seek status.  */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Write a string to the test file.  */
  status =  fx_file_write(&fx_file_ospi, data, sizeof(data));

  /* Check the file write status.  */
  if (status != FX_SUCCESS)
  {
    /* Error writing to a file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Close the test file.  */
  status =  fx_file_close(&fx_file_ospi);

  /* Check the file close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  status = fx_media_flush(&nor_ospi_flash_disk);

  /* Check the media flush  status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Open the test file.  */
  status =  fx_file_open(&nor_ospi_flash_disk, &fx_file_ospi, "STM32.TXT", FX_OPEN_FOR_READ);

  /* Check the file open status.  */
  if (status != FX_SUCCESS)
  {
    /* Error opening file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Seek to the beginning of the test file.  */
  status =  fx_file_seek(&fx_file_ospi, 0);

  /* Check the file seek status.  */
  if (status != FX_SUCCESS)
  {
    /* Error performing file seek, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Read the first 28 bytes of the test file.  */
  status =  fx_file_read(&fx_file_ospi, read_buffer, sizeof(read_buffer), &bytes_read);

  /* Check the file read status.  */
  if ((status != FX_SUCCESS) || (bytes_read != sizeof(read_buffer)))
  {
    /* Error reading file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Close the test file.  */
  status =  fx_file_close(&fx_file_ospi);

  /* Check the file close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the file, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  /* Get the available usable space, after the file has been created */
  status =  fx_media_space_available(&nor_ospi_flash_disk, &available_space_post);

  printf("User available NOR Flash disk space size after file is written: %lu bytes.\n", available_space_post);
  printf("The test file occupied a total of %lu cluster(s) (%u per cluster).\n",
         (available_space_pre - available_space_post) / (nor_ospi_flash_disk.fx_media_bytes_per_sector * nor_ospi_flash_disk.fx_media_sectors_per_cluster),
         nor_ospi_flash_disk.fx_media_bytes_per_sector * nor_ospi_flash_disk.fx_media_sectors_per_cluster);

  /* Check the get available state request status.  */
  if (status != FX_SUCCESS)
  {
    App_Error_Handler(THREAD_ID_2);
  }

  /* Close the media.  */
  status =  fx_media_close(&nor_ospi_flash_disk);

  /* Check the media close status.  */
  if (status != FX_SUCCESS)
  {
    /* Error closing the media, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  status = tx_event_flags_set(&finish_flag, FX_DIR_NOT_EMPTY, TX_OR);

  /* Check the event setting status.  */
  if (status != TX_SUCCESS)
  {
    /* Error calling the event setter, call error handler.  */
    App_Error_Handler(THREAD_ID_2);
  }

  while (1)
  {
    /* Do nothing wait for the other thread */
    tx_thread_sleep(TX_TIMER_TICKS_PER_SECOND);
  }

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
      tx_thread_terminate(&fx_app_thread);
      tx_thread_terminate(&fx_thread_two);
      break;

    case THREAD_ID_2 :
      /* terminate the other threads to preserve the call stack pointing here */
      tx_thread_terminate(&fx_app_thread);
      tx_thread_terminate(&fx_thread_one);
      break;
  }

  /* Call the main error handler */
  Error_Handler();

}

/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @param Instance  SD Instance
 * @retval Returns if SD is detected or not
 */
static uint8_t SD_IsDetected(uint32_t Instance)
{
  uint8_t ret;
  if(Instance >= 1)
  {
    ret = HAL_ERROR;
  }
  else
  {
    /* Check SD card detect pin */
    if (HAL_GPIO_ReadPin(SDCARD_DETECT_GPIO_Port, SDCARD_DETECT_Pin) == GPIO_PIN_SET)
    {
      ret = HAL_ERROR;
    }
    else
    {
      ret = HAL_OK;
    }
  }

  return ret;
}

/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  ULONG s_msg = CARD_STATUS_CHANGED;

  if(GPIO_Pin == SDCARD_DETECT_Pin)
  {
    tx_queue_send(&tx_msg_queue, &s_msg, TX_NO_WAIT);
  }
}

/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  ULONG s_msg = CARD_STATUS_CHANGED;

  if(GPIO_Pin == SDCARD_DETECT_Pin)
  {
    tx_queue_send(&tx_msg_queue, &s_msg, TX_NO_WAIT);
  }
}

/**
  * @brief  Media close notify callback function.
  * @param  media_ptr: Media control block pointer
  * @retval None
  */
static VOID media_close_callback(FX_MEDIA *media_ptr)
{
  media_status = MEDIA_CLOSED;
}
/* USER CODE END 1 */
