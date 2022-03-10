/**
  ******************************************************************************
  * @file    low_level_rng.c
  * @author  MCD Application Team
  * @brief   Low Level Interface module to use STM32 RNG Ip
  *          This file provides mbed-crypto random generataor
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "low_level_rng.h"
#include "stm32l5xx_hal.h"
extern void Error_Handler(void);

static RNG_HandleTypeDef handle;
static uint8_t users = 0;

#define COMPILER_BARRIER() __ASM __IO("" : : : "memory")

static uint8_t atomic_incr_u8(__IO uint8_t *valuePtr, uint8_t delta)
{
  COMPILER_BARRIER();
  uint8_t newValue;
  do
  {
    newValue = __LDREXB(valuePtr) + delta;
  } while (__STREXB(newValue, valuePtr));
  COMPILER_BARRIER();
  return newValue;
}

static void RNG_Init(void)
{
  uint32_t dummy;
  /*  We're only supporting a single user of RNG */
  if (atomic_incr_u8(&users, 1) > 1)
  {
    Error_Handler();
  }

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  /*Select PLLQ output as RNG clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
  PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_MSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
   Error_Handler();
  }

  /* RNG Peripheral clock enable */
  __HAL_RCC_RNG_CLK_ENABLE();

  /* Initialize RNG instance */
  handle.Instance = RNG;
  handle.State = HAL_RNG_STATE_RESET;
  handle.Lock = HAL_UNLOCKED;

  HAL_RNG_Init(&handle);

  /* first random number generated after setting the RNGEN bit should not be used */
  HAL_RNG_GenerateRandomNumber(&handle, &dummy);
}

static void RNG_GetBytes(uint8_t *output, size_t length, size_t *output_length)
{
  int32_t ret = 0;
  __IO uint8_t random[4];
  *output_length = 0;

  /* Get Random byte */
  while ((*output_length < length) && (ret == 0))
  {
    if (HAL_RNG_GenerateRandomNumber(&handle, (uint32_t *)random) != HAL_OK)
    {
      ret = -1;
    }
    else
    {
      for (uint8_t i = 0; (i < 4) && (*output_length < length) ; i++)
      {
        *output++ = random[i];
        *output_length += 1;
        random[i] = 0;
      }
    }
  }
  /* Just be extra sure that we didn't do it wrong */
  if ((__HAL_RNG_GET_FLAG(&handle, (RNG_FLAG_CECS | RNG_FLAG_SECS))) != 0)
  {
    *output_length = 0;
  }
}

static void RNG_DeInit(void)
{
  /*Disable the RNG peripheral */
  HAL_RNG_DeInit(&handle);
  /* RNG Peripheral clock disable - assume we're the only users of RNG  */
  __HAL_RCC_RNG_CLK_DISABLE();

  users = 0;
}


/*  interface for mbed-crypto */
int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
  RNG_Init();
  RNG_GetBytes(output, len, olen);
  RNG_DeInit();
  if (*olen != len)
  {
    return -1;
  }
  return 0;
}
