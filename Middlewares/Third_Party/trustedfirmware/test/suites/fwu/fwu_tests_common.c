/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#if DOMAIN_NS == 1
#include <string.h>
#else
#include "tfm_memory_utils.h"
#endif
#include "fwu_tests_common.h"
#include "flash_layout.h"

#if (MCUBOOT_APP_IMAGE_NUMBER == 2)
#define image_type_test FWU_IMAGE_TYPE_SECURE
#else
#define image_type_test FWU_IMAGE_TYPE_FULL
#endif

static psa_image_id_t image_id = \
                (psa_image_id_t)FWU_CALCULATE_IMAGE_ID(FWU_IMAGE_ID_SLOT_STAGE,
                                                       image_type_test,
                                                       0);
static const uint8_t data_block[32] = \
                        { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, \
                          0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x11, 0x12, \
                          0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, \
                          0x1b, 0x1c, 0x1d,  0x1e, 0x1f, 0x20, 0x21, 0x22 };

void tfm_fwu_test_common_001(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_id_t dependency_uuid;
    psa_image_version_t dependency_version;
    psa_image_info_t info;

    /* Write the data block with block_offset 0. */
    status = psa_fwu_write(image_id, 0, data_block, sizeof(data_block));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Write should not fail with block_offset 0");
        return;
    }

    /* Query the staging image. */
    status = psa_fwu_query(image_id, &info);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    /* Check the image state. */
    if (info.state != PSA_IMAGE_CANDIDATE) {
        TEST_FAIL("Image should be in CANDIDATE state after succesfull write");
        return;
    }

    /* Write the second data block with offset 32. */
    status = psa_fwu_write(image_id,
                           sizeof(data_block),
                           data_block,
                           sizeof(data_block));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Write should not fail with block_offset 32");
        return;
    }

    status = psa_fwu_install(image_id, &dependency_uuid, &dependency_version);

    /* In the currently inplementation, image verification is deferred to
     * reboot, so PSA_SUCCESS_REBOOT is returned when success.
     */
    if ((status != PSA_SUCCESS_REBOOT) &&
        (status != PSA_ERROR_DEPENDENCY_NEEDED)) {
        TEST_FAIL("Install should not fail after write");
        return;
    }

    /* Query the staging image. */
    if (psa_fwu_query(image_id, &info) != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    /* Check the image state. */
    if ((status == PSA_SUCCESS_REBOOT) &&
       (info.state != PSA_IMAGE_REBOOT_NEEDED)) {
        TEST_FAIL("Image should be in REBOOT_NEEDED state if PSA_SUCCESS_REBOOT is retuned in install");
        return;
    }

    /* Abort the fwu process. */
    status = psa_fwu_abort(image_id);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Abort should not fail after install");
        return;
    }

    /* Query the staging image. */
    status = psa_fwu_query(image_id, &info);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    /* Check the image state. */
    if (info.state != PSA_IMAGE_UNDEFINED) {
        TEST_FAIL("Image should be in UNDEFINED state after abort");
        return;
    }

    /* Write the data block with block_offset 0 again after abort. */
    status = psa_fwu_write(image_id, 0, data_block, sizeof(data_block));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Write should not fail with block_offset 0 after abort");
        return;
    }

    /* Query the staging image. */
    status = psa_fwu_query(image_id, &info);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    /* Check the image state. */
    if (info.state != PSA_IMAGE_CANDIDATE) {
        TEST_FAIL("Image should be in CANDIDATE state after succesfull write");
        return;
    }

    /* Abort the fwu process. */
    status = psa_fwu_abort(image_id);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Abort should not fail after write");
        return;
    }

    ret->val = TEST_PASSED;
}

#if defined TFM_FWU_TEST_WRITE_WITH_NULL
/* The TF-M core will reboot the device if the parameter check fails
 * in partition request in libaray mode.
 */
void tfm_fwu_test_common_002(struct test_result_t *ret)
{
    psa_status_t status;

    /* Write the data block NULL. */
    status = psa_fwu_write(image_id, 0, NULL, sizeof(data_block));
    if (status != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Write should fail with data block NULL");
        return;
    }

    ret->val = TEST_PASSED;
}
#endif

void tfm_fwu_test_common_003(struct test_result_t *ret)
{
    psa_status_t status;

    /* Write the data block with block_offset + len overflow. */
    status = psa_fwu_write(image_id, (size_t)0xFFFFFFFF, data_block, 0x10);
    if (status == PSA_SUCCESS) {
        TEST_FAIL("Write should fail with len + image_offset overflow");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_004(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_version_t dependency_version;

    /* Write the data block with block_offset 0. */
    status = psa_fwu_write(image_id, 0, data_block, sizeof(data_block));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Write should not fail with block_offset 0");
        return;
    }

    status = psa_fwu_install(image_id, NULL, &dependency_version);

    /* If PSA_ERROR_DEPENDENCY_NEEDED is returned, the dependency should be
     * filled in the dependency argument.
     */
    if (status == PSA_ERROR_DEPENDENCY_NEEDED) {
        TEST_FAIL("Install should not return PSA_ERROR_DEPENDENCY_NEEDED with NULL dependency");
        return;
    }

    /* Abort the fwu process. */
    status = psa_fwu_abort(image_id);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Abort should not fail after install");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_005(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_id_t dependency_uuid;

    /* Write the data block with block_offset 0. */
    status = psa_fwu_write(image_id, 0, data_block, sizeof(data_block));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Write should not fail with block_offset 0");
        return;
    }

    status = psa_fwu_install(image_id, &dependency_uuid, NULL);

    /* If PSA_ERROR_DEPENDENCY_NEEDED is returned, the dependency should be
     * filled in the dependency argument.
     */
    if (status == PSA_ERROR_DEPENDENCY_NEEDED) {
        TEST_FAIL("Install should not return PSA_ERROR_DEPENDENCY_NEEDED with NULL dependency version");
        return;
    }

    /* Abort the fwu process. */
    status = psa_fwu_abort(image_id);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Abort should not fail after install");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_006(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_id_t dependency_uuid;
    psa_image_version_t dependency_version;

    status = psa_fwu_install(image_id, &dependency_uuid, &dependency_version);

    /* Install will always fail if write is not called before. */
    if (status != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Install should not fail after write");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_007(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_id_t dependency_uuid;
    psa_image_version_t dependency_version;

    /* Write the data block with block_offset 0. */
    status = psa_fwu_write(image_id, 0, data_block, sizeof(data_block));
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Write should not fail with block_offset 0");
        return;
    }

    /* Abort the fwu process. */
    status = psa_fwu_abort(image_id);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Abort should not fail after install");
        return;
    }

    status = psa_fwu_install(image_id, &dependency_uuid, &dependency_version);

    /* Install should fail as the fwu process has been aborted. */
    if (status != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Install should fail after abort");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_008(struct test_result_t *ret)
{
    psa_status_t status;

    status = psa_fwu_abort(image_id);

    /* Install will always fail if write is not called before. */
    if (status != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Install should fail with no write before");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_009(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_info_t info = { 0 };
    psa_image_id_t image_id = FWU_CALCULATE_IMAGE_ID(FWU_IMAGE_ID_SLOT_ACTIVE,
                                                     image_type_test,
                                                     0);

    /* Query the running image. */
    status = psa_fwu_query(image_id, &info);
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Query should success");
        return;
    }

    if ((info.state != PSA_IMAGE_INSTALLED) &&
       (info.state != PSA_IMAGE_PENDING_INSTALL)) {
        TEST_FAIL("The active image should be in INSTALLED or PENDING_INSTALL state");
        return;
    }
    ret->val = TEST_PASSED;
}

#if defined TFM_FWU_TEST_QUERY_WITH_NULL
void tfm_fwu_test_common_010(struct test_result_t *ret)
{
    psa_status_t status;

    /* Query the running image. */
    status = psa_fwu_query(image_id, NULL);
    if (status == PSA_SUCCESS) {
        TEST_FAIL("Query should fail with NULL info");
        return;
    }

    ret->val = TEST_PASSED;
}
#endif

void tfm_fwu_test_common_011(struct test_result_t *ret)
{
    psa_status_t status;
    psa_image_info_t info = { 0 };

    psa_image_id_t image_id = FWU_CALCULATE_IMAGE_ID(3,
                                                     image_type_test,
                                                     0);

    /* Query the running image. */
    status = psa_fwu_query(image_id, &info);
    if (status != PSA_ERROR_INVALID_ARGUMENT) {
        TEST_FAIL("Query should fail with NULL info");
        return;
    }

    ret->val = TEST_PASSED;
}

void tfm_fwu_test_common_012(struct test_result_t *ret)
{
    psa_status_t status;

    /* Accept the running image. */
    status = psa_fwu_accept();
    if (status != PSA_SUCCESS) {
        TEST_FAIL("Accept should not fail");
        return;
    }

    ret->val = TEST_PASSED;
}

#ifdef TFM_FWU_TEST_REQUEST_REBOOT
void tfm_fwu_test_common_013(struct test_result_t *ret)
{
    /* Request reboot. */
    psa_fwu_request_reboot();
    TEST_FAIL("Request reboot should not fail");

    ret->val = TEST_FAILED;
}
#endif
