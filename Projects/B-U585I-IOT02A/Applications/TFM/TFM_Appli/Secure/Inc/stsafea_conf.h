/**
  ******************************************************************************
  * @file    stsafea_conf.h
  * @author  SMD application team
  * @brief   STSAFE-A1XX Middleware configuration file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * STSAFE DRIVER SOFTWARE LICENSE AGREEMENT (SLA0088)
  *
  * BY INSTALLING, COPYING, DOWNLOADING, ACCESSING OR OTHERWISE USING THIS SOFTWARE
  * OR ANY PART THEREOF (AND THE RELATED DOCUMENTATION) FROM STMICROELECTRONICS
  * INTERNATIONAL N.V, SWISS BRANCH AND/OR ITS AFFILIATED COMPANIES (STMICROELECTRONICS),
  * THE RECIPIENT, ON BEHALF OF HIMSELF OR HERSELF, OR ON BEHALF OF ANY ENTITY BY WHICH
  * SUCH RECIPIENT IS EMPLOYED AND/OR ENGAGED AGREES TO BE BOUND BY THIS SOFTWARE LICENSE
  * AGREEMENT.
  *
  * Under STMicroelectronics’ intellectual property rights, the redistribution,
  * reproduction and use in source and binary forms of the software or any part thereof,
  * with or without modification, are permitted provided that the following conditions
  * are met:
  * 1.  Redistribution of source code (modified or not) must retain any copyright notice,
  *     this list of conditions and the disclaimer set forth below as items 10 and 11.
  * 2.  Redistributions in binary form, except as embedded into a microcontroller or
  *     microprocessor device or a software update for such device, must reproduce any
  *     copyright notice provided with the binary code, this list of conditions, and the
  *     disclaimer set forth below as items 10 and 11, in documentation and/or other
  *     materials provided with the distribution.
  * 3.  Neither the name of STMicroelectronics nor the names of other contributors to this
  *     software may be used to endorse or promote products derived from this software or
  *     part thereof without specific written permission.
  * 4.  This software or any part thereof, including modifications and/or derivative works
  *     of this software, must be used and execute solely and exclusively in combination
  *     with a secure microcontroller device from STSAFE family manufactured by or for
  *     STMicroelectronics.
  * 5.  No use, reproduction or redistribution of this software partially or totally may be
  *     done in any manner that would subject this software to any Open Source Terms.
  *     “Open Source Terms” shall mean any open source license which requires as part of
  *     distribution of software that the source code of such software is distributed
  *     therewith or otherwise made available, or open source license that substantially
  *     complies with the Open Source definition specified at www.opensource.org and any
  *     other comparable open source license such as for example GNU General Public
  *     License(GPL), Eclipse Public License (EPL), Apache Software License, BSD license
  *     or MIT license.
  * 6.  STMicroelectronics has no obligation to provide any maintenance, support or
  *     updates for the software.
  * 7.  The software is and will remain the exclusive property of STMicroelectronics and
  *     its licensors. The recipient will not take any action that jeopardizes
  *     STMicroelectronics and its licensors' proprietary rights or acquire any rights
  *     in the software, except the limited rights specified hereunder.
  * 8.  The recipient shall comply with all applicable laws and regulations affecting the
  *     use of the software or any part thereof including any applicable export control
  *     law or regulation.
  * 9.  Redistribution and use of this software or any part thereof other than as  permitted
  *     under this license is void and will automatically terminate your rights under this
  *     license.
  * 10. THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" AND ANY
  *     EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  *     WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
  *     OF THIRD PARTY INTELLECTUAL PROPERTY RIGHTS, WHICH ARE DISCLAIMED TO THE FULLEST
  *     EXTENT PERMITTED BY LAW. IN NO EVENT SHALL STMICROELECTRONICS OR CONTRIBUTORS BE
  *     LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  *     DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  *     THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  *     NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  *     ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * 11. EXCEPT AS EXPRESSLY PERMITTED HEREUNDER, NO LICENSE OR OTHER RIGHTS, WHETHER EXPRESS
  *     OR IMPLIED, ARE GRANTED UNDER ANY PATENT OR OTHER INTELLECTUAL PROPERTY RIGHTS OF
  *     STMICROELECTRONICS OR ANY THIRD PARTY.
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STSAFEA_CONF_H
#define STSAFEA_CONF_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/** @addtogroup STSAFEA
  * @{
  */

/** @defgroup STSAFEA_CONFIG
  * @{
  */

/** @defgroup STSAFEA_CONFIG_Exported_Constants
  * @{
  */

/* Set to 1 to optimize RAM usage. If set to 1 the StSafeA_Handle_t.InOutBuffer used through the Middleware APIs
   is shared
   with the application between each command & response. It means that every time the MW API returns a TLVBuffer pointer,
   it returns in fact a pointer to the shared StSafeA_Handle_t.InOutBuffer.
   As consequence the user shall copy data from given pointer into variable defined by himself in case data need
   to be stored.
   If set to 0 the user must specifically allocate (statically or dynamically) a right sized buffer to be passed as
   parameter
   to the Middleware command API */
#define STSAFEA_USE_OPTIMIZATION_SHARED_RAM             1U

/* Set to 1 to optimize RAM/Flash usage, when Host MAC and encryption features are not required (eg. Authentication use case) */
#define STSAFEA_USE_OPTIMIZATION_NO_HOST_MAC_ENCRYPT    0U

/* Set to 1 to have CRC16_CCIT Table already calculated and placed in Flash as const. Set to zero to dynamically calculate it in RAM */
#define STSAFEA_USE_OPTIMIZATION_CRC_TABLE              0U

/* Set to 1  to expanse the "assert_param" macro in the STSAFE middleware code   */
#define STSAFEA_USE_FULL_ASSERT                         0U

/**
  * @}
  */


/** @defgroup STSAFEA_INTERFACE_Exported_Macros
  * @{
  */

/* Weak function definition. A different __weak function definition might be needed for different platforms */
#if defined ( __GNUC__ )
#if !defined (__CC_ARM)
#ifndef __weak
#define __weak   __attribute__((weak))
#endif /* __weak */
#endif /* __CC_ARM */
#endif /* __GNUC__ */

/* Endianness bytes swap */
#if defined ( __ICCARM__ )
#include "intrinsics.h"
#define SWAP2BYTES(x)    __REV16(x)            /*!< 16-bits Big-Little endian bytes swap */
#define SWAP4BYTES(x)    __REV(x)              /*!< 32-bits Big-Little endian bytes swap */
#elif defined ( __CC_ARM )
#include <stdint.h>
static __inline __asm uint32_t __rev16(uint32_t value)
{
  rev16 r0, r0
  bx lr
}
#define SWAP2BYTES(x)    __rev16(x)            /*!< 16-bits Big-Little endian bytes swap */
#define SWAP4BYTES(x)    __rev(x)              /*!< 32-bits Big-Little endian bytes swap */
#elif defined( __ARMCC_VERSION ) && ( __ARMCC_VERSION >= 6010050 )
#define SWAP2BYTES(x)    __builtin_bswap16(x)  /*!< 16-bits Big-Little endian bytes swap */
#define SWAP4BYTES(x)    __builtin_bswap32(x)  /*!< 32-bits Big-Little endian bytes swap */
#elif defined ( __GNUC__ )
#define SWAP2BYTES(x)    __builtin_bswap16(x)  /*!< 16-bits Big-Little endian bytes swap */
#define SWAP4BYTES(x)    __builtin_bswap32(x)  /*!< 32-bits Big-Little endian bytes swap */
#endif /* __ICCARM__  __CC_ARM  __ARMCC_VERSION && __ARMCC_VERSION >= 6010050  __GNUC__ */

/**
  * @}
  */


/** @defgroup STSAFEA_CONFIG_Exported_Macros
  * @{
  */

#if (STSAFEA_USE_FULL_ASSERT)
  #include <stdint.h>
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed.
  *         If expr is true, it returns no value.
  * @retval None
  */
  #define stsafea_assert_param(expr) ((expr) ? (void)0U : stsafea_assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void stsafea_assert_failed(uint8_t* file, uint32_t line);
#else
  #define stsafea_assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STSAFEA_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
