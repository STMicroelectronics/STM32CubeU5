/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "ps_encrypted_object.h"

#include <stddef.h>

#include "crypto/ps_crypto_interface.h"
#include "psa/internal_trusted_storage.h"
#include "flash_layout.h"
#include "tfm_memory_utils.h"
#include "ps_object_defs.h"
#include "ps_utils.h"

/* Gets the size of data to encrypt */
#define PS_ENCRYPT_SIZE(plaintext_size) \
    ((plaintext_size) + PS_OBJECT_HEADER_SIZE - sizeof(union ps_crypto_t))

#define PS_OBJECT_START_POSITION  0

/* Buffer to store the maximum encrypted object */
/* FIXME: Do partial encrypt/decrypt to reduce the size of internal buffer */
#define PS_MAX_ENCRYPTED_OBJ_SIZE PS_ENCRYPT_SIZE(PS_MAX_OBJECT_DATA_SIZE)

/* FIXME: add the tag length to the crypto buffer size to account for the tag
 * being appended to the ciphertext by the crypto layer.
 */
#define PS_CRYPTO_BUF_LEN (PS_MAX_ENCRYPTED_OBJ_SIZE + PS_TAG_LEN_BYTES)

static uint8_t ps_crypto_buf[PS_CRYPTO_BUF_LEN];


/**
 * \brief Performs authenticated decryption on object data, with the header as
 *        the associated data.
 *
 * \param[in]  fid       File ID
 * \param[in]  cur_size  Size of the object data to decrypt
 * \param[in,out] obj    Pointer to the object structure to authenticate and
 *                       fill in with the decrypted data. The tag of the object
 *                       is the one stored in the object table for the given
 *                       File ID.
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
static psa_status_t ps_object_auth_decrypt(uint32_t fid,
                                           uint32_t cur_size,
                                           struct ps_object_t *obj)
{
    psa_status_t err;
    uint8_t *p_obj_data = (uint8_t *)&obj->header.info;
    size_t out_len;

    err = ps_crypto_setkey();
    if (err != PSA_SUCCESS) {
        return err;
    }

    (void)tfm_memcpy(ps_crypto_buf, p_obj_data, cur_size);

    /* Use File ID as a part of the associated data to authenticate
     * the object in the FS. The tag will be stored in the object table and
     * not as a part of the object's data stored in the FS.
     */

    err = ps_crypto_auth_and_decrypt(&obj->header.crypto,
                                     (const uint8_t *)&fid,
                                     sizeof(fid),
                                     ps_crypto_buf,
                                     cur_size,
                                     p_obj_data,
                                     sizeof(*obj) - sizeof(obj->header.crypto),
                                     &out_len);
    if (err != PSA_SUCCESS || out_len != cur_size) {
        (void)ps_crypto_destroykey();
        return PSA_ERROR_GENERIC_ERROR;
    }

    return ps_crypto_destroykey();
}

/**
 * \brief Performs authenticated encryption on object data, with the header as
 *        the associated data.
 *
 * \param[in]  fid       File ID
 * \param[in]  cur_size  Size of the object data to encrypt
 * \param[out] obj       Pointer to the object structure to authenticate and
 *                       fill in with the encrypted data.
 *
 * \return Returns error code as specified in \ref psa_status_t
 */
static psa_status_t ps_object_auth_encrypt(uint32_t fid,
                                           uint32_t cur_size,
                                           struct ps_object_t *obj)
{
    psa_status_t err;
    uint8_t *p_obj_data = (uint8_t *)&obj->header.info;
    size_t out_len;

    err = ps_crypto_setkey();
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* FIXME: should have an IV per object with key diversification */
    /* Get a new IV for each encryption */
    ps_crypto_get_iv(&obj->header.crypto);

    /* Use File ID as a part of the associated data to authenticate
     * the object in the FS. The tag will be stored in the object table and
     * not as a part of the object's data stored in the FS.
     */

    err = ps_crypto_encrypt_and_tag(&obj->header.crypto,
                                    (const uint8_t *)&fid,
                                    sizeof(fid),
                                    p_obj_data,
                                    cur_size,
                                    ps_crypto_buf,
                                    sizeof(ps_crypto_buf),
                                    &out_len);
    if (err != PSA_SUCCESS || out_len != cur_size) {
        (void)ps_crypto_destroykey();
        return PSA_ERROR_GENERIC_ERROR;
    }

    (void)tfm_memcpy(p_obj_data, ps_crypto_buf, cur_size);

    return ps_crypto_destroykey();
}

psa_status_t ps_encrypted_object_read(uint32_t fid, struct ps_object_t *obj)
{
    psa_status_t err;
    uint32_t decrypt_size;
    size_t data_length;

    /* Read the encrypted object from the the persistent area */
    err = psa_its_get(fid, PS_OBJECT_START_POSITION,
                      PS_MAX_OBJECT_SIZE,
                      (void *)obj->header.crypto.ref.iv,
                      &data_length);
    if (err != PSA_SUCCESS) {
        return err;
    }

    /* Get the decrypt size */
    decrypt_size = data_length - sizeof(obj->header.crypto.ref.iv);

    /* Decrypt the object data */
    err = ps_object_auth_decrypt(fid, decrypt_size, obj);
    if (err != PSA_SUCCESS) {
        return err;
    }

    return PSA_SUCCESS;
}

psa_status_t ps_encrypted_object_write(uint32_t fid, struct ps_object_t *obj)
{
    psa_status_t err;
    uint32_t wrt_size;

    wrt_size = PS_ENCRYPT_SIZE(obj->header.info.current_size);

    wrt_size = GET_ALIGNED_FLASH_BYTES(wrt_size);

    /* Authenticate and encrypt the object */
    err = ps_object_auth_encrypt(fid, wrt_size, obj);
    if (err != PSA_SUCCESS) {
        return err;
    }

    wrt_size += sizeof(obj->header.crypto.ref.iv);

    /* Write the encrypted object to the persistent area. The tag values is not
     * copied as it is stored in the object table.
     */
    return psa_its_set(fid, wrt_size, (const void *)obj->header.crypto.ref.iv,
                       PSA_STORAGE_FLAG_NONE);
}
