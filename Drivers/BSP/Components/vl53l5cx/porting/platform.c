/**
  ******************************************************************************
  * @file    platform.h
  * @author  IMG SW Application Team
  * @brief   This file contains all the platform functions prototypes
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

#include "platform.h"

uint8_t RdByte(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_value)
{
  return p_platform->Read(p_platform->address, RegisterAdress, p_value, 1U);
}

uint8_t WrByte(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t value)
{
  return p_platform->Write(p_platform->address, RegisterAdress, &value, 1U);
}

uint8_t WrMulti(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_values,
		uint32_t size)
{
  return p_platform->Write(p_platform->address, RegisterAdress, p_values, size);
}

uint8_t RdMulti(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_values,
		uint32_t size)
{
  return p_platform->Read(p_platform->address, RegisterAdress, p_values, size);
}

void SwapBuffer(
    uint8_t     *buffer,
    uint16_t     size)
{
  uint32_t i, tmp;

  /* Example of possible implementation using <string.h> */
  for(i = 0; i < size; i = i + 4)
  {
    tmp = (
      buffer[i]<<24)
    |(buffer[i+1]<<16)
    |(buffer[i+2]<<8)
    |(buffer[i+3]);

    memcpy(&(buffer[i]), &tmp, 4);
  }
}

uint8_t WaitMs(
		VL53L5CX_Platform *p_platform,
		uint32_t TimeMs)
{
  uint32_t tickstart;
  tickstart = p_platform->GetTick();

  while ((p_platform->GetTick() - tickstart) < TimeMs);

  return 0;
}

