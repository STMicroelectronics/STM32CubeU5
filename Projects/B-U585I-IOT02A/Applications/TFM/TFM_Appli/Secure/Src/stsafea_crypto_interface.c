/**
  ******************************************************************************
  * @file    stsafea_crypto_interface.c
  * @author  SMD application team
  * @brief   Crypto Interface file to support the crypto services required by the
  *          STSAFE-A Middleware and offered by the mbedTLS crypto library:
  *           + Key Management
  *           + SHA
  *           + AES
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
  * Under STMicroelectronics' intellectual property rights, the redistribution,
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
  *     "Open Source Terms" shall mean any open source license which requires as part of
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

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "stsafea_crypto.h"
#include "stsafea_interface_conf.h"
#include "se_stsafea_api.h"
#include MCU_PLATFORM_INCLUDE

#if (USE_SIGNATURE_SESSION) || (USE_COMPUTED_HOST_KEYS)
#include "mbedtls/sha256.h"
#endif /* USE_SIGNATURE_SESSION */

#if (USE_SIGNATURE_SESSION)
#include "mbedtls/sha512.h"
#endif /* USE_SIGNATURE_SESSION */

#include "mbedtls/aes.h"
#include "mbedtls/cmac.h"


#ifndef MBEDTLS_CMAC_C
#error Please activate MBEDTLS_CMAC_C in your mbedtls config file
#endif
#ifndef MBEDTLS_AES_C
#error Please activate MBEDTLS_AES_C in your mbedtls config file
#endif

#if (USE_SIGNATURE_SESSION)
#ifndef MBEDTLS_SHA256_C
#error Please activate MBEDTLS_SHA256_C in your mbedtls config file
#endif
#ifndef MBEDTLS_SHA512_C
#error Please activate MBEDTLS_SHA512_C in your mbedtls config file
#endif
#endif

#if (USE_COMPUTED_HOST_KEYS)
#ifndef MBEDTLS_SHA256_C
#error Please activate MBEDTLS_SHA256_C in your mbedtls config file
#endif
#ifndef PSA_USE_SE_ST
#error Please activate PSA_USE_SE_ST in your project file
#endif
#ifndef HW_CRYPTO_DPA_AES
#error Please activate HW_CRYPTO_DPA_AES in your mbedtls config file
#endif
#endif /* USE_COMPUTED_HOST_KEYS */

#if (USE_SELF_PROVISIONING)
#if (USE_COMPUTED_HOST_KEYS == 0)
#ifdef USE_PAIRING
#error Self provisioning with default host keys is not recommended (one way operation)
#endif
#endif
#endif /* USE_SELF_PROVISIONING */

/* Private typedef -----------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#if (USE_COMPUTED_HOST_KEYS)
static uint8_t  aHostCipherKey[STSAFEA_HOST_KEY_LENGTH]; /*!< STSAFE-A's Host cipher key */
static uint8_t  aHostMacKey   [STSAFEA_HOST_KEY_LENGTH]; /*!< STSAFE-A's Host Mac key */
#else
static uint8_t  aHostCipherKey[] = {0x11, 0x11, 0x22, 0x22, 0x33, 0x33, 0x44, 0x44, 0x55, 0x55, 0x66, 0x66, 0x77, 0x77, 0x88, 0x88}; /*!< STSAFE-A's Host cipher key */
static uint8_t  aHostMacKey   [] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}; /*!< STSAFE-A's Host Mac key */
#endif /* USE_COMPUTED_HOST_KEYS */

#if (USE_SIGNATURE_SESSION)
#ifdef MBEDTLS_SHA256_C
static mbedtls_sha256_context         sha256_ctx;
#endif /* MBEDTLS_SHA256_C */
#ifdef MBEDTLS_SHA512_C
static mbedtls_sha512_context         sha512_ctx;
#endif /* MBEDTLS_SHA512_C */
#endif /* USE_SIGNATURE_SESSION */

#if defined MBEDTLS_AES_C & defined MBEDTLS_CIPHER_MODE_CBC
static mbedtls_cipher_context_t       cipher_ctx;
#endif /* MBEDTLS_AES_C - MBEDTLS_CIPHER_MODE_CBC */

/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Functions Definition ------------------------------------------------------*/

/** @addtogroup CRYPTO_IF_Exported_Functions_Group1 Host MAC and Cipher keys Initialization
  *  @brief    Crypto Interface APIs to be implemented at application level. Templates are provided.
  *
@verbatim
 ===============================================================================
           ##### Host MAC and Cipher keys Initialization functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief   StSafeA_HostKeys_Init
  *          Initialize STSAFE-Axxx Host MAC and Cipher Keys that will be used by the crypto interface layer
  * @note    This is a weak function that MUST be implemented at application interface level.
  *          A specific example template stsafea_crypto_xxx_interface_template.c is provided with this Middleware
  *
  * @param   None
  * @retval  0 if success. An error code otherwise
  */
int32_t StSafeA_HostKeys_Init()
{
  return 0;
}

/**
  * @}
  */

#if (USE_COMPUTED_HOST_KEYS)
static int32_t StSafeA_AES_HUK_HostKeys_Generation(uint32_t *pEncryptedkey,
                                                   uint32_t *pDataIn,
                                                   uint32_t InDataLength,
                                                   uint32_t *HostKeys)
{
  int32_t status_code;
  mbedtls_aes_context aes;

  mbedtls_aes_init(&aes);

  status_code = mbedtls_aes_setkey_enc(&aes, NULL, 0);
  if (status_code != 0)
    return -1;

  status_code =  mbedtls_aes_unwrap(&aes,
                       pEncryptedkey,
                       pDataIn,
                       InDataLength,
                       HostKeys);
  if (status_code != 0)
   return -2;

  mbedtls_aes_free(&aes);

  return(0);
}

static void StSafeA_SHA_Generation(const uint32_t *input,
                                   uint32_t InMessageLength,
                                   uint32_t *output)
{
  mbedtls_sha256_context sha;

  mbedtls_sha256_init(&sha);

  mbedtls_sha256_starts_ret(&sha, 0);
  mbedtls_sha256_update_ret(&sha,(uint8_t *)input, InMessageLength);
  mbedtls_sha256_finish_ret(&sha,(uint8_t *)output);

  mbedtls_sha256_free(&sha);
}
#endif /* USE_COMPUTED_HOST_KEYS */

StSafeA_ResponseCode_t StSafeA_Crypto_GetKeys(uint8_t *serial, uint32_t size)
{
#if (USE_COMPUTED_HOST_KEYS)
  uint32_t encryptedkey[8];   /* Application Key       */
  uint32_t hw_id[8];          /* IDs of components     */
  uint32_t hw_id_digest[8];  /* finger print of IDs   */
  uint32_t GeneratedKeys[8]; /* computed pairing keys */

  memset(encryptedkey, 0, sizeof(encryptedkey));
  memset(hw_id, 0, sizeof(hw_id));
  memset(GeneratedKeys, 0, sizeof(GeneratedKeys));

  /* STM32 revision ID (16 bits) */
  /* STM32 device ID (12 bits) */
  hw_id[0] = (uint32_t)(DBGMCU->IDCODE & 0xFFFF0FFF);

  /* STSAFE ID concatenation */
  if ( size < sizeof(hw_id) )
    memcpy( &hw_id[1], serial, size );
  else
    memcpy( &hw_id[1], serial, sizeof(hw_id) );

  StSafeA_SHA_Generation( hw_id, sizeof(hw_id), hw_id_digest );

  /* STSAFE ID as input */
  if ( size < sizeof(encryptedkey) )
    memcpy( &encryptedkey[0], serial, size );
  else
    memcpy( &encryptedkey[0], serial, sizeof(encryptedkey) );

  if (StSafeA_AES_HUK_HostKeys_Generation(encryptedkey,
                                      hw_id_digest,
                                      8,
                                      GeneratedKeys))
    return(STSAFEA_UNEXPECTED_ERROR);

  /* Set and keep the keys that will be used during the Crypto MAC operations */
  (void)memcpy(aHostMacKey,
               (uint8_t *)&GeneratedKeys[0],
               16);

  (void)memcpy(aHostCipherKey,
               (uint8_t *)&GeneratedKeys[4],
               16);

  /* wipe the keys and digest */
  memset(GeneratedKeys, 0, sizeof(GeneratedKeys));
  memset(hw_id_digest, 0, sizeof(hw_id_digest));

#if (USE_SELF_PROVISIONING)
  SE_Pairing(aHostCipherKey, aHostMacKey);
#endif

  return STSAFEA_OK;
#else

#if (USE_SELF_PROVISIONING)
  SE_Pairing(aHostCipherKey, aHostMacKey);
#endif
  return STSAFEA_OK;
#endif
}
#if (USE_SIGNATURE_SESSION)
/** @addtogroup CRYPTO_IF_Exported_Functions_Group2 HASH Functions
  *  @brief    Crypto Interface APIs to be implemented at application level. Templates are provided.
  *
@verbatim
 ===============================================================================
                          ##### HASH functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief   StSafeA_SHA_Init
  *          SHA initialization function to initialize the SHA context
  * @note    This is a weak function that MUST be implemented at application interface level.
  *          A specific example template stsafea_crypto_xxx_interface_template.c is provided with this Middleware
  *
  * @param   InHashType : type of SHA
  *          This parameter can be one of the StSafeA_HashTypes_t enum values:
  *            @arg STSAFEA_SHA_256: 256-bits
  *            @arg STSAFEA_SHA_384: 384-bits
  * @param   ppShaCtx : SHA context to be initialized
  * @retval  None
  */
void StSafeA_SHA_Init(StSafeA_HashTypes_t InHashType, void **ppShaCtx)
{
  switch (InHashType)
  {
#ifdef MBEDTLS_SHA256_C
    case STSAFEA_SHA_256:
      *ppShaCtx = &sha256_ctx;
      mbedtls_sha256_init(*ppShaCtx);
      mbedtls_sha256_starts(*ppShaCtx, 0);
      break;
#endif /* MBEDTLS_SHA256_C */

#ifdef MBEDTLS_SHA512_C
    case STSAFEA_SHA_384:
      *ppShaCtx = &sha512_ctx;
      mbedtls_sha512_init(*ppShaCtx);
      mbedtls_sha512_starts(*ppShaCtx, 1);
      break;
#endif /* MBEDTLS_SHA512_C */

    default:
      break;
  }
}

/**
  * @brief   StSafeA_SHA_Update
  *          SHA update function to process SHA over a message data buffer.
  * @note    This is a weak function that MUST be implemented at application interface level.
  *          A specific example template stsafea_crypto_xxx_interface_template.c is provided with this Middleware
  *
  * @param   InHashType : type of SHA
  *          This parameter can be one of the StSafeA_HashTypes_t enum values:
  *            @arg STSAFEA_SHA_256: 256-bits
  *            @arg STSAFEA_SHA_384: 384-bits
  * @param   pShaCtx : SHA context
  * @param   pInMessage : message data buffer
  * @param   InMessageLength : message data buffer length
  * @retval  None
  */
void StSafeA_SHA_Update(StSafeA_HashTypes_t InHashType, void *pShaCtx, uint8_t *pInMessage, uint32_t InMessageLength)
{
  switch (InHashType)
  {
#ifdef MBEDTLS_SHA256_C
    case STSAFEA_SHA_256:
      if ((pShaCtx != NULL) && (pInMessage != NULL))
      {
        mbedtls_sha256_update(pShaCtx, pInMessage, InMessageLength);
      }
      break;
#endif /* MBEDTLS_SHA256_C */

#ifdef MBEDTLS_SHA512_C
    case STSAFEA_SHA_384:
      if ((pShaCtx != NULL) && (pInMessage != NULL))
      {
        mbedtls_sha512_update(pShaCtx, pInMessage, InMessageLength);
      }
      break;
#endif /* MBEDTLS_SHA512_C */

    default:
      break;
  }
}

/**
  * @brief   StSafeA_SHA_Final
  *          SHA final function to finalize the SHA Digest
  * @note    This is a weak function that MUST be implemented at application interface level.
  *          A specific example template stsafea_crypto_xxx_interface_template.c is provided with this Middleware
  *
  * @param   InHashType : type of SHA
  *          This parameter can be one of the StSafeA_HashTypes_t enum values:
  *            @arg STSAFEA_SHA_256: 256-bits
  *            @arg STSAFEA_SHA_384: 384-bits
  * @param   ppShaCtx : SHA context to be finalized
  * @param   pMessageDigest : message digest data buffer
  * @retval  None
  */
void StSafeA_SHA_Final(StSafeA_HashTypes_t InHashType, void **ppShaCtx, uint8_t *pMessageDigest)
{
  switch (InHashType)
  {
#ifdef MBEDTLS_SHA256_C
    case STSAFEA_SHA_256:
      if (*ppShaCtx != NULL)
      {
        if (pMessageDigest != NULL)
        {
          mbedtls_sha256_finish(*ppShaCtx, pMessageDigest);
        }
        mbedtls_sha256_free(*ppShaCtx);
        *ppShaCtx = NULL;
      }
      break;
#endif /* MBEDTLS_SHA256_C */

#ifdef MBEDTLS_SHA512_C
    case STSAFEA_SHA_384:
      if (*ppShaCtx != NULL)
      {
        if (pMessageDigest != NULL)
        {
          mbedtls_sha512_finish(*ppShaCtx, pMessageDigest);
        }
        mbedtls_sha512_free(*ppShaCtx);
        *ppShaCtx = NULL;
      }
      break;
#endif /* MBEDTLS_SHA512_C */

    default:
      break;
  }
}
#endif /* USE_SIGNATURE_SESSION */

/**
  * @}
  */


/** @addtogroup CRYPTO_IF_Exported_Functions_Group3 AES Functions
  *  @brief    Crypto Interface APIs to be implemented at application level. Templates are provided.
  *
@verbatim
 ===============================================================================
                          ##### AES functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief   StSafeA_AES_MAC_Start
  *          Start AES MAC computation
  * @note    This is a weak function that MUST be implemented at application interface level.
  *          A specific example template stsafea_crypto_xxx_interface_template.c is provided with this Middleware
  *
  * @param   ppAesMacCtx : AES MAC context
  * @retval  None
  */
void StSafeA_AES_MAC_Start(void **ppAesMacCtx)
{
#if defined MBEDTLS_AES_C & defined MBEDTLS_CIPHER_MODE_CBC
  *ppAesMacCtx = &cipher_ctx;

  mbedtls_cipher_init(*ppAesMacCtx);
  (void)mbedtls_cipher_setup(*ppAesMacCtx, mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB));
  (void)mbedtls_cipher_cmac_starts(*ppAesMacCtx, aHostMacKey, STSAFEA_HOST_KEY_LENGTH * 8U);
#endif /* MBEDTLS_AES_C - MBEDTLS_CIPHER_MODE_CBC */
}

/**
  * @brief   StSafeA_AES_MAC_Update
  *          Update / Add data to MAC computation
  * @note    This is a weak function that MUST be implemented at application interface level.
  *          A specific example template stsafea_crypto_xxx_interface_template.c is provided with this Middleware
  *
  * @param   pInData : data buffer
  * @param   InDataLength : data buffer length
  * @param   pAesMacCtx : AES MAC context
  * @retval  None
  */
void StSafeA_AES_MAC_Update(uint8_t *pInData, uint16_t InDataLength, void *pAesMacCtx)
{
#if defined MBEDTLS_AES_C & defined MBEDTLS_CIPHER_MODE_CBC
  (void)mbedtls_cipher_cmac_update(pAesMacCtx, pInData, InDataLength);
#endif /* MBEDTLS_AES_C - MBEDTLS_CIPHER_MODE_CBC */
}

/**
  * @brief   StSafeA_AES_MAC_LastUpdate
  *          Update / Add data to MAC computation
  * @note    This is a weak function that MUST be implemented at application interface level.
  *          A specific example template stsafea_crypto_xxx_interface_template.c is provided with this Middleware
  *
  * @param   pInData : data buffer
  * @param   InDataLength : data buffer length
  * @param   pAesMacCtx : AES MAC context
  * @retval  None
  */
void StSafeA_AES_MAC_LastUpdate(uint8_t *pInData, uint16_t InDataLength, void *pAesMacCtx)
{
  StSafeA_AES_MAC_Update(pInData, InDataLength, pAesMacCtx);
}

/**
  * @brief   StSafeA_AES_MAC_Final
  *          Finalize AES MAC computation
  * @note    This is a weak function that MUST be implemented at application interface level.
  *          A specific example template stsafea_crypto_xxx_interface_template.c is provided with this Middleware
  *
  * @param   pOutMac : calculated MAC
  * @param   ppAesMacCtx : AES MAC context
  * @retval  None
  */
void StSafeA_AES_MAC_Final(uint8_t *pOutMac, void **ppAesMacCtx)
{
#if defined MBEDTLS_AES_C & defined MBEDTLS_CIPHER_MODE_CBC
  (void)mbedtls_cipher_cmac_finish(*ppAesMacCtx, pOutMac);
  mbedtls_cipher_free(*ppAesMacCtx);
  *ppAesMacCtx = NULL;
#endif /* MBEDTLS_AES_C - MBEDTLS_CIPHER_MODE_CBC */
}

/**
  * @brief   StSafeA_AES_ECB_Encrypt
  *          AES ECB Encryption
  * @note    This is a weak function that MUST be implemented at application interface level.
  *          A specific example template stsafea_crypto_xxx_interface_template.c is provided with this Middleware
  *
  * @param   pInData : plain data buffer
  * @param   pOutData : encrypted output data buffer
  * @param   InAesType : type of AES. Can be one of the following values:
  *            @arg STSAFEA_KEY_TYPE_AES_128: AES 128-bits
  *            @arg STSAFEA_KEY_TYPE_AES_256: AES 256-bits
  * @retval  0 if success, an error code otherwise
  */
int32_t StSafeA_AES_ECB_Encrypt(uint8_t *pInData, uint8_t *pOutData, uint8_t InAesType)
{
#ifdef MBEDTLS_AES_C
  int32_t status_code;
  mbedtls_aes_context aes;

  switch (InAesType)
  {
    case STSAFEA_KEY_TYPE_AES_128:
    case STSAFEA_KEY_TYPE_AES_256:
      mbedtls_aes_init(&aes);
      status_code = mbedtls_aes_setkey_enc(&aes, aHostCipherKey, STSAFEA_AES_KEY_BITSIZE((uint32_t)InAesType));
      if ((status_code == 0) && (pInData != NULL) && (pOutData != NULL))
      {
        status_code = mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, pInData, pOutData);
      }
      mbedtls_aes_free(&aes);
      break;

    default:
      status_code = 1;
      break;
  }

  return status_code;
#else
  return 1;
#endif /* MBEDTLS_AES_C */
}

/**
  * @brief   StSafeA_AES_CBC_Encrypt
  *          AES CBC Encryption
  * @note    This is a weak function that MUST be implemented at application interface level.
  *          A specific example template stsafea_crypto_xxx_interface_template.c is provided with this Middleware
  *
  * @param   pInData : plain data buffer
  * @param   InDataLength : plain data buffer length
  * @param   pOutData : encrypted output data buffer
  * @param   InInitialValue : initial value
  * @param   InAesType : type of AES. Can be one of the following values:
  *            @arg STSAFEA_KEY_TYPE_AES_128: AES 128-bits
  *            @arg STSAFEA_KEY_TYPE_AES_256: AES 256-bits
  * @retval  0 if success, an error code otherwise
  */
int32_t StSafeA_AES_CBC_Encrypt(uint8_t *pInData, uint16_t InDataLength, uint8_t *pOutData,
                                uint8_t *InInitialValue, uint8_t InAesType)
{
#if defined MBEDTLS_AES_C & defined MBEDTLS_CIPHER_MODE_CBC
  int32_t status_code;
  mbedtls_aes_context aes;

  switch (InAesType)
  {
    case STSAFEA_KEY_TYPE_AES_128:
    case STSAFEA_KEY_TYPE_AES_256:
      mbedtls_aes_init(&aes);
      status_code = mbedtls_aes_setkey_enc(&aes, aHostCipherKey, STSAFEA_AES_KEY_BITSIZE((uint32_t)InAesType));
      if ((status_code == 0) && (pInData != NULL) && (pOutData != NULL) && (InInitialValue != NULL))
      {
        status_code = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, InDataLength, InInitialValue, pInData, pOutData);
      }
      mbedtls_aes_free(&aes);
      break;

    default:
      status_code = 1;
      break;
  }

  return status_code;
#else
  return 1;
#endif /* MBEDTLS_AES_C - MBEDTLS_CIPHER_MODE_CBC */
}

/**
  * @brief   StSafeA_AES_CBC_Decrypt
  *          AES CBC Decryption
  * @note    This is a weak function that MUST be implemented at application interface level.
  *          A specific example template stsafea_crypto_xxx_interface_template.c is provided with this Middleware
  *
  * @param   pInData : encrypted data buffer
  * @param   InDataLength : encrypted data buffer length
  * @param   pOutData : plain output data buffer
  * @param   InInitialValue : initial value
  * @param   InAesType : type of AES. Can be one of the following values:
  *            @arg STSAFEA_KEY_TYPE_AES_128: AES 128-bits
  *            @arg STSAFEA_KEY_TYPE_AES_256: AES 256-bits
  * @retval  0 if success, an error code otherwise
  */
int32_t StSafeA_AES_CBC_Decrypt(uint8_t *pInData, uint16_t InDataLength, uint8_t *pOutData,
                                uint8_t *InInitialValue, uint8_t InAesType)
{
#if defined MBEDTLS_AES_C & defined MBEDTLS_CIPHER_MODE_CBC
  int32_t status_code;
  mbedtls_aes_context aes;

  switch (InAesType)
  {
    case STSAFEA_KEY_TYPE_AES_128:
    case STSAFEA_KEY_TYPE_AES_256:
      mbedtls_aes_init(&aes);
      status_code = mbedtls_aes_setkey_dec(&aes, aHostCipherKey, STSAFEA_AES_KEY_BITSIZE((uint32_t)InAesType));
      if ((status_code == 0) && (pInData != NULL) && (pOutData != NULL) && (InInitialValue != NULL))
      {
        status_code = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, InDataLength, InInitialValue, pInData, pOutData);
      }
      mbedtls_aes_free(&aes);
      break;

    default:
      status_code = 1;
      break;
  }

  return status_code;
#else
  return 1;
#endif /* MBEDTLS_AES_C - MBEDTLS_CIPHER_MODE_CBC */
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
