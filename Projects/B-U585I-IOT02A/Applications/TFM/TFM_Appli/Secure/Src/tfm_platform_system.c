/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "platform/include/tfm_platform_system.h"
#include "cmsis.h"
#include "flash_layout.h"
#include "platform_ioctl.h"
#if !defined(MCUBOOT_OVERWRITE_ONLY) && ((MCUBOOT_APP_IMAGE_NUMBER == 2) || (MCUBOOT_S_DATA_IMAGE_NUMBER == 1))
#include "Driver_Flash.h"
extern ARM_DRIVER_FLASH FLASH_PRIMARY_SECURE_DEV_NAME;
extern ARM_DRIVER_FLASH FLASH_PRIMARY_DATA_SECURE_DEV_NAME;
#define TRAILER_MAGIC_SIZE 16
#endif
#ifdef TFM_TEST_PRIV_PROTECTION
#include <string.h>
#endif
static enum tfm_platform_err_t tfm_platform_confirm_secure_app_image(void);
static enum tfm_platform_err_t tfm_platform_confirm_secure_data_image(void);
#ifdef TFM_TEST_PRIV_PROTECTION
static enum tfm_platform_err_t tfm_platform_read(psa_invec *in_vec, psa_outvec *out_vec);
static enum tfm_platform_err_t tfm_platform_write(psa_invec *in_vec, psa_outvec *out_vec);
static enum tfm_platform_err_t tfm_platform_write_exec(psa_invec *in_vec, psa_outvec *out_vec);
#endif
#ifdef TFM_DEV_MODE
static __IO int once=0;
void Error_Handler(void)
{
	/* Reset the system */
    while(once==0);
}
#else
void Error_Handler(void)
{
	/* Reset the system */
    NVIC_SystemReset();
}
#endif
void tfm_hal_system_reset(void)
{
    Error_Handler();
}

static enum tfm_platform_err_t tfm_platform_confirm_secure_app_image(void)
{
#if defined(MCUBOOT_OVERWRITE_ONLY) || (MCUBOOT_APP_IMAGE_NUMBER == 1)
	return TFM_PLATFORM_ERR_SYSTEM_ERROR;
#else
  const uint8_t FlagPattern[16]={0x1 ,0xff, 0xff, 0xff, 0xff , 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff , 0xff, 0xff, 0xff };
  const uint32_t ConfirmAddress = FLASH_AREA_0_OFFSET  + FLASH_AREA_0_SIZE - (TRAILER_MAGIC_SIZE + sizeof(FlagPattern));  
  if (FLASH_PRIMARY_SECURE_DEV_NAME.ProgramData(ConfirmAddress, FlagPattern, sizeof(FlagPattern)) == ARM_DRIVER_OK)
  {
        return TFM_PLATFORM_ERR_SUCCESS;
  }
  return TFM_PLATFORM_ERR_SYSTEM_ERROR;
#endif
}

static enum tfm_platform_err_t tfm_platform_confirm_secure_data_image(void)
{
#if defined(MCUBOOT_OVERWRITE_ONLY) || (MCUBOOT_S_DATA_IMAGE_NUMBER == 0)
	return TFM_PLATFORM_ERR_SYSTEM_ERROR;
#else
  const uint8_t FlagPattern[16]={0x1 ,0xff, 0xff, 0xff, 0xff , 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff , 0xff, 0xff, 0xff };
  const uint32_t ConfirmAddress = FLASH_AREA_4_OFFSET  + FLASH_AREA_4_SIZE - (TRAILER_MAGIC_SIZE + sizeof(FlagPattern));  
  if (FLASH_PRIMARY_DATA_SECURE_DEV_NAME.ProgramData(ConfirmAddress, FlagPattern, sizeof(FlagPattern)) == ARM_DRIVER_OK)
  {
        return TFM_PLATFORM_ERR_SUCCESS;
  }
  return TFM_PLATFORM_ERR_SYSTEM_ERROR;
#endif
}

#ifdef TFM_TEST_PRIV_PROTECTION
/* variable for privileged test */
__IO __attribute__((__aligned__(4))) uint32_t priv_test_table = 0x01020304;
void priv_test_func(void)
{
    static __IO int TestPrivExec=0;
    TestPrivExec++;
}

static enum tfm_platform_err_t tfm_platform_read(psa_invec *in_vec,
                                               psa_outvec *out_vec)
{
void * address;
uint32_t length;
    /* in_vec contains address to read and length */
    if ((!in_vec) || (in_vec->len != 8))
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    address = (void *)(*(uint32_t *)(in_vec->base));
    length= *((uint32_t*)((uint32_t)in_vec->base + sizeof(address)));
    if ((!out_vec) || (out_vec->len != length))
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    memcpy(out_vec->base, address, length);
    return TFM_PLATFORM_ERR_SUCCESS;
}
static enum tfm_platform_err_t tfm_platform_write(psa_invec *in_vec,
                                               psa_outvec *out_vec)
{
    void * dest;
    uint32_t length;
    void * src;
    /* in_vec contains address to write ,  length, data to write*/
    if ((!in_vec) || (in_vec->len <= 8))
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    dest = (void *)(*(uint32_t*)(in_vec->base));
    length= *((uint32_t*)((uint32_t)in_vec->base + sizeof(dest)));
    src = (void *)((uint32_t)(in_vec->base)+8);
    /* outvec contains nothing */
    if ((out_vec))
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    memcpy(dest, src, length);
    return TFM_PLATFORM_ERR_SUCCESS;
}
/*  function used to validate write_execute */
void Test_Execute_Plat()
{
    __IO static int TestPlatNumber=0;
    TestPlatNumber++;
}


static enum tfm_platform_err_t tfm_platform_write_exec(psa_invec *in_vec,
                                               psa_outvec *out_vec)
{
    void * dest;
    uint32_t length;
    uint32_t val;
    void * src;
    void (*func)(void);
    /* in_vec contains address to execute and possibly code op to write 4 bytes maximum*/
    if ((!in_vec) || (in_vec->len > 8))
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    dest = (void *)(*(uint32_t*)(in_vec->base));
    /* length to write if requested */
    length= in_vec->len - sizeof(dest);
    src = (void *)((uint32_t)(in_vec->base)+4);
    /* outvec contains nothing */
    if (out_vec)
        return TFM_PLATFORM_ERR_SYSTEM_ERROR;
    if (length) {
        /*  read value  */
        memcpy(&val, dest, length);
        /*  recopy Code OP */
        memcpy(dest, src, length);
        /* Flush and refill pipeline  */
        __DSB();
        __ISB();
    }
    if (length == 2)
        func = (void(*)(void))((uint32_t)dest+1);
    else
        func = (void(*)(void))(dest);

    func();
    if (length) {
        memcpy(dest, &val, length);
    }
    return TFM_PLATFORM_ERR_SUCCESS;
}
#endif /* TFM_TEST_PRIV_PROTECTION */

enum tfm_platform_err_t tfm_platform_hal_ioctl(tfm_platform_ioctl_req_t request,
                                               psa_invec *in_vec,
                                               psa_outvec *out_vec)
{
  switch (request)
  {
    case PLATFORM_IOTCL_FWSEC_CONFIRM:
	  return tfm_platform_confirm_secure_app_image();
	  break;
    case PLATFORM_IOTCL_DATASEC_CONFIRM:
	  return tfm_platform_confirm_secure_data_image();
	  break;
#ifdef TFM_TEST_PRIV_PROTECTION
    case PLATFORM_IOCTL_TEST_READ:
      return tfm_platform_read(in_vec,out_vec);
      break;
    case PLATFORM_IOCTL_TEST_WRITE:
      return tfm_platform_write(in_vec,out_vec);
      break;
    case PLATFORM_IOCTL_TEST_WRITE_EXEC:
      return tfm_platform_write_exec(in_vec,out_vec);
      break;
#endif /* TFM_TEST_PRIV_PROTECTION */
    default:
      return TFM_PLATFORM_ERR_NOT_SUPPORTED;
  }
} 
