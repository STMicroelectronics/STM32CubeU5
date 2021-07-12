/*
 * Copyright (c) 2018-2019 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "platform/include/tfm_plat_device_id.h"
#include "tfm_secure_api.h"
#include "bl2/include/tfm_boot_status.h"
#include <stddef.h>

enum tfm_plat_err_t tfm_plat_get_implementation_id(uint32_t *size,
                                                   uint8_t  *buf)
{
  uint8_t *p_dst;
  int32_t tfm_res;;
  p_dst = &buf[0];
  if (*size < IMPLEMENTATION_ID_MAX_SIZE)
  {
    return TFM_PLAT_ERR_SYSTEM_ERR;
  }
  /*  platform support maximum size */
  *size = IMPLEMENTATION_ID_MAX_SIZE;

  tfm_res = tfm_core_get_boot_value(SET_TLV_TYPE(TLV_MAJOR_CORE, TLV_MINOR_CORE_IAS_IMPLEMENTATION_ID), p_dst, (*size));
  if (tfm_res != (int32_t)TFM_SUCCESS)
  {
    return  TFM_PLAT_ERR_SYSTEM_ERR;
  }
  return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_get_hw_version(uint32_t *size, uint8_t *buf)
{
  uint32_t hw_version_size = 14;
  uint16_t version;
  uint16_t revision;
  /* version is used a 3 hexadecimal print in decimal = 6 bytes*/
  version = DBGMCU->IDCODE & 0xfff;
  /* revision is used 4 hexadecimal print = 8 bytes */
  revision = DBGMCU->IDCODE >> 16;
  if (*size < hw_version_size)
  {
    return TFM_PLAT_ERR_SYSTEM_ERR;
  }
  buf[0] = '0' + (((version >> 8) & 0xf) / 10);
  buf[1] = '0' + (((version >> 8) & 0xf) % 10);
  buf[2] = '0' + (((version >> 4) & 0xf) / 10);
  buf[3] = '0' + (((version >> 4) & 0xf) % 10);
  buf[4] = '0' + (((version >> 0) & 0xf) / 10);
  buf[5] = '0' + (((version >> 0) & 0xf) % 10);
  buf[6] = '0' + (((revision >> 12) & 0xf) / 10);
  buf[7] = '0' + (((revision >> 12) & 0xf) % 10);
  buf[8] = '0' + (((revision >> 8) & 0xf) / 10);
  buf[9] = '0' + (((revision >> 8) & 0xf) % 10);
  buf[10] = '0' + (((revision >> 4) & 0xf) / 10);
  buf[11] = '0' + (((revision >> 4) & 0xf) % 10);
  buf[12] = '0' + (((revision >> 0) & 0xf) / 10);
  buf[13] = '0' + (((revision >> 0) & 0xf) % 10);
  *size = hw_version_size;

  return TFM_PLAT_ERR_SUCCESS;
}
