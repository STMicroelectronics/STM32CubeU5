/**
  ******************************************************************************
  * @file    se_psa.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @brief   Implementation file of psa_drv_se interface.
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

#if defined(PSA_USE_SE_ST)

/* Includes ------------------------------------------------------------------*/
#include "tfm_mbedcrypto_include.h"
#include "se_psa.h"
#include "psa_manifest/pid.h"
#include "tfm_nspm.h"
#include "tfm_spm_log.h"
#include "stsafea_interface_conf.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  SE_Public_Key_Entry_t pub_keys[SE_RENEWABLE_PRIV_KEY_MAX];
  uint32_t map_size;
} SE_psa_drv_se_persistent_t;

/* Private define ------------------------------------------------------------*/
#define SE_ST_PRIV_MAX_SLOT 2

#ifndef SE_ST_MAX_REGION
#define SE_ST_MAX_REGION ((uint8_t)8) /* coping with SPL02 and SPL03 */
#endif

#define SE_ST_DATA       2 + SE_ST_MAX_REGION * sizeof(psa_se_st_mapping_attribute_t)

/* Private macro -------------------------------------------------------------*/
#define SE_ST_PSA_PRINT debug_print

/* Private struct ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/**
  * @brief   debug_print
  *          psa trace function for SE PSA traces.
  *
  * @param   fmt : char string to format output
  * @retval  return value from tfm_hal_output_spm_log
  */
int32_t debug_print(char const *fmt, ...)
{
  int32_t len;
  va_list args;
  va_start(args, fmt);
  char trace_buf[500];

  len = vsnprintf(trace_buf, sizeof(trace_buf), fmt, args);

  return tfm_hal_output_spm_log(trace_buf, len);
}

/**
  * @brief   se_st_to_psa_error
  *          translate SE_API_Status_t error code to psa_status_t
  *
  * @param   ret[in] : SE_API_Status_t error code from SE_API function
  * @retval  psa_status_t translation
  */
psa_status_t se_st_to_psa_error(SE_API_Status_t  ret)
{
  switch (ret)
  {
    case SE_OK:
      return PSA_SUCCESS;
    case SE_INVALID_PARAMS_ERROR:
    case SE_INVALID_STATE_ERROR:
      return PSA_ERROR_INVALID_ARGUMENT;
    case SE_COMMAND_ERROR:
      return PSA_ERROR_COMMUNICATION_FAILURE;
    case SE_BUFFER_TOO_SHORT_ERROR:
      return PSA_ERROR_BUFFER_TOO_SMALL;
    case SE_INVALID_SIG_ERROR:
      return PSA_ERROR_INVALID_SIGNATURE;
    default:
      return PSA_ERROR_HARDWARE_FAILURE;
  }
}

/**
  * @brief   register a key in its operational domain(s)
  *
  * @param   key[in]  : Identifier of the key to use for the operation.
  * @param   attr[in] : The attributes for the key.
  *
  * @retval  #PSA_SUCCESS
  *          A key slot (or several slots) are created.
  *          Not executed if the key is already registered.
  */
static psa_status_t register_key_in_all_domain(mbedtls_svc_key_id_t key,
                                               psa_key_attributes_t *attr) {
  psa_status_t psa_status = PSA_ERROR_GENERIC_ERROR;
  psa_key_handle_t key_handle;
  
  /* check if key is already registered */
  psa_status = psa_open_key(key, &key_handle);
  if (psa_status != PSA_SUCCESS) {
    psa_set_key_id(attr, key);
    psa_status = mbedtls_psa_register_se_key(attr);
    if (psa_status != PSA_SUCCESS) {
      goto exit;
    }
  }
  else
  {
    psa_close_key(key_handle);
    psa_status = PSA_SUCCESS;
  }

  if ((key.key_id == SE_ST_ID_TO_PSA_ID(SE_ST_PRIV_SLOT_0))
       || (key.key_id == SE_ST_ID_TO_PSA_ID(SE_ST_SERIAL_NUMBER))) {
    /* register to owner Initial Attestion */
    key.owner = TFM_SP_INITIAL_ATTESTATION;

    psa_status = psa_open_key(key, &key_handle);
    if (psa_status != PSA_SUCCESS) {
      psa_set_key_id(attr, key);
      psa_status = mbedtls_psa_register_se_key(attr);
    }
    else
    {
      psa_close_key(key_handle);
      psa_status = PSA_SUCCESS;
    }
  }

exit:
  return psa_status;
}

/**
  * @brief   register a function key with its corresponding attributes.
  *          This type of key is not physically in the Secure Element but
  *          represents the structure of physical element(s).
  *
  * @param   key[in]  : Identifier of the key to use for the operation.
  * @param   attr[in] : The attributes for the key.
  *
  * @retval  #PSA_SUCCESS
  *          Key is imported and usable.
  *          Its usage can be from all applications but also extended to
  *          specific partition(s).
  */
static psa_status_t register_function_key(psa_key_id_t key_id,
                                          psa_key_attributes_t *attr)
{
    mbedtls_svc_key_id_t mbedtls_id = {0,tfm_nspm_get_current_client_id()};
    /* attribute for function keys */
    psa_set_key_bits(attr, 8); /* logical asset */
    psa_set_key_lifetime(attr, PSA_SE_ST_LIFETIME_READ_ONLY);
    psa_set_key_usage_flags(attr, PSA_KEY_USAGE_EXPORT);
    psa_set_key_type(attr, PSA_KEY_TYPE_RAW_DATA);

    mbedtls_id.key_id = key_id,

    psa_set_key_slot_number(attr, PSA_ID_TO_SE_ST_ID(key_id));

    return register_key_in_all_domain(mbedtls_id, attr);
}

/**
  * @brief   register a function key to deal with the unique serial number
  *          identifier of in the Secure Element.
  *
  * @retval  #PSA_SUCCESS
  *          Key is created.
  */
static psa_status_t register_se_serial_number_key(void)
{
    psa_key_attributes_t attributes = psa_key_attributes_init();

    return register_function_key( SE_ST_ID_TO_PSA_ID(SE_ST_SERIAL_NUMBER),
                                  &attributes);
}

/**
  * @brief   register private keys
  *          They are embedded in different slots of the Secure Element :
  *          - slot0   : Pre-provisioned, non-volatile data (unerasable)
  *          - slot1   : non-volatile data (unerasable, writable))
  *          - slotFF  : volatile data
  *
  * @retval  #PSA_SUCCESS
  *          Keys are created.
  */
static psa_status_t register_se_private_keys(void)
{
    psa_key_attributes_t attributes = psa_key_attributes_init();
    mbedtls_svc_key_id_t mbedtls_id = {0,tfm_nspm_get_current_client_id()};
    psa_status_t psa_status = PSA_ERROR_GENERIC_ERROR;
    uint8_t data[SE_ST_DATA];
    size_t data_size = 0;
    psa_key_handle_t key_handle;

    psa_status = register_function_key( SE_ST_ID_TO_PSA_ID(SE_ST_KEY_MAPPING),
                                        &attributes);
    if (psa_status != PSA_SUCCESS) {
        goto exit;
    }

    /* open Key mapping */
    mbedtls_id.key_id = SE_ST_ID_TO_PSA_ID(SE_ST_KEY_MAPPING);

    attributes = psa_key_attributes_init();

    /* open Key mapping */
    psa_status = psa_open_key(mbedtls_id, &key_handle);
    if (psa_status != PSA_SUCCESS) {
        goto exit;
    }

    psa_status = psa_export_key(key_handle, data, sizeof(data), &data_size);
    if (psa_status == PSA_SUCCESS) {
        psa_se_st_key_map_t map;

        map.nb_key = *((uint16_t *)data);
        if (map.nb_key > SE_ST_PRIV_MAX_SLOT) {
            psa_close_key(key_handle);
            goto exit;
        }

        map.keys = (psa_se_st_key_attribute_t*)(data + 2);

        /* open key 0 */
        /* set structure from mapping */
        mbedtls_id.key_id = map.keys[0].id;
        psa_set_key_slot_number(&attributes, SE_ST_PRIV_SLOT_0);
        psa_set_key_lifetime(&attributes, map.keys[0].lifetime);
        psa_set_key_bits(&attributes, map.keys[0].bits);
        psa_set_key_usage_flags(&attributes, map.keys[0].usage);
        psa_set_key_type(&attributes, map.keys[0].type);
        psa_set_key_algorithm(&attributes, map.keys[0].alg);

        /* register the key */
        psa_status = register_key_in_all_domain(mbedtls_id, &attributes);
        if (psa_status != PSA_SUCCESS) {
            psa_close_key(key_handle);
            goto exit;
        }

        /* open key 1 */
        /* set structure from mapping */
        if (map.nb_key > 1)
        {
          mbedtls_id.key_id = map.keys[1].id;
          psa_set_key_slot_number(&attributes, SE_ST_PRIV_SLOT_1);
          psa_set_key_lifetime(&attributes, map.keys[1].lifetime);
          psa_set_key_bits(&attributes, map.keys[1].bits);
          psa_set_key_usage_flags(&attributes, map.keys[1].usage);
          psa_set_key_type(&attributes, map.keys[1].type);
          psa_set_key_algorithm(&attributes, map.keys[1].alg);

          /* register the key */
          psa_status = register_key_in_all_domain(mbedtls_id, &attributes);
          if (psa_status != PSA_SUCCESS) {
            psa_close_key(key_handle);
            goto exit;
          }
        }
    }
    psa_status = psa_close_key(key_handle);
    if (psa_status != PSA_SUCCESS) {
       goto exit;
    }

exit:
  return psa_status;
}

/**
  * @brief   register keys to deal with memory regions of the Secure Element
  *
  * @retval  #PSA_SUCCESS
  *          Keys are created.
  */
static psa_status_t register_se_memory_keys(void)
{
    psa_key_attributes_t attributes = psa_key_attributes_init();
    mbedtls_svc_key_id_t mbedtls_id = {0,tfm_nspm_get_current_client_id()};
    psa_status_t psa_status = PSA_ERROR_GENERIC_ERROR;
    uint8_t data[SE_ST_DATA];
    size_t data_size = 0;
    psa_key_handle_t key_handle;
    psa_se_st_mapping_attribute_t *regions;

    psa_status = register_function_key( SE_ST_ID_TO_PSA_ID(SE_ST_MAPPING),
                                        &attributes);
    if (psa_status != PSA_SUCCESS) {
        goto exit;
    }

    attributes = psa_key_attributes_init();
    {
      mbedtls_id.key_id = SE_ST_ID_TO_PSA_ID(SE_ST_MAPPING);
      psa_status = psa_open_key(mbedtls_id, &key_handle);
      if (psa_status != PSA_SUCCESS) {
          goto exit;
      }

      psa_status = psa_export_key(key_handle, data, sizeof(data), &data_size);
      if (psa_status == PSA_SUCCESS) {
          regions = (psa_se_st_mapping_attribute_t*)&data[2];
          for (int i = 0; i < *((uint16_t*)data); i++)
          {
              /* set structure from mapping */
              mbedtls_id.key_id = regions[i].id;
              psa_set_key_slot_number(&attributes, SE_ST_START_REGION + i);
              psa_set_key_lifetime(&attributes, regions[i].lifetime);
              psa_set_key_bits(&attributes, regions[i].bits);
              psa_set_key_usage_flags(&attributes, PSA_KEY_USAGE_EXPORT);
              psa_set_key_type(&attributes, PSA_KEY_TYPE_RAW_DATA);

              /* register the key */
              psa_status = register_key_in_all_domain(mbedtls_id, &attributes);
              if (psa_status != PSA_SUCCESS) {
                  psa_close_key(key_handle);
                  goto exit;
              }
          }
      }

      psa_status = psa_close_key(key_handle);
      if (psa_status != PSA_SUCCESS) {
          goto exit;
      }
    }

exit:
  return psa_status;
}

/**
  * @brief   register a key set of the Secure Element
  *
  * @retval  #PSA_SUCCESS
  *          Key set is created and subcribed in the TFM.
  */
psa_status_t register_se_keys(void)
{
psa_status_t psa_status = PSA_SUCCESS;

    if ((psa_status = register_se_serial_number_key()) != PSA_SUCCESS)
        return psa_status;
    if ((psa_status = register_se_private_keys()) != PSA_SUCCESS)
        return psa_status;
    if ((psa_status = register_se_memory_keys()) != PSA_SUCCESS)
        return psa_status;
    return psa_status;
}

/**
  * @brief   psa_se_st_init
  *          Init of PSA drv SE
  * @note    Start SE_API and load PSA driver persistent data for public key storage
  *
  * @param   drv_context : psa_drv_se_context_t structure to keep persistent data and transient data
  * @param   persistent_data : init data provided by psa drv se mechanism.
  * @param   location : location of the init call
  * @retval  PSA_SUCCESS if success, an error code otherwise
  */
static psa_status_t psa_se_st_init(psa_drv_se_context_t *drv_context,
                                   void *persistent_data,
                                   psa_key_location_t location)
{
  int32_t ret;
  SE_psa_drv_se_persistent_t *p_data = (SE_psa_drv_se_persistent_t *)persistent_data;

  /* Not used by SE_API */
  if (drv_context == NULL)
  {
    return PSA_ERROR_INVALID_ARGUMENT;
  }

  /* check PSA key location */
  if (location != PSA_SE_ST_LOCATION)
  {
    SE_ST_PSA_PRINT("Invalid driver\n");
    return PSA_ERROR_GENERIC_ERROR;
  }

  /* uncomment this line to enable debug in SE STSAFEA API */
  /*SE_SetDebug(debug_print);*/

  /* Driver initialization with a security level in the communication channel */
#ifdef USE_PAIRING
  ret = SE_Init(0, SE_SECURITY_LEVEL_R_PAIRING, 0x20);
#else
  ret = SE_Init(0, SE_SECURITY_LEVEL_NONE, 0x20);
#endif
  if (ret != SE_OK)
  {
    SE_ST_PSA_PRINT("Secure Element driver initialization failure\n");
    return PSA_ERROR_HARDWARE_FAILURE;
  }

  /* recover keys from persistent data and load them into driver */
  ret = SE_Init_PubKeys(p_data->pub_keys, 2);
  if (ret != SE_OK)
  {
    return PSA_ERROR_GENERIC_ERROR;
  }

  /* Get the secure storage size (regions number) */
  if (p_data->map_size == 0)
  {
    ret = SE_GetMapping(NULL, &(p_data->map_size));
    if (ret != SE_OK)
    {
      return PSA_ERROR_GENERIC_ERROR;
    }
  }

  SE_ST_PSA_PRINT("Secure Element driver initialized\n");
  return PSA_SUCCESS;
}

/**
  * @brief   psa_se_st_validate_slot_number_fct
  *          Validate slot number for psa drv se according to key attributes
  *
  * @param   drv_context : psa_drv_se_context_t structure to keep persistent data and transient data
  * @param   persistent_data : init data provided by psa drv se mechanism.
  * @param   attributes : key attributes to map on STSAFE-A ressources
  * @param   method : key creation context supported values
  *                   PSA_KEY_CREATION_REGISTER : persistent key declaration
  *                   PSA_KEY_CREATION_IMPORT : persistent raw key (memory region)
  *                   PSA_KEY_CREATION_GENERATE : persistent key generation on slot 1 or slot ff
  * @param   key_slot : internal key index
  * @retval  PSA_SUCCESS if success, an error code otherwise
  */
static psa_status_t psa_se_st_validate_slot_number_fct(
  psa_drv_se_context_t *drv_context,
  void *persistent_data,
  const psa_key_attributes_t *attributes,
  psa_key_creation_method_t method,
  psa_key_slot_number_t key_slot)
{
  psa_key_type_t type = psa_get_key_type(attributes);
  SE_psa_drv_se_persistent_t *p_data = (SE_psa_drv_se_persistent_t *)(persistent_data);

  /* support registration and import only for memory region */
  if ((method != PSA_KEY_CREATION_REGISTER) &&
      (method != PSA_KEY_CREATION_IMPORT)   &&
      (method != PSA_KEY_CREATION_GENERATE))
  {
    SE_ST_PSA_PRINT("Key slot validation cannot operate with method : %x\n", method);
    return PSA_ERROR_NOT_SUPPORTED;
  }
  /* if creation is import, limit to RAW_DATA for memory region access */
  if (method == PSA_KEY_CREATION_IMPORT)
  {
    if (type != PSA_KEY_TYPE_RAW_DATA)
    {
      SE_ST_PSA_PRINT("This type cannot be imported  : %d\n", type);
      return PSA_ERROR_NOT_SUPPORTED;
    }
  }
  /* check key attributes for private key and public key */
  if ((PSA_KEY_TYPE_ECC_GET_FAMILY(type) != PSA_ECC_FAMILY_SECP_R1) &&
      (PSA_KEY_TYPE_ECC_GET_FAMILY(type) != PSA_ECC_FAMILY_BRAINPOOL_P_R1) &&
      (type != PSA_KEY_TYPE_RAW_DATA))
  {
    SE_ST_PSA_PRINT("Invalid key attributes\n");
    return PSA_ERROR_NOT_SUPPORTED;
  }
  /* if type = PSA_KEY_TYPE_RAW_DATA check if key_slot is in the region range */
  if (type == PSA_KEY_TYPE_RAW_DATA)
  {
    if ((key_slot < SE_ST_START_REGION) || (key_slot > (SE_ST_START_REGION + p_data->map_size)))
    {
      if ((key_slot != SE_ST_SERIAL_NUMBER) &&
          (key_slot != SE_ST_MAPPING)       &&
          (key_slot != SE_ST_KEY_MAPPING))
      {
        return PSA_ERROR_INVALID_ARGUMENT;
      }
    }
  }
  else
  {
    /* if type = PSA_KEY_TYPE_ECC check if key_slot is 0, 1 or 2*/
    if ( (key_slot != SE_ST_PRIV_SLOT_0) &&
         (key_slot != SE_ST_PRIV_SLOT_1) &&
         (key_slot != SE_ST_PRIV_SLOT_FF))
    {
      return PSA_ERROR_INVALID_ARGUMENT;
    }
  }

  /* if already exists do not return error */
  return PSA_SUCCESS;
}

/**
  * @brief   psa_se_st_export_public_fct
  *          export public key from a key pair
  * @note    possible only on SE_ST_PRIV_SLOT_0, SE_ST_PRIV_SLOT_1, SE_ST_PRIV_SLOT_FF
  *
  * @param   drv_context : psa_drv_se_context_t structure to keep persistent data and transient data
  * @param   key : internal key index
  * @param   p_data : output buffer for public key data in non compressed mode
  *                   format is 0x04 + X + Y
  * @param   data_size : size of p_data buffer
  * @param   p_data_length : actual size of outputed public key
  * @retval  PSA_SUCCESS if success, an error code otherwise
  */
static psa_status_t psa_se_st_export_public_fct(psa_drv_se_context_t *drv_context,
                                                psa_key_slot_number_t key,
                                                uint8_t *p_data,
                                                size_t data_size,
                                                size_t *p_data_length)
{
  size_t key_size = 0;
  SE_Slot_t slot;
  SE_Key_Type_t type;
  int32_t ret;

  switch (key)
  {
    case 0 :
      slot = SE_SLOT_PREPROVISIONNED;
      break;
    case 1 :
      slot = SE_SLOT_PERMANENT;
      break;
    case 2 :
      slot = SE_SLOT_EPHEMERAL;
      break;
    default :
      return PSA_ERROR_DOES_NOT_EXIST;
  }

  ret = SE_Get_PubKey(slot, NULL, NULL, &key_size, &type);
  if ((ret != SE_OK) && (ret != SE_BUFFER_TOO_SHORT_ERROR))
  {
    return PSA_ERROR_HARDWARE_FAILURE;
  }

  if (data_size < (1 + key_size * 2))
  {
    return PSA_ERROR_BUFFER_TOO_SMALL;
  }

 /* Prefix for uncompressed public key format
  * Key is made up of a 0x4 byte and two coordinates
  * 0x04 || X_COORD || Y_COORD
  */
  p_data[0] = 0x4;

  ret = SE_Get_PubKey(slot, &p_data[1], &p_data[1 + key_size], &key_size, &type);
  if (ret != SE_OK)
  {
    return PSA_ERROR_HARDWARE_FAILURE;
  }

  if (p_data_length != NULL)
  {
    *p_data_length = 1 + key_size * 2;
  }
  return PSA_SUCCESS;
}


/**
  * @brief   psa_se_st_gen_key_fct
  *          generate key pair
  * @note    possible only on SE_ST_PRIV_SLOT_1, SE_ST_PRIV_SLOT_FF
  *
  * @param   drv_context : psa_drv_se_context_t structure to keep persistent data and transient data
  * @param   key_slot : internal key index
  * @param   attributes : key attributes to describe EC curve to use and size
  * @param   pubkey : pubkey output buffer (can be NULL is public key is not needed)
  * @param   pubkey_size : size of pubkey buffer
  * @param   pubkey_length : actual size of outputed public key
  * @retval  PSA_SUCCESS if success, an error code otherwise
  */
static psa_status_t psa_se_st_gen_key_fct(
  psa_drv_se_context_t *drv_context,
  psa_key_slot_number_t key_slot,
  const psa_key_attributes_t *attributes,
  uint8_t *pubkey, size_t pubkey_size, size_t *pubkey_length)
{
  const uint16_t l_key = key_slot;
  psa_status_t status = PSA_ERROR_GENERIC_ERROR;
  psa_key_type_t type = psa_get_key_type(attributes);
  size_t bits = psa_get_key_bits(attributes);
  SE_psa_drv_se_persistent_t *p_data = (SE_psa_drv_se_persistent_t *)(drv_context->persistent_data);
  SE_Key_Type_t curve;
  uint32_t key_size;
  uint8_t *pub_x;
  uint8_t *pub_y;
  SE_Slot_t slot;

  switch (PSA_KEY_TYPE_ECC_GET_FAMILY(type))
  {
    case PSA_ECC_FAMILY_SECP_R1 :
      if (bits == 256)
      {
        curve = SE_KEY_TYPE_NIST_256;
        key_size = 32;
        break;
      }
      if (bits == 384)
      {
        curve = SE_KEY_TYPE_NIST_384;
        key_size = 48;
        break;
      }
      return PSA_ERROR_NOT_SUPPORTED;
    case PSA_ECC_FAMILY_BRAINPOOL_P_R1 :
      if (bits == 256)
      {
        curve = SE_KEY_TYPE_BP_256;
        key_size = 32;
        break;
      }
      if (bits == 384)
      {
        curve = SE_KEY_TYPE_BP_384;
        key_size = 48;
        break;
      }
      return PSA_ERROR_NOT_SUPPORTED;
    default :
      return PSA_ERROR_NOT_SUPPORTED;
  }

  if (pubkey != NULL && pubkey_size < 1 + ((bits / 8) * 2))
  {
    return PSA_ERROR_BUFFER_TOO_SMALL;
  }

  if ((l_key != SE_ST_PRIV_SLOT_1) && (l_key != SE_ST_PRIV_SLOT_FF))
  {
    return PSA_ERROR_INVALID_ARGUMENT;
  }
  if (l_key == SE_ST_PRIV_SLOT_1)
  {
    slot = SE_SLOT_PERMANENT;
  }
  if (l_key == SE_ST_PRIV_SLOT_FF)
  {
    slot = SE_SLOT_EPHEMERAL;
  }
  /* save generated key in persistent data but update done on key allocat/destroy only */
  if (l_key == SE_ST_PRIV_SLOT_1)
  {
    pub_x = p_data->pub_keys[0].public_key_x;
    pub_y = p_data->pub_keys[0].public_key_y;
    p_data->pub_keys[0].public_key_curve = curve;
  }
  if (l_key == SE_ST_PRIV_SLOT_FF)
  {
    pub_x = p_data->pub_keys[1].public_key_x;
    pub_y = p_data->pub_keys[1].public_key_y;
    p_data->pub_keys[1].public_key_curve = curve;
  }
  ASSERT_SUCCESS(SE_GenerateKeyPair(slot, curve, pub_x, pub_y));

  if (pubkey != NULL)
  {
    pubkey[0] = 0x4;
    memcpy(&pubkey[1], pub_x, key_size);
    memcpy(&pubkey[1 + key_size], pub_y, key_size);
  }
  if (pubkey_length != NULL)
  {
    *pubkey_length = 1 + key_size * 2;
  }
exit:
  return status;
}

/**
  * @brief   psa_se_st_import_fct
  *          import raw data in STSAFE-A memory region
  * @note    possible only on key >= SE_ST_START_REGION and psa key type PSA_KEY_TYPE_RAW_DATA
  *
  * @param   drv_context : psa_drv_se_context_t structure to keep persistent data and transient data
  * @param   key_slot : internal key index
  * @param   attributes : key attributes for the memory region
  * @param   data : data buffer to write in STSAFE-A memory
  * @param   data_length : size of data to import
  * @param   bits : actual size of imported data in bits
  * @retval  PSA_SUCCESS if success, an error code otherwise
  */
psa_status_t psa_se_st_import(
  psa_drv_se_context_t *drv_context,
  psa_key_slot_number_t key_slot,
  const psa_key_attributes_t *attributes,
  const uint8_t *data,
  size_t data_length,
  size_t *bits)
{
  psa_status_t status = PSA_ERROR_GENERIC_ERROR;
  psa_key_type_t type = psa_get_key_type(attributes);
  SE_psa_drv_se_persistent_t *p_data = (SE_psa_drv_se_persistent_t *)(drv_context->persistent_data);
  const uint16_t region = key_slot - SE_ST_START_REGION;

  /* type shall be RAW_DATA */
  if (type != PSA_KEY_TYPE_RAW_DATA)
  {
    return PSA_ERROR_NOT_SUPPORTED;
  }

  /* import only works in memory region */
  if ((key_slot < SE_ST_START_REGION) || (key_slot > (SE_ST_START_REGION + p_data->map_size)))
  {
    return PSA_ERROR_INVALID_ARGUMENT;
  }

  ASSERT_SUCCESS(SE_Update(region, 0, data_length, (uint8_t *)data));

  if (bits != NULL)
  {
    *bits = 8 * data_length;
  }

exit:
  return status;
}



/**
  * @brief   psa_se_st_export_fct
  *          export raw data in STSAFE-A memory region
  * @note    possible only on key >= SE_ST_START_REGION and psa key type PSA_KEY_TYPE_RAW_DATA
  *
  * @param   drv_context : psa_drv_se_context_t structure to keep persistent data and transient data
  * @param   key_slot : internal key index
  * @param   attributes : key attributes for the memory region
  * @param   data : data buffer to read from STSAFE-A memory
  * @param   data_size : requested size of data to export
  * @param   p_data_length : actual size of exported data
  * @retval  PSA_SUCCESS if success, an error code otherwise
  */
psa_status_t psa_se_st_export(psa_drv_se_context_t *drv_context,
                              psa_key_slot_number_t key,
                              uint8_t *data,
                              size_t data_size,
                              size_t *p_data_length)
{
  const uint16_t region = key - SE_ST_START_REGION;
  const uint16_t slot = key;
  psa_status_t status = PSA_ERROR_GENERIC_ERROR;
  SE_API_Status_t ret;

  switch (slot)
  {
    case SE_ST_SERIAL_NUMBER :
    {
      if (data_size < 9)
      {
        SE_ST_PSA_PRINT("Serial Number from slot %d cannot be set in a buffer of size : %d\n", slot, data_size);
        return PSA_ERROR_BUFFER_TOO_SMALL;
      }

      if (data_size >= 19)
      {
        ret = SE_GetSerial(data, SE_SERIAL_FORMAT_HEX);
        if (p_data_length != NULL)
        {
          *p_data_length = 19;
        }

      }
      else
      {
        ret = SE_GetSerial(data, SE_SERIAL_FORMAT_RAW);
        if (p_data_length != NULL)
        {
          *p_data_length = 9;
        }
      }
      if (ret != SE_OK)
      {
        SE_ST_PSA_PRINT("Serial Number from slot %d cannot be recovered\n", slot);
        return PSA_ERROR_HARDWARE_FAILURE;
      }
      return PSA_SUCCESS;
    }
    case SE_ST_MAPPING :
    {
      psa_se_st_mapping_attribute_t *map;
      size_t map_size = SE_ST_DATA;
      uint32_t nb_region = 0;

      ret = SE_GetMapping(NULL, &nb_region);
      if (ret == SE_OK)
      {
        if (nb_region > SE_ST_MAX_REGION)
        {
          SE_ST_PSA_PRINT("Not enough memory for processing up to %u regions\n", nb_region);
          return PSA_ERROR_INVALID_ARGUMENT;
        }

        if (map_size > data_size)
        {
          SE_ST_PSA_PRINT("slot %d cannot be set in a buffer of size : %d\n", slot, data_size);
          return PSA_ERROR_BUFFER_TOO_SMALL;
        }
        map  = (psa_se_st_mapping_attribute_t *)&data[2];
        memset(map, 0, map_size - 2);
        *((uint16_t *)data) = (uint16_t)(nb_region & 0xFFFF);
        SE_Region_t mapping[SE_ST_MAX_REGION];

        ret = SE_GetMapping(mapping, &nb_region);
        if (ret != SE_OK)
        {
          SE_ST_PSA_PRINT("Mapping cannot be read\n");
          return PSA_ERROR_HARDWARE_FAILURE;
        }
        for (int32_t i = 0; i < nb_region; i++)
        {
          map[i].bits = (mapping[i].size + (((mapping[i].type & 0x1) == 1) ? 4 : 0)) * 8;
          map[i].id = SE_ST_ID_TO_PSA_ID(SE_ST_MEMORY_REGION_ID(i));
          map[i].lifetime = PSA_KEY_LIFETIME_FROM_PERSISTENCE_AND_LOCATION(
                              (mapping[i].write_ac > 2) ? PSA_KEY_PERSISTENCE_READ_ONLY : PSA_KEY_PERSISTENCE_DEFAULT,
                              PSA_SE_ST_LOCATION);
        }
        if (p_data_length != NULL)
        {
          *p_data_length = map_size;
        }
        return PSA_SUCCESS;
      }
      SE_ST_PSA_PRINT("Region number cannot be obtained\n");
      return PSA_ERROR_HARDWARE_FAILURE;
    }
    case SE_ST_KEY_MAPPING :
    {
      /* key map is defined by STSAFE-A110 capacity */
      size_t nb_key = 1;
      size_t key_map;
      psa_se_st_key_attribute_t *keys = (psa_se_st_key_attribute_t *)&data[2];
      SE_Key_Type_t curve;
      SE_API_Status_t ret;
      size_t key_size;

      ret = SE_Get_PubKey(SE_SLOT_PERMANENT, NULL, NULL, &key_size, &curve);
      if ((ret == SE_OK) || (ret == SE_BUFFER_TOO_SHORT_ERROR))
      {
        if (key_size != 0)
        {
          nb_key = 2;
        }
      }

      key_map = sizeof(psa_se_st_key_attribute_t) * nb_key + 2;

      if (data_size < key_map)
      {
        SE_ST_PSA_PRINT("slot %d cannot be set in a buffer of size : %d\n", slot, data_size);
        return PSA_ERROR_BUFFER_TOO_SMALL;
      }

      memset(keys, 0, key_map - 2);
      *((uint16_t *)data) = nb_key;

      /* key 1 is presistent and may be present */
      if ( nb_key == 2)
      {
        keys[1].alg = PSA_ALG_ECDSA(PSA_ALG_ANY_HASH);
        keys[1].lifetime = PSA_SE_ST_LIFETIME_DEFAULT;
        keys[1].usage = PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_EXPORT;
        keys[1].id = SE_ST_ID_TO_PSA_ID(SE_ST_PRIV_SLOT_1);
        keys[1].bits = key_size * 8;
        keys[1].type = PSA_KEY_TYPE_ECC_KEY_PAIR(SE_TO_PSA_CURVE(curve));
      }

      /* key 0 is always present */
      ret = SE_Get_PubKey(SE_SLOT_PREPROVISIONNED, NULL, NULL, &key_size, &curve);
      if ((ret == SE_OK) || (ret == SE_BUFFER_TOO_SHORT_ERROR))
      {
        keys[0].alg = PSA_ALG_ECDSA(PSA_ALG_ANY_HASH);
        keys[0].lifetime = PSA_SE_ST_LIFETIME_READ_ONLY;
        keys[0].usage = PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_EXPORT;
        keys[0].id = SE_ST_ID_TO_PSA_ID(SE_ST_PRIV_SLOT_0);
        keys[0].bits = key_size * 8;
        keys[0].type = PSA_KEY_TYPE_ECC_KEY_PAIR(SE_TO_PSA_CURVE(curve));
      }
      else
      {
        SE_ST_PSA_PRINT("Preprovisionned slot cannot be read\n");
        return PSA_ERROR_HARDWARE_FAILURE;
      }

      if (p_data_length != NULL)
      {
        *p_data_length = key_map;
      }
      return PSA_SUCCESS;
    }
    default :
      if (slot >= SE_ST_START_REGION)
      {
        uint16_t c_data_len = data_size;
        ret = SE_MemoryCheck(region, 0, &c_data_len);
        if ((ret != SE_OK) || (ret != SE_PARAMS_UPDATED_ERROR))
        {
          ASSERT_SUCCESS(SE_Read(region, 0, c_data_len, data));
          if (p_data_length != NULL)
          {
            *p_data_length = c_data_len;
          }
          return PSA_SUCCESS;
        }
      }
  }

exit:
  return status;
}

/**
  * @brief   psa_se_st_allocate_key
  *          undescribe allocation of SE ressources
  *
  * @param   drv_context : psa_drv_se_context_t structure to keep persistent data and transient data
  * @param   persistent_data : updatable persistent data for psa drv se
  * @param   attributes : key attributes to allocate
  * @param   method : method for key creation. only supported
  *                          PSA_KEY_CREATION_IMPORT : for memory region
  *                          PSA_KEY_CREATION_GENERATE : for private key generation
  * @param   key_slot : selected internal key slot
  * @retval  PSA_SUCCESS if success, an error code otherwise
  */
psa_status_t psa_se_st_allocate_key(
  psa_drv_se_context_t *drv_context,
  void *persistent_data,
  const psa_key_attributes_t *attributes,
  psa_key_creation_method_t method,
  psa_key_slot_number_t *key_slot)
{
  SE_psa_drv_se_persistent_t *p_data = (SE_psa_drv_se_persistent_t *)persistent_data;
  psa_key_type_t type = psa_get_key_type(attributes);

  /* check method : create a key for generate on private slot or a key for import on memory region */
  if ((method != PSA_KEY_CREATION_IMPORT) &&
      (method != PSA_KEY_CREATION_GENERATE))
  {
    SE_ST_PSA_PRINT("Key allocation cannot operate with method : %x\n", method);
    return PSA_ERROR_NOT_SUPPORTED;
  }
  if (key_slot == NULL)
  {
    return PSA_ERROR_INVALID_ARGUMENT;
  }
  /* check type RAW_DATA then allocation a memory region */
  if (type == PSA_KEY_TYPE_RAW_DATA)
  {
    /* get ID from attributes */
    mbedtls_svc_key_id_t id = psa_get_key_id(attributes);
    uint32_t region = PSA_ID_TO_SE_ST_ID(id.key_id);

    if (region >= SE_ST_START_REGION)
    {
      if ((region - SE_ST_START_REGION) > p_data->map_size)
      {
        SE_ST_PSA_PRINT("Region %d out of range\n", region);
        return PSA_ERROR_INVALID_ARGUMENT;
      }
      *key_slot = region;

      return PSA_SUCCESS;
    }
    SE_ST_PSA_PRINT("Region %d under the range\n", region);
    return PSA_ERROR_INVALID_ARGUMENT;
  }

  if ((PSA_KEY_TYPE_ECC_GET_FAMILY(type) == PSA_ECC_FAMILY_SECP_R1)
      || (PSA_KEY_TYPE_ECC_GET_FAMILY(type) == PSA_ECC_FAMILY_BRAINPOOL_P_R1))
    /*  check type ECC then allocate in slot 0 or 1 or FF */
  {
    mbedtls_svc_key_id_t id = psa_get_key_id(attributes);
    uint32_t key_id = PSA_ID_TO_SE_ST_ID(id.key_id);

    /* check if this is slot 1 or slot FF */
    if ((key_id == SE_ST_PRIV_SLOT_1) || (key_id == SE_ST_PRIV_SLOT_FF))
    {
      *key_slot = key_id;

      return PSA_SUCCESS;
    }
    return PSA_ERROR_INSUFFICIENT_STORAGE;
  }

  return PSA_ERROR_NOT_SUPPORTED;
}


/**
  * @brief   psa_se_st_destroy_key
  *          key deallocation of SE ressources
  *
  * @param   drv_context : psa_drv_se_context_t structure to keep persistent data and transient data
  * @param   persistent_data : updatable persistent data for psa drv se
  * @param   key_slot : internal key slot to free
  * @retval  PSA_SUCCESS if success, an error code otherwise
  */
psa_status_t psa_se_st_destroy_key(
  psa_drv_se_context_t *drv_context,
  void *persistent_data,
  psa_key_slot_number_t key_slot)
{
  return PSA_SUCCESS;
}



/**
  * @brief   psa_se_st_sign_fct
  *          Asymmetric signature generation
  *
  * @param   drv_context : psa_drv_se_context_t structure to keep persistent data and transient data
  * @param   key_slot : internal key slot
  * @param   alg : expected signature scheme (only PSA_ALG_ECDSA() supported)
  * @param   p_hash : hash buffer
  * @param   hash_length : size of hash buffer
  * @param   p_signature : signature buffer for RAW R and S output
  * @param   signature_size : size of signature buffer
  * @param   p_signature_length : actual size of outputed signature
  * @retval  PSA_SUCCESS if success, an error code otherwise
  */
static psa_status_t psa_se_st_sign_fct(psa_drv_se_context_t *drv_context,
                                       psa_key_slot_number_t key_slot,
                                       psa_algorithm_t alg,
                                       const uint8_t *p_hash,
                                       size_t hash_length,
                                       uint8_t *p_signature,
                                       size_t signature_size,
                                       size_t *p_signature_length)
{
  psa_status_t status = PSA_ERROR_GENERIC_ERROR;
  uint32_t key_size = 0;
  SE_Slot_t slot;

  if ((key_slot != SE_ST_PRIV_SLOT_0) && (key_slot != SE_ST_PRIV_SLOT_1))
  {
    return PSA_ERROR_NOT_SUPPORTED;
  }

  if (key_slot == SE_ST_PRIV_SLOT_0)
  {
    slot = SE_SLOT_PREPROVISIONNED;
  }
  if (key_slot == SE_ST_PRIV_SLOT_1)
  {
    slot = SE_SLOT_PERMANENT;
  }

  key_size = SE_GetKeySize(key_slot) / 8;
  if (key_size == 0)
  {
    return PSA_ERROR_HARDWARE_FAILURE;
  }

  if (signature_size < key_size * 2)
  {
    return PSA_ERROR_BUFFER_TOO_SMALL;
  }

  *p_signature_length = key_size * 2;

  ASSERT_SUCCESS(SE_GenerateSignature(slot, p_hash, (uint16_t)hash_length, p_signature, p_signature + key_size));

exit:
  return status;
}


/**
  * @brief   psa_se_st_verify_fct
  *          Asymmetric signature verification
  * @note    not implemented yet
  *
  * @param   drv_context : psa_drv_se_context_t structure to keep persistent data and transient data
  * @param   key_slot : internal key slot
  * @param   alg : expected signature scheme (only PSA_ALG_ECDSA() supported)
  * @param   p_hash : hash buffer
  * @param   hash_length : size of hash buffer
  * @param   p_signature : signature buffer for RAW R and S output
  * @param   signature_size : size of signature buffer
  * @param   p_signature_length : actual size of outputed signature
  * @retval  PSA_SUCCESS if success, an error code otherwise
  */
static psa_status_t psa_se_st_verify_fct(psa_drv_se_context_t *drv_context,
                                         psa_key_slot_number_t key_slot,
                                         psa_algorithm_t alg,
                                         const uint8_t *p_hash,
                                         size_t hash_length,
                                         const uint8_t *p_signature,
                                         size_t signature_length)
{
  return PSA_SUCCESS;
}



psa_drv_se_key_management_t psa_se_st_key_management_fct =
{
  /* first step to allocate a new private key */
  .p_allocate = psa_se_st_allocate_key,
  .p_destroy = psa_se_st_destroy_key,
  /* use to address memory region read */
  .p_export = psa_se_st_export,
  /* export public key for the 3 first sectors */
  .p_export_public = psa_se_st_export_public_fct,
  /* not use for ECDHE as not implemented yet by MbedCrypto */
  .p_generate = psa_se_st_gen_key_fct,
  /* use to address memory region update */
  .p_import = psa_se_st_import,
  /* use to validate the pre provisionned key */
  .p_validate_slot_number = psa_se_st_validate_slot_number_fct,
};


psa_drv_se_key_derivation_t psa_se_st_derivation_fct =
{
  .p_derive = NULL,
  .context_size = 0,
  .p_collateral = NULL,
  .p_export = NULL,
  .p_setup = NULL,
};

psa_drv_se_asymmetric_t psa_se_st_asymmetric_fct =
{
  .p_decrypt = NULL,
  .p_encrypt = NULL,
  .p_sign = psa_se_st_sign_fct,
  .p_verify = psa_se_st_verify_fct,
};

const psa_drv_se_t psa_se_st =
{
  .hal_version = PSA_DRV_SE_HAL_VERSION,
  .aead = NULL,
  .asymmetric = &psa_se_st_asymmetric_fct,
  .cipher = NULL,
  .derivation = &psa_se_st_derivation_fct,
  .key_management = &psa_se_st_key_management_fct,
  .mac = NULL,
  .persistent_data_size = sizeof(SE_psa_drv_se_persistent_t),
  .p_init = psa_se_st_init,
};
#endif /* PSA_USE_SE_ST */

