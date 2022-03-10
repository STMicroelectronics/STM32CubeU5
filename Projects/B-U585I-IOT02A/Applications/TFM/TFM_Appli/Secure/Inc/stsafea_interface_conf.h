/**
  ******************************************************************************
  * @file    stsafea_interface_conf.h
  * @author  SMD application team
  * @brief   STSAFE-A Interface header file for application customization and use
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
#ifndef STSAFEA_INTERFACE_CONF_H
#define STSAFEA_INTERFACE_CONF_H


#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/** @addtogroup STSAFEA
  * @{
  */

/** @defgroup STSAFEA_INTERFACE
  * @{
  */

/** @defgroup STSAFEA_INTERFACE_Exported_Constants
  * @{
  */
/* Uncomment to associate and pair definitively STM32 and STSAFE */
//#define USE_PAIRING

#ifdef USE_PAIRING
/* Set to 1 if Host Keys are retrieved from a generation algorithm during the runtime.
   Set to 0 otherwise to use static keys values.
   This is used just as example in order to support static keys values (mainly
   for debugging purpose ) or switch to the right and proper way to retrieve the keys */
#define USE_COMPUTED_HOST_KEYS       1U

/* Set to 1 if STSAFE has not been provisioned with a SW application   */
/* Set to 0 otherwise to avoid self verification of provisioning state */
#define USE_SELF_PROVISIONING        0U
#endif /* USE_PAIRING */

/* Set to 1 in order to use Signature Sessions.
   Set to 0 to optimize code/memory size otherwise */
#define USE_SIGNATURE_SESSION                   0U

/* Include for MCU definition platform */
#define MCU_PLATFORM_INCLUDE                    "stm32u5xx.h"

#define NUM_READ_FLASH 16

/**
  * @}
  */

/** @defgroup STSAFEA_INTERFACE_Exported_Macros
  * @{
  */


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

#endif /* STSAFEA_INTERFACE_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
