/**
  ******************************************************************************
  * @file    tfm_app.c
  * @author  MCD Application Team
  * @brief   TFM application examples module.
  *          This file provides examples of PSA API usages.
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "tfm_app.h"
#include "psa/error.h"
#include "crypto_tests_common.h"
#include "psa/protected_storage.h"
#include "q_useful_buf.h"
#include "psa/initial_attestation.h"
#include "psa/internal_trusted_storage.h"
#include "com.h"


/** @defgroup  TFM_App_Private_Defines Private Defines
  * @{
  */

/* Private define  ---------------------------------------------------------*/
#define TEST_UID      2U
#define TEST_DATA          "TRUSTEDFIRMWARE_FOR_STM32"
#define TEST_DATA_SIZE     (sizeof(TEST_DATA) - 1)
#define TEST_READ_DATA     "############################################"

#define ATTEST_TOKEN_MAX_SIZE  0x200
#define TOKEN_TEST_NONCE_BYTES \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define TOKEN_TEST_VALUE_NONCE \
  (struct q_useful_buf_c) {\
    (uint8_t[]){TOKEN_TEST_NONCE_BYTES},\
    64\
  }

#define TOKEN_OPT_NORMAL_CIRCUIT_SIGN 0x0U

/**
  * @}
  */

/** @defgroup  TFM_App_Private_Functions Private Functions
  * @{
  */
static void tfm_app_print_menu(void);
static void tfm_sst_set_uid(struct test_result_t *ret);
static void tfm_sst_remove_uid(struct test_result_t *ret);
static void tfm_sst_read_uid(struct test_result_t *ret);
static void tfm_its_set_uid(struct test_result_t *ret);
static void tfm_its_remove_uid(struct test_result_t *ret);
static void tfm_its_read_uid(struct test_result_t *ret);
void dump_eat_token(struct q_useful_buf_c *token);
static void tfm_eat_test_circuit_sig(uint32_t encode_options, struct test_result_t *ret);
static  enum psa_attest_err_t token_main_alt(uint32_t option_flags,
                                             struct q_useful_buf_c nonce,
                                             struct q_useful_buf buffer,
                                             struct q_useful_buf_c *completed_token);

/**
  * @}
  */

/** @defgroup  TFM_App_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  Display the TFM App TEST Main Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
void tfm_app_menu(void)
{
  uint8_t key = 0;
  uint8_t exit = 0;
  uint8_t tests_executed;
  uint8_t tests_success;
  struct test_result_t ret;
  tfm_app_print_menu();

  while (exit == 0U)
  {
    key = 0U;

    INVOKE_SCHEDULE_NEEDS();

    /* Clean the user input path */
    COM_Flush();
    /* Receive key */
    if (COM_Receive(&key, 1U, COM_UART_TIMEOUT_MAX) == HAL_OK)
    {
      switch (key)
      {
        case '0' :
          tests_executed = 0;
          tests_success = 0;
          ret.val = TEST_FAILED;
          psa_aead_test(PSA_KEY_TYPE_AES, PSA_ALG_GCM, &ret);
          printf("AES GCM test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tests_executed++;
          tests_success += (ret.val == TEST_PASSED) ? 1 : 0;
          INVOKE_SCHEDULE_NEEDS();
          ret.val = TEST_FAILED;
          psa_cipher_test(PSA_KEY_TYPE_AES, PSA_ALG_CBC_NO_PADDING, &ret);;
          printf("AES CBC test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tests_executed++;
          tests_success += (ret.val == TEST_PASSED) ? 1 : 0;
          ret.val = TEST_FAILED;
          psa_aead_test(PSA_KEY_TYPE_AES, PSA_ALG_CCM, &ret);
          printf("AES CCM test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tests_executed++;
          tests_success += (ret.val == TEST_PASSED) ? 1 : 0;
          tfm_sst_set_uid(&ret);
          printf("SST set UID test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tests_executed++;
          tests_success += (ret.val == TEST_PASSED) ? 1 : 0;
          tfm_sst_read_uid(&ret);
          printf("SST read / check UID test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tests_executed++;
          tests_success += (ret.val == TEST_PASSED) ? 1 : 0;
          tfm_sst_remove_uid(&ret);
          printf("SST remove UID test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tests_executed++;
          tests_success += (ret.val == TEST_PASSED) ? 1 : 0;
          ret.val = TEST_FAILED;
          tfm_eat_test_circuit_sig(TOKEN_OPT_NORMAL_CIRCUIT_SIGN, &ret);
          printf("EAT normal circuit sig test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tests_executed++;
          tests_success += (ret.val == TEST_PASSED) ? 1 : 0;
          tfm_its_set_uid(&ret);
          printf("ITS set UID test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tests_executed++;
          tests_success += (ret.val == TEST_PASSED) ? 1 : 0;
          tfm_its_read_uid(&ret);
          printf("ITS read / check UID test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tests_executed++;
          tests_success += (ret.val == TEST_PASSED) ? 1 : 0;
          tfm_its_remove_uid(&ret);
          printf("ITS remove UID test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tests_executed++;
          tests_success += (ret.val == TEST_PASSED) ? 1 : 0;
          ret.val = TEST_FAILED;
          psa_hash_test(PSA_ALG_SHA_224, &ret);
          printf("SHA224 test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tests_executed++;
          tests_success += (ret.val == TEST_PASSED) ? 1 : 0;
          ret.val = TEST_FAILED;
          psa_hash_test(PSA_ALG_SHA_256, &ret);
          printf("SHA256 test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tests_executed++;
          tests_success += (ret.val == TEST_PASSED) ? 1 : 0;

          printf("CUMULATIVE RESULT: %d/%d success\r\n", tests_success, tests_executed);
          tfm_app_print_menu();
          break;

        /* 1 = Tests AES-GCM Static key */
        case '1' :
          ret.val = TEST_FAILED;
          psa_aead_test(PSA_KEY_TYPE_AES, PSA_ALG_GCM, &ret);
          printf("AES GCM test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tfm_app_print_menu();
          break;
        /* 2 = Tests AES-CBC Static key */
        case '2' :
          ret.val = TEST_FAILED;
          psa_cipher_test(PSA_KEY_TYPE_AES, PSA_ALG_CBC_NO_PADDING, &ret);;
          printf("AES CBC test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tfm_app_print_menu();
          break;
         /* 3 = Tests AES-CCM Static key */
        case '3' :
          ret.val = TEST_FAILED;
          psa_aead_test(PSA_KEY_TYPE_AES, PSA_ALG_CCM, &ret);
          printf("AES CCM test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tfm_app_print_menu();
          break;
        case '4' :
          ret.val = TEST_FAILED;
          tfm_sst_set_uid(&ret);
          printf("SST set UID test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tfm_app_print_menu();
          break;
        case '5' :
          ret.val = TEST_FAILED;
          tfm_sst_read_uid(&ret);
          printf("SST read / check UID test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tfm_app_print_menu();
          break;
        case '6' :
          ret.val = TEST_FAILED;
          tfm_sst_remove_uid(&ret);
          printf("SST remove UID test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tfm_app_print_menu();
          break;
        case '7' :
          ret.val = TEST_FAILED;
          tfm_eat_test_circuit_sig(TOKEN_OPT_NORMAL_CIRCUIT_SIGN, &ret);
          printf("EAT normal circuit sig test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tfm_app_print_menu();
          break;
        case '8' :
          ret.val = TEST_FAILED;
          tfm_its_set_uid(&ret);
          printf("ITS set UID test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tfm_app_print_menu();
          break;
        case '9' :
          ret.val = TEST_FAILED;
          tfm_its_read_uid(&ret);
          printf("ITS read / check UID test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tfm_app_print_menu();
          break;
        case 'a' :
          ret.val = TEST_FAILED;
          tfm_its_remove_uid(&ret);
          printf("ITS remove UID test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tfm_app_print_menu();
          break;
        case 'b' :
          ret.val = TEST_FAILED;
          psa_hash_test(PSA_ALG_SHA_224, &ret);
          printf("SHA224 test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tfm_app_print_menu();
          break;
        case 'c' :
          ret.val = TEST_FAILED;
          psa_hash_test(PSA_ALG_SHA_256, &ret);
          printf("SHA256 test %s\r\n", (ret.val == TEST_PASSED) ? "SUCCESSFUL" : "FAILED");
          tfm_app_print_menu();
          break;

        case 'x':
          exit = 1;
          break;

        default:
          printf("Invalid Number !\r");
          tfm_app_print_menu();
          break;
      }
    }
  }
}
/**
  * @}
  */

/** @addtogroup  TFM_App_Private_Functions
  * @{
  */

/**
  * @brief  Display the TEST TFM App Menu choices on HyperTerminal
  * @param  None.
  * @retval None.
  */
static void tfm_app_print_menu(void)
{
  printf("\r\n======================= TFM Examples Menu ===========================\r\n\n");
  printf("  TFM - Test All                                   --------------------- 0\r\n\n");
  printf("  TFM - Test AES-GCM                               --------------------- 1\r\n\n");
  printf("  TFM - Test AES-CBC                               --------------------- 2\r\n\n");
  printf("  TFM - Test AES-CCM                               --------------------- 3\r\n\n");
  printf("  TFM - Test SST set UID                           --------------------- 4\r\n\n");
  printf("  TFM - Test SST read / check UID                  --------------------- 5\r\n\n");
  printf("  TFM - Test SST remove UID                        --------------------- 6\r\n\n");
  printf("  TFM - Test EAT                                   --------------------- 7\r\n\n");
  printf("  TFM - Test ITS set UID                           --------------------- 8\r\n\n");
  printf("  TFM - Test ITS read / check UID                  --------------------- 9\r\n\n");
  printf("  TFM - Test ITS remove UID                        --------------------- a\r\n\n");
  printf("  TFM - Test SHA224                                --------------------- b\r\n\n");
  printf("  TFM - Test SHA256                                --------------------- c\r\n\n");
  printf("  Exit TFM Examples Menu                           --------------------- x\r\n\n");
}
/**
  * @brief  Write in SST a TEST UID
  * @param  struct test_result_t
  * @retval None
  */
static void tfm_sst_set_uid(struct test_result_t *ret)
{
  psa_ps_status_t status;
  const psa_ps_uid_t uid = TEST_UID;
  const psa_ps_create_flags_t flags = PSA_PS_FLAG_NONE;
  const uint32_t write_len = TEST_DATA_SIZE;
  const uint8_t write_data[] = TEST_DATA;
  /* Set a UIDtime */
  status = psa_ps_set(uid, write_len, write_data, flags);
  ret->val = status == PSA_SUCCESS ? TEST_PASSED : TEST_FAILED;
  return;
}

/**
  * @brief  Remove in SST a TEST UID
  * @param  struct test_result_t
  * @retval None
  */
static void tfm_sst_remove_uid(struct test_result_t *ret)
{
  psa_ps_status_t status;
  const psa_ps_uid_t uid = TEST_UID;
  /* remove UID */
  status = psa_ps_remove(uid);
  ret->val = status == PSA_SUCCESS ? TEST_PASSED : TEST_FAILED;
  return ;
}

/**
  * @brief  Read in SST a TEST UID and compare with expected value
  * @param  struct test_result_t
  * @retval None
  */
static void tfm_sst_read_uid(struct test_result_t *ret)
{
  psa_ps_status_t status;
  const psa_ps_uid_t uid = TEST_UID;
  uint8_t read_data[] = TEST_READ_DATA ;
  uint8_t expected_data[] = TEST_DATA;
  /* read UID */
  status = psa_ps_get(uid, 0, TEST_DATA_SIZE, read_data);
  if ((status == PSA_SUCCESS) && (!memcmp(read_data, expected_data, TEST_DATA_SIZE)))
  {
    ret->val = TEST_PASSED;
  }
  else
  {
    ret->val = TEST_FAILED;
  }
  return;
}

/**
  * @brief  Write in ITS a TEST UID
  * @param  struct test_result_t
  * @retval None
  */
static void tfm_its_set_uid(struct test_result_t *ret)
{
  psa_ps_status_t status;
  const psa_ps_uid_t uid = TEST_UID;
  const psa_ps_create_flags_t flags = PSA_PS_FLAG_NONE;
  const uint32_t write_len = TEST_DATA_SIZE;
  const uint8_t write_data[] = TEST_DATA;
  /* Set a UIDtime */
  status = psa_its_set(uid, write_len, write_data, flags);
  ret->val = status == PSA_SUCCESS ? TEST_PASSED : TEST_FAILED;
  return;
}

/**
  * @brief  Remove in ITS a TEST UID
  * @param  struct test_result_t
  * @retval None
  */
static void tfm_its_remove_uid(struct test_result_t *ret)
{
  psa_ps_status_t status;
  const psa_ps_uid_t uid = TEST_UID;
  /* remove UID */
  status = psa_its_remove(uid);
  ret->val = status == PSA_SUCCESS ? TEST_PASSED : TEST_FAILED;
  return ;
}

/**
  * @brief  Read in ITS a TEST UID and compare with expected value
  * @param  struct test_result_t
  * @retval None
  */
static void tfm_its_read_uid(struct test_result_t *ret)
{
  psa_ps_status_t status;
  const psa_ps_uid_t uid = TEST_UID;
  size_t data_len;
  uint8_t read_data[] = TEST_READ_DATA ;
  uint8_t expected_data[] = TEST_DATA;
  /* read UID */
  status = psa_its_get(uid, 0, TEST_DATA_SIZE, read_data, &data_len);
  if ((status == PSA_SUCCESS) && (data_len == TEST_DATA_SIZE)
      && (!memcmp(read_data, expected_data, TEST_DATA_SIZE)))
  {
    ret->val = TEST_PASSED;
  }
  else
  {
    ret->val = TEST_FAILED;
  }
  return;
}
void dump_eat_token(struct q_useful_buf_c *token)
{
  int32_t len = token->len;
  int32_t n_item_per_line;
  int32_t i, index = 0;
  uint8_t *byte = (uint8_t *)token->ptr;
  while (index < len)
  {
    n_item_per_line = (len-index) >= 20 ? 20 : (len-index);
    for (i = 0; i < n_item_per_line; i++)
    {
      printf("%2.2x", byte[index + i]);
    }
    printf("\r\n");
    index += n_item_per_line;

  }
  printf("\r\n");

}
/**
  * @brief  request eat short cicuit, check result and
  * display response result buffer.
  * @param  struct test_result_t
  * @retval None
  */
static void tfm_eat_test_circuit_sig(uint32_t encode_options, struct test_result_t *ret)
{
  enum psa_attest_err_t status;
  Q_USEFUL_BUF_MAKE_STACK_UB(token_storage, ATTEST_TOKEN_MAX_SIZE);
  struct q_useful_buf_c completed_token;
  struct q_useful_buf_c tmp;

  /* -- Make a token with all the claims -- */
  tmp = TOKEN_TEST_VALUE_NONCE;
  printf("token request value :\r\n");
  dump_eat_token(&tmp);
  status = token_main_alt(encode_options,
                          tmp,
                          token_storage,
                          &completed_token);
  if (status == PSA_ATTEST_ERR_SUCCESS)
  {
    ret->val = TEST_PASSED;
    printf("token response value :\r\n");
    dump_eat_token(&completed_token);
  }
  else
  {
    printf("failed status %d\r\n", status);
    ret->val = TEST_FAILED;
  }

}
/**
  * \brief An alternate token_main() that packs the option flags into the nonce.
  *
  * \param[in] option_flags      Flag bits to pack into nonce.
  * \param[in] nonce             Pointer and length of the nonce.
  * \param[in] buffer            Pointer and length of buffer to
  *                              output the token into.
  * \param[out] completed_token  Place to put pointer and length
  *                              of completed token.
  *
  * \return various errors. See \ref attest_token_err_t.
  *
  */
static enum psa_attest_err_t token_main_alt(uint32_t option_flags,
                                            struct q_useful_buf_c nonce,
                                            struct q_useful_buf buffer,
                                            struct q_useful_buf_c *completed_token)
{
  enum psa_attest_err_t return_value;
  uint32_t completed_token_len;
  struct q_useful_buf_c        actual_nonce;
  Q_USEFUL_BUF_MAKE_STACK_UB(actual_nonce_storage, 64);

  if (nonce.len == 64 && q_useful_buf_is_value(nonce, 0))
  {
    /* Go into special option-packed nonce mode */
    actual_nonce = q_useful_buf_copy(actual_nonce_storage, nonce);
    /* Use memcpy as it always works and avoids type punning */
    memcpy((uint8_t *)actual_nonce_storage.ptr,
           &option_flags,
           sizeof(uint32_t));
  }
  else
  {
    actual_nonce = nonce;
  }

  completed_token_len = (uint32_t)buffer.len;
  return_value = psa_initial_attest_get_token(actual_nonce.ptr,
                                              (uint32_t)actual_nonce.len,
                                              buffer.ptr,
                                              &completed_token_len);

  *completed_token = (struct q_useful_buf_c)
  {
    buffer.ptr, completed_token_len
  };

  return return_value;
}

/**
  * @}
  */
